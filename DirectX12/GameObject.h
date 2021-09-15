#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "LightManager.h"
#include "Input.h"
#include "Vector3.h"

namespace KochaEngine
{
	enum GameObjectType
	{
		PLAYER,
		BLOCK,
		COLLISION_BLOCK,
		ENHANCEMENT_ITEM,
		JAMMING_SPINE,
	};

	class GameObject
	{
	protected:
		Camera* camera;
		LightManager* lightManager;
		ParticleEmitter* emitter;
		Object* obj;

		bool isDead = false;
		bool isDelete = false;
		bool isTrigger = false;
		bool isAlpha = false;

		Vector3 position = { 0,0,0 };
		Vector3 prePosition = { 0,0,0 };
		Vector3 velocity = { 0,0,0 };

		_Sphere sphere;

		bool hitFlag;
		int hitCount;
		int deleteCount = 0;
		Vector3 prePos = { 0,0,0 };

	public:
		GameObject();
		virtual ~GameObject();
		virtual void Initialize();
		virtual void Update();
		virtual void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager);
		virtual void SpriteDraw();

		virtual void Hit();
		virtual void HitBlock(_Box arg_box);
		virtual void Dead();
		virtual GameObjectType GetType();
		virtual float GetSpeed();
		virtual float GetCameraDistance(Camera* arg_camera);

		bool IsDead() { return isDead; }
		bool IsDelete() { return isDelete; }
		bool IsTrigger() { return isTrigger; }
		bool IsAlphaObject() { return isAlpha; }


		Vector3 GetPosition() { return position; }
		Vector3 GetVelocity() { return velocity; }
		_Sphere GetSphere() { return sphere; }
		void SetPosition(const Vector3& arg_position) { this->position = arg_position; }
	};
}