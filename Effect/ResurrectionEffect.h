#pragma once
#include <memory>
#include <vector>
#include "EvoLib.h"
#include <DxLib.h>

class CharacterBase;
class Field;
class ResurrectionEffect
{
public:
	ResurrectionEffect();
	virtual ~ResurrectionEffect();

	/// <summary>
	/// フィールドポインタを取得
	/// </summary>
	/// <param name="field">フィールドポインタ</param>
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }

	/// <summary>
	/// キャラクターベースポインタを取得
	/// </summary>
	/// <param name="characterBase">キャラクターベースポインタ</param>
	void SetCharacterBasePointer(std::shared_ptr<CharacterBase>characterBase) { m_pCharacterBase = characterBase; }



	void Update();
	void Draw();


	/// <summary>
	/// 削除フラグを返す
	/// </summary>
	/// <returns>削除フラグ</returns>
	bool GetIsDelete() { return m_isDelete; }


public:

	// エフェクト設定
	struct EffectSetting
	{
		// グラフィックハンドル
		int handle = -1;

		// グラフィックスケール
		float scale = 0.0f;

		// 座標
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);

		// 色
		VECTOR color = VGet(1.0f, 1.0f, 1.0f);
	};

	/// <summary>
	/// 初期設定
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

	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// クラスポインタ
	std::shared_ptr<Field>m_pField;
	std::shared_ptr<CharacterBase>m_pCharacterBase;
};

