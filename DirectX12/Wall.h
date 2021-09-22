#pragma once
#include "GameObject.h"
namespace KochaEngine
{
	class GameObjectManager;

	class Wall : public GameObject
	{
	private:
		GameObjectManager* gManager;

		//���݂̕ǂ͈̔�(����)
		Vector2 minPos;
		//(�E��)
		Vector2 maxPos;
		//��ʘg���̑傫��
		Vector2 playableSize;
		//�t�B�[���h�̍��[x���W
		float limitLeftPosX;
		//�E�[x���W
		float limitRightPosX;
		//�`��I�u�W�F�N�g�p�̍��W
		Vector2 objectDrawPos;

	public:
		Wall(GameObjectManager* arg_gManager, const Vector2& minPos, const Vector2& maxPos, const float& limitLeftPos, const float& limitRightPos);
		~Wall() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		GameObjectType GetType();
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		//�ǂ͈̔͂̍X�V
		void ScrollWall(float amount);

		const Vector2 GetMinPos();
		const Vector2 GetMaxPos();
		//��ʂ̒��S�̃��[���h���W��Ԃ�
		const Vector2 GetCenterPos();
		const Vector2 GetPlayableSize();
		const float GetLimitLeftPosX();
		const float GetLimitRightPosX();
	};
}
