#include "ObjResourceMgr.h"
#include"../Loader/TextureResourceMgr.h"
#include"../Helper/KazHelper.h"

ObjResourceMgr::ObjResourceMgr()
{
	modelDataBuffers = std::make_unique<CreateGpuBuffer>();
}

ObjResourceMgr::~ObjResourceMgr()
{
}

RESOURCE_HANDLE ObjResourceMgr::LoadModel(std::string RESOURCE)
{
	//�t�@�C���ǂݍ���
	ifstream file;
	file.open(RESOURCE);
	if (file.fail())
	{
		string errorName = RESOURCE + "�̓ǂݍ��݂Ɏ��s���܂���\n";
		FailCheck(errorName);

		return -1;
	}

	//�������݂��ǂ���
	for (int i = 0; i < name.size(); i++)
	{
		if (RESOURCE == name[i])
		{
			return static_cast<RESOURCE_HANDLE>(i);
		}
	}

	RESOURCE_HANDLE setHandle = handle.GetHandle();

	//���O�̓o�^
	name.push_back(RESOURCE);
	resource.push_back({});

	vector<Vertex> vert;
	vector<USHORT> indi;
	vector<UINT> indiKeepData;
	vector<DirectX::XMFLOAT4>positions;
	vector<DirectX::XMFLOAT3>normals;
	vector<DirectX::XMFLOAT2>texcoords;

	unsigned int indexCountNum = 0;

	string filePass = RESOURCE;
	for (size_t i = RESOURCE.length() - 1; 0 < i; i--)
	{
		if (filePass[i] == '/')
		{
			break;
		}
		else
		{
			filePass.pop_back();
		}
	}
	string filename;

	//��s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//��s���̕�������X�g���[���ɕϊ����ĉ�͂��₷���Ȃ�
		istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;//������v��f���̔��f������
		getline(line_stream, key, ' ');

		if (key == "v") {
			//XYZ�ǂݍ���
			DirectX::XMFLOAT4 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			positions.emplace_back(position);
		}

		if (key == "f") {
			string index_string;
			int faceIndexCount = 0;
			while (getline(line_stream, index_string, ' ')) {
				//��s���̕�������X�g���[���ɕϊ����ĉ�͂��₷���Ȃ�
				istringstream index_stream(index_string);
				USHORT indexPos, indexNormal, indexTexcoord;

				index_stream >> indexPos;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				resource[resource.size() - 1].posKey.push_back(indexPos);

				/*
				��_�ɂ����_���W/�e�N�X�`�����W/�@���x�N�g��
				*/
				vert.push_back({});
				vert[vert.size() - 1].pos = { positions[indexPos - 1].x,positions[indexPos - 1].y,positions[indexPos - 1].z };
				vert[vert.size() - 1].uv = texcoords[indexTexcoord - 1];
				vert[vert.size() - 1].normal = normals[indexNormal - 1];

				// �C���f�b�N�X�f�[�^�̒ǉ�
				if (faceIndexCount >= 3) {
					// �l�p�`�|���S����4�_�ڂȂ̂ŁA
					// �l�p�`��0,1,2,3�̓� 2,3,0�ŎO�p�`���\�z����
					indi.emplace_back(static_cast<USHORT>(indexCountNum - 1));
					indi.emplace_back(static_cast<USHORT>(indexCountNum));
					indi.emplace_back(static_cast<USHORT>(indexCountNum - 3));

					indiKeepData.emplace_back(static_cast<UINT>(indexCountNum - 1));
					indiKeepData.emplace_back(static_cast<UINT>(indexCountNum));
					indiKeepData.emplace_back(static_cast<UINT>(indexCountNum - 3));
				}
				else
				{
					indi.emplace_back(static_cast<USHORT>(indexCountNum));
					indiKeepData.emplace_back(static_cast<UINT>(indexCountNum));
				}


				faceIndexCount++;
				indexCountNum++;
			}
		}

		if (key == "vt") {
			DirectX::XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			/*
			�g�p�������f�����O�c�[���ɂ���āA
			v�����̏㉺���t�̏ꍇ������A
			�����ł͓ǂݍ��ݏ����Ŕ��]�����đΉ����Ă���
			*/
			texcoord.y = 1.0f - texcoord.y;
			texcoords.emplace_back(texcoord);
		}

		if (key == "vn") {
			DirectX::XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			normals.emplace_back(normal);
		}

		if (key == "mtllib")
		{
			line_stream >> filename;
			resource[setHandle].mtlHanlde = LoadMaterial(filePass, &resource[setHandle], filePass + filename);
		}

	}
	file.close();

	//�e�N�X�`���ǂݍ��ݎ��s
	if (resource[setHandle].mtlHanlde == -1)
	{
		string errorName = filename + "�̉摜�ǂݍ��݂Ɏ��s���܂���\n";
		FailCheck(errorName);
		//handle.DeleteHandle(setHandle);
		//return -1;
	}


	/*
	���_���{���W,uv,�@��}
	�}�e���A��
	*/
	if (vert.size() <= 0)
	{
		ErrorCheck(RESOURCE + "�̓ǂݍ��݂Ɏ��s���܂���\n");
		return -1;
	}
	if (indi.size() <= 0)
	{
		ErrorCheck(RESOURCE + "�̓ǂݍ��݂Ɏ��s���܂���\n");
		return -1;
	}

	SucceedCheck(RESOURCE + "�̓ǂݍ��݂ɐ������܂���\n");

	//���_�f�[�^�ƃC���f�b�N�X�o�b�t�@�̐����A�]��---------------------------------------
	int vertByte = KazBufferHelper::GetBufferSize<int>(vert.size(), sizeof(Vertex));
	int indexByte = KazBufferHelper::GetBufferSize<int>(indi.size(), sizeof(unsigned short));

	RESOURCE_HANDLE vertHandle = modelDataBuffers->CreateBuffer
	(
		KazBufferHelper::SetVertexBufferData(vertByte)
	);
	RESOURCE_HANDLE indexHandle = modelDataBuffers->CreateBuffer
	(
		KazBufferHelper::SetIndexBufferData(indexByte)
	);
	modelDataBuffers->TransData
	(
		vertHandle,
		vert.data(),
		vertByte
	);
	modelDataBuffers->TransData
	(
		indexHandle,
		indi.data(),
		indexByte
	);
	//���_�f�[�^�ƃC���f�b�N�X�o�b�t�@�̐����A�]��---------------------------------------


	//���_�o�b�t�@�r���[�ƃC���f�b�N�X�o�b�t�@�r���[�̐ݒ�---------------------------------------
	resource[setHandle].vertexBufferView = KazBufferHelper::SetVertexBufferView(modelDataBuffers->GetGpuAddress(vertHandle), vertByte, sizeof(vert[0]));
	resource[setHandle].indexBufferView = KazBufferHelper::SetIndexBufferView(modelDataBuffers->GetGpuAddress(indexHandle), indexByte);
	//���_�o�b�t�@�r���[�ƃC���f�b�N�X�o�b�t�@�r���[�̐ݒ�---------------------------------------


	resource[setHandle].indexNum = static_cast<UINT>(indi.size());
	resource[setHandle].vertices = positions;
	resource[setHandle].index = indiKeepData;

	return setHandle;
}

void ObjResourceMgr::Release(RESOURCE_HANDLE HANDLE)
{
	if (KazHelper::IsitInAnArray(HANDLE, resource.size()))
	{
		handle.DeleteHandle(HANDLE);
		name[HANDLE] = "noName";
		//resource[HANDLE].vert.shrink_to_fit();
		//resource[HANDLE].indexNum.shrink_to_fit();
		resource[HANDLE].mtlHanlde = -1;
		resource[HANDLE].material.Delete();
	}
}

ObjResourceData ObjResourceMgr::GetResourceData(RESOURCE_HANDLE HANDLE)
{
	if (KazHelper::IsitInAnArray(HANDLE, resource.size()))
	{
		return resource[HANDLE];
	}
	else
	{
		ObjResourceData errorResource;
		errorResource.mtlHanlde = -1;
		//resource.indexNum.resize(0);
		//resource.vert.resize(0);
		return errorResource;
	}
}

RESOURCE_HANDLE ObjResourceMgr::LoadMaterial(const std::string &FILE_NAME, ObjResourceData *RESOURCE, std::string MTL_RESOURE)
{

	ifstream file;
	file.open(MTL_RESOURE);
	if (file.fail())
	{
		return -1;
	}


	RESOURCE_HANDLE lHandle = -1;

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);


		string key;
		getline(line_stream, key, ' ');

		//�擪�̃^�u�����͖�������
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}


		if (key == "newmtl")
		{
			line_stream >> RESOURCE->material.name;
		}
		if (key == "Ka")
		{
			line_stream >> RESOURCE->material.ambient.x;
			line_stream >> RESOURCE->material.ambient.y;
			line_stream >> RESOURCE->material.ambient.z;
		}
		if (key == "Kd")
		{
			line_stream >> RESOURCE->material.diffuse.x;
			line_stream >> RESOURCE->material.diffuse.y;
			line_stream >> RESOURCE->material.diffuse.z;
		}
		if (key == "Ks")
		{
			line_stream >> RESOURCE->material.specular.x;
			line_stream >> RESOURCE->material.specular.y;
			line_stream >> RESOURCE->material.specular.z;
		}
		if (key == "map_Kd")
		{
			line_stream >> RESOURCE->material.textureFilename;
			lHandle = TextureResourceMgr::Instance()->LoadGraph(FILE_NAME + RESOURCE->material.textureFilename);
		}
	}

	file.close();

	return lHandle;
}
