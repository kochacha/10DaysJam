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
	m_camera = new Camera();
	m_gManager = new GameObjectManager();

	m_pManager = new ParticleManager();
	m_pEmitter = new ParticleEmitter(m_pManager);
	m_map = new Map(m_gManager, m_camera);
	m_lightManager = new LightManager();
	m_lightManager = LightManager::Create();
	m_itemManager = new ItemManager(m_camera, m_gManager);
	m_scoreManager = new ScoreManager();
	m_scoreManager->Initialize();
	m_pauseManager = new PauseManager();
	m_scrollManager = new ScrollManager();
	m_scoreDBAccessDev = new ScoreDBAccess();

	m_uqp_bgm = std::make_unique<Audio>();
	m_uqp_se = std::make_unique<Audio>();
	m_uqp_flameTexture = std::make_unique<Texture2D>("Resources/waku.png", Vector2(0, 0), Vector2(1280, 960), 0);
	m_uqp_controlUITexture = std::make_unique<Texture2D>("Resources/controlUI.png", Vector2(0, 900), Vector2(1280, 32), 0);
	m_uqp_rankingUITexture = std::make_unique<Texture2D>("Resources/rankingUI.png", Vector2(850, 900), Vector2(192, 32), 0);
	m_uqp_moveUITexture = std::make_unique<Texture2D>("Resources/moveUI.png", Vector2(568, 350), Vector2(144, 80), 0);
	m_uqp_smashUITexture = std::make_unique<Texture2D>("Resources/smashUI.png", Vector2(552, 350), Vector2(176, 80), 0);
	m_uqp_endlessTileTexture = std::make_unique<Texture2D>("Resources/endlessTile.png", Vector2(55, 405), Vector2(70, 270), 0);
	m_uqp_normalTileTexture = std::make_unique<Texture2D>("Resources/normalTile.png", Vector2(55, 132), Vector2(70, 270), 0);
	m_uqp_selectTileTexture = std::make_unique<Texture2D>("Resources/selectTile.png", Vector2(55, 405), Vector2(70, 270), 0);
	m_uqp_endlessPlateTexture = std::make_unique<Texture2D>("Resources/endlessPlate.png", Vector2(1040, 725), Vector2(200, 50), 0);
	m_uqp_normalPlateTexture = std::make_unique<Texture2D>("Resources/normalPlate.png", Vector2(1040, 725), Vector2(200, 50), 0);
	m_uqp_finishTexture = std::make_unique<Texture2D>("Resources/finish.png", Vector2(384, 350), Vector2(512, 128), 0);
	m_uqp_iText = std::make_unique<InputText>();
}

KochaEngine::GamePlay::~GamePlay()
{
	m_gManager->RemoveAll();
	delete m_camera;
	delete m_lightManager;
	delete m_gManager;
	delete m_pManager;
	delete m_pEmitter;
	delete m_map;
	delete m_itemManager;
	delete m_scoreManager;
	delete m_pauseManager;
	delete m_scrollManager;
	delete m_scoreDBAccessDev;
}

void KochaEngine::GamePlay::Initialize()
{
	isEnd = false;
	isGameOver = false;
	m_isOnce = false;
	m_isInGame = false;

	m_gManager->RemoveAll();	
	
	m_lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	m_lightManager->SetDirectionalLightDirection(0, Vector3(0, 0, -1));
	m_lightManager->SetDirectionalLightIsActive(0, true);
	m_lightManager->SetLightCamera(m_camera);

	//map->CreateMap(0);
	const float STAGE_SIZE = 500.0f;
	m_gManager->AddObject(new Wall(m_gManager, { -80,-23 }, { 80,45 }, -STAGE_SIZE, STAGE_SIZE)); //rightlimitはデッドライン＋１６０
	m_gManager->AddObject(new DeadLine(m_camera, m_gManager, m_pEmitter, { STAGE_SIZE - 5.0f,0,0.1f, }));
	MiniMap* miniMap = new MiniMap(m_camera, m_gManager, m_pEmitter);
	m_gManager->AddObject(miniMap);
	m_gManager->GetWall()->ScrollWall(miniMap->GetCorrectionValue());
	m_gManager->AddObject(new Player(m_camera, m_gManager, m_pEmitter,m_scoreManager, Vector3(miniMap->GetCorrectionValue(), 0, 0),&m_isInGame));
	m_itemManager->Initialize(m_scrollManager);
	m_itemManager->AddEnhItem(Vector3(110, 10, 0), ItemEmitPosition::FROM_CENTER);
	m_pauseManager->Initialize();
	m_camera->Initialize(1280, 960, 90, 100, { miniMap->GetCorrectionValue(),0,-120 }, { miniMap->GetCorrectionValue(),0,0 }, { 0,1,0 });
	m_scrollManager->Initialize();
	m_uqp_iText->Initialize();

	m_scoreDBAccessDev->Initialize();
	m_isSend = false;

	m_gManager->GetPlayer()->SetPauseManager(m_pauseManager);

	m_frameCount = 0;
	m_seconds = 0;
	m_resetCount = 100;
	m_displayRankingCount = 300;
	
	m_isDisplayRanking = false;
	m_isShowRank = false;
	m_isFade = true;
	m_fadeAlpha = 1;
	m_endCount = 180;
	m_deathWaitCount = 110;
	m_pauseBackCount = 0;
	m_currentGameMode = GameMode::TITLEMODE;

	m_bgmVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::bgmVolume * 0.1f);
	m_seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
	m_uqp_bgm->Init();
	m_uqp_se->Init();
	
}

void KochaEngine::GamePlay::Update()
{
	Fade();
	m_bgmVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::bgmVolume * 0.1f);
	m_seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
	m_uqp_bgm->SetVolume(m_bgmVolume);

	auto player = m_gManager->GetPlayer();
	if (!player->IsFinish())
	{
		m_pauseManager->Update();
	}

	if (m_pauseManager->IsPause())
	{
		m_pauseBackCount = 3;
		return; //ポーズ中
	}
	else if (m_pauseBackCount > 0)
	{
		m_pauseBackCount--;
		return;
	}

	//ヒットストップ処理
	player->HitStopTimer();
	if (player->IsHitStop()) return;

	Scroll();
		
	m_gManager->Update();
	m_pManager->Update();
	m_camera->Update();	
	m_lightManager->Update();

	
	if (m_isInGame && !player->IsFinish())
	{
		m_itemManager->Update();
		m_scrollManager->Update();
	}

	//ゲーム終了時処理の分岐
	switch (m_currentGameMode)
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

	//ポーズメニュ―からリセット
	if (m_pauseManager->IsReset())
	{
		player->Finish();
		if (m_resetCount > 0)
		{
			m_resetCount--;
		}
		else
		{
			Initialize();
		}
	}
	
	//タイトル画面
	if (!m_isInGame)
	{
		Title();
		ShowRanking();
	}

	switch (m_currentGameMode)
	{
	case KochaEngine::GamePlay::TITLEMODE:
		break;
	case KochaEngine::GamePlay::NORMALMODE:
		NormalModeEnd();
		break;
	case KochaEngine::GamePlay::SCOREATTAKMODE:
		ScoreAttackEnd();
		break;
	default:
		break;
	}

}

void KochaEngine::GamePlay::SpriteDraw()
{
	m_uqp_flameTexture->Draw();
	auto player = m_gManager->GetPlayer();

	if (!player->IsOnceMove())
	{
		m_uqp_moveUITexture->Draw();
	}
	if (!player->IsOnceSmash())
	{
		m_uqp_normalTileTexture->Draw();
		m_uqp_endlessTileTexture->Draw();

		if (player->GetPosition().y < 10)
		{
			m_uqp_selectTileTexture->Draw(Vector2(55, 405));
		}
		else
		{
			m_uqp_selectTileTexture->Draw(Vector2(55, 132));
		}

		if (player->IsSmashPossible())
		{
			m_uqp_smashUITexture->Draw();
		}	
	}

	m_uqp_controlUITexture->Draw();
	m_uqp_rankingUITexture->Draw();
	m_gManager->SpriteDraw();

	m_pauseManager->Draw();

	bool isFinishFrame = m_gManager->GetPlayer()->IsFinish() && !m_pauseManager->IsReset();

	switch (m_currentGameMode)
	{
	case KochaEngine::GamePlay::NORMALMODE:
		m_scoreManager->Draw(m_isShowRank);
		m_scoreManager->DrawQuotaScore(m_quotaScore);

		m_uqp_normalPlateTexture->Draw();

		if (isFinishFrame)
		{
			if (m_deathWaitCount >= 0)
			{
				m_uqp_finishTexture->Draw();
			}
		}
		break;
	case KochaEngine::GamePlay::SCOREATTAKMODE:

		m_uqp_endlessPlateTexture->Draw();

		if (m_scoreDBAccessDev->IsOnline())
		{
			if (m_deathWaitCount <= 0) //deathWaitCount <= 0 = ゲーム終了時のリザルト画面
			{
				m_scoreManager->DrawResultRanking(m_isShowRank, m_scoreDBAccessDev->GetRankingName(), m_scoreDBAccessDev->GetRankingScore(), m_uqp_iText->GetName());
			}
			else
			{
				m_scoreManager->DrawOnlineRinking(m_isShowRank, m_scoreDBAccessDev->GetRankingName(), m_scoreDBAccessDev->GetRankingScore());
				m_scoreManager->DrawQuotaScore(0);
			}
		}
		else
		{
			m_scoreManager->Draw(m_isShowRank); //オフラインランキングを表示
		}
		if (isFinishFrame)
		{
			if (m_deathWaitCount <= 0)
			{
				m_uqp_iText->Draw();	
			}
			else
			{
				m_uqp_finishTexture->Draw();
			}
		}
		break;
	default:
		m_scoreManager->Draw(m_isShowRank);
		m_scoreManager->DrawQuotaScore(m_quotaScore);
		break;
	}
	
}

void KochaEngine::GamePlay::ObjDraw()
{
	m_gManager->ObjDraw(m_camera, m_lightManager);
}

void KochaEngine::GamePlay::AlphaObjDraw()
{
	m_gManager->AlphaObjDraw(m_camera, m_lightManager);
	m_pManager->Draw(m_camera, m_lightManager);
}

void KochaEngine::GamePlay::DrawGUI()
{
	ImGui::Text("GamePlay");
	m_gManager->ShowGUI();
}

void KochaEngine::GamePlay::Load()
{
}

KochaEngine::Scenes KochaEngine::GamePlay::Next()
{
	m_gManager->RemoveAll();
	return ENDING;
}

void KochaEngine::GamePlay::CountTime()
{
	m_frameCount++;
	if (m_frameCount >= 60)
	{
		m_seconds++;
		m_frameCount = 0;
	}
}

void KochaEngine::GamePlay::Fade()
{
	if (m_isFade)
	{
		if (m_fadeAlpha > 0)
		{
			m_fadeAlpha -= 0.02f;
		}
	}
	else
	{
		if (m_fadeAlpha < 1.0f)
		{
			m_fadeAlpha += 0.02f;
		}
	}
}

void KochaEngine::GamePlay::Scroll()
{
	auto wall = m_gManager->GetWall();
	auto player = m_gManager->GetPlayer();

	if (player->IsFinish()) return;

	if (m_isInGame)
	{
		if (player->GetBackCount() > 0 && player->IsHitWall())
		{
			m_camera->MoveEye({ -10,0,0, });
			wall->ScrollWall(-10);
		}
		m_camera->MoveEye({ m_scrollManager->GetScrollAmount(),0,0, });
		wall->ScrollWall(m_scrollManager->GetScrollAmount());
	}
	else
	{
		//スタート時1番左にいくまでスクロールする
		if (player->GetBackCount() > 0 && player->IsHitWall() && wall->GetMinPos().x > wall->GetLimitLeftPosX())
		{
			m_camera->MoveEye({ -10,0,0, });
			wall->ScrollWall(-10);
		}
	}
}

void KochaEngine::GamePlay::Title()
{
	auto player = m_gManager->GetPlayer();
	Vector3 pos = player->GetPosition();
	if (player->IsSmashing())
	{
		m_scoreManager->Initialize();
		if (player->GetPosition().y < 10)
		{
			m_currentGameMode = GameMode::SCOREATTAKMODE;
		}
		else
		{
			m_currentGameMode = GameMode::NORMALMODE;
		}
	}
	auto wall = m_gManager->GetWall();
	if (wall->GetMinPos().x <= wall->GetLimitLeftPosX())
	{
		m_isShowRank = false;
		m_isInGame = true;
		m_uqp_bgm->LoopPlayWave("Resources/Sound/BGM.wav", m_bgmVolume);
		
	}
}

void KochaEngine::GamePlay::ScoreAttackMode()
{
	auto player = m_gManager->GetPlayer();
	//ゲーム終了時に名前入力画面を表示
	if (player->IsFinish() && !m_pauseManager->IsReset())
	{
		if (m_deathWaitCount > 0)
		{
			m_deathWaitCount--;
		}
		else
		{
			m_uqp_iText->Update();
		}
	}
	if (m_uqp_iText->IsNext())
	{
		//ゲーム終了後ランキング追加処理
		Ranking();
	}
}

void KochaEngine::GamePlay::NormalMode()
{
	auto player = m_gManager->GetPlayer();
	//ゲーム終了時に名前入力画面を表示
	if (player->IsFinish() && !m_pauseManager->IsReset())
	{
		if (m_deathWaitCount > 0)
		{
			m_deathWaitCount--;
		}
		else
		{
			Initialize();
		}
	}
}

void KochaEngine::GamePlay::Ranking()
{
	if (!m_isSend)
	{
		m_scoreDBAccessDev->StartConnection(); //ここで接続できてるか判定

		if (m_scoreDBAccessDev->IsOnline())
		{
			m_scoreDBAccessDev->GetRankingByAddScoreDB(m_uqp_iText->GetName(), m_scoreManager->GetScore()); //サーバーにデータを送る
			m_isSend = true;
		}
	}
	m_isShowRank = true;
	m_isDisplayRanking = true;

	if (m_displayRankingCount > 0)
	{
		m_displayRankingCount--;
	}
	else
	{
		Initialize();
	}
}

void KochaEngine::GamePlay::ShowRanking()
{
	if (InputManager::RankingCheckKey() && !m_uqp_iText->IsNext())
	{
		m_uqp_se->PlayWave("Resources/Sound/decision.wav", m_seVolume);
		m_isShowRank = !m_isShowRank;
		if (m_isShowRank)
		{
			m_scoreDBAccessDev->StartConnection(); //ここで接続できてるか判定
		}
		else
		{
			m_scoreDBAccessDev->Disconnect(); //切断処理
		}

		if (m_scoreDBAccessDev->IsOnline())
		{
			m_scoreDBAccessDev->LoadDBRanking(); //オンラインランキングをロード
		}
		else
		{
			m_scoreManager->LoadRankData(); //オフラインランキングをロード
		}
	}

	auto player = m_gManager->GetPlayer();
	if (player->IsSmashing() || player->GetBackCount() > 0)
	{
		m_isShowRank = false;
	}
}

void KochaEngine::GamePlay::ScoreAttackEnd()
{
	auto player = m_gManager->GetPlayer();
	//ゲーム終了
	if (m_gManager->GetWall()->GetMinPos().x >= m_gManager->GetDeadLine()->GetPosition().x + 5)
	{
		//float x = gManager->GetWall()->GetMaxPos().x;
		if (!m_isOnce)
		{
			m_isOnce = true;
			m_scoreManager->SaveScore();
			m_gManager->RemoveItem();
			player->Finish();
			m_scoreDBAccessDev->Disconnect();
		}
	}
}

void KochaEngine::GamePlay::NormalModeEnd()
{
	auto player = m_gManager->GetPlayer();
	if (m_scoreManager->GetScore() > m_quotaScore)
	{
		if (!m_isOnce)
		{
			m_isOnce = true;
			player->Finish();
			m_gManager->RemoveItem();
			m_scoreDBAccessDev->Disconnect();
		}
	}
}
