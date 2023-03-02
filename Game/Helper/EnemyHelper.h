#pragma once
#include<memory>
#include<array>
#include"../Game/Enemy/NormalEnemy.h"
#include"../Game/Enemy/KidEnemy.h"
#include"../Game/Enemy/SplineMisile.h"
#include"../Game/Enemy/NormalMisileEnemy.h"
#include"../Game/Enemy/BattleshipEnemy.h"
#include"../Game/Enemy/SplineMisileForBattleShip.h"
#include"../Game/Enemy/BikeEnemy.h"
#include"../Game/Enemy/SplineMisileForBikeEnemy.h"
#include"../Game/Enemy/SummonEnemy.h"
#include"../Game/Enemy/PopEnemy.h"
#include"../Game/UI/Cursor.h"
#include"../Game/UI/AttackLog.h"
#include"../Game/Effect/InstanceMeshParticle.h"


namespace KazEnemyHelper
{
	static const int LAYER_LEVEL_MAX = 10;				//���C���[���x���̍ő吔
	static const int ENEMY_NUM_MAX = 100;				//1���C���[���x���ɓo�ꂷ��G�̍ő吔
	static const int STAGE_NUM_MAX = 1;				//1���C���[���x���ɓo�ꂷ��G�̍ő吔

	void GenerateEnemy(
		std::array<std::array<std::unique_ptr<IEnemy>, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &ENEMIES,
		std::array<std::array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> RESPONE_DATA,
		std::array<int, 10> &ENEMISE_HANDLE
	);

	void GenerateEnemy(
		std::array<std::array<std::unique_ptr<IEnemy>, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &ENEMIES,
		std::array<std::array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> RESPONE_DATA,
		std::array<int, 10> &ENEMISE_HANDLE,
		std::vector<Sphere*> &ENEMISE_HITBOX
	);

	void AddEnemy(
		const std::array<std::array<std::unique_ptr<IEnemy>, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &ENEMIES,
		std::array<std::array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &RESPONE_DATA,
		std::array<int, 10> &ADD_ENEMISE_HANDLE,
		int GAME_FLAME,
		int GAME_STAGE_LEVEL
	);

	void InitEnemy(
		std::array<std::array<std::unique_ptr<IEnemy>, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &ENEMIES,
		std::array<std::array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> RESPONE_DATA,
		int GAME_FLAME,
		int GAME_STAGE_LEVEL,
		bool DEMO_FLAG = false
	);

	bool LockOn(Cursor *CURSOR, const std::unique_ptr<IEnemy> &ENEMY, AttackLog *LOG, float FONT_SIZE, RESOURCE_HANDLE LOCKON_SOUND);

	enum CameraDir
	{
		CAMERA_FRONT,
		CAMERA_BACK,
		CAMERA_LEFT,
		CAMERA_RIGHT,
	};
	struct ForceCameraData
	{
		int flame;
		CameraDir dir;
		ForceCameraData() :flame(-1)
		{
		};
	};
}
