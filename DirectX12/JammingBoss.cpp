#include "JammingBoss.h"

#include "GameObjectManager.h"
#include "ItemManager.h"
#include "Wall.h"
#include "Util.h"

void KochaEngine::JammingBoss::SetObjParam()
{
	sphere.position = this->position;
	obj->SetPosition(position);
}

KochaEngine::JammingBoss::JammingBoss(Camera* arg_camera, GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter,const Vector3& arg_position, ItemManager* arg_iManager)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	if (arg_pEmitter == nullptr) return;
	if (arg_iManager == nullptr) return;

	camera = arg_camera;
	gManager = arg_gManager;
	iManager = arg_iManager;
	pWall = gManager->GetWall();

	obj = new Object("plane");
	Initialize();

}

KochaEngine::JammingBoss::~JammingBoss()
{
	if (obj)
	{
		delete obj;
	}
}

void KochaEngine::JammingBoss::Initialize()
{
	isAlpha = true;
	isOnce = false;
	isSpawnEnd = false;

	velocity.Zero();

	sphere.radius = 6.0f;

	spawnCount = 120;

	SetObjParam();

	obj->SetRotate(Vector3(0, 0, 180));
	obj->SetScale(Vector3(10, 25, 10));
	obj->SetTexture("Resources/boss.png");
	obj->SetBillboardType(KochaEngine::Object::BILLBOARD);

	float leftWall = gManager->GetWall()->GetMinPos().x;
	float rightWall = gManager->GetWall()->GetMaxPos().x;


	if (gManager->GetPlayer()->GetBackCount() <= 0)
	{
		position.x = leftWall - 2;
	}
	//壁を押してるように見せる
	else
	{
		position.x = leftWall + 20;
	}

	SetObjParam();
}

void KochaEngine::JammingBoss::Update()
{
	if (!isSpawnEnd)
	{
		Spawn();
	}
	else
	{
		if (isFinish)
		{
			if (!isOnce)
			{
				isOnce = true;
				isDelete = true;
			}
			return;
		}

		float leftWall = gManager->GetWall()->GetMinPos().x;
		float rightWall = gManager->GetWall()->GetMaxPos().x;


		if (gManager->GetPlayer()->GetBackCount() <= 0)
		{
			position.x = leftWall - 2;
		}
		//壁を押してるように見せる
		else
		{
			position.x = leftWall + 20;
		}

		SetObjParam();

		gManager->HitObject(this, PLAYER);

		//プレイヤーが一番左まで到達した時(ボスが死ぬ時)
		if (gManager->GetPlayer()->IsLeftLimit())
		{
			Dead();
		}
	}
	
}

void KochaEngine::JammingBoss::Hit()
{
	auto player = gManager->GetPlayer();
	//通常時なら
	if (player->IsSmashing())
	{
		//プレイヤーはパワーダウン
		player->SuspendSmash();
	}
	else
	{
		player->HitJammingBoss();
	}
}

void KochaEngine::JammingBoss::Dead()
{
	isFinish = true;
}

void KochaEngine::JammingBoss::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	obj->Draw(arg_camera, arg_lightManager);
}

KochaEngine::GameObjectType KochaEngine::JammingBoss::GetType()
{
	return JAMMING_BOSS;
}

KochaEngine::Vector3 KochaEngine::JammingBoss::GetPrearrangedPosition() const
{
	return Vector3();
}

void KochaEngine::JammingBoss::Spawn()
{
	//登場演出処理ここに書く
	spawnCount--;
	if (spawnCount <= 0)
	{
		isSpawnEnd = true;
	}
}
