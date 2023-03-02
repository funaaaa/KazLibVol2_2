#include "CameraMgr.h"
#include"../Math/KazMath.h"

CameraMgr::CameraMgr() : orthographicMatProjectionDirtyFlag(orthographicMatProjection)
{

	//2D���W�ϊ�
	orthographicMatProjection = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,
		(float)WIN_X,
		(float)WIN_Y,
		0.0f,
		0.0f,
		1.0f
	);

	for (int i = 0; i < CAMERA_ARRAY_NUM; i++)
	{
		viewArray[i] = DirectX::XMMatrixIdentity();
		viewDirtyFlagArray[i] = std::make_unique<DirtySet>(viewArray[i]);
		billBoardArray[i] = DirectX::XMMatrixIdentity();
		billBoardDirtyFlagArray[i] = std::make_unique<DirtySet>(billBoardArray[i]);
		perspectiveMatArray[i] =
			DirectX::XMMatrixPerspectiveFovLH(
				DirectX::XMConvertToRadians(60.0f),
				static_cast<float>(WIN_X) / static_cast<float>(WIN_Y),
				0.1f,
				700.0f
			);
		perspectiveProjDirtyFlag[i] = std::make_unique<DirtySet>(perspectiveMatArray[i]);
	}
}

void CameraMgr::CameraSetting(float VIEWING_ANGLE, float FAR_SIDE, int CAMERA_INDEX)
{
	perspectiveMatArray[CAMERA_INDEX] =
		DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(VIEWING_ANGLE),
			static_cast<float>(WIN_X) / static_cast<float>(WIN_Y),
			0.1f,
			FAR_SIDE
		);
}

void CameraMgr::Camera(const KazMath::Vec3<float> &EYE_POS, const KazMath::Vec3<float> &TARGET_POS, const KazMath::Vec3<float> &UP, int CAMERA_INDEX)
{
	DirectX::XMFLOAT3 eye = { EYE_POS.x,EYE_POS.y,EYE_POS.z };
	DirectX::XMFLOAT3 target = { TARGET_POS.x,TARGET_POS.y ,TARGET_POS.z };
	DirectX::XMFLOAT3 up = { UP.x,UP.y,UP.z };
	CameraAxis cameraAxis;


	bool eyeAll0 = (eye.x == 0 && eye.y == 0 && eye.z == 0);
	//bool targetAll0 = (target.x == 0 && target.y == 0 && target.z == 0);
	bool eyeEqualTarget = (eye.x == target.x && eye.y == target.y && eye.z == target.z);

	if (eyeAll0 || eyeEqualTarget)
	{
		return;
	}

#pragma region �r���[�s��
	DirectX::XMMATRIX matView;
	// ���_���W
	DirectX::XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// �����_���W
	DirectX::XMVECTOR targetPosition = XMLoadFloat3(&target);
	// �i���́j�����
	DirectX::XMVECTOR upVector = XMLoadFloat3(&up);

	// �J����Z���i���������j
	cameraAxis.z = DirectX::XMVectorSubtract(targetPosition, eyePosition);
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!DirectX::XMVector3Equal(cameraAxis.z, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(cameraAxis.z));
	assert(!DirectX::XMVector3Equal(upVector, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(upVector));
	// �x�N�g���𐳋K��
	cameraAxis.z = DirectX::XMVector3Normalize(cameraAxis.z);

	// �J������X���i�E�����j
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxis.x = DirectX::XMVector3Cross(upVector, cameraAxis.z);
	// �x�N�g���𐳋K��
	cameraAxis.x = DirectX::XMVector3Normalize(cameraAxis.x);

	// �J������Y���i������j
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxis.y = DirectX::XMVector3Cross(cameraAxis.z, cameraAxis.x);

	cameraAxis.upVec = upVector;


	// �J������]�s��
	DirectX::XMMATRIX matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxis.x;
	matCameraRot.r[1] = cameraAxis.y;
	matCameraRot.r[2] = cameraAxis.z;
	matCameraRot.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);


	// �]�u�ɂ��t�s��i�t��]�j���v�Z
	matView = XMMatrixTranspose(matCameraRot);

	// ���_���W��-1���|�������W
	DirectX::XMVECTOR reverseEyePosition = DirectX::XMVectorNegate(eyePosition);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	DirectX::XMVECTOR tX = DirectX::XMVector3Dot(cameraAxis.x, reverseEyePosition);	// X����
	DirectX::XMVECTOR tY = DirectX::XMVector3Dot(cameraAxis.y, reverseEyePosition);	// Y����
	DirectX::XMVECTOR tZ = DirectX::XMVector3Dot(cameraAxis.z, reverseEyePosition);	// Z����
	const int x = 0;
	const int y = 1;
	const int z = 2;
	// ��̃x�N�g���ɂ܂Ƃ߂�
	DirectX::XMVECTOR translation = { tX.m128_f32[x], tY.m128_f32[y], tZ.m128_f32[z],1.0f };
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;
#pragma endregion

	//�S���ʃr���{�[�h
	DirectX::XMMATRIX matBillboard;
	matBillboard.r[0] = cameraAxis.x;
	matBillboard.r[1] = cameraAxis.y;
	matBillboard.r[2] = cameraAxis.z;
	matBillboard.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

	billBoardArray[CAMERA_INDEX] = matBillboard;
#pragma region Y���r���{�[�h�s��
	// �J����X���AY���AZ��
	DirectX::XMVECTOR billCameraAxisX, billCameraAxisY, billCameraAxisZ;

	// X���͋���
	billCameraAxisX = cameraAxis.x;
	// Y���̓��[���h���W�n��Y��
	billCameraAxisY = DirectX::XMVector3Normalize(cameraAxis.upVec);
	// Z����X����Y���̊O�ςŋ��܂�
	billCameraAxisZ = DirectX::XMVector3Cross(billCameraAxisX, billCameraAxisY);

	// Y�����r���{�[�h�s��
	matBillboard.r[0] = billCameraAxisX;
	matBillboard.r[1] = billCameraAxisY;
	matBillboard.r[2] = billCameraAxisZ;
	matBillboard.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

	yBillBoardArray[CAMERA_INDEX] = matBillboard;
#pragma endregion

	viewArray[CAMERA_INDEX] = matView;
	//view = matView;
	//billBoard = matBillboard;
}

DirectX::XMMATRIX CameraMgr::CreateCamera(const KazMath::Vec3<float> &EYE_POS, const KazMath::Vec3<float> &TARGET_POS, const KazMath::Vec3<float> &UP)
{
	DirectX::XMFLOAT3 eye = { EYE_POS.x,EYE_POS.y,EYE_POS.z };
	DirectX::XMFLOAT3 target = { TARGET_POS.x, TARGET_POS.y, TARGET_POS.z };
	DirectX::XMFLOAT3 up = { UP.x,UP.y,UP.z };
	CameraAxis cameraAxis;


	bool eyeAll0 = (eye.x == 0 && eye.y == 0 && eye.z == 0);
	//bool targetAll0 = (target.x == 0 && target.y == 0 && target.z == 0);
	bool eyeEqualTarget = (eye.x == target.x && eye.y == target.y && eye.z == target.z);

	if (eyeAll0 || eyeEqualTarget)
	{
		return DirectX::XMMATRIX();
	}

#pragma region �r���[�s��
	DirectX::XMMATRIX matView;
	// ���_���W
	DirectX::XMVECTOR eyePosition = XMLoadFloat3(&eye);
	// �����_���W
	DirectX::XMVECTOR targetPosition = XMLoadFloat3(&target);
	// �i���́j�����
	DirectX::XMVECTOR upVector = XMLoadFloat3(&up);

	// �J����Z���i���������j
	cameraAxis.z = DirectX::XMVectorSubtract(targetPosition, eyePosition);
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!DirectX::XMVector3Equal(cameraAxis.z, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(cameraAxis.z));
	assert(!DirectX::XMVector3Equal(upVector, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(upVector));
	// �x�N�g���𐳋K��
	cameraAxis.z = DirectX::XMVector3Normalize(cameraAxis.z);

	// �J������X���i�E�����j
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxis.x = DirectX::XMVector3Cross(upVector, cameraAxis.z);
	// �x�N�g���𐳋K��
	cameraAxis.x = DirectX::XMVector3Normalize(cameraAxis.x);

	// �J������Y���i������j
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxis.y = DirectX::XMVector3Cross(cameraAxis.z, cameraAxis.x);

	cameraAxis.upVec = upVector;


	// �J������]�s��
	DirectX::XMMATRIX matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxis.x;
	matCameraRot.r[1] = cameraAxis.y;
	matCameraRot.r[2] = cameraAxis.z;
	matCameraRot.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);


	// �]�u�ɂ��t�s��i�t��]�j���v�Z
	matView = XMMatrixTranspose(matCameraRot);

	// ���_���W��-1���|�������W
	DirectX::XMVECTOR reverseEyePosition = DirectX::XMVectorNegate(eyePosition);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	DirectX::XMVECTOR tX = DirectX::XMVector3Dot(cameraAxis.x, reverseEyePosition);	// X����
	DirectX::XMVECTOR tY = DirectX::XMVector3Dot(cameraAxis.y, reverseEyePosition);	// Y����
	DirectX::XMVECTOR tZ = DirectX::XMVector3Dot(cameraAxis.z, reverseEyePosition);	// Z����
	const int x = 0;
	const int y = 1;
	const int z = 2;
	// ��̃x�N�g���ɂ܂Ƃ߂�
	DirectX::XMVECTOR translation = DirectX::XMVectorSet(tX.m128_f32[x], tY.m128_f32[y], tZ.m128_f32[z], 1.0f);
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;
#pragma endregion

	return matView;
}

DirectX::XMMATRIX CameraMgr::GetViewMatrix(int CAMERA_INDEX)
{
	return viewArray[CAMERA_INDEX];
}

DirectX::XMMATRIX *CameraMgr::GetViewMatrixPointer(int CAMERA_INDEX)
{
	return &viewArray[CAMERA_INDEX];
}

DirectX::XMMATRIX CameraMgr::GetMatBillBoard(int CAMERA_INDEX)
{
	return billBoardArray[CAMERA_INDEX];
}

DirectX::XMMATRIX *CameraMgr::GetMatBillBoardPointer(int CAMERA_INDEX)
{
	return &billBoardArray[CAMERA_INDEX];
}

DirectX::XMMATRIX CameraMgr::GetPerspectiveMatProjection(int CAMERA_INDEX)
{
	return perspectiveMatArray[CAMERA_INDEX];
}

DirectX::XMMATRIX *CameraMgr::GetPerspectiveMatProjectionPointer(int CAMERA_INDEX)
{
	return &perspectiveMatArray[CAMERA_INDEX];
}

DirectX::XMMATRIX CameraMgr::GetOrthographicMatProjection()
{
	return orthographicMatProjection;
}

DirectX::XMMATRIX CameraMgr::GetPerspectiveMatProjectionAngle(float angle)
{
	return DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(angle),
		static_cast<float>(WIN_X) / static_cast<float>(WIN_Y),
		0.1f,
		100000
	);;
}

bool CameraMgr::ViewAndProjDirty(int CAMERA_INDEX)
{
	return viewDirtyFlagArray[CAMERA_INDEX]->Dirty() || perspectiveProjDirtyFlag[CAMERA_INDEX]->Dirty();
}

bool CameraMgr::BillboardDirty(int CAMERA_INDEX)
{
	return billBoardDirtyFlagArray[CAMERA_INDEX]->Dirty();
}

void CameraMgr::Record()
{
	for (int i = 0; i < CAMERA_ARRAY_NUM; ++i)
	{
		viewDirtyFlagArray[i]->Record();
		billBoardDirtyFlagArray[i]->Record();
		perspectiveProjDirtyFlag[i]->Record();
	}
}
