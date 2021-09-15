#include "GamePlay.h"
#include "Input.h"
#include "Util.h"
#include "Map.h"
#include "LightManager.h"
#include "Player.h"
#include "EnhancementItem.h"
#include "JammingSpine.h"

KochaEngine::GamePlay::GamePlay()
{
	camera = new Camera();
	gManager = new GameObjectManager();

	pManager = new ParticleManager();
	emitter = new ParticleEmitter(pManager);
	map = new Map(gManager, camera);
	lightManager = new LightManager();
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

	gManager->AddObject(new Player(camera, gManager, Vector3(0, 0, 0)));
	gManager->AddObject(new EnhancementItem(camera, gManager, Vector3(20, 10, 0)));
	gManager->AddObject(new JammingSpine(camera, gManager, Vector3(-30, -20, 0)));

	frameCount = 0;
	seconds = 0;
	
	fadeFlag = true;
	fadeAlpha = 1;
	endCount = 180;
}

void KochaEngine::GamePlay::Update()
{
	Fade();
	gManager->Update();
	pManager->Update();
	camera->Update();
	lightManager->Update();


}

void KochaEngine::GamePlay::SpriteDraw()
{
	gManager->SpriteDraw();
}

void KochaEngine::GamePlay::ObjDraw()
{
	gManager->ObjDraw(camera, lightManager);

	pManager->Draw(camera);
}

void KochaEngine::GamePlay::AlphaObjDraw()
{
	gManager->AlphaObjDraw(camera, lightManager);
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
