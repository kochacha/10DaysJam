#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ParticleEmitter;
	class Audio;
	class ItemManager;
	class ScoreManager;
	class PauseManager;

	class Player : public GameObject
	{
	private:
		GameObjectManager* gManager;
		ParticleEmitter* pEmitter;
		ScoreManager* sManager;
		PauseManager* pManager;
		Audio* se;	
		Object* wayObj;
		Object* smashLine;

		float speed;
		float wayRotate;
		bool smash;
		bool isWayDraw;
		bool isFinish;
		bool isOnce;
		int testCount;
		int asobiCount;
		int stackCount;
		//スマッシュのパワー
		int smashPower;
		int overDirveSmashPower;
		const int MAX_SMASHPOWER = 10;
		const int MAX_OVERDRIVE = 5;
		Vector3 scale;
		Vector3 endScale;

		bool isStun;
		int stunCount;
		const int stunTime = 30;

		int backCount;
		int addSmashScore;
		bool hitWall;

		Texture2D* powarGauge[10];
		Texture2D* emptyGauge[10];
		Texture2D* overDriveGauge[5];

		float seVolume;
		bool isHitStop;
		int hitStopCount;

		bool* inGame;

		void InputMove();
		void MoveX();
		void MoveY();
		void ScaleAnimation();
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

		bool IsFinish() { return isFinish; }
		void Finish() { isFinish = true; }
		bool IsHitStop() { return isHitStop; }
		void HitStopTimer();

		void SetPauseManager(PauseManager* arg_pManager);

		const int GetBackCount();
		const bool IsHitWall();
		const bool IsInGame();
	};
}