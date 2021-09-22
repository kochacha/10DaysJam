#pragma once
#include "GameObject.h"
namespace KochaEngine
{
	class GameObjectManager;

	class Wall : public GameObject
	{
	private:
		GameObjectManager* gManager;

		//現在の壁の範囲(左下)
		Vector2 minPos;
		//(右上)
		Vector2 maxPos;
		//画面枠内の大きさ
		Vector2 playableSize;
		//フィールドの左端x座標
		float limitLeftPosX;
		//右端x座標
		float limitRightPosX;
		//描画オブジェクト用の座標
		Vector2 objectDrawPos;

	public:
		Wall(GameObjectManager* arg_gManager, const Vector2& minPos, const Vector2& maxPos, const float& limitLeftPos, const float& limitRightPos);
		~Wall() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		GameObjectType GetType();
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		//壁の範囲の更新
		void ScrollWall(float amount);

		const Vector2 GetMinPos();
		const Vector2 GetMaxPos();
		//画面の中心のワールド座標を返す
		const Vector2 GetCenterPos();
		const Vector2 GetPlayableSize();
		const float GetLimitLeftPosX();
		const float GetLimitRightPosX();
	};
}
