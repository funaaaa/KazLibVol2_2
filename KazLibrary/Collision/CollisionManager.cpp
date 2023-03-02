#include"../Collision/CollisionManager.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::CheckSphereAndPlane(const Sphere &SPHERE, const Plane &PLANE, KazMath::Vec3<float> *INTER)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	float distV = SPHERE.center->Dot(PLANE.normal);

	//���ʂ̌��_���������Z���鎖�ŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV - PLANE.distance;

	//�����̐�Βl�����a���傫����Γ������ĂȂ�
	if (fabs(dist) > SPHERE.radius)
	{
		return false;
	}


	//�^�����_���v�Z
	if (INTER)
	{
		//���ʏ�̋^���ړ_���A�^����_�Ƃ���
		*INTER = PLANE.normal * -dist + *SPHERE.center;
	}
	return true;
}

bool CollisionManager::CheckSphereAndTriangle(const Sphere &SPHERE, const Triangle &TRIANGLE, KazMath::Vec3<float> *INTER)
{
	KazMath::Vec3<float> p;

	//���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(*SPHERE.center, TRIANGLE, &p);

	//�_p�Ƌ��̒��S�̍����x�N�g��
	KazMath::Vec3<float> v = p - *SPHERE.center;

	//�����̓������߂�
	//(�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����)
	float d = v.Dot(v);

	//���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (d > SPHERE.radius * SPHERE.radius)
	{
		return false;
	}

	if (INTER)
	{
		//�O�p�`��̍ŋߐړ_
		*INTER = p;
	}

	return false;
}

bool CollisionManager::CheckRayAndPlane(const Ray &RAY, const Plane &PLANE, float *DISTANCE, KazMath::Vec3<float> *INTER)
{
	//�C�v�V�I��...�덷�z���p��0�ɋ߂������Ȓl
	const float epsion = 1.0 - 5.0f;
	//�ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = PLANE.normal.Dot(RAY.dir);
	//���ʂɓ�����Ȃ�
	if (d1 > -epsion)
	{
		return false;
	}

	//�n�_�ƕ��ʂ̋���(���ʂ̖@������)
	//�ʖ@���ƃ��C�̎n�_���W(�ʒu�x�N�g��)�̓���
	float d2 = PLANE.normal.Dot(RAY.start);

	//�n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - PLANE.distance;

	//�n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;

	//��_���n�_�����ɂ���̂ŁA������Ȃ�
	if (t < 0)
	{
		return false;
	}

	//��������������
	if (DISTANCE)
	{
		*DISTANCE = t;
	}

	//��_���v�Z
	if (INTER)
	{
		*INTER = RAY.start + RAY.dir * t;
	}

	return true;
}

bool CollisionManager::CheckRayAndTriange(const Ray &RAY, const Triangle &TRIANGLE, float *DISTANCE, KazMath::Vec3<float> *INTER)
{
	//�O�p�`���ڂ��Ă��镽�ʂ��Z�o
	Plane plane;
	KazMath::Vec3<float> interPlane;

	plane.normal = TRIANGLE.normal;
	plane.distance = TRIANGLE.normal.Dot(TRIANGLE.p0);

	//Ray�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!CheckRayAndPlane(RAY, plane, DISTANCE, &interPlane))
	{
		return false;
	}

	//Ray�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ
	//Ray�ƕ��ʂ̍D�]���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0 - 5.0f;

	//��p0_p1�ɂ���
	KazMath::Vec3<float> pt_p0 = TRIANGLE.p0 - interPlane;
	KazMath::Vec3<float> p0_p1 = TRIANGLE.p1 - TRIANGLE.p0;
	KazMath::Vec3<float> m;
	m = pt_p0.Cross(p0_p1);

	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (m.Dot(TRIANGLE.normal) < -epsilon)
	{
		return false;
	}


	//��p1_p2�ɂ���
	KazMath::Vec3<float> pt_p1 = TRIANGLE.p1 - interPlane;
	KazMath::Vec3<float> p1_p2 = TRIANGLE.p2 - TRIANGLE.p1;
	m = pt_p1.Cross(p1_p2);

	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (m.Dot(TRIANGLE.normal) < -epsilon)
	{
		return false;
	}


	//��p2_p0�ɂ���
	KazMath::Vec3<float> pt_p2 = TRIANGLE.p2 - interPlane;
	KazMath::Vec3<float> p2_p0 = TRIANGLE.p0 - TRIANGLE.p2;
	m = pt_p2.Cross(p2_p0);

	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (m.Dot(TRIANGLE.normal) < -epsilon)
	{
		return false;
	}


	if (INTER)
	{
		*INTER = interPlane;
	}

	return true;
}

bool CollisionManager::CheckRayAndSphere(const Ray &RAY, const Sphere &SPHERE, float *DISTANCE, KazMath::Vec3<float> *INTER)
{
	KazMath::Vec3<float> m = RAY.start - *SPHERE.center;
	float b = m.Dot(RAY.dir);
	float c = m.Dot(m) - SPHERE.radius * SPHERE.radius;

	//Ray�̎n�_��Sphere�̊O���ɂ���(c>0),Ray��Sphere���痣��Ă����������w���Ă���ꍇ(b>0),������Ȃ�
	if (c > 0.0f && b > 0.0f)
	{
		return false;
	}

	float discr = b * b - c;
	//���̔��ʎ���Ray�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f)
	{
		return false;
	}


	//Ray�͋��ƌ������Ă���
	//��������ŏ��̒lT���v�Z
	float t = -b - sqrtf(discr);

	//t�����ł���ꍇ�ARay�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	//�N�����v(�l��͈͓��ɓ����)
	if (t < 0)
	{
		t = 0.0f;
	}

	if (DISTANCE)
	{
		*DISTANCE = t;
	}

	if (INTER)
	{
		*INTER = RAY.start + RAY.dir * t;
	}
	return true;
}

bool CollisionManager::CheckSphereAndSphere(const Sphere &SPHERE_1, const Sphere &SPHERE_2)
{
	float distance = SPHERE_1.center->Distance(*SPHERE_2.center);
	float sumRadist = SPHERE_1.radius + SPHERE_2.radius;

	return (distance <= sumRadist);
}

bool CollisionManager::CheckSquareAndSquare(const Square &SQUARE_1, const Square &SQUARE_2)
{
	std::array<KazMath::Vec3<float>, 2> size = { SQUARE_1.size / 2.0f,SQUARE_2.size / 2.0f };
	KazMath::Vec3<float>distance = SQUARE_1.center - SQUARE_2.center;
	const int square1 = 0;
	const int square2 = 1;
	bool isHitFlag = fabs(distance.x) <= size[square1].x + size[square2].x && fabs(distance.y) <= size[square1].y + size[square2].y;
	return isHitFlag;
}

bool CollisionManager::CheckThicklineAndSphere(const Sphere &SPHERE, const ModiRectangle &RECTANGLE)
{
	/*

	�Z ����̗���

	�E ��`���~�̔��a���A�אڂ��钸�_���牄�т�����Ɋg������B

		 ��			 ��
	  �� p0___________p1 ��
		 |			 |
		 |			 |
	  �� p3___________p2 ��
		 ��			 ��

	�E �~�̒��S�_�Ƌ�`�ƂŊO�ς��g�������E������g�p���āA�����蔻����s���B


	*���� �c�@��`�͍����p0�ɂ��āA���v���̏��ԂɃf�[�^�����Ă��������B

	*/



	// Z���̏���Y���ɓ����B

	Sphere sphereBuff = SPHERE;
	ModiRectangle rectangleBuff = RECTANGLE;

	sphereBuff.center->y = SPHERE.center->z;
	sphereBuff.center->z = SPHERE.center->y;

	rectangleBuff.p0.y = RECTANGLE.p0.z;
	rectangleBuff.p0.z = RECTANGLE.p0.y;

	rectangleBuff.p1.y = RECTANGLE.p1.z;
	rectangleBuff.p1.z = RECTANGLE.p1.y;

	rectangleBuff.p2.y = RECTANGLE.p2.z;
	rectangleBuff.p2.z = RECTANGLE.p2.y;

	rectangleBuff.p3.y = RECTANGLE.p3.z;
	rectangleBuff.p3.z = RECTANGLE.p3.y;


	// ��`�f�[�^��ۑ�����B
	ModiRectangle rectangleBuffBuff = rectangleBuff;

	// �~�̔��a��ۑ�����ϐ�
	KazMath::Vec3<float> sphereRadius = { sphereBuff.radius, sphereBuff.radius, sphereBuff.radius };

	// ���_���v�Z����ۂɃf�[�^���ꎞ�ۑ�����ϐ�
	KazMath::Vec3<float> dataBuff = {};
	KazMath::Vec3<float> dataBuff2 = {};


	/*-- ��`���g������B --*/

	// p0�Ɋւ��Ċg���������s���B
	{

		// p1����p0�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff = rectangleBuffBuff.p0 - rectangleBuffBuff.p1;
		dataBuff.Normalize();
		// p3����p0�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff2 = rectangleBuffBuff.p0 - rectangleBuffBuff.p3;
		dataBuff2.Normalize();

		//// �����Đ��K������
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p0 += dataBuff * sphereRadius;
		rectangleBuffBuff.p0 += dataBuff2 * sphereRadius;

	}

	// p1�Ɋւ��Ċg���������s���B
	{

		// p2����p1�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff = rectangleBuffBuff.p1 - rectangleBuffBuff.p2;
		dataBuff.Normalize();
		// p0����p1�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff2 = rectangleBuffBuff.p1 - rectangleBuffBuff.p0;
		dataBuff2.Normalize();

		//// �����Đ��K������
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p1 += dataBuff * sphereRadius;
		rectangleBuffBuff.p1 += dataBuff2 * sphereRadius;

	}

	// p2�Ɋւ��Ċg���������s���B
	{

		// p3����p2�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff = rectangleBuffBuff.p2 - rectangleBuffBuff.p3;
		dataBuff.Normalize();
		// p1����p2�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff2 = rectangleBuffBuff.p2 - rectangleBuffBuff.p1;
		dataBuff2.Normalize();

		//// �����Đ��K������
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p2 += dataBuff * sphereRadius;
		rectangleBuffBuff.p2 += dataBuff2 * sphereRadius;

	}

	// p3�Ɋւ��Ċg���������s���B
	{

		// p0����p1�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff = rectangleBuffBuff.p3 - rectangleBuffBuff.p0;
		dataBuff.Normalize();
		// p2����p3�܂ł̕����x�N�g�������߁A���K��������B
		dataBuff2 = rectangleBuffBuff.p3 - rectangleBuffBuff.p2;
		dataBuff2.Normalize();

		//// �����Đ��K������
		//dataBuff += dataBuff2;
		//dataBuff = XMVector2Normalize(dataBuff);

		rectangleBuffBuff.p3 += dataBuff * sphereRadius;
		rectangleBuffBuff.p3 += dataBuff2 * sphereRadius;

	}

	/*-- �g��������`�Ɖ~�̒��S�_���O�ς̍��E������g���ē����蔻����s���B --*/

	// �����ۑ�����悤�̕ϐ�
	std::vector<bool> isHit;

	// �O�ϔ��������ۂɃx�N�g����ۑ����邽�߂̕ϐ�
	KazMath::Vec3<float> vTvVec = {};	// ���_�ƒ��_�̃x�N�g��
	KazMath::Vec3<float> vTPVec = {};	// ���_�ƒ��S�_�̃x�N�g��

	// p0->p1�ł̍��E����
	{

		// ���_�ƒ��_�x�N�g�������߂Đ��K��
		vTvVec = rectangleBuffBuff.p1 - rectangleBuffBuff.p0;
		vTvVec.Normalize();
		// ���_�ƒ��S�_�̃x�N�g�������߂Đ��K��
		vTPVec = *sphereBuff.center - rectangleBuffBuff.p0;
		vTPVec.Normalize();

		// �O�ς��v�Z +�̏ꍇ�E����
		isHit.push_back(vTvVec.Cross(vTPVec).x < 0);

	}

	// p1->p2�ł̍��E����
	{

		// ���_�ƒ��_�x�N�g�������߂Đ��K��
		vTvVec = rectangleBuffBuff.p2 - rectangleBuffBuff.p1;
		vTvVec.Normalize();
		// ���_�ƒ��S�_�̃x�N�g�������߂Đ��K��
		vTPVec = *sphereBuff.center - rectangleBuffBuff.p1;
		vTPVec.Normalize();

		// �O�ς��v�Z +�̏ꍇ�E����
		isHit.push_back(vTvVec.Cross(vTPVec).x < 0);

	}

	// p2->p3�ł̍��E����
	{

		// ���_�ƒ��_�x�N�g�������߂Đ��K��
		vTvVec = rectangleBuffBuff.p3 - rectangleBuffBuff.p2;
		vTvVec.Normalize();
		// ���_�ƒ��S�_�̃x�N�g�������߂Đ��K��
		vTPVec = *sphereBuff.center - rectangleBuffBuff.p2;
		vTPVec.Normalize();

		// �O�ς��v�Z +�̏ꍇ�E����
		isHit.push_back(vTvVec.Cross(vTPVec).x < 0);

	}

	// p3->p0�ł̍��E����
	{

		// ���_�ƒ��_�x�N�g�������߂Đ��K��
		vTvVec = rectangleBuffBuff.p0 - rectangleBuffBuff.p3;
		vTvVec.Normalize();
		// ���_�ƒ��S�_�̃x�N�g�������߂Đ��K��
		vTPVec = *sphereBuff.center - rectangleBuffBuff.p3;
		vTPVec.Normalize();

		// �O�ς��v�Z +�̏ꍇ�E����
		isHit.push_back(vTvVec.Cross(vTPVec).x < 0);

	}

	// ���ʂ��v�Z
	bool isHitResult = true;
	for (int i = 0; i < 4; ++i)
	{
		isHitResult = isHit[i] && isHitResult;
	}


	return isHitResult;
}

std::array<KazMath::Vec2<float>, 2> CollisionManager::CheckCircleAndRay(const Sphere &SPHERE, const KazMath::Vec2<float> &START_POS, const KazMath::Vec2<float> &END_POS)
{
	//ax + by + c = 0 �̒萔��
	float a = END_POS.y - START_POS.y;
	float b = START_POS.x - END_POS.x;
	float c = -a * START_POS.x - b * START_POS.y;

	//�~�̒��S���璼���܂ł̋���
	//mag(a, b) = ��a^2+b^2
	float d = abs((a * SPHERE.center->x + b * SPHERE.center->y + c) / (sqrtf(a * a) + b * b));

	//�����̐�����X���ƕ��s�Ȑ����Ȃ��p�x��
	float theta = atan2(b, a);

	if (d > SPHERE.radius)
	{
		std::array<KazMath::Vec2<float>, 2>resultArray;
		return resultArray;
	}
	else if (d == SPHERE.radius)
	{
		//�ꍇ�킯
		if (a * SPHERE.center->x + b * SPHERE.center->y + c > 0)
		{
			theta += KazMath::PI_2F;
		}

		float crossX = SPHERE.radius * cos(theta) + SPHERE.center->x;
		float crossY = SPHERE.radius * sin(theta) + SPHERE.center->y;

		KazMath::Vec2<float> result(crossX, crossY);
		std::array<KazMath::Vec2<float>, 2>resultArray;
		resultArray[0] = result;
		return resultArray;
	}
	else
	{
		KazMath::Vec2<float> result1;
		KazMath::Vec2<float> result2;

		//alpha��beta�̊p�x�����߂�
		float alpha, beta, phi;
		phi = acos(d / SPHERE.radius);
		alpha = theta - phi;
		beta = theta + phi;

		//�ꍇ�킯
		if (a * SPHERE.center->x + b * SPHERE.center->y + c > 0) {
			alpha += KazMath::PI_2F;
			beta += KazMath::PI_2F;
		}

		//��_�̍��W�����߂�
		result1.x = SPHERE.radius * cos(alpha) + SPHERE.center->x;
		result1.y = SPHERE.radius * sin(alpha) + SPHERE.center->y;

		result2.x = SPHERE.radius * cos(beta) + SPHERE.center->x;
		result2.y = SPHERE.radius * sin(beta) + SPHERE.center->y;


		std::array<KazMath::Vec2<float>, 2>resultArray;
		resultArray[0] = result1;
		resultArray[1] = result2;

		return resultArray;
	}
}

bool CollisionManager::CheckRayAndPlane3D(const Ray &RAY, const ModiRectangle &MODI, float *DISTANCE)
{
	//�O�p�`���ڂ��Ă��镽�ʂ��Z�o
	std::array<Triangle, 2> lTriangle;

	lTriangle[0].p0 = MODI.p0;
	lTriangle[0].p1 = MODI.p1;
	lTriangle[0].p2 = MODI.p2;
	lTriangle[0].ComputeNormal();

	lTriangle[1].p0 = MODI.p2;
	lTriangle[1].p1 = MODI.p3;
	lTriangle[1].p2 = MODI.p1;
	lTriangle[1].ComputeNormal();

	//Ray�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (CheckRayAndTriange(RAY, lTriangle[0], DISTANCE, nullptr) || CheckRayAndTriange(RAY, lTriangle[1], DISTANCE, nullptr))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CollisionManager::IsIntersected(const KazMath::Vec3<float> &START_POS_A, const KazMath::Vec3<float> &END_POS_A, const KazMath::Vec3<float> &START_POS_B, const KazMath::Vec3<float> &END_POS_B)
{
	//--�����̊O�ς��v�Z���Č���������s��--
	//���� ����A���猩��B�̌�������
	KazMath::Vec3<float> lBuffA = END_POS_A - START_POS_A;
	lBuffA.Normalize();
	KazMath::Vec3<float> lBuffB = START_POS_B - START_POS_A;
	lBuffB.Normalize();
	KazMath::Vec3<float> lBuffC = lBuffA;
	lBuffC.Normalize();
	KazMath::Vec3<float> lBuffD = END_POS_B - START_POS_A;
	lBuffD.Normalize();

	KazMath::Vec3<float> lBuffE = lBuffA.Cross(lBuffB);
	KazMath::Vec3<float> lBuffF = lBuffC.Cross(lBuffD);
	KazMath::Vec3<float> lResult1 = lBuffE * lBuffF;

	//���� ����B���猩��A�̌�������
	lBuffA = END_POS_B - END_POS_A;
	lBuffA.Normalize();
	lBuffB = START_POS_A - START_POS_B;
	lBuffB.Normalize();
	lBuffC = lBuffA;
	lBuffC.Normalize();
	lBuffD = END_POS_A - START_POS_A;
	lBuffD.Normalize();

	lBuffE = lBuffA.Cross(lBuffB);
	lBuffF = lBuffC.Cross(lBuffD);
	KazMath::Vec3<float> lResult2 = lBuffE * lBuffF;

	//�������������Ă��鎞�́A�������猩�Č������肵���������̒[�_2�������ɂ��鎞�B
	//�O�ςō��E���������ƁA�������Ă��鎞�͒l�̌��ʂ�+��-�ɂȂ�B
	//�܂藼���̊O�ς��|���Č��ʂ�-�ɂȂ����ꍇ�̂݌������Ă���B
	//����A����݂�B�𔻒�A����B����݂�A�𔻒�Ɠ�ʂ蔻����s���B
	//����2�̔��茋�ʂ��|��������-�A�Ⴕ����0�̎�2�̐����͌������Ă���B
	if (lResult1.x < 0 && lResult2.x < 0)
	{
		return true;
	}
	return false;
}

KazMath::Vec3<float> CollisionManager::CalIntersectPoint(const KazMath::Vec3<float> &START_POS_A, const KazMath::Vec3<float> &END_POS_A, const KazMath::Vec3<float> &START_POS_B, const KazMath::Vec3<float> &END_POS_B)
{
	//��_�����߂� ���̎��͎������̂܂�
	KazMath::Vec3<float> lBuff = END_POS_B - START_POS_B;
	double lD1 = fabs(lBuff.Cross(START_POS_A - START_POS_B).x);
	double lD2 = fabs(lBuff.Cross(END_POS_A - START_POS_B).x);
	double lT = lD1 / (lD1 + lD2);
	return START_POS_A + (END_POS_A - START_POS_A) * static_cast<float>(lT);
}

void CollisionManager::ClosestPtPoint2Triangle(const KazMath::Vec3<float> &point, const Triangle &triangle, KazMath::Vec3<float> *closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	KazMath::Vec3<float> p0_p1 = triangle.p1 - triangle.p0;
	KazMath::Vec3<float> p0_p2 = triangle.p2 - triangle.p0;
	KazMath::Vec3<float> p0_pt = point - triangle.p0;

	float d1 = p0_p1.Dot(p0_pt);
	float d2 = p0_p2.Dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	KazMath::Vec3<float> p1_pt = point - triangle.p1;

	float d3 = p0_p1.Dot(p1_pt);
	float d4 = p0_p2.Dot(p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + p0_p1 * v;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	KazMath::Vec3<float> p2_pt = point - triangle.p2;

	float d5 = p0_p1.Dot(p2_pt);
	float d6 = p0_p2.Dot(p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + p0_p2 * w;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + (triangle.p2 - triangle.p1) * w;
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}
