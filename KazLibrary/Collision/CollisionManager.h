#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"
#include"../Render/KazRender.h"
#include"../KazLibrary/Math/KazMath.h"

struct Triangle
{
	KazMath::Vec3<float> p0;
	KazMath::Vec3<float> p1;
	KazMath::Vec3<float> p2;
	KazMath::Vec3<float> normal;

	void ComputeNormal()
	{
		KazMath::Vec3<float> p0_p1 = p1 - p0;
		KazMath::Vec3<float> p0_p2 = p2 - p0;

		//�O�ςɂ��A2�ӂɐ����ȃx�N�g�����Z�o����
		normal = p0_p1.Cross(p0_p2);
		normal.Normalize();
	}
};

struct Ray
{
	//�n�_���W
	KazMath::Vec3<float> start;
	//����
	KazMath::Vec3<float> dir;

	Ray() :start(), dir({ 1.0f,0.0f,0.0f })
	{};
};

struct Sphere
{
	// ���S���W
	KazMath::Vec3<float>* center;
	// ���a
	float radius;

	Sphere() : radius(1.0f), center(nullptr)
	{};
	Sphere(KazMath::Vec3<float> *CENTER_POS, float RADIUS) :center(CENTER_POS), radius(RADIUS)
	{};

	void operator=(const Sphere &rhs)
	{
		center = rhs.center;
		radius = rhs.radius;
	};
};

struct Plane
{
	// �@���x�N�g��
	KazMath::Vec3<float> normal;
	// ���_(0,0,0)����̋���
	float distance;

	Plane() :normal({ 0.0f,1.0f,0.0f }), distance(0.0f)
	{};
};

struct Square
{
	KazMath::Vec3<float> center;
	KazMath::Vec3<float> size;
};

struct ModiRectangle
{
	/*

		�f�[�^�͉��̊G�̂悤�ɍ���(p0)���玞�v���ɓ���Ă��������B

	   p0___________p1
		|			|
		|			|
	   p3___________p2

	*/

	KazMath::Vec3<float> p0;
	KazMath::Vec3<float> p1;
	KazMath::Vec3<float> p2;
	KazMath::Vec3<float> p3;
	KazMath::Vec3<float> normal;

	void SetNormal()
	{
		KazMath::Vec3<float> p0_p1 = p1 - p0;
		KazMath::Vec3<float> p0_p2 = p2 - p0;
		KazMath::Vec3<float> p0_p3 = p3 - p0;

		//�O�ςɂ��A2�ӂɐ����ȃx�N�g�����Z�o����
		normal = p0_p1.Cross(p0_p2);
		normal.Normalize();
	}
};

class CollisionManager :public ISingleton<CollisionManager> {
public:
	CollisionManager();
	~CollisionManager();

	/// <summary>
	/// �Ƌ��̔�������܂�
	/// </summary>
	/// <param name="SPHERE">���f�[�^</param>
	/// <param name="PLANE">�f�[�^</param>
	/// <param name="INTER">��_</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckSphereAndPlane(const Sphere& SPHERE, const Plane& PLANE, KazMath::Vec3<float>* INTER = nullptr);

	/// <summary>
	/// ���ƎO�p�`�̔�������܂�
	/// </summary>
	/// <param name="SPHERE">���f�[�^</param>
	/// <param name="TRIANGLE">�O�p�`�f�[�^</param>
	/// <param name="INTER">��_</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckSphereAndTriangle(const Sphere& SPHERE, const Triangle& TRIANGLE, KazMath::Vec3<float> *INTER = nullptr);

	/// <summary>
	/// ���Ɣ̔�������܂�
	/// </summary>
	/// <param name="RAY">���f�[�^</param>
	/// <param name="PLANE">�f�[�^</param>
	/// <param name="DISTANCE">����</param>
	/// <param name="INTER">��_</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckRayAndPlane(const Ray& RAY, const Plane& PLANE, float* DISTANCE, KazMath::Vec3<float> * INTER);

	/// <summary>
	/// ���ƎO�p�`�̔�������܂�
	/// </summary>
	/// <param name="RAY">���f�[�^</param>
	/// <param name="TRIANGLE">�O�p�`�f�[�^</param>
	/// <param name="DISTANCE">����</param>
	/// <param name="INTER">��_</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckRayAndTriange(const Ray& RAY, const Triangle& TRIANGLE, float* DISTANCE = nullptr, KazMath::Vec3<float> * INTER = nullptr);

	/// <summary>
	/// ���Ƌ��̔�������܂�
	/// </summary>
	/// <param name="RAY">���f�[�^</param>
	/// <param name="SPHERE">���f�[�^</param>
	/// <param name="DISTANCE">����</param>
	/// <param name="INTER">��_</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckRayAndSphere(const Ray& RAY, const Sphere& SPHERE, float* DISTANCE = nullptr, KazMath::Vec3<float> * INTER = nullptr);


	/// <summary>
	/// ���Ƌ��̔�������܂�
	/// </summary>
	/// <param name="SPHERE_1">���f�[�^1</param>
	/// <param name="SPHERE_2">���f�[�^2</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckSphereAndSphere(const Sphere& SPHERE_1, const Sphere& SPHERE_2);

	/// <summary>
	/// �����`�ƒ����`�̔�������܂�
	/// </summary>
	/// <param name="SQUARE_1">�l�p�f�[�^1</param>
	/// <param name="SQUARE_2">�l�p�f�[�^2</param>
	/// <returns>true...�Փ˂���,false...�Փ˂��Ă��Ȃ�</returns>
	bool CheckSquareAndSquare(const Square& SQUARE_1, const Square& SQUARE_2);

	/// <summary>
	/// ��]������`�Ƌ��̔�������܂�
	/// </summary>
	/// <param name="SPHERE"></param>
	/// <param name="RAY"></param>
	/// <returns></returns>
	bool CheckThicklineAndSphere(const Sphere& SPHERE, const ModiRectangle& RECTANGLE);


	std::array<KazMath::Vec2<float>, 2> CheckCircleAndRay(const Sphere &SPHERE, const KazMath::Vec2<float> &START_POS, const KazMath::Vec2<float> &END_POS);

	/// <summary>
	/// �|���Ɛ����̔���
	/// </summary>
	bool CheckRayAndPlane3D(const Ray &RAY, const ModiRectangle &MODI, float *DISTANCE = nullptr);


	//�����Ɛ����̓����蔻��
	bool IsIntersected(const KazMath::Vec3<float> &START_POS_A, const KazMath::Vec3<float> &END_POS_A, const KazMath::Vec3<float> &START_POS_B, const KazMath::Vec3<float> &END_POS_B);

	// �����Ɛ����̌�_�����߂鏈��
	KazMath::Vec3<float> CalIntersectPoint(const KazMath::Vec3<float> &START_POS_A, const KazMath::Vec3<float> &END_POS_A, const KazMath::Vec3<float> &START_POS_B, const KazMath::Vec3<float> &END_POS_B);

	friend ISingleton<CollisionManager>;
private:

	//�_�ƎO�p�`�̍ŋߐړ_
	void ClosestPtPoint2Triangle(const KazMath::Vec3<float> &point, const Triangle &triangle, KazMath::Vec3<float> *closest);


	float CaluCross(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b, DirectX::XMFLOAT2 c)
	{
		return (b.x - a.x) * ((c.y * -1) - (a.y * -1)) - (c.x - a.x) * ((b.y * -1) - (a.y * -1));
	}
};

