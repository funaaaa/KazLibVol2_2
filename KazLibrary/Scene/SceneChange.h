#pragma once
#include"../DirectXCommon/Base.h"
#include"../Math/KazMath.h"
#include"../Render/KazRender.h"
#include"../Render/KazRender.h"
#include"../Helper/DirtyFlag.h"
namespace ChangeScene {

	class SceneChange
	{
	public:
		SceneChange();

		void Init();
		void Finalize();
		void Update();
		void Draw();

		/// <summary>
		/// シーン遷移を開始します
		/// </summary>
		void Start();

		/// <summary>
		/// 画面が隠れた瞬間の判定を取ります
		/// 必ずトリガー判定で取る事
		/// </summary>
		/// <returns>true...隠れた,false...隠れてない</returns>
		bool AllHiden();

private:
		bool startFlag, finishFlag;
		bool allHidenFlag;
		bool initFlag;

		float tmp;
		float startOutInT[2];

		KazMath::Transform2D transform;
		Sprite2DRender sceneTex;
		RESOURCE_HANDLE handle;
	};
}