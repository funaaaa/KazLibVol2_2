#include "PolygonRender.h"
#include"../KazLibrary/Helper/KazHelper.h"

PolygonRender::PolygonRender(const std::array<SpriteVertex, 4> &DATA)
{
	anchorPoint = { 0.5f,0.5f };

	gpuBuffer = std::make_unique<CreateGpuBuffer>();

	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------
	//���_�f�[�^
	//KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, anchorPoint);
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	//�C���f�b�N�X�f�[�^
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------

	vertices = DATA;


	vertByte = KazBufferHelper::GetBufferSize<UINT>(vertices.size(), sizeof(SpriteVertex));
	indexByte = KazBufferHelper::GetBufferSize<UINT>(indices.size(), sizeof(unsigned short));


	//�o�b�t�@����-----------------------------------------------------------------------------------------------------
	//���_�o�b�t�@
	vertexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetVertexBufferData(vertByte)
	);
	//�C���f�b�N�X�o�b�t�@
	indexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetIndexBufferData(indexByte)
	);
	//�萔�o�b�t�@
	constBufferHandle = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DRAW);
	//�o�b�t�@����-----------------------------------------------------------------------------------------------------


	//�o�b�t�@�]��-----------------------------------------------------------------------------------------------------
	gpuBuffer->TransData(vertexBufferHandle, vertices.data(), vertByte);
	gpuBuffer->TransData(indexBufferHandle, indices.data(), indexByte);
	//�o�b�t�@�]��-----------------------------------------------------------------------------------------------------


	drawIndexInstanceCommandData = KazRenderHelper::SetDrawIndexInstanceCommandData(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertexBufferHandle), vertByte, sizeof(vertices[0])),
		KazBufferHelper::SetIndexBufferView(gpuBuffer->GetGpuAddress(indexBufferHandle), indexByte),
		static_cast<UINT>(indices.size()),
		1
	);
}

void PolygonRender::Draw()
{

	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------


	//�s��v�Z-----------------------------------------------------------------------------------------------------
	if (data.transform.Dirty() || data.billBoardDirtyFlag.Dirty() || renderData.cameraMgrInstance->BillboardDirty() || data.motherMat.dirty.Dirty())
	{
		baseMatWorldData.matWorld = DirectX::XMMatrixIdentity();
		baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
		baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);
		baseMatWorldData.matRota = KazMath::CaluRotaMatrix(data.transform.rotation);
		//�r���{�[�h�s����|����
		if (data.billBoardFlag)
		{
			baseMatWorldData.matWorld *= renderData.cameraMgrInstance->GetMatBillBoard(data.cameraIndex.dirty.Dirty());
		}
		baseMatWorldData.matWorld *= baseMatWorldData.matScale;
		baseMatWorldData.matWorld *= baseMatWorldData.matRota;
		baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
		baseMatWorldData.matWorld *= data.motherMat.mat;

		//�e�s����|����
		motherMat = baseMatWorldData.matWorld;
	}
	//�s��v�Z-----------------------------------------------------------------------------------------------------


	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	//�s��
	if (data.transform.Dirty() || renderData.cameraMgrInstance->ViewAndProjDirty(data.cameraIndex.id) || renderData.cameraMgrInstance->BillboardDirty() || data.motherMat.dirty.Dirty() || data.cameraIndex.dirty.Dirty())
	{
		ConstBufferData constMap;
		constMap.world = baseMatWorldData.matWorld;
		constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex.id);
		constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection(data.cameraIndex.id);
		constMap.color = data.color.ConvertColorRateToXMFLOAT4();
		constMap.mat = constMap.world * constMap.view * constMap.viewproj;
		TransData(&constMap, constBufferHandle, typeid(ConstBufferData).name());
	}
	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------


	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(data.pipelineName);
	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------

	//�`�施��-----------------------------------------------------------------------------------------------------
	DrawIndexInstanceCommand(drawIndexInstanceCommandData);
	//�`�施��-----------------------------------------------------------------------------------------------------

	data.Record();
}