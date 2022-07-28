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
	//全体影響
	if (!csvLSAM.empty())
	{
		//最大の要素数を持つ設定項だけレベルを作る
		int numElem = 0;
		for (auto rowElem : csvLSAM)
		{
			if (rowElem.size() > numElem)
			{
				numElem = rowElem.size();
			}
		}

		//要素の仮登録
		for (int i = 0; i < numElem; i++)
		{
			vecLSAM.push_back(LevelSetAllMode());
		}
		
		//csvより代入
		for (int i = 0; i < NUM_LSAM; i++)
		{
			for (int j = 0; j < vecLSAM.size(); j++)
			{
				//特定要素が設定されていたら
				if (csvLSAM[i].size() >= j)
				{
					//そのまま代入
					vecLSAM[j][i] = csvLSAM[i][j];
				}
				//設定されていなかったら
				else
				{
					//1つ前の要素をコピー
					vecLSAM[j][i] = vecLSAM[j - 1][i];
				}
			}
		}
	}

	//Normalのレベル
	if (!csvLSI_Normal.empty())
	{
		//最大の要素数を持つ設定項だけレベルを作る
		int numElem = 0;
		for (auto rowElem : csvLSI_Normal)
		{
			if (rowElem.size() > numElem)
			{
				numElem = rowElem.size();
			}
		}

		//要素の仮登録
		for (int i = 0; i < numElem; i++)
		{
			vecLSI_Normal.push_back(LevelSetIndivisual());
		}

		//csvより代入
		for (int i = 0; i < NUM_LSI; i++)
		{
			for (int j = 0; j < vecLSI_Normal.size(); j++)
			{
				//特定要素が設定されていたら
				if (csvLSI_Normal[i].size() >= j)
				{
					//そのまま代入
					vecLSI_Normal[j][i] = csvLSI_Normal[i][j];
				}
				//設定されていなかったら
				else
				{
					//1つ前の要素をコピー
					vecLSI_Normal[j][i] = vecLSI_Normal[j - 1][i];
				}
			}
		}
	}

	//Endlessのレベル
	if (!csvLSI_Endless.empty())
	{
		//最大の要素数を持つ設定項だけレベルを作る
		int numElem = 0;
		for (auto rowElem : csvLSI_Endless)
		{
			if (rowElem.size() > numElem)
			{
				numElem = rowElem.size();
			}
		}

		//要素の仮登録
		for (int i = 0; i < numElem; i++)
		{
			vecLSI_Endless.push_back(LevelSetIndivisual());
		}

		//csvより代入
		for (int i = 0; i < NUM_LSI; i++)
		{
			for (int j = 0; j < vecLSI_Endless.size(); j++)
			{
				//特定要素が設定されていたら
				if (csvLSI_Endless[i].size() >= j)
				{
					//そのまま代入
					vecLSI_Endless[j][i] = csvLSI_Endless[i][j];
				}
				//設定されていなかったら
				else
				{
					//1つ前の要素をコピー
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
