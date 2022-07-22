#define _WINSOCKAPI_
#include "GamePlay.h"
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
#include "MiniMap.h"
#include "InputManager.h"
#include "Text.h"
#include "InputText.h"

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
	sManager->Initialize();
	pauseManager = new PauseManager();
	scrollManager = new ScrollManager();
	scoreDBAccessDev = new ScoreDBAccess();

	bgm = new Audio();
	se = new Audio();
	flameTexture = new Texture2D("Resources/waku.png", Vector2(0, 0), Vector2(1280, 960), 0);
	controlUITexture = new Texture2D("Resources/controlUI.png", Vector2(0, 900), Vector2(1280, 32), 0);
	rankingUITexture = new Texture2D("Resources/rankingUI.png", Vector2(850, 900), Vector2(192, 32), 0);
	finishTexture = new Texture2D("Resources/finish.png", Vector2(384, 350), Vector2(512, 128), 0);

	iText = new InputText();
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
	delete se;
	delete flameTexture;
	delete controlUITexture;
	delete rankingUITexture;
	delete scrollManager;
	delete iText;
	delete finishTexture;
	delete scoreDBAccessDev;
}

void KochaEngine::GamePlay::Initialize()
{
	isEnd = false;
	isGameOver = false;
	isOnce = false;
	inGame = false;

	gManager->RemoveAll();	
	
	lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	lightManager->SetDirectionalLightDirection(0, Vector3(0, 0, -1));
	lightManager->SetDirectionalLightIsActive(0, true);
	lightManager->SetLightCamera(camera);

	//map->CreateMap(0);
	const float STAGE_SIZE = 500.0f;
	gManager->AddObject(new Wall(gManager, { -80,-23 }, { 80,45 }, -STAGE_SIZE, STAGE_SIZE)); //rightlimit�̓f�b�h���C���{�P�U�O
	gManager->AddObject(new DeadLine(camera, gManager, emitter, { STAGE_SIZE - 5.0f,0,0.1f, }));
	MiniMap* miniMap = new MiniMap(camera, gManager, emitter);
	gManager->AddObject(miniMap);
	gManager->GetWall()->ScrollWall(miniMap->GetCorrectionValue());
	gManager->AddObject(new Player(camera, gManager, emitter,sManager, Vector3(miniMap->GetCorrectionValue(), 0, 0),&inGame));
	iManager->Initialize(scrollManager);
	iManager->AddEnhItem(Vector3(110, 10, 0), ItemEmitPosition::FROM_CENTER);
	pauseManager->Initialize();
	camera->Initialize(1280, 960, 90, 100, { miniMap->GetCorrectionValue(),0,-120 }, { miniMap->GetCorrectionValue(),0,0 }, { 0,1,0 });
	scrollManager->Initialize();
	iText->Initialize();

	scoreDBAccessDev->Initialize();
	isSend = false;

	gManager->GetPlayer()->SetPauseManager(pauseManager);

	frameCount = 0;
	seconds = 0;
	resetCount = 100;
	displayRankingCount = 300;
	
	isDisplayRanking = false;
	isShowRank = false;
	fadeFlag = true;
	fadeAlpha = 1;
	endCount = 180;
	deathWaitCount = 110;
	pauseBackCount = 0;
	currentGameMode = GameMode::TITLEMODE;

	bgmVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::bgmVolume * 0.1f);
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
	bgm->Init();
	se->Init();
	
}

void KochaEngine::GamePlay::Update()
{
	Fade();
	bgmVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::bgmVolume * 0.1f);
	seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
	bgm->SetVolume(bgmVolume);

	auto player = gManager->GetPlayer();
	if (!player->IsFinish())
	{
		pauseManager->Update();
	}

	if (pauseManager->IsPause())
	{
		pauseBackCount = 3;
		return; //�|�[�Y��
	}
	else if (pauseBackCount > 0)
	{
		pauseBackCount--;
		return;
	}

	//�q�b�g�X�g�b�v����
	player->HitStopTimer();
	if (player->IsHitStop()) return;

	Scroll();
		
	gManager->Update();
	pManager->Update();
	camera->Update();	
	lightManager->Update();

	
	if (inGame && !player->IsFinish())
	{
		iManager->Update();
		scrollManager->Update();
	}

	//�Q�[���I���������̕���
	switch (currentGameMode)
	{
	case KochaEngine::GamePlay::TITLEMODE:
		break;
	case KochaEngine::GamePlay::NORMALMODE:
		NormalMode();
		break;
	case KochaEngine::GamePlay::SCOREATTAKMODE:
		ScoreAttackMode();
		break;
	default:
		break;
	}

	

	//�|�[�Y���j���\���烊�Z�b�g
	if (pauseManager->IsReset())
	{
		player->Finish();
		if (resetCount > 0)
		{
			resetCount--;
		}
		else
		{
			Initialize();
		}
	}
	
	//�^�C�g�����
	if (!inGame)
	{
		Title();
		ShowRanking();
	}

	//�Q�[���I���@�X�R�A�A�^�b�N���[�h�p
	if (gManager->GetWall()->GetMinPos().x >= gManager->GetDeadLine()->GetPosition().x + 5)
	{
		//float x = gManager->GetWall()->GetMaxPos().x;
		if (!isOnce)
		{
			isOnce = true;
			sManager->SaveScore();
			gManager->RemoveItem();
			player->Finish();
			scoreDBAccessDev->Disconnect();
		}
	}
}

void KochaEngine::GamePlay::SpriteDraw()
{
	flameTexture->Draw();
	controlUITexture->Draw();
	rankingUITexture->Draw();
	gManager->SpriteDraw();
	if (scoreDBAccessDev->IsOnline())
	{
		if (deathWaitCount <= 0) //deathWaitCount <= 0 = �Q�[���I�����̃��U���g���
		{
			sManager->DrawResultRanking(isShowRank, scoreDBAccessDev->GetRankingName(), scoreDBAccessDev->GetRankingScore(),iText->GetName());
		}
		else
		{
			sManager->DrawOnlineRinking(isShowRank, scoreDBAccessDev->GetRankingName(), scoreDBAccessDev->GetRankingScore());
		}
	}
	else
	{
		sManager->Draw(isShowRank); //�I�t���C�������L���O��\��
	}
	
	pauseManager->Draw();

	bool isFinishFrame = gManager->GetPlayer()->IsFinish() && !pauseManager->IsReset();
	if (isFinishFrame)
	{
		if (deathWaitCount <= 0)
		{
			iText->Draw();
		}
		else
		{
			finishTexture->Draw();
		}
	}
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

	if (player->IsFinish()) return;

	if (inGame)
	{
		if (player->GetBackCount() > 0 && player->IsHitWall())
		{
			camera->MoveEye({ -10,0,0, });
			wall->ScrollWall(-10);
		}
		camera->MoveEye({ scrollManager->GetScrollAmount(),0,0, });
		wall->ScrollWall(scrollManager->GetScrollAmount());
	}
	else
	{
		//�X�^�[�g��1�ԍ��ɂ����܂ŃX�N���[������
		if (player->GetBackCount() > 0 && player->IsHitWall() && wall->GetMinPos().x > wall->GetLimitLeftPosX())
		{
			camera->MoveEye({ -10,0,0, });
			wall->ScrollWall(-10);
		}
	}
}

void KochaEngine::GamePlay::Title()
{
	auto player = gManager->GetPlayer();
	Vector3 pos = player->GetPosition();
	if (player->IsSmashing())
	{
		if (player->GetPosition().y < 10)
		{
			currentGameMode = GameMode::SCOREATTAKMODE;
		}
		else
		{
			currentGameMode = GameMode::NORMALMODE;
		}
	}
	auto wall = gManager->GetWall();
	if (wall->GetMinPos().x <= wall->GetLimitLeftPosX())
	{
		isShowRank = false;
		inGame = true;
		bgm->LoopPlayWave("Resources/Sound/BGM.wav", bgmVolume);
		sManager->Initialize();
	}
}

void KochaEngine::GamePlay::ScoreAttackMode()
{
	auto player = gManager->GetPlayer();
	//�Q�[���I�����ɖ��O���͉�ʂ�\��
	if (player->IsFinish() && !pauseManager->IsReset())
	{
		if (deathWaitCount > 0)
		{
			deathWaitCount--;
		}
		else
		{
			iText->Update();
		}
	}
	if (iText->IsNext())
	{
		//�Q�[���I���ド���L���O�ǉ�����
		Ranking();
	}
}

void KochaEngine::GamePlay::NormalMode()
{
	auto player = gManager->GetPlayer();
	//�Q�[���I�����ɖ��O���͉�ʂ�\��
	if (player->IsFinish() && !pauseManager->IsReset())
	{
		if (deathWaitCount > 0)
		{
			deathWaitCount--;
		}
		else
		{
			Initialize();
		}
	}
}

void KochaEngine::GamePlay::Ranking()
{
	if (!isSend)
	{
		scoreDBAccessDev->StartConnection(); //�����Őڑ��ł��Ă邩����

		if (scoreDBAccessDev->IsOnline())
		{
			scoreDBAccessDev->GetRankingByAddScoreDB(iText->GetName(), sManager->GetScore()); //�T�[�o�[�Ƀf�[�^�𑗂�
			isSend = true;
		}
	}
	isShowRank = true;
	isDisplayRanking = true;

	if (displayRankingCount > 0)
	{
		displayRankingCount--;
	}
	else
	{
		Initialize();
	}
}

void KochaEngine::GamePlay::ShowRanking()
{
	if (InputManager::RankingCheckKey() && !iText->IsNext())
	{
		se->PlayWave("Resources/Sound/decision.wav", seVolume);
		isShowRank = !isShowRank;
		if (isShowRank)
		{
			scoreDBAccessDev->StartConnection(); //�����Őڑ��ł��Ă邩����
		}
		else
		{
			scoreDBAccessDev->Disconnect(); //�ؒf����
		}

		if (scoreDBAccessDev->IsOnline())
		{
			scoreDBAccessDev->LoadDBRanking(); //�I�����C�������L���O�����[�h
		}
		else
		{
			sManager->LoadRankData(); //�I�t���C�������L���O�����[�h
		}
	}

	auto pPlayer = gManager->GetPlayer();
	if (pPlayer->IsSmashing() || pPlayer->GetBackCount() > 0)
	{
		isShowRank = false;
	}
}
