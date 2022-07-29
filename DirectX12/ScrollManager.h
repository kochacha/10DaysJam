#pragma once

namespace KochaEngine
{
	class ScrollManager
	{
	private:
		//1�x�ɃX�N���[�����鋗��
		float scrollAmount;
		bool isBGMChange;
		int frameCount;
		int time;//�b
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
		//�X�N���[������X�s�[�h�̒���
		void ScrollSpeedUp();
		bool IsBGMChange() { return isBGMChange; }
		void SetIsBGMChange(const bool arg_isBGMChange) { isBGMChange = arg_isBGMChange; }
		const float GetScrollAmount();
		const int GetScrollLevel();
		void AddScrollLevel(const int arg_add = 1);
	};
}
