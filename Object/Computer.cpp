#include "StateMachine.h"// ※一番上に書く
#include "Computer.h"
#include "GameData.h"
#include "CharacterManager.h"

namespace
{
	// 衝突時間
	constexpr int kCollisionFrame = 30;

	// 攻撃クールタイム
	constexpr int kAttackCoolTime = 120;

	// レベルのクールタイム
	constexpr int kLevelCoolTime = 120;
}

Computer::Computer():
	// 攻撃関連↓
	m_attackMaxCoolTime(),
	m_attackCoolTime(),
	// 当たり判定関連↓
	m_coordinatePos()
{
}

Computer::~Computer()
{
}

void Computer::InitSet()
{
	// 座標を代入
	m_coordinatePos = m_pos;

	// 目的地を調べる
	CheckDestination();

	// レベル番号
	int levelNum = 0;

	// 取得したレベルよってに番号を代入する
	switch (GameData::getInstance().GetLevel())
	{
	case GameData::Level::Easy:

		levelNum = 2;

		break;

	case GameData::Level::Noramal:

		levelNum = 1;

		break;

	case GameData::Level::Hard:

		levelNum = 0;

		break;

	default:
		break;
	}

	// 最大攻撃クールタイム
	m_attackMaxCoolTime = kAttackCoolTime + (kLevelCoolTime * levelNum);

	// 攻撃クールタイムに最大攻撃クールタイムを代入
	m_attackCoolTime = m_attackMaxCoolTime;
}

void Computer::Operation()
{
	// 移動処理
	Move();

	// 攻撃クールタイム処理
	AttackCoolTime();
}

void Computer::Move()
{
	// 目的地へ移動する処理
	MoveDestination();

	// 方向の初期化
	m_vec = VGet(0, 0, 0);

	// 進む方向を取得
	m_directionInfo.direction.current = m_pAlgorithm->TargetDirection(m_pos, m_coordinatePos);

	// 取得した進む方向より、移動量を調べる
	switch (m_directionInfo.direction.current)
	{
	case Direction::Top:

		// 上への移動量を代入
		m_vec = VGet(0, 0, 1);

		break;
	case Direction::Bottom:

		// 下への移動量を代入
		m_vec = VGet(0, 0, -1);

		break;
	case Direction::Left:

		// 左への移動量を代入
		m_vec = VGet(-1, 0, 0);

		break;
	case Direction::Right:

		// 右への移動量を代入
		m_vec = VGet(1, 0, 0);

		break;
	default:
		break;
	}


	// 四方向すべてに進めない場合、移動量を0にする
	if (m_pAlgorithm->IsAroundObstacle(m_pos))
	{
		// 現在座標からセルを目的地にする
		m_coordinatePos = m_pAlgorithm->FindCellSafeClose(m_pos);
	}

	// 当たり判定
	Collision();
}

void Computer::BeginningAttack()
{
	// 攻撃できるかどうかを取得する
	const Algorithm::Attack attack = m_pAlgorithm->IsAssailable(m_pos, GetAlivePos());

	// falseの場合、ここで処理を終了する
	if (!attack.isAttack)
	{
		return;
	}

	// 攻撃方向を代入
	m_directionInfo.direction.current = attack.direction;

	// 攻撃インターバルを代入
	m_attackCoolTime = m_attackMaxCoolTime;

	// 攻撃処理
	Attack();

	// 残りのキャラクターがすべてコンピュータならば、攻撃速度を早める
	if (m_pCharacterManager->IsAllCP())
	{
		m_attackCoolTime = kAttackCoolTime;
	}
}

void Computer::AttackCoolTime()
{
	// 攻撃クールタイム
	{
		if (m_state == State::Pop)return;

		// 攻撃クールタイムを減らす
		m_attackCoolTime--;

		// 攻撃クールタイムが0以下になった場合、0にする
		if (m_attackCoolTime <= 0)
		{
			m_attackCoolTime = 0;
		}

		// 攻撃クールタイムが0以外の場合、ここで処理を終了する
		if (m_attackCoolTime != 0)
		{
			return;
		}
	}

	// 攻撃開始
	BeginningAttack();
}

void Computer::MoveDestination()
{

	// 現在地か、目的地に障害物がある場合、目的地を変更する
	if (m_pAlgorithm->IsObstacle(m_coordinatePos) || m_pAlgorithm->IsObstacle(m_pos))
	{
		// 現在座標からセルを目的地にする
		m_coordinatePos = m_pAlgorithm->FindCellSafeClose(m_pos);
	}

	// 座標をセルの中心位置に移動させるために、値を補正
	if (Calculation_Lib::IsTargetRangeValue(m_pos.x, m_coordinatePos.x, m_charData.moveSpeed))
	{
		m_pos.x = m_coordinatePos.x;
	}
	if (Calculation_Lib::IsTargetRangeValue(m_pos.z, m_coordinatePos.z, m_charData.moveSpeed))
	{
		m_pos.z = m_coordinatePos.z;
	}
	

	// 目的地についた場合、次の目的地を探す
	if (m_pos.x == m_coordinatePos.x &&
		m_pos.z == m_coordinatePos.z)
	{
		// アルゴリズムを考える
		ThinkAlgorithm();
		
		// 目的地を確認
		CheckDestination();
	}
}

void Computer::CheckDestination()
{
	// 目的地を代入
	m_coordinatePos = m_pAlgorithm->MoveCoordinate(m_pos);
}

void Computer::ThinkAlgorithm()
{
	// ランダムに進む
	m_pAlgorithm->RandomMove(m_pos);
}
