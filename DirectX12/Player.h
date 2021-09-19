#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ParticleEmitter;
	class Audio;
	class ItemManager;
	class ScoreManager;

	class Player : public GameObject
	{
	private:
		GameObjectManager* gManager;
		ParticleEmitter* pEmitter;
		ScoreManager* sManager;
		Audio* se;		

		float speed;
		bool smash;
		int testCount;
		//スマッシュのパワー
		int smashPower;
		int overDirveSmashPower;
		const int MAX_SMASHPOWER = 10;
		const int MAX_OVERDRIVE = 5;

		bool isStun;
		int stunCount;
		const int stunTime = 30;

		int backCount;
		int addSmashScore;
		bool hitWall;

		Texture2D* powarGauge[10];
		Texture2D* overDriveGauge[5];

		float seVolume;
		bool isHitStop;
		int hitStopCount;

		bool* inGame;

		void InputMove();
		void MoveX();
		void MoveY();
		void SetObjParam();
		void MoveWallPos();
		//パワーのリセット
		void ResetPower();

	public:
		Player(Camera* arg_camera, GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter, ScoreManager* arg_sManager, const Vector3& arg_position,bool* inGameFlag);
		~Player() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		void SpriteDraw() override;
		void ShowGUI() override;
		GameObjectType GetType();
		//スマッシュしているかどうか
		const bool IsSmashing();
		const bool IsStuning();
		const bool IsFinishSmash();

		//パワーアップ
		void PowerUp(const GameObjectType arg_objectType);
		//通常時のおじゃまトゲとの衝突時処理
		void PowerDown();

		bool IsHitStop() { return isHitStop; }
		void HitStopTimer();

		const int GetBackCount();
		const bool IsHitWall();
		const bool IsInGame();
	};
}