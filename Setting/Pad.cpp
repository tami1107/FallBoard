#include "DxLib.h"
#include "Pad.h"
#include <vector>
#include "EvoLib.h"

namespace
{
	// ���O�L�^�t���[����
	constexpr int kLogNum = 16;
	// �p�b�h�ő吔
	constexpr int kMaxPad = 4;
	// �g�p����p�b�h��
	constexpr int kUsePadData[kMaxPad] =
	{
		DX_INPUT_KEY_PAD1,
		DX_INPUT_PAD2,
		DX_INPUT_PAD3,
		DX_INPUT_PAD4,
	};

	// ���̓��O	0���ŐV�̏��
	int padLog[kMaxPad][kLogNum];
}

namespace Pad
{
	// �p�b�h�̓��͏�Ԏ擾
	void Update()
	{
		for (int padNo = 0; padNo < kMaxPad; padNo++)
		{
			// ���݂̃p�b�h�̏�Ԃ��擾
			int padState = GetJoypadInputState(kUsePadData[padNo]);

			// ���O�̍X�V
			for (int i = kLogNum - 1; i >= 1; i--)
			{
				padLog[padNo][i] = padLog[padNo][i - 1];
			}

			
			// �ŐV�̏��
			padLog[padNo][0] = padState;
		}
	}

	// ������������
	bool IsPress(int button, int padNo)
	{
		return (padLog[padNo][0] & button);
	}
	// �g���K�[����
	bool IsTrigger(int button, int padNo)
	{
		bool isNow = (padLog[padNo][0] & button);	// ���݂̏��
		bool isLast = (padLog[padNo][1] & button);	// �P�t���[���O�̏��
		return (isNow && !isLast);
	}

	// �U��
	void StartingVibration(const int& power, float frameTime, int padNo)
	{
		// �~���b�P�ʂ����߂�
		int millisecondUnit = Convert_Lib::ConvertingFrameCountToMillisecondUnit(static_cast<int>(frameTime));

		// �t���[���^�C����-1.0f�Ȃ�΃~���b�P�ʂ�-1�ɂ���
		if (frameTime == -1.0f)
		{
			millisecondUnit = -1;
		}

		// �U�����s��
		StartJoypadVibration(kUsePadData[padNo], power, millisecondUnit, -1);
	}

	// �U�����~�߂�
	void StopVibration(int padNo)
	{
		// �p�b�h�̐U�����~�߂�
		StopJoypadVibration(kUsePadData[padNo], -1);
	}
}