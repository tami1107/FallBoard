#pragma once
#include "SceneResult.h"
#include <vector>
#include <memory>

// �X�e�[�g�}�V��
template <class TState> class StateMachine;

class ResultCharacter;
class ResultCamera;
class ResultBackground;
class ResultScreen
{
public:
	ResultScreen();
	virtual ~ResultScreen();


	/// <summary>
	/// �V�[�����U���g�̃|�C���^���擾
	/// </summary>
	/// <param name="sceneResult">�V�[�����U���g�|�C���^</param>
	void SetSceneMain(SceneResult* sceneResult) { m_pSceneResult = sceneResult; }

	void Init();
	void Update();
	void Draw();

public:

	// �X�e�[�g
	enum class State
	{
		Normal,	// �ʏ폈��
		End,	// �I������
	};

private:

	// �ʏ폈��
	void NormalEnter();
	void NormalUpdate();

	// �I������
	void EndEnter();

private:

	/// <summary>
	/// ���[�h����
	/// </summary>
	void Load();

	/// <summary>
	/// �X�e�[�g����������
	/// </summary>
	void InitState();

public:

	/// <summary>
	/// �X�e�[�g�ύX����
	/// </summary>
	/// <param name="state">�X�e�[�g</param>
	void SetStateChange(const State& state);

private:

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

	// �V�[�����U���g�|�C���^
	SceneResult* m_pSceneResult;

	// ���U���g�L�����N�^�[�|�C���^
	std::shared_ptr<ResultCharacter>m_pResultCharacter;

	// ���U���g�J�����|�C���^
	std::shared_ptr<ResultCamera>m_pResultCamera;

	// ���U���g�o�b�N�O���E���h�|�C���^
	std::shared_ptr<ResultBackground>m_pResultBackground;
};