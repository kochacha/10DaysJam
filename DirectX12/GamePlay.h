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
		Map* map;
		Camera* camera;
		GameObjectManager* gManager;
		ParticleManager* pManager;
		ParticleEmitter* emitter;
		LightManager* lightManager;
		ItemManager* iManager;
		ScoreManager* sManager;
		PauseManager* pauseManager;
		ScrollManager* scrollManager;
		ScoreDBAccess* scoreDBAccessDev;

		Audio* bgm;
		Audio* se;
		Texture2D* flameTexture;
		Texture2D* controlUITexture;
		Texture2D* rankingUITexture;
		Texture2D* finishTexture;

		InputText* iText;

		bool isShowRank;
		bool fadeFlag;
		bool inGame;
		bool isOnce;
		bool isDisplayRanking;
		bool isSend;
		bool scoreDrawFlag;
		float fadeAlpha;
		float bgmVolume;
		float seVolume;

		int frameCount;
		int seconds;
		int endCount;
		int resetCount;
		int displayRankingCount;
		int deathWaitCount;
		int pauseBackCount;

		const float scrollAmount = 0.25f;
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
	};
}
