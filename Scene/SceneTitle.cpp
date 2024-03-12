#include "SceneTitle.h"
#include "SceneSelect.h"
#include "Sound.h"
#include "Pad.h"
#include "TitleScreen.h"
#include <DxLib.h>


// サウンド
namespace SoundFile
{
	// サウンドファイル名
	std::vector<std::string> kSoundFileName =
	{
		"taitleBGM",	// タイトルBGM
		"determination",// 決定音
	};

	// サウンド名
	enum class SoundName
	{
		TaitleBGM,		// タイトルBGM
		Determination	// 決定音
	};
}

SceneTitle::SceneTitle():
	// クラスポインタ関連↓
	m_pTitleScreen(std::make_shared<TitleScreen>())
{
}

SceneTitle::~SceneTitle()
{
	// サウンドのアンロード
	Sound::getInstance().UnLoad();
}

void SceneTitle::Init()
{
	// フェードのスピードを代入する
	SetFadeSpeed();

	// サウンドをロードする
	Sound::getInstance().Load(SoundFile::kSoundFileName);

	// タイトルBGMを再生する
	Sound::getInstance().Play(SoundFile::kSoundFileName[static_cast<int>(SoundFile::SoundName::TaitleBGM)]);

	////////////////////////////////////////////
	// これよ下から各クラスの初期化処理を書く //
	////////////////////////////////////////////

	// タイトルスクリーンの初期化処理
	m_pTitleScreen->Init();
}

SceneBase* SceneTitle::Update()
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
			return (new SceneSelect);
		}
	}

	// ボタンを押したとき、フェードアウト処理を行う
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		// フェードアウト処理
		FadeOut();
	}

	//////////////////////////////////////////
	// これより下に各クラスの更新処理を書く //
	//////////////////////////////////////////

	// タイトルスクリーン更新処理
	m_pTitleScreen->Update();

	return this;
}

void SceneTitle::Draw()
{
	// タイトルスクリーン描画処理
	m_pTitleScreen->Draw();

	//////////////////////////////////////////
	// これより上に各クラスの描画処理を書く //
	//////////////////////////////////////////

	// フェードインアウトのフィルター
	SceneBase::DrawFade();
}

void SceneTitle::IsASynchronousLoad(bool isLoading)
{
	// ロードを行う
	LoadingInit(isLoading);
}

bool SceneTitle::GetLoading()
{
	// ロード中かどうか
	const bool isLoading = IsLoadingProcess();

	// ロードを行っているかどうかを返す
	return isLoading;
}

void SceneTitle::FadeOut()
{
	// 現在、フェードを行っていなければフェードアウトの初期設定を行い、実行する
	if (!IsFading())
	{
		// 決定音を再生する
		Sound::getInstance().Play(SoundFile::kSoundFileName[static_cast<int>(SoundFile::SoundName::Determination)]);

		// フェードアウトを実行
		StartFadeOut();
	}
}

void SceneTitle::FadeSound()
{
	// フェードアウト中であれば、そのフェードに合わせてBGMを小さくする
	if (m_isFadeOut)
	{
		// フェードアウトが終わると同時に音が止まる
		Sound::getInstance().ScreenFadeBGMStop(FadeBrightValue());
	}
}
