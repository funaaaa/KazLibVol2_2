#include "LineRender.h"

LineRender::LineRender(int INSTANCE_NUM)
{
	gpuBuffer = std::make_unique<CreateGpuBuffer>();

	vertices[0].pos.x = 0;
	vertices[0].pos.y = 0;
	vertices[0].pos.z = 0;
	vertices[1].pos.x = 0;
	vertices[1].pos.y = 0;
	vertices[1].pos.z = 0;

	vertices[0].tickness.x = 0.0f;
	vertices[0].tickness.y = 0.0f;
	vertices[1].tickness.x = 1.0f;
	vertices[1].tickness.y = 1.0f;

	
	vertByte = KazBufferHelper::GetBufferSize<UINT>(vertices.size(), sizeof(LineVertex));

	//�o�b�t�@����-----------------------------------------------------------------------------------------------------
	//���_�o�b�t�@
	vertexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetVertexBufferData(vertByte)
	);

	if (INSTANCE_NUM == 1)
	{
		//�萔�o�b�t�@
		constBufferHandle = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DRAW);
	}
	//�o�b�t�@����-----------------------------------------------------------------------------------------------------


	drawInstanceCommandData = KazRenderHelper::SetDrawInstanceCommandData(
		D3D_PRIMITIVE_TOPOLOGY_LINELIST,
		KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertexBufferHandle), vertByte, sizeof(vertices[0])),
		static_cast<UINT>(vertices.size()),
		INSTANCE_NUM
		);
}

LineRender::~LineRender()
{
}

void LineRender::Draw()
{
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------


	if (data.startPosDirtyFlag.Dirty() || data.endPosDirtyFlag.Dirty())
	{
		vertices[0].pos = data.startPos.ConvertXMFLOAT3();
		vertices[1].pos = data.endPos.ConvertXMFLOAT3();
	}
	if (data.motherMat.dirty.Dirty())
	{
		baseMatWorldData.matWorld = DirectX::XMMatrixIdentity();
		baseMatWorldData.matWorld *= data.motherMat.mat;
	}


	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	//�s��
	if (drawInstanceCommandData.drawInstanceData.instanceCount == 1 &&
		(renderData.cameraMgrInstance->ViewAndProjDirty(data.cameraIndex.id) || data.colorData.Dirty() || data.motherMat.dirty.Dirty() || data.cameraIndex.dirty.Dirty()))
	{
		ConstBufferData constMap;
		constMap.world = baseMatWorldData.matWorld;
		constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex.id);
		constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection(data.cameraIndex.id);
		constMap.color = data.colorData.ConvertColorRateToXMFLOAT4();
		constMap.mat = constMap.world * constMap.view * constMap.viewproj;
		TransData(&constMap, constBufferHandle, typeid(constMap).name());
	}

	//���_�f�[�^�ɉ����ύX����������]������
	if (data.startPosDirtyFlag.Dirty() || data.endPosDirtyFlag.Dirty())
	{
		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), vertByte);
	}
	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------


	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(data.pipelineName);
	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------


	//�`�施��-----------------------------------------------------------------------------------------------------
	DrawInstanceCommand(drawInstanceCommandData);
	//�`�施��-----------------------------------------------------------------------------------------------------

	data.Record();
}