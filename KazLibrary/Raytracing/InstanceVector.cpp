#include "InstanceVector.h"
#include "Blas.h"

void InstanceVector::Update()
{

	/*===== 更新処理 =====*/

	// 次の描画に向けてInstanceを空にする。
	instanceDesc_.clear();

}

void InstanceVector::AddInstance(std::weak_ptr<Blas> RefBlas, const DirectX::XMMATRIX& WorldMat)
{

	/*===== Tlasに登録するInstanceを追加する =====*/

	D3D12_RAYTRACING_INSTANCE_DESC instanceDesc;

	// 行列を設定。
	XMStoreFloat3x4(
		reinterpret_cast<DirectX::XMFLOAT3X4*>(&instanceDesc.Transform),
		WorldMat);

	// インスタンスの詳細を設定。
	instanceDesc.InstanceID = 0;			// レイトレで行う処理のフラグをここで設定する。マテリアル側で設定してもよい。
	instanceDesc.InstanceMask = 0xFF;
	instanceDesc.InstanceContributionToHitGroupIndex = static_cast<int>(instanceDesc_.size());
	instanceDesc.Flags = D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
	instanceDesc.AccelerationStructure = RefBlas.lock()->GetBlasBuffer()->GetGPUVirtualAddress();

	// インスタンスを追加。
	instanceDesc_.emplace_back(instanceDesc);

}
