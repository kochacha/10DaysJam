#pragma once
#include "Number.h"
#include "Text.h"
#include <vector>
#include <string>

#define RANK_COUNT 10

namespace KochaEngine
{
	class Texture2D;
	class ScoreManager
	{
	private:

		Texture2D* backTex;
		Texture2D* onlineTex;
		Texture2D* offlineTex;

		Number* scoreTex;
		Number* rankNumTex[RANK_COUNT];
		Number* rankScoreTex[RANK_COUNT];

		Text* nameTexts[RANK_COUNT];

		/*std::vector<std::string> rankNames;
		std::vector<int> rankScores;*/


		int score;
		std::vector<int> rankScore;
		int rankScoreData[RANK_COUNT];

		void SaveRankData(int arg_rank, int arg_score);
		void LoadRankData();

	public:
		ScoreManager();
		~ScoreManager();

		void Initialize();
		void Draw(bool arg_isShow);
		void DrawOnlineRinking(bool arg_isShow, std::vector<std::string> arg_rankNames, std::vector<int> arg_rankScores);

		void UpdateRanking(const int arg_score);
		void SetScore(const int arg_score) { score = arg_score; }
		void AddScore(const int arg_addScore);
		void SaveScore();

		const int GetScore() { return score; }
	};
}