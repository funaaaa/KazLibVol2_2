#include "BlasReferenceVector.h"
#include "BLAS.h"

BlasReferenceVector::BlasReferenceVector()
{

	/*===== �R���X�g���N�^ =====*/

}

void BlasReferenceVector::Update()
{

	/*===== �X�V���� =====*/

	// ���̕`��Ɍ�����Blas�̎Q�Ƃ���ɂ���B
	refBlas_.clear();

}

void BlasReferenceVector::AddRef(std::weak_ptr<Blas> RefBlas)
{

	/*===== �Q�Ƃ�ǉ� =====*/

	// �Q�Ƃ�ǉ����ĎQ�Ɛ����C���N�������g�B
	refBlas_.emplace_back(RefBlas);

}

int BlasReferenceVector::GetBlasRefCount()
{

	/*===== �ۑ�����Ă���Q�Ƃ̐� =====*/

	return static_cast<int>(refBlas_.size());

}

uint8_t* BlasReferenceVector::WriteShaderRecord(uint8_t* Dst, UINT RecordSize, Microsoft::WRL::ComPtr<ID3D12StateObject>& StateObject, LPCWSTR HitGroupName)
{

	/*===== �V�F�[�_�[���R�[�h���������� =====*/

	for (auto& index : refBlas_) {
		Dst = index.lock()->WriteShaderRecord(Dst, RecordSize, StateObject, HitGroupName);
	}

	return Dst;

}
