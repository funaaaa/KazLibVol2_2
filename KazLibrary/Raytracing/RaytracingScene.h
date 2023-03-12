#pragma once
#include"../Game/Game.h"
#include"../KazLibrary/Scene/SceneBase.h"
#include"../Render/FbxModelRender.h"
#include"../Raytracing/RayPipeline.h"
#include <memory>
#include <array>

class RayPipeline;
class RaytracingOutput;

class RaytracingScene :public SceneBase
{

private:

	/*===== メンバ変数 =====*/

	// レイトレ用パイプライン
	std::vector<RayPipelineShaderData> pipelineShaders_;
	std::unique_ptr<RayPipeline> rayPipeline_;

	// GBuffer
	std::shared_ptr<RaytracingOutput> gBuffer0_;
	std::shared_ptr<RaytracingOutput> gBuffer1_;
	std::shared_ptr<RaytracingOutput> renderUAV_;

	// 出力用
	std::shared_ptr<RaytracingOutput> output_;

	// プレイヤー
	std::array<FbxModelRender, 3> fbxRender;

	// 天球
	FbxModelRender skydome_;
	FbxModelRender sphere_;


public:

	/*===== メンバ関数 =====*/

	RaytracingScene();
	~RaytracingScene();

	void Init();
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

};