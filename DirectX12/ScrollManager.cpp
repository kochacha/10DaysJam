#include "ScrollManager.h"
#include "Input.h"

KochaEngine::ScrollManager* KochaEngine::ScrollManager::instance = nullptr;

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
	scrollLevel = 1;
	scrollAmount = 0.40f;
	frameCount = 0;
	time = 0;
	ScrollSpeedUp();
}

void KochaEngine::ScrollManager::Update()
{
	//指定秒数経ったら
	if (time >= 30)
	{
		//レベルを上げる
		scrollLevel += 1;
		ScrollSpeedUp();
		time = 0;
	}

	//60フレームで1秒
	frameCount++;
	if (frameCount >= 60)
	{
		time++;
		frameCount = 0;		
	}
}

void KochaEngine::ScrollManager::ScrollSpeedUp()
{
	//一応区間ごとに値を調整できるようにしといた
	switch (scrollLevel)
	{
	case 1:
		scrollAmount = 0.40f;
		//何もなし
		break;
	case 2:
		scrollAmount = 0.70f;
		//星パチパチ
		break;
	case 3:
		scrollAmount = 1.00f;
		//星パチパチ強化
		break;
	case 4:
		scrollAmount = 1.20f;
		//月出現
		break;
	case 5:
		scrollAmount = 1.40f;
		//流れ星
		break;
	case 6:
		scrollAmount = 1.60f;
		//流れ星強化
		break;
	case 7:
		scrollAmount = 1.80f;
		break;
	default:
		scrollAmount = scrollAmount + 0.1f;
		break;
	}

	//scrollAmount = scrollAmount + 0.05f;
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
