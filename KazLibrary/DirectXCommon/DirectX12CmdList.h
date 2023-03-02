#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"
#include"../DirectXCommon/DirectX12Device.h"

/// <summary>
/// �R�}���h���X�g�̐������A�Ǘ����܂�
/// </summary>
class DirectX12CmdList:public ISingleton<DirectX12CmdList>
{
public:
	DirectX12CmdList();
	~DirectX12CmdList();

	/// <summary>
	/// �R�}���h���X�g�̐���
	/// </summary>
	/// <param name="cmdAllocator">�R�}���h�A���P�[�^�[</param>
	void CreateCmdList(ID3D12CommandAllocator *cmdAllocator);

	/*-- ���C�g���V�K�ǉ��@�\ --*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList4> cmdList;		// AS���\�z���邽�߂̋@�\�����̃R�}���h���X�g�ɂ͑��݂��Ȃ���������4�ɂ��܂����B

private:
	HRESULT result;

	friend ISingleton<DirectX12CmdList>;
};

