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
	 smashLine(nullptr),
	 isFinish(false),
	 isHitStop(false),
	 hitStopCount(0),
	 isOnce(false),
	 inGame(nullptr)
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

	//表示用オブジェクトの生成
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
	//パーティクル
	pEmitter->SpawnParticle(position);

	//SE
	se->Init();
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
	
	//表示用
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

	speed = 0.5f;
	wayRotate = 0;
	asobiCount = 0;
	isWayDraw = false;
	smash = false;
	isStun = false;
	stunCount = 0;
	backCount = 0;
	addSmashScore = 0;
	stackCount = 30;
	hitWall = false;
	ResetPower();

	velocity.Zero();
	sphere.radius = 4.0f;
	sphere.position = this->position;
}

void KochaEngine::Player::Update()
{
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
	if(stackCount > 0) stackCount--;

	if (isFinish)
	{
		if (!isOnce)
		{
			isOnce = true;
			se->PlayWave("Resources/Sound/finish.wav", seVolume);
			pEmitter->DeadParticle(position);
		}
	}

	if (asobiCount > 0) asobiCount--;

	if (backCount > 0)
	{
		scale = Vector3(12, 4, 10);
		addSmashScore++;
		backCount--;
		pEmitter->SmashStar(position);
		if (*inGame)
		{
			sManager->AddScore(addSmashScore * 30);
			if (pManager->IsDisplayScore())
			{
				pEmitter->SmashScore(position);
			}
		}

		if (gManager->GetWall()->GetMinPos().x <= gManager->GetWall()->GetLimitLeftPosX())
		{
			backCount = 0;			
		}
		
		if (backCount == 0)
		{
			CommonVib(20);
			addSmashScore = 0;
			camera->SetShake(1.00f);
			se->PlayWave("Resources/Sound/hit.wav", seVolume);
			scale = Vector3(1,20,10);
		}
	}
	if (backCount <= 0 && position.x >= gManager->GetWall()->GetMinPos().x)
	{
		hitWall = false;
	}
	/*if (backCount > 0 && position.x >= gManager->GetWall()->GetMinPos().x)
	{
		speed = 0;
	}*/
	else
	{
		
	}

	
	
	if (isStun)
	{
		if (speed <= 0)
		{
			isStun = false;
		}
		/*if (stunCount >= stunTime)
		{
			isStun = false;
			stunCount = 0;
		}
		stunCount++;*/
	}
	
	InputMove();
	MoveX();
	MoveY();

	ScaleAnimation();
	SetObjParam();

	//デバッグ出力用
	/*wchar_t str[256];
	swprintf_s(str, L"smashPower %d\n", smashPower);
	OutputDebugString(str);

	swprintf_s(str, L"backCount %d\n", backCount);
	OutputDebugString(str);*/
}

void KochaEngine::Player::Hit()
{
	//何かに当たった時ここ通る
}

void KochaEngine::Player::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	obj->Draw(arg_camera, arg_lightManager);

	if (isFinish) return;

	if (isWayDraw && pManager->IsDisplayDash())
	{
		wayObj->Draw(arg_camera, arg_lightManager);
	}

	if (pManager->IsDisplaySmash())
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
	if (arg_objectType == KochaEngine::GameObjectType::ENHANCEMENT_ITEM)
	{
		if (*inGame)
		{
			sManager->AddScore(1000);
			if (pManager->IsDisplayScore())
			{
				pEmitter->HitScore(position, false);
			}
		}
		if (smashPower >= MAX_SMASHPOWER)
		{
			se->PlayWave("Resources/Sound/powerMax.wav", seVolume);
		}
		else
		{
			se->PlayWave("Resources/Sound/item.wav", seVolume);
		}
	}
	else if (arg_objectType == KochaEngine::GameObjectType::JAMMING_SPINE)
	{
		if (*inGame)
		{
			sManager->AddScore(5000);
			if (pManager->IsDisplayScore())
			{
				pEmitter->HitScore(position, true);
			}
		}
		if (smashPower >= MAX_SMASHPOWER)
		{
			se->PlayWave("Resources/Sound/powerMax.wav", seVolume);
		}
		else
		{
			se->PlayWave("Resources/Sound/overDrive.wav", seVolume);
		}
		isHitStop = true;
		hitStopCount = 20;
	}

	//上限値ならreturn
	if (smashPower >= MAX_SMASHPOWER) return;

	//強化アイテム取得時
	if (arg_objectType == KochaEngine::GameObjectType::ENHANCEMENT_ITEM)
	{
		pEmitter->PowerUp(position);
		smashPower++;
	}
	//スマッシュでおじゃまトゲを巻き込んだ時
	else if (arg_objectType == KochaEngine::GameObjectType::JAMMING_SPINE)
	{
		pEmitter->PowerUp(position);
		overDirveSmashPower += 1;

		if (overDirveSmashPower > MAX_OVERDRIVE)
		{
			overDirveSmashPower = MAX_OVERDRIVE;
		}
	}
}

void KochaEngine::Player::PowerDown()
{
	CommonVib(10);
	se->PlayWave("Resources/Sound/toge.wav", seVolume);
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
	return smash;
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
	return hitWall;
}

void KochaEngine::Player::InputMove()
{	
	if (isFinish || stackCount > 0) return;

	if (smash)
	{		
		int wallPosX = gManager->GetWall()->GetMinPos().x;
		if (position.x <= wallPosX )
		{
			smash = false;
			position.x = wallPosX;
			if (*inGame)
			{
				backCount = (smashPower * 5.0f) + (overDirveSmashPower * 10.0f); //ここの倍率で戻る量が変わる
			}
			else
			{
				backCount = 10000;
			}
			
			hitWall = true;
			
			//仮置き
			ResetPower();
		
		}
	}
	if (!smash)
	{
		if (backCount <= 0)
		{
			speed -= 1.0f;
			if (speed <= 0)
			{
				speed = 0;
			}
		}
		
		
		if (backCount <= 0 && !isStun && InputManager::DashKey())
		{	
			velocity.Zero();
			speed = 6.5f;
			velocity.x = Input::GetLStickDirection().x;
			velocity.y = Input::GetLStickDirection().y;
			if (velocity.x != 0 || velocity.y != 0)
			{
				scale = Vector3(3, 3, 10);
				se->PlayWave("Resources/Sound/dash.wav", seVolume);
			}
		}
	}	

	if (InputManager::SmashKey())
	{
		asobiCount = 7;
	}
	if (speed <= 0 && !smash)
	{
		if (asobiCount != 0 && smashPower > 0)
		{
			CommonVib(10);
			se->PlayWave("Resources/Sound/smash.wav", seVolume);
			smash = true;		
			velocity.Zero();
			velocity.x = -1;
			speed = 10;
		}
		else if (asobiCount == 1)
		{
			se->PlayWave("Resources/Sound/feild.wav", seVolume);
		}
	}
	velocity.normalize();	

	if (velocity.x != 0 || velocity.y != 0)
	{
		pEmitter->MoveParticle(Vector3(position.x, position.y, position.z + 1.0f));
	}

	//衝突判定デバッグ用
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

void KochaEngine::Player::MoveX()
{
	position.x += velocity.x * speed;
	int leftWall = gManager->GetWall()->GetMinPos().x;
	int rightWall = gManager->GetWall()->GetMaxPos().x; //112
	
	if (position.x <= leftWall)
	{
		if (backCount <= 0)
		{
			position.x = leftWall;
		}
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
	int UpWall = gManager->GetWall()->GetMaxPos().y; //112
	if (position.y <= DownWall)
	{
		position.y = DownWall;
	}
	if (position.y >= UpWall)
	{
		position.y = UpWall;
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
	if (!pManager->IsVib()) return;

	Input::Vibration(60000, arg_time);
}

void KochaEngine::Player::ResetPower()
{
	smashPower = 0;
	overDirveSmashPower = 0;
}