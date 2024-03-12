#include "StateMachine.h"// ※一番上に書く
#include "ResultScreen.h"
#include "Pad.h"
#include "GameData.h"
#include "ResultCharacter.h"
#include "ResultCamera.h"
#include "ResultBackground.h"
#include <DxLib.h>
#include "Sound.h"

namespace
{
	// サウンドファイル名
	const std::vector<std::string> kSoundFileName =
	{
		"win",			// 勝利音
		"resultBGM",	// リザルトBGM
		"determination"	// 決定音
	};

	// サウンド名
	enum class SoundName
	{
		Win,			// 勝利音
		ResultBGM,		// リザルトBGM
		Determination,	// 決定音
	};
}

ResultScreen::ResultScreen() :
	// ステート関連↓
	m_state(),
	// クラスポインタ関連↓
	m_pSceneResult(nullptr),
	m_pResultCharacter(std::make_shared<ResultCharacter>()),
	m_pResultCamera(std::make_shared<ResultCamera>()),
	m_pResultBackground(std::make_shared<ResultBackground>())
{
}

ResultScreen::~ResultScreen()
{
	// ポインタの削除
	m_pSceneResult = nullptr;
	delete m_pSceneResult;

	// モデル削除
	ModelData::getInstance().DeleteGamePlayModel();

	// サウンドアンロード
	Sound::getInstance().UnLoad();
}

void ResultScreen::Init()
{
	// ステート初期化処理
	InitState();

	// リザルトバックグラウンドにリザルトキャラクターポインタを送る
	m_pResultBackground->SetResultCharacterPointer(m_pResultCharacter);

	// ロード処理
	Load();

	

	// 勝利SEを再生する
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Win)]);

	// リザルトBGMを再生する
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::ResultBGM)]);
}

void ResultScreen::Update()
{
	// ステートマシンの更新処理
	m_pStateMachine.Update();
	// モデル更新処理
	m_pResultCharacter->Update();
	// カメラ更新処理
	m_pResultCamera->Update();
	// 背景更新処理
	m_pResultBackground->Update();
}

void ResultScreen::Draw()
{
	// 背景描画処理
	m_pResultBackground->Draw();

	// モデル描画処理
	m_pResultCharacter->Draw();

	// ランキング描画処理
	m_pResultBackground->DrawRanking();

	// クラウン描画処理
	m_pResultBackground->DrawCrown();

	// エフェクト描画処理
	m_pResultBackground->DrawEffect();

	// 次のシーンの説明テキスト描画処理
	m_pResultBackground->DrawNextSceneText();
}

void ResultScreen::NormalEnter()
{
	// 各クラスの初期化処理
	m_pResultCharacter->Init();
	m_pResultCamera->Init();
	m_pResultBackground->Init();
}

void ResultScreen::NormalUpdate()
{
	// ボタンを押したとき、フェードイン中でなければステートを変更する
	if (Pad::IsTrigger(PAD_INPUT_1)&&
		!m_pSceneResult->IsFadingIn())
	{
		// 決定音を再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);

		// Endステートをセット
		m_pStateMachine.SetState(State::End);
	}
}


void ResultScreen::EndEnter()
{
	// フェードアウト処理
	m_pSceneResult->FadeOut();
}

void ResultScreen::Load()
{
	// サウンドロード
	Sound::getInstance().Load(kSoundFileName);
}

void ResultScreen::InitState()
{
	// ステートマシンの初期化、Entry
	auto dummy = []() {};

	// 通常処理をステートマシンに追加
	{
		auto normalEnter = [this]() {NormalEnter(); };
		auto normalUpdate = [this]() {NormalUpdate(); };
		auto normalDraw = dummy;
		auto normalExit = dummy;

		// ステート設定
		m_pStateMachine.AddState(State::Normal, normalEnter, normalUpdate, normalDraw, normalExit);
	}

	// 終了処理をステートマシンに追加
	{
		auto endEnter = [this]() {EndEnter(); };
		auto endUpdate = dummy;
		auto endDraw = dummy;
		auto endExit = dummy;

		// ステート設定
		m_pStateMachine.AddState(State::End, endEnter, endUpdate, endDraw, endExit);
	}

	// ステートセット
	SetStateChange(State::Normal);
}

void ResultScreen::SetStateChange(const State& state)
{
	// ステート代入
	m_state = state;

	// ステートセット
	m_pStateMachine.SetState(m_state);
}
