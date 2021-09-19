#pragma once
#include "GameObject.h"
namespace KochaEngine
{
	class GameObjectManager;
	class Wall : public GameObject
	{
	private:
		GameObjectManager* gManager;

		
		Vector2 currMinPos;
		Vector2 minPos;
		Vector2 maxPos;
		Vector2 playableSize;

		float limitLeftPos;
		float limitRightPos;
	public:
		Wall(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector2& minPos, const Vector2& maxPos, const float& limitLeftPos, const float& limitRightPos);
		~Wall() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		GameObjectType GetType();
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		void ScrollWall(float amount);

		const Vector2 GetMinPos();
		const Vector2 GetMaxPos();
		const Vector2 GetCenterPos();
		const Vector2 GetPlayableSize();
		const float GetLimitLeftPos();
		const float GetLimitRightPos();
	};

}

