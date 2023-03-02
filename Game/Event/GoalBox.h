#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Event/GoalBoxEffect.h"
#include"../Game/Event/GoalBoxPortalEffect.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../Game/Event/PortalFlame.h"
#include<array>

class GoalBox :public IOperationObject
{
public:
	GoalBox();

	void Init(const KazMath::Vec3<float> &POS);
	void Update();
	void Draw();
	
	void Appear(const KazMath::Vec3<float> &POS);

	//�S�[���G�t�F�N�g
	GoalBoxEffect lightEffect;
	Sphere hitBox;
	bool releaseFlag;
	bool startPortalEffectFlag;

private:
	static const int HP = 8;

	//�ړ�-----------------------
	KazMath::Vec3<float> lerpRota;//�p�x�̃��[�v
	KazMath::Vec3<float> lerpScale;//�p�x�̃��[�v
	KazMath::Vec3<float> lerpPos; //���W�̃��[�v
	KazMath::Vec3<float> moveVel; //���������ۂɂǂꂭ�炢�ړ����邩
	KazMath::Vec3<float> moveRotaVel;//���������ۂɂǂꂭ�炢�܂�邩
	float lerpMoveVel;		//���[�v�̑��x
	float lerpMoveRotaVel;	//���[�v�̑��x

	KazMath::Vec3<float> goCenterPos;//�S�e�q�b�g���ɃS�[���n�_�Ɍ�����


	int prevHpNum;//�O�t���[���̗̑�
	//�`��-----------------------
	ObjModelRenderPtr model;

	float addRota;
	float addVel;
	bool hitFlag;

	int intervalTimer;

	RESOURCE_HANDLE damageSoundHandle;
	RESOURCE_HANDLE warpSoundHandle;
	bool initWarpSoundFlag;

};