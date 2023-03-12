#pragma once
#include <string>
#include <wtypes.h>
#include <vector>
#include <array>
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include "../Raytracing/HitGroupMgr.h"

// ���C�g���p���[�g�V�O�l�`��
class RayRootsignature;
class RaytracingOutput;

/// <summary>
/// �V�F�[�_�[�e�[�u���ɓo�^����e��V�F�[�_�[�̃G���g���|�C���g����ۑ��������́B
/// </summary>
struct RayPipelineShaderData {

	std::string shaderPath_;					// �V�F�[�_�[�p�X
	std::vector<LPCWSTR> rayGenEnteryPoint_;	// �G���g���|�C���g
	std::vector<LPCWSTR> missEntryPoint_;		// �G���g���|�C���g
	std::vector<LPCWSTR> hitgroupEntryPoint_;	// �G���g���|�C���g
	RayPipelineShaderData() {};
	RayPipelineShaderData(const std::string& ShaderPath, const std::vector<LPCWSTR>& RGEntry, const std::vector<LPCWSTR>& MSEntry, const std::vector<LPCWSTR>& HGEntry)
		:shaderPath_(ShaderPath), rayGenEnteryPoint_(RGEntry), missEntryPoint_(MSEntry), hitgroupEntryPoint_(HGEntry) {};

};

/// <summary>
/// ���C�g���[�V���O�Ŏg�p����p�C�v���C��
/// </summary>
class RayPipeline {

private:

	/*===== �����o�ϐ� =====*/

	std::vector<RayPipelineShaderData> shaderData_;			// �g�p����V�F�[�_�[��Z�߂��\����
	std::vector<D3D12_SHADER_BYTECODE> shaderCode_;			// �g�p����V�F�[�_�[�̃o�C�g�R�[�h
	Microsoft::WRL::ComPtr<ID3D12StateObject> stateObject_;	// �X�e�[�g�I�u�W�F�N�g
	std::shared_ptr<RayRootsignature> globalRootSig_;		// �O���[�o�����[�g�V�O�l�`��
	D3D12_DISPATCH_RAYS_DESC dispatchRayDesc_;				// ���C���ˎ��̐ݒ�
	Microsoft::WRL::ComPtr<ID3D12Resource> shaderTable_;	// �V�F�[�_�[�e�[�u��
	void* shaderTalbeMapAddress_;							// �V�F�[�_�[�e�[�u���̃f�[�^�]���pMap�A�h���X
	Microsoft::WRL::ComPtr<ID3D12StateObjectProperties> rtsoProps_;
	LPCWSTR hitGroupName_;
	int numBlas_;											// �p�C�v���C���ɑg�ݍ��܂�Ă���Blas�̐��BBlasRefecenceVector���ێ����Ă���Q�Ƃ̐������̕ϐ��̐��𒴂�����p�C�v���C������蒼���B


	// �V�F�[�_�[�e�[�u���̍\�z�ɕK�v�ȕϐ��Q
	UINT raygenRecordSize_;
	UINT missRecordSize_;
	UINT hitgroupRecordSize_;
	UINT hitgroupCount;
	UINT raygenSize_;
	UINT missSize_;
	UINT hitGroupSize_;
	UINT tableAlign_;
	UINT hitgroupRegion_;
	UINT tableSize_;
	UINT raygenRegion_;
	UINT missRegion_;


public:

	/*===== �����o�֐� =====*/

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="InputData"> �g�p����V�F�[�_�[�̊e��f�[�^ </param>
	/// <param name="UseHitGroup"> �g�p����HitGroup�̖��O </param>
	/// <param name="SRVCount"> �g�p����SRV�̐� </param>
	/// <param name="CBVCount"> �g�p����CBV�̐� </param>
	/// <param name="UAVCount"> �g�p����UAV�̐� </param>
	/// <param name="PayloadSize"> �V�F�[�_�[�Ŏg�p����y�C���[�h�̃T�C�Y </param>
	/// <param name="AttribSize"> �d�S���W�Ȃǂ̌v�Z�Ɏg�p����\���̂̃T�C�Y �Œ� </param>
	/// <param name="ReflectionCount"> �ő僌�C�ˏo�� </param>
	RayPipeline(const std::vector<RayPipelineShaderData>& InputData, HitGroupMgr::HITGROUP_ID UseHitGroup, int SRVCount, int CBVCount, int UAVCount, int PayloadSize, int AttribSize, int ReflectionCount = 4);

	/// <summary>
	/// �V�F�[�_�[�e�[�u�����\�z�B ���t���[���S�Ă�Draw���I������Ƃ��ɌĂԁB
	/// </summary>
	/// <param name="DispatchX"> ���̉�ʃT�C�Y </param>
	/// <param name="DispatchY"> �c�̉�ʃT�C�Y </param>
	void BuildShaderTable(int DispatchX = 1280, int DispatchY = 720);

	/// <summary>
	/// �V�F�[�_�[�e�[�u�����\�z�B
	/// </summary>
	/// <param name="DispatchX"> ���̉�ʃT�C�Y </param>
	/// <param name="DispatchY"> �c�̉�ʃT�C�Y </param>
	void ConstructionShaderTable(int DispatchX, int DispatchY);

	/// <summary>
	/// HipGroup�̃f�[�^��GPU�ɓ]���B Blas�̃f�[�^�Ȃǂ𑗂�B
	/// </summary>
	void MapHitGroupInfo();

	/// <summary>
	/// ���C�g�����s�B
	/// </summary>
	void TraceRay(std::weak_ptr<RaytracingOutput> Output, std::weak_ptr<RaytracingOutput> GBuffer0, std::weak_ptr<RaytracingOutput> GBuffer1);

	// �e��Q�b�^
	Microsoft::WRL::ComPtr<ID3D12StateObject> GetStateObject() { return stateObject_; }
	D3D12_DISPATCH_RAYS_DESC GetDispatchRayDesc() { return dispatchRayDesc_; }
	std::shared_ptr<RayRootsignature> GetGlobalRootSig() { return globalRootSig_; }

private:

	/// <summary>
	/// �q�b�g�O���[�v�̒��ōł��g�p�f�[�^�T�C�Y���傫���̂��擾����B
	/// </summary>
	/// <returns> �q�b�g�O���[�v�̒��ōł��傫���T�C�Y </returns>
	UINT GetLargestDataSizeInHitGroup();

	/// <summary>
	/// �A���C�������g����B
	/// </summary>
	/// <param name="Size"> �A���C�������g�������T�C�Y�B </param>
	/// <param name="Align"> �A���C�������g </param>
	/// <returns> ���� </returns>
	UINT RoundUp(size_t Size, UINT Align) {
		return UINT(Size + Align - 1) & ~(Align - 1);
	}

	/// <summary>
	/// �V�F�[�_�[�e�[�u�����������݁A���C��ݒ肷��B--------------------------------------------------------------------------�V�F�[�_�[�e�[�u���̏������݂ƃ��C�̐ݒ�̊֐��𕪂���B
	/// </summary>
	/// <param name="DispatchX"></param>
	/// <param name="DispatchY"></param>
	void WriteShadetTalbeAndSettingRay(int DispatchX, int DispatchY);

	/// <summary>
	/// �V�F�[�_�[���ʎq���������ށB
	/// </summary>
	/// <param name="Dst"> �������ݐ� </param>
	/// <param name="ShaderId"> ��������ID </param>
	/// <returns> �������񂾌�̃n���h�� </returns>
	UINT WriteShaderIdentifier(void* Dst, const void* ShaderId);

	/// <summary>
	/// RayGeneration�V�F�[�_�[�̐����擾�B
	/// </summary>
	/// <returns></returns>
	int GetRayGenerationCount();

	/// <summary>
	///  MissShader�̐����擾�B
	/// </summary>
	/// <returns></returns>
	int GetMissCount();

	/// <summary>
	/// �o�b�t�@�𐶐�����֐� �ʓ|�������̂Ŏ��̃G���W�����炻�̂܂܎����Ă��܂����B
	/// </summary>
	/// <param name="Size"> �o�b�t�@�T�C�Y </param>
	/// <param name="Flags"> �o�b�t�@�̃t���O </param>
	/// <param name="InitialState"> �o�b�t�@�̏������ </param>
	/// <param name="HeapType"> �q�[�v�̎�� </param>
	/// <param name="Name"> �o�b�t�@�ɂ��閼�O </param>
	/// <returns> �������ꂽ�o�b�t�@ </returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBuffer(size_t Size, D3D12_RESOURCE_FLAGS Flags, D3D12_RESOURCE_STATES InitialState, D3D12_HEAP_TYPE HeapType, const wchar_t* Name = nullptr);

};