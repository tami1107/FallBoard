#pragma once
#include "game.h"

#include <array>
#include <vector>
#include <string>
#include <map>

// �V���O���g�����g�p�����T�E���h�N���X
class Sound
{
public:
	enum Id
	{
		Bgm,
	};
private:
	Sound() = default;
	virtual ~Sound() = default;

	static Sound* m_instance;

public:
	// �R�s�[�R���X�g���N�^�̋֎~
	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;
	Sound(Sound&&) = delete;
	Sound& operator=(Sound&&) = delete;

	static Sound& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new Sound;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	// �T�E���h�^�C�v���
	enum class SoundTypeInfo
	{
		BGM,// BGM
		SE,	// SE
	};


private:

	// �t�@�C��
	enum class File
	{
		FileName,		// �t�@�C����
		FileExtension,	// �g���q
		SoundType,		// �T�E���h�̃^�C�v
		SoundVolume,	// �T�E���h�̉���
	};

	// �T�E���h�f�[�^
	struct Data
	{
		bool isBgm = false;			// ���ꂪBGM�����ʉ���
		float volRate = 0.0f;		// �{�����[������
		int maxVolume = 0;			// �ő�{�����[��
		int soundVolume = 0;		// �T�E���h�{�����[��
		int settingSoundVolume = 0;	// �ݒ肵���T�E���h�{�����[��
		int handle = -1;			// �T�E���h�n���h��
		std::string filePath;		// �t�@�C���p�X
	};

	
public:
	// ��������
	void Init();

	/// <summary>
	/// �T�E���h���[�h
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="ASyncLoad">�񓯊��������s�����ǂ���</param>
	void Load(const std::vector<std::string>& fileName, bool ASyncLoad = false);

	/// <summary>
	/// �T�E���h�A�����[�h
	/// </summary>
	void UnLoad();

	/// <summary>
	/// �Đ�
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	void Play(const std::string& filename);

	/// <summary>
	/// ��ʂ̃t�F�[�h�ɍ��킹��BGM������
	/// </summary>
	/// <param name="fadeValue">�����������FadeBrightValue()</param>
	void ScreenFadeBGMStop(const int& fadeValue);

	/// <summary>
	/// �T�E���h���ʐݒ�
	/// </summary>
	/// <param name="soundType">�T�E���h�^�C�v</param>
	/// <param name="soundPercentVolume">�T�E���h�p�[�Z���g�{�����[��</param>
	void SetSoundVolume(const SoundTypeInfo& soundType, const int& soundPercentVolume);

private:
	// �����f�[�^
	std::map<std::string, Data>	m_data;
};

