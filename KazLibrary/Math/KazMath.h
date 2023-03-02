#pragma once
#include"../DirectXCommon/Base.h"
#include<cmath>
#pragma warning(push,0)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
#include"fbxsdk.h"
#pragma warning(pop)
#include"../Helper/DirtyFlag.h"

namespace KazMath
{

#pragma region Vec2
	template<typename T>
	struct Vec2
	{
		T x, y;

		Vec2()
		{
			x = 0;
			y = 0;
		};
		Vec2(T X, T Y) :x(X), y(Y) {};

		float Length()const
		{
			return sqrtf(powf(x, 2) + powf(y, 2));
		};
		float Distance(const Vec2 &To)const
		{
			return sqrt(pow(To.x - x, 2) + pow(To.y - y, 2));
		};
		Vec2<float> GetNormal()const
		{
			float len = Length();
			if (len == 0.0f)return Vec2<float>(0.0, 0.0);
			return Vec2<float>(x / len, y / len);
		};
		void Normalize()
		{
			float len = Length();
			x /= len;
			y /= len;
		};
		DirectX::XMFLOAT2 ConvertXMFLOAT2()
		{
			return DirectX::XMFLOAT2(static_cast<float>(x), static_cast<float>(y));
		};
		DirectX::XMVECTOR ConvertXMVECTOR()
		{
			DirectX::XMVECTOR result = { static_cast<float>(x), static_cast<float>(y), 0.0f, 0.0f };
			return result;
		};

		Vec2<int>Int()const
		{
			return Vec2<int>(static_cast<int>(x), static_cast<int>(y));
		}
		Vec2<float>Float()const
		{
			return Vec2<float>((float)x, (float)y);
		}

		float Dot(const Vec2<float> &rhs)const
		{
			auto me = Float();
			return me.x * rhs.x + me.y * rhs.y;
		}
		float Cross(const Vec2<float> &rhs)const
		{
			auto me = Float();
			return me.x * rhs.y - me.y * rhs.x;
		}

#pragma region �I�y���[�^�[���Z�q
		Vec2 operator-() const
		{
			return Vec2(-x, -y);
		}
		Vec2 operator+(const Vec2 &rhs) const
		{
			return Vec2(x + rhs.x, y + rhs.y);
		};
		Vec2 operator+(const float &rhs) const
		{
			return Vec2(x + rhs, y + rhs);
		};
		Vec2 operator-(const Vec2 &rhs)const
		{
			return Vec2(x - rhs.x, y - rhs.y);
		};
		Vec2 operator*(const Vec2 &rhs)const
		{
			return Vec2(x * rhs.x, y * rhs.y);
		};
		Vec2 operator*(const float &rhs)const
		{
			return Vec2(x * rhs, y * rhs);
		};
		Vec2 operator/(const Vec2 &rhs)const
		{
			return Vec2(x / rhs.x, y / rhs.y);
		};
		Vec2 operator/(const float &rhs)const
		{
			return Vec2(x / rhs, y / rhs);
		};
		Vec2 operator%(const Vec2 &rhs) const
		{
			return Vec2(fmodf(x, rhs.x), fmodf(y, rhs.y));
		};
		void operator=(const Vec2 &rhs)
		{
			x = rhs.x;
			y = rhs.y;
		};
		bool operator==(const Vec2 &rhs)const
		{
			return (x == rhs.x && y == rhs.y);
		};
		bool operator!=(const Vec2 &rhs)const
		{
			return !(*this == rhs);
		};
		void operator+=(const Vec2 &rhs)
		{
			x += rhs.x;
			y += rhs.y;
		};
		void operator-=(const Vec2 &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
		};
		void operator*=(const Vec2 &rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
		};
		void operator/=(const Vec2 &rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
		};
		void operator%=(const Vec2 &rhs)
		{
			x = fmodf(x, rhs.x);
			y = fmodf(y, rhs.y);
		};

		void operator+=(const float &rhs)
		{
			x += rhs;
			y += rhs;
		};
		void operator-=(const float &rhs)
		{
			x -= rhs;
			y -= rhs;
		};
		void operator*=(const float &rhs)
		{
			x *= rhs;
			y *= rhs;
		};
		void operator/=(const float &rhs)
		{
			x /= rhs;
			y /= rhs;
		};
		void operator%=(const float &rhs)
		{
			x = fmodf(x, rhs);
			y = fmodf(y, rhs);
		};

		bool operator<(const Vec2<T> &rhs)
		{
			return x < rhs.x &&y < rhs.y;
		}
#pragma endregion
	};
#pragma endregion

#pragma region Vec3
	template<typename T>
	struct Vec3
	{
		T x, y, z;

		Vec3()
		{
			x = 0;
			y = 0;
			z = 0;
		};
		Vec3(T X, T Y, T Z) :x(X), y(Y), z(Z) {};
		float Length() const
		{
			return static_cast<float>(sqrt(pow(x, 2.0f) + pow(y, 2.0f) + pow(z, 2.0f)));
		};
		Vec3(Vec2<T>XY, T Z) :x(XY.x), y(XY.y), z(Z) {};

		float Distance(const Vec3 &To)const
		{
			Vec3 lpow;
			lpow.x = { static_cast<float>(pow(To.x - x, 2.0f)) };
			lpow.y = { static_cast<float>(pow(To.y - y, 2.0f)) };
			lpow.z = { static_cast<float>(pow(To.z - z, 2.0f)) };


			return static_cast<float>(sqrt(lpow.x + lpow.y + lpow.z));
		};
		Vec3<float> GetNormal()const
		{
			float len = Length();
			if (len == 0.0f)return Vec3(0, 0, 0);
			return Vec3<float>(x / len, y / len, z / len);
		};
		void Normalize()
		{
			float len = Length();
			x /= len;
			y /= len;
			z /= len;
		};
		DirectX::XMFLOAT3 ConvertXMFLOAT3()const
		{
			return DirectX::XMFLOAT3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
		};
		DirectX::XMFLOAT4 ConvertXMFLOAT4()const
		{
			return DirectX::XMFLOAT4(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.0f);
		};
		DirectX::XMVECTOR ConvertXMVECTOR()const
		{
			DirectX::XMVECTOR result = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.0f };
			return result;
		};
		Vec2<T> ConvertVec2()const
		{
			Vec2<T> result = { x,y };
			return result;
		};

		Vec3<int>Int()const
		{
			return Vec3<int>(x, y, z);
		}
		Vec3<float>Float()const
		{
			return Vec3<float>((float)x, (float)y, (float)z);
		}

		float Dot(const Vec3<float> &rhs)const
		{
			auto me = Float();
			return me.x * rhs.x + me.y * rhs.y + me.z * rhs.z;
		}
		Vec3<float>Cross(const Vec3<float> &rhs)const
		{
			auto me = Float();
			return Vec3<float>(
				me.y * rhs.z - rhs.y * me.z,
				me.z * rhs.x - rhs.z * me.x,
				me.x * rhs.y - rhs.x * me.y);
		}

		void Abs()
		{
			x = fabs(x);
			y = fabs(y);
			z = fabs(z);
		}

#pragma region �I�y���[�^�[���Z�q
		Vec3 operator-()const
		{
			return Vec3(-x, -y, -z);
		}
		Vec3 operator+(const Vec3 &rhs)const
		{
			return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
		};
		Vec3 operator+(float rhs)const
		{
			return Vec3(x + rhs, y + rhs, z + rhs);
		};
		Vec3 operator-(const Vec3 &rhs)const
		{
			return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
		};
		Vec3 operator*(const Vec3 &rhs) const
		{
			return Vec3(x * rhs.x, y * rhs.y, z * rhs.z);
		};
		Vec3 operator*(const float &rhs)const
		{
			return Vec3(x * rhs, y * rhs, z * rhs);
		};
		Vec3 operator/(const Vec3 &rhs)const
		{
			return Vec3(x / rhs.x, y / rhs.y, z / rhs.z);
		};
		Vec3 operator/(const float &rhs)const
		{
			return Vec3(x / rhs, y / rhs, z / rhs);
		};
		Vec3 operator%(const Vec3 &rhs)const
		{
			return Vec3(fmodf(x, rhs.x), fmodf(y, rhs.y), fmodf(z, rhs.z));
		};
		void operator=(const Vec3 &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		};
		bool operator==(const Vec3 &rhs)const
		{
			return (x == rhs.x && y == rhs.y && z == rhs.z);
		};
		bool operator!=(const Vec3 &rhs)const
		{
			return !(*this == rhs);
		};
		void operator+=(const Vec3 &rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
		};
		void operator+=(const float &rhs)
		{
			x += rhs;
			y += rhs;
			z += rhs;
		};
		void operator-=(const Vec3 &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
		};
		void operator-=(const float &rhs)
		{
			x -= rhs;
			y -= rhs;
			z -= rhs;
		};
		void operator*=(const Vec3 &rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
		};
		void operator*=(const float &rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
		};
		void operator/=(const Vec3 &rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
			z /= rhs.z;
		};
		void operator/=(const float &rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
		};
		void operator%=(const Vec3 &rhs)
		{
			x = fmodf(x, rhs.x);
			y = fmodf(y, rhs.y);
			z = fmodf(z, rhs.z);
		};

#pragma endregion
	};
#pragma endregion

#pragma region Vec4
	template<typename T>
	struct Vec4
	{
		T x, y, z, a;

		Vec4()
		{
			x = 0;
			y = 0;
			z = 0;
			a = 0;
		};
		Vec4(T X, T Y, T Z, T A) :x(X), y(Y), z(Z), a(A) {};

		DirectX::XMFLOAT3 ConvertXMFLOAT3()const
		{
			return DirectX::XMFLOAT3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
		};
		DirectX::XMFLOAT4 ConvertXMFLOAT4()const
		{
			return DirectX::XMFLOAT4(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(a));
		};
		DirectX::XMVECTOR ConvertXMVECTOR()
		{
			DirectX::XMVECTOR result = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(a) };
			return result;
		};

		Vec4<int>Int()const
		{
			return Vec4<int>(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z), static_cast<int>(a));
		}
		Vec4<float>Float()const
		{
			return Vec4<float>(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), static_cast<float>(a));
		}

		void Abs()
		{
			x = fabs(x);
			y = fabs(y);
			z = fabs(z);
			a = fabs(a);
		}

#pragma region �I�y���[�^�[���Z�q
		Vec4 operator-()const
		{
			return Vec4(-x, -y, -z);
		}
		Vec4 operator+(const Vec4 &rhs)const
		{
			return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, a + rhs.a);
		};
		Vec4 operator-(const Vec4 &rhs)const
		{
			return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, a - rhs.a);
		};
		Vec4 operator*(const Vec4 &rhs) const
		{
			return Vec4(x * rhs.x, y * rhs.y, z * rhs.z, a * rhs.a);
		};
		Vec4 operator*(const float &rhs)const
		{
			return Vec4(x * rhs, y * rhs, z * rhs, a * rhs);
		};
		Vec4 operator/(const Vec4 &rhs)const
		{
			return Vec4(x / rhs.x, y / rhs.y, z / rhs.z);
		};
		Vec4 operator/(const float &rhs)const
		{
			return Vec4(x / rhs, y / rhs, z / rhs, a / rhs);
		};
		Vec4 operator%(const Vec4 &rhs)const
		{
			return Vec4(fmodf(x, rhs.x), fmodf(y, rhs.y), fmodf(z, rhs.z));
		};
		void operator=(const Vec4 &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			a = rhs.a;
		};
		bool operator==(const Vec4 &rhs)const
		{
			return (x == rhs.x && y == rhs.y && z == rhs.z && a == rhs.a);
		};
		bool operator!=(const Vec4 &rhs)const
		{
			return !(x == rhs.x && y == rhs.y && z == rhs.z && a == rhs.a);
		};
		void operator+=(const Vec4 &rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
		};
		void operator+=(const float &rhs)
		{
			x += rhs;
			y += rhs;
			z += rhs;
		};
		void operator-=(const Vec4 &rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
		};
		void operator-=(const float &rhs)
		{
			x -= rhs;
			y -= rhs;
			z -= rhs;
		};
		void operator*=(const Vec4 &rhs)
		{
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
		};
		void operator*=(const float &rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
		};
		void operator/=(const Vec4 &rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
			z /= rhs.z;
		};
		void operator/=(const float &rhs)
		{
			x /= rhs;
			y /= rhs;
			z /= rhs;
		};
		void operator%=(const Vec4 &rhs)
		{
			x = fmodf(x, rhs.x);
			y = fmodf(y, rhs.y);
			z = fmodf(z, rhs.z);
		};
#pragma endregion
	};
#pragma endregion


	struct Color
	{
		Vec4<int> color;

		Color() :color(255, 255, 255, 255), dirtyFlag(&color)
		{
		};
		Color(int R, int G, int B, int A) :color({ R,G,B,A }), dirtyFlag(&color)
		{};

		DirectX::XMFLOAT4 ConvertColorRateToXMFLOAT4()const
		{
			KazMath::Vec4<float> result = GetColorRate();
			return DirectX::XMFLOAT4(result.x, result.y, result.z, result.a);
		};
		DirectX::XMFLOAT3 ConvertColorRateToXMFLOAT3()const
		{
			KazMath::Vec4<float> result = GetColorRate();
			return DirectX::XMFLOAT3(result.x, result.y, result.z);
		};
		Vec4<float> GetColorRate()const
		{
			KazMath::Vec4<float> result = CovertToFloat();
			return KazMath::Vec4<float>(
				static_cast<float>(color.x) / 255.0f,
				static_cast<float>(color.y) / 255.0f,
				static_cast<float>(color.z) / 255.0f,
				static_cast<float>(color.a) / 255.0f
			);
		};


		bool Dirty()
		{
			return dirtyFlag.Dirty();
		};
		void Record()
		{
			dirtyFlag.Record();
		};

		void operator=(const Color &rhs)
		{
			color = rhs.color;
		}

	private:
		KazMath::Vec4<float> CovertToFloat()const
		{
			return KazMath::Vec4<float>(
				static_cast<float>(color.x),
				static_cast<float>(color.y),
				static_cast<float>(color.z),
				static_cast<float>(color.a)
			);
		};
		DirtyFlag<KazMath::Vec4<int>> dirtyFlag;
	};

	struct Transform3D;
	DirectX::XMMATRIX CaluWorld(const KazMath::Transform3D &TRANSFORM, const Vec3<float> &Y_VEC, const Vec3<float> &Z_VEC);

	/// <summary>
	/// 3D��ԏ�ŃI�u�W�F�N�g�𓮂����ۂ̃f�[�^�ł�
	/// </summary>
	struct Transform3D
	{
		Vec3<float> pos;
		Vec3<float> scale;
		Vec3<float> rotation;

		Transform3D():
			positionDirtyFlag(DirtyFlag<Vec3<float>>(&pos)),
			scaleDirtyFlag(DirtyFlag<Vec3<float>>(&scale)),
			rotationDirtyFlag(DirtyFlag<Vec3<float>>(&rotation)),
			pos(Vec3<float>(0.0f, 0.0f, 0.0f)),
			scale(Vec3<float>(1.0f, 1.0f, 1.0f)),
			rotation(Vec3<float>(0.0f, 0.0f, 0.0f))
		{
		};
		Transform3D(const Vec3<float> &POS) :
			positionDirtyFlag(DirtyFlag<Vec3<float>>(&pos)),
			scaleDirtyFlag(DirtyFlag<Vec3<float>>(&scale)),
			rotationDirtyFlag(DirtyFlag<Vec3<float>>(&rotation)),
			pos(POS),
			scale(Vec3<float>(1.0f, 1.0f, 1.0f)),
			rotation(Vec3<float>(0.0f, 0.0f, 0.0f))
		{
		};
		Transform3D(const Vec3<float> &POS, const Vec3<float> &SCALE, const Vec3<float> &ROTATION):
			positionDirtyFlag(DirtyFlag<Vec3<float>>(&pos)),
			scaleDirtyFlag(DirtyFlag<Vec3<float>>(&scale)),
			rotationDirtyFlag(DirtyFlag<Vec3<float>>(&rotation)),
			pos(POS),
			scale(SCALE),
			rotation(ROTATION)
		{
		};

		bool Dirty()
		{
			return positionDirtyFlag.Dirty() || scaleDirtyFlag.Dirty() || rotationDirtyFlag.Dirty();
		};
		void Record()
		{
			positionDirtyFlag.Record();
			scaleDirtyFlag.Record();
			rotationDirtyFlag.Record();
		};
		DirectX::XMMATRIX GetMat(const Vec3<float> &Y_VEC = { 0.0f,1.0f,0.0f }, const Vec3<float> &Z_VEC = { 0.0f,0.0f,1.0f })
		{
			matrix = CaluWorld(*this, Y_VEC, Z_VEC);
			return matrix;
		}

		void operator=(const KazMath::Transform3D &OBJ)
		{
			pos = OBJ.pos;
			scale = OBJ.scale;
			rotation = OBJ.rotation;
		}
		DirtyFlag<Vec3<float>>positionDirtyFlag;
		DirtyFlag<Vec3<float>>scaleDirtyFlag;
		DirtyFlag<Vec3<float>>rotationDirtyFlag;


	private:
		DirectX::XMMATRIX matrix;
	};

	/// <summary>
	/// 2D��ԏ�ŃI�u�W�F�N�g�𓮂����ۂ̃f�[�^�ł�
	/// </summary>
	struct Transform2D
	{
		Vec2<float> pos;
		Vec2<float> scale;
		float rotation;

		DirtyFlag<Vec2<float>>positionDirtyFlag;
		DirtyFlag<Vec2<float>>scaleDirtyFlag;
		DirtyFlag<float>rotationDirtyFlag;


		Transform2D():
			positionDirtyFlag(&pos),
			scaleDirtyFlag(&scale),
			rotationDirtyFlag(&rotation),
			pos(Vec2<float>(0.0f, 0.0f)),
			scale(Vec2<float>(1.0f, 1.0f)),
			rotation(0.0f)
		{
		};

		bool Dirty()
		{
			return positionDirtyFlag.Dirty() || scaleDirtyFlag.Dirty() || rotationDirtyFlag.Dirty();
		};
		void Record()
		{
			positionDirtyFlag.Record();
			scaleDirtyFlag.Record();
			rotationDirtyFlag.Record();
		};

		void operator=(const KazMath::Transform2D &OBJ)
		{
			pos = OBJ.pos;
			scale = OBJ.scale;
			rotation = OBJ.rotation;
		}
	};

	struct BaseMatWorldData
	{
		DirectX::XMMATRIX matWorld;
		DirectX::XMMATRIX matScale;
		DirectX::XMMATRIX matRota;
		DirectX::XMMATRIX matTrans;
	};


	static const float COLOR_MAX_VALUE = 255.0f;



	static const float PI_2F = 3.1415926540f;
	static const DirectX::XMMATRIX MAT_IDENTITY = DirectX::XMMatrixIdentity();


	void CheckIsnan(KazMath::Vec3<float> *VEC);

	DirectX::XMVECTOR LoadFloat3ToVector(DirectX::XMFLOAT3 NUM_2);
	DirectX::XMFLOAT3 LoadVecotrToXMFLOAT3(DirectX::XMVECTOR NUM_1);


	DirectX::XMVECTOR CalculateScreenToWorld(DirectX::XMVECTOR pout, DirectX::XMMATRIX View, DirectX::XMMATRIX Prj);

	Vec2<float> CaluAngle(const Vec2<float> &POS, float Angle, const Vec2<float> &CPos);
	template<typename T>
	Vec3<T> CaluAngle3D(const Vec3<T> &POS, float ANGLE, const Vec3<T> &CENTRAL_POS);

	template<typename T>
	T Rand(T MAX_NUM, T MIN_NUM)
	{
		return (MAX_NUM - MIN_NUM) * rand() / RAND_MAX + MIN_NUM;
	}

	void ConvertMatrixFromFbx(DirectX::XMMATRIX *DST, const FbxAMatrix &SRC);

	/// <summary>
	/// �b�����t���[���ɒ����܂�
	/// </summary>
	/// <param name="SECOND">�b��</param>
	/// <returns>�t���[��</returns>
	int ConvertSecondToFlame(int SECOND);

	/// <summary>
	/// �ڕW�n�_�ɓ��B����܂ł̈ړ��ʂ��v�Z���܂�
	/// </summary>
	/// <param name="A_POINT">A�n�_</param>
	/// <param name="B_POINT">B�n�_</param>
	/// <param name="FLAME">��b������̎���</param>
	/// <param name="MAX_FLAME">���B����</param>
	/// <returns>�ڕW�n�_�܂ł̈ړ���</returns>
	template <typename T>
	Vec3<T> CaluAPointToBPointVel(const Vec3<T> &A_POINT, const Vec3<T> &B_POINT, int FLAME, int MAX_FLAME);

	/// <summary>
	/// �X�N���[�����W���烏�[���h���W�ɕϊ����܂�
	/// </summary>
	/// <param name="SCREEN_POS">�X�N���[�����W</param>
	/// <param name="VIEW_MAT">�r���[�s��</param>
	/// <param name="PROJECTION_MAT">�v���W�F�N�V�����s��</param>
	/// <returns>���[���h���W</returns>
	Vec3<float> ConvertScreenPosToWorldPos(const Vec3<float> &SCREEN_POS, DirectX::XMMATRIX VIEW_MAT, DirectX::XMMATRIX PROJECTION_MAT);



	/// <summary>
	/// ��]�s����v�Z���܂�
	/// </summary>
	/// <param name="ROTATION">�p�x</param>
	/// <returns>��]�s��</returns>
	DirectX::XMMATRIX CaluRotaMatrix(const Vec3<float> &ROTATION);

	/// <summary>
	/// �X�P�[�����O�s����v�Z���܂�
	/// </summary>
	/// <param name="SCALE">�傫��</param>
	/// <returns>�X�P�[�����O�s��</returns>
	DirectX::XMMATRIX CaluScaleMatrix(const Vec3<float> &SCALE);

	/// <summary>
	/// ���s�ړ��s����v�Z���܂�
	/// </summary>
	/// <param name="POS">���W</param>
	/// <returns>���s�ړ��s��</returns>
	DirectX::XMMATRIX CaluTransMatrix(const Vec3<float> &POS);

	/// <summary>
	/// ��]�s����v�Z���܂�
	/// </summary>
	/// <param name="ROTATION">�p�x</param>
	/// <returns>��]�s��</returns>
	DirectX::XMMATRIX CaluRotaMatrix(const Vec3<float> &ROTATION);

	/// <summary>
	/// �X�P�[�����O�s����v�Z���܂�
	/// </summary>
	/// <param name="SCALE">�傫��</param>
	/// <returns>�X�P�[�����O�s��</returns>
	DirectX::XMMATRIX CaluScaleMatrix(const Vec3<float> &SCALE);

	/// <summary>
	/// ���s�ړ��s����v�Z���܂�
	/// </summary>
	/// <param name="POS">���W</param>
	/// <returns>���s�ړ��s��</returns>
	DirectX::XMMATRIX CaluTransMatrix(const Vec3<float> &POS);


	/// <summary>
	/// �K�E�V�A���̑傫�����v�Z����
	/// </summary>
	/// <param name="count"></param>
	/// <param name="s"></param>
	/// <returns></returns>
	std::vector<float> GetGaussianWeights(size_t count, float s);

	/// <summary>
	/// �o�b�t�@�̃T�C�Y���A���C�����g����
	/// </summary>
	/// <param name="size">���X�̃T�C�Y</param>
	/// <param name="alignment">���o�C�g�ɂ�������</param>
	/// <returns>�o�b�t�@�̃T�C�Y</returns>
	unsigned int AligmentedValue(unsigned int size, unsigned int alignment);

	/// <summary>
	/// �K�E�V�A���̑傫�����v�Z����
	/// </summary>
	/// <param name="weightsTbl">�z��̐擪�A�h���X</param>
	/// <param name="sizeOfWeightsTbl">�z��̑傫��</param>
	/// <param name="sigma">�ǂꂭ�炢�ڂ������B�������傫����Α傫���قǂڂ�����</param>
	void CalcWeightsTableFromGaussian(float *weightsTbl, int sizeOfWeightsTbl, float sigma);


	/// <summary>
	/// �f�o�b�N�p�̎��_���W���v�Z���܂�
	/// �㉺���E���R�ɃJ�����𓮂��������\�ɂȂ�܂�
	/// </summary>
	/// <param name="EYE_POS">���_���W</param>
	/// <param name="ANGLE">�p�x</param>
	/// <returns>�f�o�b�N�p�̎��_���W</returns>
	Vec3<float> CaluEyePosForDebug(const Vec3<float> &EYE_POS, const Vec3<float> &MOVE, const Vec2<float> &ANGLE, float SPEED = 5.0f);

	/// <summary>
	/// �f�o�b�N�p�̒����_���v�Z���܂�
	/// </summary>
	/// <param name="EYE_POS">���_���W</param>
	/// <param name="ANGLE">�p�x</param>
	/// <returns>�f�o�b�N�p�̒����_</returns>
	Vec3<float> CaluTargetPosForDebug(const Vec3<float> &EYE_POS, float ANGLE);


	bool MatrixEqualOrNot(const DirectX::XMMATRIX &MAT_1, const DirectX::XMMATRIX &MAT_2);

	DirectX::XMMATRIX CaluSlopeMatrix(const Vec3<float> &Y, const Vec3<float> &Z);

	DirectX::XMMATRIX CaluFrontMatrix(const Vec3<float> &Y, const Vec3<float> &Z);

	int RadianToAngle(float RADIAN);
	float AngleToRadian(int ANGLE);
	float AngleToRadian(float ANGLE);

	DirectX::XMMATRIX CaluMat(const KazMath::Transform3D &TRANSFORM, const DirectX::XMMATRIX &VIEW_MAT, const DirectX::XMMATRIX &PROJECT_MAT, const Vec3<float> &Y_VEC, const Vec3<float> &Z_VEC);


	void Larp(float BASE_TRANSFORM, float *TRANSFORM, float MUL);
	void Larp(const Vec2<float> BASE_TRANSFORM, Vec2<float> *TRANSFORM, float MUL);
	void Larp(const Vec3<float> BASE_TRANSFORM, Vec3<float> *TRANSFORM, float MUL);

	template<typename T>
	float GetSinFloat(T VALUE)
	{
		return static_cast<float>(sin(static_cast<float>VALUE));
	};

	template<typename T>
	float GetCosFloat(T VALUE)
	{
		return static_cast<float>(cos(static_cast<float>VALUE));
	};

	template<typename T>
	Vec3<T>CastXMVECTOR(const DirectX::XMVECTOR &VALUE)
	{
		Vec3<T> result(static_cast<T>(VALUE.m128_f32[0]), static_cast<T>(VALUE.m128_f32[1]), static_cast<T>(VALUE.m128_f32[2]));
		return result;
	};

	/// <summary>
	/// �X�v���C���Ȑ�
	/// </summary>
	/// <param name="points"></param>
	/// <param name="startIndex"></param>
	/// <param name="t"></param>
	/// <param name="Loop"></param>
	/// <returns></returns>
	Vec3<float> SplinePosition(const std::vector<Vec3<float>> &points, size_t startIndex, float t, bool Loop);


	/// <summary>
	/// ���[���h���W����X�N���[�����W�ɕϊ����܂�
	/// </summary>
	/// <param name="SCREEN_POS">���[���h���W</param>
	/// <param name="VIEW_MAT">�r���[�s��</param>
	/// <param name="PROJECTION_MAT">�v���W�F�N�V�����s��</param>
	/// <returns>�X�N���[�����W</returns>
	Vec3<float> ConvertWorldPosToScreenPos(const Vec3<float> &WORLD_POS, DirectX::XMMATRIX VIEW_MAT, DirectX::XMMATRIX PROJECTION_MAT);


	/// <summary>
	/// ���Ԃ�0.0f~1.0f�ɕϊ����܂�
	/// </summary>
	/// <param name="TIMER">���݂̎���</param>
	/// <param name="MAX_TIMER">�ő厞��</param>
	/// <returns></returns>
	inline float ConvertTimerToRate(int TIMER, int MAX_TIMER)
	{
		float rate = static_cast<float>(TIMER) / static_cast<float>(MAX_TIMER);
		if (1.0f <= rate)
		{
			rate = 1.0f;
		}
		return rate;
	}


	void ConvertMatrixToAngles(const DirectX::XMMATRIX &MAT, Vec3<float> *ANGLE);

}

