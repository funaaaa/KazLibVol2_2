#include"../Loader/TextureResourceMgr.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"
#include"../Helper/KazHelper.h"
#include"../Buffer/DescriptorHeapMgr.h"
#include"../Helper/KazRenderHelper.h"

const int texWidth = 256;
const int texDataCount = texWidth * texWidth;

TextureResourceMgr::TextureResourceMgr()
{
	handle = 0;
	setHandle = 0;
	IncreSize = 0;
	result = S_FALSE;

	IncreSize = DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	buffers.reset(new CreateGpuBuffer);
	buffers->handle->SetHandleSize(DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_TEXTURE_SPRITE));
}

TextureResourceMgr::~TextureResourceMgr()
{
}

void TextureResourceMgr::Release(RESOURCE_HANDLE HANDLE)
{
	handleName[HANDLE] = "";
	buffers->ReleaseBuffer(HANDLE);
	DescriptorHeapMgr::Instance()->Release(HANDLE);
}

RESOURCE_HANDLE TextureResourceMgr::LoadGraph(std::string RESOURCE)
{

	//���ɍ���Ă���o�b�t�@�Ɩ��O��������������n���h����Ԃ��A���Ȃ������琶��
	for (int i = 0; i < handleName.size(); i++)
	{
		if (handleName[i] == RESOURCE)
		{
			return i;
		}
	}

	//string�^����wchar_t�^�ɕϊ�
	wchar_t wfilepat[128];
	wchar_t wfilepat2[128];
	MultiByteToWideChar(CP_ACP, 0, RESOURCE.c_str(), -1, wfilepat, _countof(wfilepat));

	
	KazHelper::ConvertStringToWchar_t(RESOURCE, wfilepat2, 128);

	//�摜���[�h
	HRESULT re;
	re = LoadFromWICFile(
		wfilepat,
		DirectX::WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	img = scratchImg.GetImage(0, 0, 0);


	//�ǂݍ��߂Ȃ�������G���[��f��
	if (CheckResult(re, RESOURCE + "�̃t�@�C���ǂݍ��݂ɐ������܂���\n", RESOURCE + "�̃t�@�C���ǂݍ��݂Ɏ��s���܂���\n") != S_OK)
	{
		return -1;
	}


	//�e�N�X�`���̖��O�o�^
	handleName.push_back(RESOURCE);


	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);


	//�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);



	RESOURCE_HANDLE num = buffers->CreateBuffer(KazBufferHelper::SetShaderResourceBufferData(textureDesc));

	DescriptorHeapMgr::Instance()->CreateBufferView(num, srvDesc, buffers->GetBufferData(num).Get());

	buffers->GetBufferData(num)->WriteToSubresource
	(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);


	return num;
}

RESOURCE_HANDLE TextureResourceMgr::LoadDivGraph(string RESOURCE, int DIV_WIDTH_NUM, int DIV_HEIGHT_NUM, int TEXSIZE_WIDTH, int TEXSIZE_HEIGHT)
{
	//���ɍ���Ă���o�b�t�@�Ɩ��O��������������n���h����Ԃ��A���Ȃ������琶��
	for (int i = 0; i < handleName.size(); i++)
	{
		if (handleName[i] == RESOURCE)
		{
			return static_cast<RESOURCE_HANDLE>(i);
		}
	}

	//string�^����wchar_t�^�ɕϊ�
	wchar_t wfilepat[128];
	MultiByteToWideChar(CP_ACP, 0, RESOURCE.c_str(), -1, wfilepat, _countof(wfilepat));


	//�摜���[�h
	HRESULT re;
	re = LoadFromWICFile(
		wfilepat,
		DirectX::WIC_FLAGS_NONE,
		&metadata,
		scratchImg
	);

	img = scratchImg.GetImage(0, 0, 0);


	//�ǂݍ��߂Ȃ�������G���[��f��
	if (CheckResult(re, RESOURCE + "�̃t�@�C���ǂݍ��݂ɐ������܂���\n", RESOURCE + "�̃t�@�C���ǂݍ��݂Ɏ��s���܂���\n") != S_OK)
	{
		return -1;
	}
	//�e�N�X�`���̖��O�o�^
	handleName.push_back(RESOURCE);

	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);


	//�r���[����
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;



	RESOURCE_HANDLE lHandle = buffers->CreateBuffer(KazBufferHelper::SetShaderResourceBufferData(textureDesc));

	DescriptorHeapMgr::Instance()->CreateBufferView(lHandle, srvDesc, buffers->GetBufferData(lHandle).Get());

	buffers->GetBufferData(lHandle)->WriteToSubresource
	(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);

	//�摜�؂���̏��
	divData.push_back({});
	int arrayNum = static_cast<int>(divData.size()) - 1;
	divData[arrayNum].divSize = { TEXSIZE_WIDTH,TEXSIZE_HEIGHT };
	divData[arrayNum].handle = lHandle;

	for (int y = 0; y < DIV_HEIGHT_NUM; y++)
	{
		for (int x = 0; x < DIV_WIDTH_NUM; x++)
		{
			//uv�؂���ɕK�v�ȍ���̓_
			divData[arrayNum].divLeftUp.push_back({ 0 + TEXSIZE_WIDTH * x,0 + TEXSIZE_HEIGHT * y });
		}
	}

	return lHandle;
}

D3D12_RESOURCE_DESC TextureResourceMgr::GetTextureSize(RESOURCE_HANDLE HANDLE)
{
	//�n���h����-1�A��������texBuff��nullptr�������ꍇ�A0��Width��Height��Ԃ�
	if (HANDLE != -1)
	{
		if (buffers->GetBufferData(HANDLE) != nullptr)
		{
			return buffers->GetBufferData(HANDLE)->GetDesc();
		}
		else
		{
			D3D12_RESOURCE_DESC fail;
			fail.Width = 0;
			fail.Height = 0;
			return fail;
		}
	}
	else
	{
		D3D12_RESOURCE_DESC fail;
		fail.Width = 0;
		fail.Height = 0;
		return fail;
	}
}

DivGraphData TextureResourceMgr::GetDivData(RESOURCE_HANDLE HANDLE)
{
	if (HANDLE != -1)
	{
		for (int i = 0; i < divData.size(); i++)
		{
			if (divData[i].handle == HANDLE)
			{
				//����
				return divData[i];
			}
		}	
	}	

	//���s
	DivGraphData no;
	no.divSize = { -1,-1 };
	return no;		
}

void TextureResourceMgr::SetSRV(RESOURCE_HANDLE GRAPH_HANDLE, GraphicsRootSignatureParameter PARAM, GraphicsRootParamType TYPE)
{
	//�e�N�X�`���o�b�t�@�̃o�b�t�@���Ȃ������R�}���h���X�g�ɐς܂��Ă��G���[�͓f����Ȃ��B��������Ȃ���...
	if (GRAPH_HANDLE != -1)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = DescriptorHeapMgr::Instance()->GetGpuDescriptorView(GRAPH_HANDLE);
		int param = KazRenderHelper::SetBufferOnCmdList(PARAM, GRAPHICS_RANGE_TYPE_SRV_DESC, TYPE);
		DirectX12CmdList::Instance()->cmdList->SetGraphicsRootDescriptorTable(param, gpuDescHandleSRV);
	}
}



//�e�N�X�`���o�b�t�@�����A�⏕�Ȃ�
////�e�N�X�`���q�[�v�ݒ�
//D3D12_HEAP_PROPERTIES texHeapProp{};
//texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

////���\�[�X�ݒ� ��ŕύX�����
//D3D12_RESOURCE_DESC textureDesc{};
//textureDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
//textureDesc.Format = metadata.format;
//textureDesc.Width = metadata.width;
//textureDesc.Height = (UINT)metadata.height;
//textureDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
//textureDesc.MipLevels = (UINT16)metadata.mipLevels;
//textureDesc.SampleDesc.Count = 1;


////�e�N�X�`���̃o�b�t�@����
//HRESULT res;
//res = DirectX12Device::dev->CreateCommittedResource(
//	&texHeapProp,
//	D3D12_HEAP_FLAG_NONE,
//	&textureDesc,
//	D3D12_RESOURCE_STATE_GENERIC_READ,
//	nullptr,
//	IID_PPV_ARGS(&texBuff[handle])
//);