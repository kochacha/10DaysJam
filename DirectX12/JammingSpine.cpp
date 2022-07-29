#include "JammingSpine.h"
#include "GameObjectManager.h"
#include "ItemManager.h"
#include "Wall.h"
#include "Util.h"

const int MOVE_FRAME = 15;
float KochaEngine::JammingSpine::DEAD_X = 0.0f;
float KochaEngine::JammingSpine::CUT_OFF_X = 0.0f;
float KochaEngine::JammingSpine::SURVIVE_LENGTH = 0.0f;

KochaEngine::JammingSpine::JammingSpine(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position, ItemManager* arg_iManager, const ItemEmitOption arg_option, const bool arg_isVMove)
	:gManager(nullptr),
	 iManager(nullptr),
	 pWall(nullptr),
	 emittedPlayerPosition(Vector3()),
	 prearrangedPosition(Vector3()),
	 moveCount(0),
	 isVerticalMove(false),
	 velVertical(0.3f)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	if (arg_iManager == nullptr) return;

	camera = arg_camera;
	gManager = arg_gManager;	
	iManager = arg_iManager;
	pWall = gManager->GetWall();
	isVerticalMove = arg_isVMove;	

	switch (arg_option)
	{
	case ItemEmitOption::NORMAL:
		emittedPlayerPosition = arg_position;
		position = arg_position;
		prearrangedPosition = arg_position;
		break;
	case ItemEmitOption::SMASHING_WALL:
		emittedPlayerPosition = gManager->GetPlayer()->GetPosition();
		position = emittedPlayerPosition;
		//�X�}�b�V�����������̖ڕW���W
		prearrangedPosition = arg_position;
		moveCount = MOVE_FRAME;
		break;
	default:
		position = arg_position;
		break;
	}

	obj = new Object("plane");
	Initialize();
}

KochaEngine::JammingSpine::~JammingSpine()
{
	if (obj)
	{
		delete obj;
	}
}

void KochaEngine::JammingSpine::Initialize()
{
	isAlpha = true;

	velocity.Zero();

	sphere.radius = 4.0f;

	SetObjParam();

	obj->SetRotate(Vector3(0, 0, 0));
	obj->SetScale(Vector3(10, 10, 10));
	obj->SetTexture("Resources/toge.png");
	obj->SetBillboardType(KochaEngine::Object::BILLBOARD);
}

void KochaEngine::JammingSpine::Update()
{
	//�X�}�b�V�����ɐ�������Ă�����
	if (moveCount >= 0)
	{
		obj->SetTexture("Resources/togeShadow.png");
		float scaleValue = Util::EaseOut(3, 10, (MOVE_FRAME - moveCount) / (float)MOVE_FRAME);
		obj->SetScale(Vector3(scaleValue, scaleValue, scaleValue));

		//���`��ԂŖڕW���W�܂ňړ�
		position.x = Util::Lerp(emittedPlayerPosition.x, prearrangedPosition.x, (MOVE_FRAME - moveCount) / (float)MOVE_FRAME);
		position.y = Util::Lerp(emittedPlayerPosition.y, prearrangedPosition.y, (MOVE_FRAME - moveCount) / (float)MOVE_FRAME);

		moveCount--;
	}
	else
	{
		obj->SetTexture("Resources/toge.png");
		obj->SetScale(Vector3(10, 10, 10));
	}

	MoveVertical();

	SetObjParam();

	//���ǂɂ�����
	if (position.x <= pWall->GetMinPos().x - DEAD_X)
	{
		Dead();
		return;
	}	

	gManager->HitObject(this, PLAYER);
}

void KochaEngine::JammingSpine::Hit()
{	
	Player* player = gManager->GetPlayer();

	//���g���ړ����Ă����
	if (moveCount > 0) return;

	if (player->GetBackCount() > 0)
	{
		//�X�}�b�V������̕Ǖt�߂̃g�Q�ɓ����鎞
		if (player->IsAbleHitSpine())
		{
			//�v���C���[�̓p���[�A�b�v���A���g�͏���
			player->PowerUp(GetType());
			Dead();			
		}
		return;
	}

	//�v���C���[���X�}�b�V�����Ȃ�
	if (player->IsSmashing())
	{
		//�v���C���[�̓p���[�A�b�v���A���g�͏���
		player->PowerUp(GetType());
		Dead();		
	}
	//�ʏ펞�Ȃ�
	else if(!player->IsStuning())
	{
		//�v���C���[�̓p���[�_�E��
		player->HitJammingSpine();
	}
}

void KochaEngine::JammingSpine::Dead()
{
	//ItemManager�ɓo�^����Ă��鎩�g���폜�v��
	iManager->DeleteFromVector(this, GetType());
	isDelete = true;
}

void KochaEngine::JammingSpine::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;

	if (IsCutOffScreen())
	{
		return;
	}

	float cameraPosX = arg_camera->GetEye().x;
	const float CAMERA_DISTANCE_X = 100;
	if (abs(position.x - cameraPosX) > CAMERA_DISTANCE_X) return;
	obj->Draw(arg_camera, arg_lightManager);
}

KochaEngine::GameObjectType KochaEngine::JammingSpine::GetType()
{
	return JAMMING_SPINE;
}

KochaEngine::Vector3 KochaEngine::JammingSpine::GetPrearrangedPosition() const
{
	return prearrangedPosition;
}

void KochaEngine::JammingSpine::SetObjParam()
{
	sphere.position = this->position;
	obj->SetPosition(position);
}

void KochaEngine::JammingSpine::MoveVertical()
{
	if (!isVerticalMove) return;
	if (moveCount > 0) return;

	position.y += velVertical * GetVerticalMoveSpeed();

	//��[�ł̐܂�Ԃ�
	if (position.y >= pWall->GetMaxPos().y)
	{
		position.y = pWall->GetMaxPos().y;
		velVertical *= -1;
	}
	//���[�ł̐܂�Ԃ�
	else if (position.y <= pWall->GetMinPos().y)
	{
		position.y = pWall->GetMinPos().y;
	    velVertical *= -1;
	}
}

const float KochaEngine::JammingSpine::GetVerticalMoveSpeed()
{
	//ScrollManager��scrollLevel��iManager���Q��
	const unsigned int scrollLevel = iManager->GetScrollLevel() / 2 + 1;
	float answerVal = 0.0f;

	switch (scrollLevel)
	{
	case 1:
		answerVal = 0.3f;
		break;
	case 2:
		answerVal = 0.45f;
		break;
	case 3:
		answerVal = 0.6f;
		break;
	case 4:
		answerVal = 0.75f;
		break;
	default:
		answerVal = 0.9f;
		break;
	}

	return answerVal;
}

bool KochaEngine::JammingSpine::IsCutOffScreen()
{
	return position.x <= pWall->GetMinPos().x - CUT_OFF_X;
}

KochaEngine::Vector3 KochaEngine::JammingSpine::GetTagPosition() const
{
	float posRate = (pWall->GetMinPos().x - position.x) - CUT_OFF_X;  //0�`SURVIVE_LENGTH�ɂȂ�͂�
	float posValue = Util::EaseOut(5.0f, 1.5f, posRate / SURVIVE_LENGTH);
	return Vector3(pWall->GetMinPos().x + posValue - 2.0f, position.y , position.z - 1.0f);
}

KochaEngine::Vector3 KochaEngine::JammingSpine::GetTagScale() const
{
	float posRate = (pWall->GetMinPos().x - position.x) - CUT_OFF_X;  //0�`SURVIVE_LENGTH�ɂȂ�͂�
	float scaleValue = Util::EaseOut(10.0f, 3.0f, posRate / SURVIVE_LENGTH);
	return Vector3(-1.5f, 1.0f, 1.0f) * scaleValue;
}

void KochaEngine::JammingSpine::SetDeadLength(const int arg_length)
{
	DEAD_X = arg_length;
	CUT_OFF_X = 10.0f;
	SURVIVE_LENGTH = DEAD_X - CUT_OFF_X;
}
