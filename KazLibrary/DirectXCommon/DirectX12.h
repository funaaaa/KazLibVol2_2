#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/OutPutDebugStringAndCheckResult.h"
#include"../Helper/ISinglton.h"
#include<vector>

/// <summary>
/// DirectX�̏��������s���܂�
/// </summary>
class DirectX12 :public ISingleton<DirectX12>
{
public:
	DirectX12();
	~DirectX12();

	/// <summary>
	/// DirectX12��
	/// </summary>
	/// <param name="WINDOW_SIZE_X">�E�B���h�E��X�T�C�Y</param>
	/// <param name="WINDOW_SIZE_Y">�E�B���h�E��Y�T�C�Y</param>
	/// <param name="hwnd">�E�B���h�E�n���h��</param>
	/// <returns>S_OK...����,����ȊO...���s</returns>
	HRESULT Create(int WINDOW_SIZE_X, int WINDOW_SIZE_Y, HWND hwnd);

	/// <summary>
	/// �R�}���h���X�g�̎��s
	/// </summary>
	void ActCommand();


	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain;

public:

	int winX, winY;



	HRESULT result;

	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;
	
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps;


	Microsoft::WRL::ComPtr<IDXGIAdapter> tmpAdapter;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	UINT bbIndex;

	D3D12_RESOURCE_BARRIER barrierDesc{};

	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;



	//���񐶐��������ɏo���Ă邩�m�F�ł���悤�ɕԂ�l�Ŋm�F�ł���悤�ɂ���

	HRESULT CreateAdapter();
	HRESULT CreateCommand();
	HRESULT CreateSwapChain(int WINDOW_SIZE_X, int WINDOW_SIZE_Y, HWND hwnd);
	HRESULT CreateRenderTarget();
	HRESULT CreateFence();

};

