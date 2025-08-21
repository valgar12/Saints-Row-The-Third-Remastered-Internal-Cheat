#include "GameLoop.h"

Mem memory{};

void GameLoop()
{
	if (gl::Exploits::InfiniteAmmo && !memory.isEntryPatched(0))
	{
		memory.Nop((void*)(mem::GameBase + of::InfiniteAmmo), 4, 0);
		printf("Noped ammo at 0x%p\n", (void*)(mem::GameBase + of::InfiniteAmmo));
	}
	else if (!gl::Exploits::InfiniteAmmo && memory.isEntryPatched(0))
	{
		memory.RestoreByID(0);
		printf("Ammo restored\n");
	}

	if (gl::Exploits::NoSpread && !memory.isEntryPatched(1))
	{
		memory.Nop((void*)(mem::GameBase + of::NoSpread), 9, 1);
		printf("Noped spread at 0x%p\n", (void*)(mem::GameBase + of::NoSpread));
	}
	else if (!gl::Exploits::NoSpread && memory.isEntryPatched(1))
	{
		memory.RestoreByID(1);
		printf("Spread restored\n");
	}

	if (gl::Exploits::NoRecoil && !memory.isEntryPatched(2))
	{
		memory.Nop((void*)(mem::GameBase + of::NoRecoil), 5, 2);
		printf("Noped recoil at 0x%p\n", (void*)(mem::GameBase + of::NoRecoil));
	}
	else if (!gl::Exploits::NoRecoil && memory.isEntryPatched(2))
	{
		memory.RestoreByID(2);
		printf("Recoil restored\n");
	}

	if (localplayer != nullptr)
	{
		if (gl::Exploits::GodMode)
		{
			localplayer->DamageMultiplier = 0;
		}
		else
		{
			localplayer->DamageMultiplier = 1;
		}

		if (gl::Exploits::InfiniteMoney)
		{
			localplayer->money = 99999999999;
		}

		if (gl::Exploits::SuperWeapon)
		{
			localplayer->AttackMultiplier = 500;
		}
		else
		{
			localplayer->AttackMultiplier = 1;
		}

		if (gl::Exploits::NoFire)
			localplayer->ApplyFlags(1, pFlags::Fire);
		else
			localplayer->ApplyFlags(0, pFlags::Fire);

		if (gl::Exploits::NoRagdoll)
			localplayer->ApplyFlags(1, pFlags::Ragdoll);
		else
			localplayer->ApplyFlags(0, pFlags::Ragdoll);

		if (gl::Exploits::NoExplosions)
			localplayer->ApplyFlags(1, pFlags::Explosions);
		else
			localplayer->ApplyFlags(0, pFlags::Explosions);

		if (gl::Exploits::PerfectAim)
			localplayer->ApplyFlags(1, pFlags::Aim);
		else
			localplayer->ApplyFlags(0, pFlags::Aim);

		if (gl::Exploits::NoFlinch)
			localplayer->ApplyFlags(1, pFlags::FLinch);
		else
			localplayer->ApplyFlags(0, pFlags::FLinch);

		if (gl::Exploits::NoBump)
			localplayer->ApplyFlags(1, pFlags::Bump);
		else
			localplayer->ApplyFlags(0, pFlags::Bump);
	}

	Vec2 bestPosition{};
	float bestDistance = 99999.f;

	if (PlayerList.size() > 0)
	{
		for (int i = 0; i < PlayerList.size(); i++)
		{
			if (i >= PlayerList.size()) break;

			ctg_object* obj = PlayerList[i];

			if (!obj || !obj->objectType) continue;


			if (!localplayer || !localplayer->Controller
				|| !localplayer->Controller->characterShapePhantom) continue;

			Vec3 localPos = localplayer->Controller->characterShapePhantom->position;

			Vec3* WayPointPtr = *(Vec3**)(mem::GameBase + of::WayPoint);

			if (!WayPointPtr) continue;

			if (GetAsyncKeyState(VK_F1) & 1)
			{
				printf("[LocalPlayer] Teleported from X: %f Y: %f Z: %f to X: %f Y: %f Z: %f\n",
					localPos.x, localPos.y, localPos.z, WayPointPtr->x, WayPointPtr->y, WayPointPtr->z);
				localplayer->Controller->characterShapePhantom->position = *WayPointPtr;
			}

			if (gl::ESP::ESP_Objects)
			{
				auto t = obj->objectType;
				if (t == ObjectType::human && !gl::World::Humans) continue;
				if (t == ObjectType::weapon && !gl::World::Weapons) continue;
				if (t == ObjectType::item && !gl::World::Items) continue;
				if (t == ObjectType::vehicle && !gl::World::Vehicles) continue;

				Vec4 obj3D { obj->position.x, obj->position.y, obj->position.z, 0.f};

				std::string ObjName = "Object";
				ImColor DrawColor = Colors::Green;
				NPC* npc = (NPC*)(obj);

				switch (obj->objectType)
				{
				case 34:
					if (npc->team == 0)
						ObjName = "Team";
					else
						ObjName = "NPC";
					DrawColor = Colors::Red;
					break;
				case 33:
					ObjName = "Human";
					DrawColor = Colors::Yellow;
					break;
				case 31:
					ObjName = "Weapon";
					DrawColor = Colors::Cian;
					break;
				case 7:
					ObjName = "Vehicle";
					DrawColor = Colors::Green;
					break;
				case 30:
					ObjName = "Item";
					DrawColor = Colors::Purple;
					break;
				default:
					break;
				}

				float distance = localPos.DistanceTo(obj->position);

				if (distance > gl::ESP::RenderDistance) continue;

				Vec4 w2sOnj{};
				if (WorldToScreen(reinterpret_cast<__m128*>(&w2sOnj), reinterpret_cast<__m128*>(&obj3D)))
				{
					if (t != ObjectType::npc || (t == ObjectType::npc && gl::World::Npcs))

					ESP::DrawText2(Vec2(w2sOnj.x, w2sOnj.y), DrawColor, ObjName.c_str());
				}
			}

			
			if (obj->objectType != ObjectType::npc) continue;

			auto npc = reinterpret_cast<NPC*>(obj);

			if (!npc->health || npc->health <= 0) continue;

			if (!npc->Controller || !npc->Controller->characterShapePhantom) continue;

			auto pos = npc->Controller->characterShapePhantom->position;

			float distance = localPos.DistanceTo(obj->position);

			auto team = npc->team;
			if (team != 0 && team != 7)
			{
				if (npc->isInVehicle())
					NpcColor = Colors::DarkBlue;
				else
					NpcColor = Colors::Red;
			}
			else
				if (npc->isInVehicle())
					NpcColor = Colors::Yellow;
				else
				{
					if (team == 0)
						NpcColor = Colors::Green;
					else
						NpcColor = gl::esp_Colors::BoxColor;
				}

			if (!WorldToScreen) continue;

			if (!gl::ESP::ShowCivilians)
			{
				if (team == 7) continue;
			}
			if (!gl::ESP::ShowTeammates)
			{
				if (team == 0) continue;
			}

			if (GetAsyncKeyState(VK_F3))
			{
				npc->Controller->characterShapePhantom->position = localPos;
			}

			if (distance > gl::ESP::RenderDistance) continue;

			float health = npc->health;
			auto maxHealth = npc->maxHealth;

			/*if (gl::ESP::ESP_3DBox)
			{
				ESP::Draw3DBoxFromCenterExtentsRelative(pos, entry.center, entry.extents, gl::esp_Colors::Box3DColor);
			}*/

			Vec4 entPosFeet{ pos.x, pos.y, pos.z, 0.f};

			alignas(16) __m128 headOut = _mm_setzero_ps();
			alignas(16) __m128 matOut[3];
			auto playerInt = static_cast<int64_t>(reinterpret_cast<std::intptr_t>(npc));

			__m128* ret = GetHeadPos(playerInt, &headOut, matOut);

			Vec4 entHeadPos{ headOut.m128_f32[0], headOut.m128_f32[1], headOut.m128_f32[2], 0.f};
			Vec4 Bottom{}, Top{};

			if (WorldToScreen(reinterpret_cast<__m128*>(&Bottom), reinterpret_cast<__m128*>(&entPosFeet))
				&& WorldToScreen(reinterpret_cast<__m128*>(&Top), reinterpret_cast<__m128*>(&entHeadPos)))
			{
				const float h = std::abs(Top.y - Bottom.y);
				const float w = h * 0.2f;

				if (gl::ESP::ESP)
				{
					if (localplayer->isInVehicle())
					{
						auto vehicle = localplayer->GetVehicle();

						if (vehicle != nullptr)
						{
							Vec4 Veh3D{ vehicle->position.x, vehicle->position.y, vehicle->position.z, 0.f};
							Vec4 veh2D{};

							if (WorldToScreen(reinterpret_cast<__m128*>(&veh2D), reinterpret_cast<__m128*>(&Veh3D)))
								ESP::DrawText2(Vec2(veh2D.x, veh2D.y), Colors::Pink, "MY VEHICLE");

							
						}
					}

					if (gl::ESP::ESP_health)
					{
						ESP::DrawHealthBarLeft(Vec2(Top.x, Top.y), Vec2(Bottom.x, Bottom.y), maxHealth, health, w);
					}

					if (gl::ESP::ESP_Box)
					{
						ESP::DrawBox({ Top.x - w, Top.y }, { Bottom.x + w, Bottom.y }, NpcColor);
					}
					if (gl::ESP::ESP_CorneredBox)
					{
						ESP::DrawCorneredBox({ Top.x, Top.y }, { Bottom.x, Bottom.y }, w, h, NpcColor);
					}
					if (gl::ESP::ESP_Snaplines)
					{
						ESP::DrawLine(Vec2(*GameResX / 2, 0), Vec2(Top.x, Top.y), gl::esp_Colors::SnaplineColor);
					}
				}

				if (gl::Aimbot::Aimbot)
				{
					if (!gl::Aimbot::AttackCivilians)
					{
						if (team == 0 || team == 7) continue;
					}

					if (gl::Aimbot::ShowFov)
						ESP::DrawCircle({ (float)*GameResX / 2, (float)*GameResY / 2 }, gl::Aimbot::Fov, gl::esp_Colors::FovColor, 60);

					Vec2 headPos{ Top.x, Top.y };

					float distance = headPos.DistanceTo({ (float)*GameResX / 2, (float)*GameResY / 2 });

					if (distance > gl::Aimbot::Fov) continue;

					if (distance < bestDistance)
					{
						bestDistance = distance;
						bestPosition = headPos;
					}
				}
			}
		}

		if (GetAsyncKeyState(VK_XBUTTON2) && gl::Aimbot::Aimbot && bestPosition.x != 0.f)
		{
			/*float NewYaw = LocalCamera->CameraX;   
			float NewPitch = LocalCamera->CameraY;
			if (Aimbot(LocalCamera->position, NewYaw, NewPitch, bestPosition))
			{
				LocalCamera->CameraX = NewYaw;
				LocalCamera->CameraY = NewPitch;
			}*/
			mousemove(bestPosition.x, bestPosition.y, gl::Aimbot::Smooth);
		}
	}
}