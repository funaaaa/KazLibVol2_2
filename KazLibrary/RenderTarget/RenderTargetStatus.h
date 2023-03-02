#pragma once
#include"../DirectXCommon/Base.h"
#include"../Pipeline/GraphicsDepthTest.h"
#include"../Helper/ISinglton.h"
#include"../Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Math/KazMath.h"

struct MultiRenderTargetData
{
	DirectX::XMFLOAT3 backGroundColor;
	KazMath::Vec2<UINT> graphSize;
	MultiRenderTargetData()
	{};
};

/// <summary>
/// �����_�[�^�[�Q�b�g�̐؂�ւ�
/// �_�u���o�b�t�@�����O�̏��������C���ł���
/// RenderTarget�N���X�ɂĐ��������ϐ����g�p���āAClearRenderTargetView�����胊�\�[�X�̐؂�ւ����\�ł�
/// </summary>
class RenderTargetStatus :public ISingleton<RenderTargetStatus>
{
public:
	RenderTargetStatus();
	~RenderTargetStatus();

	/// <summary>
	/// �_�u���o�b�t�@�����O�p�̃o�b�t�@�쐬
	/// </summary>
	/// <param name="SWAPCHAIN">�X���b�v�`�F�[��</param>
	void CreateDoubleBuffer(Microsoft::WRL::ComPtr<IDXGISwapChain4> SWAPCHAIN);

	/// <summary>
	/// �_�u���o�b�t�@�����O�p�̃����_�[�^�[�Q�b�g���N���A���܂�
	/// </summary>
	void SetDoubleBufferFlame();


	void ClearDoubuleBuffer(DirectX::XMFLOAT3 COLOR);

	/// <summary>
	/// �_�u���o�b�t�@�����O�p�̃o���A�؂�ւ�
	/// </summary>
	void SwapResourceBarrier();

	/// <summary>
	/// �����_�[�^�Q�b�g�̃��\�[�X�o���A���J���܂�
	/// CLOSE_RENDERTARGET��nullptr�̏ꍇ�A�_�u���o�b�t�@�����O�p�̃t���[������܂�
	/// </summary>
	/// <param name="OPEN_RENDERTARGET">�J�����������_�[�^�[�Q�b�g�̃��\�[�X�o���A</param>
	/// <param name="CLOSE_RENDERTARGET">�����������_�[�^�[�Q�b�g�̃��\�[�X�o���A</param>
	void PrepareToChangeBarrier(RESOURCE_HANDLE OPEN_RENDERTARGET_HANDLE, RESOURCE_HANDLE CLOSE_RENDERTARGET_HANDLE = -1);

	/// <summary>
	/// �w��̃����_�[�^�[�Q�b�g�����\�[�X�o���A����܂�
	/// </summary>
	/// <param name="RENDERTARGET">�����������_�[�^�[�Q�b�g</param>
	void PrepareToCloseBarrier(RESOURCE_HANDLE RENDERTARGET_HANDLE);

	/// <summary>
	/// �����_�[�^�[�Q�b�g���N���A���܂�
	/// </summary>
	/// <param name="RENDERTARGET">�N���A���郌���_�[�^�[�Q�b�g</param>
	void ClearRenderTarget(RESOURCE_HANDLE RENDERTARGET_HANDLE);


	RESOURCE_HANDLE CreateRenderTarget(const KazMath::Vec2<UINT> &GRAPH_SIZE, const DirectX::XMFLOAT3 &CLEAR_COLOR, const DXGI_FORMAT &FORMAT);
	std::vector<RESOURCE_HANDLE> CreateMultiRenderTarget(const std::vector<MultiRenderTargetData> &MULTIRENDER_TARGET_DATA, const DXGI_FORMAT &FORMAT);
	ID3D12Resource *GetBufferData(RESOURCE_HANDLE HANDLE)const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetViewData(RESOURCE_HANDLE HANDLE);


	void DeleteRenderTarget(RESOURCE_HANDLE HANDLE);
	void DeleteMultiRenderTarget(const std::vector<RESOURCE_HANDLE> &HANDLE);


	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> multiPassRTVHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource> copyBuffer;

	GraphicsDepthTest gDepth;
	RESOURCE_HANDLE handle, handle2;

	std::unique_ptr<CreateGpuBuffer> buffers;
	int bbIndex;

	static const int RENDERTARGET_MAX_NUM = 1000;

	//�����_�[�^�[�Q�b�g�̃n���h������p�X�����L�^�����z��
	std::array<std::vector<RESOURCE_HANDLE>, RENDERTARGET_MAX_NUM>renderTargetData;

	void ChangeBarrier(ID3D12Resource *RESOURCE, const D3D12_RESOURCE_STATES &BEFORE_STATE, const D3D12_RESOURCE_STATES &AFTER_STATE);

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	Microsoft::WRL::ComPtr<IDXGISwapChain4> copySwapchain;

	static const int SWAPCHAIN_MAX_NUM = 2;
private:


	std::array<DirectX::XMFLOAT4, RENDERTARGET_MAX_NUM> clearColors;



	
	D3D12_CPU_DESCRIPTOR_HANDLE multiPassRTVHanlde;




	struct RenderTargetHandle
	{
		unsigned int renderTargetNum;
		std::vector<RESOURCE_HANDLE> handles;
		RenderTargetHandle(unsigned int RENDER_TARGET_NUM, std::vector<RESOURCE_HANDLE> FIRST_HANDLE) :renderTargetNum(RENDER_TARGET_NUM), handles(FIRST_HANDLE)
		{
		};
	};
	std::vector<RESOURCE_HANDLE> CountPass(RESOURCE_HANDLE HANDLE);
};
