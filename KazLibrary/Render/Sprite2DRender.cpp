#include "Sprite2DRender.h"
#include"../Buffer/DescriptorHeapMgr.h"
#include"../Helper/KazHelper.h"
#include"../RenderTarget/RenderTargetStatus.h"

Sprite2DRender::Sprite2DRender()
{
	anchorPoint = { 0.5f,0.5f };

	gpuBuffer = std::make_unique<CreateGpuBuffer>();


	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------
	//���_�f�[�^
	KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, anchorPoint.ConvertXMFLOAT2());
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	//�C���f�b�N�X�f�[�^
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	//�f�[�^�̒�`-----------------------------------------------------------------------------------------------------


	vertByte = KazBufferHelper::GetBufferSize<UINT>(vertices.size(), sizeof(SpriteVertex));
	indexByte = KazBufferHelper::GetBufferSize<UINT>(indices.size(), sizeof(sizeof(unsigned short)));


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

	drawIndexInstanceCommandData = KazRenderHelper::SetDrawIndexInstanceCommandData(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		KazBufferHelper::SetVertexBufferView(gpuBuffer->GetGpuAddress(vertexBufferHandle), vertByte, sizeof(vertices[0])),
		KazBufferHelper::SetIndexBufferView(gpuBuffer->GetGpuAddress(indexBufferHandle), indexByte),
		static_cast<UINT>(indices.size()),
		1);

}

Sprite2DRender::~Sprite2DRender()
{
}

void Sprite2DRender::Draw()
{
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
	renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
	//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------



	//�s��v�Z-----------------------------------------------------------------------------------------------------
	if (data.transform.Dirty())
	{
		baseMatWorldData.matWorld = DirectX::XMMatrixIdentity();
		baseMatWorldData.matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(data.transform.rotation));
		baseMatWorldData.matWorld *= DirectX::XMMatrixTranslationFromVector(data.transform.pos.ConvertXMVECTOR());
	}
	//�s��v�Z-----------------------------------------------------------------------------------------------------



	//���_�f�[�^�̏���������UV�̏�������-----------------------------------------------------------------------------------------------------

	//�ǂݍ��񂾃e�N�X�`���̃T�C�Y
	//�ǂݍ��񂾉摜�̃T�C�Y�����킹��
	if (data.handleData.flag.Dirty() ||
		data.transform.scaleDirtyFlag.Dirty() ||
		data.leftScaleDirtyFlag.Dirty() ||
		data.rightScaleDirtyFlag.Dirty()
		)
	{
		//�O�����\�[�X�̃e�N�X�`���T�C�Y
		if (DescriptorHeapMgr::Instance()->GetType(data.handleData.handle) != DESCRIPTORHEAP_MEMORY_TEXTURE_RENDERTARGET)
		{
			//�T�C�Y��DirectX::XMFLOAT3����DirectX::XMFLOAT2�ɒ���
			KazMath::Vec2<float> lTmpSize = {};
			lTmpSize = data.transform.scale;

			texSize.x = static_cast<int>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handleData.handle).Width);
			texSize.y = static_cast<int>(renderData.shaderResourceMgrInstance->GetTextureSize(data.handleData.handle).Height);


			KazMath::Vec2<float> lLeftUp, lRightDown;
			lLeftUp = { 0.0f,0.0f };
			lRightDown = { 1.0f,1.0f };

			//�T�C�Y�ύX
			std::array<KazMath::Vec2<float>, 4>lVert;
			lVert = KazRenderHelper::ChangePlaneScale(lLeftUp, lRightDown, lTmpSize, anchorPoint, texSize);

			for (int i = 0; i < lVert.size(); i++)
			{
				vertices[i].pos = { lVert[i].x,-lVert[i].y,0.0f };
			}
			KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
		}
		//�����_�[�^�[�Q�b�g�̃e�N�X�`���T�C�Y
		else
		{
			KazMath::Vec2<float> lTmpSize = data.transform.scale;

			texSize.x = static_cast<int>(RenderTargetStatus::Instance()->GetBufferData(data.handleData.handle)->GetDesc().Width);
			texSize.y = static_cast<int>(RenderTargetStatus::Instance()->GetBufferData(data.handleData.handle)->GetDesc().Height);

			KazMath::Vec2<float> lLeftUp, lRightDown;
			lLeftUp = { 0.0f,0.0f };
			lRightDown = { 1.0f,1.0f };

			//�T�C�Y�ύX
			std::array<KazMath::Vec2<float>, 4>tmp = KazRenderHelper::ChangePlaneScale(lLeftUp, lRightDown, lTmpSize, anchorPoint, texSize);
			for (int i = 0; i < tmp.size(); i++)
			{
				vertices[i].pos = { tmp[i].x,-tmp[i].y,0.0f };
			}
		}

		vertices[0].pos.x *= data.leftScale.x;
		vertices[0].pos.y *= data.leftScale.y;
		vertices[1].pos.x *= data.leftScale.x;
		vertices[1].pos.y *= data.leftScale.y;

		vertices[2].pos.x *= data.rightScale.x;
		vertices[2].pos.y *= data.rightScale.y;
		vertices[3].pos.x *= data.rightScale.x;
		vertices[3].pos.y *= data.rightScale.y;
	}




	//UV�؂���
	if (data.handleData.flag.Dirty() || data.animationHandle.flag.Dirty() || data.transform.scaleDirtyFlag.Dirty() ||
		data.leftScaleDirtyFlag.Dirty() || data.rightScaleDirtyFlag.Dirty())
	{
		KazMath::Vec2<int> lDivSize = renderData.shaderResourceMgrInstance->GetDivData(data.handleData.handle).divSize;
		KazMath::Vec2<float> lTmpSize = { data.transform.scale.x, data.transform.scale.y };


		const bool lIsItSafeToUseAnimationHandleFlag = KazHelper::IsitInAnArray(data.animationHandle.handle, renderData.shaderResourceMgrInstance->GetDivData(data.handleData.handle).divLeftUp.size());
		const bool lIsItSafeToUseDivDataFlag = (lDivSize.x != -1 && lDivSize.y != -1);


		if (lIsItSafeToUseDivDataFlag && lIsItSafeToUseAnimationHandleFlag)
		{
			KazMath::Vec2<int> lDivLeftUpPos = renderData.shaderResourceMgrInstance->GetDivData(data.handleData.handle).divLeftUp[data.animationHandle.handle];
			KazRenderHelper::VerticesCut(lDivSize, lDivLeftUpPos, &vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, lTmpSize, anchorPoint);
			KazRenderHelper::UVCut(lDivLeftUpPos, lDivSize, texSize, &vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);

			for (int i = 0; i < vertices.size(); i++)
			{
				vertices[i].pos.y *= -1.0f;
			}
		}

		bool debug = false;
		debug = false;
	}

	//X����UV�𔽓]
	if (data.flip.xDirtyFlag.Dirty())
	{
		KazRenderHelper::FlipXUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}
	//Y����UV�𔽓]
	if (data.flip.yDirtyFlag.Dirty())
	{
		KazRenderHelper::FlipYUv(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);
	}


	bool lFlipDirtyFlag = data.flip.xDirtyFlag.Dirty() || data.flip.yDirtyFlag.Dirty();
	//���_�f�[�^�ɉ����ύX����������]������
	if (data.handleData.flag.Dirty() ||
		data.transform.scaleDirtyFlag.Dirty() ||
		data.animationHandle.flag.Dirty() ||
		lFlipDirtyFlag ||
		data.leftScaleDirtyFlag.Dirty() ||
		data.rightScaleDirtyFlag.Dirty())
	{
		gpuBuffer->TransData(vertexBufferHandle, vertices.data(), vertByte);
	}
	//���_�f�[�^�̏���������UV�̏�������-----------------------------------------------------------------------------------------------------


	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
	//�s��
	if (data.transform.Dirty() || renderData.cameraMgrInstance->ViewAndProjDirty() || data.colorData.Dirty())
	{
		ConstBufferData lConstMap;
		lConstMap.world = baseMatWorldData.matWorld;
		lConstMap.view = DirectX::XMMatrixIdentity();
		lConstMap.viewproj = renderData.cameraMgrInstance->GetOrthographicMatProjection();
		lConstMap.color = data.colorData.ConvertColorRateToXMFLOAT4();
		lConstMap.mat = lConstMap.world * lConstMap.viewproj;
		TransData(&lConstMap, constBufferHandle, typeid(lConstMap).name());
	}
	//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------


	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
	SetConstBufferOnCmdList(data.pipelineName);


	if (data.handleData.handle != -1)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = DescriptorHeapMgr::Instance()->GetGpuDescriptorView(data.handleData.handle);
		int param = KazRenderHelper::SetBufferOnCmdList(GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(data.pipelineName)), GRAPHICS_RANGE_TYPE_SRV_DESC, GRAPHICS_PRAMTYPE_TEX);
		renderData.cmdListInstance->cmdList->SetGraphicsRootDescriptorTable(param, gpuDescHandleSRV);
	}

	for (int i = 0; i < data.addHandle.handle.size(); ++i)
	{
		renderData.shaderResourceMgrInstance->SetSRV(data.addHandle.handle[i], GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(data.pipelineName)), data.addHandle.paramType[i]);
	}
	//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------




	//�`�施��-----------------------------------------------------------------------------------------------------
	DrawIndexInstanceCommand(drawIndexInstanceCommandData);
	//�`�施��-----------------------------------------------------------------------------------------------------


	//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
	data.Record();
	//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
}