#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Helper/DirtyFlag.h"
#include"../Render/RenderData.h"
#include"../Loader/FbxModelResourceMgr.h"
#include<memory>

// �{����RESOURCE_HANDLE�ŗǂ��̂����AFbxModelResourceMgr�̐݌v��ς���K�v�����邱�Ƃ��������邽�߂Ɉ�U������g���B
struct FbxModelData;

/// <summary>
/// BottomLevel�̉����\���� Blas�Ƃ��čŒ���K�v�ȋ@�\���܂Ƃ߂��N���X�ŁA�S�Ẵ��f���N���X���{�N���X�����B
/// </summary>
class Blas {

private:

	/*===== �����o�ϐ� =====*/

	Microsoft::WRL::ComPtr<ID3D12Resource> blasBuffer_;		// Blas�̃o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> scratchBuffer_;	// Blas���`������ۂɕK�v�ȃo�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> updateBuffer_;	// Blas�̃A�b�v���[�h�p�o�b�t�@

	bool isRaytracingEnabled;	// ���C�g�����L��������Ă��邩�̃t���O�B�����ɂ���ă��C�g�����J�����O����ۂɈꎞ�I�ɖ�������������A���f�����`�悳��Ȃ��Ȃ����ۂ�false�ɂ���B


public:

	/*===== �����o�֐� =====*/

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="IsOpaque"> �s�����t���O t:�s���� f:�������ɂȂ�\�������� </param>
	/// <param name="ModelDataHandle"> Blas���`�����郂�f���̌`���ۑ����Ă���f�[�^�̃n���h�� </param>
	Blas(bool IsOpaque, const FbxModelData& ModelDataHandle);

	/// <summary>
	/// Blas�̃o�b�t�@��Ԃ��B
	/// </summary>
	/// <returns> Blas�̃o�b�t�@ </returns>
	inline Microsoft::WRL::ComPtr<ID3D12Resource> GetBlasBuffer() { return blasBuffer_; }

	/// <summary> 
	/// �Q�ƌ��̃V�F�[�_�[���R�[�h�ɃA�N�Z�X���ď������ށB
	/// </summary>
	/// <param name="Dst"> �������ݐ�|�C���^ </param>
	/// <param name="RecordSize"> �������ރT�C�Y </param>
	/// <param name="StateObject"> �������ރI�u�W�F�N�g </param>
	/// <param name="HitGroupName"> �g�p����HitGroup�̖��O </param>
	/// <returns> �������݌�̃|�C���^ </returns>
	uint8_t* WriteShaderRecord(uint8_t* Dst, UINT RecordSize, Microsoft::WRL::ComPtr<ID3D12StateObject>& StateObject, LPCWSTR HitGroupName);

private:

	/// <summary>
	/// Blas�̌`��ݒ�ɕK�v�ȍ\���̂�Ԃ��B
	/// </summary>
	/// <param name="IsOpaque"> �s�����t���O t:�s���� f:�������ɂȂ�\�������� </param>
	/// <param name="ModelDataHandle"> Blas���`�����郂�f���̌`���ۑ����Ă���f�[�^�̃n���h�� </param>
	/// <returns></returns>
	D3D12_RAYTRACING_GEOMETRY_DESC GetGeometryDesc(bool IsOpaque, const FbxModelData& ModelDataHandle);

	/// <summary>
	/// Blas���\�z
	/// </summary>
	/// <param name="GeomDesc"> Blas�̌`��\���́B���N���X�̊֐�����擾����B </param>
	void BuildBlas(const D3D12_RAYTRACING_GEOMETRY_DESC& GeomDesc);

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