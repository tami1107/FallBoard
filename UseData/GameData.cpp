#include "GameData.h"
#include "ModelData.h"
#include "MasterData.h"

// �V���O���g��
GameData* GameData::m_instance = nullptr;


void GameData::SetCharData(const int& playNum, const Level& level)
{
	// ������
	m_characterStatus.clear();

	// �L�����N�^�[�X�e�[�^�X���擾����
	const MasterData::FileReadCharcterStatus charcterStatus = MasterData::getInstance().GetCharcterStatus();

	// �ő�̗�
	{
		// �ő�̗�
		m_maxLifePoint = charcterStatus.lifePoint;

		// ���C�t�|�C���g��0�ȉ��̏ꍇ�A1�ɕύX����
		if (m_maxLifePoint <= 0)
		{
			m_maxLifePoint = 1;
		}
		// ���C�t�|�C���g��6�ȏ�̏ꍇ�A6�ɕύX����
		else if(m_maxLifePoint > 6)
		{
			m_maxLifePoint = 6;
		}
	}

	// ��Փx�擾
	m_level = level;

	// �ꎞ�ۑ��L�����f�[�^
	CharacterStatus saveCharacterStatus;

	// �f�[�^����
	for (int i = 0; i < kPlayerMaxCount; i++)
	{

		// �v���C���[���ǂ����𔻒�
		if (playNum > i)
		{
			saveCharacterStatus.isPlayer = true;
		}
		else
		{
			saveCharacterStatus.isPlayer = false;
		}

		// �L�����N�^�[�ԍ�����
		saveCharacterStatus.charNumber = i;

		// �L�����N�^�[�J���[����
		saveCharacterStatus.color = charcterStatus.color[i];

		// �ő�̗͂���
		saveCharacterStatus.lifePoint = m_maxLifePoint;

		// �ړ����x����
		saveCharacterStatus.moveSpeed = charcterStatus.moveSpeed;

		// �U���d�����Ԃ���
		saveCharacterStatus.attackRigorTime = charcterStatus.attackRigorTime;

		// ���A���̖��G���Ԃ���
		saveCharacterStatus.invincibleTime = charcterStatus.invincibleTime;

		// �����L���O����
		saveCharacterStatus.rank = 1;

		// m_characterStatus�̗v�f�𑝂₵�A���
		m_characterStatus.push_back(saveCharacterStatus);
	}
}

void GameData::SetCharRank(const int& charNumber, const int& rank)
{
	// �����L���O������
	m_characterStatus[charNumber].rank = rank;
}


