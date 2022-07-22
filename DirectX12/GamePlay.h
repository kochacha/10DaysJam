#pragma once
#include "Scene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "Number.h"
#include "Audio.h"
#include "Texture2D.h"
#include "ItemManager.h"
#include "ScrollManager.h"
#include "ScoreDBAccess.h"
#include <memory>

namespace KochaEngine
{
	class Map;
	class LightManager;
	class ScoreManager;
	class PauseManager;
	class Text;
	class InputText;
	class ScoreDBAccess;

	class GamePlay : public Scene
	{
	private:
		enum GameMode
		{
			TITLEMODE,
			NORMALMODE,
			SCOREATTAKMODE,
		};
		GameMode m_currentGameMode;
		Map* m_map;
		Camera* m_camera;
		GameObjectManager* m_gManager;
		ParticleManager* m_pManager;
		ParticleEmitter* m_pEmitter;
		LightManager* m_lightManager;
		ItemManager* m_itemManager;
		ScoreManager* m_scoreManager;
		PauseManager* m_pauseManager;
		ScrollManager* m_scrollManager;
		ScoreDBAccess* m_scoreDBAccessDev;

		std::unique_ptr<Audio> m_uqp_bgm;
		std::unique_ptr<Audio> m_uqp_se;
		std::unique_ptr<Texture2D> m_uqp_flameTexture;
		std::unique_ptr<Texture2D> m_uqp_controlUITexture;
		std::unique_ptr<Texture2D> m_uqp_rankingUITexture;
		std::unique_ptr<Texture2D> m_uqp_moveUITexture;
		std::unique_ptr<Texture2D> m_uqp_smashUITexture;
		std::unique_ptr<Texture2D> m_uqp_finishTexture;
		std::unique_ptr<InputText> m_uqp_iText;

		bool m_isShowRank;
		bool m_isFade;
		bool m_isInGame;
		bool m_isOnce;
		bool m_isDisplayRanking;
		bool m_isSend;
		bool m_isScoreDraw;
		float m_fadeAlpha;
		float m_bgmVolume;
		float m_seVolume;

		int32_t m_frameCount;
		int32_t m_seconds;
		int32_t m_endCount;
		int32_t m_resetCount;
		int32_t m_displayRankingCount;
		int32_t m_deathWaitCount;
		int32_t m_pauseBackCount;

		const float m_scrollAmount = 0.25f;
	public:
		GamePlay();
		~GamePlay() override;
		void Initialize() override;
		void Update() override;
		void SpriteDraw() override;
		void ObjDraw() override;
		void AlphaObjDraw() override;
		void DrawGUI() override;
		void Load() override;
		Scenes Next() override;
		void CountTime();
		void Fade();
		void Scroll();
		void Title();
		void ScoreAttackMode();
		void NormalMode();
		//ランキング登録処理
		void Ranking();
		//タイトル画面でのランキング表示処理
		void ShowRanking();
	};
}
