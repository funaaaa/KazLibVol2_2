#pragma once
#include <d3d12.h>
#include "../dxcapi.h"
#include <string>
#include <wrl.h>
#include <vector>

// �V�F�[�_�[�f�[�^��ۑ����邽�߂̃f�[�^�N���X
class RayShaderData {

private:

	/*-- �����o�ϐ� --*/

	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob_;	// �V�F�[�_�[�̃f�[�^
	Microsoft::WRL::ComPtr<IDxcBlob> shaderBlobDxc_;	// �V�F�[�_�[�̃f�[�^ dxc�p
	std::vector<char> shaderBin_;					// dxc�ŃR���p�C�������ۂɏo�͂�����̕�����
	std::string entryPoint_;							// �V�F�[�_�[�̃G���g���[�|�C���g
	std::string shaderModel_;						// �V�F�[�_�[���f��
	std::string shaderPath_;							// �V�F�[�_�[�̃t�@�C���p�X ������g���ăV�F�[�_�[�𔻒f����B


public:

	/*-- �����o�֐� --*/

	// �R���X�g���N�^
	RayShaderData(const std::string& ShaderPath, const std::string& EntryPoint, const std::string& ShaderModel, bool IsDXC = false);

	// ���[�h����
	void LoadShader();
	void LoadShaderDXC();

	// �V�F�[�_�[�̃t�@�C���p�X���擾���鏈��
	const std::string& GetShaderPath() { return shaderPath_; }
	// �V�F�[�_�[�̃f�[�^���擾���鏈��
	const Microsoft::WRL::ComPtr<ID3DBlob>& GetShaderBlob() { return shaderBlob_; }
	const Microsoft::WRL::ComPtr<IDxcBlob>& GetShaderBlobDXC() { return shaderBlobDxc_; }
	std::vector<char>& GetShaderBin() { return shaderBin_; }

private:

	std::wstring StringToWString(std::string OString);

};