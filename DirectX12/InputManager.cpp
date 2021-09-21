#include "InputManager.h"
#include "Input.h"

bool KochaEngine::InputManager::DashKey()
{
    bool result = Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE);
    return result;
}

bool KochaEngine::InputManager::SmashKey()
{
    bool result = Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_RETURN);
    return result;
}

bool KochaEngine::InputManager::DecisionKey()
{
    bool result = Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_RETURN) || Input::TriggerKey(DIK_Z);
    return result;
}

bool KochaEngine::InputManager::CancelKey()
{
    bool result = Input::TriggerPadButton(XINPUT_GAMEPAD_B) || Input::TriggerKey(DIK_X);
    return result;
}

bool KochaEngine::InputManager::PauseKey()
{
    bool result = 
        Input::TriggerPadButton(XINPUT_GAMEPAD_START) 
        || Input::TriggerKey(DIK_Q) 
        || Input::TriggerKey(DIK_P);
    return result;
}

bool KochaEngine::InputManager::UpCursorKey()
{
    bool result = 
        Input::TriggerPadLStickUp() 
        || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_UP) 
        || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT) 
        || Input::TriggerKey(DIK_UP) 
        || Input::TriggerKey(DIK_LEFT)
        || Input::TriggerKey(DIK_W)
        || Input::TriggerKey(DIK_A);
    return result;
}

bool KochaEngine::InputManager::DownCursorKey()
{
    bool result =
        Input::TriggerPadLStickDown()
        || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_DOWN)
        || Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT)
        || Input::TriggerKey(DIK_DOWN)
        || Input::TriggerKey(DIK_RIGHT)
        || Input::TriggerKey(DIK_S)
        || Input::TriggerKey(DIK_D);
    return result;
}

bool KochaEngine::InputManager::LeftKey()
{
    bool result =
        Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_LEFT)
        || Input::TriggerKey(DIK_LEFT)
        || Input::TriggerKey(DIK_A);
    return result;
}

bool KochaEngine::InputManager::RightKey()
{
    bool result =
        Input::TriggerPadButton(XINPUT_GAMEPAD_DPAD_RIGHT)
        || Input::TriggerKey(DIK_RIGHT)
        || Input::TriggerKey(DIK_D);
    return result;
}

bool KochaEngine::InputManager::RankingCheckKey()
{
    bool result = 
        Input::TriggerKey(DIK_Q) 
        || Input::TriggerPadButton(XINPUT_GAMEPAD_X);
    return result;
}
