#pragma once

namespace KochaEngine
{
	class GameSetting
	{
	public:
		static const int SETTING_VOL = 7;

		static int masterVolume;
		static int bgmVolume;
		static int seVolume;
		static bool isDashData;
		static bool isSmashData;
		static bool isScoreData;
		static bool isVibData;
	};
}