#include "PauseManager.h"
#include "Input.h"
#include "Application.h"

KochaEngine::PauseManager::PauseManager()
{
	menu[0] = new Sprite("Resources/Menu1.png", Vector2(440, 200), Vector2(400, 600), 0);
	menu[1] = new Sprite("Resources/Menu2.png", Vector2(440, 200), Vector2(400, 600), 0);
	menu[2] = new Sprite("Resources/Menu3.png", Vector2(40, 200), Vector2(1200, 600), 0);
	menu[3] = new Sprite("Resources/Menu4.png", Vector2(440, 200), Vector2(400, 600), 0);
	cursor = new Sprite("Resources/cursor.png", Vector2(0, 0), Vector2(32, 32), 0);
	Initialize();
}

KochaEngine::PauseManager::~PauseManager()
{
	for (int i = 0; i < 4; i++)
	{
		delete menu[i];
	}
	delete cursor;
}

void KochaEngine::PauseManager::Initialize()
{
	isPause = false;
	menuType = PauseManager::PauseUI::MENU_TAB;
	nowMenu = PauseManager::Menu::BACK;
	nowOption = PauseManager::Option::COLOR_PALETTE;
	nowColorPalette = PauseManager::ColorPalette::GAMEBOY;
	nowSoundMixer = PauseManager::SoundMixer::MASTER_VOLUME;
}

void KochaEngine::PauseManager::Update()
{
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_START))
	{
		PauseChange();
	}

	if (!isPause) return;

	switch (menuType)
	{
	case KochaEngine::PauseManager::PauseUI::MENU_TAB:
		MenuTab();
		break;
	case KochaEngine::PauseManager::PauseUI::OPTION_TAB:
		OptionTab();
		break;
	case KochaEngine::PauseManager::PauseUI::COLOR_TAB:
		ColorTab();
		break;
	case KochaEngine::PauseManager::PauseUI::SOUND_TAB:
		SoundTab();
		break;
	default:
		break;
	}
}

void KochaEngine::PauseManager::Draw()
{
	if (!isPause) return;

	switch (menuType)
	{
	case KochaEngine::PauseManager::PauseUI::MENU_TAB:
		menu[0]->Draw();
		break;
	case KochaEngine::PauseManager::PauseUI::OPTION_TAB:
		menu[1]->Draw();
		break;
	case KochaEngine::PauseManager::PauseUI::COLOR_TAB:
		menu[2]->Draw();
		break;
	case KochaEngine::PauseManager::PauseUI::SOUND_TAB:
		menu[3]->Draw();
		break;
	default:
		break;
	}

	cursor->Draw();
}

void KochaEngine::PauseManager::MenuTab()
{
	switch (nowMenu)
	{
	case KochaEngine::PauseManager::BACK:
		cursor->SetPosition(Vector2(490, 360));
		if (Input::TriggerPadLStickUp()) { nowMenu = PauseManager::EXIT; }
		if (Input::TriggerPadLStickDown()) { nowMenu = PauseManager::RESET; }
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) { PauseChange(); }
		break;
	case KochaEngine::PauseManager::RESET:
		cursor->SetPosition(Vector2(490, 455));
		if (Input::TriggerPadLStickUp()) { nowMenu = PauseManager::BACK; }
		if (Input::TriggerPadLStickDown()) { nowMenu = PauseManager::OPTION; }
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) { }
		break;
	case KochaEngine::PauseManager::OPTION:
		cursor->SetPosition(Vector2(490, 550));
		if (Input::TriggerPadLStickUp()) { nowMenu = PauseManager::RESET; }
		if (Input::TriggerPadLStickDown()) { nowMenu = PauseManager::EXIT; }
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) { menuType = PauseManager::PauseUI::OPTION_TAB; }
		break;
	case KochaEngine::PauseManager::EXIT:
		cursor->SetPosition(Vector2(490, 645));
		if (Input::TriggerPadLStickUp()) { nowMenu = PauseManager::OPTION; }
		if (Input::TriggerPadLStickDown()) { nowMenu = PauseManager::BACK; }
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) { Application::isExit = true; }
		break;
	default:
		break;
	}
}

void KochaEngine::PauseManager::OptionTab()
{
	switch (nowOption)
	{
	case KochaEngine::PauseManager::COLOR_PALETTE:
		cursor->SetPosition(Vector2(490, 360));
		if (Input::TriggerPadLStickUp() || Input::TriggerPadLStickDown()) { nowOption = PauseManager::SOUND_MIXER; }
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) { menuType = PauseManager::PauseUI::COLOR_TAB; }
		break;
	case KochaEngine::PauseManager::SOUND_MIXER:
		cursor->SetPosition(Vector2(490, 455));
		if (Input::TriggerPadLStickUp() || Input::TriggerPadLStickDown()) { nowOption = PauseManager::COLOR_PALETTE; }
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) { menuType = PauseManager::PauseUI::SOUND_TAB; }
		break;
	default:
		break;
	}
}

void KochaEngine::PauseManager::ColorTab()
{
	switch (nowColorPalette)
	{
	case KochaEngine::PauseManager::GAMEBOY:
		Application::paletteType = KochaEngine::PaletteType::GAMEBOY;
		cursor->SetPosition(Vector2(90, 360));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::PALETTE10; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::SEPIA; }
		break;
	case KochaEngine::PauseManager::SEPIA:
		Application::paletteType = KochaEngine::PaletteType::SEPIA;
		cursor->SetPosition(Vector2(90, 455));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::GAMEBOY; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::PALETTE1; }
		break;
	case KochaEngine::PauseManager::PALETTE1:
		Application::paletteType = KochaEngine::PaletteType::PALETTE1;
		cursor->SetPosition(Vector2(90, 550));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::SEPIA; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::PALETTE2; }
		break;
	case KochaEngine::PauseManager::PALETTE2:
		Application::paletteType = KochaEngine::PaletteType::PALETTE2;
		cursor->SetPosition(Vector2(90, 645));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::PALETTE1; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::PALETTE3; }
		break;
	case KochaEngine::PauseManager::PALETTE3:
		Application::paletteType = KochaEngine::PaletteType::PALETTE3;
		cursor->SetPosition(Vector2(490, 360));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::PALETTE2; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::PALETTE4; }
		break;
	case KochaEngine::PauseManager::PALETTE4:
		cursor->SetPosition(Vector2(490, 455));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::PALETTE3; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::PALETTE5; }
		break;
	case KochaEngine::PauseManager::PALETTE5:
		cursor->SetPosition(Vector2(490, 550));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::PALETTE4; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::PALETTE6; }
		break;
	case KochaEngine::PauseManager::PALETTE6:
		cursor->SetPosition(Vector2(490, 645));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::PALETTE5; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::PALETTE7; }
		break;
	case KochaEngine::PauseManager::PALETTE7:
		cursor->SetPosition(Vector2(890, 360));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::PALETTE6; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::PALETTE8; }
		break;
	case KochaEngine::PauseManager::PALETTE8:
		cursor->SetPosition(Vector2(890, 455));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::PALETTE7; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::PALETTE9; }
		break;
	case KochaEngine::PauseManager::PALETTE9:
		cursor->SetPosition(Vector2(890, 550));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::PALETTE8; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::PALETTE10; }
		break;
	case KochaEngine::PauseManager::PALETTE10:
		cursor->SetPosition(Vector2(890, 645));
		if (Input::TriggerPadLStickUp()) { nowColorPalette = PauseManager::PALETTE9; }
		if (Input::TriggerPadLStickDown()) { nowColorPalette = PauseManager::GAMEBOY; }
		break;
	default:
		break;
	}

	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) { PauseChange(); }
}

void KochaEngine::PauseManager::SoundTab()
{
	switch (nowSoundMixer)
	{
	case KochaEngine::PauseManager::MASTER_VOLUME:
		break;
	case KochaEngine::PauseManager::BGM_VOLUME:
		break;
	case KochaEngine::PauseManager::SE_VOLUME:
		break;
	default:
		break;
	}
}

void KochaEngine::PauseManager::PauseChange()
{
	isPause = !isPause;
	menuType = PauseManager::PauseUI::MENU_TAB;
	nowMenu = PauseManager::Menu::BACK;
	nowOption = PauseManager::Option::COLOR_PALETTE;
	nowSoundMixer = PauseManager::SoundMixer::MASTER_VOLUME;
}
