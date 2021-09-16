#include "ParticleEmitter.h"
#include "Util.h"

KochaEngine::ParticleEmitter::ParticleEmitter(ParticleManager* pManager)
{
	this->pManager = pManager;
}

KochaEngine::ParticleEmitter::~ParticleEmitter()
{
}

void KochaEngine::ParticleEmitter::MoveParticle(Vector3 position)
{
	ParticleParam param;
	param.position = position;

	param.life = 20;
	param.moveColor = Vector4(0, 0, 0, 0);
	param.moveRotate = Vector3(0, 0, 0);
	param.moveScale = Vector3(-0.2f, -0.2f, 0);
	param.scale = Vector3(4, 4, 1);
	param.textureName = "Resources/green.png";
	param.velocity = Vector3(0, 0, 0);
	param.color = Vector4(0, 0, 0, 1);
	auto rand = Util::GetIntRand(0, 360);
	param.rotate = Vector3(0,0, rand);

	pManager->AddParticle(new Particle(param));
}
