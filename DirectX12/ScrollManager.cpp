#include "ScrollManager.h"

KochaEngine::ScrollManager::ScrollManager()
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
	if (time >= 5)
	{
		scrollLevel += 1;
		ScrollSpeedUp();
		time = 0;
	}

	if (frameCount >= 60)
	{
		time++;
		frameCount = 0;
		return;
	}
	frameCount++;
}

void KochaEngine::ScrollManager::ScrollSpeedUp()
{
	//ˆê‰‹æŠÔ‚²‚Æ‚É’l‚ğ’²®‚Å‚«‚é‚æ‚¤‚É‚µ‚Æ‚¢‚½
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
