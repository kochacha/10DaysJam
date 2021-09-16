#include "Wall.h"
#include "GameObjectManager.h"

KochaEngine::Wall::Wall(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector2& minPos, const Vector2& maxPos)
{
	this->minPos = minPos;
	this->maxPos = maxPos;
	Initialize();
}

KochaEngine::Wall::~Wall()
{
}

void KochaEngine::Wall::Initialize()
{
	
}

void KochaEngine::Wall::Update()
{
}

void KochaEngine::Wall::Hit()
{
}

KochaEngine::GameObjectType KochaEngine::Wall::GetType()
{
	return WALL;
}

void KochaEngine::Wall::ScrollWall(float amount)
{
	minPos.x += amount;
	maxPos.x += amount;
}

const KochaEngine::Vector2 KochaEngine::Wall::GetMinPos()
{
	return minPos;
}

const KochaEngine::Vector2 KochaEngine::Wall::GetMaxPos()
{
	return maxPos;
}

const KochaEngine::Vector2 KochaEngine::Wall::GetCenterPos()
{
	return (maxPos + minPos) / 2;
}
