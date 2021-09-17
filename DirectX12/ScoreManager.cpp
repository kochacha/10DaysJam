#include "ScoreManager.h"
#include "CSVReader.h"
#include<fstream>

KochaEngine::ScoreManager::ScoreManager()
{
	for (int i = 0; i < RANK_COUNT; i++)
	{
		rankNumTex[i] = new Number(Vector2(400, 180 + 48 * i), Vector2(32, 32), 2);
		rankScoreTex[i] = new Number(Vector2(500, 180 + 48 * i), Vector2(32, 32), 8);
	}
	scoreTex = new Number(Vector2(950, 75), Vector2(32, 32), 8);
	Initialize();
}

KochaEngine::ScoreManager::~ScoreManager()
{
	for (int i = 0; i < RANK_COUNT; i++)
	{
		delete rankNumTex[i];
		delete rankScoreTex[i];
	}
	delete scoreTex;
}

void KochaEngine::ScoreManager::Initialize()
{
	for (int i = 0; i < RANK_COUNT; i++)
	{
		rankNumTex[i]->Init();
		rankScoreTex[i]->Init();
	}
	scoreTex->Init();

	score = 100;

	UpdateRanking(score);
}

void KochaEngine::ScoreManager::Draw(bool arg_isShow)
{
	if (arg_isShow)
	{
		for (int i = 0; i < RANK_COUNT; i++)
		{
			rankNumTex[i]->Draw(i + 1);
			rankScoreTex[i]->Draw(rankScore[i]);
		}
	}
	scoreTex->Draw(score);
}

void KochaEngine::ScoreManager::UpdateRanking(const int arg_score)
{
	LoadRankData();

	for (int i = RANK_COUNT - 1; i > -1; i--)
	{
		if (rankScore[i] < score)
		{
			int a = rankScore[i];
			rankScore[i] = score;
			if (i != RANK_COUNT - 1)
			{
				rankScore[i + 1] = a;
			}
		}
	}

	for (int i = 0; i < RANK_COUNT; i++)
	{
		SaveRankData(i, rankScore[i]);
	}
}

void KochaEngine::ScoreManager::SaveRankData(int arg_rank, int arg_score)
{
	if (arg_rank <= 0) { return; }
	if (arg_score < rankScore[arg_rank]) { return; }
	rankScore[arg_rank] = arg_score;
	std::ofstream ofs("Resources/ScoreData.txt");
	for (int i = 0; i < RANK_COUNT; i++)
	{
		ofs << std::to_string(rankScore[i]);
		ofs << ',';
	}
	ofs.close();
}

void KochaEngine::ScoreManager::LoadRankData()
{
	rankScore.resize(RANK_COUNT);
	CSVReader reader;
	reader.LoadCSV(0, "Resources/ScoreData.txt");
	std::vector<std::vector<int>> tmp = reader.GetMapData(0);
	for (int i = 0; i < RANK_COUNT; i++)
	{
		rankScore[i] = tmp[0][i];
	}
}