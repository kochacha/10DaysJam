#pragma once
#include <string>
#include "Vector2.h"

namespace KochaEngine
{
	class Texture2D;
	class Text;
	class Audio;
	class InputText
	{
	private:
		Texture2D* allTextTexture;
		Texture2D* cursorTexture;
		Text* text[10];
		Audio* se;

		Vector2 cursorPos;

		static char name[10];
		int indexNum;
		const int MAX_INDEX_NUM = 10;
		int texNum;
		const int MAX_TEX_NUM = 70;
		const float FONT_SIZE = 48;
		bool isNext;
		bool isChange;
		int count;
		float seVolume;

		void FixNumbers();
		void InputCursor();
		void InputDecision();
		const int ConvertInt(const int arg_texNum);
	public:
		InputText();
		~InputText();

		void Initialize();

		void Update();
		void Draw();

		char* GetName();
		bool IsNext() { return isNext; }
	};
}