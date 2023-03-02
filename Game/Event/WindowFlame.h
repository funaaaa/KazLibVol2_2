#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"

/// <summary>
/// �Q�[�����̃E�B���h�E�̃t���[���`��
/// </summary>

enum WindowType
{
	WINDOW_2D,
	WINDOW_3D
};

class WindowFlame
{
public:
	WindowFlame();

	void Init(const KazMath::Transform3D &TRANSFORM, const KazMath::Vec2<float> &SCALE, bool APPEAR_WINDOW, WindowType TYPE);
	void Update();
	void Draw();

	void Start();
	void End();

	bool IsEnd();
	bool ReadyToWrite();

	KazMath::Vec3<float> basePos;
private:
	bool startFlag;				//�A�i�E���X���J�n���܂�
	bool endFlag;
	bool drawFlag;
	bool readyToWriteFlag;
	int stageNum;				//���݂̃X�e�[�W�ԍ���\�����܂�

	KazMath::Vec3<float> larpScale;

	std::array<int, 10> numberHandle;
	Sprite3DRender flameTex;	//�E�B���h�E�̕`��
	Sprite2DRender flame2DTex;	//�E�B���h�E�̕`��

	KazMath::Vec2<float> lerpMaxScale;

	WindowType windowType;

	void Update2D();
	void Update3D();
};

