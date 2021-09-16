#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class EnhancementItem;
	class JammingSpine;
	class Wall;

	enum class ItemEmitOption
	{
		ABSOLUTE_WORLDPOS,
		FROM_CENTER,
		MORE_THAN_RIGHTSIDE,
	};

	class ItemManager
	{
	private:
		Camera* camera;
		GameObjectManager* gManager;

		std::vector<EnhancementItem*> enhancementItems;
		std::vector<JammingSpine*> jammingSpines;
		Vector3 theRightmostPos;
		Wall* pWall;
		const float MARGIN_RIGHTSIDE = 12.0f;

		void AddEnhItem(const Vector3& arg_position, const ItemEmitOption arg_emitOption = ItemEmitOption::ABSOLUTE_WORLDPOS);
		void AddJamSpine(const Vector3& arg_position, const ItemEmitOption arg_emitOption = ItemEmitOption::ABSOLUTE_WORLDPOS);
		void DeleteFromItems(EnhancementItem* arg_enhItem);
		void DeleteFromSpines(JammingSpine* arg_jamSpine);
		void CompareTheRightmost();

	public:
		ItemManager(Camera* arg_camera, GameObjectManager* arg_gManager);
		~ItemManager();

		void Initialize();
		void Update();
		void DeleteFromVector(GameObject* arg_pObj, const GameObjectType arg_objType);
	};
}