#pragma once
#include <cassert>

namespace KochaEngine
{
	const int NUM_LSAM = 9;
	const int NUM_LSI = 6;

	struct LevelSetAllMode
	{
		//���̕ǂ܂ŃX�}�b�V���œ��B�����Ƃ��̃X�R�A
		int scoreSmashLeftLimit;
		//�A�C�e�����擾�����Ƃ��̃X�R�A
		int scoreGetItem;
		//�g�Q���X�}�b�V�������Ƃ��̃X�R�A
		int scoreSmashSpine;
		//�A�C�e��1���̃X�}�b�V���p���[
		int amountReductionItem;
		//�g�Q1���̃X�}�b�V���p���[
		int amountReductionSpine;
		//�X�}�b�V�����ɉ��Z���ꑱ����X�R�A
		int scoreGainSmashing;
		//�m�[�}�����[�h�̃{�X�o���܂ł̃m���}
		int quotaAppearBoss;
		//�{�X�����j�����X�R�A
		int scoreDefeatBoss;
		//�g�Q�̌��؂ꋗ��
		int lengthSpineHide;

		LevelSetAllMode(const int arg_SSLL = 0, const int arg_SGI = 0, const int arg_SSS = 0, const int arg_ARI = 0,
			const int arg_ARS = 0, const int arg_SGS = 0, const int arg_QAB = 0, const int arg_SDB = 0,
			const int arg_LSH = 0)
			:scoreSmashLeftLimit(arg_SSLL),
			 scoreGetItem(arg_SGI),
			 scoreSmashSpine(arg_SSS),
			 amountReductionItem(arg_ARI),
			 amountReductionSpine(arg_ARS),
			 scoreGainSmashing(arg_SGS),
			 quotaAppearBoss(arg_QAB),
			 scoreDefeatBoss(arg_SDB),
			 lengthSpineHide(arg_LSH)
		{
		}

		int& operator[](const int& arg_index)
		{
			switch (arg_index)
			{
			case 0:
				return scoreSmashLeftLimit;
				break;
			case 1:
				return scoreGetItem;
				break;
			case 2:
				return scoreSmashSpine;
				break;
			case 3:
				return amountReductionItem;
				break;
			case 4:
				return amountReductionSpine;
				break;
			case 5:
				return scoreGainSmashing;
				break;
			case 6:
				return quotaAppearBoss;
				break;
			case 7:
				return scoreDefeatBoss;
				break;
			case 8:
				return lengthSpineHide;
				break;
			default:
				assert(0);
				break;
			}
		}
	};

	struct LevelSetIndivisual
	{
		//�A�C�e���E�g�Q�̐����Ԋu�t���[��
		int frameObjectEmitInterval;
		//�������ꂽ�g�Q�������m��
		int percentageSpineMove;
		//�X�N���[���̑�����100�{������
		int scrollSpeedHundredfold;
		//�X�N���[���X�s�[�h�ω��܂ł̕b��
		int secChangeScrollSpeedInterval;
		//�{�X�������m��
		int percentageBossMove;
		//�I�u�W�F�N�g�̂����A�A�C�e�������������m��
		int percentageEmitItem;

		LevelSetIndivisual(const int arg_FOEI = 0, const int arg_PSM = 0, const int arg_SSH = 0, const int arg_SCSS = 0,
			const int arg_PBM = 0, const int arg_PEI = 0)
			:frameObjectEmitInterval(arg_FOEI),
			 percentageSpineMove(arg_PSM),
			 scrollSpeedHundredfold(arg_SSH),
			 secChangeScrollSpeedInterval(arg_SCSS),
			 percentageBossMove(arg_PBM),
			 percentageEmitItem(arg_PEI)
		{
		}

		int& operator[](const int& arg_index)
		{
			switch (arg_index)
			{
			case 0:
				return frameObjectEmitInterval;
				break;
			case 1:
				return percentageSpineMove;
				break;
			case 2:
				return scrollSpeedHundredfold;
				break;
			case 3:
				return secChangeScrollSpeedInterval;
				break;
			case 4:
				return percentageBossMove;
				break;
			case 5:
				return percentageEmitItem;
				break;
			default:
				assert(0);
				break;
			}
		}
	};
}