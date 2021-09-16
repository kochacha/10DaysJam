#pragma once

namespace KochaEngine
{
	class PauseManager
	{
	private:
		bool isPause;

	public:
		PauseManager();
		~PauseManager();

		void Initialize();
		void Update();

		bool IsPause() { return isPause; }
	};
}