#pragma once
#include "Sprite.h"

namespace KochaEngine
{
	class PauseManager
	{
		enum PauseUI
		{
			MENU_TAB,
			OPTION_TAB,
			COLOR_TAB,
			SOUND_TAB,
		};

		enum Menu
		{
			BACK,
			RESET,
			OPTION,
			EXIT,
		};

		enum Option
		{
			COLOR_PALETTE,
			SOUND_MIXER,
		};

		enum ColorPalette
		{
			GAMEBOY,
			SEPIA,
			PALETTE1,
			PALETTE2,
			PALETTE3,
			PALETTE4,
			PALETTE5,
			PALETTE6,
			PALETTE7,
			PALETTE8,
			PALETTE9,
			PALETTE10,
		};

		enum SoundMixer
		{
			MASTER_VOLUME,
			BGM_VOLUME,
			SE_VOLUME,
		};

	private:
		bool isPause;

		Sprite* menu[4];
		Sprite* cursor;

		PauseUI menuType;
		Menu nowMenu;
		Option nowOption;
		ColorPalette nowColorPalette;
		SoundMixer nowSoundMixer;

		void MenuTab();
		void OptionTab();
		void ColorTab();
		void SoundTab();

		void PauseChange();

	public:
		PauseManager();
		~PauseManager();

		void Initialize();
		void Update();
		void Draw();

		bool IsPause() { return isPause; }
	};
}