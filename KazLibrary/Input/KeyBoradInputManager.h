#pragma once
#include"../DirectXCommon/Base.h"
#include"IInputManager.h"
#include"../Helper/ISinglton.h"

//DirextInput�̃o�[�W�����w��
#define DIRECTINPUT_VERSION 0x0800

#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

/*
��Ȗ���
�L�[�{�[�h�ƃ}�E�X���͊֘A
�}�E�X�̍��W�͎擾�ł��Ȃ�
*/

enum MouseInputNumber
{
	MOUSE_INPUT_LEFT,
	MOUSE_INPUT_RIGHT,
	MOUSE_INPUT_MIDDLE,
};

class KeyBoradInputManager:public ISingleton<KeyBoradInputManager>
{
public:
	KeyBoradInputManager();
	~KeyBoradInputManager();

	void CreateDevice(HWND *HWND, HINSTANCE *INSTANCE);

	/// <summary>
	/// ���[�v�̎n�߂̕��ɂ����Ă�������
	/// </summary>
	void InputLog();

	bool InputTrigger(int BUTTON);
	bool InputState(int BUTTON);
	bool InputRelease(int BUTTON);

	bool MouseInputTrigger(MouseInputNumber CLICK);
	bool MouseInputState(MouseInputNumber CLICK);
	bool MouseInputRelease(MouseInputNumber CLICK);
	DirectX::XMFLOAT2 GetMousePoint();
	
private:
	HRESULT result;
	//���̓f�o�C�X
	IDirectInput8 *dinput;

	//�L�[�{�[�h
	IDirectInputDevice8 *devKeyBoard;
	char key[256], oldKey[256];
	int timer;
	int flame;

	//�}�E�X
	IDirectInputDevice8 *devMouse;
	DIMOUSESTATE2 mouseState = {}, oldMouseState = {};
	HWND *hwnd;


	bool initReleaseFlag;
	int roopCount;
};

