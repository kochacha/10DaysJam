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
#include "GameMode.h"

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
		
		KochaEngine::GameMode m_currentGameMode;
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
		std::unique_ptr<Texture2D> m_uqp_heartUITexture;
		std::unique_ptr<Texture2D> m_uqp_dokuroUITexture;
		std::unique_ptr<Texture2D> m_uqp_controlUITexture;
		std::unique_ptr<Texture2D> m_uqp_rankingUITexture;
		std::unique_ptr<Texture2D> m_uqp_moveUITexture;
		std::unique_ptr<Texture2D> m_uqp_smashUITexture;
		std::unique_ptr<Texture2D> m_uqp_endlessTileTexture;
		std::unique_ptr<Texture2D> m_uqp_normalTileTexture;
		std::unique_ptr<Texture2D> m_uqp_selectTileTexture;
		std::unique_ptr<Texture2D> m_uqp_endlessPlateTexture;
		std::unique_ptr<Texture2D> m_uqp_normalPlateTexture;
		std::unique_ptr<Texture2D> m_uqp_finishTexture;
		std::unique_ptr<Texture2D> m_uqp_congratulationTexture;
		std::unique_ptr<Texture2D> m_uqp_gameOverTexture;
		std::unique_ptr<Texture2D> m_uqp_toTitleTexture;
		std::unique_ptr<InputText> m_uqp_iText;
		

		std::unique_ptr<Object> m_uqp_backLine[3];
		std::unique_ptr<Object> m_uqp_moon;
		std::unique_ptr<Object> m_uqp_rocket;
		std::unique_ptr<Object> m_uqp_flag;
		std::unique_ptr<Object> m_uqp_peropero;
		std::unique_ptr<Object> m_uqp_snowman;
		std::unique_ptr<Object> m_uqp_oldman;

		bool m_isShowRank;
		bool m_isFade;
		bool m_isInGame;
		bool m_isOnce;
		bool m_isDisplayRanking;
		bool m_isSend;
		bool m_isScoreDraw;
		bool m_isSpawnBoss; //�{�X���X�|�[�����Ă��邩
		bool m_isScroll;
		bool m_isModeSelect;
		bool m_isGameClear;
		bool m_isItemSpawnStop;//�{�X�o�ꂵ���Ƃ��A�C�e���������p
		bool m_isMoonAppear;
		bool m_isRocketAppear;
		bool m_isFlagAppear;
		bool m_isPeroperoAppear;
		bool m_isSnowmanAppear;
		bool m_isOldmanAppear;
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
		int32_t m_bossSpawnInterval;
		int32_t m_backScreenEffectRate;
		int32_t m_clearEffectRate;
		int16_t m_backObjUpdateCount;
		int8_t m_moonTexNum;
		int8_t m_rocketTexNum;
		int8_t m_flagTexNum;
		int8_t m_peroperoTexNum;
		int8_t m_snowmanTexNum;
		int8_t m_oldmanTexNum;

		int m_quotaScore;
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
		//�����L���O�o�^����
		void Ranking();
		//�^�C�g����ʂł̃����L���O�\������
		void ShowRanking();
		//�X�R�A�A�^�b�N���[�h�I������
		void ScoreAttackEnd();
		//�m�[�}�����[�h�I������
		void NormalModeEnd();
		//�{�X�o�ꉉ�o����
		void SpawnBoss();
		//�{�X��O�̔����ɖ߂�X�N���[��
		void SpawnScroll();
		//�w�i���o
		void BackScreenEffect();
	};
}
