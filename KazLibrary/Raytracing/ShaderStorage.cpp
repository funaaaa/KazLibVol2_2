#include "ShaderStorage.h"
#include "ShaderData.h"
#include <assert.h>

#pragma comment(lib, "dxcompiler.lib")

Microsoft::WRL::ComPtr<ID3DBlob> ShaderStorage::LoadShader(const std::string& ShaderPath, const std::string& ShaderModel, const std::string& EntryPoint)
{

	/*-- �V�F�[�_�[�̃��[�h���� --*/

	const int SHADER_COUNT = static_cast<int>(shaderData_.size());

	// �V�F�[�_�[�̐������[�v���āA���[�h�ς݂̃V�F�[�_�[�����`�F�b�N����B
	for (int index = 0; index < SHADER_COUNT; ++index) {

		// �V�F�[�_�̖��O������Ă����玟�ցB
		if (shaderData_[index]->GetShaderPath() != ShaderPath) continue;

		// ����index�̃V�F�[�_�[�����^�[������B
		return shaderData_[index]->GetShaderBlob();

	}

	// �V�F�[�_�[�����[�h���ĕۑ��B
	shaderData_.emplace_back(std::make_unique<ShaderData>(ShaderPath, EntryPoint, ShaderModel));

	// �Ō���̃f�[�^�����^�[������B
	return shaderData_[shaderData_.size() - 1]->GetShaderBlob();
}

Microsoft::WRL::ComPtr<ID3DBlob> ShaderStorage::LoadShaderForDXC(const std::string& ShaderPath, const std::string& ShaderModel, const std::string& EntryPoint)
{

	/*-- �V�F�[�_�[�̃��[�h���� --*/

	const int SHADER_COUNT = static_cast<int>(shaderData_.size());

	// �V�F�[�_�[�̐������[�v���āA���[�h�ς݂̃V�F�[�_�[�����`�F�b�N����B
	for (int index = 0; index < SHADER_COUNT; ++index) {

		// �V�F�[�_�̖��O������Ă����玟�ցB
		if (shaderData_[index]->GetShaderPath() != ShaderPath) continue;

		// ����index�̃V�F�[�_�[�����^�[������B
		return shaderData_[index]->GetShaderBlob();

	}

	// �V�F�[�_�[�����[�h���ĕۑ��B
	shaderData_.emplace_back(std::make_unique<ShaderData>(ShaderPath, EntryPoint, ShaderModel, true));

	// �Ō���̃f�[�^�����^�[������B
	return shaderData_[shaderData_.size() - 1]->GetShaderBlob();

}

Microsoft::WRL::ComPtr<ID3DBlob> ShaderStorage::GetShaderData(const std::string& ShaderPath)
{

	/*-- �V�F�[�_�[�f�[�^��Ԃ����� --*/

	const int SHADER_COUNT = static_cast<int>(shaderData_.size());

	// �S�ẴV�F�[�_�[�f�[�^����������B
	for (int index = 0; index < SHADER_COUNT; ++index) {

		// �����̗v�f�������Ă��邩���`�F�b�N����B
		if (!(shaderData_[index]->GetShaderPath() == ShaderPath)) continue;

		return shaderData_[index]->GetShaderBlob();

	}

	return Microsoft::WRL::ComPtr<ID3DBlob>();
}

Microsoft::WRL::ComPtr<IDxcBlob> ShaderStorage::GetShaderDataForDXC(const std::string& ShaderPath)
{
	/*-- �V�F�[�_�[�f�[�^��Ԃ����� --*/

	const int SHADER_COUNT = static_cast<int>(shaderData_.size());

	// �S�ẴV�F�[�_�[�f�[�^����������B
	for (int index = 0; index < SHADER_COUNT; ++index) {

		// �����̗v�f�������Ă��邩���`�F�b�N����B
		if (!(shaderData_[index]->GetShaderPath() == ShaderPath)) continue;

		return shaderData_[index]->GetShaderBlobDXC();

	}

	return Microsoft::WRL::ComPtr<IDxcBlob>();
}

std::vector<char>& ShaderStorage::GetShaderBin(const std::string& ShaderPath)
{
	/*-- �V�F�[�_�[�f�[�^��Ԃ����� --*/

	const int SHADER_COUNT = static_cast<int>(shaderData_.size());

	// �S�ẴV�F�[�_�[�f�[�^����������B
	for (int index = 0; index < SHADER_COUNT; ++index) {

		// �����̗v�f�������Ă��邩���`�F�b�N����B
		if (!(shaderData_[index]->GetShaderPath() == ShaderPath)) continue;

		return shaderData_[index]->GetShaderBin();

	}

	// �V�F�[�_�[�����[�h����Ă��Ȃ��B
	assert(0);
	return shaderData_[0]->GetShaderBin();
}
