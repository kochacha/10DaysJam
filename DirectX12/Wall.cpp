#include "Wall.h"
#include "GameObjectManager.h"
#include "Util.h"

const float DrawTagLength = 230.0f;
const float DifferenceLimitToSpwanDeadLine = 20.0f;
const float CloseDeadLineLength = DrawTagLength + DifferenceLimitToSpwanDeadLine;
const float DrawMarkLength = 9.5f;

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

	deadLineTag = new Object("plane");
	deadLineTag->SetRotate(Vector3(0, 0, 0));
	deadLineTag->SetTexture("Resources/deadTag.png");
	deadLineTag->SetBillboardType(Object::BillboardType::BILLBOARD);

	deadLineMark = new Object("plane");
	deadLineMark->SetRotate(Vector3(0, 0, 0));
	deadLineMark->SetTexture("Resources/deadMark.png");
	deadLineMark->SetBillboardType(Object::BillboardType::BILLBOARD);
	deadLineMark->SetScale(Vector3(12.0f, 12.0f, 12.0f));
	deadLineMark->SetPosition(Vector3(limitRightPosX - DrawMarkLength, 11.0f, -0.5f));

	Initialize();
}

KochaEngine::Wall::~Wall()
{
	delete obj;
	delete deadLineTag;
	delete deadLineMark;
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
	if (IsDrawDeadLineTag())
	{
		PreparationDeadLineTag();
		deadLineTag->Draw(arg_camera, arg_lightManager);
	}
	deadLineMark->Draw(arg_camera, arg_lightManager);
	obj->Draw(arg_camera, arg_lightManager);
}

void KochaEngine::Wall::ScrollWall(const float amount)
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

bool KochaEngine::Wall::IsDrawDeadLineTag()
{
	return maxPos.x >= limitRightPosX - CloseDeadLineLength && maxPos.x <= limitRightPosX - DifferenceLimitToSpwanDeadLine;
}

void KochaEngine::Wall::PreparationDeadLineTag()
{
	float posRate = maxPos.x - limitRightPosX + CloseDeadLineLength;  //0～DrawTagLength
	float posValue = Util::EaseOut(1.5f, 5.0f, posRate / DrawTagLength);
	float scaleValue = Util::EaseOut(3.0f, 10.0f, posRate / DrawTagLength);
	deadLineTag->SetPosition(Vector3(maxPos.x - posValue - 2.0f, 11.0f, -1.0f));
	deadLineTag->SetScale(Vector3(-1.5f, 1.0f, 1.0f) * scaleValue);
}
