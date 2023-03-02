#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/HandleMaker.h"
#include"../Helper/ISinglton.h"
#include<fstream>
#include<sstream>
#include"../Buffer/CreateGpuBuffer.h"

struct materia
{
	std::string name;//�}�e���A����
	DirectX::XMFLOAT3 ambient;//�A���r�G���g�e���x
	DirectX::XMFLOAT3 diffuse;//�f�B�t���[�Y�e���x
	DirectX::XMFLOAT3 specular;//�X�y�L�����[�e���x
	float alpha;//��
	std::string textureFilename;//�e�N�X�`���t�@�C����

	materia()
	{
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0;
	}

	void Delete()
	{
		ambient = { 0.0f,0.0f,0.0f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 0.0;
	}
};


/// <summary>
/// .Obj�̃��f����`�悷��̂ɕK�v�ȃf�[�^������������
/// </summary>
struct ObjResourceData 
{
	//vector<Vertex> vert;
	//vector<unsigned short> indexNum;

	materia material;
	RESOURCE_HANDLE mtlHanlde;
	
	std::vector<unsigned short> posKey;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	UINT indexNum;

	std::vector<DirectX::XMFLOAT4>vertices;
	std::vector<UINT>index;
};



/// <summary>
/// .obj�ɕK�v�ȃf�[�^��ۊǂ��Aobj��`�悷��ۂɃf�[�^���g���񂷃N���X
/// </summary>
class ObjResourceMgr :public ISingleton<ObjResourceMgr>
{
public:
	ObjResourceMgr();
	~ObjResourceMgr();

	/// <summary>
	/// .obj�̃��f����ǂݍ��݂܂�
	/// .obj��.png��.mtl�̖��O�͓��ꂵ�ĉ�����
	/// </summary>
	/// <param name="RESOURCE">.obj�̃f�[�^������t�@�C���p�X</param>
	/// <param name="TEX_RESOURCE">.mtl�̉摜�f�[�^������t�@�C���p�X</param>
	/// <returns>�n���h��</returns>
	RESOURCE_HANDLE LoadModel(std::string RESOURCE);

	/// <summary>
	/// �w��̃��f���f�[�^���폜���܂�
	/// </summary>
	/// <param name="HANDLE">�w��̃n���h��</param>
	void Release(RESOURCE_HANDLE HANDLE);

	/// <summary>
	/// �w��̃n���h�����烊�\�[�X���擾���܂�
	/// </summary>
	/// <param name="HANDLE">�w��̃n���h��</param>
	ObjResourceData GetResourceData(RESOURCE_HANDLE HANDLE);

private:
	HandleMaker handle;

	std::vector<ObjResourceData> resource;
	std::vector<std::string> name;

	std::unique_ptr<CreateGpuBuffer> modelDataBuffers;


	RESOURCE_HANDLE LoadMaterial(const std::string &filename, ObjResourceData *resource, std::string MTL_RESOURCE);

};
