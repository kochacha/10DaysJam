#include "Text.h"

KochaEngine::Text::Text(const char* arg_text, const Vector2& arg_position, const Vector2& arg_fontSize)
{
    position = arg_position;
    fontSize = arg_fontSize;

    const char* text = arg_text;
    incrementSize = GetStrLength(text);
    for (int i = 0; i < incrementSize; i++)
    {
        //ここでコンバート
        int texNum = ConvertAscii(*(text + i));
        textures[i] = new Texture2D("Resources/font.png", 10, 7, texNum, Vector2(position.x + (fontSize.x + 2.0f) * i, position.y), fontSize, 0);
    }
}

KochaEngine::Text::~Text()
{
    for (int i = 0; i < incrementSize; i++)
    {
        delete textures[i];
    }
}

void KochaEngine::Text::Draw()
{
    for (int i = 0; i < incrementSize; i++)
    {
        textures[i]->Draw();
    }
}

void KochaEngine::Text::SetLeadText(const int arg_texNum)
{
    textures[0]->SetTexNum(arg_texNum);
}

const int KochaEngine::Text::ConvertAscii(const char arg_charcter)
{
    int n = arg_charcter;
    Vector2 returnValue;

    //アンダーバーかどうか
    if (n == 95)
    {
        return 67;
    }

    //数字かどうか
    if (n < 60)
    {
        return n - 47;
    }
    //大文字かどうか
    else if (n < 92)
    {
        n -= 65;
        return n % 10 + (n / 10) * 10 + 11;
    }
    //小文字
    n -= 97;
    return n % 10 + (n / 10) * 10 + 41;
}

const unsigned int KochaEngine::Text::GetStrLength(const char* arg_strBegin)
{
    unsigned int index = 0;
    while (*(arg_strBegin + index) != '\0')
    {
        index++;
    }
    return index;
}
