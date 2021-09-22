#include "InputText.h"
#include "InputManager.h"
#include "Text.h"

char KochaEngine::InputText::name[10] = {};

KochaEngine::InputText::InputText()
{
	for (int i = 0; i < MAX_INDEX_NUM; i++)
	{
		char displayChar[3] = { '_' };

		text[i] = new Text(displayChar, Vector2(300 + 52 * i, 200), Vector2(48, 48));
	}

	Initialize();
}

KochaEngine::InputText::~InputText()
{
	for (int i = 0; i < MAX_INDEX_NUM; i++)
	{
		delete text[i];
	}
}

void KochaEngine::InputText::Initialize()
{
	texNum = 1;
}

void KochaEngine::InputText::Update()
{
	InputCursor();
	FixNumbers();
	InputDecision();

	wchar_t str[256];
	swprintf_s(str, L"texNum %d\n", texNum);
	OutputDebugString(str);
}

void KochaEngine::InputText::Draw()
{
	for (int i = 0; i < MAX_INDEX_NUM; i++)
	{
		text[i]->Draw();
	}
}

std::string KochaEngine::InputText::GetName()
{
	std::string result = name;
	return result;
}

void KochaEngine::InputText::FixNumbers()
{
	if (texNum > MAX_TEX_NUM)
	{
		texNum = MAX_TEX_NUM;
	}
	else if (texNum < 1)
	{
		texNum = 1;
	}

	if (indexNum > MAX_INDEX_NUM)
	{
		indexNum = MAX_INDEX_NUM;
	}
	else if(indexNum < 0)
	{
		indexNum = 0;
	}
}

void KochaEngine::InputText::InputCursor()
{
	if (InputManager::UpKey())
	{
		if (texNum < 11)
		{
			texNum += 60;
		}
		else
		{
			texNum -= 10;
		}
	}
	else if (InputManager::DownKey())
	{
		if (texNum > 61)
		{
			texNum -= 60;
		}
		else
		{
			texNum += 10;
		}
	}
	else if (InputManager::RightKey())
	{
		if (texNum % 10 == 0)
		{
			texNum -= 9;
		}
		else
		{
			texNum++;
		}
	}
	else if (InputManager::LeftKey())
	{
		if (texNum % 10 == 1)
		{
			texNum += 9;
		}
		else
		{
			texNum--;
		}
	}

}

void KochaEngine::InputText::InputDecision()
{
	bool isCenterSpace = (texNum > 36 && texNum < 41);
	bool isDownSpace = (texNum == 68);
	if (isCenterSpace || isDownSpace) return;

	if (InputManager::DecisionKey())
	{
		if (texNum == 69 && indexNum > 0) //BackSpace
		{
			indexNum--;
			name[indexNum] = '\0';
			text[indexNum]->SetLeadText(67);
		}
		else if(texNum == MAX_TEX_NUM) //OKの時はnameへ格納しない
		{
			//入力受付終了処理

		}
		else if(indexNum < MAX_INDEX_NUM)
		{
			name[indexNum] = ConvertInt(texNum);
			text[indexNum]->SetLeadText(texNum);
			indexNum++;
		}
	}
	else if(InputManager::CancelKey())
	{
		if (indexNum > 0) //BackSpace
		{
			indexNum--;
			name[indexNum] = '\0';
			text[indexNum]->SetLeadText(67);
		}
	}
}

const int KochaEngine::InputText::ConvertInt(const int arg_texNum)
{
	//アンダーバーかどうか
	if (arg_texNum == 67)
	{
		return 95;
	}

	//数字かどうか
	if (arg_texNum < 11)
	{
		return arg_texNum + 47;
	}
	//大文字かどうか
	else if (arg_texNum < 37)
	{
		return arg_texNum + 54;
	}
	//小文字
	return arg_texNum + 46;
}
