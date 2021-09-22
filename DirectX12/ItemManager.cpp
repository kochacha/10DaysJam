#include "ItemManager.h"
#include "Util.h"
#include "GameObjectManager.h"
#include "EnhancementItem.h"
#include "JammingSpine.h"
#include "Wall.h"
#include "ScrollManager.h"
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

void KochaEngine::ItemManager::Initialize(ScrollManager* arg_scrollManager)
{
	pWall = gManager->GetWall();
	scrollManager = arg_scrollManager;
	enhancementItems.clear();
	jammingSpines.clear();
	CompareTheRightmost();

	/*AddEnhItem(Vector3(20, 10, 0), ItemEmitPosition::FROM_CENTER);
	AddJamSpine(Vector3(-30, -20, 0), ItemEmitPosition::FROM_CENTER);*/
	emitInterval = 0;	

	//生成座標レーンの初期化
	arrayEmitHight[0] = pWall->GetPlayableSize().y * 0.5f;
	arrayEmitHight[5] = 0;
	arrayEmitHight[10] = -pWall->GetPlayableSize().y * 0.5f;

	float unitHeight = pWall->GetPlayableSize().y * 0.1f;
	for (int i = 1; i <= 4; i++)
	{
		arrayEmitHight[i] = pWall->GetPlayableSize().y * 0.5f - unitHeight * i;
		arrayEmitHight[10 - i] = -pWall->GetPlayableSize().y * 0.5f + unitHeight * i;
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
	EnhancementItem* item = new EnhancementItem(camera, gManager, arg_position, this, arg_emitOption);
	gManager->AddObject(item);
	enhancementItems.push_back(item);
	CompareTheRightmost();
}

void KochaEngine::ItemManager::AddJamSpine(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption)
{
	JammingSpine* spine = new JammingSpine(camera, gManager, arg_position, this, arg_emitOption, GetIsSpineMove());
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
	if (emitInterval < GetMaxEmitInterval()) return;

	//一番右のアイテムが中心より右にあったら
	//if (theRightmostPos.x > pWall->GetCenterPos().x) return;

	//生成処理
	GeneralEmitCommand(ItemEmitPosition::MORE_THAN_RIGHTSIDE, ItemEmitOption::NORMAL);

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
	GeneralEmitCommand(ItemEmitPosition::FROM_CENTER, ItemEmitOption::SMASHING_WALL);
}

void KochaEngine::ItemManager::GeneralEmitCommand(const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption)
{
	const unsigned int rndMax = 20;
	const unsigned int rndCoefficient = GetEmitTypeCoefficient();
	int rnd = Util::GetRandInt(rndMax);

	//強化アイテム生成
	if (rnd < rndCoefficient)
	{
		Vector3 emitPos;
		while (true)
		{
			emitPos = DetermineEmitPos(GameObjectType::ENHANCEMENT_ITEM);
			FixEmitPositionByCondition(emitPos, arg_emitPosition);

			if (!IsHitExistingItems(GameObjectType::ENHANCEMENT_ITEM, emitPos)) break;
		}
		AddEnhItem(emitPos, arg_emitPosition, arg_emitOption);
	}
	//おじゃまトゲ生成
	else
	{
		Vector3 emitPos;
		while (true)
		{
			emitPos = DetermineEmitPos(GameObjectType::JAMMING_SPINE);
			FixEmitPositionByCondition(emitPos, arg_emitPosition);

			if (!IsHitExistingItems(GameObjectType::JAMMING_SPINE, emitPos)) break;
		}
		AddJamSpine(emitPos, arg_emitPosition, arg_emitOption);
	}
}

void KochaEngine::ItemManager::FixEmitPositionByCondition(Vector3& arg_position, const ItemEmitPosition arg_emitPosition)
{
	switch (arg_emitPosition)
	{
	case ItemEmitPosition::ABSOLUTE_WORLDPOS:
		break;
	case ItemEmitPosition::FROM_CENTER:
		arg_position += Vector3(pWall->GetCenterPos().x, pWall->GetCenterPos().y, 0);
		break;
	case ItemEmitPosition::MORE_THAN_RIGHTSIDE:
		arg_position.x = MARGIN_FRAME;
		arg_position += Vector3(pWall->GetCenterPos().x + pWall->GetPlayableSize().x / 2, pWall->GetCenterPos().y, 0);
		break;
	default:
		break;
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

	rndMax = 5;
	rnd = Util::GetRandInt(rndMax) - 2;
	emitPos.y += rnd;

	//y座標を枠の内側に収める
	if (emitPos.y > pWall->GetPlayableSize().y * 0.5f)
	{
		emitPos.y = pWall->GetPlayableSize().y * 0.5f;
	}
	else if (emitPos.y < -pWall->GetPlayableSize().y * 0.5f)
	{
		emitPos.y = -pWall->GetPlayableSize().y * 0.5f;
	}

	return emitPos;
}

const bool KochaEngine::ItemManager::IsHitExistingItems(const GameObjectType arg_objType, const Vector3& arg_position)
{
	_Sphere sphere;
	sphere.position = arg_position;
	if (arg_objType == GameObjectType::ENHANCEMENT_ITEM)
	{
		sphere.radius = 5.0f;
	}
	else if (arg_objType == GameObjectType::JAMMING_SPINE)
	{
		sphere.radius = 4.0f;
	}
	else
	{
		assert(0);
	}

	//強化アイテムとの判定
	for (auto item : enhancementItems)
	{
		if (Collision::HitSphereToSphere(sphere, item->GetSphere()))
		{
			return true;
		}
	}

	//おじゃまトゲとの判定
	for (auto spine : jammingSpines)
	{
		if (Collision::HitSphereToSphere(sphere, spine->GetSphere()))
		{
			return true;
		}
	}

	return false;
}

const unsigned int KochaEngine::ItemManager::GetMaxEmitInterval()
{
	unsigned int answerVal = 0;
	const int scrollLevel = scrollManager->GetScrollLevel() / 2 + 1;
	switch (scrollLevel)
	{
	case 1:
		answerVal = 240;
		break;
	case 2:
		answerVal = 200;
		break;
	case 3:
		answerVal = 160;
		break;
	case 4:
		answerVal = 120;
		break;
	default:
		answerVal = 120;
		break;
	}

	return answerVal;
}

const unsigned int KochaEngine::ItemManager::GetEmitTypeCoefficient()
{
	unsigned int answerVal = 0;
	const int scrollLevel = scrollManager->GetScrollLevel() / 2 + 1;
	switch (scrollLevel)
	{
	case 1:
		answerVal = 16;
		break;
	case 2:
		answerVal = 16;
		break;
	case 3:
		answerVal = 16;
		break;
	case 4:
		answerVal = 16;
		break;
	default:
		answerVal = 16;
		break;
	}

	return answerVal;
}

const bool KochaEngine::ItemManager::GetIsSpineMove()
{
	unsigned int cofficient = 0;
	const unsigned int rndMax = 20;
	const int scrollLevel = scrollManager->GetScrollLevel() / 2 + 1;
	switch (scrollLevel)
	{
	case 1:
		cofficient = 17;
		break;
	case 2:
		cofficient = 15;
		break;
	case 3:
		cofficient = 13;
		break;
	case 4:
		cofficient = 11;
		break;
	default:
		cofficient = 9;
		break;
	}

	if (Util::GetRandInt(rndMax) > cofficient)
	{
		return true;
	}
	else
	{
		return false;
	}
}
