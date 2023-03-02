#pragma once
#include <string>
#include <memory>
#include <wtypes.h>

class RayRootsignature;

/// <summary>
/// HitGroup�𐶐�����ۂɓn���f�[�^�\����
/// </summary>
struct EntryPoint {
	LPCWSTR entryPoint_;
	bool isActive_;
	EntryPoint() {};
	EntryPoint(LPCWSTR Entry, bool Flag) :entryPoint_(Entry), isActive_(Flag) {};
};
struct HitGroupInitData {

	EntryPoint CH_;		// ClosestHitShader
	EntryPoint AH_;		// AnyHitShader
	EntryPoint IS_;		// IntersectShader
	int SRVcount_;		// SRV�̐�
	int CBVcount_;		// CBV�̐�
	int UAVcount_;		// UAV�̐�
	HitGroupInitData() {};
};

/// <summary>
/// �q�b�g�O���[�v�N���X
/// </summary>
class HitGroup {

private:

	/*===== �����o�ϐ� =====*/

	EntryPoint CH_;		// ClosestHitShader
	EntryPoint AH_;		// AnyHitShader
	EntryPoint IS_;		// IntersectShader
	int SRVcount_;		// SRV�̐�
	int CBVcount_;		// CBV�̐�
	int UAVcount_;		// UAV�̐�

	int registerSpace_;	// �V�F�[�_�[���W�X�^�[�̃��W�X�^�[�X�y�[�X�ԍ�

	LPCWSTR hitGroupName_;	// ���̃q�b�g�O���[�v�̖��O

	std::shared_ptr<RayRootsignature> localRootSig_;	// ���̃q�b�g�O���[�v�Ŏg�p���郍�[�J�����[�g�V�O�l�`��


public:

	/*===== �����o�֐� =====*/

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	HitGroup();

	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="InputData"> HitGroup�̏������p�f�[�^ </param>
	/// <param name="RegisterSpace"> ���W�X�^�X�y�[�X </param>
	/// <param name="HitGroupName"> ����HitGroup�̖��O�̕����� </param>
	void Generate(const HitGroupInitData& InputData, int RegisterSpace, const LPCWSTR& HitGroupName);

	// �e��Q�b�^
	const LPCWSTR& GetCH() { return CH_.entryPoint_; }
	const LPCWSTR& GetAH() { return AH_.entryPoint_; }
	const LPCWSTR& GetIS() { return IS_.entryPoint_; }
	bool GetCHFlag() { return CH_.isActive_; }
	bool GetAHFlag() { return AH_.isActive_; }
	bool GetISFlag() { return IS_.isActive_; }
	int GetSRVCount() { return SRVcount_; }
	int GetCBVCount() { return CBVcount_; }
	int GetUAVCount() { return UAVcount_; }
	const std::shared_ptr<RayRootsignature> GetLoacalRootSig() {return localRootSig_; };

};