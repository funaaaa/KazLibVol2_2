#pragma once
#include"../DirectXCommon/Base.h"
#include"../Math/KazMath.h"
#include"../Helper/OutPutDebugStringAndCheckResult.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Loader/FbxModelResourceMgr.h"
#include"KazRenderHelper.h"
#include<vector>
#include<array>

namespace KazHelper
{
	/// <summary>
	/// ���l���z��̍ő�𒴂��邩�ǂ������ׂ܂�
	/// </summary>
	/// <param name="NUM">���ׂ������l</param>
	/// <param name="MAX_SIZE">�z��̍ő吔</param>
	/// <returns>true...���̐��l��0~�z��̍ő吔�̓��ɓ���܂��Bfalse...���̐��l��0~�z��̍ő吔�̓��ɓ���܂���</returns>
	inline bool IsitInAnArray(int NUM, size_t MAX_SIZE)
	{
		if (0 <= NUM && NUM < MAX_SIZE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/// <summary>
	/// string�^�̕������wchar_t�^�̕�����ɕϊ����܂�
	/// </summary>
	/// <param name="STRING">string�^�̕�����</param>
	/// <param name="WCHAR_STRING">wchar_t�^�̕�����</param>
	/// <param name="ARRAY_SIZE">wchar_t�^�̕�����̍ő�̗v�f��</param>
	void ConvertStringToWchar_t(std::string STRING, wchar_t *WCHAR_STRING, size_t ARRAY_SIZE);

	/// <summary>
	/// ���t���[���l���ς�������ǂ����m�F���܂�
	/// </summary>
	/// <param name="VALUE">���݂̒l</param>
	/// <param name="OLD_VALUE">1Flame�O�̒l</param>
	/// <returns>true...�ω���������,false...�ω����Ȃ�����</returns>
	bool DirtyFlag(DirectX::XMFLOAT3 VALUE, DirectX::XMFLOAT3 OLD_VALUE);
	bool DirtyFlag(DirectX::XMFLOAT2 VALUE, DirectX::XMFLOAT2 OLD_VALUE);
	bool DirtyFlag(float VALUE, float OLD_VALUE);


	/// <summary>
	/// ���݂̃��[���h���W����ʓ��ɂ��邩�ǂ������܂�
	/// </summary>
	/// <param name="POS">���ׂ���W</param>
	/// <returns>true...��ʓ�,false...��ʊO</returns>
	bool IsInScreen(const KazMath::Vec3<float> &POS);

	KazMath::Vec3<float> GetScreenPos(const KazMath::Vec3<float> &POS, const KazMath::Vec2<float> &LEFTUP_POS = { 0.0f,0.0f });

	KazMath::Vec3<float> GetRightDownStagePos(const KazMath::Vec3<float> &POS, const KazMath::Vec2<float> &RIGHT_POS = { static_cast<float>(WIN_X) - 400.0f,static_cast<float>(WIN_Y) });


	int GetDigit(int NUM);

	std::vector<int> CountNumber(int TIME, int ZERO = -1);


	std::vector<char>CovertStringToChar(std::string STRING);



}


