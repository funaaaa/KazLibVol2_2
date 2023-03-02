#include "CreateMeshBuffer.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"
#include"../KazLibrary/Loader/FbxModelResourceMgr.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"

CreateMeshBuffer::CreateMeshBuffer(RESOURCE_HANDLE HANDLE)
{
	if (HANDLE == -1)
	{
		FailCheck("���f�����ǂݍ��܂ꂹ��ł����B");
		assert(0);
	}


	//���_������������--------------------------------------------
	std::vector<DirectX::XMFLOAT3>lVertData = FbxModelResourceMgr::Instance()->GetResourceData(HANDLE)->vertFloat3Data;
	if (lVertData.size() != 0)
	{
		bufferHandleDataArray[DATA_VERT].bufferHandle = computeHelper.CreateBuffer(
			sizeof(DirectX::XMFLOAT3),
			GRAPHICS_RANGE_TYPE_UAV_DESC,
			GRAPHICS_PRAMTYPE_DATA,
			static_cast<BUFFER_SIZE>(lVertData.size()),
			false
		);

		computeHelper.TransData(bufferHandleDataArray[DATA_VERT].bufferHandle, lVertData.data(), KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lVertData.size(), sizeof(DirectX::XMFLOAT3)));

		bufferHandleDataArray[DATA_VERT].descriptorViewHandle = computeHelper.GetBufferData(bufferHandleDataArray[DATA_VERT].bufferHandle).GetViewHandle();
	}
	//���_������������--------------------------------------------

	//UV������������--------------------------------------------
	std::vector<DirectX::XMFLOAT2>lUvData = FbxModelResourceMgr::Instance()->GetResourceData(HANDLE)->uvData;
	if (lUvData.size() != 0 && FbxModelResourceMgr::Instance()->GetResourceData(HANDLE)->textureHandle.size() != 0)
	{
		bufferHandleDataArray[DATA_UV].bufferHandle = computeHelper.CreateBuffer(
			sizeof(DirectX::XMFLOAT2),
			GRAPHICS_RANGE_TYPE_UAV_DESC,
			GRAPHICS_PRAMTYPE_DATA2,
			static_cast<BUFFER_SIZE>(lUvData.size()),
			false
		);
		computeHelper.TransData(bufferHandleDataArray[DATA_UV].bufferHandle, lUvData.data(), KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lUvData.size(), sizeof(DirectX::XMFLOAT2)));
		bufferHandleDataArray[DATA_UV].descriptorViewHandle = computeHelper.GetBufferData(bufferHandleDataArray[DATA_UV].bufferHandle).GetViewHandle();
	}
	//UV������������--------------------------------------------



	//�@��������������--------------------------------------------
	std::vector<DirectX::XMFLOAT3>lNormalData = FbxModelResourceMgr::Instance()->GetResourceData(HANDLE)->normalData;
	if (lNormalData.size() != 0)
	{
		bufferHandleDataArray[DATA_NORMAL].bufferHandle = computeHelper.CreateBuffer(
			sizeof(DirectX::XMFLOAT3),
			GRAPHICS_RANGE_TYPE_UAV_DESC,
			GRAPHICS_PRAMTYPE_DATA3,
			static_cast<BUFFER_SIZE>(lNormalData.size()),
			false
		);

		computeHelper.TransData(bufferHandleDataArray[DATA_NORMAL].bufferHandle, lNormalData.data(), KazBufferHelper::GetBufferSize<BUFFER_SIZE>(lNormalData.size(), sizeof(DirectX::XMFLOAT3)));

		bufferHandleDataArray[DATA_NORMAL].descriptorViewHandle = computeHelper.GetBufferData(bufferHandleDataArray[DATA_NORMAL].bufferHandle).GetViewHandle();
	}
	//�@��������������--------------------------------------------

}

CreateMeshBuffer::CreateMeshBuffer(std::vector<DirectX::XMFLOAT3> VERT, std::vector<DirectX::XMFLOAT2> UV)
{
	//���_������������--------------------------------------------
	if (VERT.size() != 0)
	{
		bufferHandleDataArray[DATA_VERT].bufferHandle = computeHelper.CreateBuffer(
			sizeof(DirectX::XMFLOAT3),
			GRAPHICS_RANGE_TYPE_UAV_DESC,
			GRAPHICS_PRAMTYPE_DATA,
			static_cast<BUFFER_SIZE>(VERT.size()),
			false
		);

		computeHelper.TransData(bufferHandleDataArray[DATA_VERT].bufferHandle, VERT.data(), KazBufferHelper::GetBufferSize<BUFFER_SIZE>(VERT.size(), sizeof(DirectX::XMFLOAT3)));

		bufferHandleDataArray[DATA_VERT].descriptorViewHandle = computeHelper.GetBufferData(bufferHandleDataArray[DATA_VERT].bufferHandle).GetViewHandle();
	}
	//���_������������--------------------------------------------

	//UV������������--------------------------------------------
	if (UV.size() != 0)
	{
		bufferHandleDataArray[DATA_UV].bufferHandle = computeHelper.CreateBuffer(
			sizeof(DirectX::XMFLOAT2),
			GRAPHICS_RANGE_TYPE_UAV_DESC,
			GRAPHICS_PRAMTYPE_DATA2,
			static_cast<BUFFER_SIZE>(UV.size()),
			false
		);
		computeHelper.TransData(bufferHandleDataArray[DATA_UV].bufferHandle, UV.data(), KazBufferHelper::GetBufferSize<BUFFER_SIZE>(UV.size(), sizeof(DirectX::XMFLOAT2)));
		bufferHandleDataArray[DATA_UV].descriptorViewHandle = computeHelper.GetBufferData(bufferHandleDataArray[DATA_UV].bufferHandle).GetViewHandle();
	}
	//UV������������--------------------------------------------
}

const ResouceBufferHelper::BufferData &CreateMeshBuffer::GetBufferData(MeshBufferView ENUM_VIEW)
{
	RESOURCE_HANDLE lHandle = static_cast<RESOURCE_HANDLE>(ENUM_VIEW);
	if (bufferHandleDataArray[lHandle].bufferHandle != -1)
	{
		return computeHelper.GetBufferData(lHandle);
	}
	else
	{
		//��������Ă��Ȃ��o�b�t�@�ɃA�N�Z�X���悤�Ƃ��Ă��܂��B
		assert(0);
		return computeHelper.GetBufferData(0);
	}
}
