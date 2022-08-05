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
		//�\���p�̉摜
		Texture2D* mapBar;
		Texture2D* mapPlayer;
		Texture2D* mapAfterSmash;
		Texture2D* mapLeftLimitUI;
		//�\�����W
		Vector2 miniMapPos;
		Vector2 mapPlayerPos;
		Vector2 mapAfterSmashPos;
		//�~�j�}�b�v��\������s�N�Z���T�C�Y
		Vector2 miniMapSize;
		//�~�j�}�b�v�̍��[
		float mapStartX;
		//�~�j�}�b�v�̉E�[
		float mapEndX;
		//�~�j�}�b�v�̕�
		float mapLength;
		//�~�j�}�b�v�̍L���ɂ�鑼�N���X�Ŏg���␳�l
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

