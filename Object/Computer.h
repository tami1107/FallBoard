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
	/// 初期化処理
	/// </summary>
	virtual void InitSet()override;

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

private:

	/// <summary>
	/// 攻撃クールタイム処理
	/// </summary>
	void AttackCoolTime();

	/// <summary>
	/// 目的地へ移動する処理
	/// </summary>
	void MoveDestination();

	/// <summary>
	/// 目的地を調べる処理
	/// </summary>
	void CheckDestination();

	/// <summary>
	/// アルゴリズムを考える処理
	/// </summary>
	void ThinkAlgorithm();


private:

	//////////////
	// 攻撃関連 //
	//////////////

	// 攻撃最大クールタイム
	int m_attackMaxCoolTime;

	// 攻撃クールタイム
	int m_attackCoolTime;

	////////////////////
	// 当たり判定関連 //
	////////////////////

	// 目的地の座標
	VECTOR m_coordinatePos;
};

