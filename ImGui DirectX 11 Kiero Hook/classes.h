#pragma once
#include "includes.h"

enum pFlags
{
	Aim, Ragdoll, Explosions, Bump, FLinch, Fire
};

enum ObjectType
{
	vehicle = 7,
	rig = 26,
	item = 30,
	weapon = 31,
	human = 33,
	npc = 34
};

class ctg_object
{
public:
	char pad_0000[76]; //0x0000
	int32_t objectType; //0x004C
	char pad_0050[16]; //0x0050
	Vec3 position; //0x0060
	char pad_006C[44]; //0x006C
	int32_t ObjectIndex; //0x0098
	char pad_009C[172]; //0x009C
}; //Size: 0x0148

class Mesh
{
public:
	char pad_0000[16]; //0x0000
	class rlSkinInstance* SkinInstance; //0x0010
	char pad_0018[8]; //0x0018
	class Skin* Skin; //0x0020
	char pad_0028[992]; //0x0028
}; //Size: 0x0408

class rlSkinInstance
{
public:
	char pad_0000[160]; //0x0000
	class rlScene* Scene; //0x00A0
	char pad_00A8[864]; //0x00A8
}; //Size: 0x0408

class rlScene
{
public:
	char pad_0000[424]; //0x0000
	class rlNgenSceneRenderer* NgenSceneRenderer; //0x01A8
	char pad_01B0[600]; //0x01B0
}; //Size: 0x0408

class rlNgenSceneRenderer
{
public:
	char pad_0000[128]; //0x0000
	class rlCamera* Camera; //0x0080
	char pad_0088[896]; //0x0088
}; //Size: 0x0408

class rlCamera
{
public:
	char pad_0000[1032]; //0x0000
}; //Size: 0x0408

class Skin
{
public:
	char pad_0000[56]; //0x0000
	class Ptr1* Ptr1; //0x0038
	char pad_0040[4040]; //0x0040
}; //Size: 0x1008

class Ptr1
{
public:
	char pad_0000[812]; //0x0000
	class Skinned* Skinned; //0x032C
	char pad_0334[2272]; //0x0334
}; //Size: 0x0C14

class Skinned
{
public:
	char pad_0000[192]; //0x0000
	class MeshesAndRigs* Meshes; //0x00C0
	char pad_00C8[3904]; //0x00C8
}; //Size: 0x1008

class MeshesAndRigs
{
public:
	char pad_0000[416]; //0x0000
	class Rig* FirstRig; //0x01A0
	char pad_01A8[7776]; //0x01A8
}; //Size: 0x2008

class Rig
{
public:
	char pad_0000[32]; //0x0000
	class N00003590* bones; //0x0020
	char pad_0028[992]; //0x0028
}; //Size: 0x0408


class GameInstance
{
public:
	class World* World; //0x0000
	char pad_0008[312]; //0x0008
}; //Size: 0x0140

class World
{
public:
	char pad_0000[104]; //0x0000
	void* ObjectArray; //0x0068
	int32_t ArryCapacity; //0x0070
	int32_t ArrayNum; //0x0074
	char pad_0078[1936]; //0x0078
}; //Size: 0x0808

class Hoods
{
public:
	char pad_0000[24]; //0x0000
	char* CurrentHoodName; //0x0018
	char pad_0020[56]; //0x0020
	wchar_t* DistrictName; //0x0058
	char pad_0060[4008]; //0x0060
}; //Size: 0x1008

class CharacterController
{
public:
	char pad_0000[48]; //0x0000
	class hkCharacterShapePhantom* characterShapePhantom; //0x0030
	char pad_0038[2000]; //0x0038
}; //Size: 0x0808

class hkCharacterShapePhantom
{
public:
	char pad_0000[288]; //0x0000
	Vec3 position; //0x0120
	char pad_012C[1760]; //0x012C
}; //Size: 0x080C

class NPC
{
public:
	char pad_0000[32]; //0x0000
	class Weapon* WeaponArray; //0x0020
	char pad_0028[248]; //0x0028
	class NpcInfo* NpcInfo; //0x0120
	char pad_0128[3336]; //0x0128
	class CharacterController* Controller; //0x0E30
	char pad_0E38[96]; //0x0E38
	class Mesh* Mesh; //0x0E98
	char pad_0EA0[2548]; //0x0EA0
	int32_t VehicleFlag; //0x1894
	char pad_1898[616]; //0x1898
	class Hoods* Hoods; //0x1B00
	char pad_1B08[896]; //0x1B08
	int32_t PlayerFlags; //0x1E88
	char pad_1E8C[4]; //0x1E8C
	int32_t money; //0x1E90
	char pad_1E94[4]; //0x1E94
	int32_t team; //0x1E98
	int32_t maxHealth; //0x1E9C
	char pad_1EA0[8]; //0x1EA0
	float health; //0x1EA8
	float DamageMultiplier; //0x1EAC
	float AttackMultiplier; //0x1EB0
	char pad_1EB4[2388]; //0x1EB4


	bool isInVehicle()
	{
		return VehicleFlag == 3;
	}
}; //Size: 0x2808


class Player
{
public:
	char pad_0000[32]; //0x0000
	class Weapon* WeaponArray; //0x0020
	char pad_0028[248]; //0x0028
	class NpcInfo* NpcInfo; //0x0120
	char pad_0128[3336]; //0x0128
	class CharacterController* Controller; //0x0E30
	char pad_0E38[96]; //0x0E38
	class Mesh* Mesh; //0x0E98
	char pad_0EA0[2548]; //0x0EA0
	int32_t VehicleFlag; //0x1894
	char pad_1898[616]; //0x1898
	class Hoods* Hoods; //0x1B00
	char pad_1B08[896]; //0x1B08
	int32_t PlayerFlags; //0x1E88
	char pad_1E8C[4]; //0x1E8C
	int32_t money; //0x1E90
	char pad_1E94[4]; //0x1E94
	int32_t team; //0x1E98
	int32_t maxHealth; //0x1E9C
	char pad_1EA0[8]; //0x1EA0
	float health; //0x1EA8
	float DamageMultiplier; //0x1EAC
	float AttackMultiplier; //0x1EB0
	char pad_1EB4[2388]; //0x1EB4


	void ApplyFlags(bool option, int Flag)
	{
		switch (Flag)
		{
		case pFlags::Aim:
			PlayerFlags &= ~0x100;
			PlayerFlags |= option << 8;
			break;
		case pFlags::Ragdoll:
			PlayerFlags &= ~0x80;
			PlayerFlags |= option << 7;
			break;
		case pFlags::Bump:
			PlayerFlags &= ~0x1000;
			PlayerFlags |= option << 12;
			break;
		case pFlags::FLinch:
			PlayerFlags &= ~0x800u;
			PlayerFlags |= option << 11;
			break;
		case pFlags::Explosions:
			PlayerFlags &= ~0x10u;
			PlayerFlags |= 16 * option;
			PlayerFlags &= ~0x1000u;
			PlayerFlags |= option << 12;
			break;
		case pFlags::Fire:
			PlayerFlags &= ~0x2000u;
			PlayerFlags |= option << 13;
			PlayerFlags &= ~0x4000u;
			PlayerFlags |= option << 14;
			PlayerFlags &= ~0x10000u;
			PlayerFlags |= option << 16;
			break;
		default:
			break;
		}
	}

	bool isInVehicle()
	{
		return VehicleFlag == 3;
	}

	ctg_object* GetVehicle()
	{
		float bestDistance = 9999999.f;
		ctg_object* bestVehicle = nullptr;

		for (int i = 0; i < VehicleList.size(); i++)
		{
			if (i >= VehicleList.size()) break;
			
			ctg_object* vehicle = (ctg_object*)VehicleList[i];
			if (!vehicle) continue;

			float distance = localplayer->Controller->characterShapePhantom->position.DistanceTo(vehicle->position);

			if (distance < bestDistance)
			{
				bestDistance = distance;
				bestVehicle = vehicle;
			}
		}

		return bestVehicle;
	}
	
}; //Size: 0x2808

class Weapon
{
public:
	char pad_0000[40]; //0x0000
	class Weapon* NextWeapon; //0x0028
	class Weapon* PreviusWeapon; //0x0030
	char pad_0038[24]; //0x0038
	int32_t isEquipped; //0x0050
	char pad_0054[188]; //0x0054
	class WeaponNames* WeaponNames; //0x0110
	char pad_0118[224]; //0x0118
	class WeaponExtra* WeaponExtra; //0x01F8
	char pad_0200[4]; //0x0200
	int32_t currentAmmo; //0x0204
	char pad_0208[52]; //0x0208
	int32_t shootsFired; //0x023C
	float spread; //0x0240
	char pad_0244[380]; //0x0244
}; //Size: 0x03C0


class WeaponNames
{
public:
	char* WeaponName; //0x0000
	char pad_0008[16]; //0x0008
	char* Mesh; //0x0018
	char* Texture; //0x0020
	char* Class; //0x0028
	char pad_0030[32]; //0x0030
	wchar_t* NameW; //0x0050
}; //Size: 0x0058

class WeaponExtra
{
public:
	char* WeaponName; //0x0000
	char pad_0008[128]; //0x0008
}; //Size: 0x0088

class Camera
{
public:
	Vec3 position; //0x0000
	char pad_000C[504]; //0x000C
	float CameraX; //0x0204
	char pad_0208[28]; //0x0208
	float CameraY; //0x0224
	char pad_0228[540]; //0x0228
}; //Size: 0x0444

