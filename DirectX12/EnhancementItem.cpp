#include "EnhancementItem.h"
#include "GameObjectManager.h"

KochaEngine::EnhancementItem::EnhancementItem(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;

	camera = arg_camera;
	gManager = arg_gManager;
	position = arg_position;

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
	obj->SetColor(Vector4(0.3f, 0.4f, 0.8f, 1.0f));
	obj->SetBillboardType(KochaEngine::Object::BILLBOARD);
}

void KochaEngine::EnhancementItem::Update()
{
	SetObjParam();

	gManager->HitObject(this, PLAYER);
}

void KochaEngine::EnhancementItem::Hit()
{	
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