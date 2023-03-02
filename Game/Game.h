#pragma once
#include"../KazLibrary/Render/BackGroundForDebug.h"
#include"../KazLibrary/Render/KazRender.h"
#include<memory>
#include"../Game/Player.h"
#include"../Game/UI/Cursor.h"
#include"../Game/LineEffect/LineLevel1.h"
#include"../Game/Event/GoalBox.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/RenderTarget/GaussianBuler.h"
#include"../KazLibrary/Render/PolygonRender.h"
#include"../KazLibrary/Scene/SceneBase.h"
#include"../Game/Interface/IStage.h"
#include"../Game/Stage/FirstStage.h"
#include"../Game/Stage/SecondStage.h"
#include"../Game/Stage/ThridStage.h"
#include"../Game/Stage/RezStage.h"
#include"../Game/Stage/BlockParticleStage.h"
#include"../Game/Stage/GpuParticleStage.h"

#include"../Game/UI/AnnounceStageUI.h"
#include"../Game/Event/EventSprite.h"
#include"../Game/Event/TutorialWindow.h"
#include"../Game/Enemy/HitEnemyEffect.h"
#include"../Game/Event/PortalEffect.h"
#include"../Game/Effect/IHitEffectEmitter.h"
#include"../Game/Effect/HitEffectPattern1Emitter.h"
#include"../Game/Effect/HitEffectPattern2Emitter.h"
#include"../Game/Effect/HitEffectPattern3Emitter.h"
#include"../Game/Helper/EnemyHelper.h"

#include"../Game/Effect/FireEffect.h"
#include"../Game/Effect/RocketLightEffect.h"
#include"../Game/Effect/MeshParticleEmitter.h"
#include"../Game/Effect/DeadParticle.h"

#include"../Game/UI/AttackLog.h"
#include"../Game/Helper/CameraWork.h"

#include"Tutorial.h"
#include"../Game/UI/StringWindow.h"

#include"../Game/CollisionDetection/InstanceMeshCollision.h"
#include"../Game/Effect/InstanceDeadParticle.h"


class Game
{

public:
	//�萔--------------------------


	Game(const std::array<std::array<ResponeData, KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> &RESPONE_DATA,
		const std::array<std::shared_ptr<IStage>, KazEnemyHelper::STAGE_NUM_MAX> &STAGE_ARRAY,
		const std::array<KazMath::Color, KazEnemyHelper::STAGE_NUM_MAX> &BACKGROUND_COLOR,
		const std::array<std::array<KazEnemyHelper::ForceCameraData, 10>, KazEnemyHelper::STAGE_NUM_MAX> &CAMERA_ARRAY);
	~Game();
	void Init(
		bool SKIP_FLAG
	);
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

private:

	const float LOG_FONT_SIZE;
	AttackLog stringLog;


	//�Q�[�����[�v----------------------------------------------------------------
	bool gameStartFlag;						//�Q�[���J�n��m�点��t���O
	int sceneNum;							//�������̃V�[���ɔ�Ԃ�
	bool sceneChangeFlag;
	int gameFlame;							//1�X�e�[�W�̌o�ߎ���
	int gameSpeed;							//1�X�e�[�W�Ői�ގ��Ԃ̃X�s�[�h
	int gameStageLevel;						//���݂̃X�e�[�W�̃��x��
	int gameLeyerLevel;						//���݂̃X�e�[�W�̃��x��
	std::array<int, 10>changeLayerLevelMaxTime;	//���̃��C���[���x���ɐ؂�ւ��t���[������ۊǂ���
	//�Q�[�����[�v----------------------------------------------------------------

	//�f�o�b�N�p-------------------------------------------------------------
	BackGroundForDebug bg;
	//�f�o�b�N�p-------------------------------------------------------------


	//�J����----------------------------------------------------------------
	KazMath::Vec3<float> eyePos;
	KazMath::Vec3<float> targetPos;
	std::array<std::array<KazEnemyHelper::ForceCameraData, 10>, KazEnemyHelper::STAGE_NUM_MAX>cameraMoveArray;
	float cameraAngle;
	CameraWork cameraWork;
	//�J����----------------------------------------------------------------

	//�v���C���[�����삷�����----------------------------------------------------------------
	Player player;
	Cursor cursor;
	//�v���C���[�����삷�����----------------------------------------------------------------

	//�G----------------------------------------------------------------
	std::array<unique_ptr<IEnemy>, 2>enemy;					//�G(�T���v��)
	std::array<std::array<unique_ptr<IEnemy>, KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> enemies;	//1�X�e�[�W�ɐ�������G�̑���
	std::array<int, 10> enemiesHandle;						//0���珇�Ԃɏ���������ۂɕK�v
	std::array<int, 10> addEnemiesHandle;					//0���珇�Ԃɒǉ��ŏ���������ۂɕK�v
	std::array<std::array<ResponeData, KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> responeData;		//�G�𐶐�����ۂɕK�v�Ȑݒ�
	std::array<ResponeData, 50>addResponeData;				//�G��ǉ��Ő�������ۂɕK�v�Ȑݒ���X�^�b�N��������
	bool isEnemyNotMoveFlag;
	int notMoveTimer;
	const int CHANGE_GMAE_FLAME_SPEED_MAX_TIME = 2;	//�G�����Ȃ��Ȃ��Ă���Q�[�������Ԃ̐i�ރX�s�[�h�����܂�܂ł̊Ԋu

#ifdef _DEBUG
	std::array<std::array<BoxPolygonRender, KazEnemyHelper::ENEMY_NUM_MAX>, KazEnemyHelper::LAYER_LEVEL_MAX> enemyHitBox;
#endif
	//�G----------------------------------------------------------------

	//�����o----------------------------------------------------------------
	static const int LINE_MAX_ARRAY_NUM = 16;
	std::array<LineLevel1, LINE_MAX_ARRAY_NUM> lineLevel;			//���p�ɓG�ɐ���L�΂��Ă������o
	std::array<LineEffectData, LINE_MAX_ARRAY_NUM>lineEffectArrayData;
	KazMath::Vec3<float> testEnemyPos;			//���b�N�I������Ώۍ��W(�e�X�g)
	//�����o----------------------------------------------------------------

	//�U�����o-------------------------
	std::array<HitEnemyEffect, 30> hitEffect;
	//�U�����o-------------------------

	ObjModelRenderPtr model;

	GoalBox goalBox;
	std::array<KazMath::Vec3<float>, KazEnemyHelper::STAGE_NUM_MAX> appearGoalBoxPos;
	KazMath::Vec3<float> responeGoalBoxPos;
	bool initAppearFlag;
	bool changeStageFlag;

	//��ʌ��ʏ���-----------------------

	Sprite2DRender mainRenderTarget;
	std::array<std::unique_ptr<GameRenderTarget>, KazEnemyHelper::STAGE_NUM_MAX> renderTarget;
	std::array<std::unique_ptr<GameRenderTarget>, KazEnemyHelper::STAGE_NUM_MAX> nextRenderTarget;

	int stageNum;
	std::array<std::shared_ptr<IStage>, KazEnemyHelper::STAGE_NUM_MAX>stages;
	bool cameraModeChangeFlag;
	bool lineDebugFlag;



	std::array<BoxPolygonRender, 8> lineStartPoly;


	//�Q�[���I�[�o�[���----------------------------------------------
	Sprite2DRender gameOverTex;
	Sprite2DRender pressAButtonTex;
	Sprite2DRender blackTex;
	bool gameOverFlag;
	bool readyToBlackOutFlag;
	bool readyToBlackOutToGoTitleFlag;
	int flashTimer;
	bool flashFlag;
	bool isGameOverFlag;

	RESOURCE_HANDLE gameClearResourceHandle, gameOverResouceHandle;
	//�Q�[���I�[�o�[���----------------------------------------------


	//�Q�[���N���A���--------------------------------------------
	bool isGameClearFlag;
	bool gameClearFlag;
	//�Q�[���N���A���--------------------------------------------

	//UI--------------------------------------
	AnnounceStageUI stageUI;


	RESOURCE_HANDLE bgmSoundHandle, lockSoundHandle;
	RESOURCE_HANDLE doneSoundHandle;
	RESOURCE_HANDLE damageSoundHandle;

	TutorialWindow tutorialWindow;

	KazMath::Vec3<float>tPos;

	CircleRender circle;

	bool smokeFlag;
	std::array<Sprite2DRender, 4> smokeR;

	int emittNum;

	//�G�t�F�N�g--------------------------------------
	std::array<FireEffect, 30>fireEffect;
	std::array<RocketLightEffect, 30>lightEffect;
	int rocketIndex;
	int fireIndex;

	//�G�t�F�N�g--------------------------------------

	BoxPolygonRender box;

	std::unique_ptr<Tutorial> tutorial;

	PortalIntermediateDirection portalEffect;
	//std::unique_ptr<MeshParticleEmitter> meshEmitter;


	FbxModelRender playerModel, headModel;


	bool d;
	std::unique_ptr<StringWindow> logoutWindow;
	bool prepareToClearFlag;
	bool initEndLogFlag;
	int gameClearTimer;

	KazMath::Vec3<float>p1;
	Sphere t1;
	std::vector<Sphere *>enemyHitBoxArray;
	std::unique_ptr<InstanceMeshCollision> meshCollision;
	std::unique_ptr<InstanceMeshParticle>meshParticle;
	std::unique_ptr<GPUParticleRender>particleRender;
};
