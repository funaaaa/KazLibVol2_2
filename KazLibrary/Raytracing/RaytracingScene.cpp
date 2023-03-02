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

	/*===== コンストラクタ =====*/

	// ヒープを準備
	RayDescriptorHeap::Instance()->Setting();

	// パイプラインを生成。
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

	/*===== 更新処理 =====*/

	// Instanceコンテナの更新処理。主に配列をclearする。
	InstanceVector::Instance()->Update();

	// Blas参照コンテナの更新処理。主に配列をclearする。
	BlasReferenceVector::Instance()->Update();

}

void RaytracingScene::Draw()
{

	/*===== 描画処理 =====*/

	/*-- ↓ 通常描画 ↓ --*/


	/*-- ↑ 通常描画 ↑ --*/





	/*-- ↓ レイトレ描画 ↓ --*/

	// Tlasを構築。
	Tlas::Instance()->Build();

	// レイトレ用のデータを構築。
	rayPipeline_->BuildShaderTable();

	/*-- ↑ レイトレ描画 ↑ --*/

}

int RaytracingScene::SceneChange()
{

	return 0;

}
