#pragma once
#include <cassert>

namespace KochaEngine
{
	const int NUM_LSAM = 9;
	const int NUM_LSI = 6;

	struct LevelSetAllMode
	{
		//左の壁までスマッシュで到達したときのスコア
		int scoreSmashLeftLimit;
		//アイテムを取得したときのスコア
		int scoreGetItem;
		//トゲをスマッシュしたときのスコア
		int scoreSmashSpine;
		//アイテム1個分のスマッシュパワー
		int amountReductionItem;
		//トゲ1個分のスマッシュパワー
		int amountReductionSpine;
		//スマッシュ中に加算され続けるスコア
		int scoreGainSmashing;
		//ノーマルモードのボス出現までのノルマ
		int quotaAppearBoss;
		//ボスを撃破したスコア
		int scoreDefeatBoss;
		//トゲの見切れ距離
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
		//アイテム・トゲの生成間隔フレーム
		int frameObjectEmitInterval;
		//生成されたトゲが動く確率
		int percentageSpineMove;
		//スクロールの速さを100倍した数
		int scrollSpeedHundredfold;
		//スクロールスピード変化までの秒数
		int secChangeScrollSpeedInterval;
		//ボスが動く確率
		int percentageBossMove;
		//オブジェクトのうち、アイテムが生成される確率
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