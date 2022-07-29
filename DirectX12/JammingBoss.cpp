#include "JammingBoss.h"

#include "GameObjectManager.h"
#include "ItemManager.h"
#include "Wall.h"
#include "Util.h"
#include "Audio.h"
#include "GameSetting.h"
#include "ScoreManager.h"
#include "LevelSetKeeper.h"

void KochaEngine::JammingBoss::SetObjParam()
{
	sphere.position = this->position;
	obj->SetPosition(position);
}

KochaEngine::JammingBoss::JammingBoss(Camera* arg_camera, GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter, ScoreManager* arg_sManager, const Vector3& arg_position, ItemManager* arg_iManager, GameMode* arg_gameMode)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	if (arg_pEmitter == nullptr) return;
	if (arg_sManager == nullptr) return;
	if (arg_iManager == nullptr) return;

	camera = arg_camera;
	gManager = arg_gManager;
	iManager = arg_iManager;
	pEmitter = arg_pEmitter;
	sManager = arg_sManager;
	pWall = gManager->GetWall();

	crrentGameMode = arg_gameMode;

	se = new Audio();

	obj = new Object("plane");
	Initialize();

}

KochaEngine::JammingBoss::~JammingBoss()
{
	if (obj)
	{
		delete obj;
	}
	delete se;
}

void KochaEngine::JammingBoss::Initialize()
{
	isAlpha = true;
	isOnce = false;
	isSpawnEnd = false;

	se->Init();

	velocity.Zero();

	sphere.radius = 8.0f;

	if (*crrentGameMode == GameMode::NORMALMODE)
	{
		firstInterval = 200;
		secondInterval = 60;	
		speed = 0.5f; //ボスの動くスピード
	}
	else
	{
		firstInterval = 0;
		secondInterval = 30;
		speed = 0.2f; //ボスの動くスピード
	}

	texChangeCount = 30;

	velocity.Zero();
	velocity.y = 1.0f;
	easeCount = 0;

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

	position.x -= 30;

	SetObjParam();

	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
	if (*crrentGameMode == GameMode::NORMALMODE)
	{
		camera->SetShake(firstInterval, 1.2f);
		se->PlayWave("Resources/Sound/gogogo.wav", seVolume / 1.0f);
	}
	
}

void KochaEngine::JammingBoss::Update()
{
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
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

		MoveY();
		

		SetObjParam();

		if (!gManager->GetPlayer()->IsFinish()&& gManager->GetPlayer()->GetBackCount() <= 0)
		{
			gManager->HitObject(this, PLAYER);
		}
	

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
	LevelSetAllMode lsam = LevelSetKeeper::GetInstance()->GetVecLSAM()[0];
	pEmitter->BossDeadParticle(position);
	sManager->AddScore(lsam.scoreDefeatBoss);
	isFinish = true;
}

void KochaEngine::JammingBoss::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	obj->Draw(arg_camera, arg_lightManager);
}

void KochaEngine::JammingBoss::MoveY()
{
	if (gManager->GetPlayer()->GetBackCount() <= 0)
	{
		if (texChangeCount <= 0)
		{
			obj->SetTexture("Resources/boss.png");
		}
		else
		{
			texChangeCount--;
		}
	
		position.y += velocity.y * speed;
	}
	else
	{
		obj->SetTexture("Resources/boss_smash.png");
		texChangeCount = 30;
	}

	float DownWall = gManager->GetWall()->GetMinPos().y + 6;
	float UpWall = gManager->GetWall()->GetMaxPos().y - 6;

	if (position.y <= DownWall)
	{
		position.y = DownWall;
		velocity.y = -velocity.y;
	}
	if (position.y >= UpWall)
	{
		position.y = UpWall;
		velocity.y = -velocity.y;
	}
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
	float wallPosX = gManager->GetWall()->GetMinPos().x - 2;

	//登場前のカメラシェイクを待つインターバル
	if (firstInterval <= 0)
	{
		position.x = Util::EaseInQuad(position.x, wallPosX, 30, easeCount);
		if (easeCount <= 30)
		{
			easeCount++;
		}
		SetObjParam();
	}
	else
	{
		firstInterval--;
	}
	
	if (easeCount == 20)
	{
		if (*crrentGameMode == GameMode::NORMALMODE)
		{
			
		}	
		camera->SetShake(10, 3.0f);
		se->PlayWave("Resources/Sound/don.wav", seVolume * 1.5f);
	}
	
	
	//ボスが移動し終わった後のインターバル
	if (position.x >= wallPosX - 1)
	{
		secondInterval--;
	}
	
	if (secondInterval <= 0)
	{
		isSpawnEnd = true;
	}
	
}
