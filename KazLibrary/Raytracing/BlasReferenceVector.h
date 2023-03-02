#pragma once
#include <vector>
#include <memory>
#include <wrl.h>
#include <d3d12.h>
#include "../Helper/ISinglton.h"

class Blas;

// 全てのBlasの参照を保存しておくクラス。 レイトレでは使用するすべてのBlasのデータを自分で書き込む必要があり、その際にこのクラスからBlasを参照するために使用する。 毎フレーム配列を空にする。
class BlasReferenceVector : public ISingleton<BlasReferenceVector> {

private:

	/*===== メンバ変数 =====*/

	std::vector<std::weak_ptr<Blas>> refBlas_;	// Blasの参照


public:

	/*===== メンバ関数 =====*/

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BlasReferenceVector();

	/// <summary>
	/// 更新処理 主に配列を空にする。
	/// </summary>
	void Update();

	/// <summary>
	/// 参照を追加。
	/// </summary>
	/// <param name="RefBlas"> Blasの参照 </param>
	void AddRef(std::weak_ptr<Blas> RefBlas);

	/// <summary>
	/// 保存されている参照の数。
	/// </summary>
	int GetBlasRefCount();

	/// <summary> 
	/// 参照元のシェーダーレコードにアクセスして書き込む。
	/// </summary>
	/// <param name="Dst"> 書き込み先ポインタ </param>
	/// <param name="RecordSize"> 書き込むサイズ </param>
	/// <param name="StateObject"> 書き込むオブジェクト </param>
	/// <param name="HitGroupName"> 使用するHitGroupの名前 </param>
	/// <returns> 書き込み後のポインタ </returns>
	uint8_t* WriteShaderRecord(uint8_t* Dst, UINT RecordSize, Microsoft::WRL::ComPtr<ID3D12StateObject>& StateObject, LPCWSTR HitGroupName);

};
