#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../Helper/ISinglton.h"
#include "../Raytracing/RayDescriptorHeap.h"

/// <summary>
/// TopLevelの加速構造体 InstanceVectorを登録することによってレイトレ空間を構築する。
/// </summary>
class Tlas : public ISingleton<Tlas>{

private:

	/*===== メンバ変数 =====*/

	Microsoft::WRL::ComPtr<ID3D12Resource> tlasBuffer_;			// Tlasのバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> scratchBuffer_;		// Tlasを形成する際に必要なバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> instanceDescBuffer_;	// インスタンスバッファ
	void* instanceDescMapAddress_;								// インスタンスバッファにデータを送る際のアドレス
	int instanceCapacity_;										// 現在のTlasのバッファのInstance数の許容量。 InstanceVectorの要素数がこの数を超えたら全てのバッファを再生成する。
	RAY_HEAP_HANDLE descHeapIndex_;								// Tlasを保存してあるDescriptorHeap上のインデックス


public:

	/*===== メンバ関数 =====*/

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Tlas();

	/// <summary>
	/// TLASを構築。全てのDrawコールを終えた後に呼んでください。
	/// </summary>
	void Build();

private:

	/// <summary>
	/// Tlasを構築or再構築する関数。
	/// </summary>
	void ReBuildTlas();

	/// <summary>
	/// Tlasを更新する関数。
	/// </summary>
	void UpdateTlas();

	/// <summary>
	/// アドレスに情報を書き込む処理
	/// </summary>
	/// <param name="MapAddress"> データの送り先 </param>
	/// <param name="PData"> 送るデータ </param>
	/// <param name="DataSize"> 送るデータのサイズ </param>
	void WriteToMemory(void* MapAddress, const void* PData, size_t DataSize);

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