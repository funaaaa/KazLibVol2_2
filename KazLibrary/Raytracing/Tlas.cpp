#include "Tlas.h"
#include "../DirectXCommon/DirectX12Device.h"
#include "../DirectXCommon/DirectX12CmdList.h"
#include "../Raytracing/InstanceVector.h"
#include "../Buffer/DescriptorHeapMgr.h"
#include "../Buffer/UavViewHandleMgr.h"

Tlas::Tlas()
{

	/*===== コンストラクタ =====*/

	instanceCapacity_ = -1;
	descHeapIndex_ = -1;

}

void Tlas::Build()
{

	/*===== Tlasを構築 =====*/

	// 現在のTlasに組み込むInstanceの数。
	int nowInstanceCount = InstanceVector::Instance()->GetInstanceSize();

	// Tlasに組み込むInstanceの数が、現在のTlasの許容Instance数を超えていたらバッファを再構築する。
	if (instanceCapacity_ < nowInstanceCount) {

		// 許容量を更新。
		instanceCapacity_ = nowInstanceCount;

		// バッファを再構築。
		ReBuildTlas();

	}
	else {

		// バッファを更新。
		UpdateTlas();

	}

}

void Tlas::ReBuildTlas()
{

	/*===== Tlasを再構築 =====*/

	// インスタンスの情報を記録したバッファを準備する。
	int instanceDescSize = instanceCapacity_ * sizeof(D3D12_RAYTRACING_INSTANCE_DESC);
	instanceDescBuffer_ = CreateBuffer(
		instanceDescSize,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_HEAP_TYPE_UPLOAD);
	instanceDescBuffer_->SetName(L"InstanceDescBuffer");
	instanceDescBuffer_->Map(0, nullptr, &instanceDescMapAddress_);

	// 生成したバッファにデータを書き込む。
	WriteToMemory(instanceDescMapAddress_, InstanceVector::Instance()->GetData(), instanceDescSize);

	// メモリ量を求めるための設定を行う。
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildASDesc = {};
	auto& inputs = buildASDesc.Inputs;
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.NumDescs = instanceCapacity_;
	inputs.InstanceDescs = instanceDescBuffer_->GetGPUVirtualAddress();
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;

	// メモリ量を求める関数を実行する。
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO tlasPrebuild{};
	DirectX12Device::Instance()->raytracingDevice->GetRaytracingAccelerationStructurePrebuildInfo(
		&inputs, &tlasPrebuild
	);

	// スクラッチメモリ(バッファ)を確保。
	scratchBuffer_ = CreateBuffer(
		tlasPrebuild.ScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_HEAP_TYPE_DEFAULT
	);
	D3D12_RESOURCE_BARRIER barrier = { CD3DX12_RESOURCE_BARRIER::Transition(scratchBuffer_.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_UNORDERED_ACCESS) };
	scratchBuffer_->SetName(L"TlasScratchBuffer");

	// TLAS用メモリ(バッファ)を確保。
	tlasBuffer_ = CreateBuffer(
		tlasPrebuild.ResultDataMaxSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		D3D12_HEAP_TYPE_DEFAULT
	);
	tlasBuffer_->SetName(L"TlasBuffer");

	// AccelerationStructure構築。
	buildASDesc.ScratchAccelerationStructureData = scratchBuffer_->GetGPUVirtualAddress();
	buildASDesc.DestAccelerationStructureData = tlasBuffer_->GetGPUVirtualAddress();

	// コマンドリストに積んで実行する。
	DirectX12CmdList::Instance()->cmdList->BuildRaytracingAccelerationStructure(
		&buildASDesc, 0, nullptr
	);

	// リソースバリアの設定。
	D3D12_RESOURCE_BARRIER uavBarrier = CD3DX12_RESOURCE_BARRIER::UAV(tlasBuffer_.Get());
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &uavBarrier);

	// Tlasがまだ生成されていなかったら。
	if (descHeapIndex_ == -1) {

		// 加速構造体の設定。
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.RaytracingAccelerationStructure.Location = tlasBuffer_->GetGPUVirtualAddress();

		// Indexを取得する。
		descHeapIndex_ = UavViewHandleMgr::Instance()->GetHandle();

		// ヒープの先頭にバッファを生成。
		DescriptorHeapMgr::Instance()->CreateBufferView(descHeapIndex_, srvDesc);

	}
	else {

		// 加速構造体の設定。
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.RaytracingAccelerationStructure.Location = tlasBuffer_->GetGPUVirtualAddress();

		// すでにバッファを生成してある位置に上書きする形で生成。
		DescriptorHeapMgr::Instance()->CreateBufferView(descHeapIndex_, srvDesc);

	}

}

void Tlas::UpdateTlas()
{

	/*===== Tlasを更新 =====*/

	// 生成したバッファにデータを書き込む。
	int instanceDescSize = instanceCapacity_ * sizeof(D3D12_RAYTRACING_INSTANCE_DESC);
	WriteToMemory(instanceDescMapAddress_, InstanceVector::Instance()->GetData(), instanceDescSize);

	// メモリ量を求めるための設定を行う。
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildASDesc = {};
	auto& inputs = buildASDesc.Inputs;
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.NumDescs = instanceCapacity_;
	inputs.InstanceDescs = instanceDescBuffer_->GetGPUVirtualAddress();
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;

	/*-- BLASのアドレスとスクラッチバッファアドレスとTLASのアドレスを指定して確保処理をコマンドリストに積む --*/

	// AccelerationStructure構築。
	buildASDesc.ScratchAccelerationStructureData = scratchBuffer_->GetGPUVirtualAddress();
	buildASDesc.DestAccelerationStructureData = tlasBuffer_->GetGPUVirtualAddress();

	// コマンドリストに積んで実行する。
	DirectX12CmdList::Instance()->cmdList->BuildRaytracingAccelerationStructure(
		&buildASDesc, 0, nullptr
	);

	// リソースバリアの設定。
	D3D12_RESOURCE_BARRIER uavBarrier = CD3DX12_RESOURCE_BARRIER::UAV(tlasBuffer_.Get());
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &uavBarrier);

}

void Tlas::WriteToMemory(void* MapAddress, const void* PData, size_t DataSize)
{

	/*===== メモリに値を書き込む処理 =====*/

	// nullチェック。
	if (MapAddress == nullptr) assert(0);

	// マップ処理を行う。
	memcpy(MapAddress, PData, DataSize);

}

Microsoft::WRL::ComPtr<ID3D12Resource> Tlas::CreateBuffer(size_t Size, D3D12_RESOURCE_FLAGS Flags, D3D12_RESOURCE_STATES InitialState, D3D12_HEAP_TYPE HeapType, const wchar_t* Name)
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
