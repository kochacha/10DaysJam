#pragma once
#include "Number.h"

#define RANK_COUNT 10

namespace KochaEngine
{
	class ScoreManager
	{
	private:

		Number* scoreTex;
		Number* rankNumTex[RANK_COUNT];
		Number* rankScoreTex[RANK_COUNT];

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

		void UpdateRanking(int arg_score);
	};
}