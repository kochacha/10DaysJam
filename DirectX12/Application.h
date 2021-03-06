#pragma once
#define _WINSOCKAPI_
#include <DirectXMath.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<vector>
#include<d3dcompiler.h>
#include<string>
#include "PostEffect.h"
#include "Vector3.h"
#include "Vector4.h"

namespace KochaEngine
{
	class Window;
	class Dx12_Wrapper;
	class Dx12_Blob;
	class Dx12_Descriptor;
	class Dx12_RootSignature;
	class Dx12_Pipeline;
	class SceneManager;
	class Texture2D;
	class Object;
	class Camera;
	class LightManager;

	class Application
	{
	private:
		HWND hwnd;
		Application();
		Application(const Application&) = delete;
		void operator=(const Application&) = delete;
		Window* window;
		Dx12_Wrapper* dx12;
		Dx12_Descriptor* descriptor;
		Dx12_Blob* blob;
		Dx12_RootSignature* rootSignature;
		Dx12_Pipeline* pipeline;
		SceneManager* sceneManager;

		Texture2D* texture[2];

		PostEffect* peraBloom;
		PostEffect* peraEffect;
		PostEffect* peraDof;

		ShaderType peraEffectType;

		bool isDof;
		bool isLogoFlag;
		bool isAlphaChange;
		float logoAlpha;
		int alphaCount;

		float vignetteScale;
		float gBoyPixelSize;
		float mosaicSize;
		float sepiaScale;
		float blurScale;
		float cAbeScale;

		float color[4];
		Vector4 shaderColor;

		std::vector<int> gameSetData;

		bool isActiveDirLight;
		Vector3 dirLightDirection;
		Vector3 dirLightColor;
		Vector3 pointLightPosition;
		Vector3 pointLightColor;
		Vector3 pointLightAtten;

		void Load();
		void LoadScene();
		void LoadGameSettings();
		void SaveGameSettings();
		void InitFPS();
		void DrawGUI();
		void EngineLogo();
		bool UpdateFPS();

		const double MIN_FRAME_TIME = 1.000000f / 60.000000f;
		double frameTime = 0.000000f;
		double fps = 0.000000f;
		LARGE_INTEGER timeStart;
		LARGE_INTEGER timeEnd;
		LARGE_INTEGER timeFreq;

	public:
		~Application();
		static Application& Instance();
		SIZE GetWindowSize()const;
		bool Initialize();
		void Run();
		void Terminate();

		static bool isExit;
		static float clearColor[4];
		static PaletteType paletteType;
		static bool isChange;
	};
}