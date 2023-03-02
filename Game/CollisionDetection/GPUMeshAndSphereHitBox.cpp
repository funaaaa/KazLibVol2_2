#include "GPUMeshAndSphereHitBox.h"
#include"../KazLibrary/DirectXCommon/DirectX12Device.h"
#include"../KazLibrary/DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Pipeline/GraphicsRootSignature.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"

GPUCollisionDetectionOfMeshHitBoxAndSphere::GPUCollisionDetectionOfMeshHitBoxAndSphere(std::vector<DirectX::XMFLOAT4> VERT_NUM, float PARTICLE_SCALE)
{
	if (VERT_NUM.size() == 0)
	{
		assert(0);
	}


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

	std::array<Vertex, 4>vertices;
	std::array<USHORT, 6> indices;
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, { 0.5f,0.5f });
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);

	BUFFER_SIZE vertBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(vertices.size(), sizeof(Vertex));
	BUFFER_SIZE indexBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(indices.size(), sizeof(unsigned int));

	//�o�b�t�@����-------------------------
	//�������p--------
	//�`����
	vertexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetVertexBufferData(vertBuffSize));
	indexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetIndexBufferData(indexBuffSize));
	verticesDataHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(DirectX::XMFLOAT4) * static_cast<UINT>(VERT_NUM.size())));

	//���ʏ��
	initCommonHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(InitCommonData)));

	//�o��
	outputHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(OutputData) * PARTICLE_MAX_NUM)));
	updateHitboxHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(OutputData) * PARTICLE_MAX_NUM)));
	//�������p--------

	//�X�V�p
	updateHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer((sizeof(UpdateData) * PARTICLE_MAX_NUM)));
	updateCommonHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(UpdateCommonData)));
	drawMeshCommonHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(MeshDrawCommonData)));

	//�`��p
	drawCommandHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(IndirectCommand) * DRAW_CALL));

	//�o�b�t�@����-------------------------

	//�]��-------------------------
	buffers->TransData(vertexBufferHandle, vertices.data(), vertBuffSize);
	buffers->TransData(indexBufferHandle, indices.data(), indexBuffSize);

	{
		//���_���
		memcpy
		(
			buffers->GetMapAddres(verticesDataHandle),
			VERT_NUM.data(),
			VERT_NUM.size() * sizeof(DirectX::XMFLOAT4)
		);

		constBufferData.worldPos = {};
		buffers->TransData(initCommonHandle, &constBufferData, sizeof(InitCommonData));
	}

	IndirectCommand command;
	command.drawArguments.IndexCountPerInstance = static_cast<UINT>(indices.size());
	command.drawArguments.InstanceCount = PARTICLE_MAX_NUM;
	command.drawArguments.StartIndexLocation = 0;
	command.drawArguments.StartInstanceLocation = 0;
	command.drawArguments.BaseVertexLocation = 0;

	command.uav = buffers->GetGpuAddress(updateHandle);
	buffers->TransData(drawCommandHandle, &command, sizeof(IndirectCommand) * DRAW_CALL);
	//�]��-------------------------

	outputViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		outputViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputData), PARTICLE_MAX_NUM),
		buffers->GetBufferData(outputHandle).Get(),
		nullptr
	);

	updateHitboxViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		updateHitboxViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputData), PARTICLE_MAX_NUM),
		buffers->GetBufferData(updateHitboxHandle).Get(),
		nullptr
	);

	vertDataViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		vertDataViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(DirectX::XMFLOAT4), static_cast<UINT>(VERT_NUM.size())),
		buffers->GetBufferData(verticesDataHandle).Get(),
		nullptr
	);

	updateViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		updateViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(UpdateData), PARTICLE_MAX_NUM),
		buffers->GetBufferData(updateHandle).Get(),
		nullptr
	);

	vertexBufferView = KazBufferHelper::SetVertexBufferView(buffers->GetGpuAddress(vertexBufferHandle), vertBuffSize, sizeof(vertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(buffers->GetGpuAddress(indexBufferHandle), indexBuffSize);



	//����������--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_HITBOX_TRIANGLE_INIT);

	//���_
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(vertDataViewHandle));
	//�o��
	DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputViewHandle));
	//���ʗp�o�b�t�@�̃f�[�^���M
	DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers->GetGpuAddress(initCommonHandle));
	DirectX12CmdList::Instance()->cmdList->Dispatch(10, 1, 1);
	//����������--------------------------------------------


	scale = PARTICLE_SCALE;
	scaleRotaMat = KazMath::CaluScaleMatrix({ scale,scale,scale }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f });

	drawParticleFlag = true;

	updateCommonData.meshNum = static_cast<UINT>(VERT_NUM.size()) / 3;
	//updateCommonData.meshNum /= 8;
	pointPos.data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "sphere.fbx");
	pointPos.data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
	pointPos.data.removeMaterialFlag = true;

	updateCommonData.radius = 20.0f;

	hitVel = 1;
}

GPUCollisionDetectionOfMeshHitBoxAndSphere::~GPUCollisionDetectionOfMeshHitBoxAndSphere()
{
}

void GPUCollisionDetectionOfMeshHitBoxAndSphere::Init(const DirectX::XMMATRIX *MOTHER_MAT)
{
	motherMat = MOTHER_MAT;
}

void GPUCollisionDetectionOfMeshHitBoxAndSphere::Update(RESOURCE_HANDLE BASE_HANDLE, RESOURCE_HANDLE PARTICLE_HANDLE, bool DIRTY_FLAG)
{
	ImGui::Begin("HitBox");
	KazImGuiHelper::InputXMFLOAT4("PointPos", &updateCommonData.pos);
	ImGui::DragFloat("Radius", &updateCommonData.radius);
	ImGui::DragInt("HitVel", &hitVel);
	ImGui::End();

	pointPos.data.transform.pos = { updateCommonData.pos.x,updateCommonData.pos.y,updateCommonData.pos.z };
	pointPos.data.transform.scale = { updateCommonData.radius,updateCommonData.radius,updateCommonData.radius };

	if (!drawParticleFlag)
	{
		return;
	}

	float lScale = scale;
	scaleRotaMat = KazMath::CaluScaleMatrix({ lScale,lScale,lScale }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f });




	//���b�V���`��p--------------------------------------------
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_HITBOX_MESH);
	{
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputViewHandle));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(updateHitboxViewHandle));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(2, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(updateViewHandle));
	}

	//���ʗp�o�b�t�@�̃f�[�^���M
	{
		drawMeshCommonData.scaleRotateBillboardMat = scaleRotaMat * CameraMgr::Instance()->GetMatBillBoard();
		drawMeshCommonData.viewProjection = CameraMgr::Instance()->GetViewMatrix() * CameraMgr::Instance()->GetPerspectiveMatProjection();
		drawMeshCommonData.motherMat = *motherMat;
		buffers->TransData(drawMeshCommonHandle, &drawMeshCommonData, sizeof(MeshDrawCommonData));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(3, buffers->GetGpuAddress(drawMeshCommonHandle));
	}
	DirectX12CmdList::Instance()->cmdList->Dispatch(1, 1, 1);
	//���b�V���`��p--------------------------------------------



	//�����蔻��p
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_HITBOX_UPDATE);

	{
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(updateHitboxViewHandle));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(updateViewHandle));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(2, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(PARTICLE_HANDLE));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(3, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(BASE_HANDLE));
	}

	//���ʗp�o�b�t�@�̃f�[�^���M
	{
		updateCommonData.scaleRotateBillboardMat = scaleRotaMat * CameraMgr::Instance()->GetMatBillBoard();
		updateCommonData.viewProjection = CameraMgr::Instance()->GetViewMatrix() * CameraMgr::Instance()->GetPerspectiveMatProjection();

		if (DIRTY_FLAG)
		{
			updateCommonData.hitVel = static_cast<UINT>(hitVel);
		}
		else
		{
			updateCommonData.hitVel = 0;
		}

		buffers->TransData(updateCommonHandle, &updateCommonData, sizeof(UpdateCommonData));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(4, buffers->GetGpuAddress(updateCommonHandle));
	}

	DirectX12CmdList::Instance()->cmdList->Dispatch(20, 1, 1);

}

void GPUCollisionDetectionOfMeshHitBoxAndSphere::Draw()
{
	if (!drawParticleFlag)
	{
		return;
	}
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

	pointPos.Draw();
}