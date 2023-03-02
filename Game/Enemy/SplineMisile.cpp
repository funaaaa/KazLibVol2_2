#include "SplineMisile.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

SplineMisile::SplineMisile()
{
	maxTime = 60 * 5;
	splineBox.data.color = { 255,0,0,255 };

	iEnemy_ObjModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "Misile/" + "missile_Model.obj");
}

void SplineMisile::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	InitModel(KazMath::Transform3D(GENERATE_DATA.initPos, { 1.3f,1.3f,1.3f }, { 0.0f,0.0f,0.0f }), KazFilePathName::EnemyPath + "Misile/" + "missile_Model.obj", 10.0f, ENEMY_MODEL_OBJ);
	iEnemy_ObjModelRender->data.colorData.color.a = 255;
	iEnemy_EnemyStatusData->timer = maxTime;
	iEnemy_EnemyStatusData->radius = 3.0f;
	iEnemy_EnemyStatusData->startFlag = true;
	iOperationData.Init(1, "gwJ-M");

	startIndex = 1;
	timeRate = 0.0f;


	points.clear();
	points.shrink_to_fit();

	KazMath::Vec3<float> startPos = GENERATE_DATA.initPos;
	KazMath::Vec3<float> endPos = { 0.0f,0.0f,0.0f };

	KazMath::Vec3<float> distance = startPos - endPos;
	float addDistance = distance.z / 4.0f;

	distance.z /= 2.0f;	//���Ԃ�Z�����̔���
	distance.z -= addDistance;


	//�X�^�[�g�n�_
	points.push_back(startPos);
	points.push_back(startPos);

	//����
	points.push_back(distance);

	//�S�[���n�_
	points.push_back(endPos);
	points.push_back(endPos);

	pointTime = maxTime / (static_cast<int>(points.size() )- 3);
	nowTime = 0;
	initDeadSoundFlag = false;
	hitFlag = false;

	demoFlag = DEMO_FLAG;
}

void SplineMisile::Finalize()
{
}

void SplineMisile::Update()
{
	//����������`�悵�Ȃ��Ȃ�
	if (iEnemy_EnemyStatusData->timer <= 0)
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		hitFlag = true;
	}


	//���S���o����
	//�f�o�b�N�L�[or�����蔻���&&���S��
	if (!ProcessingOfDeath(DEATH_ROLL) && !hitFlag)
	{
		if (iOperationData.enableToHitFlag)
		{
			--iEnemy_EnemyStatusData->timer;
		}

		//�ړ�����----------------------------------------------
		timeRate = static_cast<float>(nowTime) / static_cast<float>(pointTime);
		if (1.0f <= timeRate)
		{
			if (startIndex < points.size() - 3)
			{
				startIndex++;
				timeRate = 0.0f;
				nowTime = 0;
			}
			else
			{
				timeRate = 1.0f;
			}
		}
		++nowTime;
		//�ړ�����----------------------------------------------


		//���W�ݒ�----------------------------------------------
		position = KazMath::SplinePosition(points, static_cast<size_t>(startIndex), timeRate, true);

		for (int i = 0; i < pointsRender.size(); ++i)
		{
			pointsRender[i].data.transform.pos = points[i];
		}

		iEnemy_ObjModelRender->data.transform.pos = position;
		//���W�ݒ�----------------------------------------------

		//�O�x�N�g���̐ݒ�----------------------------------------------
		float nextTimeRate = static_cast<float>(nowTime + 1) / static_cast<float>(pointTime);
		KazMath::Vec3<float> nextpos = KazMath::SplinePosition(points, startIndex, nextTimeRate, true);

		KazMath::Vec3<float> upVector = nextpos - position;
		upVector.Normalize();
		iEnemy_ObjModelRender->data.upVector = upVector;
		//�O�x�N�g���̐ݒ�----------------------------------------------
	}
}

void SplineMisile::Draw()
{
	if (demoFlag)
	{
		for (int i = 0; i < pointsRender.size(); ++i)
		{
			pointsRender[i].Draw();
		}
	}

	if (iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ObjModelRender->Draw();
		LockOnWindow(iEnemy_ObjModelRender->data.transform.pos);
	}
}
