#include "ParticleWall.h"
#include"../KazLibrary/DirectXCommon/DirectX12Device.h"
#include"../KazLibrary/DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Pipeline/GraphicsRootSignature.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"

ParticleWall::ParticleWall()
{
	buffers = std::make_unique<CreateGpuBuffer>();

	//�R�}���h�V�O�l�`��---------------------------
	std::array<D3D12_INDIRECT_ARGUMENT_DESC, 2> args{};
	args[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
	args[0].UnorderedAccessView.RootParameterIndex = 0;
	args[1].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

	D3D12_COMMAND_SIGNATURE_DESC desc{};
	desc.pArgumentDescs = args.data();
	desc.NumArgumentDescs = static_cast<UINT>(args.size());
	desc.ByteStride = sizeof(IndirectCommand);

	HRESULT lR =
		DirectX12Device::Instance()->dev->CreateCommandSignature(&desc, GraphicsRootSignature::Instance()->GetRootSignature(ROOTSIGNATURE_DATA_DRAW_UAV).Get(), IID_PPV_ARGS(&commandSig));
	//�R�}���h�V�O�l�`��---------------------------
	if (lR != S_OK)
	{
		assert(0);
	}

	//static const int VERT_MAX_NUM = 200;
	const float lSize = 10.0f;
	Vertex lVertices[] =
	{
		//x,y,z	�@��	u,v
	//�O
	{{-lSize, -lSize, -lSize},{}, {0.0f,1.0f}},		//����
	{{-lSize,  lSize, -lSize},{}, {0.0f,0.0f}},		//����
	{{ lSize, -lSize, -lSize}, {}, {1.0f,1.0f}},		//�E��
	{{ lSize,  lSize, -lSize}, {}, {1.0f,0.0f}},		//�E��

	//��
	{{-lSize,-lSize,  lSize}, {}, {0.0f,1.0f}},		//����
	{{-lSize, lSize,  lSize}, {},{0.0f,0.0f}},		//����
	{{ lSize,-lSize,  lSize},{},{1.0f,1.0f}},		//�E��
	{{ lSize, lSize,  lSize},{}, {1.0f,0.0f}},		//�E��

	//��
	{{-lSize,-lSize, -lSize},{}, {0.0f,1.0f}},		//����
	{{-lSize,-lSize,  lSize}, {}, {0.0f,0.0f}},		//����
	{{-lSize, lSize, -lSize}, {}, {1.0f,1.0f}},		//�E��
	{{-lSize, lSize,  lSize}, {}, {1.0f,0.0f}},		//�E��

	//�E
	{{lSize,-lSize, -lSize},{}, {0.0f,1.0f}},		//����
	{{lSize,-lSize,  lSize}, {}, {0.0f,0.0f}},		//����
	{{lSize, lSize, -lSize}, {}, {1.0f,1.0f}},		//�E��
	{{lSize, lSize,  lSize}, {}, {1.0f,0.0f}},		//�E��

	//��
	{{ lSize, -lSize, lSize}, {}, {0.0f,1.0f}},		//����
	{{ lSize, -lSize,-lSize}, {}, {0.0f,0.0f}},		//����
	{{-lSize, -lSize, lSize}, {}, {1.0f,1.0f}},		//�E��
	{{-lSize, -lSize,-lSize}, {}, {1.0f,0.0f}},	//�E��

	//��
	{{ lSize, lSize, lSize}, {}, {0.0f,1.0f}},			//����
	{{ lSize, lSize,-lSize}, {}, {0.0f,0.0f}},			//����
	{{-lSize, lSize, lSize}, {}, {1.0f,1.0f}},			//�E��
	{{-lSize, lSize,-lSize}, {}, {1.0f,0.0f}}			//�E��
	};
	unsigned short lIndices[] =
	{
		//�O
		0,1,2,		//�O�p�`1��
		2,1,3,		//�O�p�`2��
		//��
		4,6,5,		//�O�p�`3��
		6,7,5,		//�O�p�`4��
		//��
		8,9,10,	//�O�p�`1��
		10,9,11,	//�O�p�`2��
		//�E
		12,14,13,	//�O�p�`1��
		13,14,15,//�O�p�`2��
		//��
		16,18,17,	//�O�p�`1��
		17,18,19,	//�O�p�`2��
		//��
		20,21,22,	//�O�p�`3��
		22,21,23	//�O�p�`4��
	};

	const int FRONT_LEFT_UP = 0;
	const int FRONT_LEFT_DOWN = 1;
	const int FRONT_RIGHT_UP = 2;
	const int FRONT_RIGHT_DOWN = 3;
	const int BACK_LEFT_UP = 4;
	const int BACK_LEFT_DOWN = 5;
	const int BACK_RIGHT_UP = 6;
	const int BACK_RIGHT_DOWN = 7;

	CommonData lConstBufferData;
	//���ʍ���
	lConstBufferData.vertices[FRONT_LEFT_UP] = { lVertices[1].pos.x, lVertices[1].pos.y, lVertices[1].pos.z,0.0f };
	//���ʍ���
	lConstBufferData.vertices[FRONT_LEFT_DOWN] = { lVertices[0].pos.x, lVertices[0].pos.y, lVertices[0].pos.z,0.0f };
	//���ʉE��
	lConstBufferData.vertices[FRONT_RIGHT_UP] = { lVertices[3].pos.x, lVertices[3].pos.y, lVertices[3].pos.z,0.0f };
	//���ʉE��
	lConstBufferData.vertices[FRONT_RIGHT_DOWN] = { lVertices[2].pos.x, lVertices[2].pos.y, lVertices[2].pos.z,0.0f };

	//��ʍ���
	lConstBufferData.vertices[BACK_LEFT_UP] = { lVertices[5].pos.x, lVertices[5].pos.y, lVertices[5].pos.z,0.0f };
	//��ʍ���
	lConstBufferData.vertices[BACK_LEFT_DOWN] = { lVertices[4].pos.x, lVertices[4].pos.y, lVertices[4].pos.z,0.0f };
	//��ʉE��
	lConstBufferData.vertices[BACK_RIGHT_UP] = { lVertices[7].pos.x, lVertices[7].pos.y, lVertices[7].pos.z,0.0f };
	//��ʉE��
	lConstBufferData.vertices[BACK_RIGHT_DOWN] = { lVertices[6].pos.x, lVertices[6].pos.y, lVertices[6].pos.z, 0.0f };


	std::array<Vertex, 4>vertices;
	std::array<USHORT, 6> indices;
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, { 0.5f,0.5f });
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);

	BUFFER_SIZE vertBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(vertices.size(), sizeof(Vertex));
	BUFFER_SIZE indexBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(indices.size(), sizeof(unsigned int));

	//�o�b�t�@����-------------------------
	vertexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetVertexBufferData(vertBuffSize));
	indexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetIndexBufferData(indexBuffSize));

	commonInitBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(CommonData)));
	commonBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(CommonMoveData)));
	particleDataHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(ParticleData) * PARTICLE_MAX_NUM));
	outputInitBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(OutputInitData) * PER_USE_PARTICLE_MAX_NUM) * PARTICLE_MAX_NUM));
	baseOutputInitBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(OutputInitData) * PER_USE_PARTICLE_MAX_NUM) * PARTICLE_MAX_NUM));
	outputBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(OutputData) * PER_USE_PARTICLE_MAX_NUM) * PARTICLE_MAX_NUM));
	drawCommandHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(IndirectCommand) * DRAW_CALL));
	//counterBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(UINT)));
	//�o�b�t�@����-------------------------

	//�]��-------------------------
	buffers->TransData(vertexBufferHandle, vertices.data(), vertBuffSize);
	buffers->TransData(indexBufferHandle, indices.data(), indexBuffSize);
	//UINT reset = 0;
	//buffers->TransData(counterBufferHandle, &reset, sizeof(UINT));


	{
		std::array<ParticleData, PARTICLE_MAX_NUM>lData;
		for (int y = 0; y < 10; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				int lIndex = y * 10 + x;

				lData[lIndex].pos = { 0.0f,static_cast<float>(y) * 20.0f,static_cast<float>(x) * 20.0f,0.0f };
			}
		}
		buffers->TransData(particleDataHandle, lData.data(), sizeof(ParticleData) * PARTICLE_MAX_NUM);
	}


	IndirectCommand command;
	command.drawArguments.IndexCountPerInstance = static_cast<UINT>(indices.size());
	command.drawArguments.InstanceCount = PER_USE_PARTICLE_MAX_NUM * PARTICLE_MAX_NUM;
	command.drawArguments.StartIndexLocation = 0;
	command.drawArguments.StartInstanceLocation = 0;
	command.drawArguments.BaseVertexLocation = 0;

	command.uav = buffers->GetGpuAddress(outputBufferHandle);
	buffers->TransData(drawCommandHandle, &command, sizeof(IndirectCommand) * DRAW_CALL);
	//�]��-------------------------

	outputInitViewHandle = UavViewHandleMgr::Instance()->GetHandle();

	computeMemSize = DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_TEXTURE_COMPUTEBUFFER);
	DescriptorHeapMgr::Instance()->CreateBufferView(
		outputInitViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputInitData), PARTICLE_MAX_NUM * PER_USE_PARTICLE_MAX_NUM),
		buffers->GetBufferData(outputInitBufferHandle).Get(),
		nullptr
	);

	baseOutputInitViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		baseOutputInitViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputInitData), PARTICLE_MAX_NUM *PER_USE_PARTICLE_MAX_NUM),
		buffers->GetBufferData(baseOutputInitBufferHandle).Get(),
		nullptr
	);


	particleDataViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		particleDataViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(ParticleData), PARTICLE_MAX_NUM),
		buffers->GetBufferData(particleDataHandle).Get(),
		nullptr
	);

	outputViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		outputViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputData), PARTICLE_MAX_NUM * PER_USE_PARTICLE_MAX_NUM),
		buffers->GetBufferData(outputBufferHandle).Get(),
		nullptr
	);


	vertexBufferView = KazBufferHelper::SetVertexBufferView(buffers->GetGpuAddress(vertexBufferHandle), vertBuffSize, sizeof(vertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(buffers->GetGpuAddress(indexBufferHandle), indexBuffSize);

	constBufferData.flash.x = 0.0f;


	//�p�[�e�B�N������������--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_BLOCKPARTICLE);

	{
		lConstBufferData.cameraMat = CameraMgr::Instance()->GetViewMatrix();
		lConstBufferData.projectionMat = CameraMgr::Instance()->GetPerspectiveMatProjection();
		lConstBufferData.billboardMat = CameraMgr::Instance()->GetMatBillBoard();
		buffers->TransData(commonInitBufferHandle, &lConstBufferData, sizeof(CommonData));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers->GetGpuAddress(commonInitBufferHandle));
	}

	{
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputInitViewHandle));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(particleDataViewHandle));
	}

	DirectX12CmdList::Instance()->cmdList->Dispatch(PARTICLE_MAX_NUM, 1, 1);
	//�p�[�e�B�N������������--------------------------------------------


	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(outputInitBufferHandle).Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COPY_SOURCE
	);

	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(baseOutputInitBufferHandle).Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COPY_DEST
	);

	DirectX12CmdList::Instance()->cmdList->CopyResource(buffers->GetBufferData(baseOutputInitBufferHandle).Get(), buffers->GetBufferData(outputInitBufferHandle).Get());

	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(outputInitBufferHandle).Get(),
		D3D12_RESOURCE_STATE_COPY_SOURCE,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	);

	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(baseOutputInitBufferHandle).Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	);
}

void ParticleWall::Update()
{
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_PARTICLEWALL_HITBOX);

	//���ʗp�o�b�t�@�̃f�[�^���M
	{
		constBufferData.viewProjectionMat = CameraMgr::Instance()->GetViewMatrix() * CameraMgr::Instance()->GetPerspectiveMatProjection();
		constBufferData.scaleRotateBillboardMat = KazMath::CaluScaleMatrix({ 1.0f,1.0f,1.0f }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f }) * CameraMgr::Instance()->GetMatBillBoard();

		buffers->TransData(commonBufferHandle, &constBufferData, sizeof(CommonMoveData));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(3, buffers->GetGpuAddress(commonBufferHandle));
		constBufferData.flash.y = 0;
	}

	{
		//�������l
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputInitViewHandle));
		//���ʒl
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(baseOutputInitViewHandle));

		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(2, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputViewHandle));
	}

	DirectX12CmdList::Instance()->cmdList->Dispatch((PARTICLE_MAX_NUM * PER_USE_PARTICLE_MAX_NUM) / 1000, 1, 1);
}

void ParticleWall::Draw()
{
	GraphicsPipeLineMgr::Instance()->SetPipeLineAndRootSignature(PIPELINE_NAME_GPUPARTICLE);
	DirectX12CmdList::Instance()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX12CmdList::Instance()->cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
	DirectX12CmdList::Instance()->cmdList->IASetIndexBuffer(&indexBufferView);

	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(drawCommandHandle).Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT
	);

	DirectX12CmdList::Instance()->cmdList->ExecuteIndirect
	(
		commandSig.Get(),
		1,
		buffers->GetBufferData(drawCommandHandle).Get(),
		0,
		nullptr,
		0
	);

	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(drawCommandHandle).Get(),
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	);
}
