#include "RayDescriptorHeap.h"
#include "../DirectXCommon/DirectX12Device.h"

void RayDescriptorHeap::Setting()
{

	/*===== ���O�������� =====*/

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 64;

	// �f�B�X�N���v�^�q�[�v�̐����B
	DirectX12Device::Instance()->raytracingDevice->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&heap_));
	heap_->SetName(L"RaytracingDescriptorHeap");

	head_ = 0;

}

RAY_HEAP_HANDLE RayDescriptorHeap::AllocateBuffer(BUFFER_TYPE Type, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer)
{

	/*===== �o�b�t�@�𐶐����� =====*/

	// �擪�n���h�����w�肵�ăo�b�t�@�𐶐�����B
	AllocateBufferForHandle(head_, Type, Buffer);

	// �o�b�t�@�̐擪���X�V�B
	++head_;

	return head_;

}

void RayDescriptorHeap::AllocateBufferForHandle(RAY_HEAP_HANDLE Handle, BUFFER_TYPE Type, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer)
{

	/*===== �n���h�����w�肵�ăo�b�t�@�𐶐����� =====*/

	switch (Type)
	{
	case RayDescriptorHeap::BUFFER_TYPE::AS:

		/*-- �����\���̂������� --*/

		// �擪�Ƀo�b�t�@�𐶐�����B
		AllocateASBuffer(head_, Buffer);

		break;
	case RayDescriptorHeap::BUFFER_TYPE::UAV_TEXTURE:

		/*-- �����\���̂������� --*/

		// �擪�Ƀo�b�t�@�𐶐�����B
		AllocateUAVTextureBuffer(head_, Buffer);

		break;
	default:
		break;
	}

}

D3D12_GPU_DESCRIPTOR_HANDLE RayDescriptorHeap::GetGPUDescriptorHandle(RAY_HEAP_HANDLE Handle)
{

	/*===== �w��̃n���h����GPU�n���h�����擾���� =====*/

	return CD3DX12_GPU_DESCRIPTOR_HANDLE(heap_.Get()->GetGPUDescriptorHandleForHeapStart(), Handle, DirectX12Device::Instance()->raytracingDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	
}

void RayDescriptorHeap::AllocateASBuffer(RAY_HEAP_HANDLE Handle, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer)
{

	/*===== �����\���̂̃o�b�t�@�𐶐����� =====*/

	// �f�B�X�N���v�^�q�[�v�ɉ����\���̗p��SRV���m��
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.RaytracingAccelerationStructure.Location = Buffer->GetGPUVirtualAddress();
	CD3DX12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		heap_.Get()->GetCPUDescriptorHandleForHeapStart(), Handle, DirectX12Device::Instance()->raytracingDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	DirectX12Device::Instance()->raytracingDevice->CreateShaderResourceView(nullptr, &srvDesc,
		basicHeapHandle);

}

void RayDescriptorHeap::AllocateUAVTextureBuffer(RAY_HEAP_HANDLE Handle, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer)
{

	/*===== UAV�e�N�X�`���p�̃o�b�t�@�𐶐����� =====*/

	// �擪�n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		heap_.Get()->GetCPUDescriptorHandleForHeapStart(), Handle, DirectX12Device::Instance()->raytracingDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	// �f�B�X�N���v�^�q�[�v��UAV���m��
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	DirectX12Device::Instance()->raytracingDevice->CreateUnorderedAccessView(
		Buffer.Get(), nullptr, &uavDesc, basicHeapHandle);

}
