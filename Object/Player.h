#pragma once


#include "CharacterBase.h"

class Player : public CharacterBase
{
public:
	Player();
	virtual ~Player();

private:

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
};

