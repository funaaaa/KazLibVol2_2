#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"

class LineFlashLight
{
public:
	LineFlashLight();
	void Init(const std::vector<KazMath::Vec3<float>*> &POS_ARRAY, int TIMER);
	void Finalize();
	void Update();
	void Draw();

	bool IsFinish();
private:
	std::vector<KazMath::Vec3<float>*>posArray;//OBJがどの方向に辿って進んでいくか判断する為の座標を纏めた配列

	float speed;
	int lineIndex;

	BoxPolygonRender boxR;

	void CheckWhereToGo();
	float CaluDisntace();

	float *nowPos = nullptr;
	float *nextPos = nullptr;
	bool finishFlag;

	bool useXPosFlag;
	bool useYPosFlag;
	bool useZPosFlag;

};
