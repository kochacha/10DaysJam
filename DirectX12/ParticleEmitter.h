#pragma once
#include "Particle.h"
#include "ParticleManager.h"

namespace KochaEngine
{
	enum
	{
		MAX_BAKUGEKI = 4,
		MAX_HITPARTICLE = 4,
		MAX_EXPROTION = 8,
		MAX_SHOT = 6,
		MAX_CHIT = 8,
	};

	class ParticleEmitter
	{
	private:
		ParticleManager* pManager;

	public:
		ParticleEmitter(ParticleManager* pManager);
		~ParticleEmitter();

		void MoveParticle(const Vector3& arg_position);
		void SmashStar(const Vector3& arg_position);
		void TogePowerParticle(const Vector3& arg_position);
		void PowerUp(const Vector3& arg_position);
		void Clash(const Vector3& arg_position);
		void SmashScore(const Vector3& arg_position);
		void HitScore(const Vector3& arg_position, const bool arg_flag);
		void SpawnParticle(const Vector3& arg_position);
		void DeadParticle(const Vector3& arg_position);
		void BonusParticle(const Vector3& arg_position);
	};
}
