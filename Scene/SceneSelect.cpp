#include "SceneSelect.h"
#include "Pad.h"
#include "Game.h"
#include "Sound.h"
#include "SelectScreen.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include <DxLib.h>

namespace
{
	// フェードインアウトのスピード
	constexpr int kFadeSpeed = 6;
}

SceneSelect::SceneSelect():
	// シーン変更関連↓
	m_changeScene(),
	// クラスポインタ関連↓
    m_pSelectScreen(std::make_shared<SelectScreen>())
{
}

SceneSelect::~SceneSelect()
{
	// サウンドのアンロード
	Sound::getInstance().UnLoad();
}

void SceneSelect::Init()
{
	// フェードのスピードを代入する
	SetFadeSpeed(kFadeSpeed);


	////////////////////////////////////////////
	// これよ下から各クラスの初期化処理を書く //
	////////////////////////////////////////////

	// 自身のポインタをセレクトスクリーンクラスに送る
	m_pSelectScreen->SetSceneCharSelectPointer(this);

	// セレクトスクリーンの初期化処理
	m_pSelectScreen->Init();
}

SceneBase* SceneSelect::Update()
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

			// m_changeSceneの中身によって次のシーンを変更する
			switch (m_changeScene)
			{
			case SceneSelect::ChangeScene::Main:

				// シーンメインへ移行
				return (new SceneMain);

				break;

			case SceneSelect::ChangeScene::Title:

				// シーンタイトルへ移行
				return (new SceneTitle);

				break;

			default:

				// m_changeSceneの中身が上記のcase以外でも処理を継続して行えるように、シーンメインへ移行
				return (new SceneMain);

				break;
			}
		}
	}


	//////////////////////////////////////////
	// これより下に各クラスの更新処理を書く //
	//////////////////////////////////////////


	// セレクトスクリーンの更新処理
	m_pSelectScreen->Update();

    return this;
}

void SceneSelect::Draw()
{
	// キャラセレクトスクリーンの更新処理
	m_pSelectScreen->Draw();



	//////////////////////////////////////////
	// これより上に各クラスの更新処理を書く //
	//////////////////////////////////////////

	// フェードインアウトのフィルター
	DrawFade();
}

void SceneSelect::IsASynchronousLoad(bool isLoading)
{
	// ロードを行う
	LoadingInit(isLoading);
}

bool SceneSelect::GetLoading()
{
	// ロードを行っているかどうか
	const bool isLoading = IsLoadingProcess();

	// ロードを行っているかどうかを返す
	return isLoading;
}

void SceneSelect::FadeOut(ChangeScene changeScene)
{
	// 現在、フェードを行っていなければフェードアウトの初期設定を行い、実行する
	if (!IsFading())
	{
		// どのシーンに変更するかを代入する
		m_changeScene = changeScene;

		// フェードアウトを実行
		StartFadeOut();
	}
}

void SceneSelect::FadeSound()
{
	// フェードアウト中であれば、そのフェードに合わせてBGMを小さくする
	if (m_isFadeOut)
	{
		// フェードアウトが終わると同時に音が止まる
		Sound::getInstance().ScreenFadeBGMStop(FadeBrightValue());
	}
}