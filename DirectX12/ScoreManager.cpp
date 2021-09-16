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
}

void KochaEngine::ScoreManager::Draw(bool arg_isShow)
{
	if (arg_isShow)
	{
		for (int i = 0; i < RANK_COUNT; i++)
		{
			rankNumTex[i]->Draw(i + 1);
			rankScoreTex[i]->Draw(648375);
		}
	}
	scoreTex->Draw(648375);
}

void KochaEngine::ScoreManager::SaveRankData(int arg_rank, int arg_score)
{
	if (arg_rank <= 0) { return; }
	if (arg_score < rankScore[arg_rank - 1]) { return; }
	rankScore[arg_rank - 1] = arg_score;
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