#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ParticleEmitter;
	class Audio;

	class Player : public GameObject
	{
	private:
		GameObjectManager* gManager;
		ParticleEmitter* pEmitter;
		Audio* se;

		float speed;
		bool smash;
		int testCount;
		//�X�}�b�V���̃p���[
		int smashPower;
		int overDirveSmashPower;
		const int MAX_SMASHPOWER = 10;
		const int MAX_OVERDRIVE = 5;

		bool isStun;
		int stunCount;
		const int stunTime = 30;

		int backCount;
		bool hitWall;

		Texture2D* powarGauge[10];
		Texture2D* overDriveGauge[5];

		float seVolume;
		bool isHitStop;
		int hitStopCount;

		void InputMove();
		void MoveX();
		void MoveY();
		void SetObjParam();
		void MoveWallPos();
		//�p���[�̃��Z�b�g
		void ResetPower();

	public:
		Player(Camera* arg_camera, GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter, const Vector3& arg_position);
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
		const bool IsFinishSmash();

		//�p���[�A�b�v
		void PowerUp(const GameObjectType arg_objectType);
		//�ʏ펞�̂�����܃g�Q�Ƃ̏Փˎ�����
		void PowerDown();

		bool IsHitStop() { return isHitStop; }
		void HitStopTimer();

		const int GetBackCount();
		const bool IsHitWall();
	};
}