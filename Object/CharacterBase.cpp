#include "StateMachine.h"// ※一番上に書く
#include "CharacterBase.h"
#include "Field.h"
#include "MainParam.h"
#include "CharacterManager.h"
#include "EffectManager.h"
#include "MainUI.h"
#include "Sound.h"
#include "Pad.h"


namespace
{
	// ゲーム終了フレーム
	constexpr int kGameEndMaxFrame = FPS_Lib::FPS * 4;

	// 方向転換速度
	constexpr int kChangeDirectionSpeed = 10;

	// 勝利モーションスピード
	constexpr float kWinMotionSpeed = 5.0f;

	// パッドの振動の強さ
	constexpr int kPadVibrationStrength = 1000;

	// パッドの振動の強さ
	constexpr int kPadVibrationTime = FPS_Lib::FPS * 3;
}

namespace
{
	// サウンドファイル名
	const std::vector<std::string> kSoundFileName =
	{
		"attack",		// 攻撃音
		"playerDrop",	// 落下音
		"resurrection",	// 復活音
		"win"			// 勝利音
	};


	// サウンド名
	enum class SoundName
	{
		Attack,			// 攻撃音
		Drop,			// 落下音
		Resurrection,	// 復活音
		Win,			// 勝利音
	};
}


CharacterBase::CharacterBase() :
	// キャラクターデータ関連↓
	m_charData(),
	// 方向関連↓
	m_directionInfo(),
	// 攻撃関連↓
	m_attackMotionInterval(),
	// 復活関連↓
	m_invincibleInfo(),
	// 座標関連↓
	m_pos(),
	m_vec(),
	// ステート関連↓
	m_state(),
	m_isWin(),
	// シーン変更関連↓
	m_gameEndFrame(),
	// クラス関連↓
	m_pStateMachine()
{
}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Init()
{

	// ステートマシンの初期化、Entry
	auto dummy = []() {};

	// 登場処理をステートマシンに追加
	{
		auto popEnter = [this]() {PopEnter(); };
		auto popUpdate = [this]() {PopUpdate(); };
		auto popDraw = dummy;
		auto popExit = dummy;

		m_pStateMachine.AddState(State::Pop, popEnter, popUpdate, popDraw, popExit);
	}

	// 通常処理をステートマシンに追加
	{
		auto normalEnter = dummy;
		auto normalUpdate = [this]() {NormalUpdate(); };
		auto normalDraw = []() {};
		auto normalExit = dummy;

		m_pStateMachine.AddState(State::Normal, normalEnter, normalUpdate, normalDraw, normalExit);
	}

	// 攻撃処理をステートマシンに追加
	{
		auto attackEnter = [this]() {AttackEnter(); };
		auto attackUpdate = [this]() {AttackUpdate(); };
		auto attackDraw = dummy;
		auto attackExit = dummy;

		m_pStateMachine.AddState(State::Attack, attackEnter, attackUpdate, attackDraw, attackExit);
	}


	// 落下処理をステートマシンに追加
	{
		auto fallEnter = [this]() {FallEnter(); };
		auto fallUpdate = [this]() {FallUpdate(); };
		auto fallDraw = dummy;
		auto fallExit = [this]() {FallExit(); };

		m_pStateMachine.AddState(State::Fall, fallEnter, fallUpdate, fallDraw, fallExit);
	}

	// 死亡処理をステートマシンに追加
	{
		auto deathEnter = [this]() {DeathEnter(); };
		auto deathUpdate = dummy;
		auto deathExit = dummy;

		m_pStateMachine.AddState(State::Death, deathEnter, deathUpdate, dummy, deathExit);
	}

	// 勝利処理をステートマシンに追加
	{
		auto winEnter = [this]() {WinEnter(); };
		auto winUpdate = [this]() {WinUpdate(); };
		auto winExit = dummy;

		m_pStateMachine.AddState(State::Win, winEnter, winUpdate, dummy, winExit);
	}

	// ステートセット
	SetStateChange(State::Pop);

	// フィールドポインタを送る
	m_pEffectManager->SetFieldPointer(m_pField);

	// 自身のポインタを送る
	m_pEffectManager->SetCharacterBasePointer(shared_from_this());

	// サウンドロード
	Sound::getInstance().Load(kSoundFileName);

}

void CharacterBase::Update()
{
	// プレイヤーがいることを入れる
	m_pField->SetPlayerPresent(m_pos);

	// ステートマシンの更新処理
	m_pStateMachine.Update();
}

void CharacterBase::Draw()
{
	// モデルの描画
	DrawModel();

	// 死亡している時以外はタグを表示
	if (m_state != State::Death)
	{
		// タグを描画
		m_pMainUI->CharacterTagDraw(m_charData.isPlayer, m_charData.charNumber, m_pos, m_charData.color);
	}
}

void CharacterBase::PopEnter()
{
	// 変数の初期化
	{
		// 移動ベクトルの初期化
		m_vec = VGet(0.0f, 0.0f, 0.0f);

		// キャラクターの体力をパラメータに送る
		m_pMainParam->SetCharLife(m_charData.charNumber, m_charData.lifePoint);
	}

	// 方向関連
	{
		// 角度を0にする
		m_directionInfo.modelAngle = 0;

		// 右を向いていない
		m_directionInfo.isRightDirection = false;

		// 上を向いていない
		m_directionInfo.isTopDirection = false;

		// ランダムな方向を得る
		RandomDirection();
	}

	// 座標関連
	{
		// スポーン位置取得
		m_pos = m_pField->GetSpawnPos();
	}

	// 自分のいる位置を入れる
	m_pField->SetPlayerPresent(m_pos);

	// 初期化処理
	InitSet();
}

void CharacterBase::PopUpdate()
{
	// 移動
	Move();


	// カウントダウン終わっていた場合、Normalステートに移行する
	if (m_pMainParam->GetIsCountDownEnd())
	{
		// ステートセット
		SetStateChange(State::Normal);
	}	
}

void CharacterBase::NormalUpdate()
{
	// 操作
	Operation();

	// 無敵処理
	Invincible();

	// 勝利フラグがtrueの場合、ステートを変更
	if (m_isWin)
	{
		SetStateChange(State::Win);
	}
}

void CharacterBase::AttackEnter()
{
	// 攻撃エフェクト生成
	m_pEffectManager->CreateAttackEffect(m_pos, m_directionInfo.direction.current, m_charData.color,m_charData.charNumber);

	// 移動量の初期化
	m_vec = VGet(0, 0, 0);

	// 攻撃SEを流す
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Attack)]);
}

void CharacterBase::AttackUpdate()
{
	// 無敵処理
	Invincible();

	// 攻撃インターバル
	AttackInterval();
}

void CharacterBase::FallEnter()
{
	// 体力を減らす
	m_charData.lifePoint--;

	// キャラクターの体力をパラメータに送る
	m_pMainParam->SetCharLife(m_charData.charNumber, m_charData.lifePoint);

	// キャラクターがダメージを受けた事を送る
	m_pMainParam->SetIsCharDamage(m_charData.charNumber, true);

	// 落下SEを流す
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Drop)]);

	// 振動開始
	Pad::StartingVibration(kPadVibrationStrength, kPadVibrationTime, m_charData.charNumber);
}

void CharacterBase::FallUpdate()
{
	// 落下処理
	Fall();
}

void CharacterBase::FallExit()
{
	if (m_charData.lifePoint == 0)return;

	// 方向関連
	{
		// ランダムな方向を得る
		RandomDirection();
	}

	// 座標関連
	{
		// スポーン位置取得
		m_pos = m_pField->GetSpawnPos();
	}

	// 復活関連
	{
		// 復活時のセルを取得
		m_invincibleInfo.resurrectionCell = m_pField->GetConvertPosToCell(m_pos);

		// 無敵状態にする
		m_invincibleInfo.isInvincible = true;

		// 無敵時間を代入
		m_invincibleInfo.invincibleFrame = m_charData.invincibleTime;

		if (m_charData.lifePoint != 0)
		{
			// エフェクト描画
			m_pEffectManager->CreateResurrectionEffect(m_pos, m_charData.color, m_charData.charNumber);
		}

		// 復活SEを流す
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Resurrection)]);
	}

	// コントローラーの振動を止める
	Pad::StopVibration(m_charData.charNumber);
}

void CharacterBase::DeathEnter()
{
	// 生きているプレイヤーを数え、残りプレイヤーが1人の場合、ゲームを終了する
	m_pCharacterManager->WinnerDecision();

	// 落下カウントを増やす
	m_pMainParam->BoardDropCount();
}

void CharacterBase::WinEnter()
{
	// 無敵判定をfalseにする
	m_invincibleInfo.isInvincible = false;

	// ゲーム終了フレームを代入
	m_gameEndFrame = kGameEndMaxFrame;

	// 表情変更
	ModelData::getInstance().ChangeFace(ModelData::Face::Smile, m_charData.charNumber);

	// 勝利SEを流す
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Win)]);
}

void CharacterBase::WinUpdate()
{
	// ゲーム終了フレームを減らす
	m_gameEndFrame--;

	// 勝利モーション
	WinMotion();

	// ゲーム終了フレームが0以下の時、シーン変更フラグを立てる
	if (m_gameEndFrame <= 0)
	{
		m_gameEndFrame = 0;

		// 終了フラグを送る
		m_pCharacterManager->GameEnd();
	}
}

bool CharacterBase::GetCharAlive()
{
	// ライフが0以下の場合、falseを返す
	if (m_state != State::Death)
	{
		return true;
	}

	return false;
}

void CharacterBase::SetStateChange(const State& state)
{
	// ステート代入
	m_state = state;

	// ステートセット
	m_pStateMachine.SetState(m_state);
}

void CharacterBase::Attack()
{
	// 盤面を落とす
	m_pField->AttackFallCollision(m_pos, m_directionInfo.direction.current, m_charData.charNumber);

	// 攻撃モーションインターバルを代入
	m_attackMotionInterval = m_charData.attackRigorTime;

	// 攻撃処理にする
	SetStateChange(State::Attack);
}

void CharacterBase::AttackInterval()
{
	// 攻撃インターバルを減らす
	m_attackMotionInterval--;

	// 当たり判定
	Collision();

	// 攻撃モーションインターバルが0以下の場合、0にする
	if (m_attackMotionInterval <= 0)
	{
		m_attackMotionInterval = 0;
	}

	// 攻撃モーションインターバルが0の場合、通常処理に変更
	if (m_attackMotionInterval == 0)
	{
		// 通常処理にする
		SetStateChange(State::Normal);
	}
}

void CharacterBase::Collision()
{
	// 盤面の当たり判定
	BoardCollision();
}


void CharacterBase::BoardCollision()
{
	// ベクトルの掛け算
	const VECTOR velocity = VScale(m_vec, m_charData.moveSpeed);

	// 盤面のセルに立っているかどうかを判定
	{
		// 移動量を足した後の座標
		VECTOR afterPos = VAdd(m_pos, velocity);


		// 移動前にいる、盤面のセルが落下しているかどうかを取得
		bool isBoardFall = m_pField->GetIsBoardCellFall(m_pos);
		// 移動後、盤面の地面にいるかどうかを取得
		bool isBoardGround = m_pField->GetIsBoardCellGround(afterPos);

		// 無敵中ならば、移動前にいる、盤面のセルの落下判定をfalseにする
		if (m_invincibleInfo.isInvincible)
		{
			isBoardFall = false;
			
			// 範囲内かどうか
			isBoardGround = m_pField->GetIsBoardCellRange(afterPos);
		}


		// 盤面の地面にいないのなら処理を終了する
		if (!isBoardGround)
		{
			// 盤面のセルが落下していたらFallステートに変更し、処理を終了する
			if (isBoardFall)
			{
				// ステートセット
				SetStateChange(State::Fall);

				return;
			}
			return;
		}
	}

	// ベクトルの足し算
	m_pos = VAdd(m_pos, velocity);
}

void CharacterBase::Fall()
{
	// 落下させる
	m_pos.y -= m_pField->GetFallSpeed();

	// ある程度の位置まで落ちた場合、元の位置に戻す
	if (m_pos.y <= m_pField->GetFallLowerLimitPos())
	{
		// 体力が0以下の場合、死亡状態にする
		if (m_charData.lifePoint <= 0 && !m_isWin)
		{
			// 死亡処理にする
			SetStateChange(State::Death);

			return;
		}

		// 通常処理にする
		SetStateChange(State::Normal);
	}
}

std::vector<VECTOR> CharacterBase::GetAlivePos()
{
	// 生きているキャラの座標
	const std::vector<VECTOR> alivePos = m_pCharacterManager->AlivePos(m_charData.charNumber);

	// 生きている座標を返す
	return alivePos;
}

void CharacterBase::Invincible()
{
	// 無敵中ではない場合、ここで処理を終了する
	if (!m_invincibleInfo.isInvincible)
	{
		return;
	}

	// 現在の座標からセルを取得
	Field::Cell currentCell = m_pField->GetConvertPosToCell(m_pos);


	// 現在のセルと復活時のセルが同じでなければ、無敵時間を終わらせる
	if (m_invincibleInfo.resurrectionCell.x != currentCell.x ||
		m_invincibleInfo.resurrectionCell.z != currentCell.z)
	{
		// 無敵時間を0にする
		m_invincibleInfo.invincibleFrame = 0;

		// 無敵判定をfalseにする
		m_invincibleInfo.isInvincible = false;

		return;
	}

	// 無敵時間を減らす
	m_invincibleInfo.invincibleFrame--;

	// 無敵時間が0以下の場合、無敵判定をfalseにする
	if (m_invincibleInfo.invincibleFrame <= 0)
	{
		// 無敵時間を0にする
		m_invincibleInfo.invincibleFrame = 0;

		// 無敵判定をfalseにする
		m_invincibleInfo.isInvincible = false;
	}
}

void CharacterBase::ModelAngle()
{

	
	// 1フレーム前と向きが違う場合、方向転換を行う
	if (m_directionInfo.direction.current != m_directionInfo.direction.before)
	{
		// 方向転換前
		m_directionInfo.changeDirection.before = m_directionInfo.direction.before;

		// 方向転換後
		m_directionInfo.changeDirection.after = m_directionInfo.direction.current;

		// 方向転換フラグをtrueにする
		m_directionInfo.isChangeDirection = true;
	}

	// 方向を代入
	m_directionInfo.direction.before = m_directionInfo.direction.current;


	
	
	// 現在向いている方向によってフラグを変更する
	switch (m_directionInfo.direction.current)		
	{
	case Direction::Top:

		m_directionInfo.isTopDirection = true;

		break;
	case Direction::Bottom:

		m_directionInfo.isTopDirection = false;

		break;
	case Direction::Left:

		m_directionInfo.isRightDirection = false;

		break;
	case Direction::Right:

		m_directionInfo.isRightDirection = true;

		break;
	default:
		break;
	}

	// 方向転換中でない場合ここで処理を終了する
	if (!m_directionInfo.isChangeDirection)
	{
		return;
	}

	// 足すかどうか
	bool isAdd = false;

	// 転換前の角度
	int beforeAngle = 
		static_cast<int>(Convert_Lib::ConvertDirectionToAngle(m_directionInfo.changeDirection.before));

	// 転換後の角度
	int afterAngle = 
		static_cast<int>(Convert_Lib::ConvertDirectionToAngle(m_directionInfo.changeDirection.after));


	
	// 度数が0では成り立たない場合があるので360を代入する
	if (m_directionInfo.changeDirection.after == Direction::Top && m_directionInfo.changeDirection.before == Direction::Right)
	{
		beforeAngle = 360;
	}
	else if (m_directionInfo.changeDirection.before == Direction::Top && m_directionInfo.changeDirection.after == Direction::Right)
	{
		afterAngle = 360;
	}


	
	// 回転量
	const int angleValue = afterAngle - beforeAngle;

	// 正の値かどうか
	const bool isRegularValue = (angleValue > 0);


	// 回転量が正の値なら加算フラグをtrueにする
	if (isRegularValue)
	{
		isAdd = true;
	}

	// 180度以上回転する場合、向きを調べる
	if (angleValue == 180 || angleValue == -180)
	{
		isAdd = true;

		if (m_directionInfo.changeDirection.after == Direction::Top||
			m_directionInfo.changeDirection.after == Direction::Bottom)
		{
			if (m_directionInfo.isRightDirection && isRegularValue ||
				!m_directionInfo.isRightDirection && !isRegularValue)
			{
				isAdd = false;
			}
		}
		else
		{
			if (m_directionInfo.isTopDirection && isRegularValue ||
				!m_directionInfo.isTopDirection && !isRegularValue)
			{
				isAdd = false;
			}
		}
	}
	


	// 加算フラグによって角度を加減算する
	if (isAdd)
	{
		// 角度を変更
		m_directionInfo.modelAngle =
			static_cast<int>(Calculation_Lib::AngleLimitValueNotExceedToCalculate(static_cast<float>(m_directionInfo.modelAngle), static_cast<float>(kChangeDirectionSpeed), Calculation_Lib::Calculation::Addition, true, false));
	}
	else
	{
		// 角度を変更
		m_directionInfo.modelAngle =
			static_cast<int>(Calculation_Lib::AngleLimitValueNotExceedToCalculate(static_cast<float>(m_directionInfo.modelAngle), static_cast<float>(kChangeDirectionSpeed), Calculation_Lib::Calculation::Subtraction, true, false));
	}



	// 角度の値を補正
	if (Calculation_Lib::IsTargetRangeValue(static_cast<float>(m_directionInfo.modelAngle), static_cast<float>(afterAngle), static_cast<float>(kChangeDirectionSpeed)))
	{
		m_directionInfo.modelAngle = afterAngle;

		m_directionInfo.isChangeDirection = false;
	}
}

void CharacterBase::DrawModel()
{
	// 死亡状態の場合、モデルを描画しない
	if (m_state == State::Death)
	{
		return;
	}

	// モデルの回転
	ModelAngle();


	// モデルの描画
	ModelData::DrawModelData drawModelData;

	// モデル描画情報を代入
	drawModelData.playerNumber = m_charData.charNumber;
	drawModelData.angle = m_directionInfo.modelAngle;
	drawModelData.pos = m_pos;

	// 無敵時間中ならば、点滅させる
	if (m_invincibleInfo.isInvincible)
	{
		// 無敵時間が偶数の場合に描画
		if (m_invincibleInfo.invincibleFrame % 2 == 0)
		{
			ModelData::getInstance().DrawModel(drawModelData);
		}
		return;
	}
	
	// モデルの描画
	ModelData::getInstance().DrawModel(drawModelData);
}

void CharacterBase::RandomDirection()
{
	// 方向数
	const int directionNum = (static_cast<int> (Direction::DirectionNum) - 1);

	// ランダムな数を得る
	const int randomNum = GetRand(directionNum);


	// 方向を代入
	m_directionInfo.direction.current = Direction(randomNum);
	m_directionInfo.direction.before = m_directionInfo.direction.current;


	// モデルの角度
	m_directionInfo.modelAngle = static_cast<int>(Convert_Lib::ConvertDirectionToAngle(m_directionInfo.direction.current));
}

void CharacterBase::WinMotion()
{
	// 勝利モーション角度を変更
	m_directionInfo.modelAngle =
		static_cast<int>(Calculation_Lib::AngleLimitValueNotExceedToCalculate(static_cast<float>(m_directionInfo.modelAngle), kWinMotionSpeed, Calculation_Lib::Calculation::Addition, true));
}