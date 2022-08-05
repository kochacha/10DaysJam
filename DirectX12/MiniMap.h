#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ParticleEmitter;
	class MiniMap : public GameObject
	{
	private:
		GameObjectManager* gManager;
		ParticleEmitter* pEmitter;
		//表示用の画像
		Texture2D* mapBar;
		Texture2D* mapPlayer;
		Texture2D* mapAfterSmash;
		Texture2D* mapLeftLimitUI;
		//表示座標
		Vector2 miniMapPos;
		Vector2 mapPlayerPos;
		Vector2 mapAfterSmashPos;
		//ミニマップを表示するピクセルサイズ
		Vector2 miniMapSize;
		//ミニマップの左端
		float mapStartX;
		//ミニマップの右端
		float mapEndX;
		//ミニマップの幅
		float mapLength;
		//ミニマップの広さによる他クラスで使う補正値
		float mapCorrectionValue;

	public:
		MiniMap(Camera* arg_camera,GameObjectManager* arg_gManager, ParticleEmitter* arg_pEmitter);
		~MiniMap();

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		void SpriteDraw() override;
		void ShowGUI() override;
		GameObjectType GetType();
		const float GetCorrectionValue();
	};
}

