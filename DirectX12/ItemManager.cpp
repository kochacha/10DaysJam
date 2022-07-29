#include "ItemManager.h"
#include "Util.h"
#include "GameObjectManager.h"
#include "EnhancementItem.h"
#include "JammingSpine.h"
#include "Wall.h"
#include "ScrollManager.h"
#include "LevelSetKeeper.h"

KochaEngine::ItemManager::ItemManager(Camera* arg_camera, GameObjectManager* arg_gManager)
	:camera(nullptr),
	 gManager(nullptr),
	 pWall(nullptr),
	 scrollManager(nullptr),
	 emitInterval(0),
	 MARGIN_FRAME(12.0f),
	 hitCheckCount(0)
{
	if (!arg_camera) return;
	if (!arg_gManager) return;

	camera = arg_camera;
	gManager = arg_gManager;

	enhancementItems.clear();
	jammingSpines.clear();
	memset(arrayEmitHight, 0.0f, sizeof(arrayEmitHight));

	for (int i = 0; i < 30; i++)
	{
		Object* tag = new Object("plane");
		tag->SetRotate(Vector3(0, 0, 0));
		tag->SetTexture("Resources/togeTag.png");
		tag->SetBillboardType(Object::BillboardType::BILLBOARD);
		spineTags.push_back(tag);
	}
}

KochaEngine::ItemManager::~ItemManager()
{
	enhancementItems.clear();
	jammingSpines.clear();
	for (auto tag : spineTags)
	{
		delete tag;
		tag = nullptr;
	}
	spineTags.clear();
}

void KochaEngine::ItemManager::Initialize(ScrollManager* arg_scrollManager)
{
	pWall = gManager->GetWall();
	scrollManager = arg_scrollManager;
	enhancementItems.clear();
	jammingSpines.clear();

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

	activeTagCount = 0;
}

void KochaEngine::ItemManager::Update()
{
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

void KochaEngine::ItemManager::Draw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (!gManager->GetWall())
	{
		return;
	}

	CheckTagActivation();

	for (int i = 0; i < activeTagCount; i++)
	{
		spineTags[i]->Draw(arg_camera, arg_lightManager);
	}
}

void KochaEngine::ItemManager::DeleteFromVector(GameObject* arg_pObj, const GameObjectType arg_objType)
{
	//第二引数によって何を削除するか決める
	if (arg_objType == GameObjectType::ENHANCEMENT_ITEM)
	{
		DeleteFromItems(static_cast<EnhancementItem*>(arg_pObj));
	}
	else if (arg_objType == GameObjectType::JAMMING_SPINE)
	{
		DeleteFromSpines(static_cast<JammingSpine*>(arg_pObj));
	}
	//アイテムでもトゲでもなければ警告
	else
	{
		assert(0);
	}
}

void KochaEngine::ItemManager::AddEnhItem(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption)
{
	EnhancementItem* item = new EnhancementItem(camera, gManager, arg_position, this, arg_emitOption);
	//ゲームオブジェクトマネージャーに登録
	gManager->AddObject(item);
	//自身の持つ配列にも登録
	enhancementItems.push_back(item);
}

void KochaEngine::ItemManager::AddJamSpine(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption)
{
	JammingSpine* spine = new JammingSpine(camera, gManager, arg_position, this, arg_emitOption, GetIsSpineMove());
	//ゲームオブジェクトマネージャーに登録
	gManager->AddObject(spine);
	//自身の持つ配列にも登録
	jammingSpines.push_back(spine);
}

const unsigned int KochaEngine::ItemManager::GetScrollLevel()
{
	return scrollManager->GetScrollLevel();
}

void KochaEngine::ItemManager::DeleteFromItems(EnhancementItem* arg_enhItem)
{
	//削除予定のアイテムがあるか検索
	std::vector<EnhancementItem*>::iterator itr = enhancementItems.begin();
	for (; itr != enhancementItems.end(); itr++)
	{
		if (*itr == arg_enhItem)
		{
			break;
		}		
	}
	//なかったら警告
	assert(itr != enhancementItems.end());

	//削除実行
	enhancementItems.erase(itr);
}

void KochaEngine::ItemManager::DeleteFromSpines(JammingSpine* arg_jamSpine)
{	
	//削除予定のトゲがあるか検索
	std::vector<JammingSpine*>::iterator itr = jammingSpines.begin();
	for (; itr != jammingSpines.end(); itr++)
	{
		if (*itr == arg_jamSpine)
		{
			break;
		}
	}
	//なかったら警告
	assert(itr != jammingSpines.end());	

	//削除実行
	jammingSpines.erase(itr);
}

void KochaEngine::ItemManager::EmitItemsNormalTime()
{
	//生成インターバル加算
	emitInterval++;
	//インターバルが足りなかったら
	if (emitInterval < GetMaxEmitInterval()) return;	

	//生成処理
	GeneralEmitCommand(ItemEmitPosition::MORE_THAN_RIGHTSIDE, ItemEmitOption::NORMAL);

	//インターバルリセット
	emitInterval = 0;
}

void KochaEngine::ItemManager::EmitItemsSmashing()
{
	Player* pPlayer = gManager->GetPlayer();
	int playerBackCount = pPlayer->GetBackCount();
	//プレイヤーが壁押し込みをしていなかったら
	if (playerBackCount <= 0) return;
	//疑似タイトル画面だったら
	if (!pPlayer->IsInGame()) return;
	//生成間隔管理
	if ((playerBackCount % 6) != 0) return;

	//生成処理
	GeneralEmitCommand(ItemEmitPosition::FROM_CENTER, ItemEmitOption::SMASHING_WALL);
}

void KochaEngine::ItemManager::GeneralEmitCommand(const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption)
{
	//アイテムとトゲのどちらを生成するか
	const unsigned int rndMax = 100;
	const unsigned int rndCoefficient = GetEmitTypeCoefficient();
	int rnd = Util::GetRandInt(rndMax);
	hitCheckCount = 0;

	//強化アイテム生成
	if (rnd < rndCoefficient)
	{
		Vector3 emitPos;
		//既存のオブジェクトと重ならないように座標決定
		while (true)
		{
			emitPos = DetermineEmitPos(GameObjectType::ENHANCEMENT_ITEM);
			FixEmitPositionByCondition(emitPos, arg_emitPosition);

			if (!IsHitExistingItems(GameObjectType::ENHANCEMENT_ITEM, emitPos)) break;
		}
		//生成実行
		AddEnhItem(emitPos, arg_emitPosition, arg_emitOption);
	}
	//おじゃまトゲ生成
	else
	{
		Vector3 emitPos;
		//既存のオブジェクトと重ならないように座標決定
		while (true)
		{
			emitPos = DetermineEmitPos(GameObjectType::JAMMING_SPINE);
			FixEmitPositionByCondition(emitPos, arg_emitPosition);

			if (!IsHitExistingItems(GameObjectType::JAMMING_SPINE, emitPos)) break;
		}
		//生成実行
		AddJamSpine(emitPos, arg_emitPosition, arg_emitOption);
	}
}

void KochaEngine::ItemManager::FixEmitPositionByCondition(Vector3& arg_position, const ItemEmitPosition arg_emitPosition)
{
	switch (arg_emitPosition)
	{	
	case ItemEmitPosition::ABSOLUTE_WORLDPOS:
		//ワールド座標としてそのままに
		break;
	case ItemEmitPosition::FROM_CENTER:
		//画面の中心からどれだけ離れているかに変換
		arg_position += Vector3(pWall->GetCenterPos().x, pWall->GetCenterPos().y, 0);
		break;
	case ItemEmitPosition::MORE_THAN_RIGHTSIDE:
		//y座標はそのままで右端から出てくるように変換
		arg_position.x += MARGIN_FRAME;
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

	//強化アイテムなら
	if (arg_objType == GameObjectType::ENHANCEMENT_ITEM)
	{	
		//9レーンの中からランダム
		rndMax = 9;
		rnd = Util::GetRandInt(rndMax);
		emitPos = Vector3(0, arrayEmitHight[rnd + 1], 0);
	}
	//トゲなら
	else if (arg_objType == GameObjectType::JAMMING_SPINE)
	{
		//5レーンの中からランダム
		rndMax = 5;
		rnd = Util::GetRandInt(rndMax);
		emitPos = Vector3(0, arrayEmitHight[rnd * 2 + 1], 0);
	}

	//わずかにy座標をばらけさせる
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

const bool KochaEngine::ItemManager::IsHitExistingItems(const GameObjectType arg_objType, Vector3& arg_position)
{
	static const int MAX_HITCHECK = 10;
	if (hitCheckCount >= MAX_HITCHECK)
	{
		//return false;
		arg_position.x += 5.0f * (hitCheckCount / MAX_HITCHECK);
	}

	//これから生成予定の衝突判定を再現
	//radiusは大きめにしとく
	_Sphere sphere;
	sphere.position = arg_position;
	if (arg_objType == GameObjectType::ENHANCEMENT_ITEM)
	{
		sphere.radius = 6.0f;
	}
	else if (arg_objType == GameObjectType::JAMMING_SPINE)
	{
		sphere.radius = 5.0f;
	}
	//アイテムかトゲ以外を生成させようとしていたら
	else
	{
		//警告
		assert(0);
	}

	//強化アイテムとの判定
	for (auto item : enhancementItems)
	{
		_Sphere itemSphere = _Sphere();
		itemSphere.radius = 6.0f;
		itemSphere.position = item->GetPrearrangedPosition();
		if (Collision::HitSphereToSphere(sphere, itemSphere))
		{
			hitCheckCount++;
			return true;
		}
	}

	//おじゃまトゲとの判定
	for (auto spine : jammingSpines)
	{
		_Sphere spineSphere = _Sphere();
		spineSphere.radius = 5.0f;
		spineSphere.position = spine->GetPrearrangedPosition();
		if (Collision::HitSphereToSphere(sphere, spineSphere))
		{
			hitCheckCount++;
			return true;
		}
	}

	return false;
}

const unsigned int KochaEngine::ItemManager::GetMaxEmitInterval()
{
	LevelSetIndivisual lsi = LevelSetKeeper::GetInstance()->GetCurrentModeWithLevel(scrollManager->GetScrollLevel());
	return lsi.frameObjectEmitInterval;
}

const unsigned int KochaEngine::ItemManager::GetEmitTypeCoefficient()
{
	LevelSetIndivisual lsi = LevelSetKeeper::GetInstance()->GetCurrentModeWithLevel(scrollManager->GetScrollLevel());
	return lsi.percentageEmitItem;
}

const bool KochaEngine::ItemManager::GetIsSpineMove()
{
	LevelSetIndivisual lsi = LevelSetKeeper::GetInstance()->GetCurrentModeWithLevel(scrollManager->GetScrollLevel());
	return Util::GetRandInt(100) < lsi.percentageSpineMove;
}

void KochaEngine::ItemManager::CheckTagActivation()
{
	activeTagCount = 0;

	for (auto spine : jammingSpines)
	{
		if (spine->IsCutOffScreen())
		{
			spineTags[activeTagCount]->SetPosition(spine->GetTagPosition());
			spineTags[activeTagCount]->SetScale(spine->GetTagScale());

			activeTagCount++;
		}
	}
}
