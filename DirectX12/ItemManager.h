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

		std::vector<EnhancementItem*> enhancementItems;
		std::vector<JammingSpine*> jammingSpines;

		void AddEnhItem(const Vector3& arg_position);
		void AddJamSpine(const Vector3& arg_position);
		void DeleteFromItems(EnhancementItem* arg_enhItem);
		void DeleteFromSpines(JammingSpine* arg_jamSpine);

	public:
		ItemManager(Camera* arg_camera, GameObjectManager* arg_gManager);
		~ItemManager();

		void Initialize();
		void Update();
		void DeleteFromVector(GameObject* arg_pObj, const GameObjectType arg_objType);
	};
}