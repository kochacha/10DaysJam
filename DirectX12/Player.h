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
		float seVolume;
		//スケールアニメーション用
		Vector3 scale;
		Vector3 endScale;
		//移動方向の矢印表示用
		Object* wayObj;
		//矢印を表示するかどうか
		bool isWayDraw;
		//矢印回転角度
		float wayRotate;
		//スマッシュの軌道表示用
		Object* smashLine;
		//パワーゲージ表示用
		//内側黄色
		Texture2D* powarGauge[10];
		//外枠
		Texture2D* emptyGauge[10];
		//内側黒
		Texture2D* overDriveGauge[5];

		//ゲーム1プレイが終わっているか
		bool isFinish;
		//ヒットストップ中か
		bool isHitStop;
		//ヒットストップしている時間
		int hitStopCount;
		//ゲームオーバーしたときに一度だけ処理を行う用
		bool isOnce;
		//スポーンしたときに一度だけ処理を行う用
		bool isStartOnce;
		//ゲーム中かどうか
		bool* inGame;
		//生成されてからの時間　InputMove()を通さないために使う
		int stackCount;
		//速度にかけて使う
		float speed;
		//壁に接触しているかどうか
		bool isHitWall;

		//スマッシュ関連
		//スマッシュのパワー
		int smashPower;
		int overDirveSmashPower;
		const int MAX_SMASHPOWER = 10;
		const int MAX_OVERDRIVE = 5;
		//スマッシュ操作の猶予フレーム
		int asobiCount;
		//スマッシュ中かどうか
		bool isSmashing;
		//壁を押し戻す距離
		int backCount;
		//押し戻し中の追加スコア保存用
		int addSmashScore;

		//スタン関連
		//スタンしているかどうか
		bool isStun;
		//スタン時間計上用
		int stunCount;
		//スタン最大時間
		const int stunTime = 30;		

		void InputMove();
		void MoveX();
		void MoveY();
		void ScaleAnimation();
		void SetObjParam();
		//コントローラ振動
		void CommonVib(const int arg_time);
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

		//パワーアップ
		void PowerUp(const GameObjectType arg_objectType);
		//通常時のおじゃまトゲとの衝突時処理
		void PowerDown();

		const bool IsFinish();
		//外側から動的にisFinishをtrueにする
		void Finish();
		const bool IsHitStop();
		//ヒットストップの時間を管理
		void HitStopTimer();
		const bool IsInGame();
		void SetPauseManager(PauseManager* arg_pManager);

		//スマッシュしているかどうか
		const bool IsSmashing();
		const bool IsStuning();
		const int GetBackCount();
		const bool IsHitWall();
	};
}