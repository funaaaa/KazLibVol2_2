#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include<DirectXMath.h>
#include<wrl.h>
#include"d3dx12.h"
#include<array>
#include<memory>
#include"pix3.h"

static const int WIN_X = 1280;
static const int WIN_Y = 720;

static const DirectX::XMFLOAT3 BG_COLOR_WHITE = { 255.0f,255.0f,255.0f };
static const DirectX::XMFLOAT3 BG_COLOR = { 29.0f, 19.0f, 72.0f };
//static const DirectX::XMFLOAT3 BG_COLOR = { 22,38,37 };

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

struct SpriteVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

struct ConstBufferData
{
	DirectX::XMFLOAT4 color;
	DirectX::XMMATRIX mat;
	DirectX::XMMATRIX viewproj;
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
};

struct LineVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 tickness;
};


struct posData
{
	DirectX::XMFLOAT3 endLinepos;
	bool checkFlag;
};

static const int MAX_BONES = 32;
struct ConstBufferDataSkin
{
	DirectX::XMMATRIX bones[MAX_BONES];
};

/// <summary>
/// fogData
/// xyz...Color
/// w...Density
/// </summary>
struct FogData
{
	DirectX::XMFLOAT4 fogdata;
};

static int TEST_COUNT;



struct RockOnPosData
{
	DirectX::XMFLOAT3 pos;
	bool useFlag;
};

struct LineData
{
	float tickness;
};

struct GoalLightData
{
	DirectX::XMFLOAT4 light;
};

struct GradationData
{
	DirectX::XMFLOAT4 firstColor;
	DirectX::XMFLOAT4 endColor;
};

struct Material
{
	std::string name;//�}�e���A����
	DirectX::XMFLOAT3 ambient;//�A���r�G���g�e���x
	DirectX::XMFLOAT3 diffuse;//�f�B�t���[�Y�e���x
	DirectX::XMFLOAT3 specular;//�X�y�L�����[�e���x
	float alpha;//��
	std::string textureFilename;//�e�N�X�`���t�@�C����

	Material()
	{
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0;
	}
};

struct ConstBufferDataB1
{
	DirectX::XMFLOAT3 ambient;	//�A���r�G���g�W��
	float pad1;			//�o�f�B���O
	DirectX::XMFLOAT3 diffuse;	//�f�B�t���[�Y�W��
	float pad2;			//�p�f�B���O
	DirectX::XMFLOAT3 specular;	//�X�y�L�����[�W��
	float alpha;		//�A���t�@
};

struct ScreenPosData
{
	DirectX::XMVECTOR LeftUpPos;
	DirectX::XMVECTOR LeftDownPos;
	DirectX::XMVECTOR RightUpPos;
	DirectX::XMVECTOR RightDownPos;
};

struct constBufferVhsData
{
	float time;
};

typedef int RESOURCE_HANDLE;
typedef int BUFFER_SIZE;