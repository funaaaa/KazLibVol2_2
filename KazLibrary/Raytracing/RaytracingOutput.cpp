#include "RaytracingOutput.h"
#include "../Raytracing/RayDescriptorHeap.h"
#include "../DirectXCommon/DirectX12CmdList.h"
#include "../DirectXCommon/DirectX12Device.h"

void RaytracingOutput::Setting(DXGI_FORMAT Format, LPCWSTR BufferName, KazMath::Vec2<int> TextureSize, D3D12_RESOURCE_STATES ResourceState)
{

	/*===== 設定 =====*/

	// UAVを設定
	rayTracingOutput_ = CreateTexture2D(
		static_cast<UINT>(TextureSize.x), static_cast<UINT>(TextureSize.y), Format,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		ResourceState,
		D3D12_HEAP_TYPE_DEFAULT
	);

	// ディスクリプタヒープにUAVを確保
	uavDescriptorIndex_ = RayDescriptorHeap::Instance()->AllocateBuffer(RayDescriptorHeap::BUFFER_TYPE::UAV_TEXTURE, rayTracingOutput_.Get());

	// 名前を設定
	rayTracingOutput_->SetName(BufferName);

}

void RaytracingOutput::SetComputeRootDescriptorTalbe(int RootParamIndex)
{

	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(RootParamIndex, RayDescriptorHeap::Instance()->GetGPUDescriptorHandle(uavDescriptorIndex_));

}

void RaytracingOutput::SetResourceBarrier(D3D12_RESOURCE_STATES Dst, D3D12_RESOURCE_STATES Src)
{

	/*===== リソースバリアを遷移 =====*/

	D3D12_RESOURCE_BARRIER barrierToUAV[] = { CD3DX12_RESOURCE_BARRIER::Transition(
			GetRaytracingOutput().Get(),
			Dst,
			Src)
	};

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, barrierToUAV);

}

Microsoft::WRL::ComPtr<ID3D12Resource> RaytracingOutput::CreateTexture2D(UINT Width, UINT Height, DXGI_FORMAT Format, D3D12_RESOURCE_FLAGS Flags, D3D12_RESOURCE_STATES InitialState, D3D12_HEAP_TYPE HeapType)
{
	D3D12_HEAP_PROPERTIES heapProps{};
	if (HeapType == D3D12_HEAP_TYPE_DEFAULT) {
		heapProps = D3D12_HEAP_PROPERTIES{
		D3D12_HEAP_TYPE_DEFAULT, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 1, 1
		};
	}
	if (HeapType == D3D12_HEAP_TYPE_UPLOAD) {
		heapProps = D3D12_HEAP_PROPERTIES{
		D3D12_HEAP_TYPE_UPLOAD, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 1, 1
		};
	}
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Alignment = 0;
	resDesc.Width = Width;
	resDesc.Height = Height;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = Format;
	resDesc.SampleDesc = { 1, 0 };
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.Flags = Flags;

	hr = DirectX12Device::Instance()->raytracingDevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		InitialState,
		nullptr,
		IID_PPV_ARGS(resource.ReleaseAndGetAddressOf())
	);
	if (FAILED(hr)) {
		OutputDebugStringA("CreateTexture2D failed.\n");
		assert(0);
	}
	return resource;
}
