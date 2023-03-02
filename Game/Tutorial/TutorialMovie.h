#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../Tutorial/DirectX12MoviePlayer.h"
#include"../GameRenderTarget.h"
#include"../Game/Event/TutorialGame.h"
#include"../Game/Tutorial/Gauge.h"

/// <summary>
/// �Q�[�����ŗ����`���[�g���A��
/// </summary>
class TutorialMovie
{
public:
	TutorialMovie(bool STOP_MOVIE_FLAG);
	~TutorialMovie();
	void Init(const std::string &TEXT,int MAX_ACHIEVEMENT_ITEMS);
	void Update();
	void Draw();

	void Play();
	void Noise();
	void Stop();

	void Achieved();
	bool End();

	void Succeed()
	{
		tutorialText.Succeed();
	}
	bool NextTutorial()
	{
		return tutorialText.NextTutorial();
	}
	RESOURCE_HANDLE GetTexture()
	{
		return outputRenderTargetHandle;
	};
private:
	//����Đ�--------------------------------------------
	bool startMovieFlag;
	bool startNoiseFlag;
	bool stopFlag;
	std::unique_ptr<DirectX12MoviePlayer> moviePlayer;
	RESOURCE_HANDLE vhsSeedHandle, noiseSeedHandle;
	std::unique_ptr<MovieRender> movieRender;
	float seedNum;
	//����Đ�--------------------------------------------

	//�m�C�Y�Đ�--------------------------------------------
	Sprite2DRender normalRender;
	//�m�C�Y�Đ�--------------------------------------------

	//UI--------------------------------------------
	TutorialGame tutorialText;//����UI
	Gauge gauge;
	const int ADD_NUM;
	//UI--------------------------------------------

	RESOURCE_HANDLE renderTargetHandle;
	RESOURCE_HANDLE outputRenderTargetHandle;
	Sprite2DRender outputRender;

};