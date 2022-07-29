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

	//�������W���[���̏�����
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
	//�����p�f�o�b�O
	if (Input::TriggerKey(DIK_P))
	{
		AddEnhItem(Vector3(20, 10, 0), ItemEmitPosition::MORE_THAN_RIGHTSIDE);
	}
	if (Input::TriggerKey(DIK_O))
	{
		AddJamSpine(Vector3(-30, -20, 0), ItemEmitPosition::MORE_THAN_RIGHTSIDE);
	}
	//�����p�f�o�b�O
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
	//�������ɂ���ĉ����폜���邩���߂�
	if (arg_objType == GameObjectType::ENHANCEMENT_ITEM)
	{
		DeleteFromItems(static_cast<EnhancementItem*>(arg_pObj));
	}
	else if (arg_objType == GameObjectType::JAMMING_SPINE)
	{
		DeleteFromSpines(static_cast<JammingSpine*>(arg_pObj));
	}
	//�A�C�e���ł��g�Q�ł��Ȃ���Όx��
	else
	{
		assert(0);
	}
}

void KochaEngine::ItemManager::AddEnhItem(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption)
{
	EnhancementItem* item = new EnhancementItem(camera, gManager, arg_position, this, arg_emitOption);
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�ɓo�^
	gManager->AddObject(item);
	//���g�̎��z��ɂ��o�^
	enhancementItems.push_back(item);
}

void KochaEngine::ItemManager::AddJamSpine(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption)
{
	JammingSpine* spine = new JammingSpine(camera, gManager, arg_position, this, arg_emitOption, GetIsSpineMove());
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�ɓo�^
	gManager->AddObject(spine);
	//���g�̎��z��ɂ��o�^
	jammingSpines.push_back(spine);
}

const unsigned int KochaEngine::ItemManager::GetScrollLevel()
{
	return scrollManager->GetScrollLevel();
}

void KochaEngine::ItemManager::DeleteFromItems(EnhancementItem* arg_enhItem)
{
	//�폜�\��̃A�C�e�������邩����
	std::vector<EnhancementItem*>::iterator itr = enhancementItems.begin();
	for (; itr != enhancementItems.end(); itr++)
	{
		if (*itr == arg_enhItem)
		{
			break;
		}		
	}
	//�Ȃ�������x��
	assert(itr != enhancementItems.end());

	//�폜���s
	enhancementItems.erase(itr);
}

void KochaEngine::ItemManager::DeleteFromSpines(JammingSpine* arg_jamSpine)
{	
	//�폜�\��̃g�Q�����邩����
	std::vector<JammingSpine*>::iterator itr = jammingSpines.begin();
	for (; itr != jammingSpines.end(); itr++)
	{
		if (*itr == arg_jamSpine)
		{
			break;
		}
	}
	//�Ȃ�������x��
	assert(itr != jammingSpines.end());	

	//�폜���s
	jammingSpines.erase(itr);
}

void KochaEngine::ItemManager::EmitItemsNormalTime()
{
	//�����C���^�[�o�����Z
	emitInterval++;
	//�C���^�[�o��������Ȃ�������
	if (emitInterval < GetMaxEmitInterval()) return;	

	//��������
	GeneralEmitCommand(ItemEmitPosition::MORE_THAN_RIGHTSIDE, ItemEmitOption::NORMAL);

	//�C���^�[�o�����Z�b�g
	emitInterval = 0;
}

void KochaEngine::ItemManager::EmitItemsSmashing()
{
	Player* pPlayer = gManager->GetPlayer();
	int playerBackCount = pPlayer->GetBackCount();
	//�v���C���[���ǉ������݂����Ă��Ȃ�������
	if (playerBackCount <= 0) return;
	//�^���^�C�g����ʂ�������
	if (!pPlayer->IsInGame()) return;
	//�����Ԋu�Ǘ�
	if ((playerBackCount % 6) != 0) return;

	//��������
	GeneralEmitCommand(ItemEmitPosition::FROM_CENTER, ItemEmitOption::SMASHING_WALL);
}

void KochaEngine::ItemManager::GeneralEmitCommand(const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption)
{
	//�A�C�e���ƃg�Q�̂ǂ���𐶐����邩
	const unsigned int rndMax = 100;
	const unsigned int rndCoefficient = GetEmitTypeCoefficient();
	int rnd = Util::GetRandInt(rndMax);
	hitCheckCount = 0;

	//�����A�C�e������
	if (rnd < rndCoefficient)
	{
		Vector3 emitPos;
		//�����̃I�u�W�F�N�g�Əd�Ȃ�Ȃ��悤�ɍ��W����
		while (true)
		{
			emitPos = DetermineEmitPos(GameObjectType::ENHANCEMENT_ITEM);
			FixEmitPositionByCondition(emitPos, arg_emitPosition);

			if (!IsHitExistingItems(GameObjectType::ENHANCEMENT_ITEM, emitPos)) break;
		}
		//�������s
		AddEnhItem(emitPos, arg_emitPosition, arg_emitOption);
	}
	//������܃g�Q����
	else
	{
		Vector3 emitPos;
		//�����̃I�u�W�F�N�g�Əd�Ȃ�Ȃ��悤�ɍ��W����
		while (true)
		{
			emitPos = DetermineEmitPos(GameObjectType::JAMMING_SPINE);
			FixEmitPositionByCondition(emitPos, arg_emitPosition);

			if (!IsHitExistingItems(GameObjectType::JAMMING_SPINE, emitPos)) break;
		}
		//�������s
		AddJamSpine(emitPos, arg_emitPosition, arg_emitOption);
	}
}

void KochaEngine::ItemManager::FixEmitPositionByCondition(Vector3& arg_position, const ItemEmitPosition arg_emitPosition)
{
	switch (arg_emitPosition)
	{	
	case ItemEmitPosition::ABSOLUTE_WORLDPOS:
		//���[���h���W�Ƃ��Ă��̂܂܂�
		break;
	case ItemEmitPosition::FROM_CENTER:
		//��ʂ̒��S����ǂꂾ������Ă��邩�ɕϊ�
		arg_position += Vector3(pWall->GetCenterPos().x, pWall->GetCenterPos().y, 0);
		break;
	case ItemEmitPosition::MORE_THAN_RIGHTSIDE:
		//y���W�͂��̂܂܂ŉE�[����o�Ă���悤�ɕϊ�
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

	//�����A�C�e���Ȃ�
	if (arg_objType == GameObjectType::ENHANCEMENT_ITEM)
	{	
		//9���[���̒����烉���_��
		rndMax = 9;
		rnd = Util::GetRandInt(rndMax);
		emitPos = Vector3(0, arrayEmitHight[rnd + 1], 0);
	}
	//�g�Q�Ȃ�
	else if (arg_objType == GameObjectType::JAMMING_SPINE)
	{
		//5���[���̒����烉���_��
		rndMax = 5;
		rnd = Util::GetRandInt(rndMax);
		emitPos = Vector3(0, arrayEmitHight[rnd * 2 + 1], 0);
	}

	//�킸����y���W���΂炯������
	rndMax = 5;
	rnd = Util::GetRandInt(rndMax) - 2;
	emitPos.y += rnd;

	//y���W��g�̓����Ɏ��߂�
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

	//���ꂩ�琶���\��̏Փ˔�����Č�
	//radius�͑傫�߂ɂ��Ƃ�
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
	//�A�C�e�����g�Q�ȊO�𐶐������悤�Ƃ��Ă�����
	else
	{
		//�x��
		assert(0);
	}

	//�����A�C�e���Ƃ̔���
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

	//������܃g�Q�Ƃ̔���
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
