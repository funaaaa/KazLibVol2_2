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

	/*===== コンストラクタ =====*/

	// ヒープを準備
	//RayDescriptorHeap::Instance()->Setting();

	// パイプラインを生成。
	//rayPipeline_ = std::make_unique<RayPipeline>();

	// プレイヤーのモデルをロード
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

	/*===== 更新処理 =====*/

	// Instanceコンテナの更新処理。主に配列をclearする。
	//InstanceVector::Instance()->Update();

	// Blas参照コンテナの更新処理。主に配列をclearする。
	//BlasReferenceVector::Instance()->Update();

	// カメラの位置を設定。
	CameraMgr::Instance()->Camera(KazMath::Vec3<float>(10, 0, 0), KazMath::Vec3<float>(0, 0, 0), KazMath::Vec3<float>(0, 1, 0));

}

void RaytracingScene::Draw()
{

	/*===== 描画処理 =====*/

	/*-- ↓ 通常描画 ↓ --*/

	// レンダーターゲットをセット。
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);
	
	// プレイヤーを描画
	fbxRender[0].Draw();
	fbxRender[1].Draw();
	fbxRender[2].Draw();

	/*-- ↑ 通常描画 ↑ --*/





	/*-- ↓ レイトレ描画 ↓ --*/

	// Tlasを構築。
	//Tlas::Instance()->Build();

	// レイトレ用のデータを構築。
	//rayPipeline_->BuildShaderTable();

	/*-- ↑ レイトレ描画 ↑ --*/

}

int RaytracingScene::SceneChange()
{

	return 0;

}
