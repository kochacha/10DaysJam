#pragma once
#include "GameObject.h"
namespace KochaEngine
{
	class GameObjectManager;
	class Wall : public GameObject
	{
	private:
		GameObjectManager* gManager;

		Vector2 minPos;
		Vector2 maxPos;
	public:
		Wall(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector2& minPos, const Vector2& maxPos);
		~Wall() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		GameObjectType GetType();

		const Vector2 GetMinPos();
		const Vector2 GetMaxPos();

	};

}

