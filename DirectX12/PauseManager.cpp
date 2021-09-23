#include "PauseManager.h"
#include "Application.h"
#include "GameSetting.h"
#include "Audio.h"
#include "InputManager.h"
#include "Input.h"

KochaEngine::PauseManager::PauseManager()
{
	menu[0] = new Sprite("Resources/Menu1.png", Vector2(440, 200), Vector2(400, 600), 0);
	menu[1] = new Sprite("Resources/Menu2.png", Vector2(440, 200), Vector2(400, 600), 0);
	menu[2] = new Sprite("Resources/Menu3.png", Vector2(40, 200), Vector2(1200, 600), 0);
	menu[3] = new Sprite("Resources/Menu4.png", Vector2(440, 200), Vector2(400, 600), 0);
	menu[4] = new Sprite("Resources/Menu5.png", Vector2(440, 200), Vector2(400, 600), 0);

	cursor = new Sprite("Resources/cursor.png", Vector2(0, 0), Vector2(32, 32), 0);
	_cursor = new Sprite("Resources/cursor.png", Vector2(0, 0), Vector2(32, 32), 180);

	checkBox[0] = new Sprite("Resources/black.png", Vector2(760, 360), Vector2(32, 32), 0);
	checkBox[1] = new Sprite("Resources/black.png", Vector2(760, 460), Vector2(32, 32), 0);
	checkBox[2] = new Sprite("Resources/black.png", Vector2(760, 550), Vector2(32, 32), 0);
	checkBox[3] = new Sprite("Resources/black.png", Vector2(760, 645), Vector2(32, 32), 0);

	soundNum[0] = new Number(Vector2(720, 360), Vector2(32, 32), 2);
	soundNum[1] = new Number(Vector2(720, 455), Vector2(32, 32), 2);
	soundNum[2] = new Number(Vector2(720, 550), Vector2(32, 32), 2);

	se = new Audio();

	Initialize();
}

KochaEngine::PauseManager::~PauseManager()
{
	for (int i = 0; i < 5; i++)
	{
		delete menu[i];
	}
	for (int i = 0; i < 4; i++)
	{
		delete checkBox[i];
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
	isReset = false;

	for (int i = 0; i < 3; i++)
	{
		soundNum[i]->Init();
	}

	menuType = PauseManager::PauseUI::MENU_TAB;
	nowMenu = PauseManager::Menu::BACK;
	nowOption = PauseManager::Option::COLOR_PALETTE;
	nowColorPalette = PauseManager::ColorPalette::GAMEBOY;
	nowSoundMixer = PauseManager::SoundMixer::MASTER_VOLUME;
	nowSupport = PauseManager::Support::DASH_SUPPORT;

	se->Init();
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
}

void KochaEngine::PauseManager::Update()
{
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);

	if (InputManager::PauseKey())
	{
		se->PlayWave("Resources/Sound/decision.wav", seVolume);
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
	case KochaEngine::PauseManager::PauseUI::SUPPORT_TAB:
		SupportTab();
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
		if (!isSoundMixer) break;
		_cursor->Draw();
		break;
	case KochaEngine::PauseManager::PauseUI::SUPPORT_TAB:
		menu[4]->Draw();
		if (GameSetting::isDashData) checkBox[0]->Draw();
		if (GameSetting::isSmashData) checkBox[1]->Draw();
		if (GameSetting::isScoreData) checkBox[2]->Draw();
		if (GameSetting::isVibData) checkBox[3]->Draw();
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
		cursor->SetPosition(Vector2(CENTER_POS_X, 360));
		if (InputManager::UpCursorKey())
		{
			nowMenu = PauseManager::EXIT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{
			nowMenu = PauseManager::RESET;;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (InputManager::DecisionKey())
		{ 
			PauseChange(); 
			se->PlayWave("Resources/Sound/decision.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::RESET:
		cursor->SetPosition(Vector2(CENTER_POS_X, 455));
		if (InputManager::UpCursorKey())
		{
			nowMenu = PauseManager::BACK;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{
			nowMenu = PauseManager::OPTION;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (InputManager::DecisionKey())
		{ 
			se->PlayWave("Resources/Sound/decision.wav", seVolume);
			PauseChange();
			isReset = true;
		}
		break;
	case KochaEngine::PauseManager::OPTION:
		cursor->SetPosition(Vector2(CENTER_POS_X, 550));
		if (InputManager::UpCursorKey())
		{ 
			nowMenu = PauseManager::RESET;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowMenu = PauseManager::EXIT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (InputManager::DecisionKey())
		{ 
			menuType = PauseManager::PauseUI::OPTION_TAB;
			se->PlayWave("Resources/Sound/decision.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::EXIT:
		cursor->SetPosition(Vector2(CENTER_POS_X, 645));
		if (InputManager::UpCursorKey())
		{ 
			nowMenu = PauseManager::OPTION;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowMenu = PauseManager::BACK;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (InputManager::DecisionKey())
		{ 
			Application::isExit = true;
			se->PlayWave("Resources/Sound/decision.wav", seVolume);
		}
		break;
	default:
		break;
	}

	if (InputManager::CancelKey())
	{
		PauseChange();
		se->PlayWave("Resources/Sound/cancel.wav", seVolume);
	}
}

void KochaEngine::PauseManager::OptionTab()
{
	switch (nowOption)
	{
	case KochaEngine::PauseManager::COLOR_PALETTE:
		cursor->SetPosition(Vector2(CENTER_POS_X, 360));
		if (InputManager::UpCursorKey())
		{ 
			nowOption = PauseManager::SUPPORT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{
			nowOption = PauseManager::SOUND_MIXER;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (InputManager::DecisionKey())
		{ 
			menuType = PauseManager::PauseUI::COLOR_TAB;
			se->PlayWave("Resources/Sound/decision.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::SOUND_MIXER:
		cursor->SetPosition(Vector2(CENTER_POS_X, 455));
		if (InputManager::UpCursorKey())
		{ 
			nowOption = PauseManager::COLOR_PALETTE;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{
			nowOption = PauseManager::SUPPORT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (InputManager::DecisionKey())
		{ 
			menuType = PauseManager::PauseUI::SOUND_TAB;
			se->PlayWave("Resources/Sound/decision.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::SUPPORT:
		cursor->SetPosition(Vector2(CENTER_POS_X, 550));
		if (InputManager::UpCursorKey())
		{
			nowOption = PauseManager::SOUND_MIXER;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{
			nowOption = PauseManager::COLOR_PALETTE;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (InputManager::DecisionKey())
		{
			menuType = PauseManager::PauseUI::SUPPORT_TAB;
			se->PlayWave("Resources/Sound/decision.wav", seVolume);
		}
		break;
	default:
		break;
	}
	if (InputManager::CancelKey())
	{
		PauseChange();
		se->PlayWave("Resources/Sound/cancel.wav", seVolume);
	}
}

void KochaEngine::PauseManager::ColorTab()
{
	Application::isChange = true;
	switch (nowColorPalette)
	{
	case KochaEngine::PauseManager::GAMEBOY:
		Application::paletteType = KochaEngine::PaletteType::GAMEBOY;
		cursor->SetPosition(Vector2(LEFT_POS_X, 360));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE10;
			Application::paletteType = KochaEngine::PaletteType::PALETTE10;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::SEPIA;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::SEPIA:
		Application::paletteType = KochaEngine::PaletteType::SEPIA;
		cursor->SetPosition(Vector2(LEFT_POS_X, 455));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::GAMEBOY; 
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE1;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE1:
		Application::paletteType = KochaEngine::PaletteType::PALETTE1;
		cursor->SetPosition(Vector2(LEFT_POS_X, 550));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::SEPIA;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE2;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE2:
		Application::paletteType = KochaEngine::PaletteType::PALETTE2;
		cursor->SetPosition(Vector2(LEFT_POS_X, 645));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE1;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE3;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE3:
		Application::paletteType = KochaEngine::PaletteType::PALETTE3;
		cursor->SetPosition(Vector2(CENTER_POS_X, 360));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE2;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE4;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE4:
		Application::paletteType = KochaEngine::PaletteType::PALETTE4;
		cursor->SetPosition(Vector2(CENTER_POS_X, 455));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE3;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE5;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE5:
		Application::paletteType = KochaEngine::PaletteType::PALETTE5;
		cursor->SetPosition(Vector2(CENTER_POS_X, 550));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE4;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE6;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE6:
		Application::paletteType = KochaEngine::PaletteType::PALETTE6;
		cursor->SetPosition(Vector2(CENTER_POS_X, 645));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE5;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE7;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE7:
		Application::paletteType = KochaEngine::PaletteType::PALETTE7;
		cursor->SetPosition(Vector2(RIGHT_POS_X, 360));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE6;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE8;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE8:
		Application::paletteType = KochaEngine::PaletteType::PALETTE8;
		cursor->SetPosition(Vector2(RIGHT_POS_X, 455));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE7;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE9;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE9:
		Application::paletteType = KochaEngine::PaletteType::PALETTE9;
		cursor->SetPosition(Vector2(RIGHT_POS_X, 550));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE8;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE10;
			Application::paletteType = KochaEngine::PaletteType::PALETTE10;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::PALETTE10:
		Application::isChange = false;
		cursor->SetPosition(Vector2(RIGHT_POS_X, 645));
		if (InputManager::UpCursorKey())
		{ 
			nowColorPalette = PauseManager::PALETTE9;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{ 
			nowColorPalette = PauseManager::GAMEBOY;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		break;
	default:
		break;
	}

	if (InputManager::DecisionKey())
	{ 
		PauseChange();
		se->PlayWave("Resources/Sound/decision.wav", seVolume);
	}
	else if (InputManager::CancelKey())
	{
		PauseChange();
		se->PlayWave("Resources/Sound/cancel.wav", seVolume);
	}
}

void KochaEngine::PauseManager::SoundTab()
{
	if (InputManager::DecisionKey())
	{
		isSoundMixer = !isSoundMixer;
		se->PlayWave("Resources/Sound/decision.wav", seVolume);
	}
	if (InputManager::CancelKey())
	{
		PauseChange();
		se->PlayWave("Resources/Sound/cancel.wav", seVolume);
	}

	switch (nowSoundMixer)
	{
	case KochaEngine::PauseManager::MASTER_VOLUME:
		if (!isSoundMixer)
		{
			cursor->SetPosition(Vector2(CENTER_POS_X, 360));
			if (InputManager::UpCursorKey())
			{ 
				nowSoundMixer = PauseManager::SE_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			else if (InputManager::DownCursorKey())
			{ 
				nowSoundMixer = PauseManager::BGM_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
		}
		else
		{
			cursor->SetPosition(Vector2(785, 360));
			_cursor->SetPosition(Vector2(720, 360 + 32));
			if (InputManager::LeftKey()) 
			{ 
				GameSetting::masterVolume--;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			else if (InputManager::RightKey())
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
			if (InputManager::UpCursorKey())
			{ 
				nowSoundMixer = PauseManager::MASTER_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			else if (InputManager::DownCursorKey())
			{ 
				nowSoundMixer = PauseManager::SE_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
		}
		else
		{
			cursor->SetPosition(Vector2(785, 455));
			_cursor->SetPosition(Vector2(720, 455 + 32));
			if (InputManager::LeftKey())
			{ 
				GameSetting::bgmVolume--;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			else if (InputManager::RightKey())
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
			if (InputManager::UpCursorKey())
			{ 
				nowSoundMixer = PauseManager::BGM_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			else if (InputManager::DownCursorKey())
			{ 
				nowSoundMixer = PauseManager::MASTER_VOLUME;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
		}
		else
		{
			cursor->SetPosition(Vector2(785, 550));
			_cursor->SetPosition(Vector2(720, 550 + 32));
			if (InputManager::LeftKey())
			{ 
				GameSetting::seVolume--;
				se->PlayWave("Resources/Sound/select.wav", seVolume);
			}
			else if (InputManager::RightKey())
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

void KochaEngine::PauseManager::SupportTab()
{
	if (InputManager::CancelKey())
	{
		PauseChange();
		se->PlayWave("Resources/Sound/cancel.wav", seVolume);
	}

	switch (nowSupport)
	{
	case KochaEngine::PauseManager::DASH_SUPPORT:
		cursor->SetPosition(Vector2(CENTER_POS_X, 360));
		if (InputManager::UpCursorKey())
		{
			nowSupport = PauseManager::VIVE_SUPPORT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{
			nowSupport = PauseManager::SMASH_SUPPORT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (InputManager::DecisionKey())
		{
			GameSetting::isDashData = !GameSetting::isDashData;
			se->PlayWave("Resources/Sound/decision.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::SMASH_SUPPORT:
		cursor->SetPosition(Vector2(CENTER_POS_X, 455));
		if (InputManager::UpCursorKey())
		{
			nowSupport = PauseManager::DASH_SUPPORT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{
			nowSupport = PauseManager::SCORE_SUPPORT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (InputManager::DecisionKey())
		{
			GameSetting::isSmashData = !GameSetting::isSmashData;
			se->PlayWave("Resources/Sound/decision.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::SCORE_SUPPORT:
		cursor->SetPosition(Vector2(CENTER_POS_X, 550));
		if (InputManager::UpCursorKey())
		{
			nowSupport = PauseManager::SMASH_SUPPORT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{
			nowSupport = PauseManager::VIVE_SUPPORT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (InputManager::DecisionKey())
		{
			GameSetting::isScoreData = !GameSetting::isScoreData;
			se->PlayWave("Resources/Sound/decision.wav", seVolume);
		}
		break;
	case KochaEngine::PauseManager::VIVE_SUPPORT:
		cursor->SetPosition(Vector2(CENTER_POS_X, 645));
		if (InputManager::UpCursorKey())
		{
			nowSupport = PauseManager::SCORE_SUPPORT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		else if (InputManager::DownCursorKey())
		{
			nowSupport = PauseManager::DASH_SUPPORT;
			se->PlayWave("Resources/Sound/select.wav", seVolume);
		}
		if (InputManager::DecisionKey())
		{
			GameSetting::isVibData = !GameSetting::isVibData;
			if (GameSetting::isVibData) Input::Vibration(60000, 20);
			se->PlayWave("Resources/Sound/decision.wav", seVolume);
		}
		break;
	default:
		break;
	}
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
