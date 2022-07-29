#pragma once
#include "JammingBoss.h"

class GameObjectManager;

namespace KochaEngine
{
	class BossSpawner
	{
	public:
		static BossSpawner* GetInstance();
		static void DeleteInstance();

		void SpawnBoss();
		void SetGameObjectManager(GameObjectManager* arg_oManager);

	private:
		static BossSpawner* instance;
		BossSpawner();
		~BossSpawner();
		BossSpawner(const BossSpawner& another) = delete;
		BossSpawner& operator=(const BossSpawner& another) = delete;

		GameObjectManager* oManager;
	};
}