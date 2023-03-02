#include"../Raytracing/RaytracingScene.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Input/ControllerInputManager.h"
#include"../Imgui/MyImgui.h"
#include"../Loader/ObjResourceMgr.h"
#include"../Helper/ResourceFilePass.h"
#include"../Game/Debug/ParameterMgr.h"
#include"../Math/KazMath.h"

#include"../Raytracing/InstanceVector.h"
#include"../Raytracing/Tlas.h"
#include"../Raytracing/RayDescriptorHeap.h"
#include"../Raytracing/RayPipeline.h"
#include"../Raytracing/BlasReferenceVector.h"

RaytracingScene::RaytracingScene()
{

	/*===== �R���X�g���N�^ =====*/

	// �q�[�v������
	RayDescriptorHeap::Instance()->Setting();

	// �p�C�v���C���𐶐��B
	//rayPipeline_ = std::make_unique<RayPipeline>();

}

RaytracingScene::~RaytracingScene()
{
}

void RaytracingScene::Init()
{
}

void RaytracingScene::Finalize()
{
}

void RaytracingScene::Input()
{
}

void RaytracingScene::Update()
{

	/*===== �X�V���� =====*/

	// Instance�R���e�i�̍X�V�����B��ɔz���clear����B
	InstanceVector::Instance()->Update();

	// Blas�Q�ƃR���e�i�̍X�V�����B��ɔz���clear����B
	BlasReferenceVector::Instance()->Update();

}

void RaytracingScene::Draw()
{

	/*===== �`�揈�� =====*/

	/*-- �� �ʏ�`�� �� --*/


	/*-- �� �ʏ�`�� �� --*/





	/*-- �� ���C�g���`�� �� --*/

	// Tlas���\�z�B
	Tlas::Instance()->Build();

	// ���C�g���p�̃f�[�^���\�z�B
	rayPipeline_->BuildShaderTable();

	/*-- �� ���C�g���`�� �� --*/

}

int RaytracingScene::SceneChange()
{

	return 0;

}
