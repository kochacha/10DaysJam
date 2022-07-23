#pragma once
#include "GameObject.h"

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
		
		void SetObjParam();

	public:
		JammingBoss(Camera* arg_camera, GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter, const Vector3& arg_position, ItemManager* arg_iManager);
		~JammingBoss() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void Dead()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType();
		Vector3 GetPrearrangedPosition()const;

	};
}
