#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ItemManager;	
	class Wall;

	enum class ItemEmitOption;

	class JammingSpine : public GameObject
	{
	private:
		GameObjectManager* gManager;
		ItemManager* iManager;
		Wall* pWall;
		//�ǉ������ݎ��̐����Ɏg��
		Vector3 emittedPlayerPosition;
		Vector3 prearrangedPosition;
		int moveCount;
		//�㉺�ړ����邩�ǂ���
		bool isVerticalMove;
		float velVertical;

		void SetObjParam();
		//�㉺�ړ�
		void MoveVertical();
		//�㉺�ړ��̑��x����
		const float GetVerticalMoveSpeed();

		//���؂ꋗ���p
		static float DEAD_X;
		static float CUT_OFF_X;
		static float SURVIVE_LENGTH;

	public:
		JammingSpine(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position, ItemManager* arg_iManager, const ItemEmitOption arg_option, const bool arg_isVMove);
		~JammingSpine() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void Dead()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType();
		Vector3 GetPrearrangedPosition()const;
		//���؂�Ă��邩�ǂ���
		bool IsCutOffScreen();
		//ItemManager�ł̕`��ݒ�p
		Vector3 GetTagPosition()const;
		Vector3 GetTagScale()const;

		static void SetDeadLength(const int arg_length);
	};
}