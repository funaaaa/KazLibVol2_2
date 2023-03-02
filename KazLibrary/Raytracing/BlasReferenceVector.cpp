#include "BlasReferenceVector.h"
#include "BLAS.h"

BlasReferenceVector::BlasReferenceVector()
{

	/*===== コンストラクタ =====*/

}

void BlasReferenceVector::Update()
{

	/*===== 更新処理 =====*/

	// 次の描画に向けてBlasの参照を空にする。
	refBlas_.clear();

}

void BlasReferenceVector::AddRef(std::weak_ptr<Blas> RefBlas)
{

	/*===== 参照を追加 =====*/

	// 参照を追加して参照数をインクリメント。
	refBlas_.emplace_back(RefBlas);

}

int BlasReferenceVector::GetBlasRefCount()
{

	/*===== 保存されている参照の数 =====*/

	return static_cast<int>(refBlas_.size());

}

uint8_t* BlasReferenceVector::WriteShaderRecord(uint8_t* Dst, UINT RecordSize, Microsoft::WRL::ComPtr<ID3D12StateObject>& StateObject, LPCWSTR HitGroupName)
{

	/*===== シェーダーレコードを書き込む =====*/

	for (auto& index : refBlas_) {
		Dst = index.lock()->WriteShaderRecord(Dst, RecordSize, StateObject, HitGroupName);
	}

	return Dst;

}
