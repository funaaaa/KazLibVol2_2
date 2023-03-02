#include "WinApi.h"
#include"../Imgui/imgui_impl_win32.h"

//Imgui�̃��j���[�𓮂�������A�T�C�Y�ύX���邽�߂ɕK�v
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

//DISARDBLE...����������A�C�R����j�����Ă��ǂ���������
#define KITTY ICON DISCARDABLE "icon.rc"


LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{
	switch (msg) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}



WinApi::WinApi() 
{
	//hwnd = nullptr;
}

HRESULT WinApi::CreateMyWindow(int WINDOW_SIZE_X, int WINDOW_SIZE_Y) 
{
	int CheckError = 0;

	if (WINDOW_SIZE_X <= 0 || WINDOW_SIZE_Y <= 0)
	{
		return S_FALSE;
	}

	//Win = WindowProc;


	/*
	�F��ȃE�B���h�E�̐ݒ�
	��s�ځA�E�B���h�E�N���X
	�O�s�ځA�E�B���h�E�v���V�[�W��
	�l�s�ځA�E�B���h�E�n���h��
	�܍s�ځA�J�[�\���w��
	*/

	window.cbSize = sizeof(WNDCLASSEX);
	window.lpfnWndProc = (WNDPROC)WindowProc;
	window.lpszClassName = L"DirectX";
	window.hInstance = GetModuleHandle(nullptr);
	window.hCursor = LoadCursor(NULL, IDC_ARROW);	
	window.hIcon = LoadIcon(window.hInstance, TEXT("KITTY"));

	

	//�E�B���h�E�N���X��OS�ɓo�^
	CheckError = RegisterClassEx(&window);
	if (!CheckError) 
	{
		return S_FALSE;
	}

	//�E�B���h�E�T�C�Y�쐬
	RECT wrc = { 0,0,WINDOW_SIZE_X,WINDOW_SIZE_Y };
	//�����ŃT�C�Y�␳
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);


	/*
	�E�B���h�E�I�u�W�F�N�g�̐���

	�ォ�珇��CreateWinodw�̈����̖���

	�E�B���h�E�̃N���X��
	�^�C�g���o�[�̕�����
	�W���I�ȃE�B���h�E�X�^�C��
	�\��X���W(OS�ɔC����)
	�\��Y���W(OS�ɔC����)
	�E�B���h�E����
	�E�B���h�E�c��
	�e�E�B���h�E�n���h��
	���j���[�n���h��
	�Ăяo���A�v���P�[�V����
	�I�v�V����
	*/
	
	hwnd = CreateWindow(
		window.lpszClassName,
		L"CyberJack",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		window.hInstance,
		nullptr
	);



	//�E�B���h�E�\��
	CheckError = ShowWindow(hwnd, SW_SHOW);
	if (!CheckError) 
	{
		return S_OK;
	}

	return S_FALSE;
}

void WinApi::UnregisterWindowClass()
{
	//�E�B���h�E�N���X�o�^����
	UnregisterClass(window.lpszClassName, window.hInstance);
}

