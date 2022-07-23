#include "ScoreManager.h"
#include "CSVReader.h"
#include "Texture2D.h"
#include<fstream>

KochaEngine::ScoreManager::ScoreManager()
{
	char displayChar[3] = { '_' };
	for (int i = 0; i < RANK_COUNT; i++)
	{
		rankNumTex[i] = new Number(Vector2(350, 180 + 48 * i), Vector2(32, 32), 2);
		rankScoreTex[i] = new Number(Vector2(450, 180 + 48 * i), Vector2(32, 32), 8);
		nameTexts[i] = new Text(displayChar, Vector2(750, 180 + 48 * i), Vector2(32, 32));
	}
	scoreTex = new Number(Vector2(950, 75), Vector2(32, 32), 8);
	quotaScoreTex = new Number(Vector2(950, 34), Vector2(32, 32), 8);
	resultScoreTex = new Number(Vector2(450, 180 + 48 * 8), Vector2(32, 32), 8);
	backTex = new Texture2D("Resources/rankBack.png", Vector2(150, 129), Vector2(1000, 550), 0);
	onlineTex = new Texture2D("Resources/online.png", Vector2(190, 170), Vector2(64, 64), 0);
	offlineTex = new Texture2D("Resources/offline.png", Vector2(190, 170), Vector2(64, 64), 0);
	resultName = new Text(displayChar, Vector2(750, 180 + 48 * 8), Vector2(32, 32));
	yourScoreText = new Text(displayChar, Vector2(318, 180 + 48 * 8), Vector2(32, 32));

	Initialize();
}

KochaEngine::ScoreManager::~ScoreManager()
{
	for (int i = 0; i < RANK_COUNT; i++)
	{
		delete rankNumTex[i];
		delete rankScoreTex[i];
		delete nameTexts[i];
	}
	delete scoreTex;
	delete quotaScoreTex;
	delete backTex;
	delete onlineTex;
	delete offlineTex;
	delete resultScoreTex;
	delete resultName;
	delete yourScoreText;
}

void KochaEngine::ScoreManager::Initialize()
{
	for (int i = 0; i < RANK_COUNT; i++)
	{
		rankNumTex[i]->Init();
		rankScoreTex[i]->Init();
	}
	scoreTex->Init();

	score = 0;

	LoadRankData();
}

void KochaEngine::ScoreManager::Draw(bool arg_isShow)
{
	if (arg_isShow)
	{	
		backTex->Draw();
		offlineTex->Draw();
		for (int i = 0; i < RANK_COUNT; i++)
		{
			rankNumTex[i]->Draw(i + 1);
			rankScoreTex[i]->Draw(rankScore[i]);
		}
	}
	scoreTex->Draw(score);
}

void KochaEngine::ScoreManager::DrawOnlineRinking(bool arg_isShow, std::vector<std::string> arg_rankNames, std::vector<int> arg_rankScores)
{
	if (arg_isShow)
	{		
		backTex->Draw();
		onlineTex->Draw();
		std::vector<std::string> rankNames = arg_rankNames;
		std::vector<int> rankScores = arg_rankScores;

		for (int i = 0; i < RANK_COUNT; i++)
		{
			rankNumTex[i]->Draw(Vector2(350, 180 + 70 * i), i + 1);
			rankScoreTex[i]->Draw(Vector2(450, 180 + 70 * i),rankScores[i]);
			
			nameTexts[i]->SetText(arg_rankNames[i].c_str());
			nameTexts[i]->Draw(Vector2(750, 180 + 70 * i));
		}
	}
	scoreTex->Draw(score);
}

void KochaEngine::ScoreManager::DrawResultRanking(bool arg_isShow, std::vector<std::string> arg_rankNames, std::vector<int> arg_rankScores, const std::string arg_resultMyName)
{
	if (arg_isShow)
	{
		backTex->Draw();
		onlineTex->Draw();
		std::vector<std::string> rankNames = arg_rankNames;
		std::vector<int> rankScores = arg_rankScores;

		for (int i = 0; i < RANK_COUNT; i++)
		{
			rankNumTex[i]->Draw(Vector2(350, 180 + 48 * i),i + 1);
			rankScoreTex[i]->Draw(Vector2(450, 180 + 48 * i), rankScores[i]);

			nameTexts[i]->SetText(arg_rankNames[i].c_str());
			nameTexts[i]->Draw(Vector2(750, 180 + 48 * i));
		}
		resultName->SetText(arg_resultMyName.c_str());
		resultName->Draw();
		resultScoreTex->Draw(score);
		yourScoreText->SetText("You");
		yourScoreText->Draw();
	}
	scoreTex->Draw(score);
}

void KochaEngine::ScoreManager::DrawQuotaScore(const int arg_quotaScore)
{
	quotaScoreTex->Draw(arg_quotaScore);
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

void KochaEngine::ScoreManager::AddScore(const int arg_addScore)
{
	score += arg_addScore;
}

void KochaEngine::ScoreManager::SaveScore()
{
	UpdateRanking(score);
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
	CSVReader* reader = CSVReader::GetInstance();
	reader->LoadCSV("Resources/ScoreData.txt", "LocalScoreData", true);
	std::vector<std::vector<int>> tmp = reader->GetMapData("LocalScoreData");
	for (int i = 0; i < RANK_COUNT; i++)
	{
		rankScore[i] = tmp[0][i];
	}
}