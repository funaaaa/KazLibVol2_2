#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Helper/EnemyHelper.h"

class CameraWork
{
public:
	CameraWork();
	void Init(const KazMath::Vec3<float> &BASE_POS = { 0.0f,3.0f,0.0f });
	void Update(const KazMath::Vec2<float> &CURSOR_VALUE, KazMath::Vec3<float> *PLAYER_POS, bool DEBUG_FLAG);

	void ForceCamera(KazEnemyHelper::CameraDir DIR);

	const KazMath::Vec3<float> GetEyePos();
	const KazMath::Vec3<float> GetTargetPos();

private:

	KazMath::Vec3<float> debugCameraMove;
	KazMath::Vec3<float> eyePos, targetPos;
	KazMath::Vec2<float> angle;

	KazMath::Vec3<float> baseTargetPos;
	KazMath::Vec2<float> leftRightAngleVel;					//���E���_���W�̌����p�x
	KazMath::Vec2<float> upDownAngleVel;					//�㉺���_���W�̌����p�x
	KazMath::Vec2<float> trackLeftRightAngleVel;
	KazMath::Vec2<float> trackUpDownAngleVel;
	KazMath::Vec2<float> forceCameraDirVel;					//�J�����̑O�㍶�E�̌����������I�Ɏw�肷��
	KazMath::Vec2<float> mulValue;							//�J�����̏㉺���E�̊|���銄��
	KazMath::Vec2<float> mulValue2;							//�J�����̏㉺���E�̊|���銄��

	KazMath::Vec3<float> honraiPlayerCameraPos;

	const float FORCE_CAMERA_FRONT = -90.0f;
	const float FORCE_CAMERA_BACK = -270.0f;
	const float FORCE_CAMERA_LEFT = 0.0f;
	const float FORCE_CAMERA_RIGHT = -180.0f;

	//�J���������̊m�F
	BoxPolygonRenderPtr besidePoly, verticlaPoly, cameraPoly;
	KazMath::Vec3<float> centralPos;		//���E��]�̒��S���W
	KazMath::Vec3<float> centralPos2;		//�㉺��]�̒��S���W
	float r;					//���E��]�̉~�̑傫��
	float r2;					//�㉺��]�̉~�̑傫��

	KazMath::Vec3<float> forceCameraAngle;	//�J�����������I�ɑ��̕����Ɍ�������ۂɎg�p����l
};