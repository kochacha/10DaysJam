#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class Player : public GameObject
	{
	private:
		GameObjectManager* gManager;

		float speed;
		bool smash;
		int testCount;

		int wallPosX;

		void InputMove();
		void MoveX();
		void MoveY();
		void SetObjParam();

	public:
		Player(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position);
		~Player() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType();
	};
}