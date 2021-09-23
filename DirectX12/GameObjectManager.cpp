#include "GameObjectManager.h"
#include "Collision.h"
#include <map>

KochaEngine::GameObjectManager::GameObjectManager()
{
}

KochaEngine::GameObjectManager::~GameObjectManager()
{
	RemoveAll();
}

void KochaEngine::GameObjectManager::AddObject(GameObject* arg_gameObject)
{
	gameObjects.push_back(arg_gameObject);
}

void KochaEngine::GameObjectManager::Initialize()
{
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		(*it)->Initialize();
	}
}

void KochaEngine::GameObjectManager::Update()
{
	for (int i = 0; i != gameObjects.size(); i++)
	{
		gameObjects[i]->Update();
	}
	Remove();
}

void KochaEngine::GameObjectManager::Zsort(const int arg_count)
{
	int sortCount = 0;
	while (sortCount == arg_count)
	{

	}
}

void KochaEngine::GameObjectManager::AlphaObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	this->camera = arg_camera;
	this->lightManager = arg_lightManager;

	float cameraPosZ = camera->GetEye().z;
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if (!(*it)->IsAlphaObject()) continue; //AlphaObjectじゃない場合は描画しない
		if ((*it)->IsDead()) continue;
		if ((*it)->GetPosition().z < cameraPosZ) continue; //AlphaObjectがCameraよりも後ろにあったら描画しない

		(*it)->ObjDraw(this->camera, this->lightManager);
	}

}

void KochaEngine::GameObjectManager::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	this->camera = arg_camera;
	this->lightManager = arg_lightManager;
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsAlphaObject()) continue; //AlphaObjectの場合は描画しない
		if ((*it)->IsDead()) continue;

		(*it)->ObjDraw(this->camera, this->lightManager);
	}
}

void KochaEngine::GameObjectManager::SpriteDraw()
{
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead())
		{
			continue;
		}
		(*it)->SpriteDraw();
	}
}

void KochaEngine::GameObjectManager::ShowGUI()
{
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		(*it)->ShowGUI();
	}
}

void KochaEngine::GameObjectManager::HitObject(GameObject* arg_gameObject, const GameObjectType& arg_objType)
{
	if (arg_gameObject->IsDead()) return; //オブジェクトがそもそも死んでいたら判定を取らない

	auto objType = arg_gameObject->GetType();
	auto objSphere = arg_gameObject->GetSphere();
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() != arg_objType) continue; //指定したタイプ以外だったら無視する
		if ((*it)->IsDead()) continue; //判定対象のオブジェクトが死んでいたら無視する

		//円と円の当たり判定
		if (Collision::HitSphereToSphere(objSphere, (*it)->GetSphere()))
		{
			arg_gameObject->Hit();
			(*it)->Hit();
		}
	}
}

int KochaEngine::GameObjectManager::GetTypeCount(const GameObjectType& arg_objType)
{
	int count = 0;
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == arg_objType)
		{
			count++;
		}
	}
	return count;
}

void KochaEngine::GameObjectManager::Remove()
{
	for (auto it = gameObjects.begin(); it != gameObjects.end();)
	{
		if (!(*it)->IsDelete()) { ++it; continue; }
		delete *it;
		it = gameObjects.erase(it);
	}
}

void KochaEngine::GameObjectManager::RemoveItem()
{
	for (auto it = gameObjects.begin(); it != gameObjects.end();)
	{
		if ((*it)->GetType() != GameObjectType::ENHANCEMENT_ITEM &&
			(*it)->GetType() != GameObjectType::JAMMING_SPINE)
		{
			++it;
			continue;
		}
		delete* it;
		it = gameObjects.erase(it);
	}
}

void KochaEngine::GameObjectManager::RemoveAll()
{
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		delete (*it);
	}
	gameObjects.clear();
}

KochaEngine::Player* KochaEngine::GameObjectManager::GetPlayer()
{
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == PLAYER)
		{
			Player* player = static_cast<Player*>(static_cast<void*>(*it));
			return player;
		}
	}
	return nullptr;
}

KochaEngine::Wall* KochaEngine::GameObjectManager::GetWall()
{
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == WALL)
		{
			Wall* wall = static_cast<Wall*>(static_cast<void*>(*it));
			return wall;
		}
	}
	return nullptr;
}

KochaEngine::DeadLine* KochaEngine::GameObjectManager::GetDeadLine()
{
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == DEADLINE)
		{
			DeadLine* deadLine = static_cast<DeadLine*>(static_cast<void*>(*it));
			return deadLine;
		}
	}
	return nullptr;
}
