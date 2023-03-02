#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../Helper/ISinglton.h"
#include "../Raytracing/RayDescriptorHeap.h"

/// <summary>
/// TopLevel�̉����\���� InstanceVector��o�^���邱�Ƃɂ���ă��C�g����Ԃ��\�z����B
/// </summary>
class Tlas : public ISingleton<Tlas>{

private:

	/*===== �����o�ϐ� =====*/

	Microsoft::WRL::ComPtr<ID3D12Resource> tlasBuffer_;			// Tlas�̃o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> scratchBuffer_;		// Tlas���`������ۂɕK�v�ȃo�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> instanceDescBuffer_;	// �C���X�^���X�o�b�t�@
	void* instanceDescMapAddress_;								// �C���X�^���X�o�b�t�@�Ƀf�[�^�𑗂�ۂ̃A�h���X
	int instanceCapacity_;										// ���݂�Tlas�̃o�b�t�@��Instance���̋��e�ʁB InstanceVector�̗v�f�������̐��𒴂�����S�Ẵo�b�t�@���Đ�������B
	RAY_HEAP_HANDLE descHeapIndex_;								// Tlas��ۑ����Ă���DescriptorHeap��̃C���f�b�N�X


public:

	/*===== �����o�֐� =====*/

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Tlas();

	/// <summary>
	/// TLAS���\�z�B�S�Ă�Draw�R�[�����I������ɌĂ�ł��������B
	/// </summary>
	void Build();

private:

	/// <summary>
	/// Tlas���\�zor�č\�z����֐��B
	/// </summary>
	void ReBuildTlas();

	/// <summary>
	/// Tlas���X�V����֐��B
	/// </summary>
	void UpdateTlas();

	/// <summary>
	/// �A�h���X�ɏ����������ޏ���
	/// </summary>
	/// <param name="MapAddress"> �f�[�^�̑���� </param>
	/// <param name="PData"> ����f�[�^ </param>
	/// <param name="DataSize"> ����f�[�^�̃T�C�Y </param>
	void WriteToMemory(void* MapAddress, const void* PData, size_t DataSize);

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