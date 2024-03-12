#pragma once
#include "SceneBase.h"


class SelectScreen;
class SceneSelect : public SceneBase
{
public:
	SceneSelect();

	virtual ~SceneSelect();


	virtual void Init() override;
	virtual void End() override {}

	virtual SceneBase* Update() override;
	virtual void Draw() override;


public:

	// シーン変更名
	enum class ChangeScene
	{
		Main,		// メイン
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
	/// <param name="changeScene">フェードアウト終了時、どのシーンに変更するか</param>
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

	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	//  セレクトスクリーンのクラスポインタ
	std::shared_ptr<SelectScreen>m_pSelectScreen;
};

