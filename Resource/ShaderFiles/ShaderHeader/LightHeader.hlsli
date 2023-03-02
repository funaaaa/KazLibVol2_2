struct LightVSOutput 
{
    float4 svpos:SV_POSITION;	//�V�X�e���p���_���W
    float3 normal:NORMAL;		//�@���x�N�g��
    float3 worldPos:TEXCOORD;   //����TEXCOORD...?
};

cbuffer light : register(b1)
{
    // �f�B���N�V�������C�g�p�̃f�[�^
    float3 dirDirection;    // ���C�g�̕���
    float3 dirColor;        // ���C�g�̃J���[

    // �萔�o�b�t�@�[�Ƀ|�C���g���C�g�p�̕ϐ���ǉ�
    float3 ptPosition;      // �|�C���g���C�g�̈ʒu
    float3 ptColor;         // �|�C���g���C�g�̃J���[
    float ptRange;          // �|�C���g���C�g�̉e���͈�

    float3 spPosition;      // �X�|�b�g���C�g�̈ʒu
    float3 spColor;         // �X�|�b�g���C�g�̃J���[
    float spRange;          // �X�|�b�g���C�g�̎ˏo�͈�
    float3 spDirection;     // �X�|�b�g���C�g�̎ˏo����
    float spAngle;          // �X�|�b�g���C�g�̎ˏo�p�x

    float3 eyePos;          // ���_�̈ʒu

    float3 ambientLight;    // �A���r�G���g���C�g
};

//struct DirectionalLight
//{
//    float3 direction;   // ���C�g�̕���
//    float4 color;       // ���C�g�̐F
//};
//
//// ���C�g�p�̒萔�o�b�t�@�[
//cbuffer LightCb : register(b1)
//{
//    DirectionalLight directionalLight[4];
//    float3 eyePos;          // �J�����̎��_
//    float specPow;          // �X�y�L�����̍i��
//    float3 ambientLight;    // ����
//};


float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    // �s�N�Z���̖@���ƃ��C�g�̕����̓��ς��v�Z����
    float t = dot(normal, lightDirection) * -1.0f;

    // ���ς̒l��0�ȏ�̒l�ɂ���
    t = max(0.0f, t);

    // �g�U���ˌ����v�Z����
    return lightColor * t;
}

float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
    // ���˃x�N�g�������߂�
    float3 refVec = reflect(lightDirection, normal);

    // �������������T�[�t�F�C�X���王�_�ɐL�т�x�N�g�������߂�
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);

    // ���ʔ��˂̋��������߂�
    float t = dot(refVec, toEye);

    // ���ʔ��˂̋�����0�ȏ�̐��l�ɂ���
    t = max(0.0f, t);

    // ���ʔ��˂̋������i��
    t = pow(t, 5.0f);

    // ���ʔ��ˌ������߂�
    return lightColor * t;
}


Texture2D<float4> mainRenderTargetTexture : register(t0); // ���C�������_�����O�^�[�Q�b�g�̃e�N�X�`��
sampler Sampler : register(s0);
