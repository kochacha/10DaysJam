#pragma once

namespace KochaEngine
{
	class ScrollManager
	{
	private:
		//1度にスクロールする距離
		float scrollAmount;
		int frameCount;
		int time;//秒
		int scrollLevel;

	public:
		ScrollManager();
		~ScrollManager();

		void Initialize();
		void Update();
		//スクロールするスピードの調整
		void ScrollSpeedUp();		
		const float GetScrollAmount();
		const int GetScrollLevel();
	};
}
