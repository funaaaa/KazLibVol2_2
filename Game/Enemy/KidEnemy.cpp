#include "KidEnemy.h"

KidEnemy::KidEnemy()
{
}

void KidEnemy::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	iEnemy_ObjModelRender->data.transform.pos = GENERATE_DATA.initPos;	//���W�̏�����
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//�����蔻��̑傫���ύX
	iOperationData.Init(4,"Kid");							//�c�胍�b�N�I�������̏�����

	iEnemy_ObjModelRender->data.colorData = { 255,0,0,255 };

	demoFlag = DEMO_FLAG;
}

void KidEnemy::Finalize()
{
}

void KidEnemy::Update()
{
	
}

void KidEnemy::Draw()
{
	//iEnemy_ModelRender->Draw();
}
