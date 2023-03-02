#include "RayRootsignature.h"
#include "../DirectXCommon/DirectX12Device.h"

void RayRootsignature::AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT ShaderRegister, UINT RegisterSpace)
{

	/*===== ���[�g�p�����[�^�[�ǉ����� =====*/


	if (Type == D3D12_DESCRIPTOR_RANGE_TYPE_SRV) {

		descRange_[rootparamCount_] = CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, ShaderRegister, RegisterSpace);
		rootparam_[rootparamCount_].InitAsDescriptorTable(1, &descRange_[rootparamCount_]);
		rootparam_[rootparamCount_].Descriptor.ShaderRegister = ShaderRegister;
		rootparam_[rootparamCount_].Descriptor.RegisterSpace = RegisterSpace;
		rootparam_[rootparamCount_].DescriptorTable.NumDescriptorRanges = 1;

	}
	else if (Type == D3D12_DESCRIPTOR_RANGE_TYPE_CBV) {

		rootparam_[rootparamCount_].InitAsConstantBufferView(ShaderRegister, RegisterSpace);
		rootparam_[rootparamCount_].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;

	}
	else if (Type == D3D12_DESCRIPTOR_RANGE_TYPE_UAV) {

		descRange_[rootparamCount_] = CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, ShaderRegister, RegisterSpace);
		rootparam_[rootparamCount_].InitAsDescriptorTable(1, &descRange_[rootparamCount_]);
		rootparam_[rootparamCount_].Descriptor.ShaderRegister = ShaderRegister;
		rootparam_[rootparamCount_].Descriptor.RegisterSpace = RegisterSpace;
		rootparam_[rootparamCount_].DescriptorTable.NumDescriptorRanges = 1;

	}



	// ���[�g�p�����[�^�[�̐����X�V
	++rootparamCount_;

}

void RayRootsignature::AddStaticSampler(int RegisterSpace) {

	/*===== �X�^�e�B�b�N�T���v���[�ǉ����� =====*/

	CD3DX12_STATIC_SAMPLER_DESC buff;
	buff.Init(0,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR
	);
	buff.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	buff.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	buff.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	buff.RegisterSpace = RegisterSpace;
	buff.ShaderRegister = 0;
	sampler_[samplerCount_] = buff;

	++samplerCount_;

}

void RayRootsignature::Build(bool IsLocal, const wchar_t* Name)
{

	/*===== ���[�g�V�O�l�`���������� =====*/

	// ���[�g�V�O�l�`���ݒ�p�\���̂�ݒ�B
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.NumParameters = rootparamCount_;
	rootSigDesc.pParameters = rootparam_.data();
	rootSigDesc.NumStaticSamplers = samplerCount_;
	rootSigDesc.pStaticSamplers = sampler_.data();

	// ���[�J�����[�g�V�O�l�`���̃t���O�������Ă�����A���[�J�����[�g�V�O�l�`���̃t���O��ݒ肷��B
	if (IsLocal) {

		rootSigDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;

	}

	// ���[�g�V�O�l�`�������B
	Microsoft::WRL::ComPtr<ID3DBlob> blob, errBlob;
	D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &blob, &errBlob);
	if (errBlob.Get() != nullptr) {
		std::string a = static_cast<char*>(errBlob->GetBufferPointer());
		_RPTF0(_CRT_WARN, a.c_str());
	}
	DirectX12Device::Instance()->raytracingDevice->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(),
		IID_PPV_ARGS(&rootsignature_));

	// ���O��ݒ�
	if (Name != nullptr) {
		rootsignature_->SetName(Name);
	}

}
