#include "RayDescriptorHeap.h"
#include "../DirectXCommon/DirectX12Device.h"

void RayDescriptorHeap::Setting()
{

	/*===== 事前準備処理 =====*/

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 64;

	// ディスクリプタヒープの生成。
	DirectX12Device::Instance()->raytracingDevice->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&heap_));
	heap_->SetName(L"RaytracingDescriptorHeap");

	head_ = 0;

}

RAY_HEAP_HANDLE RayDescriptorHeap::AllocateBuffer(BUFFER_TYPE Type, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer)
{

	/*===== バッファを生成する =====*/

	// 先頭ハンドルを指定してバッファを生成する。
	AllocateBufferForHandle(head_, Type, Buffer);

	// バッファの先頭を更新。
	++head_;

	return head_;

}

void RayDescriptorHeap::AllocateBufferForHandle(RAY_HEAP_HANDLE Handle, BUFFER_TYPE Type, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer)
{

	/*===== ハンドルを指定してバッファを生成する =====*/

	switch (Type)
	{
	case RayDescriptorHeap::BUFFER_TYPE::AS:

		/*-- 加速構造体だったら --*/

		// 先頭にバッファを生成する。
		AllocateASBuffer(head_, Buffer);

		break;
	case RayDescriptorHeap::BUFFER_TYPE::UAV_TEXTURE:

		/*-- 加速構造体だったら --*/

		// 先頭にバッファを生成する。
		AllocateUAVTextureBuffer(head_, Buffer);

		break;
	default:
		break;
	}

}

D3D12_GPU_DESCRIPTOR_HANDLE RayDescriptorHeap::GetGPUDescriptorHandle(RAY_HEAP_HANDLE Handle)
{

	/*===== 指定のハンドルのGPUハンドルを取得する =====*/

	return CD3DX12_GPU_DESCRIPTOR_HANDLE(heap_.Get()->GetGPUDescriptorHandleForHeapStart(), Handle, DirectX12Device::Instance()->raytracingDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	
}

void RayDescriptorHeap::AllocateASBuffer(RAY_HEAP_HANDLE Handle, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer)
{

	/*===== 加速構造体のバッファを生成する =====*/

	// ディスクリプタヒープに加速構造体用のSRVを確保
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = Buffer->GetGPUVirtualAddress();
	CD3DX12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		heap_.Get()->GetCPUDescriptorHandleForHeapStart(), Handle, DirectX12Device::Instance()->raytracingDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	DirectX12Device::Instance()->raytracingDevice->CreateShaderResourceView(nullptr, &srvDesc,
		basicHeapHandle);

}

void RayDescriptorHeap::AllocateUAVTextureBuffer(RAY_HEAP_HANDLE Handle, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer)
{

	/*===== UAVテクスチャ用のバッファを生成する =====*/

	// 先頭ハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		heap_.Get()->GetCPUDescriptorHandleForHeapStart(), Handle, DirectX12Device::Instance()->raytracingDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	// ディスクリプタヒープにUAVを確保
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	DirectX12Device::Instance()->raytracingDevice->CreateUnorderedAccessView(
		Buffer.Get(), nullptr, &uavDesc, basicHeapHandle);

}
