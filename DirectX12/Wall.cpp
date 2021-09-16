#include "Wall.h"
#include "GameObjectManager.h"

KochaEngine::Wall::Wall(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector2& minPos, const Vector2& maxPos)
{
	this->minPos = minPos;
	this->maxPos = maxPos;
	if (arg_gManager == nullptr) return;
	gManager = arg_gManager;
	
	obj = new Object("plane");
	Initialize();
}

KochaEngine::Wall::~Wall()
{
	delete obj;
}

void KochaEngine::Wall::Initialize()
{
	playableSize = maxPos - minPos;

	obj->SetTexture("Resources/wall.png");
	obj->SetScale(Vector3(-20, 120, 1));
	obj->SetPosition(Vector3(minPos.x - 10, 0, 0));
	isAlpha = true;
}

void KochaEngine::Wall::Update()
{
	obj->SetPosition(Vector3(minPos.x, 0, 0));
}

void KochaEngine::Wall::Hit()
{
}

KochaEngine::GameObjectType KochaEngine::Wall::GetType()
{
	return WALL;
}

void KochaEngine::Wall::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	obj->Draw(arg_camera, arg_lightManager);
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

const KochaEngine::Vector2 KochaEngine::Wall::GetPlayableSize()
{
	return playableSize;
}
