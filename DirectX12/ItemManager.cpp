#include "ItemManager.h"
#include "GameObjectManager.h"
#include "EnhancementItem.h"
#include "JammingSpine.h"

KochaEngine::ItemManager::ItemManager(Camera* arg_camera, GameObjectManager* arg_gManager)
{
	if (!arg_camera) return;
	if (!arg_gManager) return;

	camera = arg_camera;
	gManager = arg_gManager;
}

KochaEngine::ItemManager::~ItemManager()
{

}

void KochaEngine::ItemManager::Initialize()
{
	gManager->AddObject(new EnhancementItem(camera, gManager, Vector3(20, 10, 0)));
	gManager->AddObject(new JammingSpine(camera, gManager, Vector3(-30, -20, 0)));
}

void KochaEngine::ItemManager::Update()
{

}