#pragma once
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../Enemy/EnemyData.h"
#include"../Game/Effect/MeshParticleEmitter.h"

/// <summary>
/// �G�̒��ۃN���X
/// </summary>
class IEnemy :public IOperationObject
{
public:

	/// <summary>
	/// �m���ɏ������������f�[�^������
	/// </summary>
	IEnemy();
	/// <summary>
	/// ���z�f�X�g���N�^
	/// </summary>
	virtual ~IEnemy()
	{
	};

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="POS">�������W</param>
	virtual void Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG = false) = 0;

	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �G�����S�����܂�
	/// </summary>
	void Dead();

	/// <summary>
	/// ���S���o
	/// </summary>
	void DeadEffect(KazMath::Vec3<float> *POS, KazMath::Vec3<float> *ROTATION, int *ALPHA);

	//���S����Ə���
	bool ProcessingOfDeath(EnemyDeathType TYPE);
	bool ProcessingOfDeathFbx(EnemyDeathType TYPE);


	/// <returns>�G�̏��</returns>
	const std::unique_ptr<EnemyData> &GetData();

	void DeadSound();
	void ShotSound();
	void InitModel(const KazMath::Transform3D &TRANSFORM, const std::string &MODEL_FILEPASS, float HITBOX_RADIUS, EnemyModelType MODEL_TYPE, bool REV_UV_FLAG = false, bool GENERATE_PARTICLE_FLAG = true);

	//�U�����m�F����p�̊֐�
	virtual void DebugShot() { debugShotFlag = true; };

	void SetLight(const KazMath::Vec3<float> LIGHT_DIR, bool OBJ_FLAG)
	{
		if (!OBJ_FLAG)
		{
			DirectX::XMFLOAT3 dir = LIGHT_DIR.ConvertXMFLOAT3();
			iEnemy_FbxModelRender->TransData(&dir, fbxLightHandle, typeid(DirectX::XMFLOAT3).name());
		}
		else
		{
			DirectX::XMFLOAT3 dir = LIGHT_DIR.ConvertXMFLOAT3();
			iEnemy_ObjModelRender->TransData(&dir, objLightHandle, typeid(DirectX::XMFLOAT3).name());
		}
	};

	void ReleaseLight()
	{
		iEnemy_ObjModelRender->Release(objLightHandle);
		iEnemy_FbxModelRender->Release(fbxLightHandle);
	}

	//�ǉ��ŕ`�悵�������̂���������Ē�`����
	virtual void DrawIn() {};

	//���ʏ���--------------------------------------------
	void OnInit(bool MESH_PARTICLE_FLAG)
	{
		iEnemy_EnemyStatusData->meshParticleFlag = MESH_PARTICLE_FLAG;
	}
	void OnUpdate()
	{
		switch (modelType)
		{
		case ENEMY_MODEL_NONE:
			break;
		case ENEMY_MODEL_OBJ:
			//if (!ProcessingOfDeath(DEATH_ROLL))
			//{
			//	//�o�ꏈ��
			//	if (iEnemy_ObjModelRender->data.colorData.color.a < 255)
			//	{
			//		iEnemy_ObjModelRender->data.colorData.color.a += 5;
			//	}
			//	else
			//	{
			//		iEnemy_ObjModelRender->data.colorData.color.a = 255;
			//	}
			//}
			break;
		case ENEMY_MODEL_FBX:
			//ProcessingOfDeathFbx(DEATH_SINK);
			break;
		case ENEMY_MODEL_MESHPARTICLE:
			iEnemy_MeshModelRender->Update();
			break;
		default:
			break;
		}
	};
	//�G�̃��f���`��
	void OnDraw()
	{
		if (1.0f <= iEnemy_ObjModelRender->data.colorData.color.a)
		{
			switch (modelType)
			{
			case ENEMY_MODEL_NONE:
				break;
			case ENEMY_MODEL_OBJ:
				iEnemy_ObjModelRender->Draw();
				break;
			case ENEMY_MODEL_FBX:
				iEnemy_FbxModelRender->Draw();
				break;
			case ENEMY_MODEL_MESHPARTICLE:
				iEnemy_MeshModelRender->Draw();
				break;
			default:
				break;
			}
			DrawIn();
			LockOnWindow(*iEnemy_EnemyStatusData->hitBox.center);
		}
	};
	//���ʏ���--------------------------------------------



	std::unique_ptr<EnemyData> iEnemy_EnemyStatusData;		//�G�̏�Ԃ�ۑ�����f�[�^
	ObjModelRenderPtr iEnemy_ObjModelRender;				//�G�̕`��
	FbxModelRenderPtr iEnemy_FbxModelRender;				//�G�̕`��
	std::unique_ptr<MeshParticleEmitter> iEnemy_MeshModelRender;
	KazMath::Vec3<float> lerpPos;
	float yVel;

protected:
	bool initDeadSoundFlag;
	bool demoFlag;
	bool debugShotFlag;
private:
	int deadSoundHandle;
	int shotSoundHandle;

	DirtyFlag<short>hpDirtyFlag;
	bool hitFlag;

	RESOURCE_HANDLE fbxLightHandle;
	RESOURCE_HANDLE objLightHandle;

	KazMath::Vec3<float>initDeadRotaVel;
	KazMath::Vec3<float>initDeadYVel;

	EnemyModelType modelType;
};
