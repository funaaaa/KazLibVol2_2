#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"

class MoiveEffect
{
public:
	MoiveEffect();
	void Init();
	void Update();
	void Draw();


	bool startFlag;
private:
	Sprite2DRender topTex, buttomTex;//�㉺�Ƀe�N�X�`����`�悷��@���@�|���S���ɕύX

	KazMath::Vec2<float> lerpTopPos, lerpButtomPos;//���W
};

