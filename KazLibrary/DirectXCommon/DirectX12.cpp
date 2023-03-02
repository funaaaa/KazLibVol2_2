#include "DirectX12.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"

#include<d2d1.h>
#include<d3d11.h>
#include<d2d1.h>

#include"../DirectXTex/include/DirectXTex.h"
DirectX12::DirectX12()
{
	bbIndex = 0;
	fenceVal = 0;

	swapchain = nullptr;
	tmpAdapter = nullptr;
	fence = nullptr;
	cmdAllocator = nullptr;
}

DirectX12::~DirectX12()
{
}

HRESULT DirectX12::Create(int WINDOW_SIZE_X, int WINDOW_SIZE_Y, HWND hwnd)
{
	HRESULT checkResult = E_FAIL;


	winX = WINDOW_SIZE_X;
	winY = WINDOW_SIZE_Y;


	checkResult = CreateAdapter();
	if (checkResult != S_OK) {
		return checkResult;
	}


	//�V���O���g���Ő���
	DirectX12Device::Instance()->CreateDevice(tmpAdapter);
	if (checkResult != S_OK) {
		return checkResult;
	}


	//�V���O���g���Ő���
	checkResult = CreateCommand();
	if (checkResult != S_OK) {
		return checkResult;
	}


	checkResult = CreateSwapChain(WINDOW_SIZE_X, WINDOW_SIZE_Y, hwnd);
	if (checkResult != S_OK) {
		return checkResult;
	}


	checkResult = CreateRenderTarget();
	if (checkResult != S_OK) {
		return checkResult;
	}


	checkResult = CreateFence();
	if (checkResult != S_OK) {
		return checkResult;
	}


	//Direct2D��DirectWrite�̏�����

	//D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &factoryOptions, &factory);
	//Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	//m_d3d11Device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
	//factory.As(&m_d2dFactory);

	//m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice);
	//m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dDeviceContext);

	//Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	//m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	//backBuffer.As(&m_dxgiSurface);

	//// �`��� Bitmap �擾.
	//D2D1_BITMAP_PROPERTIES1 bitmapProps = D2D1::BitmapProperties1(
	//	D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
	//	D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
	//	float(dpi), float(dpi)
	//);
	//m_d2dDeviceContext->CreateBitmapFromDxgiSurface(m_dxgiSurface.Get(), &bitmapProps, &m_d2dBitmap);

	return checkResult;
}

void DirectX12::ActCommand()
{
	//���߂̃N���[�Y
	DirectX12CmdList::Instance()->cmdList->Close();

	//�R�}���h���X�g�̎��s
	ID3D12CommandList *cmdLists[] = { DirectX12CmdList::Instance()->cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	//�o�b�t�@���t���b�v
	swapchain->Present(1, 0);

	//�R�}���h���X�g�̎��s������҂�
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//�L���[���N���A
	cmdAllocator->Reset();
	//�Ăђ��߂�
	DirectX12CmdList::Instance()->cmdList->Reset(cmdAllocator.Get(), nullptr);

}

HRESULT DirectX12::CreateAdapter() {
	//DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	//�A�_�v�^�[�̗񋓗p
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter>> adapters;

	//����̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	for (int i = 0; dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
		adapters.push_back(tmpAdapter.Get());
	}

	for (int i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC adesc{};

		//�A�_�v�^�[�̏����擾
		adapters[i]->GetDesc(&adesc);
		//�A�_�v�^�[��
		std::wstring strDesc = adesc.Description;


		//Microsoft Basic Render Driver ,Intel UHD Graphics������������̂��̗p
		if (strDesc.find(L"Microsoft") == std::wstring::npos && strDesc.find(L"Intel") == std::wstring::npos) {
			tmpAdapter = adapters[i];
			break;
		}
	}


	return result;
}

HRESULT DirectX12::CreateCommand()
{
	result = DirectX12Device::Instance()->dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));
	if (result == S_OK) {
		result = DirectX12Device::Instance()->dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&DirectX12CmdList::Instance()->cmdList));
		if (result == S_OK) {
			//�R�}���h�L���[�̐���
			D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
			result = DirectX12Device::Instance()->dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
		}
	}

	return result;
}

HRESULT DirectX12::CreateSwapChain(int WINDOW_SIZE_X, int WINDOW_SIZE_Y, HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};

	swapchainDesc.Width = WINDOW_SIZE_X;
	swapchainDesc.Height = WINDOW_SIZE_Y;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1;

	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1
	);

	swapchain1.As(&swapchain);

	return result;
}

HRESULT DirectX12::CreateRenderTarget()
{
	//�����_�[�^�[�Q�b�g�r���[
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//���\�̓��
	heapDesc.NumDescriptors = 2;
	//DirectX12Device::dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	DirectX12Device::Instance()->dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	//���\�̓��
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffer(2);

	for (int i = 0; i < 2; i++) {

		//�X���b�v�`�F�[������o�b�t�@���擾
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffer[i]));
		backBuffer[i]->SetName(L"DirectX");
		//�f�B�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		backBuffers.push_back(backBuffer[i]);

		//�����\���ŃA�h���X�������
		handle.ptr += i * DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(heapDesc.Type);

		//�����_�[�^�[�Q�b�g�r���[�̐���
		DirectX12Device::Instance()->dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle
		);
	}

	return result;
}

HRESULT DirectX12::CreateFence()
{
	result = DirectX12Device::Instance()->dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	return result;
}
