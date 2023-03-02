#pragma once
#include"../DirectXCommon/Base.h"
#include"IInputManager.h"
#include"../Helper/ISinglton.h"
#include<Xinput.h>
#pragma comment(lib, "Xinput.lib")
/*
�p�b�h����
�X�e�B�b�N����S��
*/

enum ControllerStickSide
{
	RIGHT_STICK,
	LEFT_STICK
};

enum ControllerSide
{
	UP_SIDE,
	DOWN_SIDE,
	LEFT_SIDE,
	RIGHT_SIDE
};

enum ControllerShoulderSide
{
	LEFT_SHOULDER,
	RIGHT_SHOULDER
};

//�ő�l�܂ŃR���g���[���[�͎g�p�\�炵�����A���ݓ��������m�F�ς�
//Stick�̔���S�ʂɂăX�e�B�b�N���X������ԂŎw�𗣂��Ǝ��X�t�����̔�������
//���炭�����R���g���[���[�̂ݑΉ��\

class ControllerInputManager :public ISingleton<ControllerInputManager>, IInputManager
{
public:
	ControllerInputManager();
	~ControllerInputManager();

	/// <summary>
	/// ���[�v�̎n�߂̕��ɂ����Ă�������
	/// </summary>
	void InputLog()override;
	
	//�{�^������

	bool InputTrigger(int BUTTON)override;
	bool InputState(int BUTTON)override;
	bool InputRelease(int BUTTON)override;


	//�X�e�B�b�N����

	bool InputStickTrigger(ControllerStickSide STICK_TYPE,ControllerSide SIDE, int DEAD_ZONE = 6000);	
	bool InputStickState(ControllerStickSide STICK_TYPE, ControllerSide SIDE, int DEAD_ZONE = 6000);
	bool InputStickRelease(ControllerStickSide STICK_TYPE, ControllerSide SIDE, int DEAD_ZONE = 6000);
	//X���X�e�B�b�N�̌X���l
	const int GetJoyStickLXNum(int PAD_NUMBER = 0);
	//Y���X�e�B�b�N�̌X���l
	const int GetJoyStickLYNum(int PAD_NUMBER = 0);

	//X���X�e�B�b�N�̌X���l
	const int GetJoyStickRXNum(int PAD_NUMBER = 0);
	//Y���X�e�B�b�N�̌X���l
	const int GetJoyStickRYNum(int PAD_NUMBER = 0);


	//�g���K�[

	bool InputShoulderTrigger(ControllerShoulderSide SIDE, int DEAD_ZONE = 100);	
	bool InputShoulderState(ControllerShoulderSide SIDE, int DEAD_ZONE = 100);
	bool InputShoulderRelease(ControllerShoulderSide SIDE, int DEAD_ZONE = 100);


private:
	XINPUT_STATE state[XUSER_MAX_COUNT], oldState[XUSER_MAX_COUNT];

	//�ŏ��̈�t���[���ڂ�Release���肪����ɒʂ��Ă��܂��̂�h��
	bool initReleaseFlag;
	int roopCount;
};

