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
	//‘S‘Ì‰e‹¿
	if (!csvLSAM.empty())
	{
		//Å‘å‚Ì—v‘f”‚ğ‚Âİ’è€‚¾‚¯ƒŒƒxƒ‹‚ğì‚é
		int numElem = 0;
		for (auto rowElem : csvLSAM)
		{
			if (rowElem.size() > numElem)
			{
				numElem = rowElem.size();
			}
		}

		//—v‘f‚Ì‰¼“o˜^
		for (int i = 0; i < numElem; i++)
		{
			vecLSAM.push_back(LevelSetAllMode());
		}
		
		//csv‚æ‚è‘ã“ü
		for (int i = 0; i < NUM_LSAM; i++)
		{
			for (int j = 0; j < vecLSAM.size(); j++)
			{
				//“Á’è—v‘f‚ªİ’è‚³‚ê‚Ä‚¢‚½‚ç
				if (csvLSAM[i].size() >= j)
				{
					//‚»‚Ì‚Ü‚Ü‘ã“ü
					vecLSAM[j][i] = csvLSAM[i][j];
				}
				//İ’è‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚ç
				else
				{
					//1‚Â‘O‚Ì—v‘f‚ğƒRƒs[
					vecLSAM[j][i] = vecLSAM[j - 1][i];
				}
			}
		}
	}

	//Normal‚ÌƒŒƒxƒ‹
	if (!csvLSI_Normal.empty())
	{
		//Å‘å‚Ì—v‘f”‚ğ‚Âİ’è€‚¾‚¯ƒŒƒxƒ‹‚ğì‚é
		int numElem = 0;
		for (auto rowElem : csvLSI_Normal)
		{
			if (rowElem.size() > numElem)
			{
				numElem = rowElem.size();
			}
		}

		//—v‘f‚Ì‰¼“o˜^
		for (int i = 0; i < numElem; i++)
		{
			vecLSI_Normal.push_back(LevelSetIndivisual());
		}

		//csv‚æ‚è‘ã“ü
		for (int i = 0; i < NUM_LSI; i++)
		{
			for (int j = 0; j < vecLSI_Normal.size(); j++)
			{
				//“Á’è—v‘f‚ªİ’è‚³‚ê‚Ä‚¢‚½‚ç
				if (csvLSI_Normal[i].size() >= j)
				{
					//‚»‚Ì‚Ü‚Ü‘ã“ü
					vecLSI_Normal[j][i] = csvLSI_Normal[i][j];
				}
				//İ’è‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚ç
				else
				{
					//1‚Â‘O‚Ì—v‘f‚ğƒRƒs[
					vecLSI_Normal[j][i] = vecLSI_Normal[j - 1][i];
				}
			}
		}
	}

	//Endless‚ÌƒŒƒxƒ‹
	if (!csvLSI_Endless.empty())
	{
		//Å‘å‚Ì—v‘f”‚ğ‚Âİ’è€‚¾‚¯ƒŒƒxƒ‹‚ğì‚é
		int numElem = 0;
		for (auto rowElem : csvLSI_Endless)
		{
			if (rowElem.size() > numElem)
			{
				numElem = rowElem.size();
			}
		}

		//—v‘f‚Ì‰¼“o˜^
		for (int i = 0; i < numElem; i++)
		{
			vecLSI_Endless.push_back(LevelSetIndivisual());
		}

		//csv‚æ‚è‘ã“ü
		for (int i = 0; i < NUM_LSI; i++)
		{
			for (int j = 0; j < vecLSI_Endless.size(); j++)
			{
				//“Á’è—v‘f‚ªİ’è‚³‚ê‚Ä‚¢‚½‚ç
				if (csvLSI_Endless[i].size() >= j)
				{
					//‚»‚Ì‚Ü‚Ü‘ã“ü
					vecLSI_Endless[j][i] = csvLSI_Endless[i][j];
				}
				//İ’è‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚ç
				else
				{
					//1‚Â‘O‚Ì—v‘f‚ğƒRƒs[
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
		//ˆø”‚ÌƒŒƒxƒ‹‚Ìî•ñ‚ª“o˜^‚³‚ê‚Ä‚¢‚é
		if (arg_level <= vecLSI_Normal.size())
		{
			answer = vecLSI_Normal[arg_level - 1];
		}
		//“o˜^‚³‚ê‚Ä‚¢‚È‚¢
		else
		{
			answer = vecLSI_Normal[vecLSI_Normal.size() - 1];
		}
		break;
	case KochaEngine::SCOREATTAKMODE:
		//ˆø”‚ÌƒŒƒxƒ‹‚Ìî•ñ‚ª“o˜^‚³‚ê‚Ä‚¢‚é
		if (arg_level <= vecLSI_Endless.size())
		{
			answer = vecLSI_Endless[arg_level - 1];
		}
		//“o˜^‚³‚ê‚Ä‚¢‚È‚¢
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
