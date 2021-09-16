#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ItemManager;
	class Wall;
	class EnhancementItem : public GameObject
	{
	private:
		GameObjectManager* gManager;
		ItemManager* iManager;
		Wall* pWall;

		void SetObjParam();

	public:
		EnhancementItem(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position, ItemManager* arg_iManager);
		~EnhancementItem() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void Dead()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType();
	};
}