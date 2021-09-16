#include "PauseManager.h"
#include "Input.h"

KochaEngine::PauseManager::PauseManager()
{
	Initialize();
}

KochaEngine::PauseManager::~PauseManager()
{
}

void KochaEngine::PauseManager::Initialize()
{
	isPause = false;
}

void KochaEngine::PauseManager::Update()
{
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_START))
	{
		isPause = !isPause;
	}
}
