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
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;
private:
	HRESULT result;

	friend ISingleton<DirectX12CmdList>;
};

