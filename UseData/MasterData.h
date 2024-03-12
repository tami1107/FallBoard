#pragma once

#include <array>
#include <vector>
#include <string>
#include <map>
#include <DxLib.h>
#include "EvoLib.h"

class MasterData
{

private:
	MasterData() = default;
	virtual ~MasterData() = default;

	static MasterData* m_instance;

public:
	// �R�s�[�R���X�g���N�^�̋֎~
	MasterData(const MasterData&) = delete;
	MasterData& operator=(const MasterData&) = delete;
	MasterData(MasterData&&) = delete;
	MasterData& operator=(MasterData&&) = delete;

	static MasterData& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new MasterData;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	// �t�@�C������ǂݎ��L�����N�^�[�X�e�[�^�X
	struct FileReadCharcterStatus
	{
		// ���C�t
		int lifePoint = 0;

		// �ړ����x
		float moveSpeed = 0.0f;

		// �U���d������
		int attackRigorTime = 0;

		// ���A���̖��G����
		int invincibleTime = 0;

		// �J���[
		std::vector<VECTOR>color;
	};

	// �t�@�C������ǂݎ��U���G�t�F�N�g���
	struct FileReadAttackEffectInfo
	{
		// �t�@�C���p�X
		std::string filePath = "";

		// �O���t�B�b�N�X�P�[��
		float scale = 0.0f;

		// �g�p����v���C���[�ԍ�
		int usePlayerNumber = 0;
	};

	// �t�@�C������ǂݎ�镜���G�t�F�N�g���
	struct FileReadResurrectionEffectInfo
	{
		// �t�@�C���p�X
		std::string filePath = "";

		// �O���t�B�b�N�X�P�[��
		float scale = 0.0f;

	};

	// �J���[���
	struct ColorInfo
	{
		VECTOR color_0;// �J���[1
		VECTOR color_1;// �J���[2
	};


	// �t�@�C������ǂݎ��t�B�[���h���
	struct FileReadFieldInfo
	{
		// �t�@�C���p�X
		std::string filePath = "";

		// ���f���X�P�[��
		float modelScale = 0.0f;

		// �����C���^�[�o��
		int fallInterval = 0;

		// �����Ԋu
		int differenceFallInterval = 0;

		// �Ֆʏk�������J���[�C���^�[�o��
		int deleteFallColorInterval = 0;

		// �����X�s�[�h
		float fallSpeed = 0.0f;

		// ���������ʒu
		float fallLowerLimitPos = 0.0f;

		// �������̐F
		VECTOR fallColor = VECTOR();

		// �Ֆʂ̐F
		std::vector<ColorInfo>color;
	};

	// �t�@�C�����[�h���f�����
	struct FileReadModelInfo
	{
		// �t�@�C���p�X
		std::string filePath = "";

		// ���f������
		float radian = 0.0f;

		// ���f���X�P�[��
		float scale = 1.0f;

		// ���f��Y���W
		float pos_y = 0.0f;

		// ���[�V�������g���邩�ǂ���
		bool isUseMotion = false;
	};

public:


	/// <summary>
	/// �f�[�^�̃��[�h����
	/// </summary>
	void LoadData();

	/// <summary>
	/// �L�����N�^�[�X�e�[�^�X��Ԃ�
	/// </summary>
	/// <returns>�L�����N�^�[�X�e�[�^�X</returns>
	FileReadCharcterStatus GetCharcterStatus() { return m_charcterStatus; }

	/// <summary>
	/// �U���G�t�F�N�g����Ԃ�
	/// </summary>
	/// <returns>�U���G�t�F�N�g���</returns>
	std::vector<FileReadAttackEffectInfo> GetAttackEffectInfo() { return m_attackEffectInfo; }

	/// <summary>
	/// �����G�t�F�N�g����Ԃ�
	/// </summary>
	/// <returns>�����G�t�F�N�g���</returns>
	FileReadResurrectionEffectInfo GetResurrectionEffectInfo() { return m_resurrectionEffectInfo; }


	/// <summary>
	/// �t�B�[���h����Ԃ�
	/// </summary>
	/// <returns>�t�B�[���h���</returns>
	FileReadFieldInfo GetFieldInfo() { return m_fieldInfo; }

	/// <summary>
	/// ���f������Ԃ�
	/// </summary>
	/// <returns>���f�����</returns>
	std::vector<FileReadModelInfo> GetModelInfo() { return m_modelInfo; }

private:

	/// <summary>
	/// �L�����N�^�[�X�e�[�^�X�̃��[�h����
	/// </summary>
	void LoadCharcterStatus();

	/// <summary>
	/// �U���G�t�F�N�g���̃��[�h����
	/// </summary>
	void LoadAtackEffectInfo();

	/// <summary>
	/// �����G�t�F�N�g���̃��[�h����
	/// </summary>
	void LoadResurrectionEffectInfo();

	/// <summary>
	/// �t�B�[���h���̃��[�h����
	/// </summary>
	void LoadFieldInfo();

	/// <summary>
	/// ���f�����̃��[�h����
	/// </summary>
	void LoadModelInfo();


private:

	////////////////////
	// �ۑ��f�[�^�֘A //
	////////////////////

	// �L�����N�^�[�X�e�[�^�X���
	FileReadCharcterStatus m_charcterStatus;

	// �U���G�t�F�N�g���
	std::vector<FileReadAttackEffectInfo> m_attackEffectInfo;

	// �����G�t�F�N�g���
	FileReadResurrectionEffectInfo m_resurrectionEffectInfo;

	// �t�B�[���h���
	FileReadFieldInfo m_fieldInfo;

	// ���f�����
	std::vector<FileReadModelInfo> m_modelInfo;
};