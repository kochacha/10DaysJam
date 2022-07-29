#include "LevelSetKeeper.h"
#include "CSVReader.h"

KochaEngine::LevelSetKeeper* KochaEngine::LevelSetKeeper::instance = nullptr;

KochaEngine::LevelSetKeeper::LevelSetKeeper()
	:mode(GameMode::SCOREATTAKMODE)
{
	vecLSAM.clear();
	vecLSI_Normal.clear();
	vecLSI_Endless.clear();
}

KochaEngine::LevelSetKeeper::~LevelSetKeeper()
{
	vecLSAM.clear();
	vecLSI_Normal.clear();
	vecLSI_Endless.clear();
}

KochaEngine::LevelSetKeeper* KochaEngine::LevelSetKeeper::GetInstance()
{
	if (!instance)
	{
		instance = new LevelSetKeeper();
	}
	return instance;
}

void KochaEngine::LevelSetKeeper::DeleteInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void KochaEngine::LevelSetKeeper::StoreLevelSet()
{
	auto csvLSAM = CSVReader::GetInstance()->GetMapData("LevelSet_Overall");
	auto csvLSI_Normal = CSVReader::GetInstance()->GetMapData("LevelSet_Normal");
	auto csvLSI_Endless = CSVReader::GetInstance()->GetMapData("LevelSet_Endless");
	//�S�̉e��
	if (!csvLSAM.empty())
	{
		//�ő�̗v�f�������ݒ荀�������x�������
		int numElem = 0;
		for (auto rowElem : csvLSAM)
		{
			if (rowElem.size() > numElem)
			{
				numElem = rowElem.size();
			}
		}

		//�v�f�̉��o�^
		for (int i = 0; i < numElem; i++)
		{
			vecLSAM.push_back(LevelSetAllMode());
		}
		
		//csv�����
		for (int i = 0; i < NUM_LSAM; i++)
		{
			for (int j = 0; j < vecLSAM.size(); j++)
			{
				//����v�f���ݒ肳��Ă�����
				if (csvLSAM[i].size() >= j)
				{
					//���̂܂ܑ��
					vecLSAM[j][i] = csvLSAM[i][j];
				}
				//�ݒ肳��Ă��Ȃ�������
				else
				{
					//1�O�̗v�f���R�s�[
					vecLSAM[j][i] = vecLSAM[j - 1][i];
				}
			}
		}
	}

	//Normal�̃��x��
	if (!csvLSI_Normal.empty())
	{
		//�ő�̗v�f�������ݒ荀�������x�������
		int numElem = 0;
		for (auto rowElem : csvLSI_Normal)
		{
			if (rowElem.size() > numElem)
			{
				numElem = rowElem.size();
			}
		}

		//�v�f�̉��o�^
		for (int i = 0; i < numElem; i++)
		{
			vecLSI_Normal.push_back(LevelSetIndivisual());
		}

		//csv�����
		for (int i = 0; i < NUM_LSI; i++)
		{
			for (int j = 0; j < vecLSI_Normal.size(); j++)
			{
				//����v�f���ݒ肳��Ă�����
				if (csvLSI_Normal[i].size() >= j)
				{
					//���̂܂ܑ��
					vecLSI_Normal[j][i] = csvLSI_Normal[i][j];
				}
				//�ݒ肳��Ă��Ȃ�������
				else
				{
					//1�O�̗v�f���R�s�[
					vecLSI_Normal[j][i] = vecLSI_Normal[j - 1][i];
				}
			}
		}
	}

	//Endless�̃��x��
	if (!csvLSI_Endless.empty())
	{
		//�ő�̗v�f�������ݒ荀�������x�������
		int numElem = 0;
		for (auto rowElem : csvLSI_Endless)
		{
			if (rowElem.size() > numElem)
			{
				numElem = rowElem.size();
			}
		}

		//�v�f�̉��o�^
		for (int i = 0; i < numElem; i++)
		{
			vecLSI_Endless.push_back(LevelSetIndivisual());
		}

		//csv�����
		for (int i = 0; i < NUM_LSI; i++)
		{
			for (int j = 0; j < vecLSI_Endless.size(); j++)
			{
				//����v�f���ݒ肳��Ă�����
				if (csvLSI_Endless[i].size() >= j)
				{
					//���̂܂ܑ��
					vecLSI_Endless[j][i] = csvLSI_Endless[i][j];
				}
				//�ݒ肳��Ă��Ȃ�������
				else
				{
					//1�O�̗v�f���R�s�[
					vecLSI_Endless[j][i] = vecLSI_Endless[j - 1][i];
				}
			}
		}
	}
}

const std::vector<KochaEngine::LevelSetAllMode>& KochaEngine::LevelSetKeeper::GetVecLSAM() const
{
	return vecLSAM;
}

const std::vector<KochaEngine::LevelSetIndivisual>& KochaEngine::LevelSetKeeper::GetVecLSI_Normal() const
{
	return vecLSI_Normal;
}

const std::vector<KochaEngine::LevelSetIndivisual>& KochaEngine::LevelSetKeeper::GetVecLSI_Endless() const
{
	return vecLSI_Endless;
}

const std::vector<KochaEngine::LevelSetIndivisual>& KochaEngine::LevelSetKeeper::GetLSI_CurrentMode() const
{
	switch (mode)
	{
	case KochaEngine::TITLEMODE:
		assert(0);
		break;
	case KochaEngine::NORMALMODE:
		return vecLSI_Normal;
		break;
	case KochaEngine::SCOREATTAKMODE:
		return vecLSI_Endless;
		break;
	default:
		assert(0);
		break;
	}
}

const KochaEngine::LevelSetIndivisual KochaEngine::LevelSetKeeper::GetCurrentModeWithLevel(const int arg_level) const
{
	static LevelSetIndivisual answer = LevelSetIndivisual();

	switch (mode)
	{
	case KochaEngine::TITLEMODE:
		assert(0);
		break;
	case KochaEngine::NORMALMODE:
		//�����̃��x�����̏�񂪓o�^����Ă���
		if (arg_level <= vecLSI_Normal.size())
		{
			answer = vecLSI_Normal[arg_level - 1];
		}
		//�o�^����Ă��Ȃ�
		else
		{
			answer = vecLSI_Normal[vecLSI_Normal.size() - 1];
		}
		break;
	case KochaEngine::SCOREATTAKMODE:
		//�����̃��x�����̏�񂪓o�^����Ă���
		if (arg_level <= vecLSI_Endless.size())
		{
			answer = vecLSI_Endless[arg_level - 1];
		}
		//�o�^����Ă��Ȃ�
		else
		{
			answer = vecLSI_Endless[vecLSI_Endless.size() - 1];
		}
		break;
	default:
		assert(0);
		break;
	}

	return answer;
}

KochaEngine::GameMode KochaEngine::LevelSetKeeper::GetGameMode() const
{
	return mode;
}

void KochaEngine::LevelSetKeeper::SetGameMode(const GameMode arg_mode)
{
	mode = arg_mode;
}
