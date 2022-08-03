#include "Sprite.h"

KochaEngine::Sprite::Sprite(std::string texName, Vector2 position, Vector2 size, float rotate)
{
	tex = new Texture2D(texName, position, size, rotate);
	this->texName = texName;
	this->position = position;
	this->size = size;
	this->rotate = rotate;
}

KochaEngine::Sprite::~Sprite()
{
	delete tex;
}

void KochaEngine::Sprite::Draw()
{
	tex->Draw(position);
}

void KochaEngine::Sprite::Draw(Vector2 position, const Vector4& color)
{
	tex->SetColor(color);
	tex->Draw(position);
}
