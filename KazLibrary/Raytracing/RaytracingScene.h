#pragma once
#include"../Game/Game.h"
#include"../KazLibrary/Scene/SceneBase.h"
#include"../Render/FbxModelRender.h"
#include <memory>
#include <array>

class RayPipeline;

class RaytracingScene :public SceneBase
{

private:

	/*===== メンバ変数 =====*/

	std::unique_ptr<RayPipeline> rayPipeline_;

	// プレイヤー
	std::array<FbxModelRender, 3> fbxRender;


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