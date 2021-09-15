#include "Player.h"
#include "GameObjectManager.h"

KochaEngine::Player::Player(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	camera = arg_camera;
	gManager = arg_gManager;
	position = arg_position;

	obj = new Object("plane");
	Initialize();
}

KochaEngine::Player::~Player()
{
	delete obj;
}

void KochaEngine::Player::Initialize()
{
	isAlpha = true;

	velocity.Zero();
	speed = 0.5f;
	smash = false;
	isStun = false;
	stunCount = 0;
	ResetPower();

	wallPosX = -112;
	

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

	SetObjParam();
}

void KochaEngine::Player::Hit()
{
	//何かに当たった時ここ通る
}

void KochaEngine::Player::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	obj->Draw(arg_camera, arg_lightManager);
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
		smashPower += 3;

		if (smashPower > MAX_SMASHPOWER)
		{
			smashPower = MAX_SMASHPOWER;
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
		if (position.x <= wallPosX )
		{
			smash = false;
			speed = 0;
			position.x = wallPosX;

			//仮置き
			ResetPower();
		}
	}
	if (!smash)
	{
		speed -= 1.0f;
		if (speed <= 0)
		{
			speed = 0;
		}
		
		if (!isStun && Input::TriggerPadButton(XINPUT_GAMEPAD_A))
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
}

void KochaEngine::Player::MoveX()
{
	position.x += velocity.x * speed;
	if (position.x <= wallPosX)
	{
		position.x = wallPosX;
	}
	if (position.x >= 112)
	{
		position.x = 112;
	}
}

void KochaEngine::Player::MoveY()
{
	position.y += velocity.y * speed;
	if (position.y <= -55)
	{
		position.y = -55;
	}
	if (position.y >= 41)
	{
		position.y = 41;
	}
}

void KochaEngine::Player::SetObjParam()
{
	sphere.position = this->position;
	obj->SetPosition(position);
}

void KochaEngine::Player::ResetPower()
{
	smashPower = 0;
}