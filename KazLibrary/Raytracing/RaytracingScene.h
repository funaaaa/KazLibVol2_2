#pragma once
#include"../Game/Game.h"
#include"../KazLibrary/Scene/SceneBase.h"
#include <memory>

class RayPipeline;

class RaytracingScene :public SceneBase
{

private:

	/*===== メンバ変数 =====*/

	std::unique_ptr<RayPipeline> rayPipeline_;


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