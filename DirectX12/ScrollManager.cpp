#include "ScrollManager.h"
#include "Input.h"
#include "LevelSetKeeper.h"

KochaEngine::ScrollManager* KochaEngine::ScrollManager::instance = nullptr;

KochaEngine::ScrollManager::ScrollManager()
	:scrollAmount(0.0f),
	 isBGMChange(false),
	 frameCount(0),
	 time(0),
	 scrollLevel(0)
{
	Initialize();
}

KochaEngine::ScrollManager::~ScrollManager()
{
}

KochaEngine::ScrollManager* KochaEngine::ScrollManager::GetInstance()
{
	if (!instance)
	{
		instance = new ScrollManager();
	}
	return instance;
}

void KochaEngine::ScrollManager::DeleteInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void KochaEngine::ScrollManager::Initialize()
{
	isBGMChange = false;
	scrollLevel = 1;
	scrollAmount = 0.50f;
	frameCount = 0;
	time = 0;
	ScrollSpeedUp();
}

void KochaEngine::ScrollManager::Update()
{
	LevelSetIndivisual lsi = LevelSetKeeper::GetInstance()->GetCurrentModeWithLevel(scrollLevel);
	//�w��b���o������
	if (time >= lsi.secChangeScrollSpeedInterval)
	{
		//���x�����グ��
		scrollLevel += 1;
		isBGMChange = true;
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
	LevelSetIndivisual lsi = LevelSetKeeper::GetInstance()->GetCurrentModeWithLevel(scrollLevel);
	scrollAmount = lsi.scrollSpeedHundredfold * 0.01f;

	//�ꉞ��Ԃ��Ƃɒl�𒲐��ł���悤�ɂ��Ƃ���
	//switch (scrollLevel)
	//{
	//case 1:
	//	scrollAmount = 0.50f;
	//	//�����Ȃ�
	//	break;
	//case 2:
	//	scrollAmount = 0.70f;
	//	//���p�`�p�`
	//	break;
	//case 3:
	//	scrollAmount = 0.80f;
	//	//���p�`�p�`����
	//	break;
	//case 4:
	//	scrollAmount = 1.00f;
	//	//���o��
	//	break;
	//case 5:
	//	scrollAmount = 1.20f;
	//	//���ꐯ
	//	break;
	//default:
	//	scrollAmount = scrollAmount + 0.1f;
	//	break;
	//}
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

void KochaEngine::ScrollManager::AddScrollLevel(const int arg_add)
{
	scrollLevel += arg_add;
}
