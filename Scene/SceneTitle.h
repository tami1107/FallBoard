#pragma once
#include "SceneBase.h"
#include <memory>

class TitleScreen;
class SceneTitle : public SceneBase
{
public:
	SceneTitle();

	virtual ~SceneTitle();


	virtual void Init() override;
	virtual void End() override {}

	virtual SceneBase* Update() override;

	virtual void Draw() override;


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
	/// フェードアウト処理
	/// </summary>
	void FadeOut();

private:

	/// <summary>
	/// フェード時のサウンド処理
	/// </summary>
	void FadeSound();

private:

	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// タイトルスクリーンポインタ
	std::shared_ptr<TitleScreen>m_pTitleScreen;

};