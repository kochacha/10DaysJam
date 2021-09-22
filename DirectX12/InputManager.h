#pragma once

namespace KochaEngine
{
	class InputManager
	{
	public:
		static bool DashKey();
		static bool SmashKey();
		static bool DecisionKey();
		static bool CancelKey();
		static bool PauseKey();
		static bool UpCursorKey();
		static bool DownCursorKey();
		static bool LeftKey();
		static bool RightKey();
		static bool DownKey();
		static bool UpKey();
		static bool RankingCheckKey();
	};
}