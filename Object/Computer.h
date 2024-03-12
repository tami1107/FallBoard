#pragma once

#include "CharacterBase.h"
#include "Algorithm.h"

class Computer : public CharacterBase
{
public:
	Computer();
	virtual ~Computer();

private:

	/// <summary>
	/// ����������
	/// </summary>
	virtual void InitSet()override;

	/// <summary>
	/// ���쏈��
	/// </summary>
	virtual void Operation()override;

	/// <summary>
	/// �ړ�����
	/// </summary>
	virtual void Move()override;

	/// <summary>
	/// �U���J�n����
	/// </summary>
	virtual void BeginningAttack()override;

private:

	/// <summary>
	/// �U���N�[���^�C������
	/// </summary>
	void AttackCoolTime();

	/// <summary>
	/// �ړI�n�ֈړ����鏈��
	/// </summary>
	void MoveDestination();

	/// <summary>
	/// �ړI�n�𒲂ׂ鏈��
	/// </summary>
	void CheckDestination();

	/// <summary>
	/// �A���S���Y�����l���鏈��
	/// </summary>
	void ThinkAlgorithm();


private:

	//////////////
	// �U���֘A //
	//////////////

	// �U���ő�N�[���^�C��
	int m_attackMaxCoolTime;

	// �U���N�[���^�C��
	int m_attackCoolTime;

	////////////////////
	// �����蔻��֘A //
	////////////////////

	// �ړI�n�̍��W
	VECTOR m_coordinatePos;
};

