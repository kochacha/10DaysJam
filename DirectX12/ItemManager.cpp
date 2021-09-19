#include "ItemManager.h"
#include "Util.h"
#include "GameObjectManager.h"
#include "EnhancementItem.h"
#include "JammingSpine.h"
#include "Wall.h"
const float KochaEngine::ItemManager::MARGIN_FRAME = 12.0f;

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

	/*AddEnhItem(Vector3(20, 10, 0), ItemEmitPosition::FROM_CENTER);
	AddJamSpine(Vector3(-30, -20, 0), ItemEmitPosition::FROM_CENTER);*/
	emitInterval = 0;
	maxEmitInterval = 240;

	//生成座標レーンの初期化
	arrayEmitHight[0] = pWall->GetPlayableSize().y / 2;
	arrayEmitHight[5] = 0;
	arrayEmitHight[10] = -pWall->GetPlayableSize().y / 2;

	float unitHeight = pWall->GetPlayableSize().y / 10;
	for (int i = 1; i <= 4; i++)
	{
		arrayEmitHight[i] = pWall->GetPlayableSize().y / 2 - unitHeight * i;
		arrayEmitHight[10 - i] = -pWall->GetPlayableSize().y / 2 + unitHeight * i;
	}
}

void KochaEngine::ItemManager::Update()
{
	CompareTheRightmost();
	EmitItemsNormalTime();
	EmitItemsSmashing();

#ifdef _DEBUG
	//生成用デバッグ
	if (Input::TriggerKey(DIK_P))
	{
		AddEnhItem(Vector3(20, 10, 0), ItemEmitPosition::MORE_THAN_RIGHTSIDE);
	}
	if (Input::TriggerKey(DIK_O))
	{
		AddJamSpine(Vector3(-30, -20, 0), ItemEmitPosition::MORE_THAN_RIGHTSIDE);
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
#endif _DEBUG
}

void KochaEngine::ItemManager::AddEnhItem(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption)
{
	Vector3 emitPos = arg_position;
	switch (arg_emitPosition)
	{
	case ItemEmitPosition::ABSOLUTE_WORLDPOS:
		break;
	case ItemEmitPosition::FROM_CENTER:
		emitPos += Vector3(pWall->GetCenterPos().x, pWall->GetCenterPos().y, 0);
		break;
	case ItemEmitPosition::MORE_THAN_RIGHTSIDE:
		emitPos.x = MARGIN_FRAME;
		emitPos += Vector3(pWall->GetCenterPos().x + pWall->GetPlayableSize().x / 2, pWall->GetCenterPos().y, 0);		
		break;
	default:
		break;
	}

	EnhancementItem* item = new EnhancementItem(camera, gManager, emitPos, this, arg_emitOption);
	gManager->AddObject(item);
	enhancementItems.push_back(item);
	CompareTheRightmost();
}

void KochaEngine::ItemManager::AddJamSpine(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption)
{
	Vector3 emitPos = arg_position;
	switch (arg_emitPosition)
	{
	case ItemEmitPosition::ABSOLUTE_WORLDPOS:
		break;
	case ItemEmitPosition::FROM_CENTER:
		emitPos += Vector3(pWall->GetCenterPos().x, pWall->GetCenterPos().y, 0);
		break;
	case ItemEmitPosition::MORE_THAN_RIGHTSIDE:
		emitPos.x = MARGIN_FRAME;
		emitPos += Vector3(pWall->GetCenterPos().x + pWall->GetPlayableSize().x / 2, pWall->GetCenterPos().y, 0);
		break;
	default:
		break;
	}

	JammingSpine* spine = new JammingSpine(camera, gManager, emitPos, this, arg_emitOption);
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

void KochaEngine::ItemManager::EmitItemsNormalTime()
{
	//生成インターバル加算
	emitInterval++;
	//インターバルが足りなかったら
	if (emitInterval < maxEmitInterval) return;

	//一番右のアイテムが中心より右にあったら
	//if (theRightmostPos.x > pWall->GetCenterPos().x) return;

	//生成処理
	static int rndMax = 5;
	static int rndCoefficient = 4;
	int rnd = Util::GetRandInt(rndMax);
	
	//強化アイテム生成
	if (rnd < rndCoefficient)
	{
		Vector3 emitPos = DetermineEmitPos(GameObjectType::ENHANCEMENT_ITEM);		
		AddEnhItem(emitPos, ItemEmitPosition::MORE_THAN_RIGHTSIDE);
	}
	//おじゃまトゲ生成
	else
	{
		Vector3 emitPos = DetermineEmitPos(GameObjectType::JAMMING_SPINE);		
		AddJamSpine(emitPos, ItemEmitPosition::MORE_THAN_RIGHTSIDE);
	}

	//インターバルリセット
	emitInterval = 0;
}

void KochaEngine::ItemManager::EmitItemsSmashing()
{
	Player* pPlayer = gManager->GetPlayer();
	int playerBackCount = pPlayer->GetBackCount();
	if (playerBackCount <= 0) return;

	if (!pPlayer->IsInGame()) return;

	if ((playerBackCount % 6) != 0) return;

	//生成処理
	static int rndMax = 5;
	static int rndCoefficient = 4;
	int rnd = Util::GetRandInt(rndMax);

	//強化アイテム生成
	if (rnd < rndCoefficient)
	{
		Vector3 emitPos = DetermineEmitPos(GameObjectType::ENHANCEMENT_ITEM);
		AddEnhItem(emitPos, ItemEmitPosition::FROM_CENTER, ItemEmitOption::SMASHING_WALL);
	}
	//おじゃまトゲ生成
	else
	{
		Vector3 emitPos = DetermineEmitPos(GameObjectType::JAMMING_SPINE);
		AddJamSpine(emitPos, ItemEmitPosition::FROM_CENTER, ItemEmitOption::SMASHING_WALL);
	}
}

KochaEngine::Vector3 KochaEngine::ItemManager::DetermineEmitPos(const GameObjectType arg_objType)
{
	Vector3 emitPos = Vector3();
	int rndMax = 0;
	int rnd = 0;

	if (arg_objType == GameObjectType::ENHANCEMENT_ITEM)
	{	
		rndMax = 9;
		rnd = Util::GetRandInt(rndMax);
		emitPos = Vector3(0, arrayEmitHight[rnd + 1], 0);
	}
	else if (arg_objType == GameObjectType::JAMMING_SPINE)
	{
		rndMax = 5;
		rnd = Util::GetRandInt(rndMax);
		emitPos = Vector3(0, arrayEmitHight[rnd * 2 + 1], 0);
	}

	//y座標を枠の内側に収める
	if (emitPos.y > pWall->GetPlayableSize().y / 2)
	{
		emitPos.y = pWall->GetPlayableSize().y / 2;
	}
	else if (emitPos.y < -pWall->GetPlayableSize().y / 2)
	{
		emitPos.y = -pWall->GetPlayableSize().y / 2;
	}

	return emitPos;
}
