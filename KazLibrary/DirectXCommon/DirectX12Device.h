#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/ISinglton.h"

#include <d3d11.h>
#include <d3d11_1.h>

/// <summary>
/// �f�o�C�X�𐶐��A�Ǘ����܂�
/// </summary>
class DirectX12Device:public ISingleton<DirectX12Device> 
{
public:
	DirectX12Device();
	~DirectX12Device();

	/// <summary>
	/// �f�o�C�X�̐���
	/// </summary>
	/// <param name="adapter">�A�_�v�^�[</param>
	void CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter> adapter);
	Microsoft::WRL::ComPtr<ID3D12Device> dev;
	Microsoft::WRL::ComPtr<ID3D11Device1>dev11;

	/*-- ���C�g���V�K�ǉ��@�\ --*/

	Microsoft::WRL::ComPtr<ID3D12Device5>raytracingDevice;
private:
	HRESULT result;
	friend ISingleton<DirectX12Device>;

};

