#include "BossSpawner.h"
#include "LevelSetKeeper.h"
#include "GameObjectManager.h"
#include <cassert>

KochaEngine::BossSpawner* KochaEngine::BossSpawner::instance = nullptr;

KochaEngine::BossSpawner::BossSpawner()
	:oManager(nullptr)
{
}

KochaEngine::BossSpawner::~BossSpawner()
{
	oManager = nullptr;
}

KochaEngine::BossSpawner* KochaEngine::BossSpawner::GetInstance()
{
	if (!instance)
	{
		instance = new BossSpawner();
	}
	return instance;
}

void KochaEngine::BossSpawner::DeleteInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void KochaEngine::BossSpawner::SpawnBoss()
{
	if (!oManager)
	{
		assert(0);
	}
	if (oManager->GetTypeCount(GameObjectType::JAMMING_BOSS) != 0)
	{
		return;
	}


}

void KochaEngine::BossSpawner::SetGameObjectManager(GameObjectManager* arg_oManager)
{
	oManager = arg_oManager;
}
