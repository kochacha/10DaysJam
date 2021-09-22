#pragma once

namespace KochaEngine
{
	class ScrollManager
	{
	private:
		//1�x�ɃX�N���[�����鋗��
		float scrollAmount;
		int frameCount;
		int time;//�b
		int scrollLevel;

	public:
		ScrollManager();
		~ScrollManager();

		void Initialize();
		void Update();
		//�X�N���[������X�s�[�h�̒���
		void ScrollSpeedUp();		
		const float GetScrollAmount();
		const int GetScrollLevel();
	};
}
