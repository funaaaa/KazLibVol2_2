#include "Blas.h"
#include "../Loader/FbxModelResourceMgr.h"
#include "../Render/RenderData.h"
#include "../DirectXCommon/DirectX12Device.h"
#include "../DirectXCommon/DirectX12CmdList.h"
#include <memory>

Blas::Blas(bool IsOpaque, const FbxModelData& ModelDataHandle)
{

	/*===== �R���X�g���N�^ =====*/

	// Blas�̍\�z�ɕK�v�Ȍ`��f�[�^���擾�B
	D3D12_RAYTRACING_GEOMETRY_DESC geomDesc = GetGeometryDesc(IsOpaque, ModelDataHandle);

	// Blas���\�z�B
	BuildBlas(geomDesc);

	// ���C�g���[�V���O��L����
	isRaytracingEnabled = true;

}

uint8_t* Blas::WriteShaderRecord(uint8_t* Dst, UINT RecordSize, Microsoft::WRL::ComPtr<ID3D12StateObject>& StateObject, LPCWSTR HitGroupName)
{

	/*===== �V�F�[�_�[���R�[�h���������� =====*/

	// �g�p���钸�_�f�[�^�Ƃ������܂����炱�����������ށB
	assert(0);

	//Microsoft::WRL::ComPtr<ID3D12StateObjectProperties> rtsoProps_;
	//StateObject.As(&rtsoProps_);
	//auto entryBegin = Dst;

	//auto mode_ = rtsoProps_->GetShaderIdentifier(HitGroupName);
	//if (mode_ == nullptr) {
	//	throw std::logic_error("Not found ShaderIdentifier");
	//}

	//// �V�F�[�_�[���ʎq���������ށB
	//memcpy(Dst, mode_, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	//Dst += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;


	//// ���_�֌W�̃f�[�^���ύX����Ă�����B
	//if (hitGroupInfo_.isChangeVertex_[Index]) {
	//	// ����̃v���O�����ł͈ȉ��̏����Ńf�B�X�N���v�^���L�^�B
	//	// [0] : �C���f�b�N�X�o�b�t�@
	//	// [1] : ���_�o�b�t�@
	//	// �� ���[�J�����[�g�V�O�l�`���̏����ɍ��킹��K�v������B
	//	Dst += WriteGPUDescriptor(Dst, &blasInfo_.indexDescriptor_->GetDescriptor(Index).lock()->GetGPUHandle());
	//	Dst += WriteGPUDescriptor(Dst, &blasInfo_.vertexDescriptor_->GetDescriptor(Index).lock()->GetGPUHandle());

	//	// �}�e���A���p�̃o�b�t�@���Z�b�g�B
	//	Dst += WriteGPUDescriptor(Dst, &blasInfo_.materialDescriptor_->GetDescriptor(Index).lock()->GetGPUHandle());

	//	hitGroupInfo_.isChangeVertex_[Index] = false;

	//}
	//else {

	//	Dst += static_cast<UINT>((sizeof(D3D12_GPU_DESCRIPTOR_HANDLE*)));
	//	Dst += static_cast<UINT>((sizeof(D3D12_GPU_DESCRIPTOR_HANDLE*)));
	//	Dst += static_cast<UINT>((sizeof(D3D12_GPU_DESCRIPTOR_HANDLE*)));

	//}

	//// �q�b�g�O���[�v������q�b�g�O���[�v��ID���擾����B
	//int hitGroupID = HitGroupMgr::Ins()->GetHitGroupID(HitGroupName);

	//// ���_�A�C���f�b�N�X�A�}�e���A���̃I�t�Z�b�g
	//const int OFFSET_VERTEX_INDEX_MATERIAL = 3;

	//// �q�b�g�O���[�vID����SRV�̐����擾�B
	//int srvCount = HitGroupMgr::Ins()->GetHitGroupSRVCount(hitGroupID) - OFFSET_VERTEX_INDEX_MATERIAL;

	//// �e�N�X�`���֌W���ύX����Ă�����B
	//if (hitGroupInfo_.isChangeTexture_[Index]) {

	//	// �����̓e�N�X�`���̃T�C�Y�ł͂Ȃ��A�p�C�v���C���ɃZ�b�g���ꂽSRV�̐��������Ă��Ă�����g���B
	//	// ���̎��_��SRV�̐��ƃe�N�X�`���̐��������Ă��Ȃ�������assert���o���B
	//	for (int index = 0; index < srvCount; ++index) {

	//		// 0�Ԗڂ͊�{�e�N�X�`���B
	//		if (index == 0) {

	//			// ��{�̃e�N�X�`�����ݒ肳��Ă��Ȃ��B
	//			if (hitGroupInfo_.baseTextureHandle_ == -1) assert(0);

	//			CD3DX12_GPU_DESCRIPTOR_HANDLE texDescHandle = DescriptorHeapMgr::Ins()->GetGPUHandleIncrement(hitGroupInfo_.baseTextureHandle_);
	//			Dst += WriteGPUDescriptor(Dst, &texDescHandle);

	//		}
	//		// 1�Ԗڂ͖@���}�b�v�e�N�X�`���B
	//		else if (index == 1) {

	//			if (hitGroupInfo_.mapTextureHandle_ != -1) {

	//				CD3DX12_GPU_DESCRIPTOR_HANDLE texDescHandle = DescriptorHeapMgr::Ins()->GetGPUHandleIncrement(hitGroupInfo_.mapTextureHandle_);
	//				Dst += WriteGPUDescriptor(Dst, &texDescHandle);

	//			}
	//			else {

	//				// �@���}�b�v���ݒ肳��Ă��Ȃ�������A�������̌��Ԃ𖄂߂邽�ߒʏ�̃e�N�X�`�����������ށB
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

	//// �g�p����UAV�̐����擾�B
	//int uavCount = HitGroupMgr::Ins()->GetHitGroupUAVCount(hitGroupID);
	//for (int index = 0; index < uavCount; ++index) {

	//	// �e�N�X�`�������݂��Ă�����B
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

	/*===== Blas�̌`��ݒ�ɕK�v�ȍ\���̂�Ԃ� =====*/

	// �`��f�[�^�̃t���O��ݒ�B
	auto geometryDesc = D3D12_RAYTRACING_GEOMETRY_DESC{};
	geometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
	// �s�����t���O��ݒ�B GPU����AnyHitShader���ĂԂ��ǂ����������Ō���B
	if (IsOpaque) {
		geometryDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
	}
	else {
		geometryDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_NO_DUPLICATE_ANYHIT_INVOCATION;
	}

	// ���f���̌`��f�[�^���擾�B
	std::weak_ptr<FbxResourceData> resourceData = FbxModelResourceMgr::Instance()->GetResourceData(ModelDataHandle.handle.handle);

	// �`��f�[�^�ׂ̍������ڂ�ݒ�B
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

	/*===== Blas���\�z =====*/

	// AS(���C�Ƃ̔����v���ɍs�����߂ɕK�v�ȃ|���S���f�[�^�\����)�̐ݒ�ɕK�v�Ȋe���ڂ�ݒ�B
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC buildASDesc{};
	auto& inputs = buildASDesc.Inputs;	// D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS
	inputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
	inputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE;
	inputs.NumDescs = 1;
	inputs.pGeometryDescs = &GeomDesc;

	// �֐����g���ĕK�v�ȃ������ʂ����߂�.
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO blasPrebuild{};
	DirectX12Device::Instance()->raytracingDevice->GetRaytracingAccelerationStructurePrebuildInfo(
		&inputs, &blasPrebuild
	);

	// �X�N���b�`�o�b�t�@�𐶐�����B
	scratchBuffer_ = CreateBuffer(
		blasPrebuild.ScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_HEAP_TYPE_DEFAULT
	);
	D3D12_RESOURCE_BARRIER barrierToUAV[] = { CD3DX12_RESOURCE_BARRIER::Transition(scratchBuffer_.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_UNORDERED_ACCESS) };
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, barrierToUAV);
	scratchBuffer_->SetName(L"BlasScratchBuffer");

	// Blas�̃o�b�t�@�𐶐�����B
	blasBuffer_ = CreateBuffer(
		blasPrebuild.ResultDataMaxSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		D3D12_HEAP_TYPE_DEFAULT
	);
	blasBuffer_->SetName(L"BlasBuffer");

	// �X�V�p�o�b�t�@�𐶐�����B
	updateBuffer_ = CreateBuffer(
		blasPrebuild.UpdateScratchDataSizeInBytes,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_HEAP_TYPE_DEFAULT
	);
	barrierToUAV[0] = { CD3DX12_RESOURCE_BARRIER::Transition(updateBuffer_.Get(),D3D12_RESOURCE_STATE_COMMON,D3D12_RESOURCE_STATE_UNORDERED_ACCESS) };
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, barrierToUAV);
	updateBuffer_->SetName(L"BlasUpdateBuffer");

	// Blas���\�z����B
	buildASDesc.ScratchAccelerationStructureData = scratchBuffer_->GetGPUVirtualAddress();
	buildASDesc.DestAccelerationStructureData = blasBuffer_->GetGPUVirtualAddress();

	// �R�}���h���X�g�ɐς�Ŏ��s����B
	DirectX12CmdList::Instance()->cmdList->BuildRaytracingAccelerationStructure(
		&buildASDesc, 0, nullptr /* pPostBuildInfoDescs */
	);

	// ���\�[�X�o���A�̐ݒ�B
	D3D12_RESOURCE_BARRIER uavBarrier{};
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = blasBuffer_.Get();
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(1, &uavBarrier);

}

Microsoft::WRL::ComPtr<ID3D12Resource> Blas::CreateBuffer(size_t Size, D3D12_RESOURCE_FLAGS Flags, D3D12_RESOURCE_STATES InitialState, D3D12_HEAP_TYPE HeapType, const wchar_t* Name)
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
