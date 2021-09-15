#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class EnhancementItem : public GameObject
	{
	private:
		GameObjectManager* gManager;

		void SetObjParam();

	public:
		EnhancementItem(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position);
		~EnhancementItem() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType();
	};
}