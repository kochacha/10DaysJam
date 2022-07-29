#include "BossSpawner.h"
#include "LevelSetKeeper.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "ParticleEmitter.h"
#include "ScoreManager.h"
#include "ItemManager.h"
#include "Wall.h"
#include "Util.h"
#include <cassert>

KochaEngine::BossSpawner* KochaEngine::BossSpawner::instance = nullptr;

KochaEngine::BossSpawner::BossSpawner()
	:camera(nullptr),
	 oManager(nullptr),
	 pEmitter(nullptr),
	 sManager(nullptr),
	 iManager(nullptr)
{
}

KochaEngine::BossSpawner::~BossSpawner()
{
	camera = nullptr;
	oManager = nullptr;
	pEmitter = nullptr;
	sManager = nullptr;
	iManager = nullptr;
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

	Wall* wall = oManager->GetWall();
	float bossPosY = KochaEngine::Util::GetIntRand(10, 50) - 20;
	static GameMode mode = GameMode::SCOREATTAKMODE;
	oManager->AddObject(new JammingBoss(camera, oManager, pEmitter, sManager, Vector3(wall->GetPosition().x, bossPosY, 0), iManager, &mode));
}

void KochaEngine::BossSpawner::SetClass(Camera* arg_camera, GameObjectManager* arg_oManager, ParticleEmitter* arg_pEmitter, ScoreManager* arg_sManager, ItemManager* arg_iManager)
{
	camera = arg_camera;
	oManager = arg_oManager;
	pEmitter = arg_pEmitter;
	sManager = arg_sManager;
	iManager = arg_iManager;
}
