#include "ScrollManager.h"
#include "Input.h"
#include "LevelSetKeeper.h"
#include "BossSpawner.h"

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
	//指定秒数経ったら
	if (time >= lsi.secChangeScrollSpeedInterval)
	{
		//レベルを上げる
		scrollLevel += 1;
		isBGMChange = true;
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
	LevelSetKeeper* keeper = LevelSetKeeper::GetInstance();
	LevelSetIndivisual lsi = keeper->GetCurrentModeWithLevel(scrollLevel);
	scrollAmount = lsi.scrollSpeedHundredfold * 0.01f;

	//エンドレスモードのみボス出現
	if (keeper->GetGameMode() != GameMode::SCOREATTAKMODE)
	{
		return;
	}

	if (keeper->IsSpawnBossThisLevel(scrollLevel))
	{
		BossSpawner::GetInstance()->SpawnBoss();
	}
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
