#pragma once
#include <vector>
#include "MainScreen.h"


class MainParam
{
public:

	// �o��
	struct Pop
	{
		// �J�E���g�_�E��
		int countDownFrame = 0;
	};

	// �L�����p�����[�^
	struct CharParam
	{
		// �̗�
		int lifePoint = 0;

		// �_���[�W���󂯂����ǂ���
		bool isDamage = false;

		// ���񂾂��ǂ���
		bool isDie = false;
	};

	// �Ֆ�
	struct Board
	{
		// �ՖʃX�P�[��
		int scale = 0;

		// �����������
		int dropCount = 0;

		// �����������s�������ǂ���
		bool isDrap = false;

	};

public:

	MainParam();
	virtual ~MainParam();

	void Init();

public:

	/// <summary>
	/// �J�E���g�_�E������
	/// </summary>
	/// <returns>�J�E���g��Ԃ�</returns>
	int CountDown();

	/// <summary>
	/// �J�E���g�_�E�����I��������ǂ�����Ԃ�
	/// </summary>
	/// <returns>�J�E���g�_�E���I���t���O</returns>
	bool GetIsCountDownEnd();
	

	/// <summary>
	/// �L�����N�^�[�̗̑͂��擾
	/// </summary>
	/// <param name="charNumber">�L�����N�^�[�i���o�[</param>
	/// <param name="lifePoint">�̗�</param>
	void SetCharLife(const int& charNumber, const int& lifePoint);

	/// <summary>
	/// �L�����N�^�[�̗̑͂�Ԃ�
	/// </summary>
	/// <param name="charNumber">�L�����N�^�[�i���o�[</param>
	/// <returns>�i���o�[�ɑΉ������A�̗͂�Ԃ�</returns>
	int GetCharLife(const int& charNumber) { return m_charInfo[charNumber].lifePoint; }

	/// <summary>
	/// �L�����N�^�[���_���[�W���󂯂����ǂ������擾
	/// </summary>
	/// <param name="charNumber">�L�����N�^�[�i���o�[</param>
	/// <param name="isDamage">�_���[�W�t���O</param>
	void SetIsCharDamage(const int& charNumber, const bool& isDamage);

	/// <summary>
	/// �L�����N�^�[���_���[�W���󂯂����ǂ�����Ԃ�
	/// </summary>
	/// <param name="charNumber">�L�����N�^�[�i���o�[</param>
	/// <returns>�i���o�[�ɑΉ������A�_���[�W�t���O��Ԃ�</returns>
	bool GetIsCharDamage(const int& charNumber) { return m_charInfo[charNumber].isDamage; }
	



	/// <summary>
	/// �Ֆʂ̃X�P�[�����擾
	/// </summary>
	/// <param name="boardScale">�Ֆʂ̃X�P�[��</param>
	void SetBoardScale(const int& boardScale) { m_board.scale = boardScale; }
	
	/// <summary>
	/// �Ֆʂ̃X�P�[����Ԃ�
	/// </summary>
	/// <returns>�Ֆʂ̃X�P�[��</returns>
	int GetBoardScale() { return  m_board.scale; }

	/// <summary>
	/// �Ֆʂ𗎉��������
	/// </summary>
	void BoardDropCount();

	/// <summary>
	/// �Ֆʗ����I��
	/// </summary>
	void BoardDropEnd();

	/// <summary>
	/// �����񐔂�Ԃ�
	/// </summary>
	/// <returns>�Ֆʂ̗�����</returns>
	int GetBoardDropCount() { return m_board.dropCount; }

	/// <summary>
	/// �������������������擾
	/// </summary>
	/// <param name="isDrap">�����t���O</param>
	void SetIsDropBoard(const bool& isDrap) { m_board.isDrap = isDrap; }

	/// <summary>
	/// �����������������ǂ�����Ԃ�
	/// </summary>
	/// <returns>�������Ă���t���O</returns>
	bool GetIsDropBoard() {return m_board.isDrap; }



	/// <summary>
	/// �X�e�[�g���Z�b�g
	/// </summary>
	/// <param name="state">�X�e�[�g</param>
	void SetState(const MainScreen::State& state) { m_state = state; }


	/// <summary>
	/// �X�e�[�g��Ԃ�
	/// </summary>
	/// <returns>�X�e�[�g</returns>
	MainScreen::State GetState() { return m_state; }


private:

	
	/// <summary>
	/// ���ʂ��v�Z����
	/// </summary>
	/// <returns>����</returns>
	int RankCalculation();

private:

	//////////////
	// �o��֘A //
	//////////////

	// �o��֘A
	Pop m_pop;


	//////////////////////////////
	// �L�����p�����[�^���֘A //
	//////////////////////////////

	// �L�����p�����[�^���
	std::vector<CharParam>m_charInfo;


	//////////////
	// �Ֆʊ֘A //
	//////////////

	// �Ֆ�
	Board m_board;


	//////////////////
	// �X�e�[�g�֘A //
	//////////////////

	// �X�e�[�g
	MainScreen::State m_state;
};

