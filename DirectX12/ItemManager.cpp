#include "ItemManager.h"
#include "GameObjectManager.h"
#include "EnhancementItem.h"
#include "JammingSpine.h"
#include "Wall.h"

KochaEngine::ItemManager::ItemManager(Camera* arg_camera, GameObjectManager* arg_gManager)
{
	if (!arg_camera) return;
	if (!arg_gManager) return;

	camera = arg_camera;
	gManager = arg_gManager;
}

KochaEngine::ItemManager::~ItemManager()
{
	enhancementItems.clear();
	jammingSpines.clear();
}

void KochaEngine::ItemManager::Initialize()
{
	pWall = gManager->GetWall();
	enhancementItems.clear();
	jammingSpines.clear();
	CompareTheRightmost();

	AddEnhItem(Vector3(20, 10, 0));
	AddJamSpine(Vector3(-30, -20, 0));
}

void KochaEngine::ItemManager::Update()
{
	//生成用デバッグ
	if (Input::TriggerKey(DIK_P))
	{
		AddEnhItem(Vector3(20, 10, 0));
	}
	if (Input::TriggerKey(DIK_O))
	{
		AddJamSpine(Vector3(-30, -20, 0));
	}
	//消去用デバッグ
	if (Input::TriggerKey(DIK_L))
	{
		if (!enhancementItems.empty())
		{
			enhancementItems[0]->Dead();
		}
	}
	if (Input::TriggerKey(DIK_K))
	{
		if (!jammingSpines.empty())
		{
			jammingSpines[0]->Dead();
		}
	}
}

void KochaEngine::ItemManager::AddEnhItem(const Vector3& arg_position)
{
	EnhancementItem* item = new EnhancementItem(camera, gManager, arg_position + Vector3(pWall->GetCenterPos().x, 0, 0), this);
	gManager->AddObject(item);
	enhancementItems.push_back(item);
	CompareTheRightmost();
}

void KochaEngine::ItemManager::AddJamSpine(const Vector3& arg_position)
{
	JammingSpine* spine = new JammingSpine(camera, gManager, arg_position + Vector3(pWall->GetCenterPos().x, 0, 0), this);
	gManager->AddObject(spine);
	jammingSpines.push_back(spine);
	CompareTheRightmost();
}

void KochaEngine::ItemManager::DeleteFromVector(GameObject* arg_pObj, const GameObjectType arg_objType)
{
	if (arg_objType == GameObjectType::ENHANCEMENT_ITEM)
	{
		DeleteFromItems(static_cast<EnhancementItem*>(arg_pObj));
	}
	else if (arg_objType == GameObjectType::JAMMING_SPINE)
	{
		DeleteFromSpines(static_cast<JammingSpine*>(arg_pObj));
	}
	else
	{
		assert(0);
	}
}

void KochaEngine::ItemManager::DeleteFromItems(EnhancementItem* arg_enhItem)
{
	std::vector<EnhancementItem*>::iterator itr = enhancementItems.begin();
	for (; itr != enhancementItems.end(); itr++)
	{
		if (*itr == arg_enhItem)
		{
			break;
		}		
	}
	assert(itr != enhancementItems.end());

	enhancementItems.erase(itr);
	CompareTheRightmost();
}

void KochaEngine::ItemManager::DeleteFromSpines(JammingSpine* arg_jamSpine)
{	
	std::vector<JammingSpine*>::iterator itr = jammingSpines.begin();
	for (; itr != jammingSpines.end(); itr++)
	{
		if (*itr == arg_jamSpine)
		{
			break;
		}
	}
	assert(itr != jammingSpines.end());	

	jammingSpines.erase(itr);
	CompareTheRightmost();
}

void KochaEngine::ItemManager::CompareTheRightmost()
{
	Vector2 wallPos = pWall->GetCenterPos();
	theRightmostPos = Vector3(wallPos.x, wallPos.y, 0);

	for (std::vector<EnhancementItem*>::iterator itr = enhancementItems.begin(); itr != enhancementItems.end(); itr++)
	{
		if ((*itr)->GetPosition().x > theRightmostPos.x)
		{
			theRightmostPos = (*itr)->GetPosition();
		}
	}
	for (std::vector<JammingSpine*>::iterator itr = jammingSpines.begin(); itr != jammingSpines.end(); itr++)
	{
		if ((*itr)->GetPosition().x > theRightmostPos.x)
		{
			theRightmostPos = (*itr)->GetPosition();
		}
	}
}