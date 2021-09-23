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
		//�X�P�[���A�j���[�V�����p
		Vector3 scale;
		Vector3 endScale;
		//�ړ������̖��\���p
		Object* wayObj;
		//����\�����邩�ǂ���
		bool isWayDraw;
		//����]�p�x
		float wayRotate;
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
		//�Q�[���I�[�o�[�����Ƃ��Ɉ�x�����������s���p
		bool isOnce;
		//�X�|�[�������Ƃ��Ɉ�x�����������s���p
		bool isStartOnce;
		//�Q�[�������ǂ���
		bool* inGame;
		//��������Ă���̎��ԁ@InputMove()��ʂ��Ȃ����߂Ɏg��
		int stackCount;
		//���x�ɂ����Ďg��
		float speed;
		//�ǂɐڐG���Ă��邩�ǂ���
		bool isHitWall;

		//�X�}�b�V���֘A
		//�X�}�b�V���̃p���[
		int smashPower;
		int overDirveSmashPower;
		const int MAX_SMASHPOWER = 10;
		const int MAX_OVERDRIVE = 5;
		//�X�}�b�V������̗P�\�t���[��
		int asobiCount;
		//�X�}�b�V�������ǂ���
		bool isSmashing;
		//�ǂ������߂�����
		int backCount;
		//�����߂����̒ǉ��X�R�A�ۑ��p
		int addSmashScore;

		//�X�^���֘A
		//�X�^�����Ă��邩�ǂ���
		bool isStun;
		//�X�^�����Ԍv��p
		int stunCount;
		//�X�^���ő厞��
		const int stunTime = 30;		

		void InputMove();
		void MoveX();
		void MoveY();
		void ScaleAnimation();
		void SetObjParam();
		//�R���g���[���U��
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

		//�p���[�A�b�v
		void PowerUp(const GameObjectType arg_objectType);
		//�ʏ펞�̂�����܃g�Q�Ƃ̏Փˎ�����
		void PowerDown();

		const bool IsFinish();
		//�O�����瓮�I��isFinish��true�ɂ���
		void Finish();
		const bool IsHitStop();
		//�q�b�g�X�g�b�v�̎��Ԃ��Ǘ�
		void HitStopTimer();
		const bool IsInGame();
		void SetPauseManager(PauseManager* arg_pManager);

		//�X�}�b�V�����Ă��邩�ǂ���
		const bool IsSmashing();
		const bool IsStuning();
		const int GetBackCount();
		const bool IsHitWall();
	};
}