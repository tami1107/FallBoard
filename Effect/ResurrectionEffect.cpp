#include "StateMachine.h"// ※一番上に書く
#include "ResurrectionEffect.h"
#include "CharacterBase.h"

namespace
{
	// 調整座標
	constexpr float kInitPos_Y = -1.0f;

	// 初期角度
	const VECTOR kInitAngle = VGet(90.0f, 0.0f, 0.0f);
}

ResurrectionEffect::ResurrectionEffect():
	// エフェクト関連↓
	m_effectInfo(),
	m_isDelete(),
	// クラスポインタ関連↓
	m_pField(),
	m_pCharacterBase()
{
}

ResurrectionEffect::~ResurrectionEffect()
{
}

void ResurrectionEffect::Update()
{
	// 無敵時間中でなければ削除フラグをtureにする
	if (!m_pCharacterBase->GetIsInvincible())
	{
		// 削除フラグをtrueにする
		m_isDelete = true;
	}
}

void ResurrectionEffect::Draw()
{
	// 通常描画
	{
		// グラフィック描画
		Draw_Lib::DrawGprah3D_EvoLib(m_effectInfo.handle, m_effectInfo.scale, kInitAngle, m_effectInfo.pos,
			Draw_Lib::Color(m_effectInfo.color.x, m_effectInfo.color.y, m_effectInfo.color.z));
	}
}

void ResurrectionEffect::Init(const EffectSetting& effectSetting)
{
	// エフェクト情報を代入
	m_effectInfo = effectSetting;

	// 変換座標
	VECTOR conversionPos = m_effectInfo.pos;

	// 座標代入
	m_effectInfo.pos.y = conversionPos.z;
	m_effectInfo.pos.z = kInitPos_Y;

	// 削除フラグの初期化
	m_isDelete = false;
}
