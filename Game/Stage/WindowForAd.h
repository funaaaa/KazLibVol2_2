#pragma once
#include"../KazLibrary/Render/KazRender.h"

/// <summary>
/// 3D��ԏ�ɃX�v���C�g��z�u�A�E�B���h�E�̃t���[�����ɉ摜��`��A��O�ɓ����܂�
/// </summary>
class WindowForAd
{
public:
	WindowForAd();
	void Init(KazMath::Vec3<float> &POS, const std::array<RESOURCE_HANDLE, 10> &HANDLE_ARRAY);
	void Update();
	void Draw();

private:
	Sprite3DRenderPtr spriteRender;
	int timer;
	int maxTimer;
	bool exchangeWindowFlag;
	int noiseTimer;
	int noiseMaxTimer;

	RESOURCE_HANDLE noiseHandle;
	std::array<RESOURCE_HANDLE, 10>windowTexHandleArray;

	struct ObjectData
	{
		float seed;
		DirectX::XMFLOAT3 pad;
		DirectX::XMFLOAT4 fogData;
	};

	ObjectData data;
};

