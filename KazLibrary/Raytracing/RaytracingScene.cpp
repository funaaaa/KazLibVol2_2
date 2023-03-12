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
#include"../Raytracing/RaytracingOutput.h"
#include"../Raytracing/HitGroupMgr.h"

RaytracingScene::RaytracingScene()
{

	/*===== �R���X�g���N�^ =====*/

	// �q�[�v������
	RayDescriptorHeap::Instance()->Setting();
	HitGroupMgr::Instance()->Setting();

	// �p�C�v���C���𐶐��B
	pipelineShaders_.push_back({ "Resource/ShaderFiles/RayTracing/RaytracingShader.hlsl", {L"mainRayGen"}, {L"mainMS"}, {L"mainCHS", L"mainAnyHit"} });
	int payloadSize = sizeof(float) * 4;
	rayPipeline_ = std::make_unique<RayPipeline>(pipelineShaders_, HitGroupMgr::DEF, 1, 0, 4, payloadSize, static_cast<int>(sizeof(KazMath::Vec2<float>)), 6);

	// GBuffer�𐶐��B
	gBuffer0_ = std::make_shared<RaytracingOutput>();
	gBuffer0_->Setting(DXGI_FORMAT_R32G32B32A32_FLOAT, L"GBuffer0");
	gBuffer1_ = std::make_shared<RaytracingOutput>();
	gBuffer1_->Setting(DXGI_FORMAT_R32G32B32A32_FLOAT, L"GBuffer1");
	renderUAV_ = std::make_shared<RaytracingOutput>();
	renderUAV_->Setting(DXGI_FORMAT_R32G32B32A32_FLOAT, L"RenderUAV");
	output_ = std::make_shared<RaytracingOutput>();
	output_->Setting(DXGI_FORMAT_R8G8B8A8_UNORM, L"RaytracingOutput");

	// �v���C���[�̃��f�������[�h
	//fbxRender[0].data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::PlayerPath + "CH_Right_Back_Anim.fbx");
	//fbxRender[1].data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::PlayerPath + "CH_Left_Back_Anim.fbx");
	//fbxRender[2].data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::PlayerPath + "CH_Model_Head.fbx");

	//for (auto& index : fbxRender)
	//{
	//	index.data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO;
	//	index.data.stopAnimationFlag = true;
	//	index.data.transform.scale = { 1.0f,1.0f,1.0f };
	//	index.data.transform.pos = { 0.0f,0.0f,0.0f };
	//	index.SetupRaytracing(gBuffer0_, gBuffer1_);
	//}

	skydome_.data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::RaytracingPath + "skydome.fbx");
	skydome_.data.transform.scale = { 50.0f,50.0f,50.0f };
	skydome_.data.transform.rotation = { 90.0f,90.0f,0.0f };
	skydome_.SetupRaytracing(gBuffer0_, gBuffer1_, renderUAV_);
	skydome_.data.pipelineName = PIPELINE_NAME_FBX_RENDERTARGET_TWO_RENDERUAV;

	sphere_.data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::RaytracingPath + "sphere.fbx");
	sphere_.data.transform.scale = { 5.0f,5.0f,5.0f };
	sphere_.data.transform.rotation = { 0.0f,0.0f,0.0f };
	sphere_.SetupRaytracing(gBuffer0_, gBuffer1_, renderUAV_);

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

	//// UAV��������
	//CD3DX12_RANGE readRange(0, 0);
	//UINT8* pTextureDataBegin = nullptr;
	//gBuffer0_->GetRaytracingOutput()->Map(0, &readRange, reinterpret_cast<void**>(&pTextureDataBegin));
	//memset(pTextureDataBegin, 0, 1280 * 720 * 16);
	//gBuffer0_->GetRaytracingOutput()->Unmap(0, nullptr);

	//readRange = CD3DX12_RANGE(0, 0);
	//pTextureDataBegin = nullptr;
	//gBuffer1_->GetRaytracingOutput()->Map(0, &readRange, reinterpret_cast<void**>(&pTextureDataBegin));
	//memset(pTextureDataBegin, 0, 1280 * 720 * 16);
	//gBuffer1_->GetRaytracingOutput()->Unmap(0, nullptr);

	//readRange = CD3DX12_RANGE(0, 0);
	//pTextureDataBegin = nullptr;
	//renderUAV_->GetRaytracingOutput()->Map(0, &readRange, reinterpret_cast<void**>(&pTextureDataBegin));
	//memset(pTextureDataBegin, 0, 1280 * 720 * 16);
	//renderUAV_->GetRaytracingOutput()->Unmap(0, nullptr);

	//readRange = CD3DX12_RANGE(0, 0);
	//pTextureDataBegin = nullptr;
	//output_->GetRaytracingOutput()->Map(0, &readRange, reinterpret_cast<void**>(&pTextureDataBegin));
	//memset(pTextureDataBegin, 0, 1280 * 720 * 4);
	//output_->GetRaytracingOutput()->Unmap(0, nullptr);

	static float rotate = 0.0f;
	rotate += 1.0f;
	sphere_.data.transform.rotation = { 0.0f,rotate,0.0f };


	// Instance�R���e�i�̍X�V�����B��ɔz���clear����B
	InstanceVector::Instance()->Update();

	// Blas�Q�ƃR���e�i�̍X�V�����B��ɔz���clear����B
	BlasReferenceVector::Instance()->Update();

	// �J�����̈ʒu��ݒ�B
	CameraMgr::Instance()->Camera(KazMath::Vec3<float>(15, 0, 0), KazMath::Vec3<float>(0, 0, 0), KazMath::Vec3<float>(0, 1, 0));

}

void RaytracingScene::Draw()
{

	/*===== �`�揈�� =====*/

	/*-- �� �ʏ�`�� �� --*/

	// �����_�[�^�[�Q�b�g���Z�b�g�B
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);

	// �V����`��
	skydome_.Draw();
	
	// ���C�g���Ώۂ̋���`��
	sphere_.Draw();

	/*-- �� �ʏ�`�� �� --*/





	/*-- �� ���C�g���`�� �� --*/

	// Tlas���\�z�B
	Tlas::Instance()->Build();

	// ���C�g���p�̃f�[�^���\�z�B
	rayPipeline_->BuildShaderTable();

	// ���C�g�����s
	rayPipeline_->TraceRay(output_, gBuffer0_, gBuffer1_, renderUAV_);

	/*-- �� ���C�g���`�� �� --*/

}

int RaytracingScene::SceneChange()
{

	return 0;

}
