#include "StateMachine.h"// ����ԏ�ɏ���
#include "Player.h"
#include "Pad.h"




Player::Player()
{
}

Player::~Player()
{
}


void Player::Operation()
{
	// �ړ�
	Move();

	// �U���J�n
	BeginningAttack();
}

void Player::Move()
{
	// �����̏�����
	m_vec = VGet(0, 0, 0);


	// ��ɐi��
	if (Pad::IsPress(PAD_INPUT_UP, m_charData.charNumber))
	{
		// ��ւ̈ړ��ʂ���
		m_vec = VGet(0, 0, 1);

		// �����
		m_directionInfo.direction.current = Direction::Top;
	}
	// ���ɐi��
	if (Pad::IsPress(PAD_INPUT_DOWN, m_charData.charNumber))
	{
		// ���ւ̈ړ��ʂ���
		m_vec = VGet(0, 0, -1);

		// ������
		m_directionInfo.direction.current = Direction::Bottom;
	}
	// ���ɐi��
	if (Pad::IsPress(PAD_INPUT_LEFT, m_charData.charNumber))
	{
		// ���ւ̈ړ��ʂ���
		m_vec = VGet(-1, 0, 0);

		// ������
		m_directionInfo.direction.current = Direction::Left;
	}
	// �E�ɐi��
	if (Pad::IsPress(PAD_INPUT_RIGHT, m_charData.charNumber))
	{
		// �E�ւ̈ړ��ʂ���
		m_vec = VGet(1, 0, 0);

		// �E����
		m_directionInfo.direction.current = Direction::Right;
	}

	// �����蔻��
	Collision();
}

void Player::BeginningAttack()
{
	// �{�^�����������ꍇ�A�U�����s��
	if (Pad::IsTrigger(PAD_INPUT_1, m_charData.charNumber))
	{
		// �U������
		Attack();
	}
}
