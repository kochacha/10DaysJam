#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class JammingSpine : public GameObject
	{
	private:
		GameObjectManager* gManager;

		void SetObjParam();

	public:
		JammingSpine(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position);
		~JammingSpine() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType();
	};
}