#pragma once
#include "LevelSet.h"
#include "GameMode.h"
#include <vector>

namespace KochaEngine
{
	class LevelSetKeeper
	{
	public:
		static LevelSetKeeper* GetInstance();
		static void DeleteInstance();

		void StoreLevelSet();
		const std::vector<LevelSetAllMode>& GetVecLSAM()const;
		const std::vector<LevelSetIndivisual>& GetVecLSI_Normal()const;
		const std::vector<LevelSetIndivisual>& GetVecLSI_Endless()const;
		GameMode GetGameMode()const;
		void SetGameMode(const GameMode arg_mode);

	private:
		static LevelSetKeeper* instance;
		LevelSetKeeper();
		~LevelSetKeeper();
		LevelSetKeeper(const LevelSetKeeper& another) = delete;
		LevelSetKeeper& operator=(const LevelSetKeeper& another) = delete;

		std::vector<LevelSetAllMode> vecLSAM;
		std::vector<LevelSetIndivisual> vecLSI_Normal;
		std::vector<LevelSetIndivisual> vecLSI_Endless;
		GameMode mode;
	};
}