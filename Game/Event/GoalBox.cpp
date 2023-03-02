#include "GoalBox.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

GoalBox::GoalBox()
{
	model = std::make_unique<ObjModelRender>();
	model->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iOperationData.Init(8,"Network_Security");
	prevHpNum = iOperationData.rockOnNum;

	model->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::GoalPath + "goal.obj");

	lerpPos = { 0.0f,0.0f,0.0f };
	lerpRota = { 0.0f,0.0f,0.0f };
	model->data.transform.pos = lerpPos;
	model->data.transform.rotation = lerpRota;

	//model->data.transform.scale = { 10.0f,10.0f,10.0f };

	lerpMoveRotaVel = 0.1f;
	lerpMoveVel = 0.1f;
	moveRotaVel = { 200.0f,200.0f,200.0f };
	moveVel = { 0.0f,0.0f,15.0f };

	goCenterPos = { 0.0f, 3.0f, 50.0f };
	lightEffect.Init(&model->data.motherMat.mat);

	hitBox.center = &model->data.transform.pos;
	hitBox.radius = 10.0f;
	//std::shared_ptr<DirectX::XMMATRIX>data = std::make_shared<DirectX::XMMATRIX>(model->data.motherMat);

	damageSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "EenemyDamage.wav", false);
	warpSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "warp.wav", false);
}

void GoalBox::Init(const KazMath::Vec3<float> &POS)
{
	iOperationData.Init(HP, "Network_Security");
	prevHpNum = iOperationData.rockOnNum;


	lerpPos = POS;
	lerpRota = { 0.0f,0.0f,0.0f };
	lerpScale = { 1.0f,1.0f,1.0f };

	model->data.transform.pos = lerpPos;
	model->data.transform.rotation = lerpRota;

	lightEffect.Init(model->GetMotherMatrixPtr());
	addRota = 0;

	startPortalEffectFlag = false;
	addVel = 0.0f;

	intervalTimer = 0;
	initWarpSoundFlag = false;
}

void GoalBox::Update()
{
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_M))
	{
		Hit();
	}
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_N))
	{
		Init({ 20.0f,10.0f,10.0f });
	}

	bool reduceHpFlag = false;

	//�����[�X���Ɏc�胍�b�N�I�������O�ƈ�����瓖������������o��
	if (releaseFlag && iOperationData.rockOnNum != prevHpNum)
	{
		hitFlag = true;
		prevHpNum = iOperationData.rockOnNum;
	}

	//��������������o�āAHp���w��̉񐔕����炵����U��
	if (hitFlag)
	{
		int result = ReduceHp(HP - iOperationData.rockOnNum);
		//�̗͂����炷
		if (result == 1)
		{
			SoundManager::Instance()->PlaySoundMem(damageSoundHandle, 1);
			reduceHpFlag = true;
		}
		//�I��
		else if (result == 2)
		{
			hitFlag = false;
		}
	}

	//��ɉ�]����
	lerpRota.x += 1.0f;
	lerpRota.y += 1.0f;

	//���������ۂ̋���
	if (reduceHpFlag)
	{
		//���ɔ��
		lerpPos.z = model->data.transform.pos.z + moveVel.z;
		//��]������
		lerpRota = model->data.transform.rotation + moveRotaVel;
		lightEffect.Appear();
	}

	//�S�e�q�b�g�����牉�o�����ĉ�ʒ����Ɍ�����
	const int NO_HP = 0;
	if (iOperationData.hp <= NO_HP)
	{
		++intervalTimer;
	}

	const int INTERVAL_MAX_TIMER = 30;
	if (INTERVAL_MAX_TIMER <= intervalTimer)
	{
		lightEffect.Disappear();
		lerpPos = goCenterPos;

		if (120 <= intervalTimer)
		{
			addRota += addVel;
			addVel += 0.1f;
		}
		lerpRota.x = 0.0f;
		lerpRota.y += addRota;
		lerpRota.z = 0.0f;


		if (60.0f * 5.0f <= addRota)
		{
			lerpScale = { 35.0f ,35.0f,35.0f };
		}
		if (60.0f * 10.0f <= addRota)
		{
			KazMath::Vec3<float> adj = { 0.0f,0.0f,-15.0f };
			startPortalEffectFlag = true;
		}
	}


	//���W�̃��[�v
	{
		KazMath::Vec3<float> distance = lerpPos - model->data.transform.pos;
		model->data.transform.pos += distance * lerpMoveVel;
	}
	//��]�̃��[�v
	{
		KazMath::Vec3<float> distance = lerpRota - model->data.transform.rotation;
		model->data.transform.rotation += distance * lerpMoveRotaVel;
	}

	{
		KazMath::Vec3<float> distance = lerpScale - model->data.transform.scale;
		model->data.transform.scale += distance * 0.1f;
	}

	lightEffect.Update();
}

void GoalBox::Draw()
{
	model->Draw();
	LockOnWindow(model->data.transform.pos);
}

void GoalBox::Appear(const KazMath::Vec3<float> &POS)
{
	lerpPos = POS;
}