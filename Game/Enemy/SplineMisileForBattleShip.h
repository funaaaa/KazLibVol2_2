#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"
#include"../Game/Effect/RocketLightEffect.h"
#include"../Game/Effect/MisileSmokeEmitter.h"

class SplineMisileForBattleShip:public IEnemy
{
public:
	SplineMisileForBattleShip();

	void Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG = false);
	void Finalize();
	void Update();
	void Draw();

private:
	int maxTime;		//���v����
	int pointTime;		//����_�Ԃ��ړ����鎞��
	int nowTime;		//���݂̎���

	//�X�v���C���Ȑ�----------------------------------------------
	std::vector<KazMath::Vec3<float>> points;
	int startIndex;		//�ǂ̐���_���m���ړ����Ă��邩
	float timeRate;		//����_�Ԃ̎���
	KazMath::Vec3<float> position;	//���ݒn
	BoxPolygonRender splineBox;
	std::array<BoxPolygonRender, 3> pointsRender;
	//�X�v���C���Ȑ�----------------------------------------------

	bool hitFlag;

	RocketLightEffect rocketLight;
	//MisileSmokeEmitter smokeEmitter;
};

