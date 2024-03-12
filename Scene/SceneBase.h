#pragma once
#include "StateMachine.h"// ※これは一番上においておく
#include <memory>

// ゲームシーン基底クラス
class Loading;
class SceneBase
{
public:
	// フェード速度
	static constexpr int kFadeSpeed = 5;
	static constexpr int kVolumeSpeed = 5;

public:
	SceneBase();
	virtual ~SceneBase() {};

	// クラスポインタの取得
	void setLoadingPointer(std::shared_ptr<Loading>loading) { m_pLoading = loading; }


	virtual void Init() {}
	virtual void End() {}

	virtual SceneBase* Update() { return this; }
	virtual void Draw() {}


	// ロード関連
	void LoadingInit(bool isLoading = false);
	bool IsLoadingProcess();




	// フェード関連
	void UpdateFade();// フェード更新
	void DrawFade(int fadeColor = 0x000000) const;// フェード描画
	bool IsFadingIn() const;// フェードイン中
	bool IsFadingOut() const;// フェードアウト中
	bool IsFading() const { return IsFadingIn() || IsFadingOut(); }// フェードインorアウト中

	void StartFadeOut();// フェードアウト開始

	// フェードスピードの変更
	void SetFadeSpeed(int fadeSetUpSpeed = kFadeSpeed);

	// フェードの値を返す
	int FadeBrightValue() { return m_fadeBright; }

	// シーン終了
	virtual bool isEnd() { return false; }

private:
	// クラスポインタ
	std::shared_ptr<Loading>m_pLoading;

	

protected:
	// フェード関連処理
	int m_fadeBright;
	int m_fadeSpeed;
	int m_fadeSetUpSpeed;


	bool m_isFadeOut;// フェードアウトを行っているかどうか

	bool m_isBackScene;

	bool m_isLoading;// ロードしているかどうか


};