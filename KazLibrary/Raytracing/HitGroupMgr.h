#pragma once
#include <memory>
#include <vector>
#include <string>
#include <array>
#include <wtypes.h>
#include "../Helper/ISinglton.h"

class HitGroup;
class RayRootsignature;

// HitGroupを管理、保存するクラス。現在は1つのHitGroupしか無いが、増やす予定です。
class HitGroupMgr : public ISingleton<HitGroupMgr> {

private:

	/*===== メンバ変数 =====*/

	std::vector<std::shared_ptr<HitGroup>> hitGroup_;	// ヒットグループ


public:

	/// <summary>
	/// ヒットグループ識別ID
	/// </summary>
	enum HITGROUP_ID {

		DEF,
		MAX_HIT_GROUP,

	};

	/// <summary>
	/// ヒットグループ識別ID、文字列
	/// </summary>
	std::vector<LPCWSTR> hitGroupNames;


public:

	/*===== メンバ関数 =====*/

	/// <summary>
	/// セッティング処理
	/// </summary>
	void Setting();

	/// <summary>
	/// 配列数のゲッタ
	/// </summary>
	/// <returns> ヒットグループの数 </returns>
	int GetHitGroupCount() { return static_cast<int>(hitGroup_.size()); }

	// CH関連のゲッタ
	const LPCWSTR& GetCH(HITGROUP_ID Index);
	bool GetCHFlag(HITGROUP_ID Index);
	// AH関連のゲッタ
	const LPCWSTR& GetAH(HITGROUP_ID Index);
	bool GetAHFlag(HITGROUP_ID Index);
	// IS関連のゲッタ
	const LPCWSTR& GetIS(HITGROUP_ID Index);
	bool GetISFlag(HITGROUP_ID Index);
	// 各種要素数のゲッタ
	int GetSRVCount(HITGROUP_ID Index);
	int GetCBVCount(HITGROUP_ID Index);
	int GetUAVCount(HITGROUP_ID Index);
	// ローカルルートシグネチャのゲッタ
	const std::shared_ptr<RayRootsignature> GetLocalRootSig(HITGROUP_ID Index);
	// ヒットグループ名からヒットグループIDを取得する。
	int GetHitGroupID(const LPCWSTR& HitGroupName);

	// ヒットグループのSRVの数を取得する。
	int GetHitGroupSRVCount(HITGROUP_ID HitGroupID);
	// ヒットグループのUAVの数を取得する。
	int GetHitGroupUAVCount(HITGROUP_ID HitGroupID);

};