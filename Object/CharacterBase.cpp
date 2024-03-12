#include "StateMachine.h"// ����ԏ�ɏ���
#include "CharacterBase.h"
#include "Field.h"
#include "MainParam.h"
#include "CharacterManager.h"
#include "EffectManager.h"
#include "MainUI.h"
#include "Sound.h"
#include "Pad.h"


namespace
{
	// �Q�[���I���t���[��
	constexpr int kGameEndMaxFrame = FPS_Lib::FPS * 4;

	// �����]�����x
	constexpr int kChangeDirectionSpeed = 10;

	// �������[�V�����X�s�[�h
	constexpr float kWinMotionSpeed = 5.0f;

	// �p�b�h�̐U���̋���
	constexpr int kPadVibrationStrength = 1000;

	// �p�b�h�̐U���̋���
	constexpr int kPadVibrationTime = FPS_Lib::FPS * 3;
}

namespace
{
	// �T�E���h�t�@�C����
	const std::vector<std::string> kSoundFileName =
	{
		"attack",		// �U����
		"playerDrop",	// ������
		"resurrection",	// ������
		"win"			// ������
	};


	// �T�E���h��
	enum class SoundName
	{
		Attack,			// �U����
		Drop,			// ������
		Resurrection,	// ������
		Win,			// ������
	};
}


CharacterBase::CharacterBase() :
	// �L�����N�^�[�f�[�^�֘A��
	m_charData(),
	// �����֘A��
	m_directionInfo(),
	// �U���֘A��
	m_attackMotionInterval(),
	// �����֘A��
	m_invincibleInfo(),
	// ���W�֘A��
	m_pos(),
	m_vec(),
	// �X�e�[�g�֘A��
	m_state(),
	m_isWin(),
	// �V�[���ύX�֘A��
	m_gameEndFrame(),
	// �N���X�֘A��
	m_pStateMachine()
{
}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Init()
{

	// �X�e�[�g�}�V���̏������AEntry
	auto dummy = []() {};

	// �o�ꏈ�����X�e�[�g�}�V���ɒǉ�
	{
		auto popEnter = [this]() {PopEnter(); };
		auto popUpdate = [this]() {PopUpdate(); };
		auto popDraw = dummy;
		auto popExit = dummy;

		m_pStateMachine.AddState(State::Pop, popEnter, popUpdate, popDraw, popExit);
	}

	// �ʏ폈�����X�e�[�g�}�V���ɒǉ�
	{
		auto normalEnter = dummy;
		auto normalUpdate = [this]() {NormalUpdate(); };
		auto normalDraw = []() {};
		auto normalExit = dummy;

		m_pStateMachine.AddState(State::Normal, normalEnter, normalUpdate, normalDraw, normalExit);
	}

	// �U���������X�e�[�g�}�V���ɒǉ�
	{
		auto attackEnter = [this]() {AttackEnter(); };
		auto attackUpdate = [this]() {AttackUpdate(); };
		auto attackDraw = dummy;
		auto attackExit = dummy;

		m_pStateMachine.AddState(State::Attack, attackEnter, attackUpdate, attackDraw, attackExit);
	}


	// �����������X�e�[�g�}�V���ɒǉ�
	{
		auto fallEnter = [this]() {FallEnter(); };
		auto fallUpdate = [this]() {FallUpdate(); };
		auto fallDraw = dummy;
		auto fallExit = [this]() {FallExit(); };

		m_pStateMachine.AddState(State::Fall, fallEnter, fallUpdate, fallDraw, fallExit);
	}

	// ���S�������X�e�[�g�}�V���ɒǉ�
	{
		auto deathEnter = [this]() {DeathEnter(); };
		auto deathUpdate = dummy;
		auto deathExit = dummy;

		m_pStateMachine.AddState(State::Death, deathEnter, deathUpdate, dummy, deathExit);
	}

	// �����������X�e�[�g�}�V���ɒǉ�
	{
		auto winEnter = [this]() {WinEnter(); };
		auto winUpdate = [this]() {WinUpdate(); };
		auto winExit = dummy;

		m_pStateMachine.AddState(State::Win, winEnter, winUpdate, dummy, winExit);
	}

	// �X�e�[�g�Z�b�g
	SetStateChange(State::Pop);

	// �t�B�[���h�|�C���^�𑗂�
	m_pEffectManager->SetFieldPointer(m_pField);

	// ���g�̃|�C���^�𑗂�
	m_pEffectManager->SetCharacterBasePointer(shared_from_this());

	// �T�E���h���[�h
	Sound::getInstance().Load(kSoundFileName);

}

void CharacterBase::Update()
{
	// �v���C���[�����邱�Ƃ�����
	m_pField->SetPlayerPresent(m_pos);

	// �X�e�[�g�}�V���̍X�V����
	m_pStateMachine.Update();
}

void CharacterBase::Draw()
{
	// ���f���̕`��
	DrawModel();

	// ���S���Ă��鎞�ȊO�̓^�O��\��
	if (m_state != State::Death)
	{
		// �^�O��`��
		m_pMainUI->CharacterTagDraw(m_charData.isPlayer, m_charData.charNumber, m_pos, m_charData.color);
	}
}

void CharacterBase::PopEnter()
{
	// �ϐ��̏�����
	{
		// �ړ��x�N�g���̏�����
		m_vec = VGet(0.0f, 0.0f, 0.0f);

		// �L�����N�^�[�̗̑͂��p�����[�^�ɑ���
		m_pMainParam->SetCharLife(m_charData.charNumber, m_charData.lifePoint);
	}

	// �����֘A
	{
		// �p�x��0�ɂ���
		m_directionInfo.modelAngle = 0;

		// �E�������Ă��Ȃ�
		m_directionInfo.isRightDirection = false;

		// ��������Ă��Ȃ�
		m_directionInfo.isTopDirection = false;

		// �����_���ȕ����𓾂�
		RandomDirection();
	}

	// ���W�֘A
	{
		// �X�|�[���ʒu�擾
		m_pos = m_pField->GetSpawnPos();
	}

	// �����̂���ʒu������
	m_pField->SetPlayerPresent(m_pos);

	// ����������
	InitSet();
}

void CharacterBase::PopUpdate()
{
	// �ړ�
	Move();


	// �J�E���g�_�E���I����Ă����ꍇ�ANormal�X�e�[�g�Ɉڍs����
	if (m_pMainParam->GetIsCountDownEnd())
	{
		// �X�e�[�g�Z�b�g
		SetStateChange(State::Normal);
	}	
}

void CharacterBase::NormalUpdate()
{
	// ����
	Operation();

	// ���G����
	Invincible();

	// �����t���O��true�̏ꍇ�A�X�e�[�g��ύX
	if (m_isWin)
	{
		SetStateChange(State::Win);
	}
}

void CharacterBase::AttackEnter()
{
	// �U���G�t�F�N�g����
	m_pEffectManager->CreateAttackEffect(m_pos, m_directionInfo.direction.current, m_charData.color,m_charData.charNumber);

	// �ړ��ʂ̏�����
	m_vec = VGet(0, 0, 0);

	// �U��SE�𗬂�
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Attack)]);
}

void CharacterBase::AttackUpdate()
{
	// ���G����
	Invincible();

	// �U���C���^�[�o��
	AttackInterval();
}

void CharacterBase::FallEnter()
{
	// �̗͂����炷
	m_charData.lifePoint--;

	// �L�����N�^�[�̗̑͂��p�����[�^�ɑ���
	m_pMainParam->SetCharLife(m_charData.charNumber, m_charData.lifePoint);

	// �L�����N�^�[���_���[�W���󂯂����𑗂�
	m_pMainParam->SetIsCharDamage(m_charData.charNumber, true);

	// ����SE�𗬂�
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Drop)]);

	// �U���J�n
	Pad::StartingVibration(kPadVibrationStrength, kPadVibrationTime, m_charData.charNumber);
}

void CharacterBase::FallUpdate()
{
	// ��������
	Fall();
}

void CharacterBase::FallExit()
{
	if (m_charData.lifePoint == 0)return;

	// �����֘A
	{
		// �����_���ȕ����𓾂�
		RandomDirection();
	}

	// ���W�֘A
	{
		// �X�|�[���ʒu�擾
		m_pos = m_pField->GetSpawnPos();
	}

	// �����֘A
	{
		// �������̃Z�����擾
		m_invincibleInfo.resurrectionCell = m_pField->GetConvertPosToCell(m_pos);

		// ���G��Ԃɂ���
		m_invincibleInfo.isInvincible = true;

		// ���G���Ԃ���
		m_invincibleInfo.invincibleFrame = m_charData.invincibleTime;

		if (m_charData.lifePoint != 0)
		{
			// �G�t�F�N�g�`��
			m_pEffectManager->CreateResurrectionEffect(m_pos, m_charData.color, m_charData.charNumber);
		}

		// ����SE�𗬂�
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Resurrection)]);
	}

	// �R���g���[���[�̐U�����~�߂�
	Pad::StopVibration(m_charData.charNumber);
}

void CharacterBase::DeathEnter()
{
	// �����Ă���v���C���[�𐔂��A�c��v���C���[��1�l�̏ꍇ�A�Q�[�����I������
	m_pCharacterManager->WinnerDecision();

	// �����J�E���g�𑝂₷
	m_pMainParam->BoardDropCount();
}

void CharacterBase::WinEnter()
{
	// ���G�����false�ɂ���
	m_invincibleInfo.isInvincible = false;

	// �Q�[���I���t���[������
	m_gameEndFrame = kGameEndMaxFrame;

	// �\��ύX
	ModelData::getInstance().ChangeFace(ModelData::Face::Smile, m_charData.charNumber);

	// ����SE�𗬂�
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Win)]);
}

void CharacterBase::WinUpdate()
{
	// �Q�[���I���t���[�������炷
	m_gameEndFrame--;

	// �������[�V����
	WinMotion();

	// �Q�[���I���t���[����0�ȉ��̎��A�V�[���ύX�t���O�𗧂Ă�
	if (m_gameEndFrame <= 0)
	{
		m_gameEndFrame = 0;

		// �I���t���O�𑗂�
		m_pCharacterManager->GameEnd();
	}
}

bool CharacterBase::GetCharAlive()
{
	// ���C�t��0�ȉ��̏ꍇ�Afalse��Ԃ�
	if (m_state != State::Death)
	{
		return true;
	}

	return false;
}

void CharacterBase::SetStateChange(const State& state)
{
	// �X�e�[�g���
	m_state = state;

	// �X�e�[�g�Z�b�g
	m_pStateMachine.SetState(m_state);
}

void CharacterBase::Attack()
{
	// �Ֆʂ𗎂Ƃ�
	m_pField->AttackFallCollision(m_pos, m_directionInfo.direction.current, m_charData.charNumber);

	// �U�����[�V�����C���^�[�o������
	m_attackMotionInterval = m_charData.attackRigorTime;

	// �U�������ɂ���
	SetStateChange(State::Attack);
}

void CharacterBase::AttackInterval()
{
	// �U���C���^�[�o�������炷
	m_attackMotionInterval--;

	// �����蔻��
	Collision();

	// �U�����[�V�����C���^�[�o����0�ȉ��̏ꍇ�A0�ɂ���
	if (m_attackMotionInterval <= 0)
	{
		m_attackMotionInterval = 0;
	}

	// �U�����[�V�����C���^�[�o����0�̏ꍇ�A�ʏ폈���ɕύX
	if (m_attackMotionInterval == 0)
	{
		// �ʏ폈���ɂ���
		SetStateChange(State::Normal);
	}
}

void CharacterBase::Collision()
{
	// �Ֆʂ̓����蔻��
	BoardCollision();
}


void CharacterBase::BoardCollision()
{
	// �x�N�g���̊|���Z
	const VECTOR velocity = VScale(m_vec, m_charData.moveSpeed);

	// �Ֆʂ̃Z���ɗ����Ă��邩�ǂ����𔻒�
	{
		// �ړ��ʂ𑫂�����̍��W
		VECTOR afterPos = VAdd(m_pos, velocity);


		// �ړ��O�ɂ���A�Ֆʂ̃Z�����������Ă��邩�ǂ������擾
		bool isBoardFall = m_pField->GetIsBoardCellFall(m_pos);
		// �ړ���A�Ֆʂ̒n�ʂɂ��邩�ǂ������擾
		bool isBoardGround = m_pField->GetIsBoardCellGround(afterPos);

		// ���G���Ȃ�΁A�ړ��O�ɂ���A�Ֆʂ̃Z���̗��������false�ɂ���
		if (m_invincibleInfo.isInvincible)
		{
			isBoardFall = false;
			
			// �͈͓����ǂ���
			isBoardGround = m_pField->GetIsBoardCellRange(afterPos);
		}


		// �Ֆʂ̒n�ʂɂ��Ȃ��̂Ȃ珈�����I������
		if (!isBoardGround)
		{
			// �Ֆʂ̃Z�����������Ă�����Fall�X�e�[�g�ɕύX���A�������I������
			if (isBoardFall)
			{
				// �X�e�[�g�Z�b�g
				SetStateChange(State::Fall);

				return;
			}
			return;
		}
	}

	// �x�N�g���̑����Z
	m_pos = VAdd(m_pos, velocity);
}

void CharacterBase::Fall()
{
	// ����������
	m_pos.y -= m_pField->GetFallSpeed();

	// ������x�̈ʒu�܂ŗ������ꍇ�A���̈ʒu�ɖ߂�
	if (m_pos.y <= m_pField->GetFallLowerLimitPos())
	{
		// �̗͂�0�ȉ��̏ꍇ�A���S��Ԃɂ���
		if (m_charData.lifePoint <= 0 && !m_isWin)
		{
			// ���S�����ɂ���
			SetStateChange(State::Death);

			return;
		}

		// �ʏ폈���ɂ���
		SetStateChange(State::Normal);
	}
}

std::vector<VECTOR> CharacterBase::GetAlivePos()
{
	// �����Ă���L�����̍��W
	const std::vector<VECTOR> alivePos = m_pCharacterManager->AlivePos(m_charData.charNumber);

	// �����Ă�����W��Ԃ�
	return alivePos;
}

void CharacterBase::Invincible()
{
	// ���G���ł͂Ȃ��ꍇ�A�����ŏ������I������
	if (!m_invincibleInfo.isInvincible)
	{
		return;
	}

	// ���݂̍��W����Z�����擾
	Field::Cell currentCell = m_pField->GetConvertPosToCell(m_pos);


	// ���݂̃Z���ƕ������̃Z���������łȂ���΁A���G���Ԃ��I��点��
	if (m_invincibleInfo.resurrectionCell.x != currentCell.x ||
		m_invincibleInfo.resurrectionCell.z != currentCell.z)
	{
		// ���G���Ԃ�0�ɂ���
		m_invincibleInfo.invincibleFrame = 0;

		// ���G�����false�ɂ���
		m_invincibleInfo.isInvincible = false;

		return;
	}

	// ���G���Ԃ����炷
	m_invincibleInfo.invincibleFrame--;

	// ���G���Ԃ�0�ȉ��̏ꍇ�A���G�����false�ɂ���
	if (m_invincibleInfo.invincibleFrame <= 0)
	{
		// ���G���Ԃ�0�ɂ���
		m_invincibleInfo.invincibleFrame = 0;

		// ���G�����false�ɂ���
		m_invincibleInfo.isInvincible = false;
	}
}

void CharacterBase::ModelAngle()
{

	
	// 1�t���[���O�ƌ������Ⴄ�ꍇ�A�����]�����s��
	if (m_directionInfo.direction.current != m_directionInfo.direction.before)
	{
		// �����]���O
		m_directionInfo.changeDirection.before = m_directionInfo.direction.before;

		// �����]����
		m_directionInfo.changeDirection.after = m_directionInfo.direction.current;

		// �����]���t���O��true�ɂ���
		m_directionInfo.isChangeDirection = true;
	}

	// ��������
	m_directionInfo.direction.before = m_directionInfo.direction.current;


	
	
	// ���݌����Ă�������ɂ���ăt���O��ύX����
	switch (m_directionInfo.direction.current)		
	{
	case Direction::Top:

		m_directionInfo.isTopDirection = true;

		break;
	case Direction::Bottom:

		m_directionInfo.isTopDirection = false;

		break;
	case Direction::Left:

		m_directionInfo.isRightDirection = false;

		break;
	case Direction::Right:

		m_directionInfo.isRightDirection = true;

		break;
	default:
		break;
	}

	// �����]�����łȂ��ꍇ�����ŏ������I������
	if (!m_directionInfo.isChangeDirection)
	{
		return;
	}

	// �������ǂ���
	bool isAdd = false;

	// �]���O�̊p�x
	int beforeAngle = 
		static_cast<int>(Convert_Lib::ConvertDirectionToAngle(m_directionInfo.changeDirection.before));

	// �]����̊p�x
	int afterAngle = 
		static_cast<int>(Convert_Lib::ConvertDirectionToAngle(m_directionInfo.changeDirection.after));


	
	// �x����0�ł͐��藧���Ȃ��ꍇ������̂�360��������
	if (m_directionInfo.changeDirection.after == Direction::Top && m_directionInfo.changeDirection.before == Direction::Right)
	{
		beforeAngle = 360;
	}
	else if (m_directionInfo.changeDirection.before == Direction::Top && m_directionInfo.changeDirection.after == Direction::Right)
	{
		afterAngle = 360;
	}


	
	// ��]��
	const int angleValue = afterAngle - beforeAngle;

	// ���̒l���ǂ���
	const bool isRegularValue = (angleValue > 0);


	// ��]�ʂ����̒l�Ȃ���Z�t���O��true�ɂ���
	if (isRegularValue)
	{
		isAdd = true;
	}

	// 180�x�ȏ��]����ꍇ�A�����𒲂ׂ�
	if (angleValue == 180 || angleValue == -180)
	{
		isAdd = true;

		if (m_directionInfo.changeDirection.after == Direction::Top||
			m_directionInfo.changeDirection.after == Direction::Bottom)
		{
			if (m_directionInfo.isRightDirection && isRegularValue ||
				!m_directionInfo.isRightDirection && !isRegularValue)
			{
				isAdd = false;
			}
		}
		else
		{
			if (m_directionInfo.isTopDirection && isRegularValue ||
				!m_directionInfo.isTopDirection && !isRegularValue)
			{
				isAdd = false;
			}
		}
	}
	


	// ���Z�t���O�ɂ���Ċp�x�������Z����
	if (isAdd)
	{
		// �p�x��ύX
		m_directionInfo.modelAngle =
			static_cast<int>(Calculation_Lib::AngleLimitValueNotExceedToCalculate(static_cast<float>(m_directionInfo.modelAngle), static_cast<float>(kChangeDirectionSpeed), Calculation_Lib::Calculation::Addition, true, false));
	}
	else
	{
		// �p�x��ύX
		m_directionInfo.modelAngle =
			static_cast<int>(Calculation_Lib::AngleLimitValueNotExceedToCalculate(static_cast<float>(m_directionInfo.modelAngle), static_cast<float>(kChangeDirectionSpeed), Calculation_Lib::Calculation::Subtraction, true, false));
	}



	// �p�x�̒l��␳
	if (Calculation_Lib::IsTargetRangeValue(static_cast<float>(m_directionInfo.modelAngle), static_cast<float>(afterAngle), static_cast<float>(kChangeDirectionSpeed)))
	{
		m_directionInfo.modelAngle = afterAngle;

		m_directionInfo.isChangeDirection = false;
	}
}

void CharacterBase::DrawModel()
{
	// ���S��Ԃ̏ꍇ�A���f����`�悵�Ȃ�
	if (m_state == State::Death)
	{
		return;
	}

	// ���f���̉�]
	ModelAngle();


	// ���f���̕`��
	ModelData::DrawModelData drawModelData;

	// ���f���`�������
	drawModelData.playerNumber = m_charData.charNumber;
	drawModelData.angle = m_directionInfo.modelAngle;
	drawModelData.pos = m_pos;

	// ���G���Ԓ��Ȃ�΁A�_�ł�����
	if (m_invincibleInfo.isInvincible)
	{
		// ���G���Ԃ������̏ꍇ�ɕ`��
		if (m_invincibleInfo.invincibleFrame % 2 == 0)
		{
			ModelData::getInstance().DrawModel(drawModelData);
		}
		return;
	}
	
	// ���f���̕`��
	ModelData::getInstance().DrawModel(drawModelData);
}

void CharacterBase::RandomDirection()
{
	// ������
	const int directionNum = (static_cast<int> (Direction::DirectionNum) - 1);

	// �����_���Ȑ��𓾂�
	const int randomNum = GetRand(directionNum);


	// ��������
	m_directionInfo.direction.current = Direction(randomNum);
	m_directionInfo.direction.before = m_directionInfo.direction.current;


	// ���f���̊p�x
	m_directionInfo.modelAngle = static_cast<int>(Convert_Lib::ConvertDirectionToAngle(m_directionInfo.direction.current));
}

void CharacterBase::WinMotion()
{
	// �������[�V�����p�x��ύX
	m_directionInfo.modelAngle =
		static_cast<int>(Calculation_Lib::AngleLimitValueNotExceedToCalculate(static_cast<float>(m_directionInfo.modelAngle), kWinMotionSpeed, Calculation_Lib::Calculation::Addition, true));
}