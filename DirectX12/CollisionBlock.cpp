#include "CollisionBlock.h"

KochaEngine::CollisionBlock::CollisionBlock(const Vector3& arg_position)
{
	position = arg_position;
	Initialize();
}

KochaEngine::CollisionBlock::~CollisionBlock()
{
}

void KochaEngine::CollisionBlock::Initialize()
{
}

void KochaEngine::CollisionBlock::Update()
{
}

KochaEngine::GameObjectType KochaEngine::CollisionBlock::GetType()
{
	return COLLISION_BLOCK;
}
