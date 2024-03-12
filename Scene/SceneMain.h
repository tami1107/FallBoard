#pragma once

#include "SceneBase.h"
#include <memory>
#include "Vec2.h"


class MainScreen;
class SceneMain : public SceneBase
{
public:
	SceneMain();

	virtual ~SceneMain();


	virtual void Init() override;
	virtual void End() override {}

	virtual SceneBase* Update() override;
	virtual void Draw() override;

public:

	// ステート名
	enum class State
	{
		Fade,			// フェード
		Initialization, // 初期化
		Normal,			// 通常
	};

	// シーン変更名
	enum class ChangeScene
	{
		CharSelect,	// キャラセレクト
		Main,		// メイン
		Result,		// リザルト
		Title,		// タイトル
	};


public:

	/// <summary>
	/// ロードを行うかどうか
	/// </summary>
	/// <param name="isLoading">ロードを行うかどうか</param>
	void IsASynchronousLoad(bool isLoading = false);

	/// <summary>
	/// ローディング処理をしているかどうかを返す
	/// </summary>
	/// <returns>ローディング処理をしているかどうかを返す</returns>
	bool GetLoading();

	/// <summary>
	/// フェードアウトを行う
	/// </summary>
	/// <param name="changeScene">フェードアウト時、どのシーンに変更するか</param>
	void FadeOut(ChangeScene changeScene);

private:

	/// <summary>
	/// フェードアウト時、だんだん音が小さくなるサウンド処理
	/// </summary>
	void FadeSound();

private:

	////////////////
	// シーン関連 //
	////////////////

	// シーン変更
	ChangeScene m_changeScene;

	////////////////////
	// クラスポインタ //
	////////////////////

	// メインスクリーンのクラスポインタ
	std::shared_ptr<MainScreen>m_pMainScreen;
};