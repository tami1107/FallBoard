#pragma once


#include "CharacterBase.h"

class Player : public CharacterBase
{
public:
	Player();
	virtual ~Player();

private:

	/// <summary>
	/// 操作処理
	/// </summary>
	virtual void Operation()override;

	/// <summary>
	/// 移動処理
	/// </summary>
	virtual void Move()override;

	/// <summary>
	/// 攻撃開始処理
	/// </summary>
	virtual void BeginningAttack()override;
};

