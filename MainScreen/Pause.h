#pragma once
#include<memory>
#include<vector>
#include"Vec2.h"

// ステートマシン
template <class TState> class StateMachine;

class MainScreen;
class Pause
{
public:
	Pause();
	virtual ~Pause();

	/// <summary>
	/// メインスクリーンポインタを取得
	/// </summary>
	/// <param name="mainScreen">メインスクリーンポインタ</param>
	void SetMainScreenPointer(std::shared_ptr<MainScreen>mainScreen) { m_pMainScreen = mainScreen; }


	void Init();
	void Update();
	void Draw();

public:

	// ステート
	enum class State
	{
		// 通常処理
		Normal,
		// ポーズ処理
		Pause
	};

	
	// ポーズ処理
	void PauseEnter();
	void PauseUpdate();
	void PauseDraw();
	void PauseExit();


	
	/// <summary>
	/// ステート変更
	/// </summary>
	/// <param name="state">ステート</param>
	void SetStateChange(const State& state);

public:


	/// <summary>
	/// ハンドルのロード
	/// </summary>
	void LoadHandle();


private:

	/// <summary>
	/// ステートマシンの初期化処理
	/// </summary>
	void InitStateMachine();

	/// <summary>
	/// ポーズセレクト処理
	/// </summary>
	void PauseSelect();

	/// <summary>
	/// ポーズウィンドウ描画処理
	/// </summary>
	void PauseDrawWindow();

	/// <summary>
	/// ポーズ選択グラフィック描画処理
	/// </summary>
	void PauseDrawSelectGraph();

	/// <summary>
	/// 選択項目決定処理
	/// </summary>
	void SelectDetermination();



	/// <summary>
	/// サウンド設定更新処理
	/// </summary>
	void UpdateSoundSetting();

	/// <summary>
	/// サウンド設定描画処理
	/// </summary>
	void DrawSoundSetting();

private:

	// グラフィック
	struct Graphic
	{
		// グラフィックハンドル
		std::vector<int>handle;

		// 座標
		Vec2 pos = Vec2(0.0f, 0.0f);
	};

private:

	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// グラフィック
	std::vector<Graphic>m_graph;

	//////////////////
	// セレクト関連 //
	//////////////////

	// 選択番号
	int m_selectNum;

	// 選択フレーム
	int m_selectInterval;

	// シーン変更するかどうか
	bool m_isScreenChange;

	// 音量設定中かどうか
	bool m_isSoundSetting;

	//////////////////////
	// サインカーブ関連 //
	//////////////////////

	// サインカーブフレーム
	int m_sinFrame;

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

	// メインスクリーンポインタ
	std::shared_ptr<MainScreen>m_pMainScreen;
};