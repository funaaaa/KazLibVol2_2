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

	/*===== �����o�ϐ� =====*/

	// ���C�g���p�p�C�v���C��
	std::vector<RayPipelineShaderData> pipelineShaders_;
	std::unique_ptr<RayPipeline> rayPipeline_;

	// GBuffer
	std::shared_ptr<RaytracingOutput> gBuffer0_;
	std::shared_ptr<RaytracingOutput> gBuffer1_;
	std::shared_ptr<RaytracingOutput> renderUAV_;

	// �o�͗p
	std::shared_ptr<RaytracingOutput> output_;

	// �v���C���[
	std::array<FbxModelRender, 3> fbxRender;

	// �V��
	FbxModelRender skydome_;
	FbxModelRender sphere_;


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