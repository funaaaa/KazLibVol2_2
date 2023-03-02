#include "DirectX12Device.h"

#pragma comment(lib, "d3d11.lib")

//HRESULT DirectX12Device::result = S_FALSE;
//Microsoft::WRL::ComPtr<ID3D12Device> DirectX12Device::dev;

DirectX12Device::DirectX12Device()
{
	result = S_FALSE;
}

DirectX12Device::~DirectX12Device()
{
	dev.ReleaseAndGetAddressOf();
}

void DirectX12Device::CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter> adapter) {

	//�Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++) {
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(adapter.Get(), levels[i], IID_PPV_ARGS(&dev));

		if (result == S_OK) {
			//�f�o�C�X�̐����ɐ��������甲����
			featureLevel = levels[i];
			break;
		}
	}

	dev->SetName(L"Dev12");

	Microsoft::WRL::ComPtr<ID3D11Device> deviceD3D11;
	D3D11CreateDevice(
		adapter.Get(),
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		D3D11_CREATE_DEVICE_VIDEO_SUPPORT | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		deviceD3D11.ReleaseAndGetAddressOf(),
		nullptr,
		nullptr);
	deviceD3D11.As(&dev11);

	Microsoft::WRL::ComPtr<ID3D10Multithread> multithread;
	deviceD3D11.As(&multithread);
	multithread->SetMultithreadProtected(TRUE);


	/*-- ���C�g���V�K�ǉ��@�\ --*/

	// ���C�g���p�f�o�C�X�𐶐��B
	D3D12CreateDevice(adapter.Get(), featureLevel, IID_PPV_ARGS(&raytracingDevice));
	dev->SetName(L"RaytracingDevice");

	// DXR���T�|�[�g���Ă��邩���m�F
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options{};
	result = raytracingDevice->CheckFeatureSupport(
		D3D12_FEATURE_D3D12_OPTIONS5, &options, sizeof(options));
	if (FAILED(result) ||
		options.RaytracingTier == D3D12_RAYTRACING_TIER_NOT_SUPPORTED)
	{
		// ���C�g�����Ή����Ă��܂���B
		assert(0);
	}


}
