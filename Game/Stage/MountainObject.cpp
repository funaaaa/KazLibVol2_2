#include "MountainObject.h"
#include"../KazLibrary/Easing/easing.h"

MountainObject::MountainObject()
{
}

void MountainObject::Init(const KazMath::Transform3D &TRANSFORM, RESOURCE_HANDLE HANDLE, const int *CAMERA_ID_PTR)
{
	for (int objectIndex = 0; objectIndex < floorObjectRender.objRender.size(); ++objectIndex)
	{
		//�I�u�W�F�N�g�̏�Ԃ̏�����
		floorObjectRender.objRender[objectIndex].data.transform = TRANSFORM;
		floorObjectRender.objRender[objectIndex].data.handle = HANDLE;
		floorObjectRender.objRender[objectIndex].data.pipelineName = PIPELINE_NAME_OBJ_WIREFLAME_FOG;

		//�o�b�t�@�̐���
		RESOURCE_HANDLE lHandle = floorObjectRender.objRender[objectIndex].CreateConstBuffer(sizeof(FogData), typeid(FogData).name(), GRAPHICS_RANGE_TYPE_CBV_VIEW, GRAPHICS_PRAMTYPE_DATA);
		KazMath::Color lColor(29, 19, 72, 255);
		DirectX::XMFLOAT3 colorRate = lColor.ConvertColorRateToXMFLOAT3();
		FogData lData;
		lData.fogdata = { colorRate.x,colorRate.y,colorRate.z,0.0006f };
		floorObjectRender.objRender[objectIndex].TransData(&lData, lHandle, typeid(lData).name());
	}

	floorObjectRender.initScale = TRANSFORM.scale;

	floorObjectRender.objRender[0].data.colorData = KazMath::Color(213, 5, 228, 255);
	floorObjectRender.objRender[1].data.colorData = KazMath::Color(248, 58, 16, 255);

	scaleRate = 0.0f;
	reversValueFlag = false;
	cameraId = CAMERA_ID_PTR;
}

void MountainObject::Update()
{
	//if (reversValueFlag)
	//{
	//	scaleRate += 1.0f / 30.0f;
	//}
	//else
	//{
	//	scaleRate += -(1.0f / 10.0f);
	//}

	//if (scaleRate <= 0.0f)
	//{
	//	scaleRate = 0.0f;
	//	reversValueFlag = true;
	//}
	//else if (1.0f <= scaleRate)
	//{
	//	scaleRate = 1.0f;
	//	reversValueFlag = false;
	//}


	//�ړ���
	float lVelZ = -5.0f;
	//�I�u�W�F�N�g�̈ړ�
	floorObjectRender.objRender[0].data.transform.pos.z += lVelZ;

	//Y���̃X�P�[���ω�
	floorObjectRender.objRender[0].data.transform.scale.y = floorObjectRender.initScale.y + EasingMaker(Out, Cubic, scaleRate) * 0.2f;
	//Y���̍��W�Œ�
	floorObjectRender.objRender[0].data.transform.pos.y = -150.0f + floorObjectRender.objRender[0].data.transform.scale.y;

	//��̃I�u�W�F�N�g�̏�Ԃɍ��킹�Ă��牺�̃I�u�W�F�N�g�̏�Ԃ�ς���
	floorObjectRender.objRender[1].data.transform = floorObjectRender.objRender[0].data.transform;
	floorObjectRender.objRender[1].data.transform.rotation.x = 180.0f;

	//���[�v
	bool limitZLineFlag = floorObjectRender.objRender[0].data.transform.pos.z <= -9000.0f;
	if (limitZLineFlag)
	{
		floorObjectRender.objRender[0].data.transform.pos.z = 9000.0f;
		floorObjectRender.objRender[1].data.transform.pos = floorObjectRender.objRender[0].data.transform.pos + KazMath::Vec3<float>(0.0f, -(floorObjectRender.objRender[0].data.transform.scale.y * 2), 0.0f);
	}
	floorObjectRender.objRender[0].data.cameraIndex.id = *cameraId;
	floorObjectRender.objRender[1].data.cameraIndex.id = *cameraId;
}

void MountainObject::Draw()
{
	for (int objectIndex = 0; objectIndex < floorObjectRender.objRender.size(); ++objectIndex)
	{
		floorObjectRender.objRender[objectIndex].Draw();
	}
}
