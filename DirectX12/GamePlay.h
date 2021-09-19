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

namespace KochaEngine
{
	class Map;
	class LightManager;
	class ScoreManager;
	class PauseManager;

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

		Audio* bgm;
		Texture2D* flameTexture;
		Texture2D* controlUITexture;
		Texture2D* rankingUITexture;

		bool isShowRank;
		bool fadeFlag;
		bool inGame;
		float fadeAlpha;
		float bgmVolume;

		int frameCount;
		int seconds;
		int endCount;

		const float scrollAmount = 1.15f;
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
