#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class EnhancementItem;
	class JammingSpine;
	class Wall;
	class ScrollManager;

	//�A�C�e�����ǂ��ɐ��������邩
	enum class ItemEmitPosition
	{
		ABSOLUTE_WORLDPOS,    //���[���h���W���̂܂�
		FROM_CENTER,          //��ʒ��S����Ƃ���
		MORE_THAN_RIGHTSIDE,  //��ʂ̉E�[����
	};

	//�A�C�e�������������Ă��邩
	enum class ItemEmitOption
	{
		NORMAL,         //�ʏ펞
		SMASHING_WALL,  //�X�}�b�V����
	};

	class ItemManager
	{
	private:
		Camera* camera;
		GameObjectManager* gManager;
		Wall* pWall;
		ScrollManager* scrollManager;

		//�o��I�u�W�F�N�g2��̔z��
		std::vector<EnhancementItem*> enhancementItems;
		std::vector<JammingSpine*> jammingSpines;				
		//�ʏ펞�̃A�C�e�������C���^�[�o���J�E���g
		unsigned int emitInterval;
		//�Q�[����ʂ̘g�̑傫��(��������)
		const float MARGIN_FRAME;
		//�A�C�e���𐶐����鉼�z���[��
		float arrayEmitHight[11];
		//�������̍��W�`�F�b�N�J�E���g
		int hitCheckCount;

		//�z�񂩂�폜
		void DeleteFromItems(EnhancementItem* arg_enhItem);
		void DeleteFromSpines(JammingSpine* arg_jamSpine);
		//�ʏ펞�̐���
		void EmitItemsNormalTime();
		//�X�}�b�V�����̐���
		void EmitItemsSmashing();
		//�A�C�e�������t���[
		void GeneralEmitCommand(const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption);
		//�������������W�ɕϊ�
		void FixEmitPositionByCondition(Vector3& arg_position, const ItemEmitPosition arg_emitPosition);
		//�p�ӂ����^�����[���̂ǂ��ɐ������邩���߂�
		Vector3 DetermineEmitPos(const GameObjectType arg_objType);
		//�����̃A�C�e���Ɠ������Ă��Ȃ�������
		//���g����������
		const bool IsHitExistingItems(const GameObjectType arg_objType, Vector3& arg_position);
		//���x���f�U�C���p
		//�����C���^�[�o��
		const unsigned int GetMaxEmitInterval();
		//�A�C�e���ƃg�Q�̂ǂ���𐶐����邩
		const unsigned int GetEmitTypeCoefficient();
		//�����g�Q�ɂ��邩�ǂ���
		const bool GetIsSpineMove();

	public:
		ItemManager(Camera* arg_camera, GameObjectManager* arg_gManager);
		~ItemManager();

		void Initialize(ScrollManager* arg_scrollManager);
		void Update();
		//�A�C�e��������̍폜�\��
		void DeleteFromVector(GameObject* arg_pObj, const GameObjectType arg_objType);
		//�O������̐����˗�
		void AddEnhItem(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition = ItemEmitPosition::ABSOLUTE_WORLDPOS, const ItemEmitOption arg_emitOption = ItemEmitOption::NORMAL);
		void AddJamSpine(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition = ItemEmitPosition::ABSOLUTE_WORLDPOS, const ItemEmitOption arg_emitOption = ItemEmitOption::NORMAL);		
		//�X�N���[���}�l�[�W���[�̃��x���𑼎҂ɓ`����
		const unsigned int GetScrollLevel();		
	};
}