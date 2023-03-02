#include "RayShaderData.h"
#include <d3dcompiler.h>
#include <system_error>
#include <fstream>
#include <iostream>
#include <sstream>
#include <array>
#include "../KazLibrary/Helper/KazHelper.h"

#pragma comment(lib, "dxcompiler.lib")

RayShaderData::RayShaderData(const std::string& shaderPath_, const std::string& entryPoint_, const std::string& shaderModel_, bool isDXC)
{

	/*-- �R���X�g���N�^ --*/

	// ������ۑ��B
	this->shaderPath_ = shaderPath_;
	this->entryPoint_ = entryPoint_;
	this->shaderModel_ = shaderModel_;
	this->shaderBlob_ = nullptr;

	if (isDXC) {

		//�V�F�[�_�[�����[�h�B
		LoadShaderDXC();

	}
	else {

		//�V�F�[�_�[�����[�h�B
		LoadShader();

	}

}

void RayShaderData::LoadShader()
{

	/*-- �V�F�[�_�[�����[�h���鏈�� --*/

	std::array<wchar_t, 128> shaderPathBuff;
	KazHelper::ConvertStringToWchar_t(shaderPath_, shaderPathBuff.data(), 128);

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(
		shaderPathBuff.data(),								//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,					//�C���N���[�h�\�ɂ���
		entryPoint_.c_str(), shaderModel_.c_str(),			//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//�f�o�b�O�p�ݒ�
		0,
		&shaderBlob_, &errorBlob);

	// �V�F�[�_�[�̃G���[�`�F�b�N
	if (FAILED(result)) {

		// hresult����G���[���b�Z�[�W���擾
		std::string errorMsg = std::system_category().message(result);

		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

}

void RayShaderData::LoadShaderDXC()
{
	HRESULT hr;
	std::ifstream infile(shaderPath_, std::ifstream::binary);
	if (!infile) {
		throw std::runtime_error("failed shader compile.");
	}

	// �^�ϊ��B
	std::wstring fileName = StringToWString(shaderPath_);
	std::stringstream strstream;

	strstream << infile.rdbuf();

	std::string shaderCode_ = strstream.str();
	Microsoft::WRL::ComPtr<IDxcLibrary> library;
	DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	Microsoft::WRL::ComPtr<IDxcCompiler> compiler;
	DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
	Microsoft::WRL::ComPtr<IDxcBlobEncoding> source_;

	library->CreateBlobWithEncodingFromPinned(
		(LPBYTE)shaderCode_.c_str(), static_cast<UINT32>(shaderCode_.size()), CP_UTF8, &source_);
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler;

	// �C���N���[�h���g���ꍇ�ɂ͓K�؂ɐݒ肷�邱��
	library->CreateIncludeHandler(&includeHandler);
	// �R���p�C���I�v�V�����̎w��
	std::vector<LPCWSTR> arguments;

	arguments.emplace_back(L"/enable-16bit-types");		// 16bit�ϐ����g�p�\�ɂ���B

	// �V�F�[�_�[���f���͈�U����ŌŒ肷��B
	const auto target_ = L"lib_6_4";

	Microsoft::WRL::ComPtr<IDxcOperationResult> dxcResult;
	hr = compiler->Compile(source_.Get(), fileName.c_str(),
		L"", target_, arguments.data(), static_cast<UINT>(arguments.size()),
		nullptr, 0, includeHandler.Get(), &dxcResult);

	if (FAILED(hr)) {
		throw std::runtime_error("failed shader compile.");
	}

	dxcResult->GetStatus(&hr);
	if (FAILED(hr)) {
		Microsoft::WRL::ComPtr<IDxcBlobEncoding> errBlob;
		dxcResult->GetErrorBuffer(&errBlob);
		throw std::runtime_error("failed shader compile");
	}

	Microsoft::WRL::ComPtr<IDxcBlob> blob;
	dxcResult->GetResult(&blob);

	shaderBlobDxc_ = blob;

	std::vector<char> result;
	auto size = blob->GetBufferSize();
	result.resize(size);
	memcpy(result.data(), blob->GetBufferPointer(), size);
	shaderBin_ = result;

}

std::wstring RayShaderData::StringToWString(std::string OString)
{
	// SJIS �� wstring
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, OString.c_str()
		, -1, (wchar_t*)NULL, 0);

	// �o�b�t�@�̎擾
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];

	// SJIS �� wstring
	MultiByteToWideChar(CP_ACP, 0, OString.c_str(), -1, cpUCS2
		, iBufferSize);

	// string�̐���
	std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

	// �o�b�t�@�̔j��
	delete[] cpUCS2;

	// �ϊ����ʂ�Ԃ�
	return(oRet);
}
