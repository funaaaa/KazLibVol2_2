#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Helper/DirtyFlag.h"
#include"../Render/RenderData.h"
#include"../Loader/FbxModelResourceMgr.h"
#include<memory>

// 本来はRESOURCE_HANDLEで良いのだが、FbxModelResourceMgrの設計を変える必要があることをメモするために一旦これを使う。
struct FbxModelData;

/// <summary>
/// BottomLevelの加速構造体 Blasとして最低限必要な機能をまとめたクラスで、全てのモデルクラスが本クラスを持つ。
/// </summary>
class Blas {

private:

	/*===== メンバ変数 =====*/

	Microsoft::WRL::ComPtr<ID3D12Resource> blasBuffer_;		// Blasのバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> scratchBuffer_;	// Blasを形成する際に必要なバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> updateBuffer_;	// Blasのアップロード用バッファ

	bool isRaytracingEnabled;	// レイトレが有効化されているかのフラグ。距離によってレイトレをカリングする際に一時的に無効化させたり、モデルが描画されなくなった際にfalseにする。


public:

	/*===== メンバ関数 =====*/

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="IsOpaque"> 不透明フラグ t:不透明 f:半透明になる可能性がある </param>
	/// <param name="ModelDataHandle"> Blasを形成するモデルの形状を保存してあるデータのハンドル </param>
	Blas(bool IsOpaque, const FbxModelData& ModelDataHandle);

	/// <summary>
	/// Blasのバッファを返す。
	/// </summary>
	/// <returns> Blasのバッファ </returns>
	inline Microsoft::WRL::ComPtr<ID3D12Resource> GetBlasBuffer() { return blasBuffer_; }

	/// <summary> 
	/// 参照元のシェーダーレコードにアクセスして書き込む。
	/// </summary>
	/// <param name="Dst"> 書き込み先ポインタ </param>
	/// <param name="RecordSize"> 書き込むサイズ </param>
	/// <param name="StateObject"> 書き込むオブジェクト </param>
	/// <param name="HitGroupName"> 使用するHitGroupの名前 </param>
	/// <returns> 書き込み後のポインタ </returns>
	uint8_t* WriteShaderRecord(uint8_t* Dst, UINT RecordSize, Microsoft::WRL::ComPtr<ID3D12StateObject>& StateObject, LPCWSTR HitGroupName);

private:

	/// <summary>
	/// Blasの形状設定に必要な構造体を返す。
	/// </summary>
	/// <param name="IsOpaque"> 不透明フラグ t:不透明 f:半透明になる可能性がある </param>
	/// <param name="ModelDataHandle"> Blasを形成するモデルの形状を保存してあるデータのハンドル </param>
	/// <returns></returns>
	D3D12_RAYTRACING_GEOMETRY_DESC GetGeometryDesc(bool IsOpaque, const FbxModelData& ModelDataHandle);

	/// <summary>
	/// Blasを構築
	/// </summary>
	/// <param name="GeomDesc"> Blasの形状構造体。同クラスの関数から取得する。 </param>
	void BuildBlas(const D3D12_RAYTRACING_GEOMETRY_DESC& GeomDesc);

	/// <summary>
	/// バッファを生成する関数 面倒だったので私のエンジンからそのまま持ってきました。
	/// </summary>
	/// <param name="Size"> バッファサイズ </param>
	/// <param name="Flags"> バッファのフラグ </param>
	/// <param name="InitialState"> バッファの初期状態 </param>
	/// <param name="HeapType"> ヒープの種類 </param>
	/// <param name="Name"> バッファにつける名前 </param>
	/// <returns> 生成されたバッファ </returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBuffer(size_t Size, D3D12_RESOURCE_FLAGS Flags, D3D12_RESOURCE_STATES InitialState, D3D12_HEAP_TYPE HeapType, const wchar_t* Name = nullptr);

};