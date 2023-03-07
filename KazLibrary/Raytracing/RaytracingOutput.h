#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "../KazLibrary/Math/KazMath.h"
#include "../Raytracing/RayDescriptorHeap.h"

// レイトレ出力UAVクラス レイトレ関数からの出力結果や、デノイズ後の出力結果の保存用に使用する。
class RaytracingOutput {

private:

	/*===== メンバ変数 =====*/

	Microsoft::WRL::ComPtr<ID3D12Resource> rayTracingOutput_;	// UAV
	RAY_HEAP_HANDLE uavDescriptorIndex_;									// ディスクリプタヒープ上のこのUAVのインデックス


public:

	/*===== メンバ関数 =====*/

	// セッティング
	void Setting(DXGI_FORMAT Format, LPCWSTR BufferName, KazMath::Vec2<int> TextureSize = KazMath::Vec2<int>(1280, 720), D3D12_RESOURCE_STATES ResourceState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE);

	// ディスクリプタをセット。
	void SetComputeRootDescriptorTalbe(int RootParamIndex);

	// リソースバリア
	void SetResourceBarrier(D3D12_RESOURCE_STATES Dst, D3D12_RESOURCE_STATES Src);

	// ゲッタ
	Microsoft::WRL::ComPtr<ID3D12Resource> GetRaytracingOutput() { return rayTracingOutput_; }
	RAY_HEAP_HANDLE GetUAVIndex() { return uavDescriptorIndex_; }

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTexture2D(UINT Width, UINT Height, DXGI_FORMAT Format, D3D12_RESOURCE_FLAGS Flags, D3D12_RESOURCE_STATES IinitialState, D3D12_HEAP_TYPE HeapType);

};