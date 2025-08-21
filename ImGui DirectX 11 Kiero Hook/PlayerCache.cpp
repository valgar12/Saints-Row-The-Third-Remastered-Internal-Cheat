#include "PlayerCache.h"
class Player;

void PlayerCache::PlayerCache()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	GameInstance* gameInstance = (GameInstance*)(mem::GameBase + of::World);

	while (true)
	{
		gameInstance = reinterpret_cast<GameInstance*>(mem::GameBase + of::World);
		if (gameInstance && gameInstance->World && gameInstance->World->ObjectArray)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	World* world = gameInstance->World;

	while (alive)
	{
		PlayerList.clear();
		VehicleList.clear();

		if (gameInstance == nullptr || gameInstance->World == nullptr
			|| gameInstance->World->ObjectArray == nullptr)
			continue;

		auto objectArray = reinterpret_cast<ctg_object**>(world->ObjectArray);

		if (!objectArray) continue;

		for (int i = 0; i < world->ArrayNum; i++)
		{
			ctg_object* object = objectArray[i];
			if (!object) continue;
			if (!object->objectType) continue;
			
			auto t = object->objectType;
			if (t == ObjectType::human)
			{
				localplayer = (Player*)object;
			}
			if (t == ObjectType::vehicle)
				VehicleList.push_back(object);
			if (t == ObjectType::vehicle || t == ObjectType::npc ||
				t == ObjectType::item || t == ObjectType::weapon || t == ObjectType::human)
			{
				PlayerList.push_back(object);
			}

			
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}