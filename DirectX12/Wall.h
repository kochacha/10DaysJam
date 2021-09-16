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
		Vector2 playableSize;
	public:
		Wall(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector2& minPos, const Vector2& maxPos);
		~Wall() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		GameObjectType GetType();
		void ScrollWall(float amount);

		const Vector2 GetMinPos();
		const Vector2 GetMaxPos();
		const Vector2 GetCenterPos();
		const Vector2 GetPlayableSize();
	};

}

