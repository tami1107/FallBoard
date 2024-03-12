#pragma once


#include "CharacterBase.h"

class Player : public CharacterBase
{
public:
	Player();
	virtual ~Player();

private:

	/// <summary>
	/// ‘€ìˆ—
	/// </summary>
	virtual void Operation()override;

	/// <summary>
	/// ˆÚ“®ˆ—
	/// </summary>
	virtual void Move()override;

	/// <summary>
	/// UŒ‚ŠJnˆ—
	/// </summary>
	virtual void BeginningAttack()override;
};

