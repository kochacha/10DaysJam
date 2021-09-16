#include "GamePlay.h"
#include "Input.h"
#include "Util.h"
#include "Map.h"
#include "LightManager.h"
#include "Player.h"
#include "EnhancementItem.h"
#include "JammingSpine.h"
#include "ScoreManager.h"
#include "Wall.h"
#include "PauseManager.h"

KochaEngine::GamePlay::GamePlay()
{
	camera = new Camera();
	gManager = new GameObjectManager();

	pManager = new ParticleManager();
	emitter = new ParticleEmitter(pManager);
	map = new Map(gManager, camera);
	lightManager = new LightManager();

	iManager = new ItemManager(camera, gManager);
	sManager = new ScoreManager();
	pauseManager = new PauseManager();
}

KochaEngine::GamePlay::~GamePlay()
{
	gManager->RemoveAll();
	delete camera;
	delete lightManager;
	delete gManager;
	delete pManager;
	delete emitter;
	delete map;
	delete iManager;
	delete sManager;
	delete pauseManager;
}

void KochaEngine::GamePlay::Initialize()
{
	isEnd = false;
	isGameOver = false;

	gManager->RemoveAll();
	camera->Initialize(1280, 960, 90, 100, { 0,0,-120 }, { 0,0,0 }, { 0,1,0 });
	lightManager = LightManager::Create();
	lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	lightManager->SetDirectionalLightDirection(0, Vector3(0, 0, -1));
	lightManager->SetDirectionalLightIsActive(0, true);
	lightManager->SetLightCamera(camera);

	//map->CreateMap(0);
	gManager->AddObject(new Wall(camera, gManager, { -80,-23 }, { 80,45 }));
	gManager->AddObject(new Player(camera, gManager, emitter, Vector3(0, 0, 0)));
	iManager->Initialize();	
	pauseManager->Initialize();

	frameCount = 0;
	seconds = 0;
	
	isShowRank = false;
	fadeFlag = true;
	fadeAlpha = 1;
	endCount = 180;
}

void KochaEngine::GamePlay::Update()
{
	Fade();

	pauseManager->Update();

	if (pauseManager->IsPause()) return; //�|�[�Y��

	Scroll();
	gManager->Update();
	pManager->Update();
	camera->Update();
	
	lightManager->Update();

	iManager->Update();

	if (Input::TriggerKey(DIK_Q))
	{
		isShowRank = !isShowRank;
	}
}

void KochaEngine::GamePlay::SpriteDraw()
{
	gManager->SpriteDraw();
	sManager->Draw(isShowRank);
}

void KochaEngine::GamePlay::ObjDraw()
{
	gManager->ObjDraw(camera, lightManager);
}

void KochaEngine::GamePlay::AlphaObjDraw()
{
	gManager->AlphaObjDraw(camera, lightManager);
	pManager->Draw(camera, lightManager);
}

void KochaEngine::GamePlay::DrawGUI()
{
	ImGui::Text("GamePlay");
}

void KochaEngine::GamePlay::Load()
{
}

KochaEngine::Scenes KochaEngine::GamePlay::Next()
{
	gManager->RemoveAll();
	return ENDING;
}

void KochaEngine::GamePlay::CountTime()
{
	frameCount++;
	if (frameCount >= 60)
	{
		seconds++;
		frameCount = 0;
	}
}

void KochaEngine::GamePlay::Fade()
{
	if (fadeFlag)
	{
		if (fadeAlpha > 0)
		{
			fadeAlpha -= 0.02f;
		}
	}
	else
	{
		if (fadeAlpha < 1.0f)
		{
			fadeAlpha += 0.02f;
		}
	}
	
}

void KochaEngine::GamePlay::Scroll()
{
	camera->MoveEye({ scrollAmount,0,0, });
	gManager->GetWall()->ScrollWall(scrollAmount);
}
