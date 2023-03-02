#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Collision/BoundingBox.h"
#include"../KazLibrary/Collision/BBDuringEquallyCoordinatePlace.h"
#include"../KazLibrary/Helper/ResouceBufferHelper.h"
#include"../Game/CollisionDetection/GenerateCollisionOfParticle.h"
#include"../Game/Effect/MeshParticle.h"
#include"../Game/CollisionDetection/CollisionDetectionOfMeshCircleAndParticle.h"
#include"../Game/CollisionDetection/ComputeParticleAvoidSphere.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Render/GPUParticleRender.h"

struct ColorData
{
	DirectX::XMINT2 lightData;
	float alpha;
};

struct InitMeshCollisionData
{
	ResouceBufferHelper::BufferData vertData;
	UINT vertNumArray;
	InitMeshParticleData meshParticleData;
	const DirectX::XMMATRIX *motherMat;
	const ColorData *colorData;
};

class InstanceMeshCollision
{
public:
	/// <summary>
	/// ���_���A���b�V���p�[�e�B�N�����W�z������ă��b�V���p�[�e�B�N���̓����蔻��𐶐�����B
	/// </summary>
	InstanceMeshCollision(
		const std::vector<InitMeshCollisionData> &INIT_DATA,
		const std::vector<Sphere*> &HITBOX_ARRAY_DATA
	);

	void Init(const GPUParticleRender *RENDER_PTR);
	void Compute();

private:
	static const int PARTICLE_NUM = 200000;

	std::vector<BBDuringEquallyCoordinatePlace> generateMeshHitBox;
	std::vector<GenerateCollisionOfParticle> linkMeshHitBoxAndParticle;

	std::unique_ptr<CollisionDetectionOfMeshCircleAndCPUHitBox> cpuAndMeshCircleHitBox;
	ComputeParticleAvoidSphere particleAvoidParticle;

	struct MeshParticleData
	{
		BoundingBox bb;
		MeshParticle meshParticle;

		MeshParticleData(
			const ResouceBufferHelper::BufferData &VERT_DATA,
			const UINT &VERT_NUM_ARRAY,
			const InitMeshParticleData &DATA, UINT ID)
			:bb(VERT_DATA, VERT_NUM_ARRAY),
			meshParticle(DATA, ID)
		{
		};
	};
	std::vector<MeshParticleData>meshData;
	std::vector<Sphere*> hitBoxData;



	KazRenderHelper::ID3D12ResourceWrapper copyBuffer;


	//�c�ݍ��݂̃p�[�e�B�N������e�q�֌W�œ�����-----------------------------
	std::vector<const DirectX::XMMATRIX*>motherMatArray;
	std::vector<const ColorData*>colorDataArray;
	ResouceBufferHelper updatePosCompute;
	ResouceBufferHelper::BufferData motherMatrixBuffer;
	ResouceBufferHelper::BufferData colorDataBuffer;
	RESOURCE_HANDLE motherMatHandle, scaleRotateBillboardMatHandle;
	DirectX::XMMATRIX scaleRotaMat, scaleRotBillBoardMat;
	//�c�ݍ��݂̃p�[�e�B�N������e�q�֌W�œ�����-----------------------------

	RESOURCE_HANDLE colorDataHandle;

	ResouceBufferHelper meshMoveCompute;
	RESOURCE_HANDLE inputMeshCircleBufferHandle, outputMeshCircleBufferHandle;
	struct MeshHitBoxData
	{
		DirectX::XMFLOAT3 pos;
		UINT meshID;
		DirectX::XMUINT3 id;
	};

};
