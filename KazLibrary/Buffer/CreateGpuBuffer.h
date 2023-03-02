#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/HandleMaker.h"
#include"../Helper/KazBufferHelper.h"

/// <summary>
/// �o�b�t�@�ƃf�B�X�N���v�^�q�[�v�𐶐�����N���X�ł�
/// </summary>
class CreateGpuBuffer
{
public:	
	CreateGpuBuffer();
	~CreateGpuBuffer();

	/// <summary>
	/// �f�X�N���v�^�q�[�v�𐶐����܂�
	/// </summary>
	/// <param name="DESCRIPTOR_NUM_MAX">��������o�b�t�@�̍ő吔</param>
	void CreateDescriptorHeap(unsigned int DESCRIPTOR_NUM_MAX);

	/// <summary>
	/// �o�b�t�@�𐶐�����
	/// </summary>
	/// <param name="HEAP_PROPERTIES">�v���p�e�B</param>
	/// <param name="RESOURCE"></param>
	/// <param name="RESOURCE_STATE"></param>
	/// <param name="CLEAR_VALUE"></param>
	/// <returns>�n���h��</returns>
	RESOURCE_HANDLE CreateBuffer(D3D12_HEAP_PROPERTIES HEAP_PROPERTIES, D3D12_RESOURCE_DESC RESOURCE, D3D12_RESOURCE_STATES RESOURCE_STATE, D3D12_CLEAR_VALUE *CLEAR_VALUE = nullptr);


	RESOURCE_HANDLE CreateBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION);

	/// <summary>
	/// �f�[�^���o�b�t�@�ɓ]�����܂�
	/// </summary>
	/// <param name="HANDLE">�o�b�t�@�̃n���h��</param>
	/// <param name="DATA">���肽���f�[�^�̃A�h���X</param>
	/// <param name="DATA_SIZE">���肽���f�[�^�̃T�C�Y</param>
	void TransData(RESOURCE_HANDLE HANDLE, void *DATA, const unsigned int &DATA_SIZE);

	/// <summary>
	/// �萔�o�b�t�@�r���[�𐶐����܂�
	/// </summary>
	/// <param name="HANDLE">�����������o�b�t�@</param>
	/// <param name="VIEW">�r���[�̐ݒ�</param>
	void CreateBufferView(RESOURCE_HANDLE HANDLE, D3D12_CONSTANT_BUFFER_VIEW_DESC VIEW);

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[�𐶐����܂�
	/// </summary>
	/// <param name="HANDLE">�����������o�b�t�@</param>
	/// <param name="VIEW">�r���[�̐ݒ�</param>
	void CreateBufferView(RESOURCE_HANDLE HANDLE, D3D12_SHADER_RESOURCE_VIEW_DESC VIEW);

	/// <summary>
	/// �o�b�t�@���J�����܂�
	/// </summary>
	/// <param name="HANDLE">�J���������o�b�t�@�̃n���h��</param>
	void ReleaseBuffer(RESOURCE_HANDLE HANDLE);

	D3D12_GPU_DESCRIPTOR_HANDLE GetViewPointer(RESOURCE_HANDLE HANDLE);

	/// <summary>
	/// �o�b�t�@���󂯎��܂�
	/// </summary>
	/// <param name="HANDLE">�󂯎�肽���o�b�t�@�̃n���h��</param>
	/// <returns>�������ꂽ�o�b�t�@</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource>GetBufferData(RESOURCE_HANDLE HANDLE);


	/// <summary>
	/// �o�b�t�@��GPU�A�h���X���󂯎��܂�
	/// </summary>
	/// <param name="HANDLE">�n���h��</param>
	/// <returns>�o�b�t�@��GPU�A�h���X</returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuAddress(RESOURCE_HANDLE HANDLE);


	void *GetMapAddres(RESOURCE_HANDLE HANDLE);


	std::unique_ptr<HandleMaker> handle;
private:	
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 1000> buffers;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap;

	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	

	UINT increSize;
};

