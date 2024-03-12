#pragma once
#include "SceneMain.h"
#include <memory>
#include <iostream>

// ステートマシンクラス
template <class TState> class StateMachine;

class MainBackground;
class MainCamera;
class Field;
class MainParam;
class CharacterManager;
class MainUI;
class Pause;
class MainScreen : public std::enable_shared_from_this<MainScreen>
{
public:
	MainScreen();
	virtual ~MainScreen();

	// ポインタの取得
	void SetSceneMain(SceneMain* sceneMain) { m_pSceneMain = sceneMain; }

	
	void Init();
	void Update();
	void Draw();

public:

	// ステート
	enum class State
	{
		Init,	// 初期化処理
		Normal,	// 通常処理
		Pause,	// ポーズ処理
		End,	// 終了処理
	};

private:

	// 初期化処理
	void InitEnter();
	void InitUpdate();
	void InitExit();

	// 通常処理
	void NormalEnter();
	void NormalUpdate();
	void NormalDraw();

	// ポーズ処理
	void PauseUpdate();
	void PauseDraw();

	// 終了処理
	void EndEnter();
	void EndUpdate();
	void EndDraw();

private:

	/// <summary>
	/// ステートマシン初期化処理
	/// </summary>
	void InitStateMachine();

	/// <summary>
	/// ロード処理
	/// </summary>
	void Load();

public:

	/// <summary>
	/// ステート変更
	/// </summary>
	/// <param name="state">ステート</param>
	void SetStateChange(const State& state);

	/// <summary>
	/// シーン変更
	/// </summary>
	/// <param name="changeScene">どのシーンに変更するか</param>
	void ChangeScene(const SceneMain::ChangeScene& changeScene) 
	{ m_pSceneMain->FadeOut(changeScene); }

private:


	//////////////////
	// ステート関連 //
	//////////////////

	// ステート
	State m_state;

	// ステートマシンポインタ
	StateMachine<State>m_pStateMachine;

	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// シーンメインポインタ
	SceneMain* m_pSceneMain;

	// メインバックグラウンドポインタ
	std::shared_ptr<MainBackground>m_pMainBackground;

	// メインカメラポインタ
	std::shared_ptr<MainCamera>m_pMainCamera;

	// フィールドポインタ
	std::shared_ptr<Field>m_pField;

	// メインパラㇺポインタ
	std::shared_ptr<MainParam>m_pMainParam;

	// キャラクターマネージャーポインタ
	std::shared_ptr<CharacterManager>m_pCharacterManager;

	// メインUIポインタ
	std::shared_ptr<MainUI>m_pMainUI;

	// ポーズポインタ
	std::shared_ptr<Pause>m_pPause;
};

