#pragma once
#include<DirectXMath.h>
#include "Vector2.h"
#include "Vector3.h"

namespace KochaEngine
{
	class Camera
	{
		using XMMATRIX = DirectX::XMMATRIX;
		using XMVECTOR = DirectX::XMVECTOR;

	private:
		Vector2 winSize;
		float cameraHAngle;
		float cameraVAngle;
		float cameraDistance;
		float cameraSpeed;
		float crrentAngle;
		float nearZ;
		float farZ;
		int count;
		int nowTime;
		int shakeTime;
		float shakePower;

		bool shakeFlag;
		float H;
		float V;

		Vector3 eye;
		Vector3 target;
		Vector3 up;

		Vector3 preEye;
		Vector3 preTarget;

		XMMATRIX matProjection;
		XMMATRIX matView;

		// ビルボード行列
		XMMATRIX matBillboard;
		// Y軸回りビルボード行列
		XMMATRIX matBillboardY;

		void Shake();

	public:
		Camera();
		~Camera();

		void Initialize(float WIN_WIDTH, float WIN_HEIGHT, float angle, float distance, Vector3 eye, Vector3 target, Vector3 up);
		void Update();
		void ViewUpdate();
		void Billboard();
		void DebugCamera();

		void SetEye(const Vector3& eye);
		void SetTarget(const Vector3& target);
		void SetUp(const Vector3& up);
		void SetAngle(float angle);
		void MoveEye(const Vector3& vel);
		void MoveTarget(const Vector3& vel);

		void UpCamera();
		void RightCamera();

		Vector3 GetEye() { return eye; }
		Vector3 GetTarget() { return target; }
		Vector3 GetUp() { return up; }
		Vector3 GetVelocity();
		XMMATRIX GetMatProjection() { return matProjection; }
		XMMATRIX GetMatView() { return matView; }
		XMMATRIX GetBillboardMatrix() { return matBillboard; }
		XMMATRIX GetBillboardYMatrix() { return matBillboardY; }
		XMMATRIX GetLightCameraMatrix();
		float Getangle();

		float GetCameraSpeed();
		void SetCameraSpeed(float cameraSpeed) { this->cameraSpeed = cameraSpeed; }
		void CameraSpeedUp();
		void CameraSpeedDown();//cameraSpeed-1

		void SetShake(const float arg_power);

	};
}
