#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Render/KazRender.h"

class GoalBoxPortalStringEffect
{
public:
	GoalBoxPortalStringEffect();

	void Init(const KazMath::Vec3<float> &POS);
	void Update();
	void Draw();

	void Start();
private:
	bool startFlag;//�J�n

	KazMath::Vec3<float>basePos;//��̈ʒu
	static const int STRING_TEXTURE_ARRAY_NUM = 100;	//�\������X�v���C�g�̑���
	std::array<Sprite3DRender, STRING_TEXTURE_ARRAY_NUM> stringRender;//�\������X�v���C�g�B

	void InitStringEffect(int INDEX);
};
