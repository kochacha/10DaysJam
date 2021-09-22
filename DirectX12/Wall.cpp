#include "Wall.h"
#include "GameObjectManager.h"
#include "Util.h"

KochaEngine::Wall::Wall(GameObjectManager* arg_gManager, const Vector2& arg_minPos, const Vector2& arg_maxPos,const float& arg_limitLeftPosX, const float& arg_limitRightPosX)
	:gManager(nullptr),
	 minPos(Vector2()),
	 maxPos(Vector2()),
	 playableSize(Vector2()),
	 limitLeftPosX(0.0f),
	 limitRightPosX(0.0f),
	 objectDrawPos(Vector2())
{	
	if (arg_gManager == nullptr) return;
	gManager = arg_gManager;

	this->minPos = arg_minPos;
	this->maxPos = arg_maxPos;
	this->limitLeftPosX = arg_limitLeftPosX;
	this->limitRightPosX = arg_limitRightPosX;
	
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
	objectDrawPos = minPos;
	objectDrawPos.x -= 9.5f;

	obj->SetTexture("Resources/wall.png");
	obj->SetScale(Vector3(-70, 120, 1));
	obj->SetPosition(Vector3(minPos.x - 11.0f, 0, 0));
	isAlpha = true;
}

void KochaEngine::Wall::Update()
{
	//カメラ移動から遅れて壁が動き出して見えるように
	objectDrawPos.x = Util::EaseIn(objectDrawPos.x, minPos.x - 11.0f, 0.55f);
	obj->SetPosition(Vector3(objectDrawPos.x, 0, 0));

	//確認用出力
	/*wchar_t str[256];
	swprintf_s(str, L"minposX %f\n", minPos.x);
	OutputDebugString(str);*/
}

void KochaEngine::Wall::Hit()
{
}

KochaEngine::GameObjectType KochaEngine::Wall::GetType()
{
	return GameObjectType::WALL;
}

void KochaEngine::Wall::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	obj->Draw(arg_camera, arg_lightManager);
}

void KochaEngine::Wall::ScrollWall(float amount)
{
	//x座標だけ同じ分ずらす
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

const float KochaEngine::Wall::GetLimitLeftPosX()
{
	return limitLeftPosX;
}

const float KochaEngine::Wall::GetLimitRightPosX()
{
	return limitRightPosX;
}
