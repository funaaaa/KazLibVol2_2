#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../KazLibrary/Easing/easing.h"
#include"../Game/Effect/EaseScaleParticle.h"
#include"../Game/Effect/IHitEffectEmitter.h"
#include<array>

/// <summary>
/// https://youtu.be/nvB6ga7w-2o
/// ����̈�ԍŏ��ɓG���U��������o��G�t�F�N�g�̋���
/// </summary>
class HitEffectPattern1Emitter:public IHitEffectEmitter
{
public:
	HitEffectPattern1Emitter();

	void Init(const KazMath::Vec2<float> &POS);
	void Update();
	void Draw();

	bool IsActive();
private:
	static const int PARTICLE_MAX = 4;
	std::array<KazMath::Vec2<float>, PARTICLE_MAX>baseScale;
	std::array<KazMath::Vec2<float>, PARTICLE_MAX>easeScale;
	std::array<KazMath::Vec2<int>, PARTICLE_MAX>timer;

	std::array<std::unique_ptr<EaseScaleParticle>, PARTICLE_MAX>particle;
};

