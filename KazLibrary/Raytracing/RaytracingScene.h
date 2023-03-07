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

	/*===== �����o�ϐ� =====*/

	std::unique_ptr<RayPipeline> rayPipeline_;

	// �v���C���[
	std::array<FbxModelRender, 3> fbxRender;


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