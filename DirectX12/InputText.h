#pragma once
#include <string>
#include "Vector2.h"

namespace KochaEngine
{
	class Texture2D;
	class Text;
	class InputText
	{
	private:
		Texture2D* allTextTexture;
		Texture2D* cursorTexture;
		Text* text[10];

		Vector2 cursorPos;

		static char name[10];
		int indexNum;
		const int MAX_INDEX_NUM = 10;
		int texNum;
		const int MAX_TEX_NUM = 70;
		const float FONT_SIZE = 48;
		bool isChange;
		int count;

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

		std::string GetName();
	};
}