#include "Dx12_Blob.h"
#include "Dx12_Wrapper.h"
#include "Util.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

KochaEngine::Dx12_Blob::Dx12_Blob()
{
	Init();
}

KochaEngine::Dx12_Blob::~Dx12_Blob()
{
}

HRESULT KochaEngine::Dx12_Blob::CompileShader(const CompileShaderType& arg_shaderType, const wchar_t* arg_ShaderName, const std::string& arg_EntryPoint, const std::string& arg_ShaderModel, Blob& arg_Blob)
{
	HRESULT result;
	switch (arg_shaderType)
	{
	case CompileShaderType::PS:
		result = D3DCompileFromFile(
			arg_ShaderName, //�V�F�[�_�[�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
			arg_EntryPoint.c_str(), arg_ShaderModel.c_str(), //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
			0, &arg_Blob.psBlob, &arg_Blob.errorBlob);
		break;
	case CompileShaderType::VS:
		result = D3DCompileFromFile(
			arg_ShaderName, //�V�F�[�_�[�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
			arg_EntryPoint.c_str(), arg_ShaderModel.c_str(), //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
			0, &arg_Blob.vsBlob, &arg_Blob.errorBlob);
		break;
	case CompileShaderType::GS:
		result = D3DCompileFromFile(
			arg_ShaderName, //�V�F�[�_�[�t�@�C����
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
			arg_EntryPoint.c_str(), arg_ShaderModel.c_str(), //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
			0, &arg_Blob.gsBlob, &arg_Blob.errorBlob);
		break;
	default:
		break;
	}
	return result;
}

void KochaEngine::Dx12_Blob::ErrorBlob(const HRESULT& result, const Blob& blob)
{
	//�V�F�[�_�[�̃G���[���e��\��
	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(blob.errorBlob->GetBufferSize());
		std::copy_n((char*)blob.errorBlob->GetBufferPointer(),
			blob.errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void KochaEngine::Dx12_Blob::Init()
{
	HRESULT result;

	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = CompileShader(CompileShaderType::VS, L"Shader/BasicVertexShader.hlsl", "VSmain", "vs_5_0", basicBlob);
	ErrorBlob(result, basicBlob);

	result = CompileShader(CompileShaderType::VS, L"Shader/TextureVertexShader.hlsl", "VSmain", "vs_5_0", texBlob);
	ErrorBlob(result, texBlob);

	result = CompileShader(CompileShaderType::VS, L"Shader/PMDVertexShader.hlsl", "VSmain", "vs_5_0", pmdBlob);
	ErrorBlob(result, pmdBlob);

	result = CompileShader(CompileShaderType::VS, L"Shader/FBXVertexShader.hlsl", "VSmain", "vs_5_0", fbxBlob);
	ErrorBlob(result, fbxBlob);

	result = CompileShader(CompileShaderType::VS, L"Shader/PeraVertexShader.hlsl", "main", "vs_5_0", peraBlob);
	ErrorBlob(result, peraBlob);

	result = CompileShader(CompileShaderType::VS, L"Shader/BasicVertexShader.hlsl", "ShadowVS", "vs_5_0", shadowBlob);
	ErrorBlob(result, shadowBlob);


	//�s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = CompileShader(CompileShaderType::PS, L"Shader/BasicPixelShader.hlsl", "PSmain", "ps_5_0", basicBlob);
	ErrorBlob(result, basicBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/TexturePixelShader.hlsl", "PSmain", "ps_5_0", texBlob);
	ErrorBlob(result, texBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/PMDPixelShader.hlsl", "PSmain", "ps_5_0", pmdBlob);
	ErrorBlob(result, pmdBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/FBXPixelShader.hlsl", "PSmain", "ps_5_0", fbxBlob);
	ErrorBlob(result, fbxBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/PeraPixelShader.hlsl", "main", "ps_5_0", peraBlob);
	ErrorBlob(result, peraBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/VignetteShader.hlsl", "main", "ps_5_0", vignetteBlob);
	ErrorBlob(result, vignetteBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/BloomShader.hlsl", "main", "ps_5_0", bloomBlob);
	ErrorBlob(result, bloomBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/GameBoyShader.hlsl", "main", "ps_5_0", gameBoyBlob);
	ErrorBlob(result, gameBoyBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/ChromaticAberrationShader.hlsl", "main", "ps_5_0", cAberrationBlob);
	ErrorBlob(result, cAberrationBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/ToonShader.hlsl", "main", "ps_5_0", toonBlob);
	ErrorBlob(result, toonBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/GrayScaleShader.hlsl", "main", "ps_5_0", grayScaleBlob);
	ErrorBlob(result, grayScaleBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/MosaicShader.hlsl", "main", "ps_5_0", mosaicBlob);
	ErrorBlob(result, mosaicBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/GaussianBlurShader.hlsl", "main", "ps_5_0", blurBlob);
	ErrorBlob(result, blurBlob);

	result = CompileShader(CompileShaderType::PS, L"Shader/DepthOfFieldShader.hlsl", "main", "ps_5_0", dofBlob);
	ErrorBlob(result, dofBlob);


	//�W�I���g���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = CompileShader(CompileShaderType::GS, L"Shader/BasicGeometryShader.hlsl", "GSmain", "gs_5_0", basicBlob);
	ErrorBlob(result, basicBlob);

	result = CompileShader(CompileShaderType::GS, L"Shader/BasicGeometryShader.hlsl", "GSmain", "gs_5_0", shadowBlob);
	ErrorBlob(result, shadowBlob);

}