#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include<array>
#include<vector>
#include"../KazLibrary/Render/KazRender.h"

/// <summary>
/// ���b�V�������G�~�b�^�[�Ƃ���GPGPU�ł̃p�[�e�B�N���\��
/// </summary>
class MeshParticleEmitter
{
public:
	MeshParticleEmitter(std::vector<DirectX::XMFLOAT4> VERT_NUM, float PARTICLE_SCALE = 0.18f);
	~MeshParticleEmitter();
	void Init(const DirectX::XMMATRIX *MOTHER_MAT);
	void Update(float ALPHA = 1.0f);
	void Draw();

	D3D12_GPU_VIRTUAL_ADDRESS GetAddress()
	{
		return buffers->GetGpuAddress(outputHandle);
	}
	int GetVertNum()
	{
		return constBufferData.vertMaxNum;
	}
	bool resetSceneFlag;

private:
	int sceneNum;
	//�o�b�t�@
	std::unique_ptr<CreateGpuBuffer> buffers;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	//�o�b�t�@

	static const int PARTICLE_MAX_NUM = 100000;
	static const int DRAW_CALL = 1;

	struct IndirectCommand
	{
		D3D12_GPU_VIRTUAL_ADDRESS uav;
		D3D12_DRAW_INDEXED_ARGUMENTS drawArguments;
	};



	Microsoft::WRL::ComPtr<ID3D12CommandSignature> commandSig;
	bool drawParticleFlag;
	int bias, prevBias;
	float scale;

	//�K�v���--------------------------------------------
	KazMath::Vec3<float>*pos;
	const DirectX::XMMATRIX *motherMat;
	//�K�v���--------------------------------------------

	//������--------------------------------------------
	struct OutputData
	{
		DirectX::XMFLOAT4 pos;
	};
	struct InitCommonData
	{
		DirectX::XMFLOAT4 worldPos;
		UINT vertMaxNum;
		UINT bias;
	};
	InitCommonData constBufferData;
	RESOURCE_HANDLE vertexBufferHandle, indexBufferHandle;
	RESOURCE_HANDLE verticesDataHandle, initCommonHandle;

	RESOURCE_HANDLE outputHandle;

	RESOURCE_HANDLE outputViewHandle, vertDataViewHandle;
	//������--------------------------------------------

	//�X�V�p--------------------------------------------
	struct UpdateData
	{
		DirectX::XMMATRIX matrix;
		DirectX::XMFLOAT4 color;
	};
	struct UpdateCommonData
	{
		DirectX::XMMATRIX scaleRotateBillboardMat;
		DirectX::XMMATRIX viewProjection;
		DirectX::XMMATRIX motherMat;
		float alpha;
	};
	UpdateCommonData updateCommonData;
	RESOURCE_HANDLE updateHandle,updateCommonHandle;
	RESOURCE_HANDLE updateViewHandle;
	DirectX::XMMATRIX scaleRotaMat;


	UINT indexNum;

	//�X�V�p--------------------------------------------

	RESOURCE_HANDLE drawCommandHandle;
};
