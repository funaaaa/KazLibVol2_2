#pragma once
#include <vector>
#include <memory>
#include <wrl.h>
#include <d3d12.h>
#include "../Helper/ISinglton.h"

class Blas;

// �S�Ă�Blas�̎Q�Ƃ�ۑ����Ă����N���X�B ���C�g���ł͎g�p���邷�ׂĂ�Blas�̃f�[�^�������ŏ������ޕK�v������A���̍ۂɂ��̃N���X����Blas���Q�Ƃ��邽�߂Ɏg�p����B ���t���[���z�����ɂ���B
class BlasReferenceVector : public ISingleton<BlasReferenceVector> {

private:

	/*===== �����o�ϐ� =====*/

	std::vector<std::weak_ptr<Blas>> refBlas_;	// Blas�̎Q��


public:

	/*===== �����o�֐� =====*/

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BlasReferenceVector();

	/// <summary>
	/// �X�V���� ��ɔz�����ɂ���B
	/// </summary>
	void Update();

	/// <summary>
	/// �Q�Ƃ�ǉ��B
	/// </summary>
	/// <param name="RefBlas"> Blas�̎Q�� </param>
	void AddRef(std::weak_ptr<Blas> RefBlas);

	/// <summary>
	/// �ۑ�����Ă���Q�Ƃ̐��B
	/// </summary>
	int GetBlasRefCount();

	/// <summary> 
	/// �Q�ƌ��̃V�F�[�_�[���R�[�h�ɃA�N�Z�X���ď������ށB
	/// </summary>
	/// <param name="Dst"> �������ݐ�|�C���^ </param>
	/// <param name="RecordSize"> �������ރT�C�Y </param>
	/// <param name="StateObject"> �������ރI�u�W�F�N�g </param>
	/// <param name="HitGroupName"> �g�p����HitGroup�̖��O </param>
	/// <returns> �������݌�̃|�C���^ </returns>
	uint8_t* WriteShaderRecord(uint8_t* Dst, UINT RecordSize, Microsoft::WRL::ComPtr<ID3D12StateObject>& StateObject, LPCWSTR HitGroupName);

};
