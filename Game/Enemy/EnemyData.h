#pragma once
#include"../Game/Interface/IOperationObject.h"
#include"../Game/Effect/InstanceDeadParticle.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Loader/MeshParticleLoader.h"


/// <summary>
/// �G�̎��
/// </summary>
enum eEnemyType
{
	ENEMY_TYPE_NONE = -1,
	ENEMY_TYPE_NORMAL,
	ENEMY_TYPE_MISILE,
	ENEMY_TYPE_MISILE_SPLINE,
	ENEMY_TYPE_MOTHER,
	ENEMY_TYPE_KID,
	ENEMY_TYPE_POP,
	ENEMY_TYPE_BATTLESHIP,
	ENEMY_TYPE_BATTLESHIP_MISILE,
	ENEMY_TYPE_BIKE,
	ENEMY_TYPE_BIKE_MISILE
};

enum EnemyModelType
{
	ENEMY_MODEL_NONE,
	ENEMY_MODEL_OBJ,
	ENEMY_MODEL_FBX,
	ENEMY_MODEL_MESHPARTICLE
};

enum BattleShipMisileEnum
{
	BATTLESHIP_MISILE_SHOT_LEFT,
	BATTLESHIP_MISILE_SHOT_RIGHT,
};

struct BattleShipMisileData
{
	//���E�ǂ���ɔ�Ԃ�
	BattleShipMisileEnum shotEnum;
};

struct BattleShipData
{
	//�~�T�C���������ǂ���
	bool isShotFlag;

	BattleShipData() :isShotFlag(false)
	{};
};

struct MisileEnemy
{
	//�~�T�C���������ǂ���
	bool isShotFlag;

	MisileEnemy() :isShotFlag(false)
	{};
};

struct PopEnemyData
{
	//�~�T�C���������ǂ���
	bool leftRightFlag;

	PopEnemyData() :leftRightFlag(false)
	{};
};

/// <summary>
/// �G�𐶐�����ۂɕK�v�ȏ����l�ߍ���
/// </summary>
struct EnemyGenerateData
{
	KazMath::Vec3<float> initPos;	//�������W
	float speed;
	BattleShipMisileData battleShipEnemyData;
	BattleShipData battleShipData;
	PopEnemyData popEnemyData;
	MisileEnemy misileEnemy;
	bool useMeshPaticleFlag;

	EnemyGenerateData() :speed(-1.5f)
	{
	};
};

struct ResponeData
{
	ResponeData() :layerLevel(-1), flame(-1)
	{
	}
	int layerLevel;		//�ǂ̃��C���[���x���Ō���邩
	int flame;		//�G�������t���[����
	EnemyGenerateData generateData;
};


/// <summary>
/// �G���瑼��OBJ�𐶐�����ۂɓn���f�[�^
/// </summary>
struct GenarateOtherObjData
{
	GenarateOtherObjData() :enemyType(-1)
	{
	}

	void Finalize()
	{
		enemyType = -1;
	}
	int enemyType;			//�ǂ̎�ނ�o�ꂳ���邩
	EnemyGenerateData enemyGenerateData;
};

//���b�V���p�[�e�B�N���ɕK�v�ȃf�[�^
struct MeshData
{
	RESOURCE_HANDLE resourceHandle;
	const DirectX::XMMATRIX *motherMat;
	InitMeshParticleData meshParticleData;

	MeshData(RESOURCE_HANDLE HANDLE, const DirectX::XMMATRIX *MOTHER_MAT) :resourceHandle(HANDLE), motherMat(MOTHER_MAT)
	{
	};
};

/// <summary>
/// �Q�[�����ɓo�ꂷ��G�̃f�[�^
/// </summary>
struct EnemyData
{
	/// <summary>
	/// �G�̏������������܂�
	/// </summary>
	/// <param name="HITBOX_POS_ADRESS">�����蔻��p�̍��W�A�h���X</param>
	/// <param name="RADIUS">�����蔻��̑傫��</param>
	/// <param name="MAX_ROCKON_NUM">���b�N�I���񐔂̃A�h���X</param>
	EnemyData(KazMath::Vec3<float> *HITBOX_POS_ADRESS, const float &RADIUS, IOperationObjectData *OPERATION_DATA) :
		hitBox({ HITBOX_POS_ADRESS ,RADIUS }),
		oprationObjData(OPERATION_DATA),
		timer(1),
		outOfStageFlag(false),
		radius(0.0f),
		startFlag(false), objFlag(false),
		meshParticleFlag(false)
	{
	};

	Sphere hitBox;							//�G�̓����蔻��
	IOperationObjectData *oprationObjData;	//����\OBJ������{�f�[�^�̃A�h���X
	GenarateOtherObjData genarateData;		//�ǉ��œG�𐶐�����ۂɕK�v�ȏ��
	int timer;								//�v���C���[�ɓ�����܂ł̎���
	bool outOfStageFlag;					//�X�e�[�W�͈̔͊O�ɏo�Ă��܂���
	float radius;
	bool startFlag;
	bool objFlag;
	int *alpha;
	float *fAlpha;

	bool meshParticleFlag;
	const DirectX::XMMATRIX *motherMat;
	std::vector<std::shared_ptr<MeshData>> meshParticleData;

	InitDeadParticleData deadParticleData;
};

enum EnemyDeathType
{
	DEATH_ROLL,
	DEATH_SINK
};

struct DeadLineData
{
	KazMath::Vec3<float>enableToHitMaxLine;
	KazMath::Vec3<float>enableToHitMinLine;

	DeadLineData(int LINE_TYPE)
	{
		switch (LINE_TYPE)
		{
		case 0:
			enableToHitMinLine = { -100.0f,-100.0f,-100.0f };
			enableToHitMaxLine = { 100.0f,100.0f,700.0f };
			break;
		case 1:
			enableToHitMinLine = { -3000.0f,-100.0f,-10000.0f };
			enableToHitMaxLine = { 3000.0f,100.0f,1000.0f };
			break;
		default:
			break;
		}
	}
};