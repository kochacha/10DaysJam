#include "PostEffect.h"
#include "Dx12_RootSignature.h"
#include "Dx12_Pipeline.h"
#include "Application.h"
#include "Input.h"
#include "Util.h"
#include <d3dx12.h>

ID3D12Device* KochaEngine::PostEffect::device{};
ID3D12GraphicsCommandList* KochaEngine::PostEffect::cmdList{};
SIZE KochaEngine::PostEffect::winSize{};

KochaEngine::PostEffect::PostEffect()
{
	Initialize();
	SetVertices();
	CreateBufferView();
}

KochaEngine::PostEffect::~PostEffect()
{
}

void KochaEngine::PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//リソースバリアを変更
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		auto resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmdList->ResourceBarrier(1, &resBarrier);
	}

	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[TEX_BUFF_COUNT];
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}

	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(TEX_BUFF_COUNT, rtvHs, false, &dsvH);

	//ビューポートの設定
	CD3DX12_VIEWPORT viewports[TEX_BUFF_COUNT];
	//シザリング矩形の設定
	CD3DX12_RECT scissorRects[TEX_BUFF_COUNT];
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, winSize.cx, winSize.cy);
		scissorRects[i] = CD3DX12_RECT(0, 0, winSize.cx, winSize.cy);
	}
	cmdList->RSSetViewports(TEX_BUFF_COUNT, viewports);
	cmdList->RSSetScissorRects(TEX_BUFF_COUNT, scissorRects);

	//全画面クリア
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		cmdList->ClearRenderTargetView(rtvHs[i], Application::clearColor, 0, nullptr);
	}
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void KochaEngine::PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//リソースバリアを変更
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		auto resBarrier = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		cmdList->ResourceBarrier(1, &resBarrier);
	}
}

void KochaEngine::PostEffect::Draw()
{
	time++;
	auto result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color1 = color[0];
	constMap->color2 = color[1];
	constMap->color3 = color[2];
	constMap->color4 = color[3];
	constMap->value = value;
	constMap->time = time;
	constBuff->Unmap(0, nullptr);

	cmdList->SetGraphicsRootSignature(Dx12_RootSignature::GetPeraRootSignature().Get());
	cmdList->SetPipelineState(Dx12_Pipeline::peraPipelineState.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->IASetVertexBuffers(0, 1, &vertBuffView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootDescriptorTable(1, 
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	cmdList->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	cmdList->SetGraphicsRootDescriptorTable(3,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 2,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//深度バッファテクスチャ
	cmdList->SetDescriptorHeaps(1, _depthSRVHeap.GetAddressOf());
	//auto handle = _depthSRVHeap->GetGPUDescriptorHandleForHeapStart();
	//handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cmdList->SetGraphicsRootDescriptorTable(4, _depthSRVHeap->GetGPUDescriptorHandleForHeapStart());

	cmdList->DrawInstanced(4, 1, 0, 0);
}

void KochaEngine::PostEffect::Draw(const ShaderType& arg_type)
{
	time++;
	auto result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color1 = color[0];
	constMap->color2 = color[1];
	constMap->color3 = color[2];
	constMap->color4 = color[3];
	constMap->value = value;
	constMap->time = time;
	constBuff->Unmap(0, nullptr);

	cmdList->SetGraphicsRootSignature(Dx12_RootSignature::GetPeraRootSignature().Get());

	switch (arg_type)
	{
	case ShaderType::PERA_SHADER: //通常
		cmdList->SetPipelineState(Dx12_Pipeline::peraPipelineState.Get());
		break;
	case ShaderType::VIGNETTE_SHADER: //ビネット
		cmdList->SetPipelineState(Dx12_Pipeline::vignettePipelineState.Get());
		break;
	case ShaderType::BLOOM_SHADER: //ブルーム
		cmdList->SetPipelineState(Dx12_Pipeline::bloomPipelineState.Get());
		break;
	case ShaderType::GAME_BOY_SHADER: //ゲームボーイ風
		cmdList->SetPipelineState(Dx12_Pipeline::gameBoyPipelineState.Get());
		break;
	case ShaderType::CHROMATIC_ABERRATION_SHADER: //色収差
		cmdList->SetPipelineState(Dx12_Pipeline::cAbePipelineState.Get());
		break;
	case ShaderType::TOON_SHADER: //階調
		cmdList->SetPipelineState(Dx12_Pipeline::toonPipelineState.Get());
		break;
	case ShaderType::GRAY_SCALE_SHADER: //グレースケール
		cmdList->SetPipelineState(Dx12_Pipeline::grayScalePipelineState.Get());
		break;
	case ShaderType::MOSAIC_SHADER: //モザイク
		cmdList->SetPipelineState(Dx12_Pipeline::mosaicPipelineState.Get());
		break;
	case ShaderType::GAUSSIAN_BLUR_SHADER: //ガウシアンブラー
		cmdList->SetPipelineState(Dx12_Pipeline::blurPipelineState.Get());
		break;
	case ShaderType::DEPTH_OF_FIELD_SHADER: //被写界深度
		cmdList->SetPipelineState(Dx12_Pipeline::dofPipelineState.Get());
		break;
	default: //通常
		cmdList->SetPipelineState(Dx12_Pipeline::peraPipelineState.Get());
		break;
	}

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	cmdList->IASetVertexBuffers(0, 1, &vertBuffView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	cmdList->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	cmdList->SetGraphicsRootDescriptorTable(3,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 2,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//深度バッファテクスチャ
	cmdList->SetDescriptorHeaps(1, _depthSRVHeap.GetAddressOf());
	//auto handle = _depthSRVHeap->GetGPUDescriptorHandleForHeapStart();
	//handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	cmdList->SetGraphicsRootDescriptorTable(4, _depthSRVHeap->GetGPUDescriptorHandleForHeapStart());

	cmdList->DrawInstanced(4, 1, 0, 0);
}

void KochaEngine::PostEffect::SetColorPalette(const PaletteType& arg_paletteType)
{
	switch (arg_paletteType)
	{
	case KochaEngine::GAMEBOY:
		color[0] = Vector4(0.06f, 0.22f, 0.06f, 1.0f);
		color[1] = Vector4(0.19f, 0.38f, 0.19f, 1.0f);
		color[2] = Vector4(0.45f, 0.60f, 0.06f, 1.0f);
		color[3] = Vector4(0.60f, 0.74f, 0.06f, 1.0f);
		break;
	case KochaEngine::SEPIA:
		color[0] = Vector4(0.20f, 0.11f, 0.00f, 1.0f);
		color[1] = Vector4(0.36f, 0.20f, 0.00f, 1.0f);
		color[2] = Vector4(0.54f, 0.30f, 0.00f, 1.0f);
		color[3] = Vector4(1.00f, 0.92f, 0.82f, 1.0f);
		break;
	case KochaEngine::PALETTE1:
		color[0] = Vector4(0.20f, 0.19f, 0.19f, 1.0f);
		color[1] = Vector4(0.54f, 0.52f, 0.52f, 1.0f);
		color[2] = Vector4(0.64f, 0.63f, 0.63f, 1.0f);
		color[3] = Vector4(0.89f, 0.87f, 0.87f, 1.0f);
		break;
	case KochaEngine::PALETTE2:
		color[0] = Vector4(0.71f, 0.53f, 0.69f, 1.0f);
		color[1] = Vector4(0.83f, 0.64f, 0.81f, 1.0f);
		color[2] = Vector4(0.97f, 0.73f, 0.86f, 1.0f);
		color[3] = Vector4(1.00f, 0.94f, 0.67f, 1.0f);
		break;
	case KochaEngine::PALETTE3:
		color[0] = Vector4(0.07f, 0.10f, 0.45f, 1.0f);
		color[1] = Vector4(0.89f, 0.17f, 0.39f, 1.0f);
		color[2] = Vector4(0.17f, 0.26f, 0.54f, 1.0f);
		color[3] = Vector4(0.38f, 0.29f, 0.82f, 1.0f);
		break;
	case KochaEngine::PALETTE4:
		color[0] = Vector4(0.94f, 0.47f, 0.06f, 1.0f);
		color[1] = Vector4(0.93f, 0.77f, 0.04f, 1.0f);
		color[2] = Vector4(0.77f, 0.05f, 0.40f, 1.0f);
		color[3] = Vector4(0.96f, 0.95f, 0.90f, 1.0f);
		break;
	case KochaEngine::PALETTE5:
		color[0] = Vector4(0.75f, 0.30f, 0.00f, 1.0f);
		color[1] = Vector4(0.83f, 0.37f, 0.00f, 1.0f);
		color[2] = Vector4(0.92f, 0.46f, 0.00f, 1.0f);
		color[3] = Vector4(0.93f, 0.93f, 0.93f, 1.0f);
		break;
	case KochaEngine::PALETTE6:
		color[0] = Vector4(0.00f, 0.00f, 0.63f, 1.0f);
		color[1] = Vector4(0.12f, 0.43f, 0.83f, 1.0f);
		color[2] = Vector4(0.22f, 0.73f, 0.90f, 1.0f);
		color[3] = Vector4(0.72f, 0.92f, 0.97f, 1.0f);
		break;
	case KochaEngine::PALETTE7:
		color[0] = Vector4(0.36f, 0.25f, 0.18f, 1.0f);
		color[1] = Vector4(0.99f, 0.92f, 0.71f, 1.0f);
		color[2] = Vector4(0.94f, 0.47f, 0.09f, 1.0f);
		color[3] = Vector4(0.47f, 0.75f, 0.65f, 1.0f);
		break;
	case KochaEngine::PALETTE8:
		color[0] = Vector4(0.30f, 0.05f, 0.18f, 1.0f);
		color[1] = Vector4(0.58f, 0.48f, 0.53f, 1.0f);
		color[2] = Vector4(0.88f, 0.90f, 0.92f, 1.0f);
		color[3] = Vector4(0.68f, 0.72f, 0.76f, 1.0f);
		break;
	case KochaEngine::PALETTE9:
		color[0] = Vector4(0.12f, 0.28f, 0.41f, 1.0f);
		color[1] = Vector4(0.03f, 1.00f, 0.78f, 1.0f);
		color[2] = Vector4(0.85f, 0.85f, 0.85f, 1.0f);
		color[3] = Vector4(1.00f, 0.96f, 0.96f, 1.0f);
		break;

	case KochaEngine::PALETTE10:
		float rand[12];
		for (int i = 0; i < 12; i++)
		{
			rand[i] = (float)Util::GetIntRand(0, 255) / 255.0f;
		}
		color[0] = Vector4(rand[0], rand[1], rand[2], 1.0f);
		color[1] = Vector4(rand[3], rand[4], rand[5], 1.0f);
		color[2] = Vector4(rand[6], rand[7], rand[8], 1.0f);
		color[3] = Vector4(rand[9], rand[10], rand[11], 1.0f);
		break;
	default:
		break;
	}
}

void KochaEngine::PostEffect::StaticInit(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const SIZE winSize)
{
	KochaEngine::PostEffect::winSize = winSize;

	if (device == nullptr) return;
	KochaEngine::PostEffect::device = device;

	if (cmdList == nullptr) return;
	KochaEngine::PostEffect::cmdList = cmdList;
}

void KochaEngine::PostEffect::Initialize()
{
	paletteType = KochaEngine::PaletteType::PALETTE1;
	SetColorPalette(paletteType);
	time = 0;
	value = 0;

	HRESULT result;

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		winSize.cx,
		(UINT)winSize.cy,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	//テクスチャバッファの生成
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
		auto clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, Application::clearColor);
		for (int i = 0; i < TEX_BUFF_COUNT; i++)
		{
			result = device->CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&texDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearValue,
				IID_PPV_ARGS(&texBuff[i]));
			assert(SUCCEEDED(result));

			//画素数
			const UINT pixelCount = winSize.cx * winSize.cy;
			//画像一桁分のデータサイズ
			const UINT rowPitch = sizeof(UINT) * winSize.cx;
			//画像全体のデータサイズ
			const UINT depthPitch = rowPitch * winSize.cy;
			//画像イメージ
			UINT* img = new UINT[pixelCount];
			for (int j = 0; j < pixelCount; j++)
			{
				img[j] = 0xff0000ff;
			}

			//テクスチャバッファにデータ転送
			result = texBuff[i]->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
			assert(SUCCEEDED(result));
			delete[] img;
		}

	}

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc{};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = TEX_BUFF_COUNT;

	//SRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV作成
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		device->CreateShaderResourceView(texBuff[i].Get(), &srvDesc,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapSRV->GetCPUDescriptorHandleForHeapStart(), i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}

	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = TEX_BUFF_COUNT;

	//RTV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにRTV作成
	for (int i = 0; i < TEX_BUFF_COUNT; i++)
	{
		device->CreateRenderTargetView(texBuff[i].Get(), nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(),i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		winSize.cx,
		winSize.cy,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//深度バッファの生成
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&depthResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(&depthBuff));
		assert(SUCCEEDED(result));
	}
	{
		depthResDesc.Width = winSize.cx / 200;
		depthResDesc.Height = winSize.cy / 200;
		depthResDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);
		result = device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&depthResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(lightDepthBuff.ReleaseAndGetAddressOf()));
		assert(SUCCEEDED(result));
	}

	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descHeapDesc.NumDescriptors = 2;

	//DSV用デスクリプタヒープを作成
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSV作成
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		auto handle = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

		device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, handle);
		handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		device->CreateDepthStencilView(lightDepthBuff.Get(), &dsvDesc, handle);
	
	}
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = 2;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

		result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_depthSRVHeap));
	}
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC resDesc = {};
		resDesc.Format = DXGI_FORMAT_R32_FLOAT;
		resDesc.Texture2D.MipLevels = 1;
		resDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		resDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		auto handle = _depthSRVHeap->GetCPUDescriptorHandleForHeapStart();

		device->CreateShaderResourceView(depthBuff.Get(), &resDesc, handle);
		handle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		device->CreateShaderResourceView(lightDepthBuff.Get(), &resDesc, handle);
	}
}

void KochaEngine::PostEffect::SetVertices()
{
	vertex[0] = { {-1, -1, 0.1f}, {0, 1} }; //左下
	vertex[1] = { {-1,  1, 0.1f}, {0, 0} }; //左上
	vertex[2] = { { 1, -1, 0.1f}, {1, 1} }; //右下
	vertex[3] = { { 1,  1, 0.1f}, {1, 0} }; //右上
//#ifdef DEBUG
//	vertex[0] = { {-1, -1, 0.1f}, {0, 1} }; //左下
//	vertex[1] = { {-1,  1, 0.1f}, {0, 0} }; //左上
//	vertex[2] = { { 1, -1, 0.1f}, {1, 1} }; //右下
//	vertex[3] = { { 1,  1, 0.1f}, {1, 0} }; //右上
//#else
//	vertex[0] = { {-0.9f, -0.9f, 0.1f}, {0, 1} }; //左下
//	vertex[1] = { {-0.9f,  0.9f, 0.1f}, {0, 0} }; //左上
//	vertex[2] = { { 0.9f, -0.9f, 0.1f}, {1, 1} }; //右下
//	vertex[3] = { { 0.9f,  0.9f, 0.1f}, {1, 0} }; //右上
//#endif


}

void KochaEngine::PostEffect::CreateBufferView()
{
	auto heap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertex));
	auto result = device->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertBuff.ReleaseAndGetAddressOf()));

	vertBuffView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vertBuffView.SizeInBytes = sizeof(vertex);
	vertBuffView.StrideInBytes = sizeof(PeraVartex);

	PeraVartex* vertMap = nullptr;
	vertBuff->Map(0, nullptr, (void**)&vertMap);
	std::copy(std::begin(vertex), std::end(vertex), vertMap);
	vertBuff->Unmap(0, nullptr);

	// 定数バッファの生成
	{
		auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);
		result = device->CreateCommittedResource(
			&heapProp, 	// アップロード可能
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff));
	}
}
