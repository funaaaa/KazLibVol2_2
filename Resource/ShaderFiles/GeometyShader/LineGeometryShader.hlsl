#include"../ShaderHeader/BasicShaderHeader.hlsli"

struct GSOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

//���̑�����ς���V�F�[�_�[
[maxvertexcount(6)]
void GSmain(
	line VSOutputLine input[2],
	inout TriangleStream<GSOutput> output
)
{
    
    //0�n�_,1�I�_
    
    
    for (int i = 0; i < 2; i++)
    {
        float offset = input[i].tickness.x / 2.0f;

        //�����̓_�ƘA�����Ă���
        {
            //�E��
            GSOutput element;
            element.pos = input[i].svpos + float4(offset, 0.0f, 0.0f, 0.0f);
            element.uv = 0;
            
            output.Append(element);
        }
        {
            //����...�����オ����?
            GSOutput element;
            element.pos = input[i].svpos + float4(-offset, 0.0f, 0.0f, 0.0f);
            element.uv = 0;
            output.Append(element);
        }
        
            //����
        if (i == 0)
        {
            GSOutput element;
            element.pos = input[(i + 1) % 2].svpos + float4(offset * sign(i - 1), 0.0f, 0.0f, 0.0f);
            element.uv = 0;
            output.Append(element);
        }
        else
        {
            GSOutput element;
            element.pos = input[(i + 1) % 2].svpos + float4(offset * sign(i - 1) + offset, 0.0f, 0.0f, 0.0f);
            element.uv = 0;
            output.Append(element);
        }
        

        output.RestartStrip();
    }
}