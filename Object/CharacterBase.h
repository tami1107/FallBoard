#pragma once

#include <memory>
#include <DxLib.h>
#include "EvoLib.h"
#include "ModelData.h"
#include "Field.h"
#include <array>
#include "GameData.h"

// �X�e�[�g�}�V��
template <class TState> class StateMachine;

class Field;
class MainParam;
class Algorithm;
class CharacterManager;
class EffectManager;
class MainUI;
class CharacterBase : public std::enable_shared_from_this<CharacterBase>
{

public:

	// �X�e�[�g
	enum class State
	{
		// �o�ꏈ��
		Pop,
		// �ʏ���
		Normal,
		// �U�����
		Attack,
		// �������
		Fall,
		// ���S���
		Death,
		// �������
		Win,
	};


protected:

	// �ړ�����
	struct MoveDirection
	{
		// �O
		Direction before = Direction::Right;

		// ����
		Direction current = Direction::Right;

		// ��
		Direction after = Direction::Right;
	};

	// �������
	struct DirectionInfo
	{
		// ����
		MoveDirection direction = MoveDirection();

		// �]������
		MoveDirection changeDirection = MoveDirection();

		// �����]�������ǂ���
		bool isChangeDirection = false;

		// �E�������Ă������ǂ���
		bool isRightDirection = false;

		// ��������Ă������ǂ���
		bool isTopDirection = false;

		// �p�x
		int modelAngle = 0;
	};

	// ���G���
	struct InvincibleInfo
	{
		// ���G���ǂ���
		bool isInvincible = false;

		// ���G����
		int invincibleFrame = 0;

		// �������̃Z��
		Field::Cell resurrectionCell = Field::Cell();
	};


public:
	CharacterBase();
	virtual ~CharacterBase();

	/// <summary>
	/// �t�B�[���h�|�C���^���擾
	/// </summary>
	/// <param name="field">�t�B�[���h�|�C���^</param>
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }

	/// <summary>
	/// ���C���p�����|�C���^���擾
	/// </summary>
	/// <param name="mainParam">���C���p�����|�C���^</param>
	void SetMainParamPointer(std::shared_ptr<MainParam>mainParam) { m_pMainParam = mainParam; }

	/// <summary>
	/// �A���S���Y���|�C���^���擾
	/// </summary>
	/// <param name="algorithm">�A���S���Y���|�C���^</param>
	void SetAlgorithmPointer(std::shared_ptr<Algorithm>algorithm) { m_pAlgorithm = algorithm; }

	/// <summary>
	/// �L�����N�^�[�}�l�[�W���[�|�C���^���擾
	/// </summary>
	/// <param name="characterManager">�L�����N�^�[�}�l�[�W���[�|�C���^</param>
	void SetCharacterManagerPointer(std::shared_ptr<CharacterManager>characterManager) { m_pCharacterManager = characterManager; }

	/// <summary>
	/// �G�t�F�N�g�}�l�[�W���[�|�C���^���擾
	/// </summary>
	/// <param name="effectManager">�G�t�F�N�g�}�l�[�W���[�|�C���^</param>
	void SetEffectManagerPointer(std::shared_ptr<EffectManager>effectManager) { m_pEffectManager = effectManager; }

	/// <summary>
	/// ���C��UI�|�C���^���擾
	/// </summary>
	/// <param name="mainUI">���C��UI�|�C���^</param>
	void SetMainUIPointer(std::shared_ptr<MainUI>mainUI) { m_pMainUI = mainUI; }

		
	virtual void Init();
	virtual void Update();
	virtual void Draw();

public:

	// �o�ꏈ��
	void PopEnter();
	void PopUpdate();

	// �ʏ폈��
	void NormalUpdate();

	// �ʏ폈��
	void AttackEnter();
	void AttackUpdate();

	// �������
	void FallEnter();
	void FallUpdate();
	void FallExit();

	// ���S���
	void DeathEnter();

	// �������
	void WinEnter();
	void WinUpdate();


	/// <summary>
	/// �L�����N�^�[�����擾
	/// </summary>
	/// <param name="charData">�L�����N�^�[���</param>
	void SetCharInfo(const GameData::CharacterStatus& charData) { m_charData = charData; }

	/// <summary>
	/// �L�����������Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns>�L�����������Ă��邩�ǂ����̃t���O</returns>
	bool GetCharAlive();

	/// <summary>
	/// ���g���v���C���[���ǂ�����Ԃ�
	/// </summary>
	/// <returns></returns>
	bool GetIsPlayer() { return m_charData.isPlayer; }

	/// <summary>
	/// �L�����N�^�[�ԍ���Ԃ�
	/// </summary>
	/// <returns>�L�����N�^�[�ԍ�</returns>
	int GetCharNumber() const { return m_charData.charNumber; }

	/// <summary>
	/// ���G���Ԓ����ǂ�����Ԃ�
	/// </summary>
	/// <returns>���G���Ԓ����ǂ����̃t���O</returns>
	bool GetIsInvincible(){return m_invincibleInfo.isInvincible;}

	/// <summary>
	/// �����������ǂ������󂯎��
	/// </summary>
	void GetIsWin() { m_isWin = true; }

	/// <summary>
	/// �X�e�[�g�̕ύX
	/// </summary>
	/// <param name="state">�X�e�[�g</param>
	void SetStateChange(const State& state);

	/// <summary>
	/// �X�e�[�g��Ԃ�
	/// </summary>
	/// <returns>�X�e�[�g</returns>
	State GetState() { return m_state; }

	/// <summary>
	/// ���g�̍��W��Ԃ�
	/// </summary>
	/// <returns>���W</returns>
	VECTOR GetPos() { return m_pos; }

protected:

	/// <summary>
	/// ����������
	/// </summary>
	virtual void InitSet(){};

	/// <summary>
	/// ���쏈��
	/// </summary>
	virtual void Operation() = 0;

	/// <summary>
	/// �ړ�����
	/// </summary>
	virtual void Move() = 0;

	/// <summary>
	/// �U���J�n����
	/// </summary>
	virtual void BeginningAttack() = 0;

	/// <summary>
	/// �U������
	/// </summary>
	virtual void Attack();

	/// <summary>
	/// �U���C���^�[�o������
	/// </summary>
	virtual void AttackInterval();

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	virtual void Collision();

	/// <summary>
	/// �Ֆʂ̓����蔻��
	/// </summary>
	virtual void BoardCollision();

	/// <summary>
	/// ��������
	/// </summary>
	virtual void Fall();

	/// <summary>
	/// �����Ă���L�����N�^�[�̍��W���擾
	/// </summary>
	/// <returns>�����Ă���L�����N�^�[�̍��W</returns>
	std::vector<VECTOR> GetAlivePos();


private:


	/// <summary>
	/// ���G����
	/// </summary>
	void Invincible();

	/// <summary>
	/// ���f���̊p�x����
	/// </summary>
	void ModelAngle();

	/// <summary>
	/// ���f���`�揈��
	/// </summary>
	void DrawModel();

	/// <summary>
	/// �����_����������
	/// </summary>
	void RandomDirection();

	/// <summary>
	/// �������[�V��������
	/// </summary>
	void WinMotion();

protected:

	////////////////////////////
	// �L�����N�^�[�f�[�^�֘A //
	////////////////////////////
	
	GameData::CharacterStatus m_charData;// �L�����f�[�^

	//////////////
	// �����֘A //
	//////////////

	// �������
	DirectionInfo m_directionInfo;

	//////////////
	// �U���֘A //
	//////////////

	// �U�����[�V�����C���^�[�o��
	int m_attackMotionInterval;

	//////////////
	// ���A�֘A //
	//////////////

	// ���G���
	InvincibleInfo m_invincibleInfo;

	//////////////
	// ���W�֘A //
	//////////////

	// ���W
	VECTOR m_pos;

	// �ړ��x�N�g��
	VECTOR m_vec;

	
	//////////////////
	// �X�e�[�g�֘A //
	//////////////////

	// �X�e�[�g
	State m_state;

	// �����������ǂ���
	bool m_isWin;

	////////////////////
	// �V�[���ύX�֘A //
	////////////////////

	// �Q�[���I���t���[��
	int m_gameEndFrame;

	////////////////
	// �N���X�֘A //
	////////////////

	StateMachine<State>m_pStateMachine;
	std::shared_ptr<Field>m_pField;
	std::shared_ptr<MainParam>m_pMainParam;
	std::shared_ptr<Algorithm>m_pAlgorithm;
	std::shared_ptr<CharacterManager>m_pCharacterManager;
	std::shared_ptr<EffectManager>m_pEffectManager;
	std::shared_ptr<MainUI>m_pMainUI;
};

