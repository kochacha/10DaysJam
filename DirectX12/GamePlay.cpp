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
#include "DeadLine.h"
#include "Audio.h"
#include "GameSetting.h"

KochaEngine::GamePlay::GamePlay()
{
	camera = new Camera();
	gManager = new GameObjectManager();

	pManager = new ParticleManager();
	emitter = new ParticleEmitter(pManager);
	map = new Map(gManager, camera);
	lightManager = new LightManager();
	lightManager = LightManager::Create();
	iManager = new ItemManager(camera, gManager);
	sManager = new ScoreManager();
	pauseManager = new PauseManager();

	bgm = new Audio();
	flameTexture = new Texture2D("Resources/waku.png", Vector2(0, 0), Vector2(1280, 960), 0);
	controlUITexture = new Texture2D("Resources/controlUI.png", Vector2(0, 900), Vector2(1280, 32), 0);
	rankingUITexture = new Texture2D("Resources/rankingUI.png", Vector2(850, 900), Vector2(192, 32), 0);
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
	delete bgm;
	delete flameTexture;
	delete controlUITexture;
	delete rankingUITexture;
}

void KochaEngine::GamePlay::Initialize()
{
	isEnd = false;
	isGameOver = false;

	inGame = false;

	gManager->RemoveAll();
	camera->Initialize(1280, 960, 90, 100, { 0,0,-120 }, { 0,0,0 }, { 0,1,0 });
	
	lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	lightManager->SetDirectionalLightDirection(0, Vector3(0, 0, -1));
	lightManager->SetDirectionalLightIsActive(0, true);
	lightManager->SetLightCamera(camera);

	//map->CreateMap(0);
	gManager->AddObject(new Wall(camera, gManager, { -80,-23 }, { 80,45 }, -500));
	gManager->AddObject(new DeadLine(camera, gManager, emitter, { 80,0,0, }));
	gManager->AddObject(new Player(camera, gManager, emitter, Vector3(0, 0, 0),&inGame));
	iManager->Initialize();	
	iManager->AddEnhItem(Vector3(20, 10, 0), ItemEmitPosition::FROM_CENTER);
	pauseManager->Initialize();

	frameCount = 0;
	seconds = 0;
	
	isShowRank = false;
	fadeFlag = true;
	fadeAlpha = 1;
	endCount = 180;

	bgmVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::bgmVolume * 0.1f);
	bgm->Init();
	
	
}

void KochaEngine::GamePlay::Update()
{
	
	Fade();

	pauseManager->Update();
	bgmVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::bgmVolume * 0.1f);
	bgm->SetVolume(bgmVolume);

	if (pauseManager->IsPause()) return; //ポーズ中

	auto player = gManager->GetPlayer();
	player->HitStopTimer();
	if (player->IsHitStop()) return;

	
	Scroll();
	
	
	gManager->Update();
	pManager->Update();
	camera->Update();
	
	lightManager->Update();

	if (inGame)
	{
		iManager->Update();
	}
	
	if (!inGame)
	{
		Title();
	}

	if (Input::TriggerKey(DIK_Q) || Input::TriggerPadButton(XINPUT_GAMEPAD_X))
	{
		isShowRank = !isShowRank;
	}

	//ゲーム終了
	if (gManager->GetWall()->GetMinPos().x >= gManager->GetDeadLine()->GetPosition().x + 5)
	{
		Initialize();
	}
}

void KochaEngine::GamePlay::SpriteDraw()
{
	flameTexture->Draw();
	controlUITexture->Draw();
	rankingUITexture->Draw();
	gManager->SpriteDraw();
	sManager->Draw(isShowRank);
	pauseManager->Draw();
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
	gManager->ShowGUI();
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
	auto wall = gManager->GetWall();
	auto player = gManager->GetPlayer();

	if (inGame)
	{
		if (player->GetBackCount() > 0 && player->IsHitWall())
		{
			camera->MoveEye({ -10,0,0, });
			wall->ScrollWall(-10);
		}
		camera->MoveEye({ scrollAmount,0,0, });
		wall->ScrollWall(scrollAmount);
	}
	else
	{
		if (player->GetBackCount() > 0 && player->IsHitWall() && wall->GetMinPos().x > wall->GetLimitLeftPos() )
		{
			camera->MoveEye({ -10,0,0, });
			wall->ScrollWall(-10);
		}
	}
	
	
}

void KochaEngine::GamePlay::Title()
{
	auto wall = gManager->GetWall();
	if (wall->GetMinPos().x <= wall->GetLimitLeftPos())
	{
		inGame = true;
		bgm->LoopPlayWave("Resources/Sound/BGM.wav", bgmVolume);
	}
}
