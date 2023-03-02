#include "HitGroup.h"
#include "RayRootsignature.h"

HitGroup::HitGroup()
{
}

void HitGroup::Generate(const HitGroupInitData& InputData, int RegisterSpace, const LPCWSTR& HitGroupName)
{

	/*===== �������� =====*/

	// ���͂��ꂽ�f�[�^�̒l��ۑ�����B
	CH_ = InputData.CH_;
	AH_ = InputData.AH_;
	IS_ = InputData.IS_;
	SRVcount_ = InputData.SRVcount_;
	CBVcount_ = InputData.CBVcount_;
	UAVcount_ = InputData.UAVcount_;
	registerSpace_ = RegisterSpace;
	hitGroupName_ = HitGroupName;

	// ���[�J�����[�g�V�O�l�`���𐶐�����B
	localRootSig_ = std::make_shared<RayRootsignature>();

	// SRV�ɂ��Ēǉ��B
	for (int index = 0; index < SRVcount_; ++index) {

		localRootSig_->AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, index, registerSpace_);

	}
	// UAV�ɂ��Ēǉ��B
	for (int index = 0; index < UAVcount_; ++index) {

		localRootSig_->AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, index, registerSpace_);

	}
	// CBV�ɂ��Ēǉ��B
	for (int index = 0; index < CBVcount_; ++index) {

		localRootSig_->AddRootparam(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, index, registerSpace_);

	}
	// �T���v���[��ǉ��B
	localRootSig_->AddStaticSampler(registerSpace_);

	// ���[�J�����[�g�V�O�l�`���𐶐��B
	localRootSig_->Build(true, hitGroupName_);

}
