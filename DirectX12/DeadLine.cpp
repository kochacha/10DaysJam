#include "DeadLine.h"

KochaEngine::DeadLine::DeadLine(Camera* arg_camera, GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter, const Vector3& arg_position)
	:gManager(nullptr),
	 pEmitter(nullptr)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	if (arg_pEmitter == nullptr) return;
	
	camera = arg_camera;
	gManager = arg_gManager;
	pEmitter = arg_pEmitter;
	position = arg_position;

	obj = new Object("plane");
	Initialize();
}

KochaEngine::DeadLine::~DeadLine()
{
	delete obj;
}

void KochaEngine::DeadLine::Initialize()
{
	obj->SetTexture("Resources/endLine.png");
	obj->SetScale(Vector3(-10, 120, 1));
	obj->SetPosition(Vector3(position.x - 2.0f, position.y, position.z));
	isAlpha = true;
}

void KochaEngine::DeadLine::Update()
{
}

void KochaEngine::DeadLine::Hit()
{
}

void KochaEngine::DeadLine::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	obj->Draw(arg_camera, arg_lightManager);
}

KochaEngine::GameObjectType KochaEngine::DeadLine::GetType()
{
	return GameObjectType::DEADLINE;
}
