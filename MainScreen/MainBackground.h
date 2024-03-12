#pragma once
#include <vector>
#include "Vec2.h"
#include "GameData.h"

class MainBackground
{
public:
	MainBackground();
	virtual ~MainBackground();

	void Init();
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="isPause">�|�[�Y�����ǂ���</param>
	void Draw(bool isPause = false);

public:

	// �n���h���̃��[�h
	void LoadHandle();

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

	// ��Փx
	GameData::Level m_level;

	// �ړ���
	float m_moveValue;
};

