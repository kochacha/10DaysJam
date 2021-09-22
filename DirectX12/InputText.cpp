#include "InputText.h"
#include "InputManager.h"
#include "Text.h"
#include "Texture2D.h"

char KochaEngine::InputText::name[10] = {};

KochaEngine::InputText::InputText()
{
	for (int i = 0; i < MAX_INDEX_NUM; i++)
	{
		char displayChar[3] = { '_' };

		text[i] = new Text(displayChar, Vector2(380 + 52 * i, 200), Vector2(FONT_SIZE, FONT_SIZE));
	}
	
	cursorPos = Vector2(400, 300);
	allTextTexture = new Texture2D("Resources/font.png", Vector2(400, 300), Vector2(FONT_SIZE * 10, 336), 0);
	cursorTexture = new Texture2D("Resources/textCursor.png", cursorPos, Vector2(FONT_SIZE, FONT_SIZE), 0);

	Initialize();
}

KochaEngine::InputText::~InputText()
{
	for (int i = 0; i < MAX_INDEX_NUM; i++)
	{
		delete text[i];
	}
	delete allTextTexture;
	delete cursorTexture;
}

void KochaEngine::InputText::Initialize()
{
	cursorPos = Vector2(400, 300);
	texNum = 1;
	count = 0;
	isChange = false;
	isNext = false;
}

void KochaEngine::InputText::Update()
{
	if (isNext) return;
	InputCursor();
	FixNumbers();
	InputDecision();
	if (count < 10)
	{
		count++;
	}
	else
	{
		count = 0;
		isChange = !isChange;
	}
}

void KochaEngine::InputText::Draw()
{
	if (isNext) return;
	for (int i = 0; i < MAX_INDEX_NUM; i++)
	{
		if (i == indexNum && isChange) continue; //現在入力可能文字点滅処理
		text[i]->Draw();
	}
	allTextTexture->Draw();
	cursorTexture->Draw(cursorPos);
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
			cursorPos.y += FONT_SIZE * 6.0f;
		}
		else
		{
			texNum -= 10;
			cursorPos.y -= FONT_SIZE;
		}
	}
	else if (InputManager::DownKey())
	{
		if (texNum > 61)
		{
			texNum -= 60;
			cursorPos.y -= FONT_SIZE * 6.0f;
		}
		else
		{
			texNum += 10;
			cursorPos.y += FONT_SIZE;
		}
	}
	else if (InputManager::RightKey())
	{
		if (texNum % 10 == 0)
		{
			texNum -= 9;
			cursorPos.x -= FONT_SIZE * 9.0f;
		}
		else
		{
			texNum++;
			cursorPos.x += FONT_SIZE;
		}
	}
	else if (InputManager::LeftKey())
	{
		if (texNum % 10 == 1)
		{
			texNum += 9;
			cursorPos.x += FONT_SIZE * 9.0f;
		}
		else
		{
			texNum--;
			cursorPos.x -= FONT_SIZE;
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
			isNext = true;
		}
		else if(indexNum < MAX_INDEX_NUM && texNum != 69)
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
