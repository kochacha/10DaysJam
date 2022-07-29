#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class ItemManager;	
	class Wall;

	enum class ItemEmitOption;

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
		//上下移動
		void MoveVertical();
		//上下移動の速度決定
		const float GetVerticalMoveSpeed();

		//見切れ距離用
		static float DEAD_X;
		static float CUT_OFF_X;
		static float SURVIVE_LENGTH;

	public:
		JammingSpine(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position, ItemManager* arg_iManager, const ItemEmitOption arg_option, const bool arg_isVMove);
		~JammingSpine() override;

		void Initialize() override;
		void Update() override;
		void Hit()override;
		void Dead()override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		GameObjectType GetType();
		Vector3 GetPrearrangedPosition()const;
		//見切れているかどうか
		bool IsCutOffScreen();
		//ItemManagerでの描画設定用
		Vector3 GetTagPosition()const;
		Vector3 GetTagScale()const;

		static void SetDeadLength(const int arg_length);
	};
}