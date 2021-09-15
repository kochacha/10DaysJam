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
	

	wallPosX = -118;

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
	
	InputMove();
	MoveX();
	MoveY();

	SetObjParam();
}

void KochaEngine::Player::Hit()
{
	//‰½‚©‚É“–‚½‚Á‚½Žž‚±‚±’Ê‚é
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
		}
	}
	if (!smash)
	{
		speed -= 1.0f;
		if (speed <= 0)
		{
			speed = 0;
		}
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
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
	if (position.x <= -118)
	{
		position.x = -118;
	}
	if (position.x >= 120)
	{
		position.x = 120;
	}
}

void KochaEngine::Player::MoveY()
{
	position.y += velocity.y * speed;
	if (position.y <= -63)
	{
		position.y = -63;
	}
	if (position.y >= 63)
	{
		position.y = 63;
	}
}

void KochaEngine::Player::SetObjParam()
{
	sphere.position = this->position;
	obj->SetPosition(position);
}