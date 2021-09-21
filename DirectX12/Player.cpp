#include "Player.h"
#include "GameObjectManager.h"
#include "Audio.h"
#include "GameSetting.h"
#include "ScoreManager.h"
#include "Util.h"
#include "InputManager.h"
#include "PauseManager.h"

KochaEngine::Player::Player(Camera* arg_camera, GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter, ScoreManager* arg_sManager, const Vector3& arg_position, bool* inGameFlag)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	if (arg_pEmitter == nullptr) return;
	if (arg_sManager == nullptr) return;
	camera = arg_camera;
	gManager = arg_gManager;
	pEmitter = arg_pEmitter;
	sManager = arg_sManager;
	position = arg_position;
	this->inGame = inGameFlag;

	obj = new Object("plane");
	wayObj = new Object("plane");
	smashLine = new Object("plane");
	se = new Audio();
	for (int i = 0; i < 10; i++)
	{
		powarGauge[i] = new Texture2D("Resources/normalGauge.png", Vector2(150 + 70 * i, 720), Vector2(65, 55), 0);
		emptyGauge[i] = new Texture2D("Resources/emptyGauge.png", Vector2(150 + 70 * i, 720), Vector2(65, 55), 0);
		if (i < 5)
		{
			overDriveGauge[i] = new Texture2D("Resources/gauge.png", Vector2(0, 720), Vector2(65, 55), 0);
		}
		
	}
	Initialize();
}

KochaEngine::Player::~Player()
{
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
	delete se;
}

void KochaEngine::Player::Initialize()
{
	isAlpha = true;

	velocity.Zero();
	speed = 0.5f;
	wayRotate = 0;
	asobiCount = 0;
	isWayDraw = false;
	smash = false;
	isStun = false;
	stunCount = 0;
	backCount = 0;
	addSmashScore = 0;
	hitWall = false;
	isHitStop = false;
	ResetPower();
	

	sphere.radius = 4.0f;
	sphere.position = this->position;

	scale = Vector3(10, 10, 10);
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

	se->Init();
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
}

void KochaEngine::Player::Update()
{
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);

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

		if (gManager->GetWall()->GetMinPos().x <= gManager->GetWall()->GetLimitLeftPos())
		{
			backCount = 0;			
		}
		
		if (backCount == 0)
		{
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
	MoveWallPos();
	InputMove();
	MoveX();
	MoveY();

	ScaleAnimation();
	SetObjParam();

	wchar_t str[256];
	swprintf_s(str, L"smashPower %d\n", smashPower);
	OutputDebugString(str);

	swprintf_s(str, L"backCount %d\n", backCount);
	OutputDebugString(str);
	
}

void KochaEngine::Player::Hit()
{
	//何かに当たった時ここ通る
}

void KochaEngine::Player::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	if (isWayDraw && pManager->IsDisplayDash())
	{
		wayObj->Draw(arg_camera, arg_lightManager);
	}

	if (pManager->IsDisplaySmash())
	{
		smashLine->Draw(arg_camera, arg_lightManager);
	}


	obj->Draw(arg_camera, arg_lightManager);

	/*ImGui::Begin("smashPower");
	ImGui::Text("smashPowar %f", smashPower);
	ImGui::End();*/
}

void KochaEngine::Player::ShowGUI()
{
	float _position[3] = { position.x, position.y, position.z };
	ImGui::InputFloat3("##PlayerPosition", _position, "%f");
	ImGui::InputInt("##PlayerBackCount", &backCount);
}

KochaEngine::GameObjectType KochaEngine::Player::GetType()
{
	return PLAYER;
}

const bool KochaEngine::Player::IsSmashing()
{
	return smash;
}

const bool KochaEngine::Player::IsStuning()
{
	return isStun;
}

const bool KochaEngine::Player::IsFinishSmash()
{
	Wall* pWall = gManager->GetWall();

	if (backCount != 1) return false;
	if (position.x < pWall->GetMinPos().x) return false;
	if (!hitWall) return false;

	return true;
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
		se->PlayWave("Resources/Sound/item.wav", seVolume);
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
		se->PlayWave("Resources/Sound/overDrive.wav", seVolume);
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
		isHitStop = true;
		hitStopCount = 20;
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
	se->PlayWave("Resources/Sound/toge.wav", seVolume);
	pEmitter->PowerDown(position);
	smashPower -= 3;
	if (smashPower < 0)
	{
		smashPower = 0;
	}
	velocity.x = -velocity.x;
	velocity.y = -velocity.y;
	speed = 6.0f;
	isStun = true;
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

void KochaEngine::Player::SetPauseManager(PauseManager* arg_pManager)
{
	if (arg_pManager == nullptr) return;
	pManager = arg_pManager;
}

const int KochaEngine::Player::GetBackCount()
{
	return backCount;
}

const bool KochaEngine::Player::IsHitWall()
{
	return hitWall;
}

const bool KochaEngine::Player::IsInGame()
{
	return *inGame;
}

void KochaEngine::Player::InputMove()
{	
	if (smash)
	{		
		//Input::Vibration(60000, 10);
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
		//動いていたら
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

void KochaEngine::Player::MoveWallPos()
{

}

void KochaEngine::Player::ResetPower()
{
	smashPower = 0;
	overDirveSmashPower = 0;
}