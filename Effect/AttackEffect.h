#pragma once
#include <memory>
#include <vector>
#include "EvoLib.h"
#include <DxLib.h>


class Field;
class AttackEffect
{
public:

	AttackEffect();
	virtual ~AttackEffect();


	/// <summary>
	/// フィールドポインタを取得
	/// </summary>
	/// <param name="field">フィールドポインタ</param>
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }


	void Update();
	void Draw();


	/// <summary>
	/// 削除フラグを返す
	/// </summary>
	/// <returns>削除フラグ/returns>
	bool GetIsDelete() { return m_isDelete; }

private:
	
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 座標変換処理
	/// </summary>
	void ConvertPos();

	/// <summary>
	/// 残像描画処理
	/// </summary>
	void DrawAfterEffect();

public:

	// エフェクト設定
	struct EffectSetting
	{
		// グラフィックハンドル
		int handle = -1;

		// グラフィックスケール
		float graphScale = 0.0f;

		// 座標
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);

		// 方向
		Direction directon = Direction::Right;

		// 角度
		float angle = 0.0f;

		// 色
		VECTOR color = VGet(1.0f, 1.0f, 1.0f);

		// 移動スピード
		float moveSpeed = 0.0f;
	};



public:

	/// <summary>
	/// エフェクト初期設定
	/// </summary>
	/// <param name="effectSetting">エフェクト設定値</param>
	void Init(const EffectSetting& effectSetting);

private:

	////////////////////
	// エフェクト関連 //
	////////////////////

	// エフェクト情報
	EffectSetting m_effectInfo;

	// 削除するかどうかのフラグ
	bool m_isDelete;

	// 残像座標
	std::vector<VECTOR>m_afterPos;

	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// フィールドクラスポインタ
	std::shared_ptr<Field>m_pField;
};