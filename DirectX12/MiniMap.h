#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ParticleEmitter;
	class MiniMap :
		public GameObject
	{
	private:
		GameObjectManager* gManager;
		ParticleEmitter* pEmitter;

		Texture2D* mapBar;
		Texture2D* mapPlayer;

		Vector2 miniMapPos;
		Vector2 mapPlayerPos;
		Vector2 miniMapSize;

		float mapStart;
		float mapEnd;
		float mapLength;
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

