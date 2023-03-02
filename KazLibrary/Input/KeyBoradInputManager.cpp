#include "KeyBoradInputManager.h"
#include<d3d10.h>

KeyBoradInputManager::KeyBoradInputManager() {
	dinput = nullptr;
	devKeyBoard = nullptr;

	for (int i = 0; i < 256; i++) {
		oldKey[i] = 0;
		key[i] = 0;
	}

	timer = 0;
	flame = 0;

	initReleaseFlag = false;
	roopCount = 0;
}

KeyBoradInputManager::~KeyBoradInputManager()
{
}

void KeyBoradInputManager::CreateDevice(HWND *HWND, HINSTANCE *INSTANCE)
{
	//�f�o�C�X����
	result = DirectInput8Create(*INSTANCE, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);

	//�L�[�{�[�h
	result = dinput->CreateDevice(GUID_SysKeyboard, &devKeyBoard, NULL);
	result = devKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	result = devKeyBoard->SetCooperativeLevel(*HWND, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	//�}�E�X
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	result = devMouse->SetDataFormat(&c_dfDIMouse2);
	result = devMouse->SetCooperativeLevel(*HWND, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	hwnd = HWND;
}

void KeyBoradInputManager::InputLog()
{
	roopCount++;
	if (2 <= roopCount && !initReleaseFlag)
	{
		initReleaseFlag = true;
	}

	//�L�[�{�[�h
	result = devKeyBoard->Acquire();
	for (int i = 0; i < 256; i++)
	{
		oldKey[i] = key[i];
	}
	result = devKeyBoard->GetDeviceState(sizeof(key), key);


	//�}�E�X
	result = devMouse->Acquire();
	// �O��̓��͂�ۑ�
	oldMouseState = mouseState;
	// �}�E�X�̓���
	result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
}

bool KeyBoradInputManager::InputTrigger(int BUTTON)
{
	if (key[BUTTON] && !oldKey[BUTTON])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KeyBoradInputManager::InputState(int BUTTON)
{
	if (key[BUTTON])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KeyBoradInputManager::InputRelease(int BUTTON)
{
	if (initReleaseFlag)
	{
		if (!key[BUTTON] && oldKey[BUTTON])
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

bool KeyBoradInputManager::MouseInputTrigger(MouseInputNumber CLICK)
{
	if (!oldMouseState.rgbButtons[CLICK] && mouseState.rgbButtons[CLICK])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KeyBoradInputManager::MouseInputState(MouseInputNumber CLICK)
{
	if (oldMouseState.rgbButtons[CLICK] && mouseState.rgbButtons[CLICK])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KeyBoradInputManager::MouseInputRelease(MouseInputNumber CLICK)
{
	if (initReleaseFlag)
	{
		if (oldMouseState.rgbButtons[CLICK] && !mouseState.rgbButtons[CLICK])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

DirectX::XMFLOAT2 KeyBoradInputManager::GetMousePoint()
{
	//WinAPI�ŊȒP�����A�x��
	POINT WinAPIMousePos;
	GetCursorPos(&WinAPIMousePos);
	ScreenToClient(*hwnd, &WinAPIMousePos);


	DirectX::XMFLOAT2 mousePos;
	mousePos.x = static_cast<float>(WinAPIMousePos.x);
	mousePos.y = static_cast<float>(WinAPIMousePos.y);
	return mousePos;
}