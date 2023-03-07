#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Helper/DirtyFlag.h"
#include"../Render/RenderData.h"
#include"../Loader/FbxModelResourceMgr.h"
#include<memory>

class Blas;
class RaytracingOutput;

class FbxModelRender :public IRender
{
public:
	FbxModelRender();
	void Draw(bool DRAW_FLAG = true);
	void CaluMat();

	void ReleaseSkining();
	FbxModelData data;
	FbxTime currentTime;

	DirectX::XMMATRIX motherMat;
	DirectX::XMMATRIX motherBoneMat;

	/*-- ���C�g���V�K�ǉ��@�\ --*/

	std::shared_ptr<Blas> blas;	// ���C�g���Ɏg�p����Blas�B�S�Ẵ��f�������̃N���X�����悤�ɂ������̂Ŗ{����IRender�ɏ����ׂ����낤���A���̃N���X�݌v�Ȃ̂ň����₷�������瑤�ɐݒ肷��B
	bool isRaytracingSetUp;		// ���C�g�����Z�b�g�A�b�v�ς݂��𔻒f����t���O�BSetupRaytracing�֐����ĂԂ��Ƃ�true�ɂȂ�Bfalse�̊Ԃ̓��C�g������Ȃ��B
	bool isRayTracingEnabled;	// ���C�g�����L���ɂȂ��Ă��邩�t���O�B���C�g�����Z�b�g�A�b�v�ς݂ł��̃t���O��true�ɂȂ��Ă����Ԃ�Draw���ĂԂ��Ƃ�Tlas�ɓo�^�����B
	std::weak_ptr<RaytracingOutput> refGBuffer0;
	std::weak_ptr<RaytracingOutput> refGBuffer1;

	/// <summary>
	/// ���C�g���[�V���O�̏����֐��B���̊֐����ĂԂ��Ƃł��̃��f���Ń��C�g���[�V���O���L���������B
	/// </summary>
	/// <param name="IsOpaque"> �s�����t���O ���̃��f���͕s�������I���ďꍇ��true �������ɂȂ邩���I���ďꍇ��false�����Ă��������B�y�ʉ��̂��߂ɕK�v�ȕϐ��B </param>
	void SetupRaytracing(std::weak_ptr<RaytracingOutput> GBuffer0, std::weak_ptr<RaytracingOutput> GBuffer1, bool IsOpaque = true);

private:

	UINT vertByte;
	UINT indexByte;

	short vertexBufferHandle;
	short indexBufferHandle;

	std::array<RESOURCE_HANDLE, 2> constBufferHandle;
	std::shared_ptr<FbxResourceData> resourceData;

	FbxTime frameTime;
	FbxTime startTime;
	FbxTime endTime;


	bool removeSkining;
};

