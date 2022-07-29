#pragma once
#include "JammingBoss.h"

namespace KochaEngine
{
	class Camera;
	class GameObjectManager;
	class ParticleEmitter;
	class ScoreManager;
	class ItemManager;

	class BossSpawner
	{
	public:
		static BossSpawner* GetInstance();
		static void DeleteInstance();

		void SpawnBoss();
		void SetClass(Camera* arg_camera, GameObjectManager* arg_oManager, ParticleEmitter* arg_pEmitter, ScoreManager* arg_sManager, ItemManager* arg_iManager);

	private:
		static BossSpawner* instance;
		BossSpawner();
		~BossSpawner();
		BossSpawner(const BossSpawner& another) = delete;
		BossSpawner& operator=(const BossSpawner& another) = delete;

		Camera* camera;
		GameObjectManager* oManager;
		ParticleEmitter* pEmitter;
		ScoreManager* sManager;
		ItemManager* iManager;
	};
}