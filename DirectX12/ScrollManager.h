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

		static ScrollManager* instance;
		ScrollManager();
		~ScrollManager();
		ScrollManager(const ScrollManager& another) = delete;
		ScrollManager& operator=(const ScrollManager& another) = delete;

	public:
		static ScrollManager* GetInstance();
		static void DeleteInstance();

		void Initialize();
		void Update();
		//スクロールするスピードの調整
		void ScrollSpeedUp();		
		const float GetScrollAmount();
		const int GetScrollLevel();
	};
}
