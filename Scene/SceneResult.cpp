#include "SceneResult.h"
#include "Pad.h"
#include "SceneSelect.h"
#include "Game.h"
#include "ResultScreen.h"
#include "Sound.h"
#include <DxLib.h>


SceneResult::SceneResult() :
	// クラスポインタ関連↓
	m_pResultScreen(std::make_shared<ResultScreen>())
{
}

SceneResult::~SceneResult()
{
	// サウンドのアンロード
	Sound::getInstance().UnLoad();
}

void SceneResult::Init()
{
	// 自身のポインタを送る
	m_pResultScreen->SetSceneMain(this);

    // フェードのスピードを代入する
    SetFadeSpeed();


	////////////////////////////////////////////
	// これよ下から各クラスの初期化処理を書く //
	////////////////////////////////////////////

	// リザルトの初期化処理
	m_pResultScreen->Init();
}

SceneBase* SceneResult::Update()
{
	// フェードインアウトしている
	if (IsFading())
	{
		m_isFadeOut = IsFadingOut();
		SceneBase::UpdateFade();

		// フェード時のサウンド処理
		FadeSound();

		// フェードアウト終了時
		if (!IsFading() && m_isFadeOut && !m_isBackScene)
		{
			// シーンセレクトへシーンを移行する
			return (new SceneSelect);
		}
	}

	//////////////////////////////////////////
	// これより下に各クラスの更新処理を書く //
	//////////////////////////////////////////

	// リザルトの更新処理
	m_pResultScreen->Update();


    return this;
}

void SceneResult::Draw()
{
	// リザルトの更新処理
	m_pResultScreen->Draw();

	//////////////////////////////////////////
	// これより上に各クラスの描画処理を書く //
	//////////////////////////////////////////

	// フェードインアウトのフィルター
	DrawFade();
}

void SceneResult::IsASynchronousLoad(bool isLoading)
{
	// ロードを行う
	LoadingInit(isLoading);
}

bool SceneResult::GetLoading()
{
	// ロード中かどうか
	const bool isLoading = IsLoadingProcess();

	// ロードを行っているかどうかを返す
	return isLoading;
}

void SceneResult::FadeOut()
{
	// 現在、フェードを行っていなければフェードアウトの初期設定を行い、実行する
	if (!IsFading())
	{
		// フェードアウトを実行
		StartFadeOut();
	}
}

void SceneResult::FadeSound()
{
	// フェードアウト中であれば、そのフェードに合わせてBGMを小さくする
	if (m_isFadeOut)
	{
		// フェードアウトが終わると同時に音が止まる
		Sound::getInstance().ScreenFadeBGMStop(FadeBrightValue());
	}
}
