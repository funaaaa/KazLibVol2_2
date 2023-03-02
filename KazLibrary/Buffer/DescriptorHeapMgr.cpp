#include "DescriptorHeapMgr.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"

DescriptorHeapMgr::DescriptorHeapMgr()
{
	//4096���ő�̃o�b�t�@�T�C�Y�Ƃ��A256���̈��n���悤�ɂ���
	short heapMemoryMax = DESCRIPTOR_HEAP_SIZE_MAX / DESCRIPTORHEAP_MEMORY_MAX;

	for (short i = 0; i < DESCRIPTORHEAP_MEMORY_MAX; i++)
	{
		heapMaxSizes[i].startSize = heapMemoryMax * i;
		heapMaxSizes[i].endSize = heapMemoryMax * (i + 1);

		if (1 <= i)
		{
			++heapMaxSizes[i].startSize;
		}
	}

	for (int i = 0; i < DESCRIPTOR_HEAP_SIZE_MAX; i++)
	{
		usedHandle[i] = false;
	}


	//�C���N�������g�̃T�C�Y
	shaderResourceHeapIncreSize = DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	rtvHeapIncreSize = DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	{
		//�f�X�N���v�^�q�[�v�쐬
		D3D12_DESCRIPTOR_HEAP_DESC srbDescHeap{};
		srbDescHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		//�V�F�[�_�[���猩����
		srbDescHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		//�o�b�t�@�̐�
		srbDescHeap.NumDescriptors = DESCRIPTOR_HEAP_SIZE_MAX;
		DirectX12Device::Instance()->dev->CreateDescriptorHeap(&srbDescHeap, IID_PPV_ARGS(&heaps));
	}
}

DescriptorHeapMgr::~DescriptorHeapMgr()
{
}

void DescriptorHeapMgr::CreateBufferView(RESOURCE_HANDLE HANDLE, const D3D12_CONSTANT_BUFFER_VIEW_DESC &BUFFER_VIEW)
{
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	if (isSafeToUseThisHandle(HANDLE))
	{
		heapHandle = heaps->GetCPUDescriptorHandleForHeapStart();
		heapHandle.ptr += shaderResourceHeapIncreSize * static_cast<UINT64>(HANDLE);
		DirectX12Device::Instance()->dev->CreateConstantBufferView(&BUFFER_VIEW, heapHandle);
		usedHandle[HANDLE] = true;
	}
	else
	{
		ErrorCheck("���Ɏg��ꂽ�A�������̓f�X�N���v�^�q�[�v�͈̔͊O�̏ꏊ�ŃV�F�[�_�[���\�[�X�r���[���쐬���悤�Ƃ��Ă��܂�");
	}
}

void DescriptorHeapMgr::CreateBufferView(RESOURCE_HANDLE HANDLE, const D3D12_SHADER_RESOURCE_VIEW_DESC &BUFFER_VIEW, ID3D12Resource *RESOURCE)
{
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	if (isSafeToUseThisHandle(HANDLE))
	{
		heapHandle = heaps->GetCPUDescriptorHandleForHeapStart();
		heapHandle.ptr += shaderResourceHeapIncreSize * static_cast<UINT64>(HANDLE);
		DirectX12Device::Instance()->dev->CreateShaderResourceView(RESOURCE, &BUFFER_VIEW, heapHandle);
		usedHandle[HANDLE] = true;
	}
	else
	{
		ErrorCheck("���Ɏg��ꂽ�A�������̓f�X�N���v�^�q�[�v�͈̔͊O�̏ꏊ�ŃV�F�[�_�[���\�[�X�r���[���쐬���悤�Ƃ��Ă��܂�");
	}
}

void DescriptorHeapMgr::CreateBufferView(RESOURCE_HANDLE HANDLE, const D3D12_UNORDERED_ACCESS_VIEW_DESC &BUFFER_VIEW, ID3D12Resource *ADDRESS, ID3D12Resource *COUNTER_RESOURCE)
{
	D3D12_CPU_DESCRIPTOR_HANDLE heapHandle;
	if (isSafeToUseThisHandle(HANDLE))
	{
		heapHandle = heaps->GetCPUDescriptorHandleForHeapStart();
		heapHandle.ptr += shaderResourceHeapIncreSize * static_cast<UINT64>(HANDLE);
		DirectX12Device::Instance()->dev->CreateUnorderedAccessView(ADDRESS, COUNTER_RESOURCE, &BUFFER_VIEW, heapHandle);
		usedHandle[HANDLE] = true;
	}
	else
	{
		ErrorCheck("���Ɏg��ꂽ�A�������̓f�X�N���v�^�q�[�v�͈̔͊O�̏ꏊ�ŃA���I�[�_�[���\�[�X�r���[���쐬���悤�Ƃ��Ă��܂�");
	}
}

void DescriptorHeapMgr::Release(RESOURCE_HANDLE HANDLE)
{
	if (isSafeToReadThisHandle(HANDLE))
	{
		usedHandle[HANDLE] = false;
	}
}

const D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapMgr::GetGpuDescriptorView(RESOURCE_HANDLE HANDLE)
{
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
	if (isSafeToReadThisHandle(HANDLE))
	{
		gpuDescHandle = heaps->GetGPUDescriptorHandleForHeapStart();
		gpuDescHandle.ptr += shaderResourceHeapIncreSize * HANDLE;
		return gpuDescHandle;
	}
	else
	{
		gpuDescHandle.ptr = UINT64(-1);
		ErrorCheck("�쐬����Ă��Ȃ��r���[����낤�Ƃ��Ă��܂�");
		return gpuDescHandle;
	}
}

const D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapMgr::GetCpuDescriptorView(RESOURCE_HANDLE HANDLE)
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandle;
	if (isSafeToReadThisHandle(HANDLE))
	{
		cpuDescHandle = heaps->GetCPUDescriptorHandleForHeapStart();
		cpuDescHandle.ptr += shaderResourceHeapIncreSize *HANDLE;
		return cpuDescHandle;
	}
	else
	{
		cpuDescHandle.ptr = UINT64(-1);
		ErrorCheck("�쐬����Ă��Ȃ��r���[����낤�Ƃ��Ă��܂�");
		return cpuDescHandle;
	}
}

BufferMemorySize DescriptorHeapMgr::GetSize(const BufferMemory &TYPE)const
{
	if (TYPE != DESCRIPTORHEAP_MEMORY_MAX)
	{
		return heapMaxSizes[TYPE];
	}
	else
	{
		return BufferMemorySize();
	}
};

void DescriptorHeapMgr::SetDescriptorHeap()
{
	ID3D12DescriptorHeap *ppHeap[] = { heaps.Get() };
	DirectX12CmdList::Instance()->cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);
}

int DescriptorHeapMgr::GetType(RESOURCE_HANDLE HANDLE)
{
	for (int i = 0; i < heapMaxSizes.size(); i++)
	{
		if (heapMaxSizes[i].startSize <= HANDLE && HANDLE <= heapMaxSizes[i].endSize)
		{
			return i;
		}
	}
	return -1;
}
