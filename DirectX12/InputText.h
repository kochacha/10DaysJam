#pragma once
#include <string>

namespace KochaEngine
{
	class Texture2D;
	class Text;
	class InputText
	{
	private:
		Text* text[10];

		static char name[10];
		int indexNum;
		const int MAX_INDEX_NUM = 10;
		int texNum;
		const int MAX_TEX_NUM = 70;

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