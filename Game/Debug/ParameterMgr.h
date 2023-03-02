#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include<string>
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../lib/rapidjson/include/rapidjson/document.h"
#include"../lib/rapidjson/include/rapidjson/istreamwrapper.h"
#include "../lib/rapidjson/include/rapidjson/stringbuffer.h"
#include "../lib/rapidjson/include/rapidjson/writer.h"
#include "../lib/rapidjson/include/rapidjson/ostreamwrapper.h"

class ParameterMgr
{
public:
	ParameterMgr();

	/// <summary>
	/// �t�@�C���ǂݍ���
	/// </summary>
	/// <param name="FILE_PASS">�t�@�C����</param>
	void LoadFile(const std::string &FILE_PASS);

	/// <summary>
	/// �t�@�C���쐬
	/// </summary>
	/// <param name="FILE_PASS">�t�@�C����</param>
	void ExportFile(const std::string &FILE_PASS);



	rapidjson::Document doc;

private:
};
