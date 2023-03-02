#pragma once
#include <memory>
#include <vector>
#include <d3d12.h>
#include <string>
#include <wrl/client.h>
#include "../Helper/ISinglton.h"
#include "dxcapi.h"

// �V�F�[�_�[�f�[�^�N���X�̑O���錾
class ShaderData;

/// <summary>
/// �V�F�[�_�[�f�[�^�ۑ��N���X ���C�g���p�Ɏ����Ă��܂����B�V�����G���W���ł̓��X�^���C�Y�Ŏg���Ă��Ɠ��������܂��B
/// </summary>
class ShaderStorage : public ISingleton<ShaderStorage> {

private:

	/*-- �����o�ϐ� --*/

	std::vector<std::shared_ptr<ShaderData>> shaderData_;


public:

	/*-- �����o�֐� --*/

	// �V�F�[�_�[�����[�h����B
	Microsoft::WRL::ComPtr<ID3DBlob> LoadShader(const std::string& ShaderPath, const std::string& ShaderModel, const std::string& EntryPoint);
	Microsoft::WRL::ComPtr<ID3DBlob> LoadShaderForDXC(const std::string& ShaderPath, const std::string& ShaderModel, const std::string& EntryPoint);

	// �V�F�[�_�[�f�[�^��Ԃ��B
	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderData(const std::string& ShaderPath);
	Microsoft::WRL::ComPtr<IDxcBlob> GetShaderDataForDXC(const std::string& ShaderPath);
	std::vector<char>& GetShaderBin(const std::string& ShaderPath);

};


