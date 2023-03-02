#pragma once
#include"../DirectXCommon/Base.h"
#include"../Buffer/CreateGpuBuffer.h"
#include"../DirectXCommon/DirectX12Device.h"
#include"../DirectXCommon/DirectX12CmdList.h"
#include"../Pipeline/GraphicsRootSignature.h"
#include"../Pipeline/GraphicsPipeLineMgr.h"
#include"../RenderTarget/RenderTargetStatus.h"
#include"../Helper/KazRenderHelper.h"
#include"../Render/KazRender.h"

/// <summary>
/// �K�E�V�A���u���[�����������摜���쐬���܂�
/// </summary>
class GaussianBuler
{
public:
	/// <summary>
	/// �K�E�V�A���u���[���������摜�����O�������s���܂�
	/// </summary>
	/// <param name="GRAPH_SIZE">��������摜�T�C�Y</param>
	/// <param name="COLOR">�摜�̔w�i�̐F</param>
	GaussianBuler(const KazMath::Vec2<UINT> &GRAPH_SIZE);
	~GaussianBuler();

	/// <summary>
	///  �����_�[�^�[�Q�b�g���ŕ`�悳�ꂽ�I�u�W�F�N�g���ڂ����܂�
	///�@�������_�[�^�[�Q�b�g���ł��̊֐����ĂԎ���CURRENT_RENDERTARGET�Ɍ��݂̃����_�[�^�[�Q�b�g�����Ă�������
	/// </summary>
	/// <param name="RENDERTARGET_HANDLE">�ڂ����Ώۂ��`�悳��Ă��郌���_�[�^�[�Q�b�g</param>
	/// <param name="CURRENT_RENDERTARGET">���݂̃����_�[�^�[�Q�b�g</param>
	RESOURCE_HANDLE BlurImage(RESOURCE_HANDLE RENDERTARGET_HANDLE, RESOURCE_HANDLE CURRENT_RENDERTARGET_HANDLE = -1);

private:
	KazMath::Vec2<UINT> graphSize;

	//�K�E�V�A���u���[��������
	std::array<Sprite2DRender, 2> tex;
	KazRenderHelper::RenderInstancesData renderData;
	RESOURCE_HANDLE cbvHandle;

	CD3DX12_RECT rect;
	CD3DX12_VIEWPORT viewport;


	unique_ptr<CreateGpuBuffer> buffers;



	RESOURCE_HANDLE renderTargetBlurUpHandle;
	RESOURCE_HANDLE renderTargetBlurSideHandle;
};

