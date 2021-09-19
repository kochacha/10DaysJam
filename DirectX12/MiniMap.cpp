#include "MiniMap.h"
#include "GameObjectManager.h"

KochaEngine::MiniMap::MiniMap(Camera* arg_camera,GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	if (arg_pEmitter == nullptr) return;
	camera = arg_camera;
	gManager = arg_gManager;
	pEmitter = arg_pEmitter;

	miniMapLength = { 1000, 60 };
	miniMapPos = { 140, 800 };
	mapBar = new Texture2D("Resources/mapUI.png", miniMapPos, miniMapLength, 0);
	mapPlayer = new Texture2D("Resources/playerUI.png", Vector2(100, 800), Vector2(50, 50), 0);
	
	Initialize();
}

KochaEngine::MiniMap::~MiniMap()
{
	delete mapBar;
	delete mapPlayer;
}

void KochaEngine::MiniMap::Initialize()
{
	mapPlayerPos = { 640,800 };
	
}

void KochaEngine::MiniMap::Update()
{
	auto wall = gManager->GetWall();
	float length = fabs(wall->GetLimitLeftPos()) + fabs(wall->GetLimitRightPos()); //600
	float x = camera->GetTarget().x + fabs(wall->GetLimitLeftPos()); //300
	float ratio = x / length; //0.5f
	mapPlayerPos.x = (miniMapLength.x) * ratio -25; //140
}

void KochaEngine::MiniMap::Hit()
{
}

void KochaEngine::MiniMap::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
}

void KochaEngine::MiniMap::SpriteDraw()
{
	mapBar->Draw();
	mapPlayer->Draw(mapPlayerPos);
}

void KochaEngine::MiniMap::ShowGUI()
{
}

KochaEngine::GameObjectType KochaEngine::MiniMap::GetType()
{
	return GameObjectType();
}
