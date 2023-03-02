#include "InstanceMeshCollision.h"

InstanceMeshCollision::InstanceMeshCollision(const std::vector<InitMeshCollisionData> &INIT_DATA, const std::vector<Sphere *> &HITBOX_ARRAY_DATA)
	:hitBoxData(HITBOX_ARRAY_DATA)
{
	//BB���쐬����
	for (int i = 0; i < INIT_DATA.size(); ++i)
	{
		//���b�V���p�[�e�B�N������
		meshData.emplace_back(MeshParticleData(INIT_DATA[i].vertData, INIT_DATA[i].vertNumArray, INIT_DATA[i].meshParticleData, i));
		//BB����
		meshData[i].bb.Compute();

		motherMatArray.emplace_back(INIT_DATA[i].motherMat);
		colorDataArray.emplace_back(INIT_DATA[i].colorData);
	}

	UINT lNum = 0;
	KazBufferHelper::BufferResourceData lBufferData
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(UINT)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		"CopyCounterBuffer"
	);

	copyBuffer.CreateBuffer(lBufferData);
	copyBuffer.TransData(&lNum, sizeof(UINT));
}

void InstanceMeshCollision::Init(const GPUParticleRender *RENDER_PTR)
{
	inputMeshCircleBufferHandle = meshMoveCompute.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(MeshHitBoxData) * PARTICLE_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA,
		sizeof(MeshHitBoxData),
		PARTICLE_NUM,
		true
	);

	outputMeshCircleBufferHandle = meshMoveCompute.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(MeshHitBoxData) * PARTICLE_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		sizeof(MeshHitBoxData),
		PARTICLE_NUM,
		false
	);

	float lRadius = 30.0f;
	UINT meshCircleNum = 0;
	//���b�V���p�[�e�B�N���̓����蔻�萶���[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	for (int i = 0; i < meshData.size(); ++i)
	{
		//���b�V��������
		generateMeshHitBox.emplace_back(BBDuringEquallyCoordinatePlace(meshData[i].bb.GetBBBuffer(), meshData[i].bb.GetData(0), meshMoveCompute.GetBufferData(inputMeshCircleBufferHandle), lRadius, i));

#ifdef DEBUG
		generateMeshHitBox[i].SetDebugDraw(RENDER_PTR->GetStackBuffer());
#endif
		generateMeshHitBox[i].Compute();


		InitCollisionOfParticleData lInitData
		(
			generateMeshHitBox[i].GetHitBoxPosData(),
			meshData[i].meshParticle.GetBuffer(),
			particleAvoidParticle.GetStackParticleHitBoxBuffer(),
			0.1f,
			lRadius,
			generateMeshHitBox[i].MaxHitBoxPosNum()
		);
		//�p�[�e�B�N���ƃ����N�t��
		linkMeshHitBoxAndParticle.emplace_back(GenerateCollisionOfParticle(lInitData));
		linkMeshHitBoxAndParticle[i].Compute();

		meshCircleNum += generateMeshHitBox[i].MaxHitBoxPosNum();
	}
	//���b�V���p�[�e�B�N���̓����蔻�萶���[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

	particleAvoidParticle.GenerateHitNum(generateMeshHitBox[0].MaxHitBoxPosNum());

	cpuAndMeshCircleHitBox = std::make_unique<CollisionDetectionOfMeshCircleAndCPUHitBox>(hitBoxData, lRadius / 2.0f, meshCircleNum);

	//�ړ����buffer��n��
	cpuAndMeshCircleHitBox->SetStackMeshCircleBuffer(meshMoveCompute.GetBufferData(outputMeshCircleBufferHandle));

	//�����Ɠ����������̏��L�^
	particleAvoidParticle.SetHitIDBuffer(cpuAndMeshCircleHitBox->GetStackIDBuffer());



	updatePosCompute.SetBuffer(particleAvoidParticle.GetOutputParticleData(), GRAPHICS_PRAMTYPE_DATA);
	motherMatHandle = updatePosCompute.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(KazBufferHelper::GetBufferSize<UINT>(motherMatArray.size(), sizeof(DirectX::XMMATRIX))),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA2,
		sizeof(DirectX::XMMATRIX),
		static_cast<UINT>(motherMatArray.size())
	);

	updatePosCompute.SetBuffer(RENDER_PTR->GetStackBuffer(), GRAPHICS_PRAMTYPE_DATA5);

	scaleRotateBillboardMatHandle = updatePosCompute.CreateBuffer(
		sizeof(DirectX::XMMATRIX),
		GRAPHICS_RANGE_TYPE_CBV_VIEW,
		GRAPHICS_PRAMTYPE_DATA6,
		1,
		false);

	float lScale = 0.2f;
	scaleRotaMat = KazMath::CaluScaleMatrix({ lScale,lScale,lScale }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f });

	//�e�s��]���p
	motherMatrixBuffer.bufferWrapper.CreateBuffer(
		KazBufferHelper::BufferResourceData(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			CD3DX12_RESOURCE_DESC::Buffer(KazBufferHelper::GetBufferSize<BUFFER_SIZE>(motherMatArray.size(), sizeof(DirectX::XMMATRIX))),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			"RAMmatData")
	);

	meshMoveCompute.SetBuffer(updatePosCompute.GetBufferData(motherMatHandle), GRAPHICS_PRAMTYPE_DATA2);



	colorDataBuffer.bufferWrapper.CreateBuffer(
		KazBufferHelper::BufferResourceData(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			CD3DX12_RESOURCE_DESC::Buffer(KazBufferHelper::GetBufferSize<BUFFER_SIZE>(motherMatArray.size(), sizeof(ColorData))),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			"RAMColorData")
	);

	colorDataHandle = updatePosCompute.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(KazBufferHelper::GetBufferSize<UINT>(motherMatArray.size(), sizeof(ColorData))),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA4,
		sizeof(ColorData),
		static_cast<UINT>(motherMatArray.size())
	);


	updatePosCompute.CreateBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(KazBufferHelper::GetBufferSize<UINT>(PARTICLE_NUM, sizeof(DirectX::XMFLOAT4))),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA3,
		sizeof(DirectX::XMFLOAT4),
		PARTICLE_NUM
	);
}

void InstanceMeshCollision::Compute()
{
#ifdef DEBUG
	for (int i = 0; i < generateMeshHitBox.size(); ++i)
	{
		generateMeshHitBox[i].DebugCompute();
	}
#endif


	PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "TransMotherMat");
#pragma region �e�s��̓]��

	std::vector<DirectX::XMMATRIX>lMatArray(motherMatArray.size());
	for (int i = 0; i < lMatArray.size(); ++i)
	{
		lMatArray[i] = *motherMatArray[i];
	}
	motherMatrixBuffer.bufferWrapper.TransData(lMatArray.data(), sizeof(DirectX::XMMATRIX) * static_cast<int>(lMatArray.size()));

	updatePosCompute.GetBufferData(motherMatHandle).bufferWrapper.CopyBuffer(motherMatrixBuffer.bufferWrapper.GetBuffer().Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_DEST);

#pragma endregion

#pragma region �F���̓]��

	std::vector<ColorData>lColorArray(colorDataArray.size());
	for (int i = 0; i < lColorArray.size(); ++i)
	{
		lColorArray[i] = *colorDataArray[i];
	}
	colorDataBuffer.bufferWrapper.TransData(lColorArray.data(), sizeof(ColorData) * static_cast<int>(lColorArray.size()));



	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(updatePosCompute.GetBufferData(colorDataHandle).bufferWrapper.GetBuffer().Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_COPY_DEST
		)
	);

	DirectX12CmdList::Instance()->cmdList->CopyResource(updatePosCompute.GetBufferData(colorDataHandle).bufferWrapper.GetBuffer().Get(), colorDataBuffer.bufferWrapper.GetBuffer().Get());

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(updatePosCompute.GetBufferData(colorDataHandle).bufferWrapper.GetBuffer().Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS
		)
	);
#pragma endregion
	PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());


	//���b�V�����̈ړ�
	PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "MoveMeshHitBox");
	meshMoveCompute.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_HITBOX_MESHCIRCLE_MOVE, { 1000,1,1 });
	PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());


	//���b�V�����ƑΏۂ̓����蔻��
	PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "CollisionDetectionOfMeshHitBoxANdCircleHitBox");
	cpuAndMeshCircleHitBox->Compute();
	PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());


	//�Փ˔��肪��ꂽ�p�[�e�B�N���̋���(�����ŕ`��N���X�ɓn��)
	PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "AvoidMeshParticle");
	particleAvoidParticle.Compute();
	PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());

	//���b�V���p�[�e�B�N���̍��W�ړ�
	PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "MoveMeshParticle");
	scaleRotBillBoardMat = scaleRotaMat * CameraMgr::Instance()->GetMatBillBoard();
	updatePosCompute.TransData(scaleRotateBillboardMatHandle, &scaleRotBillBoardMat, sizeof(DirectX::XMMATRIX));
	updatePosCompute.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_UPDATE_STAGE_MESHPARTICLE, { 1000,1,1 });
	PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());
}
