#include "JammingSpine.h"
#include "GameObjectManager.h"
#include "ItemManager.h"
#include "Wall.h"
#include "Util.h"

KochaEngine::JammingSpine::JammingSpine(Camera* arg_camera, GameObjectManager* arg_gManager, const Vector3& arg_position, ItemManager* arg_iManager, const ItemEmitOption arg_option, const bool arg_isVMove)
{
	if (arg_camera == nullptr) return;
	if (arg_gManager == nullptr) return;
	if (arg_iManager == nullptr) return;

	camera = arg_camera;
	gManager = arg_gManager;	
	iManager = arg_iManager;
	pWall = gManager->GetWall();
	isVerticalMove = arg_isVMove;
	velVertical = 0.3f;

	switch (arg_option)
	{
	case ItemEmitOption::NORMAL:
		position = arg_position;
		break;
	case ItemEmitOption::SMASHING_WALL:
		emittedPlayerPosition = gManager->GetPlayer()->GetPosition();
		position = emittedPlayerPosition;
		prearrangedPosition = arg_position;
		moveCount = 5;
		break;
	default:
		position = arg_position;
		break;
	}

	obj = new Object("plane");
	Initialize();
}

KochaEngine::JammingSpine::~JammingSpine()
{
	if (obj)
	{
		delete obj;
	}
}

void KochaEngine::JammingSpine::Initialize()
{
	isAlpha = true;

	velocity.Zero();

	sphere.radius = 4.0f;

	SetObjParam();

	obj->SetRotate(Vector3(0, 0, 0));
	obj->SetScale(Vector3(10, 10, 10));
	obj->SetTexture("Resources/toge.png");
	obj->SetBillboardType(KochaEngine::Object::BILLBOARD);
}

void KochaEngine::JammingSpine::Update()
{
	if (moveCount > 0)
	{
		position.y = Util::Lerp(emittedPlayerPosition.y, prearrangedPosition.y, (5 - moveCount) / 5.0f);

		moveCount--;
	}

	MoveVertical();

	SetObjParam();

	//左壁にあたる
	if (position.x <= pWall->GetMinPos().x - 10.0f)
	{
		Dead();
		return;
	}
	//右側においていく
	/*if (position.x >= pWall->GetMaxPos().x + pWall->GetPlayableSize().x / 2)
	{
		Dead();
		return;
	}*/

	gManager->HitObject(this, PLAYER);
}

void KochaEngine::JammingSpine::Hit()
{	
	Player* player = gManager->GetPlayer();

	//自身が移動している間
	if (moveCount > 0) return;

	//プレイヤーがスマッシュ中なら
	if (player->IsSmashing())
	{
		player->PowerUp(GetType());

		Dead();		
	}
	//通常時なら
	else if(!player->IsSmashing() && !player->IsStuning())
	{
		player->PowerDown();
	}
}

void KochaEngine::JammingSpine::Dead()
{
	iManager->DeleteFromVector(this, GetType());
	isDelete = true;
}

void KochaEngine::JammingSpine::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	obj->Draw(arg_camera, arg_lightManager);
}

KochaEngine::GameObjectType KochaEngine::JammingSpine::GetType()
{
	return JAMMING_SPINE;
}

void KochaEngine::JammingSpine::SetObjParam()
{
	sphere.position = this->position;
	obj->SetPosition(position);
}

void KochaEngine::JammingSpine::MoveVertical()
{
	if (!isVerticalMove) return;
	if (moveCount > 0) return;

	position.y += velVertical;

	//上端での折り返し
	if (position.y >= pWall->GetMaxPos().y)
	{
		position.y = pWall->GetMaxPos().y;
		velVertical *= -1;
	}
	//下端での折り返し
	else if (position.y <= pWall->GetMinPos().y)
	{
		position.y = pWall->GetMinPos().y;
	    velVertical *= -1;
	}
}
