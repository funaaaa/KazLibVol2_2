#include "../Raytracing/HitGroupMgr.h"
#include "../Raytracing/HitGroup.h"
#include <assert.h>

const LPCWSTR& HitGroupMgr::GetCH(HITGROUP_ID Index)
{
	return hitGroup_[Index]->GetCH();
}

const LPCWSTR& HitGroupMgr::GetAH(HITGROUP_ID Index)
{
	return hitGroup_[Index]->GetAH();
}

const LPCWSTR& HitGroupMgr::GetIS(HITGROUP_ID Index)
{
	return hitGroup_[Index]->GetIS();
}
bool HitGroupMgr::GetCHFlag(HITGROUP_ID Index)
{
	return hitGroup_[Index]->GetCHFlag();
}
bool HitGroupMgr::GetAHFlag(HITGROUP_ID Index)
{
	return hitGroup_[Index]->GetAHFlag();
}

bool HitGroupMgr::GetISFlag(HITGROUP_ID Index)
{
	return hitGroup_[Index]->GetISFlag();
}

int HitGroupMgr::GetSRVCount(HITGROUP_ID Index)
{
	return hitGroup_[Index]->GetSRVCount();
}

int HitGroupMgr::GetCBVCount(HITGROUP_ID Index)
{
	return hitGroup_[Index]->GetCBVCount();
}

int HitGroupMgr::GetUAVCount(HITGROUP_ID Index)
{
	return hitGroup_[Index]->GetUAVCount();
}

const std::shared_ptr<RayRootsignature> HitGroupMgr::GetLocalRootSig(HITGROUP_ID Index)
{
	return hitGroup_[Index]->GetLoacalRootSig();
}

int HitGroupMgr::GetHitGroupID(const LPCWSTR& HitGroupName)
{

	/*===== �q�b�g�O���[�v������q�b�g�O���[�vID���擾����B =====*/

	int hitGroupID = -1;

	for (int index = 0; index < MAX_HIT_GROUP; ++index) {

		// �������O��������B
		if (HitGroupName != hitGroupNames[index]) continue;

		// ID��ۑ��B
		hitGroupID = index;

	}

	// -1�������疼�O�̑��݂��Ȃ��q�b�g�O���[�v���Z�b�g���悤�Ƃ��Ă���̂ŃA�T�[�g���o���B
	if (hitGroupID == -1) {

		// ���݂��Ȃ��q�b�g�O���[�v���Z�b�g���悤�Ƃ��Ă���B
		assert(0);

	}

	return hitGroupID;
}

int HitGroupMgr::GetHitGroupSRVCount(HITGROUP_ID HitGroupID)
{

	/*===== �q�b�g�O���[�v��SRV�̐����擾 =====*/

	return hitGroup_[HitGroupID]->GetSRVCount();

}

int HitGroupMgr::GetHitGroupUAVCount(HITGROUP_ID HitGroupID)
{

	/*===== �q�b�g�O���[�v��SRV�̐����擾 =====*/

	return hitGroup_[HitGroupID]->GetUAVCount();

}

void HitGroupMgr::Setting()
{

	/*===== �q�b�g�O���[�v�ݒ� =====*/

	// �q�b�g�O���[�v���̃R���e�i��ݒ�B
	hitGroupNames.emplace_back(L"DENOISE_AO_HIT_GROUP");

	// �q�b�g�O���[�v�𐶐��B
	HitGroupInitData initData;

	// ���v�f
	initData.CH_ = { L"mainCHS",true };
	initData.AH_ = { L"mainAnyHit",true };
	initData.IS_ = { L"",false };
	initData.SRVcount_ = 3;
	initData.UAVcount_ = 0;
	initData.CBVcount_ = 0;
	hitGroup_.push_back(std::make_shared<HitGroup>());
	hitGroup_[DEF]->Generate(initData, 1, hitGroupNames[DEF]);

}