#include "Box2DRender.h"

Box2DRender::Box2DRender()
{
	gpuBuffer = std::make_unique<CreateGpuBuffer>();

	vertByte = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(vertices.size(), sizeof(SpriteVertex));
	//���_�o�b�t�@
	vertexBufferHandle = gpuBuffer->CreateBuffer
	(
		KazBufferHelper::SetVertexBufferData(vertByte)
	);
	constBufferHandle = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DRAW);

	drawInstanceCommandData =
		KazRenderHelper::SetDrawInstanceCommandData(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
			KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertexBufferHandle), vertByte, sizeof(vertices[0])),
			static_cast<UINT>(vertices.size()),
			1
		);
}

void Box2DRender::Draw()
{
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------


	//�ʒu�ύX-------------------------
	if (data.VertDirty())
	{
		vertices[LEFT_UP].pos = KazMath::Vec3<float>(data.leftUpPos, 0.0f).ConvertXMFLOAT3();
		vertices[LEFT_DOWN].pos = KazMath::Vec3<float>(data.leftDownPos, 0.0f).ConvertXMFLOAT3();
		vertices[RIGHT_UP].pos = KazMath::Vec3<float>(data.rightUpPos, 0.0f).ConvertXMFLOAT3();
		vertices[RIGHT_DOWN].pos = KazMath::Vec3<float>(data.rightDownPos, 0.0f).ConvertXMFLOAT3();
		vertices[4].pos = vertices[LEFT_UP].pos;

		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), vertByte);
	}
	//�ʒu�ύX-------------------------



	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	if (data.color.Dirty() || data.VertDirty())
	{
		constMap.world = DirectX::XMMatrixIdentity();
		constMap.view = DirectX::XMMatrixIdentity();
		constMap.viewproj = renderData.cameraMgrInstance->GetOrthographicMatProjection();
		constMap.color = data.color.ConvertColorRateToXMFLOAT4();
		constMap.mat = constMap.world * constMap.viewproj;
		TransData(&constMap, constBufferHandle, typeid(constMap).name());
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
