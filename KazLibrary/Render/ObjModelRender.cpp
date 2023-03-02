#include "ObjModelRender.h"

ObjModelRender::ObjModelRender(bool INSTANCE_FLAG, int INSTANCE_NUM, bool MATERIAL_NONE_FALG)
{
	instanceFlag = INSTANCE_FLAG;
	instanceNum = INSTANCE_NUM;

	gpuBuffer = std::make_unique<CreateGpuBuffer>();

	if (!instanceFlag)
	{
		constBufferHandle[0] = CreateConstBuffer(sizeof(ConstBufferData), typeid(ConstBufferData).name(), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DRAW);
	}

	if (!MATERIAL_NONE_FALG)
	{
		constBufferHandle[1] = CreateConstBuffer(sizeof(ConstBufferDataB1), typeid(ConstBufferDataB1).name(), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA);
	}
	data.removeMaterialFlag = MATERIAL_NONE_FALG;
}

ObjModelRender::~ObjModelRender()
{
}

DirectX::XMMATRIX* ObjModelRender::GetMotherMatrixPtr()
{
	return &motherMat;
}

void ObjModelRender::Draw()
{
	if (data.handle.flag.Dirty())
	{
		modelData = ObjResourceMgr::Instance()->GetResourceData(data.handle.handle);
		drawIndexInstanceCommandData = KazRenderHelper::SetDrawIndexInstanceCommandData(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, modelData.vertexBufferView, modelData.indexBufferView, modelData.indexNum, instanceNum);
	}

	if (data.handle.handle != -1)
	{

		//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------
		renderData.pipelineMgr->SetPipeLineAndRootSignature(data.pipelineName);
		//�p�C�v���C���ݒ�-----------------------------------------------------------------------------------------------------


		//�s��v�Z-----------------------------------------------------------------------------------------------------
		//�C���X�^���V���O�`����s���Ȃ炱�̏����͓]�����Ȃ�
		if (!instanceFlag)
		{
			if (data.transform.Dirty() || data.frontVecDirtyFlag.Dirty() || data.upVecDirtyFlag.Dirty() || data.motherMat.dirty.Dirty())
			{
				baseMatWorldData.matWorld = DirectX::XMMatrixIdentity();
				baseMatWorldData.matScale = KazMath::CaluScaleMatrix(data.transform.scale);
				baseMatWorldData.matTrans = KazMath::CaluTransMatrix(data.transform.pos);
				baseMatWorldData.matRota = KazMath::CaluRotaMatrix(data.transform.rotation);
				//���[���h�s��̌v�Z
				baseMatWorldData.matWorld = DirectX::XMMatrixIdentity();
				baseMatWorldData.matWorld *= baseMatWorldData.matScale;
				baseMatWorldData.matWorld *= baseMatWorldData.matRota;
				if (data.upVector.x != 0.0f ||
					data.upVector.y != 1.0f ||
					data.upVector.z != 0.0f)
				{
					baseMatWorldData.matWorld *= KazMath::CaluSlopeMatrix(data.upVector, { 0.0f,0.0f,1.0f });
				}
				if (data.frontVector.x != 0.0f ||
					data.frontVector.y != 0.0f ||
					data.frontVector.z != 1.0f)
				{
					baseMatWorldData.matWorld *= KazMath::CaluFrontMatrix(KazMath::Vec3<float>(0.0f, 1.0f, 0.0f), data.frontVector);
				}
				baseMatWorldData.matWorld *= baseMatWorldData.matTrans;
				//�e�s����|����
				baseMatWorldData.matWorld *= data.motherMat.mat;
			}
			//�s��v�Z-----------------------------------------------------------------------------------------------------
			motherMat = baseMatWorldData.matWorld;


			//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------
			//�s��
			if (renderData.cameraMgrInstance->ViewAndProjDirty(data.cameraIndex.id) || data.transform.Dirty() || data.colorData.Dirty() || data.motherMat.dirty.Dirty() || data.cameraIndex.dirty.Dirty())
			{
				ConstBufferData constMap;
				constMap.world = baseMatWorldData.matWorld;
				constMap.view = renderData.cameraMgrInstance->GetViewMatrix(data.cameraIndex.id);
				constMap.viewproj = renderData.cameraMgrInstance->GetPerspectiveMatProjection(data.cameraIndex.id);
				constMap.color = data.colorData.ConvertColorRateToXMFLOAT4();
				constMap.mat = constMap.world * constMap.view * constMap.viewproj;

				TransData(&constMap, constBufferHandle[0], typeid(constMap).name());
			}
		}


		if (!data.removeMaterialFlag)
		{
			//�}�e���A���f�[�^
			if (data.handle.flag.Dirty() || data.colorData.Dirty())
			{
				ConstBufferDataB1 constMap;
				constMap.alpha = data.colorData.color.a / 255.0f;
				constMap.ambient = modelData.material.ambient;
				constMap.diffuse = modelData.material.diffuse;
				constMap.specular = modelData.material.specular;
				TransData(&constMap, constBufferHandle[1], typeid(constMap).name());
			}
			//�o�b�t�@�̓]��-----------------------------------------------------------------------------------------------------


			//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
			renderData.shaderResourceMgrInstance->SetSRV(modelData.mtlHanlde, GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(data.pipelineName)), GRAPHICS_PRAMTYPE_TEX);
			//�o�b�t�@���R�}���h���X�g�ɐς�-----------------------------------------------------------------------------------------------------
		}

		SetConstBufferOnCmdList(data.pipelineName, data.removeMaterialFlag, demoFlag);


		//�ǉ��̃e�N�X�`���𑗂�---------------------------------------------------------------
		for (int i = 0; i < data.addHandle.handle.size(); ++i)
		{
			renderData.shaderResourceMgrInstance->SetSRV(data.addHandle.handle[i], GraphicsRootSignature::Instance()->GetRootParam(renderData.pipelineMgr->GetRootSignatureName(data.pipelineName)), data.addHandle.paramType[i]);
		}
		//�ǉ��̃e�N�X�`���𑗂�---------------------------------------------------------------


		//�`�施��-----------------------------------------------------------------------------------------------------
		DrawIndexInstanceCommand(drawIndexInstanceCommandData);
		//�`�施��-----------------------------------------------------------------------------------------------------


		//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
		data.Record();
		//DirtyFlag�̍X�V-----------------------------------------------------------------------------------------------------
	}
}