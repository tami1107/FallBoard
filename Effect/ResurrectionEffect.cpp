#include "StateMachine.h"// ����ԏ�ɏ���
#include "ResurrectionEffect.h"
#include "CharacterBase.h"

namespace
{
	// �������W
	constexpr float kInitPos_Y = -1.0f;

	// �����p�x
	const VECTOR kInitAngle = VGet(90.0f, 0.0f, 0.0f);
}

ResurrectionEffect::ResurrectionEffect():
	// �G�t�F�N�g�֘A��
	m_effectInfo(),
	m_isDelete(),
	// �N���X�|�C���^�֘A��
	m_pField(),
	m_pCharacterBase()
{
}

ResurrectionEffect::~ResurrectionEffect()
{
}

void ResurrectionEffect::Update()
{
	// ���G���Ԓ��łȂ���΍폜�t���O��ture�ɂ���
	if (!m_pCharacterBase->GetIsInvincible())
	{
		// �폜�t���O��true�ɂ���
		m_isDelete = true;
	}
}

void ResurrectionEffect::Draw()
{
	// �ʏ�`��
	{
		// �O���t�B�b�N�`��
		Draw_Lib::DrawGprah3D_EvoLib(m_effectInfo.handle, m_effectInfo.scale, kInitAngle, m_effectInfo.pos,
			Draw_Lib::Color(m_effectInfo.color.x, m_effectInfo.color.y, m_effectInfo.color.z));
	}
}

void ResurrectionEffect::Init(const EffectSetting& effectSetting)
{
	// �G�t�F�N�g������
	m_effectInfo = effectSetting;

	// �ϊ����W
	VECTOR conversionPos = m_effectInfo.pos;

	// ���W���
	m_effectInfo.pos.y = conversionPos.z;
	m_effectInfo.pos.z = kInitPos_Y;

	// �폜�t���O�̏�����
	m_isDelete = false;
}
