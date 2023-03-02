#pragma once
#include"../Game/Game.h"
#include"../KazLibrary/Scene/SceneBase.h"
#include <memory>

class RayPipeline;

class RaytracingScene :public SceneBase
{

private:

	/*===== ƒƒ“ƒo•Ï” =====*/

	std::unique_ptr<RayPipeline> rayPipeline_;


public:

	/*===== ƒƒ“ƒoŠÖ” =====*/

	RaytracingScene();
	~RaytracingScene();

	void Init();
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

};