#include "EnhancementItem.h"
#include "GameObjectManager.h"
#include "ItemManager.h"
#include "Wall.h"
#include "Util.h"

KochaEngine::EnhancementItem::EnhancementItem(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position, ItemManager* arg_iManager, const ItemEmitOption arg_option)
	:gManager(nullptr),
	 iManager(nullptr),
	 pWall(nullptr),
	 emittedPlayerPosition(Vector3()),
	 prearrangedPosition(Vector3()),
	 moveCount(0)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	if (arg_iManager == nullptr) return;

	camera = arg_camera;
	gManager = arg_gManager;	
	iManager = arg_iManager;
	pWall = gManager->GetWall();
	moveCount = 0;

	switch (arg_option)
	{
	case ItemEmitOption::NORMAL:
		emittedPlayerPosition = arg_position;
		position = arg_position;
		prearrangedPosition = arg_position;
		break;
	case ItemEmitOption::SMASHING_WALL:
		emittedPlayerPosition = gManager->GetPlayer()->GetPosition();
		position = emittedPlayerPosition;
		//スマッシュ中生成時の目標座標
		prearrangedPosition = arg_position;
		moveCount = 5;
		break;
	default:
		position = arg_position;
		break;
	}	

	obj = new Object("plane");
	Initialize();
}

KochaEngine::EnhancementItem::~EnhancementItem()
{
	if (obj)
	{
		delete obj;
	}
}

void KochaEngine::EnhancementItem::Initialize()
{
	isAlpha = true;

	velocity.Zero();	

	sphere.radius = 5.0f;
	
	SetObjParam();

	obj->SetRotate(Vector3(0, 0, 0));
	obj->SetScale(Vector3(10, 10, 10));
	obj->SetTexture("Resources/item.png");
	obj->SetBillboardType(KochaEngine::Object::BILLBOARD);
}

void KochaEngine::EnhancementItem::Update()
{
	//スマッシュ中に生成されていたら
	if (moveCount >= 0)
	{
		//線形補間で目標座標まで移動
		position.x = Util::Lerp(emittedPlayerPosition.x, prearrangedPosition.x, (5 - moveCount) / 5.0f);
		position.y = Util::Lerp(emittedPlayerPosition.y, prearrangedPosition.y, (5 - moveCount) / 5.0f);

		moveCount--;
	}

	SetObjParam();

	//左壁にあたる
	if (position.x <= pWall->GetMinPos().x - 20.0f)
	{
		Dead();
		return;
	}	

	gManager->HitObject(this, PLAYER);
}

void KochaEngine::EnhancementItem::Hit()
{	
	//自身が移動している間
	if (moveCount > 0) return;

	if (gManager->GetPlayer()->GetBackCount() > 0) return;

	//プレイヤーはパワーアップし、自身は消滅
	gManager->GetPlayer()->PowerUp(GetType());
	Dead();
}

void KochaEngine::EnhancementItem::Dead()
{
	//ItemManagerに登録されている自身を削除要請
	iManager->DeleteFromVector(this, GetType());
	isDelete = true;
}

void KochaEngine::EnhancementItem::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	float cameraPosX = arg_camera->GetEye().x;
	const float CAMERA_DISTANCE_X = 100;
	if (abs(position.x - cameraPosX) > CAMERA_DISTANCE_X) return;

	obj->Draw(arg_camera, arg_lightManager);
}

KochaEngine::GameObjectType KochaEngine::EnhancementItem::GetType()
{
	return ENHANCEMENT_ITEM;
}

KochaEngine::Vector3 KochaEngine::EnhancementItem::GetPrearrangedPosition() const
{
	return prearrangedPosition;
}

void KochaEngine::EnhancementItem::SetObjParam()
{
	sphere.position = this->position;
	obj->SetPosition(position);
}