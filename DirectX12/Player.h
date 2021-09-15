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
		//スマッシュのパワー
		int smashPower;
		const int MAX_SMASHPOWER = 10;

		int wallPosX;
		bool isStun;
		int stunCount;
		const int stunTime = 30;

		void InputMove();
		void MoveX();
		void MoveY();
		void SetObjParam();
		void MoveWallPos();
		//パワーのリセット
		void ResetPower();

	public:
		Player(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position);
		~Player() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType();
		//スマッシュしているかどうか
		const bool IsSmashing();
		const bool IsStuning();

		//パワーアップ
		void PowerUp(const GameObjectType arg_objectType);
		//通常時のおじゃまトゲとの衝突時処理
		void PowerDown();
	};
}