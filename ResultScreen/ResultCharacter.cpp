#include "ResultCharacter.h"
#include "GameData.h"
#include "Game.h"
#include "EvoLib.h"
#include "Vec2.h"
#include "ModelData.h"

namespace
{
	// ���W�̕�
	const Vec2 kDistancePos = Vec2(15.0f, 0.0);
	
	// �����L���O���W
	constexpr float kRankAdjustmentPos_Z = 8.0f;

	// �������f���p�x
	constexpr int kInitModelAngle = 90;

	// ���f����]�X�s�[�h
	constexpr int kRotationSpeed = 3;
}

ResultCharacter::ResultCharacter():
	// ���f���֘A��
	m_pos(),
	m_modelData(),
	// ���[�V�����֘A��
	m_angle()
{
}

ResultCharacter::~ResultCharacter()
{
}

void ResultCharacter::Init()
{
	// �������W
	InitPos();

	// ���f�������p�x����
	m_angle = kInitModelAngle;
}

void ResultCharacter::Update()
{
	// ���[�V�����X�V����
	UpdateMotion();
}

void ResultCharacter::Draw()
{
	// ���f���`�揈��
	DrawModel();
}

void ResultCharacter::InitPos()
{
	// �v���C�l�����擾
	const int playCount = GameData::getInstance().GetGamePlayCount();

	// �v���C�l�����A�v�f�𑝂₷
	m_pos.resize(playCount);

	// �����l���������W����
	const std::vector<Vec2> distancePos = Calculation_Lib::SortCoordinateEqually(playCount, Vec2(0.0f, 0.0f), kDistancePos);

	// ���W�̑��
	for (int i = 0; i < playCount; i++)
	{
		// ���߂����W��������
		m_pos[i] = VGet(distancePos[i].x, 0.0f, 0.0f);
	}

	// �v���C�l�����A�v�f�𑝂₷
	m_modelData.resize(playCount);

	// �����L���O
	int rank = 0;

	// �����L���O�̍��W
	float rankPos_z = 0.0f;

	for (int i = 0; i < static_cast<int>(m_pos.size()); i++)
	{
		// ���f���̌�������
		m_modelData[i].angle = kInitModelAngle;

		// ���f���̔ԍ�����
		m_modelData[i].playerNumber = i;

		// ���W����
		m_modelData[i].pos = m_pos[i];

		// �����L���O�擾
		rank = (GameData::getInstance().GetCharRank(i));
		
		// ���f����Z���W�����߂�
		m_modelData[i].pos.z += (kRankAdjustmentPos_Z * (rank -1));

		// ���W��������
		m_pos[i] = m_modelData[i].pos;

	

		// �t�F�C�X�ϐ�
		ModelData::Face face = ModelData::Face::Sulking;

		// �����N�ɂ���ăt�F�C�X��ύX����
		switch (rank)
		{
		case 1:
			// Smile����
			face = ModelData::Face::Smile;

			break;
		
		case 3:
			// Sulking����
			face = ModelData::Face::Sulking;

			break;
		case 4:
			// Cry����
			face = ModelData::Face::Cry;
			break;

		default:
			break;
		}

		// �����N��2�ȊO�Ȃ�Ε\���ύX����
		if (rank != 2)
		{
			// �\��ύX
			ModelData::getInstance().ChangeFace(face, i);
		}
	}
}

void ResultCharacter::UpdateMotion()
{
	// m_angle��kRotationSpeed���A���Z����
	m_angle += kRotationSpeed;

	// �l��360�𒴂����ꍇ�A0��������
	if (m_angle >= 360)
	{
		m_angle = 0;
	}
}

void ResultCharacter::DrawModel()
{
	// ���f���f�[�^�T�C�Y��������
	const int num = static_cast<int>(m_modelData.size());

	// �����L���O������ϐ�
	int rank = 0;

	// ���f����`�悷��
	for (int i = 0; i < num; i++)
	{
		// �����L���O�擾
		rank = GameData::getInstance().GetCharRank(i);

		// �����N�̒l��1�̏ꍇ�A���f���f�[�^�̃A���O���ɉ��Z���ꂽ�A���O����������
		if (rank == 1)
		{
			// �A���O����������
			m_modelData[i].angle = m_angle;
		}
	
		// ���f���`��
		ModelData::getInstance().DrawModel(m_modelData[i]);
	}
}
