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

	/*===== コンストラクタ =====*/

	// ヒープを準備
	RayDescriptorHeap::Instance()->Setting();
	HitGroupMgr::Instance()->Setting();

	// パイプラインを生成。
	pipelineShaders_.push_back({ "Resource/ShaderFiles/RayTracing/RaytracingShader.hlsl", {L"mainRayGen"}, {L"mainMS"}, {L"mainCHS", L"mainAnyHit"} });
	int payloadSize = sizeof(float) * 4;
	rayPipeline_ = std::make_unique<RayPipeline>(pipelineShaders_, HitGroupMgr::DEF, 1, 0, 4, payloadSize, static_cast<int>(sizeof(KazMath::Vec2<float>)), 6);

	// GBufferを生成。
	gBuffer0_ = std::make_shared<RaytracingOutput>();
	gBuffer0_->Setting(DXGI_FORMAT_R32G32B32A32_FLOAT, L"GBuffer0", KazMath::Vec2<int>(1280, 720), D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	gBuffer1_ = std::make_shared<RaytracingOutput>();
	gBuffer1_->Setting(DXGI_FORMAT_R32G32B32A32_FLOAT, L"GBuffer1", KazMath::Vec2<int>(1280, 720), D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	renderUAV_ = std::make_shared<RaytracingOutput>();
	renderUAV_->Setting(DXGI_FORMAT_R32G32B32A32_FLOAT, L"RenderUAV", KazMath::Vec2<int>(1280, 720), D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	output_ = std::make_shared<RaytracingOutput>();
	output_->Setting(DXGI_FORMAT_R16G16B16A16_FLOAT, L"RaytracingOutput", KazMath::Vec2<int>(1280, 720), D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	// プレイヤーのモデルをロード
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
	skydome_.data.transform.scale = { 70.0f,70.0f,70.0f };
	skydome_.data.transform.rotation = { 90.0f,90.0f,0.0f };
	skydome_.data.colorData.color.a = 0;
	skydome_.SetupRaytracing(gBuffer0_, gBuffer1_, renderUAV_);

	sphere_.data.handle = FbxModelResourceMgr::Instance()->LoadModel(KazFilePathName::RaytracingPath + "sphere.fbx");
	sphere_.data.transform.scale = { 5.0f,5.0f,5.0f };
	sphere_.data.transform.rotation = { 90.0f,0.0f,0.0f };
	sphere_.data.colorData.color.a = 255;
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

	/*===== 更新処理 =====*/

	static float rotate = 0.0f;
	rotate += 0.5f;
	sphere_.data.transform.rotation = { 90.0f,rotate,0.0f };

	static float rskydomeRotate = 0.0f;
	rskydomeRotate -= 0.3f;
	skydome_.data.transform.rotation = { 90.0f,rskydomeRotate,0.0f };


	// Instanceコンテナの更新処理。主に配列をclearする。
	InstanceVector::Instance()->Update();

	// Blas参照コンテナの更新処理。主に配列をclearする。
	BlasReferenceVector::Instance()->Update();

	// カメラの位置を設定。
	CameraMgr::Instance()->Camera(KazMath::Vec3<float>(15, 0, 0), KazMath::Vec3<float>(0, 0, 0), KazMath::Vec3<float>(0, 1, 0));

}

void RaytracingScene::Draw()
{

	/*===== 描画処理 =====*/

	/*-- ↓ 通常描画 ↓ --*/

	// レンダーターゲットをセット。
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);

	// 天球を描画
	skydome_.Draw();

	// レイトレ対象の球を描画
	sphere_.Draw();

	/*-- ↑ 通常描画 ↑ --*/





	/*-- ↓ レイトレ描画 ↓ --*/

	// Tlasを構築。
	Tlas::Instance()->Build();

	// レイトレ用のデータを構築。
	rayPipeline_->BuildShaderTable();

	// レイトレ実行
	rayPipeline_->TraceRay(output_, gBuffer0_, gBuffer1_, renderUAV_);

	/*-- ↑ レイトレ描画 ↑ --*/

}

int RaytracingScene::SceneChange()
{

	return 0;

}
