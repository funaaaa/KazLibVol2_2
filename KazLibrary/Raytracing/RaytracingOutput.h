#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "../KazLibrary/Math/KazMath.h"
#include "../Raytracing/RayDescriptorHeap.h"

// ���C�g���o��UAV�N���X ���C�g���֐�����̏o�͌��ʂ�A�f�m�C�Y��̏o�͌��ʂ̕ۑ��p�Ɏg�p����B
class RaytracingOutput {

private:

	/*===== �����o�ϐ� =====*/

	Microsoft::WRL::ComPtr<ID3D12Resource> rayTracingOutput_;	// UAV
	RAY_HEAP_HANDLE uavDescriptorIndex_;									// �f�B�X�N���v�^�q�[�v��̂���UAV�̃C���f�b�N�X


public:

	/*===== �����o�֐� =====*/

	// �Z�b�e�B���O
	void Setting(DXGI_FORMAT Format, LPCWSTR BufferName, KazMath::Vec2<int> TextureSize = KazMath::Vec2<int>(1280, 720), D3D12_RESOURCE_STATES ResourceState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);

	// �f�B�X�N���v�^���Z�b�g�B
	void SetComputeRootDescriptorTalbe(int RootParamIndex);

	// ���\�[�X�o���A
	void SetResourceBarrier(D3D12_RESOURCE_STATES Dst, D3D12_RESOURCE_STATES Src);

	// �Q�b�^
	Microsoft::WRL::ComPtr<ID3D12Resource> GetRaytracingOutput() { return rayTracingOutput_; }
	RAY_HEAP_HANDLE GetUAVIndex() { return uavDescriptorIndex_; }

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTexture2D(UINT Width, UINT Height, DXGI_FORMAT Format, D3D12_RESOURCE_FLAGS Flags, D3D12_RESOURCE_STATES IinitialState, D3D12_HEAP_TYPE HeapType);

};