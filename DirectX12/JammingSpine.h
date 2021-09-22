#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ItemManager;
	enum class ItemEmitOption;
	class Wall;
	class JammingSpine : public GameObject
	{
	private:
		GameObjectManager* gManager;
		ItemManager* iManager;
		Wall* pWall;
		//壁押し込み時の生成に使う
		Vector3 emittedPlayerPosition;
		Vector3 prearrangedPosition;
		int moveCount;
		//上下移動するかどうか
		bool isVerticalMove;
		float velVertical;

		void SetObjParam();
		void MoveVertical();

	public:
		JammingSpine(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position, ItemManager* arg_iManager, const ItemEmitOption arg_option, const bool arg_isVMove);
		~JammingSpine() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void Dead()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType();
	};
}