#include "AttackEffect.h"
#include "Field.h"

namespace
{
	// 初期Y座標
	constexpr float kInitPos_Y = -4.0f;

	// 初期角度
	const VECTOR kInitAngle = VGet(90.0f, 0.0f, 0.0f);

	// 移動速度調整値
	constexpr float kMoveSpeedAdjustmentValue = 0.8f;

	// 残像数
	constexpr int kAfterEffectValue = 15;

	// 残像の透明度
	constexpr int kAfterEffectAlphaValue = 20;
}


AttackEffect::AttackEffect():
	// エフェクト関連↓
	m_effectInfo(),
	m_isDelete(),
	m_afterPos(),
	// クラスポインタ関連↓
	m_pField()
{
}

AttackEffect::~AttackEffect()
{
}

void AttackEffect::Update()
{
	// 移動処理
	Move();
}

void AttackEffect::Draw()
{
	// 通常描画
	{
		Draw_Lib::DrawGprah3D_EvoLib(m_effectInfo.handle, m_effectInfo.graphScale, VGet(kInitAngle.x, m_effectInfo.angle, 0.0f), m_afterPos[0],
			Draw_Lib::Color(m_effectInfo.color.x, m_effectInfo.color.y, m_effectInfo.color.z));
	}

	// 残像描画
	DrawAfterEffect();
}

void AttackEffect::Move()
{

	// 方角によって移動する座標を変える
	switch (m_effectInfo.directon)
	{
	case Direction::Top:

		// 上方向に進む
		m_effectInfo.pos.z += m_effectInfo.moveSpeed;

		break;

	case Direction::Bottom:

		// 下方向に進む
		m_effectInfo.pos.z -= m_effectInfo.moveSpeed;

		break;

	case Direction::Left:

		// 左方向に進む
		m_effectInfo.pos.x -= m_effectInfo.moveSpeed;

		break;

	case Direction::Right:

		// 右方向に進む
		m_effectInfo.pos.x += m_effectInfo.moveSpeed;

		break;

	default:
		break;
	}


	// 座標がフィールド外のとき、削除フラグをtrueにする
	{
		// 現在の座標がフィールドの範囲外の場合、削除フラグをtrueにする
		if (!m_pField->GetIsBoardCurrentCellRange(m_effectInfo.pos))
		{
			// フラグをtrueにする
			m_isDelete = true;
		}
	}

	// 座標変換
	ConvertPos();


	// 残像の座標データを一つづつずらす
	for (int i = kAfterEffectValue - 1; i > 0; i--)
	{
		m_afterPos[i] = m_afterPos[i - 1];
	}
}

void AttackEffect::ConvertPos()
{
	// 変換座標
	VECTOR conversionPos;

	// 変換座標を代入
	conversionPos.x = m_effectInfo.pos.x;
	conversionPos.y = m_effectInfo.pos.z;


	// 進む方角によって変換座標に代入する値を変更する
	switch (m_effectInfo.directon)
	{
	case Direction::Top:

		// 変換座標を代入
		conversionPos.x = m_effectInfo.pos.z;
		conversionPos.y = -m_effectInfo.pos.x;

		break;

	case Direction::Bottom:

		// 変換座標を代入
		conversionPos.x = -m_effectInfo.pos.z;
		conversionPos.y = m_effectInfo.pos.x;

		break;

	case Direction::Left:

		// 変換座標を代入
		conversionPos.x = -m_effectInfo.pos.x;
		conversionPos.y = -m_effectInfo.pos.z;

		break;

	case Direction::Right:

		// 変換座標を代入
		conversionPos.x = m_effectInfo.pos.x;
		conversionPos.y = m_effectInfo.pos.z;

		break;

	default:
		break;
	}

	// 変換座標を代入
	conversionPos.z = m_effectInfo.pos.y;

	// 変換した座標を代入
	m_afterPos[0] = conversionPos;
}

void AttackEffect::DrawAfterEffect()
{
	// 透明にして表示する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAfterEffectAlphaValue);

	// 残像を描画
	for (int i = kAfterEffectValue - 1; i >= 0; i -= 1)
	{
		Draw_Lib::DrawGprah3D_EvoLib(m_effectInfo.handle, m_effectInfo.graphScale, VGet(kInitAngle.x, m_effectInfo.angle, 0.0f), m_afterPos[i],
			Draw_Lib::Color(m_effectInfo.color.x, m_effectInfo.color.y, m_effectInfo.color.z));
	}

	// 描画を通常描画にする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void AttackEffect::Init(const EffectSetting& effectSetting)
{
	
	// エフェクト情報
	{
		// エフェクト情報代入
		m_effectInfo = effectSetting;

		// 座標代入
		m_effectInfo.pos = m_pField->GetFieldCellCenterPos(m_effectInfo.pos);

		// 高さを調整する
		m_effectInfo.pos.y = kInitPos_Y;

		// 角度
		m_effectInfo.angle = Convert_Lib::ConvertDirectionToAngle(m_effectInfo.directon);

		// 移動スピード代入
		m_effectInfo.moveSpeed = static_cast<float>(m_pField->GetDeleteFallColorInterval());

		// スピード調整
		m_effectInfo.moveSpeed *= kMoveSpeedAdjustmentValue;

		// 削除フラグの初期化
		m_isDelete = false;
	}

	// 座標調整
	{
		// フィールドモデルサイズ取得
		float posAdjustment = m_pField->GetFieldCellModelScale();


		// 座標を調整する
		switch (m_effectInfo.directon)
		{
		case Direction::Top:

			// 調整座標の代入
			m_effectInfo.pos.z += posAdjustment;

			break;

		case Direction::Bottom:

			// 調整座標の代入
			m_effectInfo.pos.z -= posAdjustment;
			
			break;

		case Direction::Left:

			// 調整座標の代入
			m_effectInfo.pos.x -= posAdjustment;

			break;

		case Direction::Right:

			// 調整座標の代入
			m_effectInfo.pos.x += posAdjustment;

			break;

		default:
			break;
		}
	}


	// 要素を増やす
	m_afterPos.resize(kAfterEffectValue);

	// 座標変換処理
	ConvertPos();

	// 残像座標の初期化
	for (auto& pos : m_afterPos)
	{
		// 座標代入
		pos = m_afterPos.front();
	}
}