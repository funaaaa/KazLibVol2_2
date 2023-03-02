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
		/// �V�[���J�ڂ��J�n���܂�
		/// </summary>
		void Start();

		/// <summary>
		/// ��ʂ��B�ꂽ�u�Ԃ̔�������܂�
		/// �K���g���K�[����Ŏ�鎖
		/// </summary>
		/// <returns>true...�B�ꂽ,false...�B��ĂȂ�</returns>
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