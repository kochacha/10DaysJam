#include "ParticleEmitter.h"
#include "Util.h"

KochaEngine::ParticleEmitter::ParticleEmitter(ParticleManager* pManager)
{
	this->pManager = pManager;
}

KochaEngine::ParticleEmitter::~ParticleEmitter()
{
}

void KochaEngine::ParticleEmitter::MoveParticle(const Vector3& arg_position)
{
	ParticleParam param;
	param.position = arg_position;

	param.life = 20;
	param.moveColor = Vector4(0, 0, 0, 0);
	param.moveRotate = Vector3(0, 0, 0);
	param.moveScale = Vector3(-0.2f, -0.2f, 0);
	param.scale = Vector3(4, 4, 1);
	param.textureName = "Resources/green.png";
	param.velocity = Vector3(0, 0, 0);
	param.color = Vector4(0, 0, 0, 1);
	//auto rand = Util::GetIntRand(0, 360);
	param.rotate = Vector3(0, 0, 45);

	pManager->AddParticle(new Particle(param));
}

void KochaEngine::ParticleEmitter::SmashStar(const Vector3& arg_position)
{
	ParticleParam param;
	param.position = arg_position;

	auto randScale = (float)Util::GetIntRand(5, 8);
	auto randY = (float)(Util::GetIntRand(0, 50) - 25) * 0.1f;
	param.life = 50;
	param.moveColor = Vector4(0, 0, 0, 0);
	param.moveRotate = Vector3(0, 0, randY);
	param.moveScale = Vector3(-randScale / (float)param.life, -randScale / (float)param.life, 0);
	param.scale = Vector3(randScale, randScale, 1);
	param.textureName = "Resources/star.png";
	param.velocity = Vector3(0, randY, 0);
	param.color = Vector4(0, 0, 0, 1);
	param.rotate = Vector3(0, 0, 0);

	pManager->AddParticle(new Particle(param));

}

void KochaEngine::ParticleEmitter::TogePowerParticle(const Vector3& arg_position)
{
	ParticleParam param;
	param.position = arg_position;

	auto randScale = (float)Util::GetIntRand(3, 5);
	auto randY = (float)(Util::GetIntRand(0, 50) - 25) * 0.4f;
	param.life = 25;
	param.moveColor = Vector4(0, 0, 0, 0);
	param.moveRotate = Vector3(0, 0, randY);
	param.moveScale = Vector3(-randScale / (float)param.life, -randScale / (float)param.life, 0);
	param.scale = Vector3(randScale, randScale, 1);
	param.textureName = "Resources/green.png";
	param.velocity = Vector3(0, randY, 0);
	param.color = Vector4(0, 0, 0, 1);
	param.rotate = Vector3(0, 0, 0);

	pManager->AddParticle(new Particle(param));
}

void KochaEngine::ParticleEmitter::PowerUp(const Vector3& arg_position)
{
	ParticleParam param;

	for (int i = 0; i < 3; i++)
	{
		auto randPosX = (float)(Util::GetIntRand(0, 30) - 15) * 0.2f;
		auto randPosY = (float)(Util::GetIntRand(0, 30) - 15) * 0.07f;
		param.position = Vector3(arg_position.x + randPosX, arg_position.y + randPosY, arg_position.z + 0.1f);
		auto randLife = Util::GetIntRand(15, 30);
		param.life = randLife;
		param.moveColor = Vector4(0, 0, 0, 0);
		param.moveRotate = Vector3(0, 0, 0);
		param.moveScale = Vector3(0, 0, 0);
		auto randScale = (float)Util::GetIntRand(3, 5);
		param.scale = Vector3(randScale, randScale, 1);
		param.textureName = "Resources/arrow.png";
		auto randVelY = (float)Util::GetIntRand(20, 30) * 0.03f;
		param.velocity = Vector3(0, randVelY, 0);
		param.color = Vector4(1, 1, 1, 1);
		param.rotate = Vector3(0, 0, 0);

		pManager->AddParticle(new Particle(param));
	}

}

void KochaEngine::ParticleEmitter::Clash(const Vector3& arg_position)
{
	ParticleParam param;

	for (int i = 0; i < 4; i++)
	{
		//auto randPosX = (float)(Util::GetIntRand(0, 30) - 15) * 0.2f;
		//auto randPosY = (float)(Util::GetIntRand(0, 30) - 15) * 0.07f;
		param.position = Vector3(arg_position.x, arg_position.y, arg_position.z + 0.1f);
		auto randLife = Util::GetIntRand(10, 20);
		param.life = randLife;
		param.moveColor = Vector4(0, 0, 0, 0);
		param.moveRotate = Vector3(0, 0, 0);
		param.moveScale = Vector3(-0.1f, -0.1f, 0);
		auto randScale = (float)Util::GetIntRand(4, 6);
		param.scale = Vector3(randScale, randScale, 1);
		param.textureName = "Resources/star.png";
		auto randVelX = (float)(Util::GetIntRand(0,10) - 5) * 0.2f;
		auto randVelY = (float)(Util::GetIntRand(0,10) - 5) * 0.2f;
		param.velocity = Vector3(randVelX, randVelY, 0);
		param.color = Vector4(1, 1, 1, 1);
		param.rotate = Vector3(0, 0, 180);

		pManager->AddParticle(new Particle(param));
	}
}

void KochaEngine::ParticleEmitter::SmashScore(const Vector3& arg_position)
{
	ParticleParam param;
	param.position = arg_position;

	for (int i = 0; i < 2; i++)
	{
		auto randY = (float)(Util::GetIntRand(0, 30) - 15) * 0.1f;
		param.life = 30;
		param.moveColor = Vector4(0, 0, 0, 0);
		param.moveRotate = Vector3(0, 0, 0);
		param.moveScale = Vector3(0, 0, 0);
		param.scale = Vector3(9, 6, 1);
		param.textureName = "Resources/wallPush.png";
		param.velocity = Vector3(0, randY, 0);
		param.color = Vector4(1, 1, 1, 1);
		param.rotate = Vector3(0, 0, 180);

		pManager->AddParticle(new Particle(param));
	}

}

void KochaEngine::ParticleEmitter::HitScore(const Vector3& arg_position, const bool arg_flag)
{
	ParticleParam param;
	param.position = Vector3(arg_position.x, arg_position.y + 6.0f, arg_position.z);

	param.life = 40;
	param.moveColor = Vector4(0, 0, 0, 0);
	param.moveRotate = Vector3(0, 0, 0);
	param.moveScale = Vector3(0, 0, 0);
	param.scale = Vector3(-16, 8, 1);
	if (arg_flag)
	{
		param.textureName = "Resources/togeGet.png";
	}
	else
	{
		param.textureName = "Resources/itemGet.png";
	}
	param.velocity = Vector3(0, 0.25f, 0);
	param.color = Vector4(1, 1, 1, 1);
	param.rotate = Vector3(0, 0, 0);

	pManager->AddParticle(new Particle(param));
}

void KochaEngine::ParticleEmitter::SpawnParticle(const Vector3& arg_position)
{
	ParticleParam param;
	param.life = 30;
	param.textureName = "Resources/circle.png";
	param.moveColor = Vector4(0, 0, 0, 0);
	param.moveRotate = Vector3(0, 0, 0);
	param.moveScale = Vector3(5.0f / 30.0f, 5.0f / 30.0f, 0);
	param.scale = Vector3(0, 0, 1);
	param.color = Vector4(1, 1, 1, 1);
	param.rotate = Vector3(0, 0, 0);

	for (int i = 0; i < 24; i++)
	{
		Vector2 vel = Util::AngleToVector2(15.0f * i);
		param.velocity = Vector3(-vel.x, -vel.y, 0);
		param.position = Vector3(arg_position.x + vel.x * 30, arg_position.y + vel.y * 30, arg_position.z + 0.1f);

		pManager->AddParticle(new Particle(param));
	}
}

void KochaEngine::ParticleEmitter::DeadParticle(const Vector3& arg_position)
{
	ParticleParam param;
	param.position = Vector3(arg_position.x, arg_position.y, arg_position.z + 0.1f);
	param.life = 120;
	param.textureName = "Resources/circle.png";
	param.moveColor = Vector4(0, 0, 0, 0);
	param.moveRotate = Vector3(0, 0, 0);
	param.moveScale = Vector3(-5.0f / 120.0f, -5.0f / 120.0f, 0);
	param.scale = Vector3(5, 5, 1);
	param.color = Vector4(1, 1, 1, 1);
	param.rotate = Vector3(0, 0, 0);

	for (int i = 0; i < 24; i++)
	{
		Vector2 vel = Util::AngleToVector2(15.0f * i);
		param.velocity = Vector3(vel.x * 0.5f, vel.y * 0.5f, 0);

		pManager->AddParticle(new Particle(param));
	}
}

void KochaEngine::ParticleEmitter::BonusParticle(const Vector3& arg_position)
{
	ParticleParam param;
	param.position = Vector3(arg_position.x, arg_position.y, arg_position.z);

	param.life = 150;
	param.moveColor = Vector4(0, 0, 0, 0);
	param.moveRotate = Vector3(0, 0, 0);
	param.moveScale = Vector3(0, 0, 0);
	param.scale = Vector3(-32, 12, 1);
	param.textureName = "Resources/bonusGet.png";
	param.velocity = Vector3(0, 0.2f, 0);
	param.color = Vector4(1, 1, 1, 1);
	param.rotate = Vector3(0, 0, 0);

	pManager->AddParticle(new Particle(param));
}
