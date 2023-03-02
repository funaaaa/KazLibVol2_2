#pragma once
#include <array>
#include <wrl.h>
#include "../DirectXCommon/d3dx12.h"

/// <summary>
/// レイトレーシングで使用するグローバルルートシグネチャ、ローカルルートシグネチャクラス
/// </summary>
class RayRootsignature {

private:

	/*===== 定数 =====*/

	static const int MAX_ROOTPARAM = 16;
	static const int MAX_SAMPLER = 26;


private:

	/*===== メンバ変数 =====*/

	std::array<CD3DX12_ROOT_PARAMETER, MAX_ROOTPARAM> rootparam_;	// ルートパラメーター
	std::array<CD3DX12_DESCRIPTOR_RANGE, MAX_ROOTPARAM> descRange_;	// ディスクリプタテーブル
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature_;		// ルートシグネチャ
	std::array<CD3DX12_STATIC_SAMPLER_DESC, MAX_SAMPLER> sampler_;	// スタティックサンプラー
	UINT rootparamCount_;											// ルートパラメーター数
	UINT samplerCount_;												// サンプラーの数


public:

	/*===== メンバ関数 =====*/

	/// <summary>
	/// コンストラクタ
	/// </summary>
	RayRootsignature() {
		rootparamCount_ = 0;
		samplerCount_ = 0;
	}

	/// <summary>
	/// ルートパラメーター追加処理
	/// </summary>
	/// <param name="Type"> データの種類 SRVとかUAVとか </param>
	/// <param name="ShaderRegister"> レジスタ番号 t0とかの0の方 </param>
	/// <param name="RegisterSpace"> レジスタスペース t0の後にくるspaceの数字 </param>
	void AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE Type, UINT ShaderRegister, UINT RegisterSpace = 0);

	/// <summary>
	/// スタティックサンプラー追加処理
	/// </summary>
	/// <param name="RegisterSpace"> レジスタスペース t0の後にくるspaceの数字 </param>
	void AddStaticSampler(int RegisterSpace = 0);

	/// <summary>
	/// ルートシグネチャの生成
	/// </summary>
	/// <param name="IsLocal"> ローカルルートシグネチャかどうか </param>
	/// <param name="Name"> バッファの名前 </param>
	void Build(bool IsLocal, const wchar_t* Name = nullptr);

	// ゲッタ
	inline Microsoft::WRL::ComPtr<ID3D12RootSignature>& GetRootSig() { return rootsignature_; }

};