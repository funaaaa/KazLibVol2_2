#include "Blas.h"
#include "../Loader/FbxModelResourceMgr.h"
#include "../Render/RenderData.h"
#include "../DirectXCommon/DirectX12Device.h"
#include "../DirectXCommon/DirectX12CmdList.h"
#include "../Raytracing/HitGroupMgr.h"
#include "../Buffer/DescriptorHeapMgr.h"
#include <memory>

Blas::Blas(bool IsOpaque, const FbxModelData& ModelDataHandle)
{

	/*===== コンストラクタ =====*/

	// Blasの構築に必要な形状データを取得。
	refModelData_ = &ModelDataHandle;
	D3D12_RAYTRACING_GEOMETRY_DESC geomDesc = GetGeometryDesc(IsOpaque, ModelDataHandle);

	// Blasを構築。
	BuildBlas(geomDesc);

}

uint8_t* Blas::WriteShaderRecord(uint8_t* Dst, UINT RecordSize, Microsoft::WRL::ComPtr<ID3D12StateObject>& StateObject, LPCWSTR HitGroupName)
{

	/*===== シェーダーレコードを書き込む =====*/

	Microsoft::WRL::ComPtr<ID3D12StateObjectProperties> rtsoProps_;
	StateObject.As(&rtsoProps_);
	auto entryBegin = Dst;

	auto mode_ = rtsoProps_->GetShaderIdentifier(HitGroupName);
	if (mode_ == nullptr) {
		throw std::logic_error("Not found ShaderIdentifier");
	}

	// シェーダー識別子を書き込む。
	memcpy(Dst, mode_, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	Dst += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;

	// 今回のプログラムでは以下の順序でディスクリプタを記録。
	// [0] : インデックスバッファ
	// [1] : 頂点バッファ
	// ※ ローカルルートシグネチャの順序に合わせる必要がある。
	Dst += WriteGPUDescriptor(Dst, &DescriptorHeapMgr::Instance()->GetGpuDescriptorView(FbxModelResourceMgr::Instance()->GetResourceData(refModelData_->handle.handle)->indexDescriptor));
	Dst += WriteGPUDescriptor(Dst, &DescriptorHeapMgr::Instance()->GetGpuDescriptorView(FbxModelResourceMgr::Instance()->GetResourceData(refModelData_->handle.handle)->vertexDescriptor));
	// テクスチャを書き込む。
	//Dst += WriteGPUDescriptor(Dst, &DescriptorHeapMgr::Instance()->GetGpuDescriptorView(TextureResourceMgr::Instance()->GetDivData(refModelData_->handle.handle).handle));
	Dst += static_cast<UINT>(sizeof(D3D12_GPU_DESCRIPTOR_HANDLE*));
	
	Dst = entryBegin + RecordSize;
	return Dst;

}

D3D12_RAYTRACING_GEOMETRY_DESC Blas::GetGeometryDesc(bool IsOpaque, const FbxModelData& ModelDataHandle)
{

	/*===== Blasの形状設定に必要な構造体を返す =====*/

	// 形状データのフラグを設定。
	auto geometryDesc = D3D12_RAYTRACING_GEOMETRY_DESC{};
	geometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
	// 不透明フラグを設定。 GPU側でAnyHitShaderを呼ぶかどうかをここで決定。
	if (IsOpaque) {
		geometryDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
	}
	else {
		geometryDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_NO_DUPLICATE_ANYHIT_INVOCATION;
	}

	// モデルの形状データを取得。
	std::weak_ptr<FbxResourceData> resourceData = FbxModelResourceMgr::Instance()->GetResourceData(ModelDataHandle.handle.handle);

	// 形状データの細かい項目を設定。
	auto& triangles = geometryDesc.Triangles;
	triangles.VertexBuffer.StartAddress = resourceData.lock()->buffers->GetGpuAddress(resourceData.lock()->vertBuffetHandle);
	triangles.VertexBuffer.StrideInBytes = static_cast<UINT64>(sizeof(Model::VertexPosNormalUvSkin));
	triangles.VertexCount = resourceData.lock()->vertexCount;
	triangles.IndexBuffer = resourceData.lock()->buffers->GetGpuAddress(resourceData.lock()->indexBufferHandle);
	triangles.IndexCount = resourceData.lock()->indexCount;
	triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
	triangles.IndexFormat = DXGI_FORMAT_R32_UINT;

	return geometryDesc;

}

void Blas::BuildBlas(const D3D12_RAYTRACING_GEOMETRY_DESC& GeomDesc)
{

	/*===== Blasを構築 =====*/

	// AS(レイとの判定を迅速に行うために必要なポリゴンデータ構造体)の設定に必要な各項目を設定。
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildASDesc{};
	auto& inputs = buildASDesc.Inputs;	// D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
	inputs.NumDescs = 1;
	inputs.pGeometryDescs = &GeomDesc;

	// 関数を使って必要なメモリ量を求める.
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO blasPrebuild{};
	DirectX12Device::Instance()->raytracingDevice->GetRaytracingAccelerationStructurePrebuildInfo(
		&inputs, &blasPrebuild
	);

	// スクラッチバッファを生成する。
	scratchBuffer_ = CreateBuffer(
		blasPrebuild.ScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_HEAP_TYPE_DEFAULT
	);
	D3D12_RESOURCE_BARRIER barrierToUAV[] = { CD3DX12_RESOURCE_BARRIER::Transition(scratchBuffer_.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_UNORDERED_ACCESS) };
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, barrierToUAV);
	scratchBuffer_->SetName(L"BlasScratchBuffer");

	// Blasのバッファを生成する。
	blasBuffer_ = CreateBuffer(
		blasPrebuild.ResultDataMaxSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		D3D12_HEAP_TYPE_DEFAULT
	);
	blasBuffer_->SetName(L"BlasBuffer");

	// 更新用バッファを生成する。
	updateBuffer_ = CreateBuffer(
		blasPrebuild.UpdateScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_HEAP_TYPE_DEFAULT
	);
	barrierToUAV[0] = { CD3DX12_RESOURCE_BARRIER::Transition(updateBuffer_.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_UNORDERED_ACCESS) };
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, barrierToUAV);
	updateBuffer_->SetName(L"BlasUpdateBuffer");

	// Blasを構築する。
	buildASDesc.ScratchAccelerationStructureData = scratchBuffer_->GetGPUVirtualAddress();
	buildASDesc.DestAccelerationStructureData = blasBuffer_->GetGPUVirtualAddress();

	// コマンドリストに積んで実行する。
	DirectX12CmdList::Instance()->cmdList->BuildRaytracingAccelerationStructure(
		&buildASDesc, 0, nullptr /* pPostBuildInfoDescs */
	);

	// リソースバリアの設定。
	D3D12_RESOURCE_BARRIER uavBarrier{};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = blasBuffer_.Get();
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &uavBarrier);

}

Microsoft::WRL::ComPtr<ID3D12Resource> Blas::CreateBuffer(size_t Size, D3D12_RESOURCE_FLAGS Flags, D3D12_RESOURCE_STATES InitialState, D3D12_HEAP_TYPE HeapType, const wchar_t* Name)
{

	/*===== バッファを生成 =====*/

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
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Alignment = 0;
	resDesc.Width = Size;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc = { 1, 0 };
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
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
		OutputDebugStringA("CreateBuffer failed.\n");
	}
	if (resource != nullptr && Name != nullptr) {
		resource->SetName(Name);
	}
	return resource;

}
