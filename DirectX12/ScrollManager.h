#pragma once

namespace KochaEngine
{
	class ScrollManager
	{
	private:
		float scrollAmount;
		int frameCount;
		int time;//•b
		int scrollLevel;
	public:
		ScrollManager();
		~ScrollManager();

		void Initialize();
		void Update();

		void ScrollSpeedUp();

		const float GetScrollAmount();

		const int GetScrollLevel();
	};
}
