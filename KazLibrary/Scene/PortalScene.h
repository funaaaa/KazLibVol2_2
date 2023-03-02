#pragma once
#include"../Game/Game.h"
#include"../KazLibrary/Scene/SceneBase.h"
#include"../Game/Event/GoalBoxPortalEffect.h"
#include"../Game/Event/GoalBoxPortalStringEffect.h"
#include"../Game/Event/PortalFlame.h"
#include"../KazLibrary/Render/KazRender.h"

struct PortalRender
{
	BackGroundForDebug bg;
	std::array<BoxPolygonRender, 2>stage;

	PortalRender(int CAMERA_INDEX) :bg(CAMERA_INDEX)
	{
	}
};

enum StageE
{
	STAGE_RED,
	STAGE_GREEN,
	STAGE_GAME,
};

enum 
{
	FLOOR_GREEN,
	FLOOR_RED
};

class PortalScene :public SceneBase
{
public:
	PortalScene();
	~PortalScene();

	void Init();
	void Finalize();
	void Input();
	void Update();
	void Draw();

	int SceneChange();

private:
	KazMath::Vec3<float> debugCameraMove;
	KazMath::Vec3<float> eyePos, targetPos;
	KazMath::Vec3<float> baseEyePos, baseTargetPos;
	KazMath::Vec2<float> angle;

	KazMath::Vec3<float>initPos;
	GoalBoxPortalEffect portal;
	GoalBoxPortalStringEffect stringEffect;


	bool changeFlag;
	bool animFlag;
	std::array<std::unique_ptr<PortalRender>, 3> stages;

	RESOURCE_HANDLE redPortalRenderHandle;
	RESOURCE_HANDLE greenPortalRenderHandle;


	PortalFlame portalFlame;
	bool gameModeFlag;

	Cursor cursor;
	BoxPolygonRender besidePoly, verticlaPoly, cameraPoly, playerBox, warpPlayerBox;;
	KazMath::Vec3<float> nowTargerPos, trackingTargetPos;	//�{���|�Y�A���݃|�Y
	KazMath::Vec2<float> leftRightAngleVel;					//���E���_���W�̌����p�x
	KazMath::Vec2<float> upDownAngleVel;					//�㉺���_���W�̌����p�x
	KazMath::Vec2<float> trackLeftRightAngleVel;
	KazMath::Vec2<float> trackUpDownAngleVel;
	KazMath::Vec2<float> forceCameraDirVel;					//�J�����̑O�㍶�E�̌����������I�Ɏw�肷��
	KazMath::Vec2<float> mulValue;							//�J�����̏㉺���E�̊|���銄��
	KazMath::Vec2<float> mulValue2;							//�J�����̏㉺���E�̊|���銄��

	Sprite3DRender redPortal;
	KazMath::Vec3<float>redPortalCameraPos;

	Sprite3DRender greenPortal;
	KazMath::Vec3<float>greenPortalCameraPos;


	StageE stageGameMode;
	StageE stagePortalMode;
};


