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

namespace KochaEngine
{
	class Map;
	class LightManager;
	class ScoreManager;
	class PauseManager;
	class Text;
	class InputText;

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

		Audio* bgm;
		Texture2D* flameTexture;
		Texture2D* controlUITexture;
		Texture2D* rankingUITexture;
		Texture2D* finishTexture;

		InputText* iText;

		bool isShowRank;
		bool fadeFlag;
		bool inGame;
		bool isOnce;
		float fadeAlpha;
		float bgmVolume;

		int frameCount;
		int seconds;
		int endCount;

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
