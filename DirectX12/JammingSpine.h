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
		//•Ç‰Ÿ‚µ‚İ‚Ì¶¬‚Ég‚¤
		Vector3 emittedPlayerPosition;
		Vector3 prearrangedPosition;
		int moveCount;
		//ã‰ºˆÚ“®‚·‚é‚©‚Ç‚¤‚©
		bool isVerticalMove;
		float velVertical;

		void SetObjParam();
		//ã‰ºˆÚ“®
		void MoveVertical();
		//ã‰ºˆÚ“®‚Ì‘¬“xŒˆ’è
		const float GetVerticalMoveSpeed();

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
		//Œ©Ø‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©
		bool IsCutOffScreen();
		//ItemManager‚Å‚Ì•`‰æİ’è—p
		Vector3 GetTagPosition()const;
		Vector3 GetTagScale()const;
	};
}