#include "StateMachine.h"// ※一番上に書く
#include "Player.h"
#include "Pad.h"




Player::Player()
{
}

Player::~Player()
{
}


void Player::Operation()
{
	// 移動
	Move();

	// 攻撃開始
	BeginningAttack();
}

void Player::Move()
{
	// 方向の初期化
	m_vec = VGet(0, 0, 0);


	// 上に進む
	if (Pad::IsPress(PAD_INPUT_UP, m_charData.charNumber))
	{
		// 上への移動量を代入
		m_vec = VGet(0, 0, 1);

		// 上向き
		m_directionInfo.direction.current = Direction::Top;
	}
	// 下に進む
	if (Pad::IsPress(PAD_INPUT_DOWN, m_charData.charNumber))
	{
		// 下への移動量を代入
		m_vec = VGet(0, 0, -1);

		// 下向き
		m_directionInfo.direction.current = Direction::Bottom;
	}
	// 左に進む
	if (Pad::IsPress(PAD_INPUT_LEFT, m_charData.charNumber))
	{
		// 左への移動量を代入
		m_vec = VGet(-1, 0, 0);

		// 左向き
		m_directionInfo.direction.current = Direction::Left;
	}
	// 右に進む
	if (Pad::IsPress(PAD_INPUT_RIGHT, m_charData.charNumber))
	{
		// 右への移動量を代入
		m_vec = VGet(1, 0, 0);

		// 右向き
		m_directionInfo.direction.current = Direction::Right;
	}

	// 当たり判定
	Collision();
}

void Player::BeginningAttack()
{
	// ボタンを押した場合、攻撃を行う
	if (Pad::IsTrigger(PAD_INPUT_1, m_charData.charNumber))
	{
		// 攻撃処理
		Attack();
	}
}
