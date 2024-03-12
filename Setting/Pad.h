#pragma once

namespace Pad
{
	// �p�b�h�̓��͏�Ԏ擾
	void Update();

	/// <summary>
	/// ������������
	/// </summary>
	/// <param name="button">�ǂ̃{�^����</param>
	/// <param name="padNo">�p�b�h�̔ԍ�/param>
	/// <returns>�{�^���������������Ă��邩�ǂ�����Ԃ�</returns>
	bool IsPress(int button, int padNo = 0);

	
	/// <summary>
	/// �g���K�[����
	/// </summary>
	/// <param name="button">�ǂ̃{�^����</param>
	/// <param name="padNo">�p�b�h�̔ԍ�</param>
	/// <returns>�{�^���̃g���K�[�����Ԃ�</returns>
	bool IsTrigger(int button, int padNo = 0);


	/// <summary>
	/// �p�b�h�̐U�����J�n����
	/// </summary>
	/// <param name="power">�U���̋���(0�`1000)</param>
	/// <param name="frameTime">�U������t���[����(�l��-1.0f���������ꍇ�AStopVibration()���Ăяo�����܂ŐU����������)</param>
	/// <param name="padNo">�p�b�h�̔ԍ�</param>
	void StartingVibration(const int& power, float frameTime = -1, int padNo = 0);

	/// <summary>
	/// �p�b�h�̐U�����~�߂�
	/// </summary>
	/// <param name="padNo"></param>
	void StopVibration(int padNo = 0);

}
