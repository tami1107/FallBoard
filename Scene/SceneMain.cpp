#include "StateMachine.h"// ※これは一番上においておく

#include "SceneMain.h"
#include "SceneResult.h"
#include "Player.h"
#include "Sound.h"
#include "MainScreen.h"
#include "SceneSelect.h"
#include "SceneTitle.h"
#include "ModelData.h"

namespace
{
	// フェードインアウトのスピード
	constexpr int kFadeSpeed = 6;
}

SceneMain::SceneMain():
	// シーン関連↓
	m_changeScene(),
	// クラスポインタ関連↓
	m_pMainScreen(std::make_shared<MainScreen>())
{
}

SceneMain::~SceneMain()
{
	// サウンドのアンロード
	Sound::getInstance().UnLoad();
}

void SceneMain::Init()
{
	// フェードのスピードを代入する
	SetFadeSpeed(kFadeSpeed);

	////////////////////////////////////////////
	// これよ下から各クラスの初期化処理を書く //
	////////////////////////////////////////////

	// 自身のポインタをメインスクリーンクラスに送る
	m_pMainScreen->SetSceneMain(this);

	// メインスクリーンの初期化処理
	m_pMainScreen->Init();
}

SceneBase* SceneMain::Update()
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

			// シーン変更がリザルト以外ならば、プレイモデルを削除する
			if (m_changeScene != SceneMain::ChangeScene::Result)
			{
				ModelData::getInstance().DeleteGamePlayModel();
			}


			// m_changeSceneの中身によって次のシーンを変更する
			switch (m_changeScene)
			{
			case SceneMain::ChangeScene::CharSelect:

				// シーンセレクトへ移行
				return (new SceneSelect);

				break;
			case SceneMain::ChangeScene::Main:

				// シーンメインへ移行
				return (new SceneMain);

				break;
			case SceneMain::ChangeScene::Result:

				// シーンリザルトへ移行
				return (new SceneResult);

				break;
			case SceneMain::ChangeScene::Title:

				// シーンタイトルへ移行
				return (new SceneTitle);

				break;

			default:
				// m_changeSceneの中身が上記のcase以外でも処理を継続して行えるように、シーンリザルトへ移行
				return (new SceneResult);

				break;
			}
		}
	}

	//////////////////////////////////////////
	// これより下に各クラスの更新処理を書く //
	//////////////////////////////////////////


	// メインスクリーンの更新処理
	m_pMainScreen->Update();


	return this;
}

void SceneMain::Draw()
{
	// メインスクリーンの描画処理
	m_pMainScreen->Draw();


	//////////////////////////////////////////
	// これより上に各クラスの描画処理を書く //
	//////////////////////////////////////////

	// フェードインアウトのフィルター
	DrawFade();
}

void SceneMain::IsASynchronousLoad(bool isLoading)
{
	// ロードを行う
	LoadingInit(isLoading);
}

bool SceneMain::GetLoading()
{
	// ロード中かどうか
	const bool isLoading = IsLoadingProcess();

	// ロードを行っているかどうかを返す
	return isLoading;
}

void SceneMain::FadeOut(ChangeScene changeScene)
{
	// 現在、フェードを行っていなければフェードアウトの初期設定を行い、実行する
	if (!IsFading())
	{
		// 値を代入
		m_changeScene = changeScene;

		// フェードアウトを実行
		StartFadeOut();	
	}
}

void SceneMain::FadeSound()
{
	// フェードアウト中であれば、そのフェードに合わせてBGMを小さくする
	if (m_isFadeOut)
	{
		// フェードアウトが終わると同時に音が止まる
		Sound::getInstance().ScreenFadeBGMStop(FadeBrightValue());
	}
}

