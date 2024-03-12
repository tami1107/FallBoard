#include "AttackEffect.h"
#include "Field.h"

namespace
{
	// ����Y���W
	constexpr float kInitPos_Y = -4.0f;

	// �����p�x
	const VECTOR kInitAngle = VGet(90.0f, 0.0f, 0.0f);

	// �ړ����x�����l
	constexpr float kMoveSpeedAdjustmentValue = 0.8f;

	// �c����
	constexpr int kAfterEffectValue = 15;

	// �c���̓����x
	constexpr int kAfterEffectAlphaValue = 20;
}


AttackEffect::AttackEffect():
	// �G�t�F�N�g�֘A��
	m_effectInfo(),
	m_isDelete(),
	m_afterPos(),
	// �N���X�|�C���^�֘A��
	m_pField()
{
}

AttackEffect::~AttackEffect()
{
}

void AttackEffect::Update()
{
	// �ړ�����
	Move();
}

void AttackEffect::Draw()
{
	// �ʏ�`��
	{
		Draw_Lib::DrawGprah3D_EvoLib(m_effectInfo.handle, m_effectInfo.graphScale, VGet(kInitAngle.x, m_effectInfo.angle, 0.0f), m_afterPos[0],
			Draw_Lib::Color(m_effectInfo.color.x, m_effectInfo.color.y, m_effectInfo.color.z));
	}

	// �c���`��
	DrawAfterEffect();
}

void AttackEffect::Move()
{

	// ���p�ɂ���Ĉړ�������W��ς���
	switch (m_effectInfo.directon)
	{
	case Direction::Top:

		// ������ɐi��
		m_effectInfo.pos.z += m_effectInfo.moveSpeed;

		break;

	case Direction::Bottom:

		// �������ɐi��
		m_effectInfo.pos.z -= m_effectInfo.moveSpeed;

		break;

	case Direction::Left:

		// �������ɐi��
		m_effectInfo.pos.x -= m_effectInfo.moveSpeed;

		break;

	case Direction::Right:

		// �E�����ɐi��
		m_effectInfo.pos.x += m_effectInfo.moveSpeed;

		break;

	default:
		break;
	}


	// ���W���t�B�[���h�O�̂Ƃ��A�폜�t���O��true�ɂ���
	{
		// ���݂̍��W���t�B�[���h�͈̔͊O�̏ꍇ�A�폜�t���O��true�ɂ���
		if (!m_pField->GetIsBoardCurrentCellRange(m_effectInfo.pos))
		{
			// �t���O��true�ɂ���
			m_isDelete = true;
		}
	}

	// ���W�ϊ�
	ConvertPos();


	// �c���̍��W�f�[�^����Â��炷
	for (int i = kAfterEffectValue - 1; i > 0; i--)
	{
		m_afterPos[i] = m_afterPos[i - 1];
	}
}

void AttackEffect::ConvertPos()
{
	// �ϊ����W
	VECTOR conversionPos;

	// �ϊ����W����
	conversionPos.x = m_effectInfo.pos.x;
	conversionPos.y = m_effectInfo.pos.z;


	// �i�ޕ��p�ɂ���ĕϊ����W�ɑ������l��ύX����
	switch (m_effectInfo.directon)
	{
	case Direction::Top:

		// �ϊ����W����
		conversionPos.x = m_effectInfo.pos.z;
		conversionPos.y = -m_effectInfo.pos.x;

		break;

	case Direction::Bottom:

		// �ϊ����W����
		conversionPos.x = -m_effectInfo.pos.z;
		conversionPos.y = m_effectInfo.pos.x;

		break;

	case Direction::Left:

		// �ϊ����W����
		conversionPos.x = -m_effectInfo.pos.x;
		conversionPos.y = -m_effectInfo.pos.z;

		break;

	case Direction::Right:

		// �ϊ����W����
		conversionPos.x = m_effectInfo.pos.x;
		conversionPos.y = m_effectInfo.pos.z;

		break;

	default:
		break;
	}

	// �ϊ����W����
	conversionPos.z = m_effectInfo.pos.y;

	// �ϊ��������W����
	m_afterPos[0] = conversionPos;
}

void AttackEffect::DrawAfterEffect()
{
	// �����ɂ��ĕ\������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAfterEffectAlphaValue);

	// �c����`��
	for (int i = kAfterEffectValue - 1; i >= 0; i -= 1)
	{
		Draw_Lib::DrawGprah3D_EvoLib(m_effectInfo.handle, m_effectInfo.graphScale, VGet(kInitAngle.x, m_effectInfo.angle, 0.0f), m_afterPos[i],
			Draw_Lib::Color(m_effectInfo.color.x, m_effectInfo.color.y, m_effectInfo.color.z));
	}

	// �`���ʏ�`��ɂ���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void AttackEffect::Init(const EffectSetting& effectSetting)
{
	
	// �G�t�F�N�g���
	{
		// �G�t�F�N�g�����
		m_effectInfo = effectSetting;

		// ���W���
		m_effectInfo.pos = m_pField->GetFieldCellCenterPos(m_effectInfo.pos);

		// �����𒲐�����
		m_effectInfo.pos.y = kInitPos_Y;

		// �p�x
		m_effectInfo.angle = Convert_Lib::ConvertDirectionToAngle(m_effectInfo.directon);

		// �ړ��X�s�[�h���
		m_effectInfo.moveSpeed = static_cast<float>(m_pField->GetDeleteFallColorInterval());

		// �X�s�[�h����
		m_effectInfo.moveSpeed *= kMoveSpeedAdjustmentValue;

		// �폜�t���O�̏�����
		m_isDelete = false;
	}

	// ���W����
	{
		// �t�B�[���h���f���T�C�Y�擾
		float posAdjustment = m_pField->GetFieldCellModelScale();


		// ���W�𒲐�����
		switch (m_effectInfo.directon)
		{
		case Direction::Top:

			// �������W�̑��
			m_effectInfo.pos.z += posAdjustment;

			break;

		case Direction::Bottom:

			// �������W�̑��
			m_effectInfo.pos.z -= posAdjustment;
			
			break;

		case Direction::Left:

			// �������W�̑��
			m_effectInfo.pos.x -= posAdjustment;

			break;

		case Direction::Right:

			// �������W�̑��
			m_effectInfo.pos.x += posAdjustment;

			break;

		default:
			break;
		}
	}


	// �v�f�𑝂₷
	m_afterPos.resize(kAfterEffectValue);

	// ���W�ϊ�����
	ConvertPos();

	// �c�����W�̏�����
	for (auto& pos : m_afterPos)
	{
		// ���W���
		pos = m_afterPos.front();
	}
}