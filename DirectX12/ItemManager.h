#pragma once
#include "GameObject.h"

namespace KochaEngine
{
	class GameObjectManager;
	class EnhancementItem;
	class JammingSpine;
	class Wall;
	class ScrollManager;

	//アイテムをどこに生成させるか
	enum class ItemEmitPosition
	{
		ABSOLUTE_WORLDPOS,    //ワールド座標そのまま
		FROM_CENTER,          //画面中心を基準として
		MORE_THAN_RIGHTSIDE,  //画面の右端から
	};

	//アイテムをいつ生成しているか
	enum class ItemEmitOption
	{
		NORMAL,         //通常時
		SMASHING_WALL,  //スマッシュ中
	};

	class ItemManager
	{
	private:
		Camera* camera;
		GameObjectManager* gManager;
		Wall* pWall;
		ScrollManager* scrollManager;

		//登場オブジェクト2種の配列
		std::vector<EnhancementItem*> enhancementItems;
		std::vector<JammingSpine*> jammingSpines;				
		//通常時のアイテム生成インターバルカウント
		unsigned int emitInterval;
		//ゲーム画面の枠の大きさ(だいたい)
		const float MARGIN_FRAME;
		//アイテムを生成する仮想レーン
		float arrayEmitHight[11];
		//生成時の座標チェックカウント
		int hitCheckCount;

		//配列から削除
		void DeleteFromItems(EnhancementItem* arg_enhItem);
		void DeleteFromSpines(JammingSpine* arg_jamSpine);
		//通常時の生成
		void EmitItemsNormalTime();
		//スマッシュ中の生成
		void EmitItemsSmashing();
		//アイテム生成フロー
		void GeneralEmitCommand(const ItemEmitPosition arg_emitPosition, const ItemEmitOption arg_emitOption);
		//生成したい座標に変換
		void FixEmitPositionByCondition(Vector3& arg_position, const ItemEmitPosition arg_emitPosition);
		//用意した疑似レーンのどこに生成するか決める
		Vector3 DetermineEmitPos(const GameObjectType arg_objType);
		//既存のアイテムと当たっていないか検証
		//※使いすぎ注意
		const bool IsHitExistingItems(const GameObjectType arg_objType, Vector3& arg_position);
		//レベルデザイン用
		//生成インターバル
		const unsigned int GetMaxEmitInterval();
		//アイテムとトゲのどちらを生成するか
		const unsigned int GetEmitTypeCoefficient();
		//動くトゲにするかどうか
		const bool GetIsSpineMove();

	public:
		ItemManager(Camera* arg_camera, GameObjectManager* arg_gManager);
		~ItemManager();

		void Initialize(ScrollManager* arg_scrollManager);
		void Update();
		//アイテム側からの削除申請
		void DeleteFromVector(GameObject* arg_pObj, const GameObjectType arg_objType);
		//外部からの生成依頼
		void AddEnhItem(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition = ItemEmitPosition::ABSOLUTE_WORLDPOS, const ItemEmitOption arg_emitOption = ItemEmitOption::NORMAL);
		void AddJamSpine(const Vector3& arg_position, const ItemEmitPosition arg_emitPosition = ItemEmitPosition::ABSOLUTE_WORLDPOS, const ItemEmitOption arg_emitOption = ItemEmitOption::NORMAL);		
		//スクロールマネージャーのレベルを他者に伝える
		const unsigned int GetScrollLevel();		
	};
}