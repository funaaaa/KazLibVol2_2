#pragma once
#include<math.h>

enum EasingType 
{
	In, Out, InOut
};

enum EaseInType
{
	Linear,Smooth,Quad,Cubic,Quart,Quint,Sine,Circ,Exp,Back,SoftBack,Elastic,Bounce
};

//EasingType...In,Out,InOut�AEaseInType...InLinear,InSmooth...�At...�ω���

/// <summary>
/// �n�܂���Ǝ�ނ����킹�ăC�[�W���O�̃O���t�����A�ω��������Ēl��Ԃ��܂�
/// </summary>
/// <param name="ChooseType">�C�[�W���O�̎n�܂��</param>
/// <param name="ChooseEase">�C�[�W���O�̎��</param>
/// <param name="t">�ω���</param>
/// <returns></returns>
float EasingMaker(int ChooseType, int ChooseEase, float t);

//EaseIn�̎��

float InLinear(float t);
float InSmooth(float t);
float InQuad(float t);
float InCubic(float t);
float InQuart(float t);
float InQuint(float t);
float InSine(float t);
float InCirc(float t);
float InExp(float t);
float InBack(float t);
float InSoftBack(float t);
float InElastic(float t);
float InBounce(float t);
float Max(float a, float b);

/// <summary>
/// �ω������v�Z���܂�
/// </summary>
/// <param name="*t">�ω���������ϐ�</param>
/// <param name="rate">1Flame�ɂǂꂭ�炢���������邩</param>
/// <param name="maxRate">�ő�</param>
/// <returns></returns>
void Rate(float *T, float RATE, float MAX_RATE);