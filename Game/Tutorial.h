#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../Game/Stage/DrawGrid.h"
#include"../KazLibrary/RenderTarget/GaussianBuler.h"
#include"../KazLibrary/Helper/Select.h"
#include"../Game/UI/DrawMenu.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Player.h"
#include"../Game/UI/AttackLog.h"
#include"../Game/Event/WindowFlame.h"
#include"../Game/UI/Cursor.h"
#include"../Game/Helper/CameraWork.h"
#include"../Game/LineEffect/LineLevel1.h"
#include"../Game/Event/TutorialGame.h"
#include"../Game/Effect/PortalIntermediateDirection.h"
#include"../Game/GameRenderTarget.h"
#include"../Game/Tutorial/DirectX12MoviePlayer.h"

#include"../Game/Tutorial/TutorialMovie.h"
#include"../Game/Tutorial/OldComputer.h"

/// <summary>
/// �Q�[���J�n�O�̃`���[�g���A���p�̃X�e�[�W
/// </summary>
class Tutorial
{
public:
	Tutorial();
	~Tutorial();

	void Init(bool SKIP_FLAG);
	void Finalize();
	void Input();
	void Update();
	void Draw();

	bool tutorialFlag;
	RESOURCE_HANDLE handle;

	Sprite2DRender mainRenderTarget;

	Cursor cursor;
private:
	int sceneNum;

	int cameraIndex;
	KazMath::Vec3<float> debugCameraMove;
	KazMath::Vec3<float> eyePos, targetPos;
	KazMath::Vec2<float> angle;

	std::array<std::unique_ptr<DrawGrid>, 2> gridR;

	bool cameraFlag;
	Player player;
	CameraWork cameraWork;
	const float LOG_FONT_SIZE;
	AttackLog stringLog;

	//�����o----------------------------------------------------------------
	static const int LINE_MAX_ARRAY_NUM = 16;
	std::array<LineLevel1, LINE_MAX_ARRAY_NUM> lineLevel;			//���p�ɓG�ɐ���L�΂��Ă������o
	std::array<LineEffectData, LINE_MAX_ARRAY_NUM>lineEffectArrayData;
	KazMath::Vec3<float> testEnemyPos;			//���b�N�I������Ώۍ��W(�e�X�g)
	//�����o----------------------------------------------------------------

	static const int TUTORIAL_MAX_NUM = 1;

	std::array<std::array<std::unique_ptr<IEnemy>, 10>, TUTORIAL_MAX_NUM> enemies;
	RESOURCE_HANDLE damageSoundHandle, lockSoundHandle;

	//�`���[�g���A���p----------------------------------------------------------------
	bool startFlag;
	std::array<bool, 3>initEnemyFlagArray;
	int tutorialArrayIndex;
	bool tutorialAllClearFlag;
	bool initEffectFlag;
	int noiseTimer;
	//�`���[�g���A���p----------------------------------------------------------------

	OldComputer pc;



	std::unique_ptr<GameRenderTarget> renderTarget;
	PortalIntermediateDirection portalEffect;
	bool initSceneFlag;

	float angleC;

	std::array<std::unique_ptr<TutorialMovie>, TUTORIAL_MAX_NUM> tutorialMovieArray;
	std::array<std::array<KazMath::Vec3<float>, 8>, TUTORIAL_MAX_NUM>tutorialPosArray;
	bool playFlag;
	bool stopFlag;
	bool noiseFlag;

	int startTime;
	bool deadAllEnemyFlag;

	KazMath::Transform3D pcTransform;

};