#include "StateMachine.h"// ����ԏ�ɏ���
#include "Computer.h"
#include "GameData.h"
#include "CharacterManager.h"

namespace
{
	// �Փˎ���
	constexpr int kCollisionFrame = 30;

	// �U���N�[���^�C��
	constexpr int kAttackCoolTime = 120;

	// ���x���̃N�[���^�C��
	constexpr int kLevelCoolTime = 120;
}

Computer::Computer():
	// �U���֘A��
	m_attackMaxCoolTime(),
	m_attackCoolTime(),
	// �����蔻��֘A��
	m_coordinatePos()
{
}

Computer::~Computer()
{
}

void Computer::InitSet()
{
	// ���W����
	m_coordinatePos = m_pos;

	// �ړI�n�𒲂ׂ�
	CheckDestination();

	// ���x���ԍ�
	int levelNum = 0;

	// �擾�������x������Ăɔԍ���������
	switch (GameData::getInstance().GetLevel())
	{
	case GameData::Level::Easy:

		levelNum = 2;

		break;

	case GameData::Level::Noramal:

		levelNum = 1;

		break;

	case GameData::Level::Hard:

		levelNum = 0;

		break;

	default:
		break;
	}

	// �ő�U���N�[���^�C��
	m_attackMaxCoolTime = kAttackCoolTime + (kLevelCoolTime * levelNum);

	// �U���N�[���^�C���ɍő�U���N�[���^�C������
	m_attackCoolTime = m_attackMaxCoolTime;
}

void Computer::Operation()
{
	// �ړ�����
	Move();

	// �U���N�[���^�C������
	AttackCoolTime();
}

void Computer::Move()
{
	// �ړI�n�ֈړ����鏈��
	MoveDestination();

	// �����̏�����
	m_vec = VGet(0, 0, 0);

	// �i�ޕ������擾
	m_directionInfo.direction.current = m_pAlgorithm->TargetDirection(m_pos, m_coordinatePos);

	// �擾�����i�ޕ������A�ړ��ʂ𒲂ׂ�
	switch (m_directionInfo.direction.current)
	{
	case Direction::Top:

		// ��ւ̈ړ��ʂ���
		m_vec = VGet(0, 0, 1);

		break;
	case Direction::Bottom:

		// ���ւ̈ړ��ʂ���
		m_vec = VGet(0, 0, -1);

		break;
	case Direction::Left:

		// ���ւ̈ړ��ʂ���
		m_vec = VGet(-1, 0, 0);

		break;
	case Direction::Right:

		// �E�ւ̈ړ��ʂ���
		m_vec = VGet(1, 0, 0);

		break;
	default:
		break;
	}


	// �l�������ׂĂɐi�߂Ȃ��ꍇ�A�ړ��ʂ�0�ɂ���
	if (m_pAlgorithm->IsAroundObstacle(m_pos))
	{
		// ���ݍ��W����Z����ړI�n�ɂ���
		m_coordinatePos = m_pAlgorithm->FindCellSafeClose(m_pos);
	}

	// �����蔻��
	Collision();
}

void Computer::BeginningAttack()
{
	// �U���ł��邩�ǂ������擾����
	const Algorithm::Attack attack = m_pAlgorithm->IsAssailable(m_pos, GetAlivePos());

	// false�̏ꍇ�A�����ŏ������I������
	if (!attack.isAttack)
	{
		return;
	}

	// �U����������
	m_directionInfo.direction.current = attack.direction;

	// �U���C���^�[�o������
	m_attackCoolTime = m_attackMaxCoolTime;

	// �U������
	Attack();

	// �c��̃L�����N�^�[�����ׂăR���s���[�^�Ȃ�΁A�U�����x�𑁂߂�
	if (m_pCharacterManager->IsAllCP())
	{
		m_attackCoolTime = kAttackCoolTime;
	}
}

void Computer::AttackCoolTime()
{
	// �U���N�[���^�C��
	{
		if (m_state == State::Pop)return;

		// �U���N�[���^�C�������炷
		m_attackCoolTime--;

		// �U���N�[���^�C����0�ȉ��ɂȂ����ꍇ�A0�ɂ���
		if (m_attackCoolTime <= 0)
		{
			m_attackCoolTime = 0;
		}

		// �U���N�[���^�C����0�ȊO�̏ꍇ�A�����ŏ������I������
		if (m_attackCoolTime != 0)
		{
			return;
		}
	}

	// �U���J�n
	BeginningAttack();
}

void Computer::MoveDestination()
{

	// ���ݒn���A�ړI�n�ɏ�Q��������ꍇ�A�ړI�n��ύX����
	if (m_pAlgorithm->IsObstacle(m_coordinatePos) || m_pAlgorithm->IsObstacle(m_pos))
	{
		// ���ݍ��W����Z����ړI�n�ɂ���
		m_coordinatePos = m_pAlgorithm->FindCellSafeClose(m_pos);
	}

	// ���W���Z���̒��S�ʒu�Ɉړ������邽�߂ɁA�l��␳
	if (Calculation_Lib::IsTargetRangeValue(m_pos.x, m_coordinatePos.x, m_charData.moveSpeed))
	{
		m_pos.x = m_coordinatePos.x;
	}
	if (Calculation_Lib::IsTargetRangeValue(m_pos.z, m_coordinatePos.z, m_charData.moveSpeed))
	{
		m_pos.z = m_coordinatePos.z;
	}
	

	// �ړI�n�ɂ����ꍇ�A���̖ړI�n��T��
	if (m_pos.x == m_coordinatePos.x &&
		m_pos.z == m_coordinatePos.z)
	{
		// �A���S���Y�����l����
		ThinkAlgorithm();
		
		// �ړI�n���m�F
		CheckDestination();
	}
}

void Computer::CheckDestination()
{
	// �ړI�n����
	m_coordinatePos = m_pAlgorithm->MoveCoordinate(m_pos);
}

void Computer::ThinkAlgorithm()
{
	// �����_���ɐi��
	m_pAlgorithm->RandomMove(m_pos);
}
