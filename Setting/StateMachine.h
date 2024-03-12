#pragma once
#include <functional>
#include <map>

/// <summary>
/// �X�e�[�g�}�V��
/// </summary>
template <class TState> class StateMachine
{
public:
	typedef std::function<void(void)> Delegate;

private:
	class StateDelegateSet
	{
	public:
		TState state;
		Delegate enter, update, draw, exit;

		// �ϐ��������x�����o��̂ŁA�f�t�H���g�R���X�g���N�^�ŏ�����
		StateDelegateSet() : state(TState()){}
	};

	TState currentState{};
	std::map<TState, StateDelegateSet> stateFuncMap;
	bool isInitialized = false;

public:
	/// <summary>
	/// �X�e�[�g�̒ǉ�
	/// </summary>
	void AddState(TState state, Delegate enter, Delegate update, Delegate draw, Delegate exit)
	{
		// FIXME: �d���l��
		StateDelegateSet set;
		set.state = state;
		set.enter = enter;
		set.update = update;
		set.draw = draw;
		set.exit = exit;
		stateFuncMap.emplace(state, set);
	}

	/// <summary>
	/// �X�e�[�g�̐ݒ�
	/// </summary>
	void SetState(TState state)
	{
		// �����enter�����Ă�
		if (isInitialized == false)
		{
			isInitialized = true;
			currentState = state;
			stateFuncMap[currentState].enter();
		}
		// ���O��state��exit���Ă�ŃX�e�[�g���X�V
		else if (currentState != state)
		{
			stateFuncMap[currentState].exit();
			currentState = state;
			stateFuncMap[currentState].enter();
		}
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update()
	{
		stateFuncMap[currentState].update();
	}

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()
	{
		stateFuncMap[currentState].draw();
	}


	// �X�e�[�g�̎擾
	TState GetCurrentState() const { return currentState; }
};

