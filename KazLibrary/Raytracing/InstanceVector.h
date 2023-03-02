#pragma once
#include <vector>
#include <d3d12.h>
#include <memory>
#include <DirectXMath.h>
#include "../Helper/ISinglton.h"

class Blas;

// Drawコールの際にBlasのパラメーターをもとにInstanceを生成して一時的に配列として管理するクラス。
class InstanceVector : public ISingleton<InstanceVector> {

private:

	/*===== メンバ変数 =====*/

	std::vector<D3D12_RAYTRACING_INSTANCE_DESC> instanceDesc_;	// BlasをTlasに登録すべく変形した姿。


public:

	/*===== メンバ関数 =====*/

	/// <summary>
	/// コンストラクタ
	/// </summary>
	InstanceVector() {};

	/// <summary>
	/// 更新処理 主にvectorを空にする。
	/// </summary>
	void Update();

	/// <summary>
	/// Tlasに登録するInstanceを追加する。
	/// </summary>
	/// <param name="RefBlas"> Blasの参照 </param>
	/// <param name="WorldMat"> ワールド行列 </param>
	void AddInstance(std::weak_ptr<Blas> RefBlas, const DirectX::XMMATRIX& WorldMat);

	/// <summary>
	/// InstanceDessc_のサイズを返す。
	/// </summary>
	/// <returns></returns>
	inline int GetInstanceSize() { return static_cast<int>(instanceDesc_.size()); }

	/// <summary>
	/// Instance配列の先頭アドレスを返す。
	/// </summary>
	/// <returns></returns>
	inline void* GetData() { return instanceDesc_.data(); }

};