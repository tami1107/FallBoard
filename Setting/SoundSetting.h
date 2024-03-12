#pragma once
#include <vector>
#include "Vec2.h"

class SoundSetting
{

private:
	SoundSetting() = default;
	virtual ~SoundSetting() = default;

	static SoundSetting* m_instance;

public:
	// �R�s�[�R���X�g���N�^�̋֎~
	SoundSetting(const SoundSetting&) = delete;
	SoundSetting& operator=(const SoundSetting&) = delete;
	SoundSetting(SoundSetting&&) = delete;
	SoundSetting& operator=(SoundSetting&&) = delete;

	static SoundSetting& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new SoundSetting;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	// ����������
	void Init();

	// �폜
	void UnLoad();

	// �X�V
	void Update();

	// �`��
	void Draw();

	
private:

	// �n���h���̃��[�h
	void LoadHandle();

	// �I��
	void Select();

	// ���ʒ���
	void VolumeAdjustment();

private:

	// �O���t�B�b�N
	struct Graphic
	{
		// �O���t�B�b�N�n���h��
		std::vector<int>handle;

		// ���W
		Vec2 pos = Vec2(0.0f, 0.0f);
	};

private:

	//////////////////////
	// �O���t�B�b�N�֘A //
	//////////////////////

	// �O���t�B�b�N
	std::vector<Graphic>m_graph;

	//////////////
	// ���ʊ֘A //
	//////////////


	// BGM����
	int m_bgmValue = 0;

	// SE����
	int m_seValue = 0;

	//////////////////
	// �Z���N�g�֘A //
	//////////////////

	// �I��ԍ�
	int m_selectNum = 0;

	// �I���t���[��
	int m_selectInterval = 0;

};

