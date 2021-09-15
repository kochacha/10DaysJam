#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class EnhancementItem;
	class JammingSpine;

	class ItemManager
	{
	private:
		Camera* camera;
		GameObjectManager* gManager;

	public:
		ItemManager(Camera* arg_camera, GameObjectManager* arg_gManager);
		~ItemManager();

		void Initialize();
		void Update();
	};
}