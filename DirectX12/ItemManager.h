#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class EnhancementItem;
	class JammingSpine;
	class Wall;
	class ScrollManager;

	enum class ItemEmitPosition
	{
		ABSOLUTE_WORLDPOS,
		FROM_CENTER,
		MORE_THAN_RIGHTSIDE,
	};

	enum class ItemEmitOption
	{
		NORMAL,
		SMASHING_WALL,
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
		ScrollManager* scrollManager;
		
		unsigned int emitInterval;		

		void DeleteFromItems(EnhancementItem* arg_enhItem);
		void DeleteFromSpines(JammingSpine* arg_jamSpine);
		void CompareTheRightmost();
		void EmitItemsNormalTime();
		void EmitItemsSmashing();
		void GeneralEmitCommand(const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption);
		void FixEmitPositionByCondition(Vector3& arg_position, const ItemEmitPosition arg_emitPosition);
		Vector3 DetermineEmitPos(const GameObjectType arg_objType);
		//égÇ¢Ç∑Ç¨íçà”
		const bool IsHitExistingItems(const GameObjectType arg_objType, const Vector3& arg_position);
		const unsigned int GetMaxEmitInterval();
		const unsigned int GetEmitTypeCoefficient();
		const bool GetIsSpineMove();

	public:
		ItemManager(Camera* arg_camera, GameObjectManager* arg_gManager);
		~ItemManager();

		void Initialize(ScrollManager* arg_scrollManager);
		void Update();
		void DeleteFromVector(GameObject* arg_pObj, const GameObjectType arg_objType);
		void AddEnhItem(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition = ItemEmitPosition::ABSOLUTE_WORLDPOS, const ItemEmitOption arg_emitOption = ItemEmitOption::NORMAL);
		void AddJamSpine(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition = ItemEmitPosition::ABSOLUTE_WORLDPOS, const ItemEmitOption arg_emitOption = ItemEmitOption::NORMAL);		

		const unsigned int GetScrollLevel();

		static const float MARGIN_FRAME;
		float arrayEmitHight[11];
	};
}