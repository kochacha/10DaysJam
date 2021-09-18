#include "PauseManager.h"
#include "Input.h"
#include "Application.h"
#include "GameSetting.h"
#include "Audio.h"

KochaEngine::PauseManager::PauseManager()
{
	menu[0] = new Sprite("Resources/Menu1.png", Vector2(440, 200), Vector2(400, 600), 0);
	menu[1] = new Sprite("Resources/Menu2.png", Vector2(440, 200), Vector2(400, 600), 0);
	menu[2] = new Sprite("Resources/Menu3.png", Vector2(40, 200), Vector2(1200, 600), 0);
	menu[3] = new Sprite("Resources/Menu4.png", Vector2(440, 200), Vector2(400, 600), 0);
	cursor = new Sprite("Resources/cursor.png", Vector2(0, 0), Vector2(32, 32), 0);
	_cursor = new Sprite("Resources/cursor.png", Vector2(0, 0), Vector2(32, 32), 180);
	soundNum[0] = new Number(Vector2(720, 360), Vector2(32, 32), 2);
	soundNum[1] = new Number(Vector2(720, 455), Vector2(32, 32), 2);
	soundNum[2] = new Number(Vector2(720, 550), Vector2(32, 32), 2);

	se = new Audio();

	Initialize();
}

KochaEngine::PauseManager::~PauseManager()
{
	for (int i = 0; i < 4; i++)
	{
		delete menu[i];
	}
	for (int i = 0; i < 3; i++)
	{
		delete soundNum[i];
	}
	delete cursor;
	delete _cursor;
	delete se;
}

void KochaEngine::PauseManager::Initialize()
{
	isPause = false;
	isSoundMixer = false;

	for (int i = 0; i < 3; i++)
	{
		soundNum[i]->Init();
	}

	menuType = PauseManager::PauseUI::MENU_TAB;
	nowMenu = PauseManager::Menu::BACK;
	nowOption = PauseManager::Option::COLOR_PALETTE;
	nowColorPalette = PauseManager::ColorPalette::GAMEBOY;
	nowSoundMixer = PauseManager::SoundMixer::MASTER_VOLUME;

	se->Init();
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
}

void KochaEngine::PauseManager::Update()
{
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);

	if (Input::TriggerPadButton(XINPUT_GAMEPAD_START))
	{
		se->PlayWave("Resources/Sound/select.wav", seVolume);
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
		soundNum[0]->Draw(GameSetting::masterVolume);
		soundNum[1]->Draw(GameSetting::bgmVolume);
		soundNum[2]->Draw(GameSetting::seVolume);
		break;
	default:
		break;
	}

	cursor->Draw();
	if (!isSoundMixer) return;
	_cursor->Draw();
}

void KochaEngine::PauseManager::MenuTab()
{
	switch (nowMenu)
	{
	case KochaEngine::PauseManager::BACK:
		cursor->SetPosition(Vector2(CENTER_POS_X, 360));
		if (Input::TriggerPadLStickUp()) 
		{
			nowMenu = PauseManager::EXIT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{
			nowMenu = PauseManager::RESET;;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) 
		{ 
			PauseChange(); 
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::RESET:
		cursor->SetPosition(Vector2(CENTER_POS_X, 455));
		if (Input::TriggerPadLStickUp()) 
		{
			nowMenu = PauseManager::BACK;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{
			nowMenu = PauseManager::OPTION;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) 
		{ 
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::OPTION:
		cursor->SetPosition(Vector2(CENTER_POS_X, 550));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowMenu = PauseManager::RESET;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowMenu = PauseManager::EXIT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) 
		{ 
			menuType = PauseManager::PauseUI::OPTION_TAB;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::EXIT:
		cursor->SetPosition(Vector2(CENTER_POS_X, 645));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowMenu = PauseManager::OPTION;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowMenu = PauseManager::BACK;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) 
		{ 
			Application::isExit = true;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	default:
		break;
	}

	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B))
	{
		PauseChange();
		se->PlayWave("Resources/Sound/select.wav", seVolume);
	}
}

void KochaEngine::PauseManager::OptionTab()
{
	switch (nowOption)
	{
	case KochaEngine::PauseManager::COLOR_PALETTE:
		cursor->SetPosition(Vector2(CENTER_POS_X, 360));
		if (Input::TriggerPadLStickUp() || Input::TriggerPadLStickDown()) 
		{ 
			nowOption = PauseManager::SOUND_MIXER; 
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) 
		{ 
			menuType = PauseManager::PauseUI::COLOR_TAB;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::SOUND_MIXER:
		cursor->SetPosition(Vector2(CENTER_POS_X, 455));
		if (Input::TriggerPadLStickUp() || Input::TriggerPadLStickDown()) 
		{ 
			nowOption = PauseManager::COLOR_PALETTE;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_A)) 
		{ 
			menuType = PauseManager::PauseUI::SOUND_TAB;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	default:
		break;
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B))
	{
		PauseChange();
		se->PlayWave("Resources/Sound/select.wav", seVolume);
	}
}

void KochaEngine::PauseManager::ColorTab()
{
	switch (nowColorPalette)
	{
	case KochaEngine::PauseManager::GAMEBOY:
		Application::paletteType = KochaEngine::PaletteType::GAMEBOY;
		cursor->SetPosition(Vector2(LEFT_POS_X, 360));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::PALETTE10;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::SEPIA;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::SEPIA:
		Application::paletteType = KochaEngine::PaletteType::SEPIA;
		cursor->SetPosition(Vector2(LEFT_POS_X, 455));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::GAMEBOY; 
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::PALETTE1;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE1:
		Application::paletteType = KochaEngine::PaletteType::PALETTE1;
		cursor->SetPosition(Vector2(LEFT_POS_X, 550));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::SEPIA;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::PALETTE2;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE2:
		Application::paletteType = KochaEngine::PaletteType::PALETTE2;
		cursor->SetPosition(Vector2(LEFT_POS_X, 645));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::PALETTE1;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::PALETTE3;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE3:
		Application::paletteType = KochaEngine::PaletteType::PALETTE3;
		cursor->SetPosition(Vector2(CENTER_POS_X, 360));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::PALETTE2;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::PALETTE4;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE4:
		Application::paletteType = KochaEngine::PaletteType::PALETTE4;
		cursor->SetPosition(Vector2(CENTER_POS_X, 455));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::PALETTE3;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::PALETTE5;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE5:
		Application::paletteType = KochaEngine::PaletteType::PALETTE5;
		cursor->SetPosition(Vector2(CENTER_POS_X, 550));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::PALETTE4;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::PALETTE6;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE6:
		Application::paletteType = KochaEngine::PaletteType::PALETTE6;
		cursor->SetPosition(Vector2(CENTER_POS_X, 645));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::PALETTE5;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::PALETTE7;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE7:
		Application::paletteType = KochaEngine::PaletteType::PALETTE7;
		cursor->SetPosition(Vector2(RIGHT_POS_X, 360));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::PALETTE6;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::PALETTE8;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE8:
		Application::paletteType = KochaEngine::PaletteType::PALETTE8;
		cursor->SetPosition(Vector2(RIGHT_POS_X, 455));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::PALETTE7;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::PALETTE9;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE9:
		Application::paletteType = KochaEngine::PaletteType::PALETTE9;
		cursor->SetPosition(Vector2(RIGHT_POS_X, 550));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::PALETTE8;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::PALETTE10;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE10:
		Application::paletteType = KochaEngine::PaletteType::PALETTE10;
		cursor->SetPosition(Vector2(RIGHT_POS_X, 645));
		if (Input::TriggerPadLStickUp()) 
		{ 
			nowColorPalette = PauseManager::PALETTE9;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (Input::TriggerPadLStickDown()) 
		{ 
			nowColorPalette = PauseManager::GAMEBOY;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	default:
		break;
	}

	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerPadButton(XINPUT_GAMEPAD_B)) 
	{ 
		PauseChange();
		se->PlayWave("Resources/Sound/select.wav", seVolume);
	}
}

void KochaEngine::PauseManager::SoundTab()
{
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
	{
		isSoundMixer = !isSoundMixer;
		se->PlayWave("Resources/Sound/select.wav", seVolume);
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_B))
	{
		PauseChange();
		se->PlayWave("Resources/Sound/select.wav", seVolume);
	}

	switch (nowSoundMixer)
	{
	case KochaEngine::PauseManager::MASTER_VOLUME:
		if (!isSoundMixer)
		{
			cursor->SetPosition(Vector2(CENTER_POS_X, 360));
			if (Input::TriggerPadLStickUp()) 
			{ 
				nowSoundMixer = PauseManager::SE_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			if (Input::TriggerPadLStickDown()) 
			{ 
				nowSoundMixer = PauseManager::BGM_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
		}
		else
		{
			cursor->SetPosition(Vector2(785, 360));
			_cursor->SetPosition(Vector2(720, 360 + 32));
			if (Input::TriggerPadLStickLeft()) 
			{ 
				GameSetting::masterVolume--;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			if (Input::TriggerPadLStickRight()) 
			{ 
				GameSetting::masterVolume++;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
		}
		break;
	case KochaEngine::PauseManager::BGM_VOLUME:
		if (!isSoundMixer)
		{
			cursor->SetPosition(Vector2(CENTER_POS_X, 455));
			if (Input::TriggerPadLStickUp()) 
			{ 
				nowSoundMixer = PauseManager::MASTER_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			if (Input::TriggerPadLStickDown()) 
			{ 
				nowSoundMixer = PauseManager::SE_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
		}
		else
		{
			cursor->SetPosition(Vector2(785, 455));
			_cursor->SetPosition(Vector2(720, 455 + 32));
			if (Input::TriggerPadLStickLeft()) 
			{ 
				GameSetting::bgmVolume--;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			if (Input::TriggerPadLStickRight()) 
			{ 
				GameSetting::bgmVolume++; 
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
		}
		break;
	case KochaEngine::PauseManager::SE_VOLUME:
		if (!isSoundMixer)
		{
			cursor->SetPosition(Vector2(CENTER_POS_X, 550));
			if (Input::TriggerPadLStickUp()) 
			{ 
				nowSoundMixer = PauseManager::BGM_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			if (Input::TriggerPadLStickDown()) 
			{ 
				nowSoundMixer = PauseManager::MASTER_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
		}
		else
		{
			cursor->SetPosition(Vector2(785, 550));
			_cursor->SetPosition(Vector2(720, 550 + 32));
			if (Input::TriggerPadLStickLeft()) 
			{ 
				GameSetting::seVolume--;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			if (Input::TriggerPadLStickRight()) 
			{ 
				GameSetting::seVolume++;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
		}
		break;
	default:
		break;
	}

	const int MAX_VOLUME = 20;
	if (GameSetting::masterVolume < 0) GameSetting::masterVolume = 0;
	if (GameSetting::masterVolume > MAX_VOLUME) GameSetting::masterVolume = MAX_VOLUME;
	if (GameSetting::bgmVolume < 0) GameSetting::bgmVolume = 0;
	if (GameSetting::bgmVolume > MAX_VOLUME) GameSetting::bgmVolume = MAX_VOLUME;
	if (GameSetting::seVolume < 0) GameSetting::seVolume = 0;
	if (GameSetting::seVolume > MAX_VOLUME) GameSetting::seVolume = MAX_VOLUME;
}

void KochaEngine::PauseManager::PauseChange()
{
	isPause = !isPause;
	isSoundMixer = false;
	menuType = PauseManager::PauseUI::MENU_TAB;
	nowMenu = PauseManager::Menu::BACK;
	nowOption = PauseManager::Option::COLOR_PALETTE;
	nowSoundMixer = PauseManager::SoundMixer::MASTER_VOLUME;
}
