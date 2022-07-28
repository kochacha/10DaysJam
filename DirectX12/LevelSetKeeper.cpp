#include "LevelSetKeeper.h"
#include "CSVReader.h"

KochaEngine::LevelSetKeeper* KochaEngine::LevelSetKeeper::instance = nullptr;

KochaEngine::LevelSetKeeper::LevelSetKeeper()
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
