#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Player.h"
#include "Wall.h"
#include <vector>

namespace KochaEngine
{
	class GameObjectManager
	{
	private:
		Camera* camera;
		LightManager* lightManager;
		std::vector<GameObject*> gameObjects;

		void Zsort(const int arg_count);

	public:
		GameObjectManager();
		~GameObjectManager();

		void AddObject(GameObject* arg_gameObject);
		void Initialize();
		void Update();
		void AlphaObjDraw(Camera* arg_camera, LightManager* arg_lightManager);
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager);
		void SpriteDraw();
		void ShowGUI();

		void HitObject(GameObject* arg_gameObject, const GameObjectType& arg_objType);
		int GetTypeCount(const GameObjectType& arg_objType);

		Player* GetPlayer();
		Wall* GetWall();

		void Remove();
		void RemoveAll();
	};
}