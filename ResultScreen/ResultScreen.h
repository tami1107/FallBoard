#pragma once
#include "SceneResult.h"
#include <vector>
#include <memory>

// ステートマシン
template <class TState> class StateMachine;

class ResultCharacter;
class ResultCamera;
class ResultBackground;
class ResultScreen
{
public:
	ResultScreen();
	virtual ~ResultScreen();


	/// <summary>
	/// シーンリザルトのポインタを取得
	/// </summary>
	/// <param name="sceneResult">シーンリザルトポインタ</param>
	void SetSceneMain(SceneResult* sceneResult) { m_pSceneResult = sceneResult; }

	void Init();
	void Update();
	void Draw();

public:

	// ステート
	enum class State
	{
		Normal,	// 通常処理
		End,	// 終了処理
	};

private:

	// 通常処理
	void NormalEnter();
	void NormalUpdate();

	// 終了処理
	void EndEnter();

private:

	/// <summary>
	/// ロード処理
	/// </summary>
	void Load();

	/// <summary>
	/// ステート初期化処理
	/// </summary>
	void InitState();

public:

	/// <summary>
	/// ステート変更処理
	/// </summary>
	/// <param name="state">ステート</param>
	void SetStateChange(const State& state);

private:

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

	// シーンリザルトポインタ
	SceneResult* m_pSceneResult;

	// リザルトキャラクターポインタ
	std::shared_ptr<ResultCharacter>m_pResultCharacter;

	// リザルトカメラポインタ
	std::shared_ptr<ResultCamera>m_pResultCamera;

	// リザルトバックグラウンドポインタ
	std::shared_ptr<ResultBackground>m_pResultBackground;
};