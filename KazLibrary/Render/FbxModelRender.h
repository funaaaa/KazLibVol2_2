#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Helper/DirtyFlag.h"
#include"../Render/RenderData.h"
#include"../Loader/FbxModelResourceMgr.h"
#include<memory>

class Blas;
class RaytracingOutput;

class FbxModelRender :public IRender
{
public:
	FbxModelRender();
	void Draw(bool DRAW_FLAG = true);
	void CaluMat();

	void ReleaseSkining();
	FbxModelData data;
	FbxTime currentTime;

	DirectX::XMMATRIX motherMat;
	DirectX::XMMATRIX motherBoneMat;

	/*-- レイトレ新規追加機能 --*/

	std::shared_ptr<Blas> blas;	// レイトレに使用するBlas。全てのモデルがこのクラスを持つようにしたいので本来はIRenderに書くべきだろうが、仮のクラス設計なので扱いやすいこちら側に設定する。
	bool isRaytracingSetUp;		// レイトレがセットアップ済みかを判断するフラグ。SetupRaytracing関数を呼ぶことでtrueになる。falseの間はレイトレされない。
	bool isRayTracingEnabled;	// レイトレが有効になっているかフラグ。レイトレがセットアップ済みでこのフラグがtrueになっている状態でDrawを呼ぶことでTlasに登録される。
	std::weak_ptr<RaytracingOutput> refGBuffer0;
	std::weak_ptr<RaytracingOutput> refGBuffer1;

	/// <summary>
	/// レイトレーシングの準備関数。この関数を呼ぶことでこのモデルでレイトレーシングが有効化される。
	/// </summary>
	/// <param name="IsOpaque"> 不透明フラグ このモデルは不透明だ！って場合はtrue 半透明になるかも！って場合はfalseを入れてください。軽量化のために必要な変数。 </param>
	void SetupRaytracing(std::weak_ptr<RaytracingOutput> GBuffer0, std::weak_ptr<RaytracingOutput> GBuffer1, bool IsOpaque = true);

private:

	UINT vertByte;
	UINT indexByte;

	short vertexBufferHandle;
	short indexBufferHandle;

	std::array<RESOURCE_HANDLE, 2> constBufferHandle;
	std::shared_ptr<FbxResourceData> resourceData;

	FbxTime frameTime;
	FbxTime startTime;
	FbxTime endTime;


	bool removeSkining;
};

