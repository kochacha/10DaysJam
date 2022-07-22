#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ItemManager;
	class Wall;

	enum class ItemEmitOption;

	class EnhancementItem : public GameObject
	{
	private:
		GameObjectManager* gManager;
		ItemManager* iManager;
		Wall* pWall;
		//�ǉ������ݎ��̐����Ɏg��
		Vector3 emittedPlayerPosition;
		Vector3 prearrangedPosition;
		int moveCount;
		
		void SetObjParam();

	public:
		EnhancementItem(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position, ItemManager* arg_iManager, const ItemEmitOption arg_option);
		~EnhancementItem() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void Dead()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType();
		Vector3 GetPrearrangedPosition()const;
	};
}