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

	miniMapSize = { 1000, 60 };
	miniMapPos = { 140, 800 };
	mapBar = new Texture2D("Resources/mapUI.png", miniMapPos, miniMapSize, 0);
	mapPlayer = new Texture2D("Resources/playerUI.png", Vector2(100, 800), Vector2(50, 50), 0);
	
	auto wall = gManager->GetWall();
	mapStart = wall->GetLimitLeftPosX() + wall->GetPlayableSize().x / 2;
	mapEnd = (wall->GetLimitRightPosX() - 5.0f) + wall->GetPlayableSize().x / 2;
	mapLength = fabs(mapStart) + fabs(mapEnd);
	mapCorrectionValue = mapLength / 2.0f - fabs(mapStart);

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
	//float length = fabs(wall->GetLimitLeftPosX()) + fabs(wall->GetLimitRightPosX()); //600
	//float x = camera->GetTarget().x + fabs(wall->GetLimitLeftPosX()); //300
	//float ratio = x / length; //0.5f
	//mapPlayerPos.x = (miniMapSize.x) * ratio -25; //140

	//ŽŽ‚µ	
	float cameraPos = camera->GetTarget().x;
	float ratio = cameraPos + fabs(mapStart);
	mapPlayerPos.x = ratio / mapLength;
	mapPlayerPos.x *= miniMapSize.x;
	mapPlayerPos.x += miniMapPos.x;
	mapPlayerPos.x -= 25.0f;
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
	ImGui::InputFloat("##MapPlayerPosX", &mapPlayerPos.x);
}

KochaEngine::GameObjectType KochaEngine::MiniMap::GetType()
{
	return GameObjectType::MINI_MAP;
}

const float KochaEngine::MiniMap::GetCorrectionValue()
{
	return mapCorrectionValue;
}
