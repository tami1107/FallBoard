#pragma once
#include <memory>
#include <vector>
#include "EvoLib.h"
#include <DxLib.h>
#include "MasterData.h"

class Field;
class AttackEffect;
class ResurrectionEffect;
class CharacterBase;
class EffectManager
{	
public:

	EffectManager();
	virtual ~EffectManager();

	
	/// <summary>
	/// フィールドクラスポインタの取得
	/// </summary>
	/// <param name="field">フィールドポインタ</param>
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }

	/// <summary>
	/// キャラクターベースクラスポインタの取得
	/// </summary>
	/// <param name="characterBase">キャラクターベースポインタ</param>
	void SetCharacterBasePointer(std::shared_ptr<CharacterBase>characterBase);


	void Init();
	void Update();
	void Draw();

public:

	/// <summary>
	/// 攻撃エフェクト生成
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="directon">方向</param>
	/// <param name="color">色</param>
	/// <param name="charNum">キャラ番号</param>
	void CreateAttackEffect(const VECTOR& pos, const Direction& directon,
		const VECTOR& color, const int& charNum);

	/// <summary>
	/// 復活エフェクト生成
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="color">色</param>
	/// <param name="charNum">キャラ番号</param>
	void CreateResurrectionEffect(const VECTOR& pos,const VECTOR& color, const int& charNum);

private:

	// エフェクト情報
	struct EffectInfo
	{
		// エフェクトハンドル
		int handle = -1;

		// グラフィックスケール
		float scale = 0.0f;
	};


private:

	/// <summary>
	/// ハンドルのロード
	/// </summary>
	void LoadHandle();

	/// <summary>
	/// エフェクトの削除
	/// </summary>
	void DeleteEffect();

private:

	//////////////////////
	// グラフィック関連 //
	//////////////////////


	// 攻撃エフェクト情報
	std::vector<EffectInfo> m_attackEffectInfo;

	// 復活エフェクト情報
	EffectInfo m_resurrectionEffectInfo;


	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// 攻撃エフェクトクラスポインタ
	std::vector<std::shared_ptr<AttackEffect>>m_pAttackEffect;

	// 復活エフェクトクラスポインタ
	std::vector<std::shared_ptr<ResurrectionEffect>>m_pResurrectionEffect;

	// フィールドクラスポインタ
	std::shared_ptr<Field>m_pField;

	// キャラクターベースポインタ
	std::vector<std::shared_ptr<CharacterBase>>m_pCharacterBase;
};