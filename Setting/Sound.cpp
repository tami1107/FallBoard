#include "Sound.h"
#include <DxLib.h>

#include <cassert>
#include <fstream>
#include <sstream>
#include "EvoLib.h"

Sound* Sound::m_instance = nullptr;

namespace
{
	// �T�E���h�̃t�@�C���p�X
	std::string  kDataFilePaht = "Data/Sound/";

	// ���ʂ̍ő�l
	constexpr int kSoundMaxValue = 255;

	// input�����������delimiter�ŕ�������
	// ������̕������Ԃ�
	std::vector<std::string> split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);	// string��getline�Ŏg����f�[�^�ɕϊ��H
		std::string field;					// ��������������1�����i�[����H
		std::vector<std::string> result;	// ������̕�����̔z��

		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}
}

void Sound::Init()
{
	// �t�@�C�����̓ǂݍ���
	std::ifstream ifs("Data/csv/ChangeableData/Sound.csv");
	std::string line;

	// �s�J�E���g
	int lineCount = 0;

	while (getline(ifs, line))
	{
		// �s�J�E���g�����Z
		lineCount++;

		// ��s�ڂ͐����̂��ߏ������΂�
		if (lineCount <= 1)continue;


		//printfDx("%s\n", line.c_str());	// test 1�s�\��
		// csv�f�[�^1�s��','�ŕ����̕�����ɕ���
		std::vector<std::string>	strvec = split(line, ',');

		// strvec[0]	: �t�@�C����	string
		// strvec[1]	: �g���q		string
		// strvec[2]	: BGM�t���O		int(bool�ɕϊ�������)
		// strvec[3]	: ����			float

		// �������K�؂ȃf�[�^�^�ɕϊ����Ċi�[
		Data data;

		// �t�@�C��������
		std::string fileName = strvec[static_cast<int>(File::FileName)];
		std::string fileExtension = strvec[static_cast<int>(File::FileExtension)];
		data.filePath = kDataFilePaht + fileName + fileExtension;


		// BGM���ǂ����𔻒�
		if (strvec[static_cast<int>(File::SoundType)] == "BGM")
		{
			data.isBgm = true;
		}
		else
		{
			data.isBgm = false;
		}

		// �{�����[������
		data.volRate = stof(strvec[static_cast<int>(File::SoundVolume)]);

		// �ő�{�����[��
		data.maxVolume = Convert_Lib::ConvertingValueToPercent(kSoundMaxValue, data.volRate);
		
		// �ő�{�����[�����{�����[���ɑ��
		data.soundVolume = data.maxVolume;

		// �{�����[����ݒ�{�����[���ɑ��
		data.settingSoundVolume = data.soundVolume;

		// �n���h���̏�����
		data.handle = -1;

		m_data[strvec[0]] = data;
	}
}

void Sound::Load(const std::vector<std::string>& fileName, bool ASyncLoad)
{
	// �񓯊��������s�����ǂ���
	if (ASyncLoad)
	{
		// ���\�[�X�̓ǂݍ���
		SetUseASyncLoadFlag(true);	// �񓯊��ǂݍ��݂��s������
	}

	// �T�E���h�̓ǂݍ���
	for (auto& file : fileName)
	{
		// �n���h����������
		m_data[file].handle = LoadSoundMem(m_data[file].filePath.c_str());

		// ���ʂ�������
		m_data[file].soundVolume = m_data[file].settingSoundVolume;

		// ���ʐݒ�
		ChangeVolumeSoundMem(m_data[file].soundVolume, m_data[file].handle);
	}

	// ���\�[�X�̓ǂݍ���
	SetUseASyncLoadFlag(false);	// �f�t�H���g�ɖ߂��Ă���
}

void Sound::UnLoad()
{
	// ���ׂẴn���h�����폜
	InitSoundMem();
}

void Sound::Play(const std::string& filename)
{
	// �T�E���h�̃v���C�^�C�v
	int playType = 0;

	if (m_data[filename].isBgm)
	{
		playType = DX_PLAYTYPE_LOOP;
	}
	else
	{
		playType = DX_PLAYTYPE_BACK;
	}

	// ���ʐݒ�
	ChangeVolumeSoundMem(m_data[filename].soundVolume, m_data[filename].handle);

	// �T�E���h��炷
	PlaySoundMem(m_data[filename].handle, playType);
}

void Sound::ScreenFadeBGMStop(const int& fadeValue)
{
	// �t�F�[�h�̍ő�l
	const int fadeMaxValue = 255;

	// �t�F�[�h�l�̌v�Z
	const int fadeValueReverse = fadeMaxValue - fadeValue;

	// �t�F�[�h�l��S�����Ōv�Z
	const int fadePercent= Convert_Lib::PercentageRate(static_cast<float>(fadeMaxValue), static_cast<float>(fadeValueReverse));

	// �T�E���h�̓ǂݍ���
	for (auto& data : m_data)
	{
		// BGM�łȂ������ꍇ�A�R���e�B�j���[����
		if (!data.second.isBgm)continue;

		// �T�E���h�{�����[�������߂�
		data.second.soundVolume = Convert_Lib::ConvertingFromPercentToValue(data.second.settingSoundVolume, static_cast<float>(fadePercent));
		
		// ���ʐݒ�
		ChangeVolumeSoundMem(data.second.soundVolume, data.second.handle);
	}
}

void Sound::SetSoundVolume(const SoundTypeInfo& soundType, const int& soundPercentVolume)
{
	// �f�[�^���̃T�E���h�^�C�v
	SoundTypeInfo dataSoundType = SoundTypeInfo::BGM;

	// �T�E���h�̓ǂݍ���
	for (auto& data : m_data)
	{
		// ���̃T�E���h��BGM��SE���𒲂ׁA�T�E���h�^�C�v�ɑ������
		if (data.second.isBgm)
		{
			dataSoundType = SoundTypeInfo::BGM;
		}
		else
		{
			dataSoundType = SoundTypeInfo::SE;
		}

		// �T�E���h�^�C�v�������ł͂Ȃ��ꍇ�Acontinue����
		if (soundType != dataSoundType)continue;

		// �T�E���h���ʂ𒲂ׂ�
		const int soundVolume = Convert_Lib::ConvertingFromPercentToValue(data.second.maxVolume, static_cast<float>(soundPercentVolume));

		// �ݒ�{�����[���ɒ��ׂ��T�E���h�{�����[����������
		data.second.settingSoundVolume = soundVolume;

		// �{�����[���ɐݒ�{�����[����������
		data.second.soundVolume = data.second.settingSoundVolume;

		// ���ʐݒ�
		ChangeVolumeSoundMem(data.second.soundVolume, data.second.handle);
	}
}

