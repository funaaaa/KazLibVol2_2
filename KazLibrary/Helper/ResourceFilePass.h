#pragma once
#include"../DirectXCommon/Base.h"

/// <summary>
/// �t�H���_�܂ł̃p�X��Z�߂܂���
/// ���\�[�X�̓ǂݍ��݂Ńp�X�������ꍇ�́A�ǂݍ��݂����t�@�C��������t�H���_����₩��I��Ő擪�ɏ�������A�t�@�C�����������Ă�������
/// </summary>
namespace KazFilePathName
{
	static std::string RelativeResourcePath = "Resource/";

	//�V�F�[�_�[�p�X-----------------------

	static std::string RelativeShaderPath = RelativeResourcePath + "ShaderFiles/";
	static std::string VertexShaderPath = RelativeShaderPath + "VertexShader/";
	static std::string PixelShaderPath = RelativeShaderPath + "PixelShader/";
	static std::string GeometryShaderPath = RelativeShaderPath + "GeometryShader/";
	static std::string ComputeShaderPath = RelativeShaderPath + "ComputeShader/";


	//�摜�p�X-----------------------

	static std::string TestPath = RelativeResourcePath + "Test/";
	static std::string CursorPath = RelativeResourcePath + "Cursor/";
	static std::string HpPath = RelativeResourcePath + "Hp/";
	static std::string GoalPath = RelativeResourcePath + "Goal/";
	static std::string GameOverPath = RelativeResourcePath + "GameOver/";
	static std::string StageUIPath = RelativeResourcePath + "StageUI/";
	static std::string UIPath = RelativeResourcePath + "UI/";
	static std::string TitlePath = RelativeResourcePath + "Title/";
	static std::string SceneChangePath = RelativeResourcePath + "SceneChange/";
	static std::string TutorialPath = RelativeResourcePath + "Tutorial/";
	static std::string EnemyPath = RelativeResourcePath + "Enemy/";
	static std::string EffectPath = RelativeResourcePath + "Effect/";
	static std::string EffectFirePath = EffectPath + "Fire/";
	static std::string LinePath = RelativeResourcePath + "Line/";
	static std::string SoundPath = RelativeResourcePath + "Sound/";
	static std::string StagePath = RelativeResourcePath + "Stage/";
	static std::string PlayerPath = RelativeResourcePath + "Player/";


	//�f�o�b�N�p�X-----------------------

	static std::string ParamPath = RelativeResourcePath + "Parameter/";
	static std::string StageParamPath = ParamPath + "Stage/";

}