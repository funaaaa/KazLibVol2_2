#pragma once
#include <array>
#include <wrl.h>
#include "../DirectXCommon/d3dx12.h"

/// <summary>
/// ���C�g���[�V���O�Ŏg�p����O���[�o�����[�g�V�O�l�`���A���[�J�����[�g�V�O�l�`���N���X
/// </summary>
class RayRootsignature {

private:

	/*===== �萔 =====*/

	static const int MAX_ROOTPARAM = 16;
	static const int MAX_SAMPLER = 26;


private:

	/*===== �����o�ϐ� =====*/

	std::array<CD3DX12_ROOT_PARAMETER, MAX_ROOTPARAM> rootparam_;	// ���[�g�p�����[�^�[
	std::array<CD3DX12_DESCRIPTOR_RANGE, MAX_ROOTPARAM> descRange_;	// �f�B�X�N���v�^�e�[�u��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature_;		// ���[�g�V�O�l�`��
	std::array<CD3DX12_STATIC_SAMPLER_DESC, MAX_SAMPLER> sampler_;	// �X�^�e�B�b�N�T���v���[
	UINT rootparamCount_;											// ���[�g�p�����[�^�[��
	UINT samplerCount_;												// �T���v���[�̐�


public:

	/*===== �����o�֐� =====*/

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	RayRootsignature() {
		rootparamCount_ = 0;
		samplerCount_ = 0;
	}

	/// <summary>
	/// ���[�g�p�����[�^�[�ǉ�����
	/// </summary>
	/// <param name="Type"> �f�[�^�̎�� SRV�Ƃ�UAV�Ƃ� </param>
	/// <param name="ShaderRegister"> ���W�X�^�ԍ� t0�Ƃ���0�̕� </param>
	/// <param name="RegisterSpace"> ���W�X�^�X�y�[�X t0�̌�ɂ���space�̐��� </param>
	void AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT ShaderRegister, UINT RegisterSpace = 0);

	/// <summary>
	/// �X�^�e�B�b�N�T���v���[�ǉ�����
	/// </summary>
	/// <param name="RegisterSpace"> ���W�X�^�X�y�[�X t0�̌�ɂ���space�̐��� </param>
	void AddStaticSampler(int RegisterSpace = 0);

	/// <summary>
	/// ���[�g�V�O�l�`���̐���
	/// </summary>
	/// <param name="IsLocal"> ���[�J�����[�g�V�O�l�`�����ǂ��� </param>
	/// <param name="Name"> �o�b�t�@�̖��O </param>
	void Build(bool IsLocal, const wchar_t* Name = nullptr);

	// �Q�b�^
	inline Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSig() { return rootsignature_; }

};