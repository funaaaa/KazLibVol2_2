#pragma once
#include <memory>
#include <vector>
#include <d3d12.h>
#include <string>
#include <wrl/client.h>
#include "../Helper/ISinglton.h"
#include "dxcapi.h"

// シェーダーデータクラスの前方宣言
class ShaderData;

/// <summary>
/// シェーダーデータ保存クラス レイトレ用に持ってきました。新しいエンジンではラスタライズで使ってるやつと統合させます。
/// </summary>
class ShaderStorage : public ISingleton<ShaderStorage> {

private:

	/*-- メンバ変数 --*/

	std::vector<std::shared_ptr<ShaderData>> shaderData_;


public:

	/*-- メンバ関数 --*/

	// シェーダーをロードする。
	Microsoft::WRL::ComPtr<ID3DBlob> LoadShader(const std::string& ShaderPath, const std::string& ShaderModel, const std::string& EntryPoint);
	Microsoft::WRL::ComPtr<ID3DBlob> LoadShaderForDXC(const std::string& ShaderPath, const std::string& ShaderModel, const std::string& EntryPoint);

	// シェーダーデータを返す。
	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderData(const std::string& ShaderPath);
	Microsoft::WRL::ComPtr<IDxcBlob> GetShaderDataForDXC(const std::string& ShaderPath);
	std::vector<char>& GetShaderBin(const std::string& ShaderPath);

};


