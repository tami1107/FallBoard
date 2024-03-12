#pragma once
#include<memory>
#include<vector>
#include"Vec2.h"

// �X�e�[�g�}�V��
template <class TState> class StateMachine;

class MainScreen;
class Pause
{
public:
	Pause();
	virtual ~Pause();

	/// <summary>
	/// ���C���X�N���[���|�C���^���擾
	/// </summary>
	/// <param name="mainScreen">���C���X�N���[���|�C���^</param>
	void SetMainScreenPointer(std::shared_ptr<MainScreen>mainScreen) { m_pMainScreen = mainScreen; }


	void Init();
	void Update();
	void Draw();

public:

	// �X�e�[�g
	enum class State
	{
		// �ʏ폈��
		Normal,
		// �|�[�Y����
		Pause
	};

	
	// �|�[�Y����
	void PauseEnter();
	void PauseUpdate();
	void PauseDraw();
	void PauseExit();


	
	/// <summary>
	/// �X�e�[�g�ύX
	/// </summary>
	/// <param name="state">�X�e�[�g</param>
	void SetStateChange(const State& state);

public:


	/// <summary>
	/// �n���h���̃��[�h
	/// </summary>
	void LoadHandle();


private:

	/// <summary>
	/// �X�e�[�g�}�V���̏���������
	/// </summary>
	void InitStateMachine();

	/// <summary>
	/// �|�[�Y�Z���N�g����
	/// </summary>
	void PauseSelect();

	/// <summary>
	/// �|�[�Y�E�B���h�E�`�揈��
	/// </summary>
	void PauseDrawWindow();

	/// <summary>
	/// �|�[�Y�I���O���t�B�b�N�`�揈��
	/// </summary>
	void PauseDrawSelectGraph();

	/// <summary>
	/// �I�����ڌ��菈��
	/// </summary>
	void SelectDetermination();



	/// <summary>
	/// �T�E���h�ݒ�X�V����
	/// </summary>
	void UpdateSoundSetting();

	/// <summary>
	/// �T�E���h�ݒ�`�揈��
	/// </summary>
	void DrawSoundSetting();

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

	//////////////////
	// �Z���N�g�֘A //
	//////////////////

	// �I��ԍ�
	int m_selectNum;

	// �I���t���[��
	int m_selectInterval;

	// �V�[���ύX���邩�ǂ���
	bool m_isScreenChange;

	// ���ʐݒ蒆���ǂ���
	bool m_isSoundSetting;

	//////////////////////
	// �T�C���J�[�u�֘A //
	//////////////////////

	// �T�C���J�[�u�t���[��
	int m_sinFrame;

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

	// ���C���X�N���[���|�C���^
	std::shared_ptr<MainScreen>m_pMainScreen;
};