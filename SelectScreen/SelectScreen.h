#pragma once

#include<vector>
#include "Vec2.h"
#include "SceneSelect.h"

template <class TState> class StateMachine;

class SelectScreen
{
public:
	SelectScreen();
	virtual ~SelectScreen();

	/// <summary>
	/// キャラセレクトシーンのポインタを入れる
	/// </summary>
	/// <param name="sceneCharSelect">キャラセレクトシーンのポインタ</param>
	void SetSceneCharSelectPointer(SceneSelect* sceneCharSelect) { m_pSceneCharSelect = sceneCharSelect; }

	void Init();
	void Update();
	void Draw();

public:

	// ステート
	enum class State
	{
		PlayCountSelect,// プレイヤー人数を数える
		LevelSelect,	// 難易度選択
		End,			// 終了処理
	};

	// グラフィック情報
	struct Graphic
	{
		// グラフィックハンドル
		std::vector<int>handle;

		// 座標
		Vec2 pos = Vec2(0.0f, 0.0f);
	};
	

private:

	// プレイヤー人数選択処理
	void PlayCountSelectEnter();
	void PlayCountSelectUpdate();

	// 難易度選択処理
	void LevelSelectEnter();
	void LevelSelectUpdate();

	// 終了処理
	void EndEnter();
	void EndUpdate();

private:

	/// <summary>
	/// ステートマシンの初期設定処理
	/// </summary>
	void InitStateMachine();

	/// <summary>
	/// グラフィックのロード処理
	/// </summary>
	void LoadHandle();

	/// <summary>
	/// 選択のインターバル処理
	/// </summary>
	void SelectInterval();

	/// <summary>
	/// プレイヤー人数選択処理
	/// </summary>
	void PlayCountSelect();

	/// <summary>
	/// 難易度選択処理
	/// </summary>
	void LevelSelect();

	/// <summary>
	/// グラフィックの移動処理
	/// </summary>
	void MoveGraph();

	/// <summary>
	/// 背景描画処理
	/// </summary>
	void DrawBackGround();

public:

	/// <summary>
	/// ステート変更処理
	/// </summary>
	/// <param name="state">変更するステート</param>
	void SetStateChange(const State state);

private:

	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// グラフィック
	std::vector<Graphic>m_graph;

	// ベース座標
	Vec2 m_basePos;

	// ターゲット座標
	Vec2 m_targetPos;

	// フレーム
	float m_moveValue;

	//////////////////
	// セレクト関連 //
	//////////////////

	// 人数選択
	int m_playCountSelectNum;

	// 難易度選択
	int m_levelSelectNum;

	// 選択フレーム
	int m_selectInterval;

	//////////////
	// 演出関連 //
	//////////////

	// サインカーブフレーム
	int m_sinFrame;


	////////////////
	// シーン関連 //
	////////////////

	// シーン変更
	SceneSelect::ChangeScene m_changeScene;


	//////////////////
	// ステート関連 //
	//////////////////

	// ステート
	State m_state;

	// ステートマシン
	StateMachine<State>m_pStateMachine;


	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// キャラセレクトシーンポインタ
	SceneSelect* m_pSceneCharSelect;
};

