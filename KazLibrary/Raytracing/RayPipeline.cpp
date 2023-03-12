#include "../Raytracing/RayPipeline.h"
#include "../Raytracing/RayRootsignature.h"
#include "../Raytracing/HitGroupMgr.h"
#include "../DirectXCommon/DirectX12Device.h"
#include "../DirectXCommon/DirectX12CmdList.h"
#include "../Buffer/DescriptorHeapMgr.h"
#include "../Raytracing/ShaderStorage.h"
#include "../Raytracing/BlasReferenceVector.h"
#include "../Raytracing/Tlas.h"
#include "../Raytracing/RaytracingOutput.h"
#include <DirectXMath.h>

RayPipeline::RayPipeline(const std::vector<RayPipelineShaderData>& InputData, HitGroupMgr::HITGROUP_ID UseHitGroup, int SRVCount, int CBVCount, int UAVCount, int PayloadSize, int AttribSize, int ReflectionCount)
{

	/*===== �Z�b�e�B���O���� =====*/

	// �q�b�g�O���[�v����ۑ��B
	hitGroupName_ = HitGroupMgr::Instance()->hitGroupNames[UseHitGroup];

	// ���͂��ꂽ�f�[�^��ۑ�����B
	const int INPUT_COUNT = static_cast<int>(InputData.size());
	for (int index = 0; index < INPUT_COUNT; ++index) {

		// ���͂��ꂽ�f�[�^�\���́B
		RayPipelineShaderData buff;

		// �ۑ�����B
		buff.shaderPath_ = InputData[index].shaderPath_;

		// �ۑ�����Ă���G���g���|�C���g��ۑ��B
		const int RG_ENTRY_COUNT = static_cast<int>(InputData[index].rayGenEnteryPoint_.size());
		for (int entryPointIndex = 0; entryPointIndex < RG_ENTRY_COUNT; ++entryPointIndex) {

			// �ۑ�����B
			buff.rayGenEnteryPoint_.push_back(InputData[index].rayGenEnteryPoint_[entryPointIndex]);

		}
		const int MS_ENTRY_COUNT = static_cast<int>(InputData[index].missEntryPoint_.size());
		for (int entryPointIndex = 0; entryPointIndex < MS_ENTRY_COUNT; ++entryPointIndex) {

			// �ۑ�����B
			buff.missEntryPoint_.push_back(InputData[index].missEntryPoint_[entryPointIndex]);

		}

		// �ۑ�����B
		const int HS_ENTRY_COUNT = static_cast<int>(InputData[index].hitgroupEntryPoint_.size());
		for (int entryPointIndex = 0; entryPointIndex < HS_ENTRY_COUNT; ++entryPointIndex) {
			buff.hitgroupEntryPoint_.push_back(InputData[index].hitgroupEntryPoint_[entryPointIndex]);
		}

		// �ۑ�����B
		shaderData_.push_back(buff);

	}

	// �O���[�o�����[�g�V�O�l�`����ݒ�B
	globalRootSig_ = std::make_shared<RayRootsignature>();
	// �p�����[�^�[t0��TLAS(SRV)��ݒ�B
	for (int index = 0; index < SRVCount; ++index)globalRootSig_->AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, index);
	// �p�����[�^�[b0�ɃJ�����p�o�b�t�@��ݒ�B
	for (int index = 0; index < CBVCount; ++index) {
		globalRootSig_->AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, index);
	}
	// �p�����[�^�[u0�ɏo�͗p�o�b�t�@��ݒ�B
	for (int index = 0; index < UAVCount; ++index)globalRootSig_->AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, index);

	// ���[�g�V�O�l�`���𐶐��B
	globalRootSig_->Build(false, L"GlobalRootSig");

	// �X�e�[�g�I�u�W�F�N�g�̐ݒ��ۑ����Ă����悤�̍\���́B
	CD3DX12_STATE_OBJECT_DESC subobjects;
	// �X�e�[�g�I�u�W�F�N�g�̏�Ԃ�ݒ�B
	subobjects.SetStateObjectType(D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE);

	// �V�F�[�_�[��ǂݍ��ށB
	const int SHADER_COUNT = static_cast<int>(shaderData_.size());
	for (int index = 0; index < SHADER_COUNT; ++index) {

		shaderCode_.emplace_back();

		// �V�F�[�_�[���R���p�C������B
		ShaderStorage::Instance()->LoadShaderForDXC(shaderData_[index].shaderPath_, "lib_6_4", "");

		// �V�F�[�_�[��ǂݍ��ށB
		shaderCode_[index].BytecodeLength = static_cast<SIZE_T>(ShaderStorage::Instance()->GetShaderBin(shaderData_[index].shaderPath_).size());
		shaderCode_[index].pShaderBytecode = ShaderStorage::Instance()->GetShaderBin(shaderData_[index].shaderPath_).data();

		// �V�F�[�_�[�̊e�֐����R�[�h�̓o�^�B
		auto dxilLib = subobjects.CreateSubobject<CD3DX12_DXIL_LIBRARY_SUBOBJECT>();
		dxilLib->SetDXILLibrary(&shaderCode_[index]);

		// �V�F�[�_�[�̃G���g���|�C���g��o�^�B
		const int RG_ENTRY_COUNT = static_cast<int>(InputData[index].rayGenEnteryPoint_.size());
		for (int entryPointIndex = 0; entryPointIndex < RG_ENTRY_COUNT; ++entryPointIndex) {

			// �ۑ�����B
			dxilLib->DefineExport(shaderData_[index].rayGenEnteryPoint_[entryPointIndex]);

		}
		const int MS_ENTRY_COUNT = static_cast<int>(InputData[index].missEntryPoint_.size());
		for (int entryPointIndex = 0; entryPointIndex < MS_ENTRY_COUNT; ++entryPointIndex) {

			// �ۑ�����B
			dxilLib->DefineExport(shaderData_[index].missEntryPoint_[entryPointIndex]);

		}
		const int HG_ENTRY_COUNT = static_cast<int>(InputData[index].hitgroupEntryPoint_.size());
		for (int entryPointIndex = 0; entryPointIndex < HG_ENTRY_COUNT; ++entryPointIndex) {

			// �ۑ�����B
			dxilLib->DefineExport(shaderData_[index].hitgroupEntryPoint_[entryPointIndex]);

		}

	}

	// �q�b�g�O���[�v�̐ݒ�B
	auto hitGroup_ = subobjects.CreateSubobject<CD3DX12_HIT_GROUP_SUBOBJECT>();
	hitGroup_->SetHitGroupType(D3D12_HIT_GROUP_TYPE_TRIANGLES);

	// ClosestHitShader���G���g���|�C���g��ۑ��B
	if (HitGroupMgr::Instance()->GetCHFlag(UseHitGroup)) {
		hitGroup_->SetClosestHitShaderImport(HitGroupMgr::Instance()->GetCH(UseHitGroup));
	}
	// AnyHitShader�̃G���g���|�C���g��ۑ��B
	if (HitGroupMgr::Instance()->GetAHFlag(UseHitGroup)) {
		hitGroup_->SetAnyHitShaderImport(HitGroupMgr::Instance()->GetAH(UseHitGroup));
	}
	// IntersectShader�̃G���g���|�C���g��ۑ��B
	if (HitGroupMgr::Instance()->GetISFlag(UseHitGroup)) {
		hitGroup_->SetIntersectionShaderImport(HitGroupMgr::Instance()->GetIS(UseHitGroup));
	}
	// �q�b�g�O���[�v����ۑ��B
	hitGroup_->SetHitGroupExport(HitGroupMgr::Instance()->hitGroupNames[UseHitGroup]);

	// �O���[�o�����[�g�V�O�l�`���̐ݒ�B
	auto rootSig = subobjects.CreateSubobject<CD3DX12_GLOBAL_ROOT_SIGNATURE_SUBOBJECT>();
	rootSig->SetRootSignature(globalRootSig_->GetRootSig().Get());

	// HitGroup�̃��[�J�����[�g�V�O�l�`����ݒ�B
	auto chLocalRootSig = subobjects.CreateSubobject<CD3DX12_LOCAL_ROOT_SIGNATURE_SUBOBJECT>();
	chLocalRootSig->SetRootSignature(HitGroupMgr::Instance()->GetLocalRootSig(UseHitGroup)->GetRootSig().Get());
	auto chAssocModel = subobjects.CreateSubobject<CD3DX12_SUBOBJECT_TO_EXPORTS_ASSOCIATION_SUBOBJECT>();
	chAssocModel->AddExport(HitGroupMgr::Instance()->hitGroupNames[UseHitGroup]);
	chAssocModel->SetSubobjectToAssociate(*chLocalRootSig);

	// �V�F�[�_�[�̐ݒ�B
	auto shaderConfig = subobjects.CreateSubobject<CD3DX12_RAYTRACING_SHADER_CONFIG_SUBOBJECT>();
	shaderConfig->Config(PayloadSize, AttribSize);

	// �p�C�v���C���̐ݒ�B
	auto pipelineConfig = subobjects.CreateSubobject<CD3DX12_RAYTRACING_PIPELINE_CONFIG_SUBOBJECT>();
	pipelineConfig->Config(ReflectionCount);

	// ��������B
	DirectX12Device::Instance()->raytracingDevice->CreateStateObject(
		subobjects, IID_PPV_ARGS(&stateObject_)
	);
	stateObject_->SetName(L"StateObject");

	// Blas�̕ێ���
	numBlas_ = 0;

}

void RayPipeline::BuildShaderTable(int DispatchX, int DispatchY)
{

	/*===== �V�F�[�_�[�e�[�u�����\�z =====*/

	// �R���e�i�ɓo�^����Ă���Blas�̐��B
	int blasRefCount = BlasReferenceVector::Instance()->GetBlasRefCount();

	// Blas�̐���0�������牽�����Ȃ��B
	if (blasRefCount == 0) return;

	// Blas�̎Q�Ɛ����p�C�v���C���Ɋ܂܂�Ă���Blas�̐�����������V�F�[�_�[�e�[�u�����č\�z����B
	if (numBlas_ < blasRefCount) {

		// �č\�z�B
		ConstructionShaderTable(DispatchX, DispatchY);

		numBlas_ = blasRefCount;

	}
	else {

		// �č\�z������Blas�̏����X�V�B
		MapHitGroupInfo();

	}

}

void RayPipeline::ConstructionShaderTable(int DispatchX, int DispatchY)
{

	/*===== �V�F�[�_�[�e�[�u�����\�z =====*/

	// �V�F�[�_�[�e�[�u���̃T�C�Y���v�Z�B
	const auto ShaderRecordAlignment = D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT;

	// RayGeneration�V�F�[�_�[�ł̓��[�J�����[�g�V�O�l�`�����g�p�B
	raygenRecordSize_ += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	raygenRecordSize_ = RoundUp(raygenRecordSize_, ShaderRecordAlignment);

	// Miss�V�F�[�_�[�ł̓��[�J�����[�g�V�O�l�`�����g�p�B
	missRecordSize_ += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	missRecordSize_ = RoundUp(missRecordSize_, ShaderRecordAlignment);

	// �q�b�g�O���[�v�ł́A�ۑ�����Ă���q�b�g�O���[�v�̒�����ő�̃T�C�Y�̂��̂Ńf�[�^���m�ۂ���B
	hitgroupRecordSize_ += D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
	hitgroupRecordSize_ += GetLargestDataSizeInHitGroup();
	hitgroupRecordSize_ = RoundUp(hitgroupRecordSize_, ShaderRecordAlignment);

	// �g�p����e�V�F�[�_�[�̌����A�V�F�[�_�[�e�[�u���̃T�C�Y�����߂�B
	hitgroupCount = HitGroupMgr::Instance()->GetHitGroupCount();
	raygenSize_ = GetRayGenerationCount() * raygenRecordSize_;
	missSize_ = GetMissCount() * missRecordSize_;
	hitGroupSize_ = hitgroupCount * hitgroupRecordSize_;

	// �e�e�[�u���̊J�n�ʒu�ɃA���C�����g���񂪂���̂Œ�������B
	tableAlign_ = D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT;
	raygenRegion_ = RoundUp(raygenRecordSize_, tableAlign_);
	missRegion_ = RoundUp(missSize_, tableAlign_);
	hitgroupRegion_ = RoundUp(hitGroupSize_, tableAlign_);

	// �������ꂽBLAS�̐��B
	const int BLAS_COUNT = BlasReferenceVector::Instance()->GetBlasRefCount();

	// �V�F�[�_�[�e�[�u���̃T�C�Y�B
	tableSize_ = raygenRegion_ + missRegion_ + hitgroupRegion_ * BLAS_COUNT;

	/*========== �V�F�[�_�[�e�[�u���̍\�z ==========*/

	// �V�F�[�_�[�e�[�u���m�ہB
	shaderTable_ = CreateBuffer(
		tableSize_, D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_HEAP_TYPE_UPLOAD,
		L"ShaderTable0");
	shaderTalbeMapAddress_ = nullptr;
	shaderTable_->Map(0, nullptr, &shaderTalbeMapAddress_);

	stateObject_.As(&rtsoProps_);

	// �V�F�[�_�[�e�[�u�����������݁A���C��ݒ肷��B
	WriteShadetTalbeAndSettingRay(DispatchX, DispatchY);

}

void RayPipeline::MapHitGroupInfo()
{

	/*===== HitGroup�̏���]�� =====*/

	uint8_t* pStart = static_cast<uint8_t*>(shaderTalbeMapAddress_);

	// Hit Group �p�̃V�F�[�_�[���R�[�h���������݁B
	uint8_t* hitgroupStart = pStart + raygenRegion_ + missRegion_;
	{

		uint8_t* pRecord = hitgroupStart;

		pRecord = BlasReferenceVector::Instance()->WriteShaderRecord(pRecord, hitgroupRecordSize_, stateObject_, hitGroupName_);

	}

}

void RayPipeline::TraceRay(std::weak_ptr<RaytracingOutput> Output, std::weak_ptr<RaytracingOutput> GBuffer0, std::weak_ptr<RaytracingOutput> GBuffer1)
{

	/*===== ���C�g���[�V���O�����s =====*/

	// �O���[�o�����[�g�V�O�l�`���Ŏg���Ɛ錾���Ă��郊�\�[�X����Z�b�g�B
	ID3D12DescriptorHeap* descriptorHeaps[] = { DescriptorHeapMgr::Instance()->GetHeap().Get() };
	DirectX12CmdList::Instance()->cmdList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	DirectX12CmdList::Instance()->cmdList->SetComputeRootSignature(GetGlobalRootSig()->GetRootSig().Get());

	// TLAS��ݒ�B
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(Tlas::Instance()->GetDescHeapHandle()));

	// �o�͗pUAV��ݒ�B
	Output.lock()->SetComputeRootDescriptorTalbe(1);
	GBuffer0.lock()->SetComputeRootDescriptorTalbe(2);
	GBuffer1.lock()->SetComputeRootDescriptorTalbe(3);

	// �p�C�v���C����ݒ�B
	DirectX12CmdList::Instance()->cmdList->SetPipelineState1(stateObject_.Get());

	// ���C�g���[�V���O�����s�B
	DirectX12CmdList::Instance()->cmdList->DispatchRays(&dispatchRayDesc_);

}

UINT RayPipeline::GetLargestDataSizeInHitGroup()
{

	/*===== �S�Ẵq�b�g�O���[�v�̒��ōł��f�[�^�T�C�Y���傫�����̂��擾���� =====*/

	UINT largestDataSize = 0;

	const int HIT_GROUP_COUNT = HitGroupMgr::Instance()->GetHitGroupCount();
	for (int index = 0; index < HIT_GROUP_COUNT; ++index) {

		// �f�[�^�T�C�Y���擾�B
		UINT dataSize = 0;
		dataSize += sizeof(D3D12_GPU_DESCRIPTOR_HANDLE) * HitGroupMgr::Instance()->GetSRVCount(static_cast<HitGroupMgr::HITGROUP_ID>(index));
		dataSize += sizeof(D3D12_GPU_VIRTUAL_ADDRESS) * HitGroupMgr::Instance()->GetCBVCount(static_cast<HitGroupMgr::HITGROUP_ID>(index));
		dataSize += sizeof(D3D12_GPU_DESCRIPTOR_HANDLE) * HitGroupMgr::Instance()->GetUAVCount(static_cast<HitGroupMgr::HITGROUP_ID>(index));

		// �擾�����f�[�^�T�C�Y���ۑ�����Ă���f�[�^�T�C�Y��菬���������珈�����΂��B
		if (dataSize < largestDataSize) continue;

		// �擾�����f�[�^�T�C�Y��ۑ�����B
		largestDataSize = dataSize;

	}

	return largestDataSize;

}

void RayPipeline::WriteShadetTalbeAndSettingRay(int DispatchX, int DispatchY)
{

	/*===== �V�F�[�_�[�e�[�u�����������݁A���C��ݒ肷�� =====*/

	// �e�V�F�[�_�[���R�[�h����������ł����B
	uint8_t* pStart = static_cast<uint8_t*>(shaderTalbeMapAddress_);

	// RayGeneration �p�̃V�F�[�_�[���R�[�h���������݁B
	uint8_t* rgsStart = pStart;
	{
		uint8_t* p = rgsStart;

		const int SHADER_DATA_COUNT = static_cast<int>(shaderData_.size());
		for (int index = 0; index < SHADER_DATA_COUNT; ++index) {

			const int RG_COUNT = static_cast<int>(shaderData_[index].rayGenEnteryPoint_.size());
			for (int rgIndex = 0; rgIndex < RG_COUNT; ++rgIndex) {

				void* id = rtsoProps_->GetShaderIdentifier(shaderData_[index].rayGenEnteryPoint_[rgIndex]);
				p += WriteShaderIdentifier(p, id);

			}

		}

	}

	// Miss Shader �p�̃V�F�[�_�[���R�[�h���������݁B
	uint8_t* missStart = pStart + raygenRegion_;
	{
		uint8_t* p = missStart;

		const int SHADER_DATA_COUNT = static_cast<int>(shaderData_.size());
		for (int index = 0; index < SHADER_DATA_COUNT; ++index) {

			const int MS_COUNT = static_cast<int>(shaderData_[index].missEntryPoint_.size());
			for (int msIndex = 0; msIndex < MS_COUNT; ++msIndex) {

				void* id = rtsoProps_->GetShaderIdentifier(shaderData_[index].missEntryPoint_[msIndex]);
				p += WriteShaderIdentifier(p, id);

			}

		}

	}

	// Hit Group �p�̃V�F�[�_�[���R�[�h���������݁B
	uint8_t* hitgroupStart = pStart + raygenRegion_ + missRegion_;
	{

		uint8_t* pRecord = hitgroupStart;

		pRecord = BlasReferenceVector::Instance()->WriteShaderRecord(pRecord, hitgroupRecordSize_, stateObject_, hitGroupName_);

	}

	// ���C���ˎ��̐ݒ��ݒ�B

	// DispatchRays �̂��߂ɏ����Z�b�g���Ă���.
	auto startAddress = shaderTable_->GetGPUVirtualAddress();
	// RayGeneration�V�F�[�_�[�̏��
	auto& shaderRecordRG = dispatchRayDesc_.RayGenerationShaderRecord;
	shaderRecordRG.StartAddress = startAddress;
	shaderRecordRG.SizeInBytes = raygenSize_;
	startAddress += raygenRegion_;
	// Miss�V�F�[�_�[�̏��
	auto& shaderRecordMS = dispatchRayDesc_.MissShaderTable;
	shaderRecordMS.StartAddress = startAddress;
	shaderRecordMS.SizeInBytes = missSize_;
	shaderRecordMS.StrideInBytes = missRecordSize_;
	startAddress += missRegion_;
	// HitGroup�̏��
	auto& shaderRecordHG = dispatchRayDesc_.HitGroupTable;
	shaderRecordHG.StartAddress = startAddress;
	shaderRecordHG.SizeInBytes = hitGroupSize_;
	shaderRecordHG.StrideInBytes = hitgroupRecordSize_;
	startAddress += hitgroupRegion_;
	// ���C�̏��
	dispatchRayDesc_.Width = DispatchX;
	dispatchRayDesc_.Height = DispatchY;
	dispatchRayDesc_.Depth = 1;

}

UINT RayPipeline::WriteShaderIdentifier(void* dst, const void* shaderId)
{
	memcpy(dst, shaderId, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
	return D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
}

int RayPipeline::GetRayGenerationCount()
{
	int count = 0;

	const int SHADER_DATA_COUNT = static_cast<int>(shaderData_.size());
	for (int index = 0; index < SHADER_DATA_COUNT; ++index) {

		count += static_cast<int>(shaderData_[index].rayGenEnteryPoint_.size());

	}

	return count;

}

int RayPipeline::GetMissCount()
{
	int count = 0;

	const int SHADER_DATA_COUNT = static_cast<int>(shaderData_.size());
	for (int index = 0; index < SHADER_DATA_COUNT; ++index) {

		count += static_cast<int>(shaderData_[index].missEntryPoint_.size());

	}

	return count;

}

Microsoft::WRL::ComPtr<ID3D12Resource> RayPipeline::CreateBuffer(size_t Size, D3D12_RESOURCE_FLAGS Flags, D3D12_RESOURCE_STATES InitialState, D3D12_HEAP_TYPE HeapType, const wchar_t* Name)
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