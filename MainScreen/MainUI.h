#pragma once

#include <memory>
#include <vector>
#include "Vec2.h"
#include <DxLib.h>

// ステートマシン
template <class TState> class StateMachine;

class MainParam;
class MainUI
{
public:
	MainUI();
	virtual ~MainUI();

	/// <summary>
	/// メインパラㇺポインタ取得
	/// </summary>
	/// <param name="mainParam">メインパラㇺポインタ</param>
	void SetMainParamPointer(std::shared_ptr<MainParam>mainParam) { m_pMainParam = mainParam; }

	void Init();
	void Update();
	void Draw();


public:

	// カウント処理
	void CountEnter();
	void CountUpdate();
	void CountDraw();

public:

	/// <summary>
	/// ステートマシンの初期化処理
	/// </summary>
	void InitStateMachine();


	/// <summary>
	/// ハンドルのロード
	/// </summary>
	void LoadHandle();

	/// <summary>
	/// キャラクターのタグ描画
	/// </summary>
	/// <param name="isPlayer">プレイヤーかどうか</param>
	/// <param name="charcterNum">キャラクター番号</param>
	/// <param name="pos">座標</param>
	/// <param name="color">色</param>
	void CharacterTagDraw(const bool& isPlayer, const int& charcterNum, VECTOR pos, VECTOR color);

private:


	/// <summary>
	/// ライフ初期化処理
	/// </summary>
	void LifeInit();

	/// <summary>
	/// ライフ更新処理
	/// </summary>
	void LifeUpdate();

	/// <summary>
	/// ライフ描画処理
	/// </summary>
	void LifeDraw();
	
public:

	// ステート
	enum class State
	{
		// カウント
		Count,
		// 通常処理
		Normal,
	};


	// カウント
	struct Count
	{
		// ハンドル
		std::vector<int>handle;

		// フレーム
		int frame = 0;

		// 秒数
		int second = 0;

		// アルファ値
		int alphaValue = 0;

		// サイズ
		float scaleValue = 0.0f;

	};
	
	// 体力
	struct Life
	{
		// ハンドル
		std::vector<int>handle;

		// グラフィック座標
		std::vector<Vec2>graphicPos;

		// 最大体力
		int maxLifePoint = 0;

		std::vector<int> lifePoint;

		// 点滅フレーム
		std::vector<int> flashingFrame;

		// 点滅回数
		std::vector<int> flashingCount;
	};

	// フェイス
	struct Face
	{
		// グラフィックハンドル
		std::vector<int>handle;
	};

	// タグ
	struct Tag
	{
		// グラフィックハンドル
		std::vector<int>handle;

		// 座標
		Vec2 pos = Vec2(0.0f, 0.0f);
	};



private:

	//////////////////
	// カウント関連 //
	//////////////////

	// カウント
	Count m_count;

	// 説明グラフィックハンドル
	int m_operationGraphHandle;

	////////////////
	// ライフ関連 //
	////////////////

	// ライフ
	Life m_life;

	// フェイス
	std::vector<Face>m_face;

	//////////////
	// タグ関連 //
	//////////////

	// タグ
	std::vector<Tag> m_tag;

	//////////////////
	// ステート関連 //
	//////////////////

	// ステートマシンポインタ
	StateMachine<State>m_pStateMachine;

	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// メインパラㇺポインタ
	std::shared_ptr<MainParam>m_pMainParam;
};