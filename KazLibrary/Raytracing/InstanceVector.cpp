#include "InstanceVector.h"
#include "Blas.h"

void InstanceVector::Update()
{

	/*===== �X�V���� =====*/

	// ���̕`��Ɍ�����Instance����ɂ���B
	instanceDesc_.clear();

}

void InstanceVector::AddInstance(std::weak_ptr<Blas> RefBlas, const DirectX::XMMATRIX& WorldMat)
{

	/*===== Tlas�ɓo�^����Instance��ǉ����� =====*/

	D3D12_RAYTRACING_INSTANCE_DESC instanceDesc;

	// �s���ݒ�B
	XMStoreFloat3x4(
		reinterpret_cast<DirectX::XMFLOAT3X4*>(&instanceDesc.Transform),
		WorldMat);

	// �C���X�^���X�̏ڍׂ�ݒ�B
	instanceDesc.InstanceID = 0;			// ���C�g���ōs�������̃t���O�������Őݒ肷��B�}�e���A�����Őݒ肵�Ă��悢�B
	instanceDesc.InstanceMask = 0xFF;
	instanceDesc.InstanceContributionToHitGroupIndex = static_cast<int>(instanceDesc_.size());
	instanceDesc.Flags = D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
	instanceDesc.AccelerationStructure = RefBlas.lock()->GetBlasBuffer()->GetGPUVirtualAddress();

	// �C���X�^���X��ǉ��B
	instanceDesc_.emplace_back(instanceDesc);

}
