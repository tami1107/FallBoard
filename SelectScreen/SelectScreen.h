#pragma once

#include<vector>
#include "Vec2.h"
#include "SceneSelect.h"

template <class TState> class StateMachine;

class SelectScreen
{
public:
	SelectScreen();
	virtual ~SelectScreen();

	/// <summary>
	/// �L�����Z���N�g�V�[���̃|�C���^������
	/// </summary>
	/// <param name="sceneCharSelect">�L�����Z���N�g�V�[���̃|�C���^</param>
	void SetSceneCharSelectPointer(SceneSelect* sceneCharSelect) { m_pSceneCharSelect = sceneCharSelect; }

	void Init();
	void Update();
	void Draw();

public:

	// �X�e�[�g
	enum class State
	{
		PlayCountSelect,// �v���C���[�l���𐔂���
		LevelSelect,	// ��Փx�I��
		End,			// �I������
	};

	// �O���t�B�b�N���
	struct Graphic
	{
		// �O���t�B�b�N�n���h��
		std::vector<int>handle;

		// ���W
		Vec2 pos = Vec2(0.0f, 0.0f);
	};
	

private:

	// �v���C���[�l���I������
	void PlayCountSelectEnter();
	void PlayCountSelectUpdate();

	// ��Փx�I������
	void LevelSelectEnter();
	void LevelSelectUpdate();

	// �I������
	void EndEnter();
	void EndUpdate();

private:

	/// <summary>
	/// �X�e�[�g�}�V���̏����ݒ菈��
	/// </summary>
	void InitStateMachine();

	/// <summary>
	/// �O���t�B�b�N�̃��[�h����
	/// </summary>
	void LoadHandle();

	/// <summary>
	/// �I���̃C���^�[�o������
	/// </summary>
	void SelectInterval();

	/// <summary>
	/// �v���C���[�l���I������
	/// </summary>
	void PlayCountSelect();

	/// <summary>
	/// ��Փx�I������
	/// </summary>
	void LevelSelect();

	/// <summary>
	/// �O���t�B�b�N�̈ړ�����
	/// </summary>
	void MoveGraph();

	/// <summary>
	/// �w�i�`�揈��
	/// </summary>
	void DrawBackGround();

public:

	/// <summary>
	/// �X�e�[�g�ύX����
	/// </summary>
	/// <param name="state">�ύX����X�e�[�g</param>
	void SetStateChange(const State state);

private:

	//////////////////////
	// �O���t�B�b�N�֘A //
	//////////////////////

	// �O���t�B�b�N
	std::vector<Graphic>m_graph;

	// �x�[�X���W
	Vec2 m_basePos;

	// �^�[�Q�b�g���W
	Vec2 m_targetPos;

	// �t���[��
	float m_moveValue;

	//////////////////
	// �Z���N�g�֘A //
	//////////////////

	// �l���I��
	int m_playCountSelectNum;

	// ��Փx�I��
	int m_levelSelectNum;

	// �I���t���[��
	int m_selectInterval;

	//////////////
	// ���o�֘A //
	//////////////

	// �T�C���J�[�u�t���[��
	int m_sinFrame;


	////////////////
	// �V�[���֘A //
	////////////////

	// �V�[���ύX
	SceneSelect::ChangeScene m_changeScene;


	//////////////////
	// �X�e�[�g�֘A //
	//////////////////

	// �X�e�[�g
	State m_state;

	// �X�e�[�g�}�V��
	StateMachine<State>m_pStateMachine;


	////////////////////////
	// �N���X�|�C���^�֘A //
	////////////////////////

	// �L�����Z���N�g�V�[���|�C���^
	SceneSelect* m_pSceneCharSelect;
};

