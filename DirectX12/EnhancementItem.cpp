#include "EnhancementItem.h"
#include "GameObjectManager.h"
#include "ItemManager.h"
#include "Wall.h"
#include "Util.h"

KochaEngine::EnhancementItem::EnhancementItem(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position, ItemManager* arg_iManager, const ItemEmitOption arg_option)
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
		position = arg_position;
		break;
	case ItemEmitOption::SMASHING_WALL:
		emittedPlayerPosition = gManager->GetPlayer()->GetPosition();
		position = emittedPlayerPosition;
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

	sphere.radius = 4.0f;
	
	SetObjParam();

	obj->SetRotate(Vector3(0, 0, 0));
	obj->SetScale(Vector3(10, 10, 10));
	obj->SetTexture("Resources/item.png");
	obj->SetBillboardType(KochaEngine::Object::BILLBOARD);
}

void KochaEngine::EnhancementItem::Update()
{
	if (moveCount > 0)
	{
		position.y = Util::Lerp(emittedPlayerPosition.y, prearrangedPosition.y, (5 - moveCount) / 5.0f);

		moveCount--;
	}

	SetObjParam();

	//ç∂ï«Ç…Ç†ÇΩÇÈ
	if (position.x <= pWall->GetMinPos().x)
	{
		Dead();
		return;
	}
	//âEë§Ç…Ç®Ç¢ÇƒÇ¢Ç≠
	/*if (position.x >= pWall->GetMaxPos().x + pWall->GetPlayableSize().x / 2)
	{
		Dead();
		return;
	}*/

	gManager->HitObject(this, PLAYER);
}

void KochaEngine::EnhancementItem::Hit()
{	
	//é©êgÇ™à⁄ìÆÇµÇƒÇ¢ÇÈä‘
	if (moveCount > 0) return;

	gManager->GetPlayer()->PowerUp(GetType());
	Dead();
}

void KochaEngine::EnhancementItem::Dead()
{
	iManager->DeleteFromVector(this, GetType());
	isDelete = true;
}

void KochaEngine::EnhancementItem::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	obj->Draw(arg_camera, arg_lightManager);
}

KochaEngine::GameObjectType KochaEngine::EnhancementItem::GetType()
{
	return ENHANCEMENT_ITEM;
}

void KochaEngine::EnhancementItem::SetObjParam()
{
	sphere.position = this->position;
	obj->SetPosition(position);
}