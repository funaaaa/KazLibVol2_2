#pragma once
#include <string>
#include <memory>
#include <wtypes.h>

class RayRootsignature;

/// <summary>
/// HitGroupを生成する際に渡すデータ構造体
/// </summary>
struct EntryPoint {
	LPCWSTR entryPoint_;
	bool isActive_;
	EntryPoint() {};
	EntryPoint(LPCWSTR Entry, bool Flag) :entryPoint_(Entry), isActive_(Flag) {};
};
struct HitGroupInitData {

	EntryPoint CH_;		// ClosestHitShader
	EntryPoint AH_;		// AnyHitShader
	EntryPoint IS_;		// IntersectShader
	int SRVcount_;		// SRVの数
	int CBVcount_;		// CBVの数
	int UAVcount_;		// UAVの数
	HitGroupInitData() {};
};

/// <summary>
/// ヒットグループクラス
/// </summary>
class HitGroup {

private:

	/*===== メンバ変数 =====*/

	EntryPoint CH_;		// ClosestHitShader
	EntryPoint AH_;		// AnyHitShader
	EntryPoint IS_;		// IntersectShader
	int SRVcount_;		// SRVの数
	int CBVcount_;		// CBVの数
	int UAVcount_;		// UAVの数

	int registerSpace_;	// シェーダーレジスターのレジスタースペース番号

	LPCWSTR hitGroupName_;	// このヒットグループの名前

	std::shared_ptr<RayRootsignature> localRootSig_;	// このヒットグループで使用するローカルルートシグネチャ


public:

	/*===== メンバ関数 =====*/

	/// <summary>
	/// コンストラクタ
	/// </summary>
	HitGroup();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="InputData"> HitGroupの初期化用データ </param>
	/// <param name="RegisterSpace"> レジスタスペース </param>
	/// <param name="HitGroupName"> このHitGroupの名前の文字列 </param>
	void Generate(const HitGroupInitData& InputData, int RegisterSpace, const LPCWSTR& HitGroupName);

	// 各種ゲッタ
	const LPCWSTR& GetCH() { return CH_.entryPoint_; }
	const LPCWSTR& GetAH() { return AH_.entryPoint_; }
	const LPCWSTR& GetIS() { return IS_.entryPoint_; }
	bool GetCHFlag() { return CH_.isActive_; }
	bool GetAHFlag() { return AH_.isActive_; }
	bool GetISFlag() { return IS_.isActive_; }
	int GetSRVCount() { return SRVcount_; }
	int GetCBVCount() { return CBVcount_; }
	int GetUAVCount() { return UAVcount_; }
	const std::shared_ptr<RayRootsignature> GetLoacalRootSig() {return localRootSig_; };

};