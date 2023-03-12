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

	/*===== ヒットグループ名からヒットグループIDを取得する。 =====*/

	int hitGroupID = -1;

	for (int index = 0; index < MAX_HIT_GROUP; ++index) {

		// 同じ名前だったら。
		if (HitGroupName != hitGroupNames[index]) continue;

		// IDを保存。
		hitGroupID = index;

	}

	// -1だったら名前の存在しないヒットグループをセットしようとしているのでアサートを出す。
	if (hitGroupID == -1) {

		// 存在しないヒットグループをセットしようとしている。
		assert(0);

	}

	return hitGroupID;
}

int HitGroupMgr::GetHitGroupSRVCount(HITGROUP_ID HitGroupID)
{

	/*===== ヒットグループのSRVの数を取得 =====*/

	return hitGroup_[HitGroupID]->GetSRVCount();

}

int HitGroupMgr::GetHitGroupUAVCount(HITGROUP_ID HitGroupID)
{

	/*===== ヒットグループのSRVの数を取得 =====*/

	return hitGroup_[HitGroupID]->GetUAVCount();

}

void HitGroupMgr::Setting()
{

	/*===== ヒットグループ設定 =====*/

	// ヒットグループ名のコンテナを設定。
	hitGroupNames.emplace_back(L"DENOISE_AO_HIT_GROUP");

	// ヒットグループを生成。
	HitGroupInitData initData;

	// 第一要素
	initData.CH_ = { L"mainCHS",true };
	initData.AH_ = { L"mainAnyHit",true };
	initData.IS_ = { L"",false };
	initData.SRVcount_ = 3;
	initData.UAVcount_ = 0;
	initData.CBVcount_ = 0;
	hitGroup_.push_back(std::make_shared<HitGroup>());
	hitGroup_[DEF]->Generate(initData, 1, hitGroupNames[DEF]);

}