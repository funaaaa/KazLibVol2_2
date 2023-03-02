#pragma once
#include"../DirectXCommon/Base.h"
#include<DirectXTex.h>
#include<string>
#include"../Pipeline/Shader.h"
#include<vector>
#include"../Helper/ISinglton.h"
#include"../Pipeline/GraphicsRootSignature.h"
#include"../Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Math/KazMath.h"

/// <summary>
/// �摜�̕���������ۂɂǂ̍��W����ǂꂭ�炢�̃T�C�Y��UV�؂�������邩�L�^��������
/// </summary>
struct DivGraphData
{
	std::vector<KazMath::Vec2<int>> divLeftUp;
	KazMath::Vec2<int> divSize;
	RESOURCE_HANDLE handle;
};

/// <summary>
/// �摜�̓ǂݍ��݁A�摜�̊Ǘ�
/// </summary>
class TextureResourceMgr :public ISingleton<TextureResourceMgr> {
public:

	TextureResourceMgr();
	~TextureResourceMgr();

	/// <summary>
	/// �t�@�C���p�X�������ĉ摜�̓ǂݍ��݂��s���܂�
	/// </summary>
	/// <param name="RESOURCE">�ǂݍ��݂����摜�̃t�@�C���p�X</param>
	/// <returns>�n���h��</returns>
	RESOURCE_HANDLE LoadGraph(std::string RESOURCE);
	
	/// <summary>
	///  �t�@�C���p�X�������ĉ摜��ǂݍ��݁A�������܂�
	/// </summary>
	/// <param name="RESOURCE">�ǂݍ��݂����摜�̃t�@�C���p�X</param>
	/// <param name="DIV_WIDTH_NUM">X���ɉ摜�𕪊����鐔</param>
	/// <param name="DIV_HEIGHT_NUM">Y���ɉ摜�𕪊����鐔</param>
	/// <param name="TEXSIZE_WIDTH">��������摜��X�T�C�Y</param>
	/// <param name="TEXSIZE_HEIGHT">��������摜��Y�T�C�Y</param>
	/// <returns>�n���h��</returns>
	RESOURCE_HANDLE LoadDivGraph(string RESOURCE,
		int DIV_WIDTH_NUM,
		int DIV_HEIGHT_NUM,
		int TEXSIZE_WIDTH,
		int TEXSIZE_HEIGHT
	);

	/// <summary>
	/// �ǂݍ��񂾉摜���폜���܂�
	/// </summary>
	/// <param name="HANDLE">�폜�������摜�̃n���h��</param>
	void Release(RESOURCE_HANDLE HANDLE);

	/// <summary>
	/// �n���h������V�F�[�_�[���\�[�X���R�}���h���X�g�ɐς܂��܂�
	/// </summary>
	/// <param name="GRAPH_HANDLE">�n���h��</param>
	/// <param name="PARAM">���[�g�p����</param>
	/// <param name="TYPE">���[�g�p�����̎��</param>
	void SetSRV(RESOURCE_HANDLE GRAPH_HANDLE, GraphicsRootSignatureParameter PARAM, GraphicsRootParamType TYPE);


	/// <summary>
	/// �摜�̏���n���܂�
	/// �o�b�t�@�̎擾�Ɏ��s�����ꍇ Width��Height��0���Ԃ���܂�
	/// </summary>
	/// <param name="HANDLE"></param>
	/// <returns></returns>
	D3D12_RESOURCE_DESC GetTextureSize(RESOURCE_HANDLE HANDLE);

	/// <summary>
	/// ��������摜�̏���n���܂�
	/// �o�b�t�@�̎擾�Ɏ��s�����ꍇ texSize��X��Y��-1������܂�
	/// </summary>
	/// <param name="HANDLE">�n���h��</param>
	/// <returns>��������摜�̏��</returns>
	DivGraphData GetDivData(RESOURCE_HANDLE HANDLE);

	std::unique_ptr<CreateGpuBuffer> buffers;
private:
	//std::unique_ptr<CreateGpuBuffer> buffers;
	const DirectX::Image *img;

	D3D12_STATIC_SAMPLER_DESC samplerDesc;

	static const int texWidth = 256;
	static const int texDataCount = texWidth * texWidth;
	const int DescriptorMaxNum = 5000;

	DirectX::ScratchImage scratchImg;
	UINT handle;
	UINT setHandle;
	UINT IncreSize;
	DirectX::TexMetadata metadata;

	HRESULT result;

	std::vector<std::string> handleName;

	vector<DivGraphData> divData;
	friend ISingleton<TextureResourceMgr>;
};