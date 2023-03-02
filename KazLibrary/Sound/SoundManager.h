#pragma once
#include"../DirectXCommon/Base.h"
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>
#include"../Helper/ISinglton.h"
#include"../Helper/HandleMaker.h"

struct ChunkHeader
{
	std::array<char,4> id;		//�`�����N����ID
	int32_t size;	//�`�����N�T�C�Y
};

struct RiffHeader
{
	ChunkHeader chunk;	//RIFF
	std::array<char,4> type;	//WAVE
};

struct FormatChunk
{
	ChunkHeader chunk;	//fmt
	WAVEFORMATEX fmt; //�g�`�t�H�[�}�b�g
};

struct SoundData
{
	WAVEFORMATEX wfex;
	BYTE *pBuffer;
	unsigned int bufferSize;
};

struct Sound
{
	std::string filePass;
	std::unique_ptr<SoundData> soundData;
	IXAudio2SourceVoice* soundSorce;
};
/// <summary>
/// ���Đ��Ɋւ���N���X�ł�
/// </summary>
class SoundManager :public ISingleton<SoundManager>
{
public:
	SoundManager();	

	/// <summary>
	/// �S���̉����J�����܂�
	/// </summary>
	void Finalize();

	/// <summary>
	/// .wav�̉���ǂݍ��݂܂�
	/// </summary>
	/// <param name="FILE_PASS">�t�@�C���p�X</param>
	/// <returns>�n���h��</returns>
	RESOURCE_HANDLE LoadSoundMem(std::string FILE_PASS, bool BGM_FLAG = true);

	/// <summary>
	/// �����Đ����܂�
	/// </summary>
	/// <param name="HANDLE">�Đ����������f�[�^�̃n���h��</param>
	/// <param name="VOLUME">����</param>
	/// <param name="LOOP_FLAG">�������[�v�Đ����邩���Ȃ���</param>
	void PlaySoundMem(RESOURCE_HANDLE HANDLE, int VOLUME, bool LOOP_FLAG = false);

	void ChangeSoundMem(RESOURCE_HANDLE HANDLE, int VOLUME);
	/// <summary>
	/// ���y���~�߂܂�
	/// </summary>
	/// <param name="HANDLE">�~�߂������y�f�[�^�̃n���h��</param>
	void StopSoundMem(RESOURCE_HANDLE HANDLE);

	/// <summary>
	/// ���f�[�^�������[�X���܂�
	/// </summary>
	/// <param name="HANDLE">�����[�X���������f�[�^�̃n���h��</param>
	void ReleaseSoundMem(RESOURCE_HANDLE HANDLE);


private:
	HandleMaker handle;
	std::array<Sound, 30> masterSoundData;

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice *masterVoice;
};

