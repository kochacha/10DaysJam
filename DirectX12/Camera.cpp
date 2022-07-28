#include "Camera.h"
#include<iostream>
#include"Input.h"
#include"Quaternion.h"
#include "Util.h"

using namespace DirectX;

KochaEngine::Camera::Camera()
{
}

KochaEngine::Camera::~Camera()
{
}

void KochaEngine::Camera::Initialize(float WIN_WIDTH, float WIN_HEIGHT, float angle, float distance, Vector3 eye, Vector3 target, Vector3 up)
{
	winSize = Vector2(WIN_WIDTH, WIN_HEIGHT);
	cameraHAngle = 0;
	cameraVAngle = 0;
	cameraDistance = distance;
	cameraSpeed = 2;
	crrentAngle = 0;

	shakeTime = 0;
	shakePower = 0;
	nowTime = 0;

	this->eye = eye;
	this->target = target;
	this->up = up;

	preEye = eye;
	preTarget = target;

	shakeFlag = false;
	H = 0;
	V = 0;

	nearZ = 0.1f;
	farZ = 1500.0f;

	matProjection = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(60.0f),
		WIN_WIDTH / WIN_HEIGHT,
		nearZ, farZ);

	matView = DirectX::XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));
}

void KochaEngine::Camera::Update()
{	
	Shake();

	Billboard();

	// カメラの設定に反映する
	ViewUpdate();
}

void KochaEngine::Camera::ViewUpdate()
{
	//ビュー変換行列
	matView = DirectX::XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up)
	);
}

void KochaEngine::Camera::Billboard()
{
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	XMVECTOR targetPosition = XMLoadFloat3(&target);
	XMVECTOR upVector = XMLoadFloat3(&up);

	// カメラZ軸（視線方向）
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// カメラのX軸（右方向）
	XMVECTOR cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// カメラのY軸（上方向）
	XMVECTOR cameraAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// 転置により逆行列を計算
	matView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// カメラの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	matView.r[3] = translation;

	// 全方向ビルボード行列の計算
	{
		matBillboard.r[0] = cameraAxisX;
		matBillboard.r[1] = cameraAxisY;
		matBillboard.r[2] = cameraAxisZ;
		matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
	}

	//Y軸回りビルボード行列の計算
	{
		XMVECTOR yBillCameraAxisX, yBillCameraAxisY, yBillCameraAxisZ;

		yBillCameraAxisX = cameraAxisX;
		yBillCameraAxisY = XMVector3Normalize(upVector);
		yBillCameraAxisZ = XMVector3Cross(yBillCameraAxisX, yBillCameraAxisY);

		matBillboardY.r[0] = yBillCameraAxisX;
		matBillboardY.r[1] = yBillCameraAxisY;
		matBillboardY.r[2] = yBillCameraAxisZ;
		matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
	}

}

void KochaEngine::Camera::DebugCamera()
{

}

void KochaEngine::Camera::SetEye(const Vector3& eye)
{
	this->eye = eye;
	ViewUpdate();
}

void KochaEngine::Camera::SetTarget(const Vector3& target)
{
	this->target = target;
	Billboard();
	ViewUpdate();
}

void KochaEngine::Camera::SetUp(const Vector3& up)
{
	this->up = up;
	Billboard();
	ViewUpdate();
}

void KochaEngine::Camera::SetAngle(float angle)
{
	crrentAngle = angle;
}

void KochaEngine::Camera::MoveEye(const Vector3& vel)
{
	this->eye.x += vel.x;
	this->eye.y += vel.y;
	this->eye.z += vel.z;
	this->target.x += vel.x;
	this->target.y += vel.y;
	this->target.z += vel.z;
	Billboard();
	ViewUpdate();
}

void KochaEngine::Camera::MoveTarget(const Vector3& vel)
{
	this->target.x += vel.x;
	this->target.y += vel.y;
	this->target.z += vel.z;
	Billboard();
	ViewUpdate();
}

XMMATRIX KochaEngine::Camera::GetLightCameraMatrix()
{
	return matView * DirectX::XMMatrixOrthographicLH(200, 200, nearZ, farZ);
}

float KochaEngine::Camera::Getangle()
{
	return cameraHAngle;
}

float KochaEngine::Camera::GetCameraSpeed()
{
	float result;
	result = cameraSpeed * 2;
	return result;
}

void KochaEngine::Camera::CameraSpeedUp()
{
	if (cameraSpeed < 4.5f)
	{
		this->cameraSpeed += 0.5f;
	}
}

void KochaEngine::Camera::CameraSpeedDown()
{
	if (cameraSpeed > 0.5f)
	{
		this->cameraSpeed -= 0.5f;
	}
}

void KochaEngine::Camera::Shake()
{
	if (!shakeFlag) return;

	if (shakeTime % 2)
	{
		MoveEye(Vector3(shakePower, 0, 0));
	}
	else
	{
		MoveEye(Vector3(-shakePower, 0, 0));
	}

	if (shakeTime >= 0)
	{
		shakeTime--;
	}
	else
	{
		shakeFlag = false;
		/*SetEye(preEye);
		SetTarget(preTarget);*/
	}
}

void KochaEngine::Camera::SetShake(const int arg_shakeTime, const float arg_power)
{
	preEye = eye;
	preTarget = target;
	shakeTime = arg_shakeTime;
	shakePower = arg_power;
	shakeFlag = true;
}


void KochaEngine::Camera::UpCamera()
{
	
}

void KochaEngine::Camera::RightCamera()
{
	
}

KochaEngine::Vector3 KochaEngine::Camera::GetVelocity()
{
	Vector3 result;
	result.x = target.x - eye.x;
	result.y = target.y - eye.y;
	result.z = target.z - eye.z;
	XMVECTOR vec = XMLoadFloat3(&result);
	vec = XMVector3Normalize(vec);
	XMStoreFloat3(&result, vec);
	if (result.y <= 0) result.y = 0;
	return result;
}


