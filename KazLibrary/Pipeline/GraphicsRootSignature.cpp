#include"../Pipeline/GraphicsRootSignature.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Helper/KazHelper.h"
#include<assert.h>

#define STR(var) #var   //�����ɂ����ϐ���ϐ��������������񃊃e�����Ƃ��ĕԂ��}�N���֐�

GraphicsRootSignature::GraphicsRootSignature()
{
	paramD.resize(50);
	rootSignature.resize(50);
}

GraphicsRootSignature::~GraphicsRootSignature()
{
}

void GraphicsRootSignature::CreateRootSignature(RootSignatureMode ROOTSIGNATURE, RootSignatureData ROOTSIGNATURE_DATA, short DATA_MAX)
{
	vector<CD3DX12_ROOT_PARAMETER> rootparam(DATA_MAX);
	vector<CD3DX12_DESCRIPTOR_RANGE> descRangeSRV;
	descRangeSRV.reserve(DATA_MAX);

#pragma region ���[�g�p�����̐ݒ�

	int param1 = 0;
	int param2 = 0;
	paramD[ROOTSIGNATURE].paramMax = DATA_MAX;
	for (int i = 0; i < DATA_MAX; i++)
	{
		switch (ROOTSIGNATURE_DATA.range[i])
		{
		case GRAPHICS_RANGE_TYPE_CBV_VIEW:
			//���[�g�p�����̐ݒ�
			rootparam[i].InitAsConstantBufferView(param1, 0, D3D12_SHADER_VISIBILITY_ALL);

			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_CBV_VIEW;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;
			param1++;
			break;

		case GRAPHICS_RANGE_TYPE_SRV_DESC:
			//���[�g�p�����̐ݒ�
			descRangeSRV.push_back({});
			descRangeSRV[descRangeSRV.size() - 1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, param2);
			rootparam[i].InitAsDescriptorTable(1, &descRangeSRV[descRangeSRV.size() - 1], D3D12_SHADER_VISIBILITY_ALL);

			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_SRV_DESC;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;
			param2++;
			break;

		case GRAPHICS_RANGE_TYPE_SRV_VIEW:
			//���[�g�p�����̐ݒ�
			rootparam[i].InitAsShaderResourceView(param1, 0, D3D12_SHADER_VISIBILITY_ALL);

			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_SRV_DESC;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;
			param1++;
			break;

		case GRAPHICS_RANGE_TYPE_CBV_DESC:
			//���[�g�p�����̐ݒ�
			descRangeSRV.push_back({});
			descRangeSRV[descRangeSRV.size() - 1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, param2);
			rootparam[i].InitAsDescriptorTable(1, &descRangeSRV[descRangeSRV.size() - 1], D3D12_SHADER_VISIBILITY_ALL);


			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_CBV_DESC;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;
			param2++;
			break;

		case GRAPHICS_RANGE_TYPE_UAV_VIEW:
			//���[�g�p�����̐ݒ�
			rootparam[i].InitAsUnorderedAccessView(param1, 0, D3D12_SHADER_VISIBILITY_ALL);

			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_UAV_VIEW;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;
			param1++;
			break;

		case GRAPHICS_RANGE_TYPE_UAV_DESC:
			//���[�g�p�����̐ݒ�
			descRangeSRV.push_back({});
			descRangeSRV[descRangeSRV.size() - 1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, param2);
			rootparam[i].InitAsDescriptorTable(1, &descRangeSRV[descRangeSRV.size() - 1], D3D12_SHADER_VISIBILITY_ALL);

			//�󂯓n���p
			paramD[ROOTSIGNATURE].range[i] = GRAPHICS_RANGE_TYPE_UAV_VIEW;
			paramD[ROOTSIGNATURE].paramData[i].param = ROOTSIGNATURE_DATA.paramData[i].param;
			paramD[ROOTSIGNATURE].paramData[i].type = ROOTSIGNATURE_DATA.paramData[i].type;
			paramD[ROOTSIGNATURE].type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;
			param2++;
		case -1:
			break;
		}
	}
#pragma endregion

	if (ROOTSIGNATURE == ROOTSIGNATURE_DATA_SRV_UAV)
	{
		//�R���s���[�g�V�F�[�_�[�p�̃��[�g�V�O�l�`���[(�Վ��p)
		std::array <CD3DX12_DESCRIPTOR_RANGE, 3> ranges{};
		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
		ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);
		ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

		std::array<CD3DX12_ROOT_PARAMETER, 3> computeRootParameters;
		computeRootParameters[0].InitAsDescriptorTable(1, &ranges[0]);
		computeRootParameters[1].InitAsDescriptorTable(1, &ranges[1]);
		computeRootParameters[2].InitAsConstantBufferView(0, 0);
		//computeRootParameters[5].InitAsDescriptorTable(1, &ranges[4]);
		CreateMyRootSignature(ROOTSIGNATURE_DATA.sample, computeRootParameters.data(), computeRootParameters.size(), ROOTSIGNATURE);
	}
	else if (ROOTSIGNATURE == ROOTSIGNATURE_DATA_DRAW_UAV)
	{
		std::array<CD3DX12_ROOT_PARAMETER, 1> computeRootParameters;
		computeRootParameters[0].InitAsUnorderedAccessView(0, 0);
		CreateMyRootSignature(ROOTSIGNATURE_DATA.sample, computeRootParameters.data(), computeRootParameters.size(), ROOTSIGNATURE);

	}
	else if (ROOTSIGNATURE == ROOTSIGNATURE_DATA_UAB_UAB_UAB_CB)
	{
		std::array<CD3DX12_ROOT_PARAMETER, 5> computeRootParameters;
		std::array <CD3DX12_DESCRIPTOR_RANGE, 4> ranges{};
		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
		ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);
		ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 2);
		ranges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 3);
		computeRootParameters[0].InitAsDescriptorTable(1, &ranges[0]);
		computeRootParameters[1].InitAsDescriptorTable(1, &ranges[1]);
		computeRootParameters[2].InitAsDescriptorTable(1, &ranges[2]);
		computeRootParameters[3].InitAsDescriptorTable(1, &ranges[3]);
		computeRootParameters[4].InitAsConstantBufferView(0, 0);

		CreateMyRootSignature(ROOTSIGNATURE_DATA.sample, computeRootParameters.data(), computeRootParameters.size(), ROOTSIGNATURE);
	}
	else if (ROOTSIGNATURE == ROOTSIGNATURE_DATA_UAVDESC_UAVVIEW_CBV)
	{
		std::array<CD3DX12_ROOT_PARAMETER, 3> computeRootParameters;
		std::array <CD3DX12_DESCRIPTOR_RANGE, 2> ranges{};
		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);
		ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);
		computeRootParameters[0].InitAsDescriptorTable(1, &ranges[0]);
		computeRootParameters[1].InitAsUnorderedAccessView(1, 0);
		computeRootParameters[2].InitAsConstantBufferView(2, 0);

		CreateMyRootSignature(ROOTSIGNATURE_DATA.sample, computeRootParameters.data(), computeRootParameters.size(), ROOTSIGNATURE);

	}
	else
	{
		CreateMyRootSignature(ROOTSIGNATURE_DATA.sample, rootparam.data(), rootparam.size(), ROOTSIGNATURE);
	}


	if (ROOTSIGNATURE == ROOTSIGNATURE_DATA_SRV_UAV)
	{
		rootSignature[ROOTSIGNATURE]->SetName(L"ComputeRootSignature");
	}
	else
	{
		rootSignature[ROOTSIGNATURE]->SetName(L"RootSignature");
	}
}

void GraphicsRootSignature::SetRootSignature(RootSignatureMode ROOTSIGNATURE_MODE)
{
	DirectX12CmdList::Instance()->cmdList->SetGraphicsRootSignature(
		rootSignature[ROOTSIGNATURE_MODE].Get()
	);
}

void GraphicsRootSignature::SetComputeRootSignature(RootSignatureMode ROOTSIGNATURE_MODE)
{
	DirectX12CmdList::Instance()->cmdList->SetComputeRootSignature(
		rootSignature[ROOTSIGNATURE_MODE].Get()
	);
}

const Microsoft::WRL::ComPtr<ID3D12RootSignature> GraphicsRootSignature::GetRootSignature(RootSignatureMode ROOT_SIGNATURE)
{
	return rootSignature[ROOT_SIGNATURE];
}

void GraphicsRootSignature::CreateMyRootSignature(D3D12_STATIC_SAMPLER_DESC SAMPLER_DATA, D3D12_ROOT_PARAMETER *ROOT_PARAM_DATA, size_t DATA_MAX, RootSignatureMode ROOTSIGNATURE)
{
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(
		static_cast<UINT>(DATA_MAX),
		ROOT_PARAM_DATA,
		1,
		&SAMPLER_DATA,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//�o�[�W������������ł̃V���A���C�Y
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT lR = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);

	if (lR != S_OK)
	{
		assert(0);
	}

	//���[�g�V�O�l�`���̐���
	DirectX12Device::Instance()->dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature[ROOTSIGNATURE])
	);
}

void GraphicsRootSignature::CreateMyRootSignature(D3D12_STATIC_SAMPLER_DESC SAMPLER_DATA, D3D12_ROOT_PARAMETER1 *ROOT_PARAM_DATA, size_t DATA_MAX, RootSignatureMode ROOTSIGNATURE)
{
	SAMPLER_DATA = {};
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	if (ROOTSIGNATURE == ROOTSIGNATURE_DATA_DRAW)
	{
		rootSignatureDesc.Init_1_1(static_cast<UINT>(DATA_MAX), ROOT_PARAM_DATA, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	}
	else
	{
		rootSignatureDesc.Init_1_1(static_cast<UINT>(DATA_MAX), ROOT_PARAM_DATA);
	}
	//�o�[�W������������ł̃V���A���C�Y
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);

	//���[�g�V�O�l�`���̐���
	DirectX12Device::Instance()->dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature[ROOTSIGNATURE])
	);
}

const GraphicsRootSignatureParameter &GraphicsRootSignature::GetRootParam(RootSignatureMode ROOTSIGNATURE_MODE)
{
	return paramD[ROOTSIGNATURE_MODE];
}

const std::vector<RootSignatureParameter> &GraphicsRootSignature::GetRootParam(int ROOTSIGNATURE_HANDLE)
{
	return computeParamArrayData[ROOTSIGNATURE_HANDLE];
}

Microsoft::WRL::ComPtr<ID3D12RootSignature> GraphicsRootSignature::CreateRootSignature(const RootSignatureDataTest &ROOTSIGNATURE_DATA, RootsignatureType TYPE)
{
	std::vector<CD3DX12_ROOT_PARAMETER> lRootparamArray(ROOTSIGNATURE_DATA.rangeArray.size());

	int lCountDescriptorNum = 0;
	//�f�X�N���v�^�q�[�v�̐ݒ肪�K�v�Ȑ����v�Z����
	for (int i = 0; i < ROOTSIGNATURE_DATA.rangeArray.size(); ++i)
	{
		const bool L_THIS_TYPE_IS_DESCRIPTOR_FLAG = ROOTSIGNATURE_DATA.rangeArray[i].rangeType % 2 == 0;
		if (L_THIS_TYPE_IS_DESCRIPTOR_FLAG)
		{
			++lCountDescriptorNum;
		}
	}
	std::vector<CD3DX12_DESCRIPTOR_RANGE> lDescRangeRangeArray(lCountDescriptorNum);


#pragma region ���[�g�p�����[�^�[��ݒ肷��
	ParamData lViewData;
	UINT lDescriptorArrayNum = 0;

	std::vector<RootSignatureParameter> lParamArrayData;
	for (int i = 0; i < ROOTSIGNATURE_DATA.rangeArray.size(); i++)
	{
		switch (ROOTSIGNATURE_DATA.rangeArray[i].rangeType)
		{
		case GRAPHICS_RANGE_TYPE_CBV_VIEW:
			//���[�g�p�����̐ݒ�
			lRootparamArray[i].InitAsConstantBufferView(lViewData.cbv, 0, D3D12_SHADER_VISIBILITY_ALL);
			++lViewData.cbv;
			break;

		case GRAPHICS_RANGE_TYPE_CBV_DESC:
			//���[�g�p�����̐ݒ�
			lDescRangeRangeArray[lDescriptorArrayNum].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, lViewData.cbv);
			lRootparamArray[i].InitAsDescriptorTable(1, &lDescRangeRangeArray[lDescriptorArrayNum], D3D12_SHADER_VISIBILITY_ALL);
			++lViewData.cbv;
			break;

		case GRAPHICS_RANGE_TYPE_SRV_VIEW:
			//���[�g�p�����̐ݒ�
			lRootparamArray[i].InitAsShaderResourceView(lViewData.srv, 0, D3D12_SHADER_VISIBILITY_ALL);
			++lViewData.srv;
			break;

		case GRAPHICS_RANGE_TYPE_SRV_DESC:
			//���[�g�p�����̐ݒ�
			lDescRangeRangeArray[lDescriptorArrayNum].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, lViewData.srv);
			lRootparamArray[i].InitAsDescriptorTable(1, &lDescRangeRangeArray[lDescriptorArrayNum], D3D12_SHADER_VISIBILITY_ALL);
			++lViewData.srv;
			break;


		case GRAPHICS_RANGE_TYPE_UAV_VIEW:
			//���[�g�p�����̐ݒ�
			lRootparamArray[i].InitAsUnorderedAccessView(lViewData.uav, 0, D3D12_SHADER_VISIBILITY_ALL);
			++lViewData.uav;
			break;

		case GRAPHICS_RANGE_TYPE_UAV_DESC:
			//���[�g�p�����̐ݒ�
			lDescRangeRangeArray[lDescriptorArrayNum].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, lViewData.uav);
			lRootparamArray[i].InitAsDescriptorTable(1, &lDescRangeRangeArray[lDescriptorArrayNum], D3D12_SHADER_VISIBILITY_ALL);
			++lViewData.uav;
			break;
		case -1:
			break;
		}

#pragma region ���[�g�p�����[�^�[�����ݒ���s���ׂ̐ݒ�
		RootSignatureParameter lData;
		lData.range = ROOTSIGNATURE_DATA.rangeArray[i].rangeType;
		lData.paramData.type = ROOTSIGNATURE_DATA.rangeArray[i].dataType;

		const bool L_THIS_TYPE_IS_DESCRIPTOR_FLAG = ROOTSIGNATURE_DATA.rangeArray[i].rangeType % 2 == 0;
		if (L_THIS_TYPE_IS_DESCRIPTOR_FLAG)
		{
			lData.type = GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE;
			++lDescriptorArrayNum;
		}
		else
		{
			lData.type = GRAPHICS_ROOTSIGNATURE_TYPE_VIEW;
		}
		lData.paramData.param = lViewData.TotalNum() - 1;
		lParamArrayData.push_back(lData);
#pragma endregion
	}

#pragma endregion

#pragma region �o�[�W������������ł̃V���A���C�Y

	D3D12_ROOT_SIGNATURE_FLAGS lRootSignatureType = D3D12_ROOT_SIGNATURE_FLAG_NONE;
	switch (TYPE)
	{
	case ROOTSIGNATURE_NONE:
		break;
	case ROOTSIGNATURE_GRAPHICS:
		lRootSignatureType = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		break;
	case ROOTSIGNATURE_COMPUTE:
		lRootSignatureType = D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS;
		break;
	default:
		break;
	}


	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC lRootSignatureDesc;
	lRootSignatureDesc.Init_1_0(
		static_cast<UINT>(lRootparamArray.size()),
		lRootparamArray.data(),
		static_cast<UINT>(ROOTSIGNATURE_DATA.samplerArray.size()),
		ROOTSIGNATURE_DATA.samplerArray.data(),
		lRootSignatureType);

	Microsoft::WRL::ComPtr<ID3DBlob> lRootSigBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> lErrorBlob = nullptr;
	HRESULT lResult = D3DX12SerializeVersionedRootSignature(
		&lRootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&lRootSigBlob,
		&lErrorBlob);

	if (lResult != S_OK)
	{
		assert(0);
	}
#pragma endregion




	//���[�g�V�O�l�`���̐���
	Microsoft::WRL::ComPtr<ID3D12RootSignature> lRootSignature;
	DirectX12Device::Instance()->dev->CreateRootSignature(
		0,
		lRootSigBlob->GetBufferPointer(),
		lRootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&lRootSignature)
	);

	//�R���s���[�g�p�C�v���C���p�̃p�����[�^�[�ۊ�
	computeParamArrayData.push_back(lParamArrayData);


	return lRootSignature;
}

