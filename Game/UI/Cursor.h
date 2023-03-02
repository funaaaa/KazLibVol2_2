#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/UI/LockOnBoxEffect.h"

class Cursor
{
public:
	Cursor();

	void Init();
	void Input(bool UP_FLAG, bool DOWN_FLAG, bool LEFT_FLAG, bool RIGHT_FLAG, bool DONE_FLAG, bool RELEASE_FLAG, const KazMath::Vec2<float> &ANGLE);
	void Update();
	void Draw();

	/// <summary>
	/// ���b�N�I�������ő�l�����ł��邱�ƁA���b�N�I�����L���Ń��b�N�I���̓��͂�����Ă���
	/// ����true���ς���
	/// </summary>
	/// <returns></returns>
	bool LockOn();
	bool Release();

	void Hit(KazMath::Vec3<float> *POS);
	const int &GetCount();

	KazMath::Vec2<float> GetValue();

	void Disappear();
	void Appear();

	/// <summary>
	/// ���b�N�I���������炷
	/// </summary>
	/// <param name="SUB_VALUE">���炷��</param>
	void SubCount(int SUB_VALUE);

	KazMath::Vec2<float> honraiCursorPos;
	KazMath::Vec2<float> cursorPos;
	KazMath::Vec2<float> prevCursorPos;
	KazMath::Vec2<float> vel;
	Ray hitBox;
	DirectX::XMINT2 stopFlag;
	bool releaseFlag;


	KazMath::Vec2<float> honraiCameraMoveValue;//�㉺���E�ɃJ�������ǂꂭ�炢���������̒l��ۑ�����(�{���|�Y)
	KazMath::Vec2<float> cameraMoveValue;//�㉺���E�ɃJ�������ǂꂭ�炢���������̒l��ۑ�����
	KazMath::Vec2<float> dontMoveCameraStartPos;//�J�����̖�����J�n�n�_
	KazMath::Vec2<float> dontMoveCameraEndPos;//�J�����̖�����I���n�_

	KazMath::Vec2<float> limitValue;//�J�����̒[�̒l

	static KazMath::Vec2<float> NO_MOVE_DISTANCE;//XY���ꂼ��ǂ��܂ł͈̔͂Ȃ�J�[�\���������Ă��J�����ɉe�����o�Ȃ���

private:
	//���b�N�I����----------------------------------------------------------------
	static const int LOCKON_MAX_NUM = 8;	//�ő働�b�N�I����
	int lockOnNum;							//���݂̃��b�N�I����
	bool notEnableLockOnFlag;
	int enableLockOnTimer;

	//�ړ�----------------------------------------------------------------
	bool upFlag, downFlag, leftFlag, rightFlag,doneFlag;
	KazMath::Vec2<float> stickAngle;
	float deadLine;
	float baseSpeed;
	DirectX::XMFLOAT2 speed;

	//���͉��o-----------------------

	struct CursorEffectData
	{
		bool initFlag;
		Sprite2DRenderPtr cursorEffectTex;

		CursorEffectData() :cursorEffectTex(std::make_unique<Sprite2DRender>()), initFlag(false)
		{
		}
	};

	std::array<CursorEffectData, 10> cursorEffectTex;


	//���얳��----------------------------------------------
	bool dontMoveFlag;	//�J�������Œ�ɂȂ����ۂɃJ�[�\���𓮂����Ȃ�����
	int cursorAlpha;	//�J�[�\���̓����x
	bool disappearFlag;//�J�[�\���������t���O
	//���얳��----------------------------------------------


	//�摜----------------------------------------------------------------
	Sprite2DRenderPtr cursorFlameTex, numberTex;	//�X�N���[�����W�̕`��
	std::array<RESOURCE_HANDLE, LOCKON_MAX_NUM + 1> numberHandle;			//�����p�̉摜�n���h��
	RESOURCE_HANDLE flameHandle;
	KazMath::Vec2<float> baseScale;

	KazMath::Vec2<float> oldPos;
	KazMath::Vec2<float> oldVel;


	//�m�b�N�o�b�N����-----------------------
	KazMath::Vec2<float> knockBackVal;	//���]�ړ��p�̈ړ���
	static DirectX::XMFLOAT2 KOCKBACK_MAX_VALUE;		//�m�b�N�o�b�N�̍ő��(�l�����܂�����const�ɂ���)
	static DirectX::XMFLOAT2 KOCKBACK_VELOCITY;		//�m�b�N�o�b�N�̉��Z��(�l�����܂�����const�ɂ���)
	DirectX::XMINT2 noTochFlag;
	DirectX::XMINT2 noTockTimer;
	//�m�b�N�o�b�N����-----------------------

	int clickSoundHandle;
	bool initClickSoundFlag;


	//���b�N�I�����o
	std::array<LockOnBoxEffect, LOCKON_MAX_NUM * 2> boxEffectArray;

};

