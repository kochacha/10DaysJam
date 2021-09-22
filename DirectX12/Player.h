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
		//�ړ������̖��\���p
		Object* wayObj;
		//�X�}�b�V���̋O���\���p
		Object* smashLine;
		//�p���[�Q�[�W�\���p
		//�������F
		Texture2D* powarGauge[10];
		//�O�g
		Texture2D* emptyGauge[10];
		//������
		Texture2D* overDriveGauge[5];

		//�Q�[��1�v���C���I����Ă��邩
		bool isFinish;
		//�q�b�g�X�g�b�v����
		bool isHitStop;
		//�q�b�g�X�g�b�v���Ă��鎞��
		int hitStopCount;

		float speed;
		float wayRotate;
		bool smash;
		bool isWayDraw;
		bool isOnce;
		int testCount;
		int asobiCount;
		int stackCount;
		//�X�}�b�V���̃p���[
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

		float seVolume;		

		bool* inGame;

		void InputMove();
		void MoveX();
		void MoveY();
		void ScaleAnimation();
		void SetObjParam();
		void MoveWallPos();
		void CommonVib(const int arg_time);
		//�p���[�̃��Z�b�g
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
		//�X�}�b�V�����Ă��邩�ǂ���
		const bool IsSmashing();
		const bool IsStuning();

		//�p���[�A�b�v
		void PowerUp(const GameObjectType arg_objectType);
		//�ʏ펞�̂�����܃g�Q�Ƃ̏Փˎ�����
		void PowerDown();

		const bool IsFinish();
		//�O�����瓮�I��isFinish��true�ɂ���
		void Finish();
		const bool IsHitStop();
		void HitStopTimer();

		void SetPauseManager(PauseManager* arg_pManager);

		const int GetBackCount();
		const bool IsHitWall();
		const bool IsInGame();
	};
}