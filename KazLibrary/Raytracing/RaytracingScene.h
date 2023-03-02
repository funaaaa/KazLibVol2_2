#pragma once
#include"../Game/Game.h"
#include"../KazLibrary/Scene/SceneBase.h"
#include <memory>

class RayPipeline;

class RaytracingScene :public SceneBase
{

private:

	/*===== �����o�ϐ� =====*/

	std::unique_ptr<RayPipeline> rayPipeline_;


public:

	/*===== �����o�֐� =====*/

	RaytracingScene();
	~RaytracingScene();

	void Init();
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

};