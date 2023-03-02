#include"../ShaderHeader/peraHeader.hlsli"

//float WhiteNoise(float2 coord) {
//	return frac(sin(dot(coord, float2(8.7819, 3.255))) * 437.645);
//}


float4 PSmain(Output input) : SV_TARGET
{

	/*float2 samplePoint = input.uv;
	float4 Tex = tex.Sample(smp, input.uv);
	float noise = WhiteNoise(input.uv * Time) - 0.5;
	Tex.rgb += float3(noise, noise, noise);
	return Tex;*/


	float w, h, levels;
	tex.GetDimensions(0, w, h, levels);

	float dx = 1.0f / w;
	float dy = 1.0f / h;
	float blur = 15;
	float4 ret = float4(0, 0, 0, 0);
	float4 col = tex.Sample(smp,input.uv);

	return tex.Sample(smp, input.uv);
	//return float4(input.uv, 1, 1);

	//���m�N����
	//RGB�ɂԂ肾�Ə�肭���m�N���ɏo���Ȃ�

	//float y = dot(col.rgb, float3(0.299, 0.587, 0.114));
	//return float4(y, y, y, 1);

	//�F�̔��]
	//return float4(1.0f - col.rgb, col.a);

	//�F�̊K���𗎂Ƃ�
	//float4 down = float4(col.rgb - fmod(col.rgb, 0.55f), col.a);
	//return down;

	//�ڂ�������



	//ret += tex.Sample(smp, input.uv + float2(-blur * dx, -blur * dy));
	//ret += tex.Sample(smp, input.uv + float2(0, -blur * dy));
	//ret += tex.Sample(smp, input.uv + float2(blur * dx, -blur * dy));

	//ret += tex.Sample(smp, input.uv + float2(-blur * dx, 0));
	//ret += tex.Sample(smp, input.uv);
	//ret += tex.Sample(smp, input.uv + float2(blur * dx, 0));

	//ret += tex.Sample(smp, input.uv + float2(-blur * dx, blur * dy));
	//ret += tex.Sample(smp, input.uv + float2(0, blur * dy));
	//ret += tex.Sample(smp, input.uv + float2(blur * dx, blur * dy));

	//return ret / 9.0f;



	////�G���{�X���H
	//float emboss = 2;
	//ret += tex.Sample(smp, input.uv + float2(-emboss * dx, -emboss * dy)) * 2;
	//ret += tex.Sample(smp, input.uv + float2(0, -emboss * dy));
	//ret += tex.Sample(smp, input.uv + float2(emboss * dx, -emboss * dy)) * 0;

	//ret += tex.Sample(smp, input.uv + float2(-emboss * dx, 0));
	//ret += tex.Sample(smp, input.uv);
	//ret += tex.Sample(smp, input.uv + float2(emboss * dx, 0)) * -1;

	//ret += tex.Sample(smp, input.uv + float2(-emboss * dx, emboss * dy)) * 0;
	//ret += tex.Sample(smp, input.uv + float2(0, emboss * dy)) * -1;
	//ret += tex.Sample(smp, input.uv + float2(emboss * dx, emboss * dy)) * -2;

	////���m�N�������Ă�����ۂ�������
	//float y = dot(ret.rgb, float3(0.299, 0.587, 0.114));
	//return float4(y, y, y, 1);



	//�V���[�v�l�X(�G�b�W�̋���)
	//float shapeness = 10;
	//ret += tex.Sample(smp, input.uv + float2(-shapeness * dx, -shapeness * dy)) * 0;
	//ret += tex.Sample(smp, input.uv + float2(0, -shapeness * dy)) * -1;
	//ret += tex.Sample(smp, input.uv + float2(shapeness * dx, -shapeness * dy)) * 0;

	//ret += tex.Sample(smp, input.uv + float2(-shapeness * dx, 0)) * -1;
	//ret += tex.Sample(smp, input.uv) * 5;
	//ret += tex.Sample(smp, input.uv + float2(shapeness * dx, 0)) * -1;

	//ret += tex.Sample(smp, input.uv + float2(-shapeness * dx, shapeness * dy)) * 0;
	//ret += tex.Sample(smp, input.uv + float2(0, shapeness * dy)) * -1;
	//ret += tex.Sample(smp, input.uv + float2(shapeness * dx, shapeness * dy)) * 0;

	//return ret;



	//float shapeness = 10;
	//ret += tex.Sample(smp, input.uv + float2(0, -2 * dy)) * -1;
	//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0)) * -1;
	//ret += tex.Sample(smp, input.uv) * 4;
	//ret += tex.Sample(smp, input.uv + float2(2 * dx, 0)) * -1;
	//ret += tex.Sample(smp, input.uv + float2(0, 2 * dy)) * -1;

	//float y = dot(ret.rgb, float3(0.299, 0.587, 0.114));
	////�G�b�W�̋���
	//y = pow(1.0f - y, 10.0f);

	////�]�v�ȗ֊s���������Ă����
	////����������������菬�������1,����ȊO�Ȃ�0�Ƃ���
	////y = step(0.2, y);

	//return float4(y, y, y, col.a);



	////�K�E�V�A���ڂ���(�ȈՔ�)
	////�ŏ�i
	//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 2 * dy)) * 1 / 256;
	//ret += tex.Sample(smp, input.uv + float2(-1 * dx, 2 * dy)) * 4 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 0 * dx, 2 * dy)) * 6 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 1 * dx, 2 * dy)) * 4 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 2 * dx, 2 * dy)) * 1 / 256;

	////���i
	//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 1 * dy)) * 4 / 256;
	//ret += tex.Sample(smp, input.uv + float2(-1 * dx, 1 * dy)) * 16 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 0 * dx, 1 * dy)) * 24 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 1 * dx, 1 * dy)) * 16 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 2 * dx, 1 * dy)) * 4 / 256;

	////���i
	//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0 * dy)) * 6 / 256;
	//ret += tex.Sample(smp, input.uv + float2(-1 * dx, 0 * dy)) * 24 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 0 * dx, 0 * dy)) * 36 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 1 * dx, 0 * dy)) * 24 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 2 * dx, 0 * dy)) * 6 / 256;

	////����i
	//ret += tex.Sample(smp, input.uv + float2(-2 * dx, -1 * dy)) * 4 / 256;
	//ret += tex.Sample(smp, input.uv + float2(-1 * dx, -1 * dy)) * 16 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 0 * dx, -1 * dy)) * 24 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 1 * dx, -1 * dy)) * 16 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 2 * dx, -1 * dy)) * 4 / 256;

	////�ŉ��i
	//ret += tex.Sample(smp, input.uv + float2(-2 * dx, -2 * dy)) * 1 / 256;
	//ret += tex.Sample(smp, input.uv + float2(-1 * dx, -2 * dy)) * 4 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 0 * dx, -2 * dy)) * 6 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 1 * dx, -2 * dy)) * 4 / 256;
	//ret += tex.Sample(smp, input.uv + float2( 2 * dx, -2 * dy)) * 1 / 256;

	//return ret;


	//�K�E�V�A���ڂ���(��)
	//ret += bkweights[0] * col;

	for (int i = 1; i < 8; ++i)
	{
		ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, input.uv + float2(dx * i, 0));
		ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, input.uv + float2(dx * -i,0));
	}

	//return float4(ret.rgb, col.a);



	//�c��
	/*float2 nmTex = effectTex.Sample(smp, input.uv).xy;
	nmTex = nmTex * 2.0f - 1.0f;

	return tex.Sample(smp, input.uv + nmTex * 0.1f);*/
}

float4 VerticalBokehPS(Output input) : SV_TARGET
{
	float w, h, levels;
	tex.GetDimensions(0, w, h, levels);




	float dx = 1.0f / w;
	float dy = 1.0f / h;
	float4 ret = float4(0, 0, 0, 0);
	float4 col = tex.Sample(smp,input.uv);


	ret += bkweights[0] * col;

	for (int i = 1; i < 8; ++i)
	{
		ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, input.uv + float2(0, dy * i));

		ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, input.uv + float2(0, dy * -i));
	}

	return float4(ret.rgb, col.a);
	return col;

}