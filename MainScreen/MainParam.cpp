#include "MainParam.h"
#include <DxLib.h>
#include "EvoLib.h"
#include "GameData.h"


namespace
{
	// �J�E���g�_�E���b��
	constexpr int kCountDownSecond = 4;

	// �J�E���g�_�E���t���[��(0�b�����ꂽ���̂� +1 ���s��)
	constexpr int kCountDownFrame = FPS_Lib::FPS * (kCountDownSecond + 1);
}

MainParam::MainParam() :
	// �o��֘A��
	m_pop(),
	// �L�����p�����[�^�֘A��
	m_charInfo(),
	// �Ֆʊ֘A��
	m_board(),
	// �X�e�[�g�֘A��
	m_state()
{
}

MainParam::~MainParam()
{
}

void MainParam::Init()
{
	// �J�E���g�_�E���̃t���[������
	m_pop.countDownFrame = kCountDownFrame;

	// �v���C�l�����v�f�𑝂₷
	m_charInfo.resize(GameData::getInstance().GetGamePlayCount());
}

int MainParam::CountDown()
{
	// �J�E���g�_�E���t���[�������炷
	m_pop.countDownFrame--;

	// �J�E���g�_�E���t���[����0�ȉ��̏ꍇ�A�����ŏ������I������B
	if (m_pop.countDownFrame <= 0)
	{
		m_pop.countDownFrame = 0;
	}

	// �t���[����Ԃ�
	return m_pop.countDownFrame;
}

bool MainParam::GetIsCountDownEnd()
{
	// FPS����
	const int fps = FPS_Lib::FPS;

	// �Q�[���X�^�[�g�t���[��
	const int gameStartFrame = static_cast<int>(fps * 0.5f);

	// �J�E���g�_�E���̃t���[�����Q�[���X�^�[�g�t���[���ȉ��̏ꍇ�A�J�E���g�_�E���I���Ƃ݂Ȃ�
	if (m_pop.countDownFrame <= gameStartFrame)
	{
		return true;
	}

	// �t���O��Ԃ�
	return false;
}

void MainParam::SetCharLife(const int& charNumber, const int& lifePoint)
{
	// �̗͂̑��
	m_charInfo[charNumber].lifePoint = lifePoint;


	// �̗͂�0�ȉ��̏ꍇ�A���S����
	if (m_charInfo[charNumber].lifePoint <= 0)
	{
		// ���ʌv�Z
		const int rank = RankCalculation();

		// �L�����̏��ʂ𑗂�
		GameData::getInstance().SetCharRank(charNumber, rank);

		// ���S�����true�ɂ���
		m_charInfo[charNumber].isDie = true;
	}
}

void MainParam::SetIsCharDamage(const int& charNumber, const bool& isDamage)
{
	// �_���[�W�t���O����
	 m_charInfo[charNumber].isDamage = isDamage; 
}


void MainParam::BoardDropCount()
{
	// ����������񐔂𑝂₷
	m_board.dropCount++;
}

void MainParam::BoardDropEnd()
{
	// ����������񐔂����炷
	m_board.dropCount--;

	// ����������񐔂�0�ȉ��̏ꍇ�A0�ɂ���
	if (m_board.dropCount <= 0)
	{
		m_board.dropCount = 0;
	}
}

int MainParam::RankCalculation()
{
	// ����ł���l��
	int dieCount = 0;

	// �v���C���[�l��
	const int playCount = static_cast<int>(m_charInfo.size());

	// ����ł���l���𐔂���
	for (int i = 0; i < playCount; i++)
	{
		if (m_charInfo[i].isDie)
		{
			dieCount++;
		}
	}

	// �v���C�l�����玀��ł���l���������ă����L���O�����߂�
	const int rank = playCount - dieCount;

	// �����N��Ԃ�
	return rank;
}