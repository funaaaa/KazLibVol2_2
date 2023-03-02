#pragma once
#include <vector>
#include <d3d12.h>
#include <memory>
#include <DirectXMath.h>
#include "../Helper/ISinglton.h"

class Blas;

// Draw�R�[���̍ۂ�Blas�̃p�����[�^�[�����Ƃ�Instance�𐶐����Ĉꎞ�I�ɔz��Ƃ��ĊǗ�����N���X�B
class InstanceVector : public ISingleton<InstanceVector> {

private:

	/*===== �����o�ϐ� =====*/

	std::vector<D3D12_RAYTRACING_INSTANCE_DESC> instanceDesc_;	// Blas��Tlas�ɓo�^���ׂ��ό`�����p�B


public:

	/*===== �����o�֐� =====*/

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	InstanceVector() {};

	/// <summary>
	/// �X�V���� ���vector����ɂ���B
	/// </summary>
	void Update();

	/// <summary>
	/// Tlas�ɓo�^����Instance��ǉ�����B
	/// </summary>
	/// <param name="RefBlas"> Blas�̎Q�� </param>
	/// <param name="WorldMat"> ���[���h�s�� </param>
	void AddInstance(std::weak_ptr<Blas> RefBlas, const DirectX::XMMATRIX& WorldMat);

	/// <summary>
	/// InstanceDessc_�̃T�C�Y��Ԃ��B
	/// </summary>
	/// <returns></returns>
	inline int GetInstanceSize() { return static_cast<int>(instanceDesc_.size()); }

	/// <summary>
	/// Instance�z��̐擪�A�h���X��Ԃ��B
	/// </summary>
	/// <returns></returns>
	inline void* GetData() { return instanceDesc_.data(); }

};