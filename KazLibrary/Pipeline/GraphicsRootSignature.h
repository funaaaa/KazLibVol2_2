#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"
#include<vector>
#include"../KazLibrary/Helper/HandleMaker.h"

using namespace std;

enum RootsignatureType
{
	ROOTSIGNATURE_NONE,
	ROOTSIGNATURE_GRAPHICS,
	ROOTSIGNATURE_COMPUTE,
};

/// <summary>
/// ���[�g�V�O�l�`���[�̎��
/// </summary>
enum RootSignatureMode
{
	ROOTSIGNATURE_DATA_DRAW,
	ROOTSIGNATURE_DATA_DRAW_UAV,
	ROOTSIGNATURE_DATA_UAV,
	ROOTSIGNATURE_DATA_DRAW_TEX,
	ROOTSIGNATURE_DATA_DRAW_TEX_DATA1,
	ROOTSIGNATURE_DATA_DRAW_TEX_TEX2,
	ROOTSIGNATURE_DATA_DRAW_DATA1_TEX_TEX2,
	ROOTSIGNATURE_DATA_DRAW_UAV_DATA2_TEX_TEX2,
	ROOTSIGNATURE_DATA_DRAW_UAV_DATA_TEX,
	ROOTSIGNATURE_DATA_UAV_UAV_UAV_CBV,
	ROOTSIGNATURE_DATA_UAV_UAV_UAV_CBV_ALLVIEW,
	ROOTSIGNATURE_DATA_UAV_CBV,
	ROOTSIGNATURE_DATA_UAV_UAV_CBV,
	ROOTSIGNATURE_DATA_UAVDESC_UAVVIEW_CBV,
	ROOTSIGNATURE_DATA_UAVVIEW_UAVDESC_CBV,
	ROOTSIGNATURE_DATA_UAV_UAV_CBV_CBV,
	ROOTSIGNATURE_DATA_UAV_UAV_UAV_CBV_CBV,
	ROOTSIGNATURE_DATA_UAV_UAV_UAV_UAV_CBV,
	ROOTSIGNATURE_DATA_UAV_UAV_CBV_SRV,
	ROOTSIGNATURE_DATA_CBV_CBV_CBV_SRV_SRV_SRV,
	ROOTSIGNATURE_DATA_DRAW_UAB_CB,
	ROOTSIGNATURE_DATA_UAV_CB,
	ROOTSIGNATURE_DATA_UAV_UAV,
	ROOTSIGNATURE_DATA_UAB_UAB_UAB_CB,
	ROOTSIGNATURE_DATA_DRAW_UAB_TEX,
	ROOTSIGNATURE_DATA_DRAW_TEX_DATA1_DATA2,
	ROOTSIGNATURE_DATA_DRAW_TEX_TEX2_DATA1_DATA2,
	ROOTSIGNATURE_MODE_MULTIPASS,
	ROOTSIGNATURE_MODE_LIGHT,
	ROOTSIGNATURE_MODE_LIGHT_BORDER,
	ROOTSIGNATURE_DATA_DRAW_TEX4,
	ROOTSIGNATURE_DATA_DRAW_TEX_SKINING,
	ROOTSIGNATURE_DATA_DRAW_TEX_SKINING_DATA3,
	ROOTSIGNATURE_DATA_DRAW_DATA1,
	ROOTSIGNATURE_DATA_DRAW_TEX_SKINING_DATA1,
	ROOTSIGNATURE_DATA_DRAW_DATA1_TEX_DATA2,
	ROOTSIGNATURE_MODE_LINE,
	ROOTSIGNATURE_DATA_DATA1_TEX,
	ROOTSIGNATURE_DATA_DATA1_DATA2_DATA3_TEX,
	ROOTSIGNATURE_DATA_DATA1,
	ROOTSIGNATURE_DATA_DATA1_DATA2,
	ROOTSIGNATURE_DATA_DRAW_DATA1_DATA2,
	ROOTSIGNATURE_DATA_DRAW_SKINING_DATA1,
	ROOTSIGNATURE_DATA_SRV_UAV,
	ROOTSIGNATURE_DATA_CBV,
};


/// <summary>
/// SetSRV��SetCBV�̈����ɒl��n�����ۂ�
///�R�}���h���X�g��SetGraphicsRootConstantBufferView()�ɒl��n���̂�
///SetGraphicsDescriptorTable()�ɒl��n���̂����f����ۂɕK�v
/// </summary>
enum GraphicsRootSignatureType
{
	GRAPHICS_ROOTSIGNATURE_TYPE_NONE = -1,
	GRAPHICS_ROOTSIGNATURE_TYPE_DESCRIPTORTABLE,
	GRAPHICS_ROOTSIGNATURE_TYPE_VIEW
};

/// <summary>
/// �ǂ���������ނ̃f�X�N���v�^�����W�Ȃ̂����f����ۂɕK�v
/// </summary>
enum GraphicsRangeType
{
	GRAPHICS_RANGE_TYPE_NONE = -1,
	GRAPHICS_RANGE_TYPE_SRV_DESC,
	GRAPHICS_RANGE_TYPE_SRV_VIEW,
	GRAPHICS_RANGE_TYPE_UAV_DESC,
	GRAPHICS_RANGE_TYPE_UAV_VIEW,
	GRAPHICS_RANGE_TYPE_CBV_DESC,
	GRAPHICS_RANGE_TYPE_CBV_VIEW,
	GRAPHICS_RANGE_TYPE_SAMPLER
};

/// <summary>
/// ���̃��[�g�p�����͂ǂ������������������Ă���̂�
/// </summary>
enum GraphicsRootParamType
{
	GRAPHICS_PRAMTYPE_NONE = -1,
	//�`��n�̂ݎg�p
	GRAPHICS_PRAMTYPE_DRAW,
	//�e�N�X�`���̂ݎg�p
	GRAPHICS_PRAMTYPE_TEX,
	GRAPHICS_PRAMTYPE_TEX2,
	GRAPHICS_PRAMTYPE_TEX3,
	GRAPHICS_PRAMTYPE_TEX4,
	GRAPHICS_PRAMTYPE_TEX5,
	GRAPHICS_PRAMTYPE_TEX6,
	GRAPHICS_PRAMTYPE_TEX7,
	//�}�e���A���f�[�^�̂ݎg�p
	GRAPHICS_PRAMTYPE_MATERIAL,
	//���̑�(���C�g�f�[�^�Ȃǎg�p)
	GRAPHICS_PRAMTYPE_DATA,
	GRAPHICS_PRAMTYPE_DATA2,
	GRAPHICS_PRAMTYPE_DATA3,
	GRAPHICS_PRAMTYPE_DATA4,
	GRAPHICS_PRAMTYPE_DATA5,
	GRAPHICS_PRAMTYPE_DATA6,
	//�X�L�j���O���̂ݎg�p
	GRAPHICS_PRAMTYPE_SKINING
};

/// <summary>
/// ���[�g�p�����[�^�[�̔ԍ��ƃ��[�g�p�����[�^�[�̖������w�肵�܂�
/// </summary>
struct RootParamData
{
	short param;
	GraphicsRootParamType type;
};

struct BufferRootsignature
{
	GraphicsRangeType rangeType;
	GraphicsRootParamType dataType;
	BufferRootsignature(GraphicsRangeType RANGE, GraphicsRootParamType DATA) :rangeType(RANGE), dataType(DATA)
	{

	}
};

struct RootSignatureDataTest
{
	std::vector<BufferRootsignature> rangeArray;
	std::vector<D3D12_STATIC_SAMPLER_DESC> samplerArray;

	RootSignatureDataTest()
	{
		samplerArray.push_back({});

		samplerArray[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerArray[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerArray[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerArray[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplerArray[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		samplerArray[0].MaxLOD = D3D12_FLOAT32_MAX;
		samplerArray[0].MinLOD = 0.0f;
		samplerArray[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplerArray[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		samplerArray[0].MaxAnisotropy = 16;
		samplerArray[0].MipLODBias = 0;
		samplerArray[0].ShaderRegister = 0;
		samplerArray[0].RegisterSpace = 0;
	}
};


/// <summary>
/// SetSRV�A��������SetCBV�ɂāA ���[�g�p�����̔ԍ��ƃf�B�X�N���v�^�q�[�v�Ƃ��Đς܂��邩�A�r���[�Ƃ��Đς܂��邩
/// �𔻒f����ۂɕK�v�ɂȂ�\���̂ł�
/// </summary>
struct GraphicsRootSignatureParameter
{
	GraphicsRangeType range[10];			//���
	RootParamData paramData[10];			//�p�����[�^�[
	GraphicsRootSignatureType type[10];		//�ǂ̃Z�b�g���邩
	short paramMax;		    				//�g�p�����p�����[�^�[�̐�

	//������
	GraphicsRootSignatureParameter()
	{
		for (int i = 0; i < 10; i++)
		{
			range[i] = GRAPHICS_RANGE_TYPE_NONE;
			paramData[i].param = -1;
			paramData[i].type = GRAPHICS_PRAMTYPE_NONE;
			type[i] = GRAPHICS_ROOTSIGNATURE_TYPE_NONE;
		}
		paramMax = -1;
	}
};


struct RootSignatureParameter
{
	GraphicsRangeType range;			//���
	RootParamData paramData;			//�p�����[�^�[
	GraphicsRootSignatureType type;		//�ǂ̃Z�b�g���邩

	//������
	RootSignatureParameter()
	{
		range = GRAPHICS_RANGE_TYPE_NONE;
		paramData.param = -1;
		paramData.type = GRAPHICS_PRAMTYPE_NONE;
		type = GRAPHICS_ROOTSIGNATURE_TYPE_NONE;
	}
};



/// <summary>
/// ���[�g�V�O�l�`���𐶐�����ۂɕK�v�ɂȂ�f�[�^�ł�
/// </summary>
struct RootSignatureData
{
	GraphicsRangeType range[10];
	RootParamData paramData[10];
	D3D12_STATIC_SAMPLER_DESC sample;

	RootSignatureData()
	{
		sample.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sample.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sample.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sample.MaxLOD = D3D12_FLOAT32_MAX;
		sample.MinLOD = 0.0f;
		sample.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sample.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		sample.MaxAnisotropy = 16;
		sample.MipLODBias = 0;
		sample.ShaderRegister = 0;
		sample.RegisterSpace = 0;
	}
};


/// <summary>
/// ���[�g�V�O�l�`���[�𐶐��A�Ǘ����܂�
/// </summary>
class GraphicsRootSignature :public ISingleton<GraphicsRootSignature>
{
public:
	GraphicsRootSignature();
	~GraphicsRootSignature();

	/// <summary>
	/// ���[�g�V�O�l�`���𐶐����܂�
	/// </summary>
	/// <param name="ROOTSIGNATURE_DATA">���[�g�V�O�l�`���𐶐�����ۂɕK�v�ɂȂ�f�[�^</param>
	void CreateRootSignature(RootSignatureMode ROOTSIGNATURE, RootSignatureData ROOTSIGNATURE_DATA, short DATA_MAX);

	/// <summary>
	/// �w��̃��[�g�V�O�l�`�����R�}���h���X�g�ɐς݂܂�
	/// </summary>
	/// <param name="ROOTSIGNATURE_MODE">�ǂ̃��[�g�V�O�l�`����ݒ肷�邩</param>
	void SetRootSignature(RootSignatureMode ROOTSIGNATURE_MODE);

	/// <summary>
	/// �w��̃��[�g�V�O�l�`�����R�}���h���X�g�ɐς݂܂�
	/// </summary>
	/// <param name="ROOTSIGNATURE_MODE">�ǂ̃��[�g�V�O�l�`����ݒ肷�邩</param>
	void SetComputeRootSignature(RootSignatureMode ROOTSIGNATURE_MODE);


	/// <summary>
	/// �w��̃��[�g�V�O�l�`���[��Ԃ�l�œn���܂�
	/// </summary>
	/// <param name="ROOT_SIGNATURE">�w��̃��[�g�V�O�l�`���[</param>
	/// <returns>�������ꂽ���[�g�V�O�l�`���[</returns>
	const Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature(RootSignatureMode ROOT_SIGNATURE);

	/// <summary>
	/// �w��̃��[�g�V�O�l�`���[���ɐݒ肳�ꂽ���[�g�p�����[�^�[��n���܂�
	/// </summary>
	/// <param name="ROOTSIGNATURE_MODE">�w��̃��[�g�V�O�l�`���[</param>
	/// <returns>���[�g�p�����[�^�[</returns>
	const GraphicsRootSignatureParameter &GetRootParam(RootSignatureMode ROOTSIGNATURE_MODE);

	const std::vector<RootSignatureParameter> &GetRootParam(int ROOTSIGNATURE_HANDLE);


	Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature(const RootSignatureDataTest &ROOTSIGNATURE_DATA, RootsignatureType TYPE);


private:

	std::vector<Microsoft::WRL::ComPtr<ID3D12RootSignature>> rootSignature;
	std::vector<GraphicsRootSignatureParameter> paramD;

	std::vector<std::vector<RootSignatureParameter>> computeParamArrayData;

	friend ISingleton<GraphicsRootSignature>;


	void CreateMyRootSignature(D3D12_STATIC_SAMPLER_DESC SAMPLER_DATA, D3D12_ROOT_PARAMETER *ROOT_PARAM_DATA, size_t DATA_MAX, RootSignatureMode ROOTSIGNATURE);
	void CreateMyRootSignature(D3D12_STATIC_SAMPLER_DESC SAMPLER_DATA, D3D12_ROOT_PARAMETER1 *ROOT_PARAM_DATA, size_t DATA_MAX, RootSignatureMode ROOTSIGNATURE);

	void CreateMyRootSignature(std::vector<D3D12_STATIC_SAMPLER_DESC> SAMPLER_DATA, D3D12_ROOT_PARAMETER *ROOT_PARAM_DATA, size_t DATA_MAX, RootSignatureMode ROOTSIGNATURE);

	RootSignatureMode CheckEnum(RootSignatureMode MODE)
	{
		RootSignatureMode tmp = MODE;
		return tmp;
	};

	struct ParamData
	{
		UINT cbv, srv, uav;
		ParamData() :cbv(0), srv(0), uav(0), prevCbv(cbv), prevSrv(srv), prevUav(uav)
		{
		};

		UINT DirtyNum()
		{
			if (CheckNum(cbv, &prevCbv))
			{
				return cbv;
			}
			if (CheckNum(srv, &prevSrv))
			{
				return srv;
			}
			if (CheckNum(uav, &prevUav))
			{
				return uav;
			}

			//�l���ς��Ȃ��ƌ������͉�����������Ă���ׁA�G���[��f������
			assert(0);
			return 0;
		}

		UINT TotalNum()
		{
			return cbv + srv + uav;
		}

	private:
		UINT prevCbv, prevSrv, prevUav;
		bool CheckNum(UINT NUM, UINT *PREV_NUM)
		{
			bool dirtyFlag = NUM != *PREV_NUM;
			*PREV_NUM = NUM;
			return dirtyFlag;
		}
	};
};
