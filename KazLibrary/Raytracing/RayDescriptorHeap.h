#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../Helper/ISinglton.h"

typedef int RAY_HEAP_HANDLE;	// ���C�g���p�̃f�B�X�N���v�^�q�[�v�N���X�Ŏg�p����n���h��

/// <summary>
/// ���C�g���p�̃f�B�X�N���v�^�q�[�v�N���X DescriptorHeapMgr����Ɛ݌v�����킹��Ǝ��Ԃ������肻���������̂ň�U�����ɁB
/// </summary>
class RayDescriptorHeap : public ISingleton<RayDescriptorHeap> {

private:

	/*===== �����o�ϐ� =====*/

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;	// ���C�g���p�̃f�B�X�N���v�^�q�[�v
	RAY_HEAP_HANDLE head_;								// ���݂̐擪

public:

	/// <summary>
	/// ��������o�b�t�@�̎��
	/// </summary>
	enum class BUFFER_TYPE {
		//SRV,	// �e�N�X�`���n
		//CBV,	// �萔�o�b�t�@�n
		UAV_TEXTURE,	// UAV�e�N�X�`���n
		AS,		// �����\����
	};


public:

	/*===== �����o�֐� =====*/

	/// <summary>
	/// ���O��������
	/// </summary>
	void Setting();

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�̍Ō���Ƀo�b�t�@�𐶐��B
	/// </summary>
	/// <param name="Type"> �o�b�t�@�̌`�� </param>
	/// <param name="Buffer"> ��������o�b�t�@�̃f�[�^ </param>
	/// <returns> �o�b�t�@�𐶐������ʒu </returns>
	RAY_HEAP_HANDLE AllocateBuffer(BUFFER_TYPE Type, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer = nullptr);

	/// <summary>
	/// �w�肵���n���h���̈ʒu�Ƀo�b�t�@�𐶐� ��ɍ쐬�ς݂̃o�b�t�@���㏑������`�Ŏg�p���܂��B �{�����̂悤�Ȏg�����͂悭�Ȃ��Ǝv���܂����A���u���Ƃ������Ƃł��̂܂܎����B
	/// </summary>
	/// <param name="Handle"> �o�b�t�@�𐶐�����ʒu </param>
	/// <param name="Type"> �o�b�t�@�̌`�� </param>
	/// <param name="Buffer"> ��������o�b�t�@�̃f�[�^ </param>
	void AllocateBufferForHandle(RAY_HEAP_HANDLE Handle, BUFFER_TYPE Type, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer = nullptr);

	/// <summary>
	/// �w��̃n���h����GPU�n���h�����擾����B
	/// </summary>
	/// <param name="Handle"> GPU�n���h�����擾�������o�b�t�@�̃n���h�� </param>
	/// <returns> GPU�n���h�� </returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(RAY_HEAP_HANDLE Handle);

private:

	/// <summary>
	/// �����\���̂̃o�b�t�@�𐶐�����B
	/// </summary>
	/// <param name="Handle"> �o�b�t�@�𐶐�����ʒu </param>
	/// <param name="Buffer"> ��������o�b�t�@�̃f�[�^ </param>
	void AllocateASBuffer(RAY_HEAP_HANDLE Handle, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer);

	/// <summary>
	/// UAV�e�N�X�`���p�̃o�b�t�@�𐶐�����B
	/// </summary>
	/// <param name="Handle"> �o�b�t�@�𐶐�����ʒu </param>
	/// <param name="Buffer"> ��������o�b�t�@�̃f�[�^ </param>
	void AllocateUAVTextureBuffer(RAY_HEAP_HANDLE Handle, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer);

};