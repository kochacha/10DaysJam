#include "Wall.h"
#include "GameObjectManager.h"
#include "Util.h"

KochaEngine::Wall::Wall(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector2& minPos, const Vector2& maxPos,const float& limitLeftPos)
{
	this->minPos = minPos;
	this->maxPos = maxPos;
	this->limitLeftPos = limitLeftPos;
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
	currMinPos = minPos;
	currMinPos.x -= 9.5f;

	obj->SetTexture("Resources/wall.png");
	obj->SetScale(Vector3(-70, 120, 1));
	obj->SetPosition(Vector3(minPos.x - 9.5f, 0, 0));
	isAlpha = true;
}

void KochaEngine::Wall::Update()
{
	currMinPos.x = Util::EaseIn(currMinPos.x, minPos.x - 9.5f, 0.55f);
	obj->SetPosition(Vector3(currMinPos.x, 0, 0));

	wchar_t str[256];
	swprintf_s(str, L"minposX %f\n", minPos.x);
	OutputDebugString(str);
	
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

const float KochaEngine::Wall::GetLimitLeftPos()
{
	return limitLeftPos;
}
