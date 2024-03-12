#pragma once
#include "Pad.h"
#include "ModelData.h"
#include <DxLib.h>
#include <vector>


// �V���O���g�����g�p�����Q�[���f�[�^�N���X
class GameData
{
public:

	// �L�����N�^�[�X�e�[�^�X
	struct CharacterStatus
	{
		// �v���C���[���ǂ���
		bool isPlayer = true;

		// �L�����N�^�[�ԍ�
		int charNumber = 0;

		// �F
		VECTOR color = VGet(0.0f, 0.0f, 0.0f);

		// �̗�
		int lifePoint = 0;

		// �ړ����x
		float moveSpeed = 0;

		// �U���d������
		int attackRigorTime = 0;

		// ���A���̖��G����
		int invincibleTime = 0;

		// �����L���O
		int rank = 0;
	};


protected:

	// �g�p���f���ԍ�
	static constexpr int kUseModelNumber = 1;

	// �v���C���[�̍ő吔
	static constexpr int kPlayerMaxCount = 4;

private:
	
	GameData() = default;

	virtual ~GameData() = default;

	static GameData* m_instance;

public:

	// �R�s�[�R���X�g���N�^�̋֎~
	GameData(const GameData&) = delete;
	GameData& operator=(const GameData&) = delete;
	GameData(GameData&&) = delete;
	GameData& operator=(GameData&&) = delete;

	static GameData& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new GameData;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	// ��Փx
	enum class Level
	{
		Easy,	// �ȒP
		Noramal,// ����
		Hard,	// ���
	};

public:

	/// <summary>
	/// �L�����f�[�^���Z�b�g����
	/// </summary>
	/// <param name="playerNum">�v���C���[�̐l��</param>
	/// <param name="level">��Փx</param>
	void SetCharData(const int& playerNum, const Level& level);

	/// <summary>
	/// �L�����f�[�^��Ԃ�
	/// </summary>
	/// <returns>�L�����f�[�^</returns>
	std::vector<CharacterStatus> GetCharData() { return m_characterStatus; }

	/// <summary>
	/// �Q�[���v���C�l����Ԃ�
	/// </summary>
	/// <returns>�Q�[���v���C�l��</returns>
	int GetGamePlayCount() { return kPlayerMaxCount; }

	/// <summary>
	/// �L�����̍ő�̗͂�Ԃ�
	/// </summary>
	/// <returns></returns>
	int GetMaxLifePoint() { return m_maxLifePoint; }

	/// <summary>
	/// �g�p���郂�f���̔ԍ���Ԃ�
	/// </summary>
	/// <returns>�g�p���郂�f���̔ԍ�</returns>
	int GetModelNumber() { return kUseModelNumber; }

	/// <summary>
	/// �v���C���[�J���[��Ԃ�
	/// </summary>
	/// <param name="charNumber">�L�����N�^�[�i���o�[</param>
	/// <returns>�v���C���[�J���[</returns>
	VECTOR GetPlayerColor(const int& charNumber){ return m_characterStatus[charNumber].color; }


	/// <summary>
	/// ���ʂ��󂯎��
	/// </summary>
	/// <param name="charNumber">�L�����ԍ�</param>
	/// <param name="rank">����/param>
	void SetCharRank(const int& charNumber, const int& rank);

	/// <summary>
	/// �L�����N�^�[�̃����N���擾
	/// </summary>
	/// <param name="charNumber">�L�����̔ԍ�</param>
	/// <returns></returns>
	int GetCharRank(const int& charNumber) { return m_characterStatus[charNumber].rank; }

	/// <summary>
	/// ��Փx��Ԃ�
	/// </summary>
	/// <returns>��Փx</returns>
	Level GetLevel() { return m_level; }

private:

	// �ő�̗�
	int m_maxLifePoint = 0;

	// ��Փx
	Level m_level = Level::Easy;

	// �L�����N�^�[�X�e�[�^�X
	std::vector<CharacterStatus>m_characterStatus;
};

