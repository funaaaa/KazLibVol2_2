#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "../Helper/ISinglton.h"

typedef int RAY_HEAP_HANDLE;	// レイトレ用のディスクリプタヒープクラスで使用するハンドル

/// <summary>
/// レイトレ用のディスクリプタヒープクラス DescriptorHeapMgr周りと設計を合わせると時間がかかりそうだったので一旦ここに。
/// </summary>
class RayDescriptorHeap : public ISingleton<RayDescriptorHeap> {

private:

	/*===== メンバ変数 =====*/

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;	// レイトレ用のディスクリプタヒープ
	RAY_HEAP_HANDLE head_;								// 現在の先頭

public:

	/// <summary>
	/// 生成するバッファの種類
	/// </summary>
	enum class BUFFER_TYPE {
		//SRV,	// テクスチャ系
		//CBV,	// 定数バッファ系
		UAV_TEXTURE,	// UAVテクスチャ系
		AS,		// 加速構造体
	};


public:

	/*===== メンバ関数 =====*/

	/// <summary>
	/// 事前準備処理
	/// </summary>
	void Setting();

	/// <summary>
	/// ディスクリプタヒープの最後尾にバッファを生成。
	/// </summary>
	/// <param name="Type"> バッファの形式 </param>
	/// <param name="Buffer"> 生成するバッファのデータ </param>
	/// <returns> バッファを生成した位置 </returns>
	RAY_HEAP_HANDLE AllocateBuffer(BUFFER_TYPE Type, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer = nullptr);

	/// <summary>
	/// 指定したハンドルの位置にバッファを生成 主に作成済みのバッファを上書きする形で使用します。 本来このような使い方はよくないと思いますが、仮置きということでこのまま実装。
	/// </summary>
	/// <param name="Handle"> バッファを生成する位置 </param>
	/// <param name="Type"> バッファの形式 </param>
	/// <param name="Buffer"> 生成するバッファのデータ </param>
	void AllocateBufferForHandle(RAY_HEAP_HANDLE Handle, BUFFER_TYPE Type, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer = nullptr);

	/// <summary>
	/// 指定のハンドルのGPUハンドルを取得する。
	/// </summary>
	/// <param name="Handle"> GPUハンドルを取得したいバッファのハンドル </param>
	/// <returns> GPUハンドル </returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(RAY_HEAP_HANDLE Handle);

private:

	/// <summary>
	/// 加速構造体のバッファを生成する。
	/// </summary>
	/// <param name="Handle"> バッファを生成する位置 </param>
	/// <param name="Buffer"> 生成するバッファのデータ </param>
	void AllocateASBuffer(RAY_HEAP_HANDLE Handle, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer);

	/// <summary>
	/// UAVテクスチャ用のバッファを生成する。
	/// </summary>
	/// <param name="Handle"> バッファを生成する位置 </param>
	/// <param name="Buffer"> 生成するバッファのデータ </param>
	void AllocateUAVTextureBuffer(RAY_HEAP_HANDLE Handle, Microsoft::WRL::ComPtr<ID3D12Resource> Buffer);

};