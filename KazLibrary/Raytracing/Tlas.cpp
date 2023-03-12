#include "Tlas.h"
#include "../DirectXCommon/DirectX12Device.h"
#include "../DirectXCommon/DirectX12CmdList.h"
#include "../Raytracing/InstanceVector.h"
#include "../Buffer/DescriptorHeapMgr.h"
#include "../Buffer/UavViewHandleMgr.h"

Tlas::Tlas()
{

	/*===== �R���X�g���N�^ =====*/

	instanceCapacity_ = -1;
	descHeapIndex_ = -1;

}

void Tlas::Build()
{

	/*===== Tlas���\�z =====*/

	// ���݂�Tlas�ɑg�ݍ���Instance�̐��B
	int nowInstanceCount = InstanceVector::Instance()->GetInstanceSize();

	// Tlas�ɑg�ݍ���Instance�̐����A���݂�Tlas�̋��eInstance���𒴂��Ă�����o�b�t�@���č\�z����B
	if (instanceCapacity_ < nowInstanceCount) {

		// ���e�ʂ��X�V�B
		instanceCapacity_ = nowInstanceCount;

		// �o�b�t�@���č\�z�B
		ReBuildTlas();

	}
	else {

		// �o�b�t�@���X�V�B
		UpdateTlas();

	}

}

void Tlas::ReBuildTlas()
{

	/*===== Tlas���č\�z =====*/

	// �C���X�^���X�̏����L�^�����o�b�t�@����������B
	int instanceDescSize = instanceCapacity_ * sizeof(D3D12_RAYTRACING_INSTANCE_DESC);
	instanceDescBuffer_ = CreateBuffer(
		instanceDescSize,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_HEAP_TYPE_UPLOAD);
	instanceDescBuffer_->SetName(L"InstanceDescBuffer");
	instanceDescBuffer_->Map(0, nullptr, &instanceDescMapAddress_);

	// ���������o�b�t�@�Ƀf�[�^���������ށB
	WriteToMemory(instanceDescMapAddress_, InstanceVector::Instance()->GetData(), instanceDescSize);

	// �������ʂ����߂邽�߂̐ݒ���s���B
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildASDesc = {};
	auto& inputs = buildASDesc.Inputs;
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.NumDescs = instanceCapacity_;
	inputs.InstanceDescs = instanceDescBuffer_->GetGPUVirtualAddress();
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;

	// �������ʂ����߂�֐������s����B
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO tlasPrebuild{};
	DirectX12Device::Instance()->raytracingDevice->GetRaytracingAccelerationStructurePrebuildInfo(
		&inputs, &tlasPrebuild
	);

	// �X�N���b�`������(�o�b�t�@)���m�ہB
	scratchBuffer_ = CreateBuffer(
		tlasPrebuild.ScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_HEAP_TYPE_DEFAULT
	);
	D3D12_RESOURCE_BARRIER barrier = { CD3DX12_RESOURCE_BARRIER::Transition(scratchBuffer_.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_UNORDERED_ACCESS) };
	scratchBuffer_->SetName(L"TlasScratchBuffer");

	// TLAS�p������(�o�b�t�@)���m�ہB
	tlasBuffer_ = CreateBuffer(
		tlasPrebuild.ResultDataMaxSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		D3D12_HEAP_TYPE_DEFAULT
	);
	tlasBuffer_->SetName(L"TlasBuffer");

	// AccelerationStructure�\�z�B
	buildASDesc.ScratchAccelerationStructureData = scratchBuffer_->GetGPUVirtualAddress();
	buildASDesc.DestAccelerationStructureData = tlasBuffer_->GetGPUVirtualAddress();

	// �R�}���h���X�g�ɐς�Ŏ��s����B
	DirectX12CmdList::Instance()->cmdList->BuildRaytracingAccelerationStructure(
		&buildASDesc, 0, nullptr
	);

	// ���\�[�X�o���A�̐ݒ�B
	D3D12_RESOURCE_BARRIER uavBarrier = CD3DX12_RESOURCE_BARRIER::UAV(tlasBuffer_.Get());
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &uavBarrier);

	// Tlas���܂���������Ă��Ȃ�������B
	if (descHeapIndex_ == -1) {

		// �����\���̂̐ݒ�B
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.RaytracingAccelerationStructure.Location = tlasBuffer_->GetGPUVirtualAddress();

		// Index���擾����B
		descHeapIndex_ = UavViewHandleMgr::Instance()->GetHandle();

		// �q�[�v�̐擪�Ƀo�b�t�@�𐶐��B
		DescriptorHeapMgr::Instance()->CreateBufferView(descHeapIndex_, srvDesc);

	}
	else {

		// �����\���̂̐ݒ�B
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.RaytracingAccelerationStructure.Location = tlasBuffer_->GetGPUVirtualAddress();

		// ���łɃo�b�t�@�𐶐����Ă���ʒu�ɏ㏑������`�Ő����B
		DescriptorHeapMgr::Instance()->CreateBufferView(descHeapIndex_, srvDesc);

	}

}

void Tlas::UpdateTlas()
{

	/*===== Tlas���X�V =====*/

	// ���������o�b�t�@�Ƀf�[�^���������ށB
	int instanceDescSize = instanceCapacity_ * sizeof(D3D12_RAYTRACING_INSTANCE_DESC);
	WriteToMemory(instanceDescMapAddress_, InstanceVector::Instance()->GetData(), instanceDescSize);

	// �������ʂ����߂邽�߂̐ݒ���s���B
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildASDesc = {};
	auto& inputs = buildASDesc.Inputs;
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.NumDescs = instanceCapacity_;
	inputs.InstanceDescs = instanceDescBuffer_->GetGPUVirtualAddress();
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;

	/*-- BLAS�̃A�h���X�ƃX�N���b�`�o�b�t�@�A�h���X��TLAS�̃A�h���X���w�肵�Ċm�ۏ������R�}���h���X�g�ɐς� --*/

	// AccelerationStructure�\�z�B
	buildASDesc.ScratchAccelerationStructureData = scratchBuffer_->GetGPUVirtualAddress();
	buildASDesc.DestAccelerationStructureData = tlasBuffer_->GetGPUVirtualAddress();

	// �R�}���h���X�g�ɐς�Ŏ��s����B
	DirectX12CmdList::Instance()->cmdList->BuildRaytracingAccelerationStructure(
		&buildASDesc, 0, nullptr
	);

	// ���\�[�X�o���A�̐ݒ�B
	D3D12_RESOURCE_BARRIER uavBarrier = CD3DX12_RESOURCE_BARRIER::UAV(tlasBuffer_.Get());
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &uavBarrier);

}

void Tlas::WriteToMemory(void* MapAddress, const void* PData, size_t DataSize)
{

	/*===== �������ɒl���������ޏ��� =====*/

	// null�`�F�b�N�B
	if (MapAddress == nullptr) assert(0);

	// �}�b�v�������s���B
	memcpy(MapAddress, PData, DataSize);

}

Microsoft::WRL::ComPtr<ID3D12Resource> Tlas::CreateBuffer(size_t Size, D3D12_RESOURCE_FLAGS Flags, D3D12_RESOURCE_STATES InitialState, D3D12_HEAP_TYPE HeapType, const wchar_t* Name)
{

	/*===== �o�b�t�@�𐶐� =====*/

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
