#pragma once
#include <memory>
#include <vector>
#include <string>
#include <array>
#include <wtypes.h>
#include "../Helper/ISinglton.h"

class HitGroup;
class RayRootsignature;

// HitGroup���Ǘ��A�ۑ�����N���X�B���݂�1��HitGroup�����������A���₷�\��ł��B
class HitGroupMgr : public ISingleton<HitGroupMgr> {

private:

	/*===== �����o�ϐ� =====*/

	std::vector<std::shared_ptr<HitGroup>> hitGroup_;	// �q�b�g�O���[�v


public:

	/// <summary>
	/// �q�b�g�O���[�v����ID
	/// </summary>
	enum HITGROUP_ID {

		DEF,
		MAX_HIT_GROUP,

	};

	/// <summary>
	/// �q�b�g�O���[�v����ID�A������
	/// </summary>
	std::vector<LPCWSTR> hitGroupNames;


public:

	/*===== �����o�֐� =====*/

	/// <summary>
	/// �Z�b�e�B���O����
	/// </summary>
	void Setting();

	/// <summary>
	/// �z�񐔂̃Q�b�^
	/// </summary>
	/// <returns> �q�b�g�O���[�v�̐� </returns>
	int GetHitGroupCount() { return static_cast<int>(hitGroup_.size()); }

	// CH�֘A�̃Q�b�^
	const LPCWSTR& GetCH(HITGROUP_ID Index);
	bool GetCHFlag(HITGROUP_ID Index);
	// AH�֘A�̃Q�b�^
	const LPCWSTR& GetAH(HITGROUP_ID Index);
	bool GetAHFlag(HITGROUP_ID Index);
	// IS�֘A�̃Q�b�^
	const LPCWSTR& GetIS(HITGROUP_ID Index);
	bool GetISFlag(HITGROUP_ID Index);
	// �e��v�f���̃Q�b�^
	int GetSRVCount(HITGROUP_ID Index);
	int GetCBVCount(HITGROUP_ID Index);
	int GetUAVCount(HITGROUP_ID Index);
	// ���[�J�����[�g�V�O�l�`���̃Q�b�^
	const std::shared_ptr<RayRootsignature> GetLocalRootSig(HITGROUP_ID Index);
	// �q�b�g�O���[�v������q�b�g�O���[�vID���擾����B
	int GetHitGroupID(const LPCWSTR& HitGroupName);

	// �q�b�g�O���[�v��SRV�̐����擾����B
	int GetHitGroupSRVCount(HITGROUP_ID HitGroupID);
	// �q�b�g�O���[�v��UAV�̐����擾����B
	int GetHitGroupUAVCount(HITGROUP_ID HitGroupID);

};