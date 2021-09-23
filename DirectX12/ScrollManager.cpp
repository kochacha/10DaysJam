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
	//�ꉞ��Ԃ��Ƃɒl�𒲐��ł���悤�ɂ��Ƃ���
	switch (scrollLevel)
	{
	case 1:
		scrollAmount = 0.25f;
		break;
	case 2:
		scrollAmount = 0.40f;
		break;
	case 3:
		scrollAmount = 0.55f;
		break;
	case 4:
		scrollAmount = 0.65f;
		break;
	case 5:
		scrollAmount = 0.75f;
		break;
	case 6:
		scrollAmount = 0.85f;
		break;
	case 7:
		scrollAmount = 0.95f;
		break;
	case 8:
		scrollAmount = 1.00f;
		break;
	default:
		scrollAmount = scrollAmount + 0.025f;
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
