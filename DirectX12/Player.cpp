#include "Player.h"
#include "GameObjectManager.h"



KochaEngine::Player::Player(Camera* arg_camera, GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter, const Vector3& arg_position)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	if (arg_pEmitter == nullptr) return;
	camera = arg_camera;
	gManager = arg_gManager;
	pEmitter = arg_pEmitter;
	position = arg_position;

	obj = new Object("plane");
	for (int i = 0; i < 10; i++)
	{
		powarGauge[i] = new Texture2D("Resources/white.png", Vector2(150 + 70 * i, 720), Vector2(40, 40), 45);
		if (i < 5)
		{
			overDriveGauge[i] = new Texture2D("Resources/black.png", Vector2(0, 720), Vector2(40, 40), 45);
		}
		
	}
	Initialize();
}

KochaEngine::Player::~Player()
{
	delete obj;
	for (int i = 0; i < 10; i++)
	{
		delete powarGauge[i];
		if (i < 5)
		{
			delete overDriveGauge[i];
		}
	}
}

void KochaEngine::Player::Initialize()
{
	isAlpha = true;

	velocity.Zero();
	speed = 0.5f;
	smash = false;
	isStun = false;
	stunCount = 0;
	backCount = 0;
	hitWall = false;
	ResetPower();
	

	sphere.radius = 4.0f;
	sphere.position = this->position;

	obj->SetPosition(position);
	obj->SetRotate(Vector3(0, 0, 0));
	obj->SetScale(Vector3(10, 10, 10));
	obj->SetTexture("Resources/player0.png");
	obj->SetBillboardType(KochaEngine::Object::BILLBOARD);
}

void KochaEngine::Player::Update()
{
	gManager->HitObject(this, COLLISION_BLOCK);

	if (backCount > 0)
	{
		backCount--;
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
	obj->Draw(arg_camera, arg_lightManager);

	/*ImGui::Begin("smashPower");
	ImGui::Text("smashPowar %f", smashPower);
	ImGui::End();*/
}

void KochaEngine::Player::ShowGUI()
{
	float _position[3] = { position.x, position.y, position.z };
	ImGui::InputFloat3("##PlayerPosition", _position, "%f");
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
	if (backCount != 1) return false;
	if (position.x < gManager->GetWall()->GetMinPos().x) return false;
	if (!hitWall) return false;

	return true;
}

void KochaEngine::Player::SpriteDraw()
{
	for (int i = 0; i < smashPower; i++)
	{
		powarGauge[i]->Draw();
	}

	for (int i = 0; i < overDirveSmashPower; i++)
	{
		overDriveGauge[i]->Draw(Vector2(150 + 70 * smashPower + 80 * i, 720));
	}
}

void KochaEngine::Player::PowerUp(const GameObjectType arg_objectType)
{
	//上限値ならreturn
	if (smashPower >= MAX_SMASHPOWER) return;

	//強化アイテム取得時
	if (arg_objectType == KochaEngine::GameObjectType::ENHANCEMENT_ITEM)
	{
		smashPower++;
	}
	//スマッシュでおじゃまトゲを巻き込んだ時
	else if (arg_objectType == KochaEngine::GameObjectType::JAMMING_SPINE)
	{
		overDirveSmashPower += 1;

		if (overDirveSmashPower > MAX_OVERDRIVE)
		{
			overDirveSmashPower = MAX_OVERDRIVE;
		}
	}
}

void KochaEngine::Player::PowerDown()
{
	smashPower -= 3;
	velocity.x = -velocity.x;
	velocity.y = -velocity.y;
	speed = 6.0f;
	isStun = true;
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
	
	/*if (Input::CheckKey(DIK_LSHIFT))
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
	if (smash)
	{		
		Input::Vibration(60000, 10);
		int wallPosX = gManager->GetWall()->GetMinPos().x;
		if (position.x <= wallPosX )
		{
			smash = false;			
			position.x = wallPosX;
			backCount = (smashPower * 7.0f) + (overDirveSmashPower * 5.0f); //ここの倍率で戻る量が変わる
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
		
		
		if (backCount <= 0 && !isStun && Input::TriggerPadButton(XINPUT_GAMEPAD_A))
		{
			velocity.Zero();
			speed = 8.0f;

			velocity.x = Input::GetLStickDirection().x;
			velocity.y = Input::GetLStickDirection().y;
		}
	}	
	if (speed <= 0 && !smash)
	{
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_B))
		{
			smash = true;		
			velocity.Zero();
			velocity.x = -1;
			speed = 10;
		}
	}
	velocity.normalize();	

	if (velocity.x != 0 || velocity.y != 0)
	{
		//動いていたら
		pEmitter->MoveParticle(Vector3(position.x, position.y, position.z + 1.0f));
	}
}

void KochaEngine::Player::MoveX()
{
	position.x += velocity.x * speed;
	int leftWall = gManager->GetWall()->GetMinPos().x;
	int rightWall = gManager->GetWall()->GetMaxPos().x; //112
	if (position.x <= leftWall)
	{
		position.x = leftWall;
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

void KochaEngine::Player::SetObjParam()
{
	sphere.position = this->position;
	obj->SetPosition(position);
}

void KochaEngine::Player::MoveWallPos()
{

}

void KochaEngine::Player::ResetPower()
{
	smashPower = 0;
	overDirveSmashPower = 0;
}