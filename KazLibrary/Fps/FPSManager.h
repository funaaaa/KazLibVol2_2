#pragma once
#include"../Helper/ISinglton.h"

/// <summary>
/// fps���Ǘ����܂��B
/// </summary>
class FpsManager final : public ISingleton<FpsManager> {
	//friend Singleton<FpsManager>;

	// �t���[�����J�E���g
	static long long count_;
	// ��{fps�l
	static int fps_limit_;
	// �Ȃɂ���
	static long long time_;
	// fps�l�o��
	static double fps_result_;
	// ���ݎ���
	static long long current_time_;
	// ���̍X�V����
	static long long next_;

public:

	static const int &GetFpsLimit() { return fps_limit_; }

	/// <summary>
	/// fps�𐧌�����
	/// </summary>
	/// <param name="fps_limit">fps���</param>
	static void RegulateFps(const int fps_limit);

	/// <summary>
	/// �^�C�g���o�[��fps��`��
	/// </summary>
	static void ObserveFPS();

private:
	/// <summary>
	/// ���ݎ������擾����
	/// </summary>
	/// <returns>���ݎ���</returns>
	static long long CurrentTime();

	/// <summary>
	/// fps���v�Z����
	/// </summary>
	static void CalculateFps();
};
