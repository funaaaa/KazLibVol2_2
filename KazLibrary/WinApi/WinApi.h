#pragma once
#include<Windows.h>
#include<string>

/// <summary>
/// �E�B���h�E�̍쐬�����܂�
/// </summary>
class WinApi 
{
public:
	WinApi();

	/// <summary>
	/// �E�B���h�E�̍쐬�����܂�
	/// </summary>
	/// <param name="WINDOW_SIZE_X">�E�B���h�E��X�T�C�Y</param>
	/// <param name="WINDOW_SIZE_Y">�E�B���h�E��Y�T�C�Y</param>
	/// <returns>true...����,false...���s</returns>
	HRESULT CreateMyWindow(int WINDOW_SIZE_X,int WINDOW_SIZE_Y);


	void FPS()
	{
		static DWORD prev_time = timeGetTime();    // �O��̎���
		static int frame_count = 0;        // �t���[���J�E���g
		DWORD now_time = timeGetTime();        // ����̃t���[���̎���

		frame_count++;    // �t���[�������J�E���g����

		// �o�ߎ��Ԃ��P�b�𒴂�����J�E���g�Ǝ��Ԃ����Z�b�g
		if (now_time - prev_time >= 1000)
		{
			wchar_t fps[1000];
			_itow_s(frame_count, fps, 10);
			wchar_t moji[] = L"FPS";
			wcscat_s(fps, moji);
			SetWindowText(hwnd, fps);
			//OutputDebugString(fps);

			prev_time = now_time;
			frame_count = 0;
		}
	}


	/// <summary>
	/// �E�B���h�E�N���X�o�^����
	/// </summary>
	void UnregisterWindowClass();

	HWND hwnd;
	WNDCLASSEX window{};
private:
};

