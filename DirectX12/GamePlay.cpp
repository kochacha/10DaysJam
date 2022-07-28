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
#include "JammingBoss.h"
#include "Application.h"

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
	m_scrollManager = ScrollManager::GetInstance();
	m_scoreDBAccessDev = new ScoreDBAccess();

	m_uqp_bgm = std::make_unique<Audio>();
	m_uqp_se = std::make_unique<Audio>();
	m_uqp_flameTexture = std::make_unique<Texture2D>("Resources/waku.png", Vector2(0, 0), Vector2(1280, 960), 0);
	m_uqp_heartUITexture = std::make_unique<Texture2D>("Resources/heartUI.png", Vector2(70, 795), Vector2(64, 64), 0);
	m_uqp_dokuroUITexture = std::make_unique<Texture2D>("Resources/dokuroUI.png", Vector2(1140, 795), Vector2(64, 64), 0);
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
	m_uqp_congratulationTexture = std::make_unique<Texture2D>("Resources/congratulationUI.png", Vector2(128, 300), Vector2(1024, 128), 0);
	m_uqp_gameOverTexture = std::make_unique<Texture2D>("Resources/gameover.png", Vector2(272, 300), Vector2(736, 128), 0);
	m_uqp_toTitleTexture = std::make_unique<Texture2D>("Resources/toTitleUI.png", Vector2(512, 550), Vector2(256, 64), 0);
	m_uqp_iText = std::make_unique<InputText>();

	for (int i = 0; i < 3; i++)
	{
		m_uqp_backLine[i] = std::make_unique<Object>("plane");
	}
	m_uqp_moon = std::make_unique<Object>("plane");
	m_uqp_rocket = std::make_unique<Object>("plane");
	m_uqp_flag = std::make_unique<Object>("plane");
	m_uqp_peropero = std::make_unique<Object>("plane");
	m_uqp_snowman = std::make_unique<Object>("plane");
	m_uqp_oldman = std::make_unique<Object>("plane");
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
	ScrollManager::DeleteInstance();
	delete m_scoreDBAccessDev;
}

void KochaEngine::GamePlay::Initialize()
{
	isEnd = false;
	isGameOver = false;
	m_isOnce = false;
	m_isInGame = false;
	m_isGameClear = false;

	Application::paletteType = KochaEngine::PaletteType::GAMEBOY;
	Application::isChange = true;

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
	m_backScreenEffectRate = 0;
	m_backObjUpdateCount = 0;
	m_moonTexNum = 0;
	m_rocketTexNum = 0;
	m_flagTexNum = 0;
	m_peroperoTexNum = 0;
	m_snowmanTexNum = 0;
	m_oldmanTexNum = 0;
	m_clearEffectRate = 0;
	m_endCount = 180;
	m_deathWaitCount = 300;
	m_pauseBackCount = 0;

	m_bossSpawnInterval = 60;
	
	m_isDisplayRanking = false;
	m_isShowRank = false;
	m_isFade = true;
	m_isModeSelect = false;
	m_isMoonAppear = false;
	m_isRocketAppear = false;
	m_isFlagAppear = false;
	m_isPeroperoAppear = false;
	m_isSnowmanAppear = false;
	m_isOldmanAppear = false;
	m_isSpawnBoss = false;
	m_isScroll = true;
	m_isItemSpawnStop = false;
	m_fadeAlpha = 1;
	
	m_currentGameMode = GameMode::SCOREATTAKMODE;
	
	m_bgmVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::bgmVolume * 0.1f);
	m_seVolume = ((float)GameSetting::masterVolume * 0.1f) * ((float)GameSetting::seVolume * 0.1f);
	m_uqp_bgm->Init();
	m_uqp_se->Init();
	
	for (int i = 0; i < 3; i++)
	{
		m_uqp_backLine[i]->SetPosition(Vector3(-272.0f + 260.0f * i, 10, 1.0f));
		m_uqp_backLine[i]->SetScale(Vector3(2, 110, 1));
		m_uqp_backLine[i]->SetTexture("Resources/backLine.png");
	}

	m_uqp_moon->SetScale(Vector3(-24, 24, 1));
	m_uqp_moon->SetTexture("Resources/moon_0.png");

	m_uqp_rocket->SetScale(Vector3(24, 24, 1));
	m_uqp_rocket->SetTexture("Resources/rocket_0.png");

	m_uqp_flag->SetScale(Vector3(-14, 14, 1));
	m_uqp_flag->SetTexture("Resources/flag_0.png");
	
	m_uqp_peropero->SetScale(Vector3(24, 24, 1));
	m_uqp_peropero->SetRotate(Vector3(0, 0, 0));
	m_uqp_peropero->SetTexture("Resources/peropero_0.png");

	m_uqp_snowman->SetScale(Vector3(-24, 24, 1));
	m_uqp_snowman->SetTexture("Resources/snowman_0.png");

	m_uqp_oldman->SetScale(Vector3(-32, 32, 1));
	m_uqp_oldman->SetRotate(Vector3(0, 0, -10));
	m_uqp_oldman->SetTexture("Resources/oldman_0.png");
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

	if (m_isScroll)
	{
		Scroll();
	}


	m_gManager->Update();
	m_pManager->Update();
	m_camera->Update();	
	m_lightManager->Update();

	
	if (m_isInGame && !player->IsFinish() && !m_isItemSpawnStop)
	{
		m_itemManager->Update();
		m_scrollManager->Update();
	}

	m_pauseManager->SetIsInGame(m_isInGame);

	//ゲーム終了時処理の分岐
	switch (m_currentGameMode)
	{
	case KochaEngine::GameMode::TITLEMODE:
		break;
	case KochaEngine::GameMode::NORMALMODE:
		NormalMode();
		break;
	case KochaEngine::GameMode::SCOREATTAKMODE:
		ScoreAttackMode();
		BackScreenEffect();
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
	case KochaEngine::GameMode::TITLEMODE:
		break;
	case KochaEngine::GameMode::NORMALMODE:
		NormalModeEnd();
		break;
	case KochaEngine::GameMode::SCOREATTAKMODE:
		ScoreAttackEnd();
		break;
	default:
		break;
	}

}

void KochaEngine::GamePlay::SpriteDraw()
{
	m_uqp_flameTexture->Draw();
	//m_uqp_heartUITexture->Draw();
	m_uqp_dokuroUITexture->Draw();

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
			if (m_isModeSelect)
			{
				m_isModeSelect = false;
				m_uqp_se->PlayWave("Resources/Sound/decision.wav", m_seVolume);
			}
		}
		else
		{
			m_uqp_selectTileTexture->Draw(Vector2(55, 132));
			if (!m_isModeSelect)
			{
				m_isModeSelect = true;
				m_uqp_se->PlayWave("Resources/Sound/decision.wav", m_seVolume);
			}
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
	case KochaEngine::GameMode::NORMALMODE:
		m_scoreManager->Draw(m_isShowRank);
		m_scoreManager->DrawQuotaScore(m_quotaScore);

		if (player->IsOnceSmash())
		{
			m_uqp_normalPlateTexture->Draw();
		}

		if (isFinishFrame)
		{
			if (m_isGameClear)
			{
				if (m_deathWaitCount <= 120)
				{
					if (m_deathWaitCount == 119)
					{
						m_uqp_se->PlayWave("Resources/Sound/dash.wav", m_seVolume);
					}
					m_uqp_congratulationTexture->Draw();
				}
			}
			else
			{
				if (m_deathWaitCount <= 240)
				{
					if (m_deathWaitCount == 239)
					{
						m_uqp_se->PlayWave("Resources/Sound/dash.wav", m_seVolume);
					}
					m_uqp_gameOverTexture->Draw();
				}
			}

			if (m_deathWaitCount <= 1)
			{
				if (m_deathWaitCount == 1)
				{
					m_uqp_se->PlayWave("Resources/Sound/dash.wav", m_seVolume);
				}
				m_uqp_toTitleTexture->Draw();
			}
				
		}
		break;
	case KochaEngine::GameMode::SCOREATTAKMODE:

		if (player->IsOnceSmash())
		{
			m_uqp_endlessPlateTexture->Draw();
		}

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
	if (m_isMoonAppear)
	{
		m_uqp_moon->Draw(m_camera, m_lightManager);
	}
	if (m_isRocketAppear)
	{
		m_uqp_rocket->Draw(m_camera, m_lightManager);
	}
	if (m_isFlagAppear)
	{
		m_uqp_flag->Draw(m_camera, m_lightManager);
	}
	if (m_isPeroperoAppear)
	{
		m_uqp_peropero->Draw(m_camera, m_lightManager);
	}
	if (m_isSnowmanAppear)
	{
		m_uqp_snowman->Draw(m_camera, m_lightManager);
	}
	if (m_isOldmanAppear)
	{
		m_uqp_oldman->Draw(m_camera, m_lightManager);
	}

	for (int i = 0; i < 3; i++)
	{
		m_uqp_backLine[i]->Draw(m_camera, m_lightManager);
	}
	m_itemManager->Draw(m_camera, m_lightManager);
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
		int debugBackCount = player->GetBackCount();
		bool debugIshitWall = player->IsHitWall();
		if (player->GetBackCount() > 0 && player->IsHitWall())
		{
			m_camera->MoveEye({ -10.0f,0,0, });
			wall->ScrollWall(-10.0f);
		}
		else
		{
			m_camera->MoveEye({ m_scrollManager->GetScrollAmount(),0,0, });
			wall->ScrollWall(m_scrollManager->GetScrollAmount());
		}
		
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
	if (!m_isScroll && !m_isSpawnBoss && m_gManager->GetPlayer()->GetBackCount() <= 0)
	{
		m_bossSpawnInterval--;
		if (m_bossSpawnInterval <= 0)
		{
			SpawnBoss();
		}
		
	}

	auto boss = m_gManager->GetBoss();

	if (boss != nullptr)
	{
		if (!m_isScroll && m_gManager->GetBoss()->IsSpawnEnd())
		{
			m_isItemSpawnStop = false;
			m_isScroll = true;
			m_uqp_bgm->LoopPlayWave("Resources/Sound/BGM2.wav", m_bgmVolume);
		}
	}
	

	auto player = m_gManager->GetPlayer();
	//ゲーム終了時に名前入力画面を表示
	if (player->IsFinish() && !m_pauseManager->IsReset())
	{
		if (m_deathWaitCount > 0)
		{
			m_deathWaitCount--;
		}
		if (m_deathWaitCount <= 0)
		{
			if (Input::TriggerPadButton(XINPUT_GAMEPAD_A))
			{
				m_uqp_se->PlayWave("Resources/Sound/decision.wav", m_seVolume);
				Initialize();
			}
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
	auto wall = m_gManager->GetWall();
	auto player = m_gManager->GetPlayer();
	auto boss = m_gManager->GetBoss();

	SpawnScroll();
	
	//ゲームオーバー
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
	//クリア後演出
	if (m_deathWaitCount <= 120 && m_isGameClear)
	{
		m_clearEffectRate++;
		if (m_clearEffectRate > 15)
		{
			auto wallPosition = m_gManager->GetWall()->GetMinPos();
			auto starPosition = Vector3(wallPosition.x + 80.0f, wallPosition.y, 0);
			m_pEmitter->BackStarParticle(starPosition, false);
			m_pEmitter->BackStarParticle(starPosition, true);
			m_clearEffectRate = 0;
		}
	}

	if (boss == nullptr) { return; }

	//ゲームクリア
	if (boss->IsFinish())
	{
		if (!m_isOnce)
		{
			m_isGameClear = true;
			m_isOnce = true;
			player->Finish();
			m_gManager->RemoveItem();
			m_scoreDBAccessDev->Disconnect();
		}
	}

	
}

void KochaEngine::GamePlay::SpawnBoss()
{
	auto wall = m_gManager->GetWall();
	float bossPosY = KochaEngine::Util::GetIntRand(10, 50) - 20;
	m_gManager->AddObject(new JammingBoss(m_camera, m_gManager, m_pEmitter, { wall->GetPosition().x,bossPosY,0 }, m_itemManager,&m_currentGameMode));
	m_isSpawnBoss = true;
}

void KochaEngine::GamePlay::SpawnScroll()
{
	
	auto wall = m_gManager->GetWall();
	auto player = m_gManager->GetPlayer();

	if (player->GetBackCount() <= 0)
	{
		if (!m_isSpawnBoss && (m_scoreManager->GetScore() > m_quotaScore))
		{
			if (!m_isItemSpawnStop)
			{
				m_gManager->RemoveItem();
				m_isItemSpawnStop = true;
			}

			float pAddValue = 5.0f;

			if (wall->GetMinPos().x < 0 && m_isScroll)
			{
				pAddValue = 6.0f;

				m_camera->MoveEye({ 5,0,0, });
				wall->ScrollWall(5);
				player->AddPlayerPosX(pAddValue);
				if (wall->GetMinPos().x >= 0)
				{
					m_isScroll = false;
					m_uqp_bgm->Stop();
				}
			}
			else if (wall->GetMinPos().x > 0 && m_isScroll)
			{
				pAddValue = -2.5f;

				m_camera->MoveEye({ -5,0,0, });
				wall->ScrollWall(-5);
				player->AddPlayerPosX(pAddValue);
				if (wall->GetMinPos().x <= 0)
				{
					m_isScroll = false;
					m_uqp_bgm->Stop();
				}
			}

		}
	}
}

void KochaEngine::GamePlay::BackScreenEffect()
{
	//画面横サイズ160

	auto wallPosition = m_gManager->GetWall()->GetMinPos();
	auto starPosition = Vector3(wallPosition.x + 80.0f, wallPosition.y, 0);
	auto moonPosition = Vector3(wallPosition.x + 137.0f, wallPosition.y + 50.0f, 1.11f);
	auto rocketPosition = Vector3(wallPosition.x + 25.0f, wallPosition.y + 5.0f, 1.12f);
	auto flagPosition = Vector3(wallPosition.x + 133.0f, wallPosition.y + 50.0f, 1.13f);
	auto peroperoPosition = Vector3(wallPosition.x + 135.0f, wallPosition.y + 5.0f, 1.14f);
	auto snowmanPosition = Vector3(wallPosition.x + 90.0f, wallPosition.y + 5.0f, 1.15f);
	auto oldmanPosition = Vector3(wallPosition.x + 30.0f, wallPosition.y + 50.0f, 1.16f);

	m_uqp_moon->SetPosition(moonPosition);
	m_uqp_rocket->SetPosition(rocketPosition);
	m_uqp_flag->SetPosition(flagPosition);
	m_uqp_peropero->SetPosition(peroperoPosition);
	m_uqp_snowman->SetPosition(snowmanPosition);
	m_uqp_oldman->SetPosition(oldmanPosition);

	bool isEmitt = false;
	if (m_backScreenEffectRate < 20)
	{
		m_backScreenEffectRate++;
	}
	else
	{
		m_backScreenEffectRate = 0;
		isEmitt = true;
	}

	if (m_backObjUpdateCount < 30)
	{
		m_backObjUpdateCount++;
	}
	else if (m_isMoonAppear && m_moonTexNum < 3)
	{
		m_backObjUpdateCount = 0;
		m_moonTexNum++;
		auto pathStr = std::to_string(m_moonTexNum) + ".png";
		m_uqp_moon->SetTexture("Resources/moon_" + pathStr);
	}
	else if (m_isRocketAppear && m_rocketTexNum < 3)
	{
		m_backObjUpdateCount = 0;
		m_rocketTexNum++;
		auto pathStr = std::to_string(m_rocketTexNum) + ".png";
		m_uqp_rocket->SetTexture("Resources/rocket_" + pathStr);
	}
	else if (m_isFlagAppear && m_flagTexNum < 3)
	{
		m_backObjUpdateCount = 0;
		m_flagTexNum++;
		auto pathStr = std::to_string(m_flagTexNum) + ".png";
		m_uqp_flag->SetTexture("Resources/flag_" + pathStr);
	}
	else if(m_isPeroperoAppear && m_peroperoTexNum < 3)
	{
		m_backObjUpdateCount = 0;
		m_peroperoTexNum++;
		auto pathStr = std::to_string(m_peroperoTexNum) + ".png";
		m_uqp_peropero->SetTexture("Resources/peropero_" + pathStr);
	}
	else if (m_isSnowmanAppear && m_snowmanTexNum < 3)
	{
		m_backObjUpdateCount = 0;
		m_snowmanTexNum++;
		auto pathStr = std::to_string(m_snowmanTexNum) + ".png";
		m_uqp_snowman->SetTexture("Resources/snowman_" + pathStr);
	}
	else if (m_isOldmanAppear && m_oldmanTexNum < 3)
	{
		m_backObjUpdateCount = 0;
		m_oldmanTexNum++;
		auto pathStr = std::to_string(m_oldmanTexNum) + ".png";
		m_uqp_oldman->SetTexture("Resources/oldman_" + pathStr);
	}

	switch (m_scrollManager->GetScrollLevel())
	{
	case 1:
		//何もなし
		//m_isMoonAppear = true;
		//m_isRocketAppear = true;
		//m_isFlagAppear = true;
		//m_isPeroperoAppear = true;
		//m_isSnowmanAppear = true;
		//m_isOldmanAppear = true;
		//if (isEmitt)
		//{
		//	m_pEmitter->ShootingPeroParticle(starPosition);
		//}
		break;
	case 2:
		//星パチパチ
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition,false);
		}
		break;
	case 3:
		//星パチパチ強化
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition,false);
			m_pEmitter->BackStarParticle(starPosition,true);
		}

		break;
	case 4:
		Application::paletteType = KochaEngine::PaletteType::SEPIA;
		Application::isChange = true;
		if (m_scrollManager->IsBGMChange())
		{
			m_scrollManager->SetIsBGMChange(false);
			m_uqp_bgm->Stop();
			m_uqp_bgm->LoopPlayWave("Resources/Sound/BGM6.wav", m_bgmVolume);
		}
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, false);
		}

		break;
	case 5:
		//月出現
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, false);
		}
		m_isMoonAppear = true;

		break;
	case 6:
		//流れ星
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, true);
			m_pEmitter->ShootingStarParticle(starPosition, false);
		}
		break;
	case 7:
		//流れ星強化
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, true);
			m_pEmitter->ShootingStarParticle(starPosition, false);
			m_pEmitter->ShootingStarParticle(starPosition, true);
		}
		break;
	case 8:
		Application::paletteType = KochaEngine::PaletteType::PALETTE1;
		Application::isChange = true;
		if (isEmitt)
		{
			m_pEmitter->ShootingStarParticle(starPosition, false);
		}
		if (m_scrollManager->IsBGMChange())
		{
			m_scrollManager->SetIsBGMChange(false);
			m_uqp_bgm->Stop();
			m_uqp_bgm->LoopPlayWave("Resources/Sound/BGM4.wav", m_bgmVolume);
		}

		break;
	case 9:
		//ロケット出現
		if (isEmitt)
		{
			m_pEmitter->ShootingStarParticle(starPosition, false);
			m_pEmitter->ShootingStarParticle(starPosition, true);
		}
		m_isRocketAppear = true;
		break;
	case 10:
		//旗出現
		if (isEmitt)
		{
			m_pEmitter->ShootingStarParticle(starPosition, false);
			m_pEmitter->ShootingStarParticle(starPosition, true);
		}
		m_isFlagAppear = true;
		break;
	case 11:
		Application::paletteType = KochaEngine::PaletteType::PALETTE2;
		Application::isChange = true;
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, false);
			m_pEmitter->BackStarParticle(starPosition, true);
		}
		if (m_scrollManager->IsBGMChange())
		{
			m_scrollManager->SetIsBGMChange(false);
			m_uqp_bgm->Stop();
			m_uqp_bgm->LoopPlayWave("Resources/Sound/BGM5.wav", m_bgmVolume);
		}
		break;
	case 12:
		//ぺろぺろキャンディー出現
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, false);
			m_pEmitter->BackStarParticle(starPosition, true);
		}
		m_isPeroperoAppear = true;
		break;
	case 13:
		//ぺろキャン流星群
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, false);
			m_pEmitter->BackStarParticle(starPosition, true);
			m_pEmitter->ShootingPeroParticle(starPosition);
		}
		break;
	case 14:
		Application::paletteType = KochaEngine::PaletteType::PALETTE6;
		Application::isChange = true;
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, false);
			m_pEmitter->BackStarParticle(starPosition, true);
		}
		if (m_scrollManager->IsBGMChange())
		{
			m_scrollManager->SetIsBGMChange(false);
			m_uqp_bgm->Stop();
			m_uqp_bgm->LoopPlayWave("Resources/Sound/BGM3.wav", m_bgmVolume);
		}
		break;
	case 15:
		//降雪
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, false);
			m_pEmitter->BackStarParticle(starPosition, true);
			m_pEmitter->SnowParticle(starPosition);
			m_pEmitter->SnowParticle(starPosition);
		}
		break;
	case 16:
		//雪だるま追加
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, false);
			m_pEmitter->BackStarParticle(starPosition, true);
			m_pEmitter->SnowParticle(starPosition);
			m_pEmitter->SnowParticle(starPosition);
		}
		m_isSnowmanAppear = true;
		break;
	case 17:
		//サンタクロース追加
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, false);
			m_pEmitter->BackStarParticle(starPosition, true);
			m_pEmitter->SnowParticle(starPosition);
			m_pEmitter->SnowParticle(starPosition);
		}
		m_isOldmanAppear = true;
		break;
	default:
		if (isEmitt)
		{
			m_pEmitter->BackStarParticle(starPosition, false);
			m_pEmitter->BackStarParticle(starPosition, true);
			m_pEmitter->SnowParticle(starPosition);
			m_pEmitter->SnowParticle(starPosition);
		}

		break;
	}
}
