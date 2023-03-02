#include "Blas.h"
#include "../Loader/FbxModelResourceMgr.h"
#include "../Render/RenderData.h"
#include "../DirectXCommon/DirectX12Device.h"
#include "../DirectXCommon/DirectX12CmdList.h"
#include <memory>

Blas::Blas(bool IsOpaque, const FbxModelData& ModelDataHandle)
{

	/*===== コンストラクタ =====*/

	// Blasの構築に必要な形状データを取得。
	D3D12_RAYTRACING_GEOMETRY_DESC geomDesc = GetGeometryDesc(IsOpaque, ModelDataHandle);

	// Blasを構築。
	BuildBlas(geomDesc);

	// レイトレーシングを有効化
	isRaytracingEnabled = true;

}

uint8_t* Blas::WriteShaderRecord(uint8_t* Dst, UINT RecordSize, Microsoft::WRL::ComPtr<ID3D12StateObject>& StateObject, LPCWSTR HitGroupName)
{

	/*===== シェーダーレコードを書き込む =====*/

	// 使用する頂点データとかが決まったらここを書き込む。
	assert(0);

	//Microsoft::WRL::ComPtr<ID3D12StateObjectProperties> rtsoProps_;
	//StateObject.As(&rtsoProps_);
	//auto entryBegin = Dst;

	//auto mode_ = rtsoProps_->GetShaderIdentifier(HitGroupName);
	//if (mode_ == nullptr) {
	//	throw std::logic_error("Not found ShaderIdentifier");
	//}

	//// シェーダー識別子を書き込む。
	//memcpy(Dst, mode_, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	//Dst += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;


	//// 頂点関係のデータが変更されていたら。
	//if (hitGroupInfo_.isChangeVertex_[Index]) {
	//	// 今回のプログラムでは以下の順序でディスクリプタを記録。
	//	// [0] : インデックスバッファ
	//	// [1] : 頂点バッファ
	//	// ※ ローカルルートシグネチャの順序に合わせる必要がある。
	//	Dst += WriteGPUDescriptor(Dst, &blasInfo_.indexDescriptor_->GetDescriptor(Index).lock()->GetGPUHandle());
	//	Dst += WriteGPUDescriptor(Dst, &blasInfo_.vertexDescriptor_->GetDescriptor(Index).lock()->GetGPUHandle());

	//	// マテリアル用のバッファをセット。
	//	Dst += WriteGPUDescriptor(Dst, &blasInfo_.materialDescriptor_->GetDescriptor(Index).lock()->GetGPUHandle());

	//	hitGroupInfo_.isChangeVertex_[Index] = false;

	//}
	//else {

	//	Dst += static_cast<UINT>((sizeof(D3D12_GPU_DESCRIPTOR_HANDLE*)));
	//	Dst += static_cast<UINT>((sizeof(D3D12_GPU_DESCRIPTOR_HANDLE*)));
	//	Dst += static_cast<UINT>((sizeof(D3D12_GPU_DESCRIPTOR_HANDLE*)));

	//}

	//// ヒットグループ名からヒットグループ名IDを取得する。
	//int hitGroupID = HitGroupMgr::Ins()->GetHitGroupID(HitGroupName);

	//// 頂点、インデックス、マテリアルのオフセット
	//const int OFFSET_VERTEX_INDEX_MATERIAL = 3;

	//// ヒットグループIDからSRVの数を取得。
	//int srvCount = HitGroupMgr::Ins()->GetHitGroupSRVCount(hitGroupID) - OFFSET_VERTEX_INDEX_MATERIAL;

	//// テクスチャ関係が変更されていたら。
	//if (hitGroupInfo_.isChangeTexture_[Index]) {

	//	// ここはテクスチャのサイズではなく、パイプラインにセットされたSRVの数を持ってきてそれを使う。
	//	// この時点でSRVの数とテクスチャの数が合っていなかったらassertを出す。
	//	for (int index = 0; index < srvCount; ++index) {

	//		// 0番目は基本テクスチャ。
	//		if (index == 0) {

	//			// 基本のテクスチャが設定されていない。
	//			if (hitGroupInfo_.baseTextureHandle_ == -1) assert(0);

	//			CD3DX12_GPU_DESCRIPTOR_HANDLE texDescHandle = DescriptorHeapMgr::Ins()->GetGPUHandleIncrement(hitGroupInfo_.baseTextureHandle_);
	//			Dst += WriteGPUDescriptor(Dst, &texDescHandle);

	//		}
	//		// 1番目は法線マップテクスチャ。
	//		else if (index == 1) {

	//			if (hitGroupInfo_.mapTextureHandle_ != -1) {

	//				CD3DX12_GPU_DESCRIPTOR_HANDLE texDescHandle = DescriptorHeapMgr::Ins()->GetGPUHandleIncrement(hitGroupInfo_.mapTextureHandle_);
	//				Dst += WriteGPUDescriptor(Dst, &texDescHandle);

	//			}
	//			else {

	//				// 法線マップが設定されていなかったら、メモリの隙間を埋めるため通常のテクスチャを書き込む。
	//				CD3DX12_GPU_DESCRIPTOR_HANDLE texDescHandle = DescriptorHeapMgr::Ins()->GetGPUHandleIncrement(hitGroupInfo_.baseTextureHandle_);
	//				Dst += WriteGPUDescriptor(Dst, &texDescHandle);

	//			}

	//		}

	//	}

	//	hitGroupInfo_.isChangeTexture_[Index] = false;

	//}
	//else {

	//	Dst += static_cast<UINT>(sizeof(D3D12_GPU_DESCRIPTOR_HANDLE*)) * static_cast<UINT>(srvCount);

	//}

	//// 使用するUAVの数を取得。
	//int uavCount = HitGroupMgr::Ins()->GetHitGroupUAVCount(hitGroupID);
	//for (int index = 0; index < uavCount; ++index) {

	//	// テクスチャが存在していたら。
	//	if (0 < hitGroupInfo_.uavHandle_.size()) {

	//		CD3DX12_GPU_DESCRIPTOR_HANDLE texDescHandle = DescriptorHeapMgr::Ins()->GetGPUHandleIncrement(hitGroupInfo_.uavHandle_[index]);
	//		Dst += WriteGPUDescriptor(Dst, &texDescHandle);

	//	}
	//	else {

	//		Dst += sizeof(D3D12_GPU_DESCRIPTOR_HANDLE*);

	//	}

	//}

	//Dst = entryBegin + recordSize;
	//return Dst;

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
	triangles.VertexBuffer.StrideInBytes = static_cast<UINT64>(sizeof(Model::VertexSize));
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
