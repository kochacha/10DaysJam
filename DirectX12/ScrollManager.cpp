#include "ScrollManager.h"

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
	frameCount = 0;
	time = 0;
	ScrollSpeedUp();
}

void KochaEngine::ScrollManager::Update()
{
	//指定秒数経ったら
	if (time >= 10)
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
		scrollAmount = 0.20f;
		break;
	case 2:
		scrollAmount = 0.30f;
		break;
	case 3:
		scrollAmount = 0.35f;
		break;
	case 4:
		scrollAmount = 0.40f;
		break;
	case 5:
		scrollAmount = 0.45f;
		break;
	case 6:
		scrollAmount = 0.50f;
		break;
	case 7:
		scrollAmount = 0.55f;
		break;
	case 8:
		scrollAmount = 0.60f;
		break;
	default:
		scrollAmount = scrollAmount + 0.05f;
		break;
	}
}

const float KochaEngine::ScrollManager::GetScrollAmount()
{
	return scrollAmount;
}

const int KochaEngine::ScrollManager::GetScrollLevel()
{
	return scrollLevel;
}
