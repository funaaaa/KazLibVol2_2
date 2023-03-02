#include "FPSManager.h"
#include <thread>
#include <chrono>
//#include "Window.h"

#pragma comment(lib, "winmm.lib")

long long FpsManager::count_ = 0;
int FpsManager::fps_limit_ = 0;
long long FpsManager::time_ = FpsManager::CurrentTime();
double FpsManager::fps_result_ = 0;
long long FpsManager::current_time_ = 0;
long long FpsManager::next_ = 0;

void FpsManager::RegulateFps(const int fps_limit) {

	// fps�l��ݒ�
	fps_limit_ = fps_limit;

	count_++;

	// ����t���[�����ɂȂ�����t���[�����[�g�̍X�V
	if (fps_limit_ <= count_) {
		CalculateFps();
	}

	current_time_ = CurrentTime();

	if (current_time_ < next_) {
		// �X�V���Ԃ܂őҋ@
		std::this_thread::sleep_for(std::chrono::microseconds(next_ - current_time_));

		// ���̍X�V���Ԃ��v�Z(1�b / �t���[�����[�g���Z)
		next_ += (1000 * 1000 / fps_limit_);
	} else {
		// �X�V���Ԃ��߂����ꍇ�͌��ݎ������玟�̍X�V���Ԃ��v�Z
		next_ = current_time_ + (1000 * 1000 / fps_limit_);
	}
}

long long FpsManager::CurrentTime() {
	using namespace std::chrono;

	system_clock::duration duration = system_clock::now().time_since_epoch();

	return duration_cast<microseconds>(duration).count();
}

void FpsManager::CalculateFps() {
	long long end = CurrentTime();

	fps_result_ = static_cast<double>(1000) / (end - time_) * count_;

	time_ = end;

	count_ = 0;
}

//void FpsManager::ObserveFPS() {
//	static DWORD prev_time = timeGetTime();	// �O��̎���
//	static int frame_count = 0;				// �t���[���J�E���g
//	DWORD now_time = timeGetTime();			// ����̃t���[���̎���
//
//	frame_count++;							// �t���[�������J�E���g����
//
//	// �o�ߎ��Ԃ��P�b�𒴂�����J�E���g�Ǝ��Ԃ����Z�b�g
//	if (now_time - prev_time >= 1000) {
//		wchar_t fps[1000];
//		_itow_s(frame_count, fps, 10);
//		wchar_t moji[] = L"FPS";
//		wcscat_s(fps, moji);
//		SetWindowText(Window::GetHwnd(), fps);
//		//OutputDebugString(fps);
//
//		prev_time = now_time;
//		frame_count = 0;
//	}
//}
