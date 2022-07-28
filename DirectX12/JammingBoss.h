#pragma once
#include "GameObject.h"
#include "GameMode.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ItemManager;
	class Wall;

	class JammingBoss : public GameObject
	{
	private:
		GameObjectManager* gManager;
		ItemManager* iManager;
		Wall* pWall;

		GameMode* crrentGameMode;

		bool isFinish;
		bool isOnce;
		bool isSpawnEnd;

		int firstInterval;
		int secondInterval;
		int easeCount;
		int texChangeCount;

		float speed;
		
		void SetObjParam();

	public:
		JammingBoss(Camera* arg_camera, GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter, const Vector3& arg_position, ItemManager* arg_iManager,GameMode* arg_gameMode);
		~JammingBoss() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void Dead()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		void MoveY();
		GameObjectType GetType();
		Vector3 GetPrearrangedPosition()const;
		const bool IsFinish() { return isFinish; }
		const bool IsSpawnEnd() { return isSpawnEnd; }

		void Spawn();
	};
}
