#pragma once
#include "Texture2D.h"

namespace KochaEngine
{
	class Texture2D;

	class Text
	{
	private:
		Texture2D* textures[10];
		Vector2 position;
		Vector2 fontSize;
		int incrementSize;

		const int ConvertAscii(const char arg_charcter);
		const unsigned int GetStrLength(const char* arg_strBegin);

	public:
		Text(const char* arg_text, const Vector2& arg_position, const Vector2& arg_fontSize);
		~Text();

		void Draw();
	};
}