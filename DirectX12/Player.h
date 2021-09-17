#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ParticleEmitter;

	class Player : public GameObject
	{
	private:
		GameObjectManager* gManager;
		ParticleEmitter* pEmitter;

		float speed;
		bool smash;
		int testCount;
		//スマッシュのパワー
		int smashPower;
		const int MAX_SMASHPOWER = 10;

		bool isStun;
		int stunCount;
		const int stunTime = 30;

		int backCount;
		bool hitWall;

		void InputMove();
		void MoveX();
		void MoveY();
		void SetObjParam();
		void MoveWallPos();
		//パワーのリセット
		void ResetPower();

	public:
		Player(Camera* arg_camera, GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter, const Vector3& arg_position);
		~Player() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		void ShowGUI() override;
		GameObjectType GetType();
		//スマッシュしているかどうか
		const bool IsSmashing();
		const bool IsStuning();

		//パワーアップ
		void PowerUp(const GameObjectType arg_objectType);
		//通常時のおじゃまトゲとの衝突時処理
		void PowerDown();

		const int GetBackCount();
		const bool IsHitWall();
	};
}