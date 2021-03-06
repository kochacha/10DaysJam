#include "Application.h"
#include "Window.h"
#include "Input.h"
#include "Dx12_Wrapper.h"
#include "Dx12_Blob.h"
#include "Dx12_Descriptor.h"
#include "Dx12_Texture.h"
#include "Dx12_RootSignature.h"
#include "Dx12_Pipeline.h"
#include "Dx12_Object.h"
#include "SceneManager.h"
#include "Title.h"
#include "StageSelect.h"
#include "GamePlay.h"
#include "Ending.h"
#include "GameOver.h"
#include "Texture2D.h"
#include "Object.h"
#include "Camera.h"
#include "Util.h"
#include "CustomGui.h"
#include "EffectManager.h"
#include "LightManager.h"
#include "GameSetting.h"
#include "CSVReader.h"

#include <sstream>
#include <mmsystem.h>
#include <omp.h>

#pragma comment(lib,"winmm.lib")

using namespace DirectX;

bool KochaEngine::Application::isExit = false;
bool KochaEngine::Application::isChange = true;
float KochaEngine::Application::clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
KochaEngine::PaletteType KochaEngine::Application::paletteType = KochaEngine::GAMEBOY;

KochaEngine::Application::Application()
{
}

KochaEngine::Application::~Application()
{
	SaveGameSettings();
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void KochaEngine::Application::Run()
{
	InitFPS();
	window->Show();
	MSG msg = {};
	while (1)
	{
		//メッセージがあるか
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); //キー入力メッセージの処理
			DispatchMessage(&msg); //プロシージャにメッセージを送る
		}

		//終了メッセージが来たら＆ESCAPEを押したらループを抜ける
		if (msg.message == WM_QUIT) break;
		if (Input::TriggerKey(DIK_ESCAPE)) break;
		if (isExit) break;

		if (UpdateFPS()) continue;

		////↓毎フレーム処理↓//
		Input::Update();

		if (isLogoFlag)
		{
			sceneManager->Update();
		}
		else
		{
			EngineLogo();
			texture[0]->SetColor(Vector4(1, 1, 1, logoAlpha));
		}

		if (isChange)
		{
			isChange = false;
			peraEffect->SetColorPalette(paletteType);
		}

		////↑毎フレーム処理↑//
		
		////１パス
		//{
		//	peraDof->PreDrawScene(dx12->GetCmdList().Get());

		//	
		//	peraDof->PostDrawScene(dx12->GetCmdList().Get());
		//}

		////２パス
		//{
		//	peraBloom->PreDrawScene(dx12->GetCmdList().Get());

		//	if (isDof)
		//	{
		//		peraDof->Draw(ShaderType::DEPTH_OF_FIELD_SHADER); //被写界深度
		//	}
		//	else
		//	{
		//		peraDof->Draw();
		//	}

		//	peraBloom->PostDrawScene(dx12->GetCmdList().Get());
		//}

		//1パス
		{
			peraEffect->PreDrawScene(dx12->GetCmdList().Get());

			//peraBloom->Draw();

			if (isLogoFlag)
			{
				Object::BeginDraw(dx12->GetCmdList().Get());
				//↓ObjDraw↓//

				sceneManager->ObjDraw();

				//↑ObjDraw↑//
				Object::EndDraw();

				Object::BeginAlphaDraw(dx12->GetCmdList().Get());

				sceneManager->AlphaObjDraw();

				//↑ObjDraw↑//
				Object::EndDraw();

				Texture2D::BeginDrawAlphaSort(dx12->GetCmdList().Get());
				//↓SpriteDraw↓//

				//texture[0]->Draw();
				sceneManager->SpriteDraw();

				//↑SpriteDraw↑//
				Texture2D::EndDraw();
			}

			peraEffect->PostDrawScene(dx12->GetCmdList().Get());
		}

		//2パス
		{
			dx12->BeginDraw(clearColor[0], clearColor[1], clearColor[2]);

			peraEffect->Draw(peraEffectType);

			Texture2D::BeginDraw(dx12->GetCmdList().Get());
			//↓SpriteDraw↓//

			if (!isLogoFlag)
			{
				texture[1]->Draw();
				texture[0]->Draw();
			}

			//↑SpriteDraw↑//
			Texture2D::EndDraw();

#ifdef _DEBUG
			DrawGUI();
#endif

			dx12->EndDraw();
		}

		dx12->GetSwapchain()->Present(1, 0);
	}
}

void KochaEngine::Application::Load()
{
	auto device = dx12->GetDevice().Get();

	//Textureのロード
	Dx12_Texture::LoadTexture(device, "Resources/white.png");
	Dx12_Texture::LoadTexture(device, "Resources/black.png");
	Dx12_Texture::LoadTexture(device, "Resources/player0.png");
	Dx12_Texture::LoadTexture(device, "Resources/green.png");
	Dx12_Texture::LoadTexture(device, "Resources/PIEN.png");
	Dx12_Texture::LoadTexture(device, "Resources/kaku4.png");
	Dx12_Texture::LoadTexture(device, "Resources/number.png");
	Dx12_Texture::LoadTexture(device, "Resources/waku.png");
	Dx12_Texture::LoadTexture(device, "Resources/toge.png");
	Dx12_Texture::LoadTexture(device, "Resources/item.png");
	Dx12_Texture::LoadTexture(device, "Resources/wall.png");
	Dx12_Texture::LoadTexture(device, "Resources/endlessTile.png");
	Dx12_Texture::LoadTexture(device, "Resources/endlessPlate.png");
	Dx12_Texture::LoadTexture(device, "Resources/normalTile.png");
	Dx12_Texture::LoadTexture(device, "Resources/normalPlate.png");
	Dx12_Texture::LoadTexture(device, "Resources/selectTile.png");
	Dx12_Texture::LoadTexture(device, "Resources/Menu1.png");
	Dx12_Texture::LoadTexture(device, "Resources/Menu2.png");
	Dx12_Texture::LoadTexture(device, "Resources/Menu3.png");
	Dx12_Texture::LoadTexture(device, "Resources/Menu4.png");
	Dx12_Texture::LoadTexture(device, "Resources/Menu5.png");
	Dx12_Texture::LoadTexture(device, "Resources/cursor.png");
	Dx12_Texture::LoadTexture(device, "Resources/normalGauge.png");
	Dx12_Texture::LoadTexture(device, "Resources/gauge.png");
	Dx12_Texture::LoadTexture(device, "Resources/controlUI.png");
	Dx12_Texture::LoadTexture(device, "Resources/rankingUI.png");
	Dx12_Texture::LoadTexture(device, "Resources/moveUI.png");
	Dx12_Texture::LoadTexture(device, "Resources/smashUI.png");
	Dx12_Texture::LoadTexture(device, "Resources/engineLogo.png");
	Dx12_Texture::LoadTexture(device, "Resources/mapUI.png");
	Dx12_Texture::LoadTexture(device, "Resources/playerUI.png");
	Dx12_Texture::LoadTexture(device, "Resources/emptyGauge.png");
	Dx12_Texture::LoadTexture(device, "Resources/star.png");
	Dx12_Texture::LoadTexture(device, "Resources/arrow.png");
	Dx12_Texture::LoadTexture(device, "Resources/way.png");
	Dx12_Texture::LoadTexture(device, "Resources/smashLine.png");
	Dx12_Texture::LoadTexture(device, "Resources/wallPush.png");
	Dx12_Texture::LoadTexture(device, "Resources/togeGet.png");
	Dx12_Texture::LoadTexture(device, "Resources/itemGet.png");
	Dx12_Texture::LoadTexture(device, "Resources/circle.png");
	Dx12_Texture::LoadTexture(device, "Resources/endLine.png");
	Dx12_Texture::LoadTexture(device, "Resources/finish.png");
	Dx12_Texture::LoadTexture(device, "Resources/font.png");
	Dx12_Texture::LoadTexture(device, "Resources/textCursor.png");
	Dx12_Texture::LoadTexture(device, "Resources/rankBack.png");
	Dx12_Texture::LoadTexture(device, "Resources/offline.png");
	Dx12_Texture::LoadTexture(device, "Resources/online.png");
	Dx12_Texture::LoadTexture(device, "Resources/next_ranking.png");
	Dx12_Texture::LoadTexture(device, "Resources/next_title.png");
	Dx12_Texture::LoadTexture(device, "Resources/next_waku.png");
	Dx12_Texture::LoadTexture(device, "Resources/boss.png");
	Dx12_Texture::LoadTexture(device, "Resources/bossGet.png");
	Dx12_Texture::LoadTexture(device, "Resources/bonusGet.png");
	
	//.objのロード
	Dx12_Object::LoadObject(device, "box");
	Dx12_Object::LoadObject(device, "plane");

}

void KochaEngine::Application::LoadScene()
{
	sceneManager->Load(TITLE);
	sceneManager->Load(STAGESELECT);
	sceneManager->Load(GAMEPLAY);
	sceneManager->Load(ENDING);
	sceneManager->Load(GAMEOVER);
	sceneManager->ChangeScene(GAMEPLAY);
}

void KochaEngine::Application::LoadGameSettings()
{
	gameSetData.resize(GameSetting::SETTING_VOL);
	CSVReader* reader = CSVReader::GetInstance();
	reader->LoadCSV("Resources/GameSetting.txt", "GameSetting", true);
	std::vector<std::vector<int>> tmp = reader->GetMapData("GameSetting");
	for (int i = 0; i < GameSetting::SETTING_VOL; i++)
	{
		gameSetData[i] = tmp[0][i];
	}

	GameSetting::masterVolume = gameSetData[0];
	GameSetting::bgmVolume = gameSetData[1];
	GameSetting::seVolume = gameSetData[2];
	GameSetting::isDashData = gameSetData[3];
	GameSetting::isSmashData = gameSetData[4];
	GameSetting::isScoreData = gameSetData[5];
	GameSetting::isVibData = gameSetData[6];
	GameSetting::isShowPowerData = gameSetData[7];
}

void KochaEngine::Application::SaveGameSettings()
{
	gameSetData[0] = GameSetting::masterVolume;
	gameSetData[1] = GameSetting::bgmVolume;
	gameSetData[2] = GameSetting::seVolume;
	gameSetData[3] = GameSetting::isDashData;
    gameSetData[4] = GameSetting::isSmashData;
	gameSetData[5] = GameSetting::isScoreData;
	gameSetData[6] = GameSetting::isVibData;
	gameSetData[7] = GameSetting::isShowPowerData;

	std::ofstream ofs("Resources/GameSetting.txt");
	for (int i = 0; i < GameSetting::SETTING_VOL; i++)
	{
		ofs << std::to_string(gameSetData[i]);
		ofs << ',';
	}
	ofs.close();
}

void KochaEngine::Application::InitFPS()
{
	QueryPerformanceFrequency(&timeFreq);
	QueryPerformanceCounter(&timeStart);
}

void KochaEngine::Application::DrawGUI()
{
	ImGui::ShowDemoWindow();

	//シーン情報
	ImGui::Begin("Scene");
	sceneManager->DrawGUI();
	ImGui::End();

	//共通設定
	ImGui::Begin("App");
	//ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Text("FPS:%f", fps);
	if (ImGui::CollapsingHeader("ClearColor"))
	{
		ImGui::ColorPicker4(" Color", clearColor);
	}
	ImGui::End();

	//DirectionalLight
	ImGui::Begin("Light");
	if (ImGui::CollapsingHeader("DirectionalLight"))
	{
		ImGui::Checkbox("isActive", &isActiveDirLight);
		ImGui::Text("Direction:");
		ImGui::SameLine();
		ImGui::DragFloat3("##0", &dirLightDirection.x, 0.1f, -1.0f, 1.0f);
	}
	if (ImGui::CollapsingHeader("PointLight"))
	{
		ImGui::Text(" Position:");
		ImGui::SameLine();
		ImGui::DragFloat3("##1", &pointLightPosition.x);
		ImGui::Text("    Atten:");
		ImGui::SameLine();
		ImGui::DragFloat3("##2", &pointLightAtten.x, 0.010f, 0.001f, 1.0f);
	}
	ImGui::End();


	//画面効果切り替え
	ImGui::Begin("PostEffect");
	ImGui::Checkbox("isDof", &isDof);
	int _effectType = peraEffectType;
	ImGui::RadioButton("None", &_effectType, ShaderType::PERA_SHADER);
	ImGui::RadioButton("Vignette", &_effectType, ShaderType::VIGNETTE_SHADER);
	ImGui::RadioButton("GameBoy", &_effectType, ShaderType::GAME_BOY_SHADER);
	ImGui::RadioButton("ChromaticAberration", &_effectType, ShaderType::CHROMATIC_ABERRATION_SHADER);
	ImGui::RadioButton("Toon", &_effectType, ShaderType::TOON_SHADER);
	ImGui::RadioButton("GrayScale", &_effectType, ShaderType::GRAY_SCALE_SHADER);
	ImGui::RadioButton("Mosaic", &_effectType, ShaderType::MOSAIC_SHADER);
	ImGui::RadioButton("GaussianBlur", &_effectType, ShaderType::GAUSSIAN_BLUR_SHADER);

	if (_effectType == ShaderType::PERA_SHADER)
	{
		peraEffectType = ShaderType::PERA_SHADER;
	}
	else if (_effectType == ShaderType::VIGNETTE_SHADER)
	{
		ImGui::Text("VignetteScale");
		ImGui::SliderFloat(" ", &vignetteScale, 0.0f, 5.0f);
		peraEffectType = ShaderType::VIGNETTE_SHADER;
		peraEffect->SetValue(vignetteScale);
	}
	else if (_effectType == ShaderType::GAME_BOY_SHADER)
	{
		ImGui::Text("PixelSize");
		ImGui::SliderFloat(" ", &gBoyPixelSize, 1.0f, 50.0f);
		peraEffectType = ShaderType::GAME_BOY_SHADER;
		peraEffect->SetValue(gBoyPixelSize);
	}
	else if (_effectType == ShaderType::CHROMATIC_ABERRATION_SHADER)
	{
		ImGui::Text("Misalignment");
		ImGui::SliderFloat(" ", &cAbeScale, 0.0f, 1.0f);
		peraEffectType = ShaderType::CHROMATIC_ABERRATION_SHADER;
		peraEffect->SetValue(cAbeScale);
	}
	else if (_effectType == ShaderType::TOON_SHADER)
	{
		peraEffectType = ShaderType::TOON_SHADER;
	}
	else if (_effectType == ShaderType::GRAY_SCALE_SHADER)
	{
		ImGui::Text("SepiaScale");
		ImGui::SliderFloat(" ", &sepiaScale, 0.0f, 1.0f);
		peraEffectType = ShaderType::GRAY_SCALE_SHADER;
		peraEffect->SetValue(sepiaScale);
	}
	else if (_effectType == ShaderType::MOSAIC_SHADER)
	{
		ImGui::Text("PixelSize");
		ImGui::SliderFloat(" ", &mosaicSize, 1.0f, 50.0f);
		peraEffectType = ShaderType::MOSAIC_SHADER;
		peraEffect->SetValue(mosaicSize);
	}
	else if (_effectType == ShaderType::GAUSSIAN_BLUR_SHADER)
	{
		ImGui::Text("BlurScale");
		ImGui::SliderFloat(" ", &blurScale, 0.0f, 5.0f);
		peraEffectType = ShaderType::GAUSSIAN_BLUR_SHADER;
		peraEffect->SetValue(blurScale);
	}

	ImGui::Text("ShaderColor");
	ImGui::ColorPicker4(" Color", color);
	shaderColor.x = color[0];
	shaderColor.y = color[1];
	shaderColor.z = color[2];
	//peraEffect->SetColor(shaderColor);

	ImGui::End();
}

void KochaEngine::Application::EngineLogo()
{
	if (!isAlphaChange)
	{
		if (logoAlpha < 1.00f)
		{
			logoAlpha += 0.02f;
		}
		else
		{
			alphaCount++;
		}
		if (alphaCount > 150)
		{
			isAlphaChange = true;
		}
	}
	else
	{
		if (logoAlpha > -0.80f)
		{
			logoAlpha -= 0.02f;
		}
		else
		{
			isLogoFlag = true;
		}
	}

}

bool KochaEngine::Application::UpdateFPS()
{
	// 今の時間を取得
	QueryPerformanceCounter(&timeEnd);
	// (今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
	frameTime = static_cast<double>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<double>(timeFreq.QuadPart);
	//処理時間に余裕がある場合はその分待つ
	if (frameTime < MIN_FRAME_TIME)
	{
		//Sleepの時間を計算
		DWORD sleepTime = static_cast<DWORD>((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
		return true;
	}
	fps = 1.000000f / frameTime;
	timeStart = timeEnd;
	return false;
}

bool KochaEngine::Application::Initialize()
{
	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);

	window = new Window;
	window->Create();
	window->IsCreated();

	Input::Init(window->GetHwnd());

	dx12 = new Dx12_Wrapper(window->GetHwnd());
	blob = new Dx12_Blob();
	descriptor = new Dx12_Descriptor(*dx12);
	rootSignature = new Dx12_RootSignature(*dx12);
	pipeline = new Dx12_Pipeline(*dx12, *blob);

	Texture2D::StaticInit(dx12->GetDevice().Get(), dx12->GetWinSize());
	PostEffect::StaticInit(dx12->GetDevice().Get(), dx12->GetCmdList().Get(), dx12->GetWinSize());
	Object::StaticInit(dx12->GetDevice().Get(), dx12->GetWinSize());
	LightManager::StaticInitialize(dx12->GetDevice().Get());

	Load();

	sceneManager = new SceneManager();
	sceneManager->AddScene(TITLE, new Title());
	sceneManager->AddScene(STAGESELECT, new StageSelect());
	sceneManager->AddScene(GAMEPLAY, new GamePlay());
	sceneManager->AddScene(ENDING, new Ending());
	sceneManager->AddScene(GAMEOVER, new GameOver());
	//sceneManager->ChangeScene(TITLE);

	LoadScene();
	LoadGameSettings();

	CustomGui::DefaultCustom();

	dirLightDirection = Vector3(0, 1, -1);
	dirLightColor = Vector3(1, 0.2f, 0);
	pointLightPosition = Vector3(0, 12, 0);
	pointLightColor = Vector3(1.0, 0.7, 0.0);
	pointLightAtten = Vector3(1.000f, 0.050f, 0.001f);
	isActiveDirLight = true;

	texture[0] = new Texture2D("Resources/engineLogo.png", Vector2(240, 180), Vector2(800, 600), 0);
	texture[1] = new Texture2D("Resources/white.png", Vector2(0, 0), Vector2(1280, 960), 0);

	peraBloom = new PostEffect();
	peraEffect = new PostEffect();
	peraDof = new PostEffect();
	peraEffectType = ShaderType::GAME_BOY_SHADER;
	isDof = false;

	vignetteScale = 0.2f;
	gBoyPixelSize = 5.81f;
	mosaicSize = 4.0f;
	sepiaScale = 0.2f;
	blurScale = 2.0f;
	cAbeScale = 0.4f;

	shaderColor = Vector4(1, 1, 1, 1);

	logoAlpha = 0.0f;
	alphaCount = 0;
	isLogoFlag = false;
	isAlphaChange = false;

	return true;
}

void KochaEngine::Application::Terminate()
{
	sceneManager->Terminate();
	delete sceneManager;
	delete texture[0];
	delete texture[1];
	delete blob;
	delete descriptor;
	delete rootSignature;
	delete pipeline;
	delete dx12;
	delete window;
	delete peraBloom;
	delete peraEffect;
	delete peraDof;

	Input::Terminate();
}

KochaEngine::Application& KochaEngine::Application::Instance()
{
	static Application instance;
	return instance;
}

SIZE KochaEngine::Application::GetWindowSize()const
{
	SIZE ret;
	ret.cx = window->GetWIN_WIDTH();
	ret.cy = window->GetWIN_HEIGHT();
	return ret;
}