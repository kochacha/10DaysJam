#include "Player.h"
#include "GameObjectManager.h"
#include "Audio.h"
#include "GameSetting.h"
#include "ScoreManager.h"
#include "Util.h"
#include "InputManager.h"
#include "PauseManager.h"

KochaEngine::Player::Player(Camera* arg_camera, GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter, ScoreManager* arg_sManager, const Vector3& arg_position, bool* inGameFlag)
	:gManager(nullptr),
	 pEmitter(nullptr),
	 sManager(nullptr),
	 pManager(nullptr),
	 se(nullptr),
	 seVolume(0.1f),
	 scale(Vector3()),
	 endScale(Vector3()),
	 wayObj(nullptr),
	 isWayDraw(false),
	 wayRotate(0),
	 smashLine(nullptr),
	 isFinish(false),
	 isHitStop(false),
	 hitStopCount(0),
	 isOnce(false),
	 isStartOnce(false),
	 inGame(nullptr),
	 stackCount(0),
	 speed(0.0f),
	 isHitWall(false),
	 smashPower(0),
	 overDirveSmashPower(0),
	 asobiCount(0),
	 isSmashing(false),
	 backCount(0),
	 addSmashScore(0),
	 isStun(false)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	if (arg_pEmitter == nullptr) return;
	if (arg_sManager == nullptr) return;
	camera = arg_camera;
	gManager = arg_gManager;
	pEmitter = arg_pEmitter;
	sManager = arg_sManager;
	se = new Audio();

	//�\���p�I�u�W�F�N�g�̐���
	obj = new Object("plane");
	wayObj = new Object("plane");
	smashLine = new Object("plane");
	for (int i = 0; i < 10; i++)
	{
		powarGauge[i] = new Texture2D("Resources/normalGauge.png", Vector2(150 + 70 * i, 720), Vector2(65, 55), 0);
		emptyGauge[i] = new Texture2D("Resources/emptyGauge.png", Vector2(150 + 70 * i, 720), Vector2(65, 55), 0);
		if (i < 5)
		{
			overDriveGauge[i] = new Texture2D("Resources/gauge.png", Vector2(0, 720), Vector2(65, 55), 0);
		}
	}

	position = arg_position;
	this->inGame = inGameFlag;
	
	Initialize();
}

KochaEngine::Player::~Player()
{
	delete se;
	delete obj;
	delete wayObj;
	delete smashLine;
	for (int i = 0; i < 10; i++)
	{
		delete powarGauge[i];
		delete emptyGauge[i];
		if (i < 5)
		{
			delete overDriveGauge[i];
		}
	}
}

void KochaEngine::Player::Initialize()
{
	//�p�[�e�B�N��
	pEmitter->SpawnParticle(position);

	//SE
	se->Init();
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
	
	//�\���p
	scale = Vector3(0, 0, 10);
	endScale = Vector3(10, 10, 10);
	obj->SetPosition(position);
	obj->SetRotate(Vector3(0, 0, 0));
	obj->SetScale(scale);
	obj->SetTexture("Resources/player0.png");
	obj->SetBillboardType(KochaEngine::Object::BILLBOARD);
	wayObj->SetPosition(Vector3(position.x, position.y, position.z + 0.1f));
	wayObj->SetRotate(Vector3(0, 0, 0));
	wayObj->SetScale(Vector3(30, 30, 1));
	wayObj->SetTexture("Resources/way.png");
	wayObj->SetBillboardType(KochaEngine::Object::BILLBOARD);
	isWayDraw = false;
	wayRotate = 0;
	smashLine->SetPosition(Vector3(position.x - 90, position.y, position.z + 0.1f));
	smashLine->SetRotate(Vector3(0, 0, 0));
	smashLine->SetScale(Vector3(180, 4, 1));
	smashLine->SetTexture("Resources/smashLine.png");
	smashLine->SetBillboardType(KochaEngine::Object::BILLBOARD);
	isAlpha = true;

	isFinish = false;
	isHitStop = false;
	hitStopCount = 0;
	isOnce = false;
	stackCount = 30;
	speed = 0.5f;
	isHitWall = false;
	isStartOnce = false;

	ResetPower();
	asobiCount = 0;
	isSmashing = false;
	backCount = 0;
	addSmashScore = 0;

	isStun = false;	

	velocity.Zero();
	sphere.radius = 4.0f;
	sphere.position = this->position;
}

void KochaEngine::Player::Update()
{
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
	
	//�v���C���[�o����
	if (!isStartOnce)
	{
		isStartOnce = true;
		se->PlayWave("Resources/Sound/spawn.wav", seVolume / 2.0f);
	}

	//��������Ă���̐��t���[����InputMove()��ʂ��Ȃ�
	if (stackCount > 0)
	{
		stackCount--;
	}

	if (isFinish)
	{
		//�v���C�I����A���߂Ēʂ����Ȃ�
		if (!isOnce)
		{
			isOnce = true;
			se->PlayWave("Resources/Sound/finish.wav", seVolume);
			pEmitter->DeadParticle(position);
		}
	}
	else
	{
		//���͑O����
		PrepareInput();
		//���͏���
		InputForMove();
		//���͌㏈��
		Move();
		//���W�ړ�����
		UpdatePosition();
		//���W�ړ��㏈��
		ProcessingAfterUpdatePosition();
	}

	ScaleAnimation();
	SetObjParam();

	//�f�o�b�O�o�͗p
	/*wchar_t str[256];
	swprintf_s(str, L"smashPower %d\n", smashPower);
	OutputDebugString(str);

	swprintf_s(str, L"backCount %d\n", backCount);
	OutputDebugString(str);*/
}

void KochaEngine::Player::Hit()
{
	//�����ɓ��������������ʂ�
}

void KochaEngine::Player::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	obj->Draw(arg_camera, arg_lightManager);

	//�v���C���I����Ă���΁A�ȍ~��\�����Ȃ�
	if (isFinish) return;

	if (isWayDraw && GameSetting::isDashData)
	{
		wayObj->Draw(arg_camera, arg_lightManager);
	}

	if (GameSetting::isSmashData)
	{
		smashLine->Draw(arg_camera, arg_lightManager);
	}	
}

void KochaEngine::Player::SpriteDraw()
{
	for (int i = 0; i < MAX_SMASHPOWER; i++)
	{
		emptyGauge[i]->Draw();
	}
	for (int i = 0; i < smashPower; i++)
	{
		powarGauge[i]->Draw();
	}

	for (int i = 0; i < overDirveSmashPower; i++)
	{
		overDriveGauge[i]->Draw(Vector2(150 + ((70 * smashPower) + (70 * i)), 720));
	}
}

void KochaEngine::Player::ShowGUI()
{
	float _position[3] = { position.x, position.y, position.z };
	ImGui::InputFloat3("##PlayerPosition", _position, "%f");
	ImGui::InputInt("##PlayerBackCount", &backCount);
}

KochaEngine::GameObjectType KochaEngine::Player::GetType()
{
	return GameObjectType::PLAYER;
}

void KochaEngine::Player::PowerUp(const GameObjectType arg_objectType)
{
	//�A�C�e���擾��
	if (arg_objectType == KochaEngine::GameObjectType::ENHANCEMENT_ITEM)
	{
		if (*inGame)
		{
			sManager->AddScore(1500);
			if (GameSetting::isScoreData)
			{
				pEmitter->HitScore(position, false);
			}
		}

		//SE�Đ�
		if (smashPower >= MAX_SMASHPOWER)
		{
			se->PlayWave("Resources/Sound/powerMax.wav", seVolume);
		}
		else
		{
			se->PlayWave("Resources/Sound/item.wav", seVolume);
		}

		//�X�}�b�V���p���[���Z
		if (smashPower < MAX_SMASHPOWER)
		{
			pEmitter->PowerUp(position);
			smashPower++;
		}
	}
	else if (arg_objectType == KochaEngine::GameObjectType::JAMMING_SPINE)
	{
		if (*inGame)
		{
			sManager->AddScore(7777);
			if (GameSetting::isScoreData)
			{
				pEmitter->HitScore(position, true);
			}
		}

		//�T�E���h�Đ�
		se->PlayWave("Resources/Sound/overDrive.wav", seVolume);

		//�I�[�o�[�h���C���X�}�b�V���p���[���Z
		if (overDirveSmashPower < MAX_OVERDRIVE)
		{
			//�q�b�g�X�g�b�v
			isHitStop = true;
			hitStopCount = 20;

			pEmitter->PowerUp(position);
			overDirveSmashPower += 1;
		}		
	}
}

void KochaEngine::Player::PowerDown()
{
	CommonVib(10);
	se->PlayWave("Resources/Sound/toge.wav", seVolume);

	//�X�}�b�V���p���[���Z
	pEmitter->PowerDown(position);
	smashPower--;
	if (smashPower < 0)
	{
		smashPower = 0;
	}

	velocity.x = -velocity.x;
	velocity.y = -velocity.y;
	speed = 6.0f;
	isStun = true;
}

const bool KochaEngine::Player::IsFinish()
{
	return isFinish;
}

void KochaEngine::Player::Finish()
{
	isFinish = true;
}

const bool KochaEngine::Player::IsHitStop()
{
	return isHitStop;
}

void KochaEngine::Player::HitStopTimer()
{
	if (hitStopCount > 0)
	{
		hitStopCount--;
	}
	else
	{
		isHitStop = false;
	}
}

const bool KochaEngine::Player::IsInGame()
{
	return *inGame;
}

void KochaEngine::Player::SetPauseManager(PauseManager* arg_pManager)
{
	if (arg_pManager == nullptr) return;
	pManager = arg_pManager;
}

const bool KochaEngine::Player::IsSmashing()
{
	return isSmashing;
}

const bool KochaEngine::Player::IsStuning()
{
	return isStun;
}

const int KochaEngine::Player::GetBackCount()
{
	return backCount;
}

const bool KochaEngine::Player::IsHitWall()
{
	return isHitWall;
}

void KochaEngine::Player::PrepareInput()
{
	//��������Ă���̐��t���[����InputMove()��ʂ��Ȃ�
	if (stackCount > 0)
	{
		stackCount--;
	}

	//�ʏ�ړ����A�m�b�N�o�b�N���ɃX�s�[�h������������
	if (!isSmashing && backCount <= 0)
	{
		speed -= 1.0f;
		if (speed <= 0)
		{
			speed = 0;
		}
	}

	//�X�^�����Ă���Ƃ�
	if (isStun)
	{
		//���x��0�ȉ��Ȃ�
		if (speed <= 0)
		{
			//�X�^����ԏI��
			isStun = false;
		}
	}

	//�X�}�b�V������̗P�\�t���[���v��
	if (asobiCount > 0)
	{
		asobiCount--;
	}
}

void KochaEngine::Player::InputForMove()
{
	//��������͑���ł��Ȃ�
	if (stackCount > 0) return;
	//�v���C�I����͑���ł��Ȃ�
	if (isFinish) return;

	if (!isSmashing)
	{
		//�ʏ�ړ�
		if (backCount <= 0 && !isStun && InputManager::DashKey())
		{	
			velocity.Zero();
			velocity.x = Input::GetLStickDirection().x;
			velocity.y = Input::GetLStickDirection().y;
			if (velocity.x != 0 || velocity.y != 0)
			{
				speed = 6.5f;
				scale = Vector3(3, 3, 10);
				se->PlayWave("Resources/Sound/dash.wav", seVolume);
			}
		}
	}

	//�X�}�b�V���{�^���������ꂽ��
	if (InputManager::SmashKey())
	{
		//���s�P�\���Ԃ����Z
		asobiCount = 7;
	}
	//�~�܂��Ă��āA�X�}�b�V�����łȂ��Ȃ�
	if (speed <= 0 && (!isSmashing || backCount > 0))
	{
		//���s�P�\�t���[�����ŁA�X�}�b�V������p���[�������
		if (asobiCount > 0 && smashPower > 0)
		{
			CommonVib(10);
			se->PlayWave("Resources/Sound/smash.wav", seVolume);
			//�X�}�b�V���J�n
			isSmashing = true;
			//����
			velocity.Zero();
			velocity.x = -1;

			speed = 10;
			//�P�\���ԃ��Z�b�g
			asobiCount = 0;
		}
	}

	//�Փ˔���f�o�b�O�p
	/*velocity.Zero();
	if (Input::CheckKey(DIK_LSHIFT))
	{
		speed = 0.8f;
	}
	else
	{
		speed = 0.5f;
	}
	if (Input::CheckKey(DIK_W))
	{
		velocity.y = 1;
	}
	if (Input::CheckKey(DIK_S))
	{
		velocity.y = -1;
	}
	if (Input::CheckKey(DIK_A))
	{
		velocity.x = -1;
	}
	if (Input::CheckKey(DIK_D))
	{
		velocity.x = 1;
	}*/
}

void KochaEngine::Player::Move()
{
	//�X�}�b�V�����ĕǂɓ�����܂ł̊�
	if (isSmashing)
	{
		int wallPosX = gManager->GetWall()->GetMinPos().x;
		if (position.x <= wallPosX)
		{
			isSmashing = false;
			position.x = wallPosX;
			if (*inGame)
			{
				backCount = (smashPower * 5.0f) + (overDirveSmashPower * 15.0f); //�����̔{���Ŗ߂�ʂ��ς��
			}
			else
			{
				backCount = 10000;
			}

			isHitWall = true;

			//���u��
			ResetPower();
		}
	}

	//�ǉ����߂��������c���Ă���Ȃ�
	if (backCount > 0)
	{
		scale = Vector3(12, 4, 10);
		addSmashScore++;
		backCount--;
		pEmitter->SmashStar(position);
		ResetPower();

		//�Q�[�����Ȃ�
		if (*inGame)
		{
			//�X�R�A���Z
			sManager->AddScore(addSmashScore * 300);
			if (GameSetting::isScoreData)
			{
				pEmitter->SmashScore(position);
			}
		}
	}

	velocity.normalize();

	//�ړ����Ȃ�
	if (speed > 0.0f)
	{
		pEmitter->MoveParticle(Vector3(position.x, position.y, position.z + 1.0f));
	}
	//�P�\�t���[���𒴂����̂ŁA�X�}�b�V���s��
	if (asobiCount == 1)
	{
		se->PlayWave("Resources/Sound/feild.wav", seVolume);
	}
}

void KochaEngine::Player::UpdatePosition()
{
	MoveX();
	MoveY();
}

void KochaEngine::Player::MoveX()
{
	position.x += velocity.x * speed;

	int leftWall = gManager->GetWall()->GetMinPos().x;
	int rightWall = gManager->GetWall()->GetMaxPos().x;
	
	if (position.x <= leftWall)
	{
		if (backCount <= 0)
		{
			position.x = leftWall;
		}
		//�ǂ������Ă�悤�Ɍ�����
		else
		{
			position.x = leftWall + 20;
		}
	}

	if (position.x >= rightWall)
	{
		position.x = rightWall;
	}
}

void KochaEngine::Player::MoveY()
{
	position.y += velocity.y * speed;

	int DownWall = gManager->GetWall()->GetMinPos().y;
	int UpWall = gManager->GetWall()->GetMaxPos().y;

	if (position.y <= DownWall)
	{
		position.y = DownWall;
	}
	if (position.y >= UpWall)
	{
		position.y = UpWall;
	}
}

void KochaEngine::Player::ProcessingAfterUpdatePosition()
{
	//�����߂����łȂ�,Wall��荶�ɂ����
	if (backCount <= 0 && position.x > gManager->GetWall()->GetMinPos().x)
	{
		//�ǂɐڐG���Ă��Ȃ�
		isHitWall = false;
	}

	//�ǂ̎��̂���ԍ��܂ŗ�����
	if (gManager->GetWall()->GetMinPos().x <= gManager->GetWall()->GetLimitLeftPosX())
	{
		//�����߂������������I��0�ɂ���
		backCount = 0;
	}
	//�X�}�b�V�����ɉ����߂�������0�ɂȂ�����
	if (addSmashScore != 0 && backCount == 0)
	{
		//�X�}�b�V�����I������
		CommonVib(20);
		addSmashScore = 0;
		camera->SetShake(1.00f);
		se->PlayWave("Resources/Sound/hit.wav", seVolume);
		scale = Vector3(1, 20, 10);
	}
}

void KochaEngine::Player::ScaleAnimation()
{
	if (isFinish)
	{
		endScale = Vector3(0, 0, 0);
	}
	scale.x = Util::EaseIn(scale.x, endScale.x, 0.4f);
	scale.y = Util::EaseIn(scale.y, endScale.y, 0.4f);
}

void KochaEngine::Player::SetObjParam()
{
	sphere.position = this->position;
	obj->SetPosition(position);
	if (velocity.x >= 0)
	{
		obj->SetScale(Vector3(-scale.x, scale.y, scale.z));
	}
	else
	{
		obj->SetScale(scale);
	}

	Vector3 inputVec = Vector3(Input::GetLStickDirection().x, Input::GetLStickDirection().y, 0);
	if (inputVec.x != 0 || inputVec.y != 0)
	{
		isWayDraw = true;
	}
	else
	{
		isWayDraw = false;
	}
	wayRotate = 360 - Util::Vector2ToAngle(inputVec);
	wayObj->SetPosition(Vector3(position.x, position.y, position.z + 0.1f));
	wayObj->SetRotate(Vector3(0, 0, wayRotate));

	smashLine->SetPosition(Vector3(position.x - 90, position.y, position.z + 0.1f));
}

void KochaEngine::Player::CommonVib(const int arg_time)
{
	if (!GameSetting::isVibData) return;

	Input::Vibration(60000, arg_time);
}

void KochaEngine::Player::ResetPower()
{
	smashPower = 0;
	overDirveSmashPower = 0;
}