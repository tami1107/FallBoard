#pragma once

#include <DxLib.h>
#include "EvoLib.h"
#include <array>

// �V���O���g�����g�p�������f���f�[�^�N���X
class ModelData
{
private:
	ModelData() = default;
	virtual ~ModelData() = default;

	static ModelData* m_instance;

public:
	
	// �R�s�[�R���X�g���N�^�̋֎~
	ModelData(const ModelData&) = delete;
	ModelData& operator=(const ModelData&) = delete;
	ModelData(ModelData&&) = delete;
	ModelData& operator=(ModelData&&) = delete;

	static ModelData& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new ModelData;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	// ���f��
	struct Model
	{
		// �n���h��
		int handle = -1;

		// ���W
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);

		// �����p�x
		float initAngle = 0.0f;
	};

	// ���f���`��f�[�^
	struct DrawModelData
	{
		// �v���C���[�ԍ�
		int playerNumber = 0;

		// �p�x
		int angle = 0;

		// ���W
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);
	};


	// �t�F�C�X
	enum class Face
	{
		Noramal,// �ʏ�
		Smile,	// �Ί�
		Sulking,// �X�˂�
		Cry		// ����
	};

public:

	/// <summary>
	/// ���[�h����
	/// </summary>
	void Load();

	/// <summary>
	/// �A�����[�h����
	/// </summary>
	void UnLoad();



	/// <summary>
	/// �Q�[���v���C�p�̃��f�����[�h����
	/// </summary>
	void LoadGamePlayModel();

	/// <summary>
	/// �Q�[���v���C�p�̃��f�����폜����
	/// </summary>
	void DeleteGamePlayModel();

	/// <summary>
	/// ���f���`�揈��
	/// </summary>
	/// <param name="drawModelData">���f���`��f�[�^</param>
	void DrawModel(const DrawModelData& drawModelData);


	/// <summary>
	/// �\��ύX����
	/// </summary>
	/// <param name="face">�t�F�C�X���</param>
	/// <param name="modelNum">���f���ԍ�</param>
	void ChangeFace(const Face& face, const int& modelNum);

private:

	/// <summary>
	/// ���f���Z�b�g�A�b�v����
	/// </summary>
	void ModelSetUp();

	/// <summary>
	/// ���f���̉�]����
	/// </summary>
	/// <param name="typeNum">�^�C�v�ԍ�</param>
	/// <param name="angle">�p�x</param>
	void RotModel(const int& typeNum, const int& angle);

	/// <summary>
	/// ���W���ړ�������
	/// </summary>
	/// <param name="typeNum">�^�C�v�ԍ�</param>
	/// <param name="pos">���W</param>
	void CoordinateMove(const int& typeNum, const VECTOR& pos);

private:

	// ���f���X�P�[��
	float m_modelScale = 0.0f;

	// �\��n���h��
	std::vector<int>m_faceHandle;

	// ���f�����
	std::vector<Model>m_saveModel;

	// ���f���ꎞ�ۑ�
	std::vector<Model>m_playModel;
};