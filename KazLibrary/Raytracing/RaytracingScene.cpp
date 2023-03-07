#include"../Raytracing/RaytracingScene.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Input/ControllerInputManager.h"
#include"../Imgui/MyImgui.h"
#include"../Loader/ObjResourceMgr.h"
#include"../Helper/ResourceFilePass.h"
#include"../Game/Debug/ParameterMgr.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
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
	//RayDescriptorHeap::Instance()->Setting();

	// �p�C�v���C���𐶐��B
	//rayPipeline_ = std::make_unique<RayPipeline>();

	// �v���C���[�̃��f�������[�h
	fbxRender[0].data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::PlayerPath + "CH_Right_Back_Anim.fbx");
	fbxRender[1].data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::PlayerPath + "CH_Left_Back_Anim.fbx");
	fbxRender[2].data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::PlayerPath + "CH_Model_Head.fbx");

	for (int i = 0; i < fbxRender.size(); ++i)
	{
		fbxRender[i].data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO;
		fbxRender[i].data.stopAnimationFlag = true;
		fbxRender[i].data.transform.scale = { 1.0f,1.0f,1.0f };
		fbxRender[i].data.transform.pos = { 0.0f,0.0f,0.0f };
	}

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
	//InstanceVector::Instance()->Update();

	// Blas�Q�ƃR���e�i�̍X�V�����B��ɔz���clear����B
	//BlasReferenceVector::Instance()->Update();

	// �J�����̈ʒu��ݒ�B
	CameraMgr::Instance()->Camera(KazMath::Vec3<float>(10, 0, 0), KazMath::Vec3<float>(0, 0, 0), KazMath::Vec3<float>(0, 1, 0));

}

void RaytracingScene::Draw()
{

	/*===== �`�揈�� =====*/

	/*-- �� �ʏ�`�� �� --*/

	// �����_�[�^�[�Q�b�g���Z�b�g�B
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);
	
	// �v���C���[��`��
	fbxRender[0].Draw();
	fbxRender[1].Draw();
	fbxRender[2].Draw();

	/*-- �� �ʏ�`�� �� --*/





	/*-- �� ���C�g���`�� �� --*/

	// Tlas���\�z�B
	//Tlas::Instance()->Build();

	// ���C�g���p�̃f�[�^���\�z�B
	//rayPipeline_->BuildShaderTable();

	/*-- �� ���C�g���`�� �� --*/

}

int RaytracingScene::SceneChange()
{

	return 0;

}
