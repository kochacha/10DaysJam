#include "ScrollManager.h"
#include "Input.h"

KochaEngine::ScrollManager::ScrollManager()
	:scrollAmount(0.0f),
	 frameCount(0),
	 time(0),
	 scrollLevel(0)
{
	Initialize();
}

KochaEngine::ScrollManager::~ScrollManager()
{
}

void KochaEngine::ScrollManager::Initialize()
{
	scrollLevel = 1;
	scrollAmount = 0.30f;
	frameCount = 0;
	time = 0;
	ScrollSpeedUp();
}

void KochaEngine::ScrollManager::Update()
{
	//�w��b���o������
	if (time >= 10)
	{
		//���x�����グ��
		scrollLevel += 1;
		ScrollSpeedUp();
		time = 0;
	}

	//60�t���[����1�b
	frameCount++;
	if (frameCount >= 60)
	{
		time++;
		frameCount = 0;		
	}
}

void KochaEngine::ScrollManager::ScrollSpeedUp()
{
	////�ꉞ��Ԃ��Ƃɒl�𒲐��ł���悤�ɂ��Ƃ���
	//switch (scrollLevel)
	//{
	//case 1:
	//	scrollAmount = 0.30f;
	//	break;
	//case 2:
	//	scrollAmount = 0.40f;
	//	break;
	//case 3:
	//	scrollAmount = 0.50f;
	//	break;
	//case 4:
	//	scrollAmount = 0.60f;
	//	break;
	//case 5:
	//	scrollAmount = 0.80f;
	//	break;
	//case 6:
	//	scrollAmount = 0.90f;
	//	break;
	//case 7:
	//	scrollAmount = 1.00f;
	//	break;
	//case 8:
	//	scrollAmount = 1.10f;
	//	break;
	//default:
	//	scrollAmount = scrollAmount + 0.05f;
	//	break;
	//}

	scrollAmount = scrollAmount + 0.05f;
}

const float KochaEngine::ScrollManager::GetScrollAmount()
{
#ifdef _DEBUG
	static bool isScroll = true;
	if (Input::TriggerKey(DIK_LCONTROL))
	{
		isScroll = !isScroll;
	}

	if (isScroll)
	{
		return scrollAmount;
	}
	else
	{
		return 0;
	}
#endif // _DEBUG

	return scrollAmount;
}

const int KochaEngine::ScrollManager::GetScrollLevel()
{
	return scrollLevel;
}
