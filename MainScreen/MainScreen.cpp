#include "StateMachine.h"// ※一番上に書く
#include "MainScreen.h"
#include <DxLib.h>
#include "Pad.h"
#include "Sound.h"
#include "MainBackground.h"
#include "MainCamera.h"
#include "Field.h"
#include "CharacterManager.h"
#include "ModelData.h"
#include "MainParam.h"
#include "MainUI.h"
#include "GameData.h"
#include "Pause.h"

MainScreen::MainScreen():
	// ステート関連↓
	m_state(),
	// クラスポインタ関連↓
	m_pSceneMain(nullptr),
	m_pMainBackground(std::make_shared<MainBackground>()),
	m_pMainCamera(std::make_shared<MainCamera>()),
	m_pField(std::make_shared<Field>()),
	m_pCharacterManager(std::make_shared<CharacterManager>()),
	m_pMainParam(std::make_shared<MainParam>()),
	m_pMainUI(std::make_shared<MainUI>()),
	m_pPause(std::make_shared<Pause>())
{
}

MainScreen::~MainScreen()
{
	// ポインタの削除
	m_pSceneMain = nullptr;
	delete m_pSceneMain;
}

void MainScreen::Init()
{
	// ステートマシン初期化処理
	InitStateMachine();
}

void MainScreen::Update()
{
	// ステートマシンの更新処理
	m_pStateMachine.Update();
}

void MainScreen::Draw()
{
	// ステートマシンの描画処理
	m_pStateMachine.Draw();
}

void MainScreen::InitStateMachine()
{
	// ステートマシンの初期化、Entry
	auto dummy = []() {};

	// 初期化処理をステートマシンに追加
	{
		auto initEnter = [this]() {InitEnter(); };
		auto initUpdate = [this]() {InitUpdate(); };
		auto initDraw = dummy;
		auto initExit = [this]() {InitExit(); };

		m_pStateMachine.AddState(State::Init, initEnter, initUpdate, initDraw, initExit);
	}

	// 通常処理をステートマシンに追加
	{
		auto normalEnter = [this]() {NormalEnter(); };
		auto normalUpdate = [this]() {NormalUpdate(); };
		auto normalDraw = [this]() {NormalDraw(); };
		auto normalExit = dummy;

		m_pStateMachine.AddState(State::Normal, normalEnter, normalUpdate, normalDraw, normalExit);
	}

	// ポーズ処理をステートマシンに追加
	{
		auto pauseEnter = dummy;
		auto pauseUpdate = [this]() {PauseUpdate(); };
		auto pauseDraw = [this]() {PauseDraw(); };
		auto pauseExit = dummy;

		m_pStateMachine.AddState(State::Pause, pauseEnter, pauseUpdate, pauseDraw, pauseExit);
	}


	// 終了処理をステートマシンに追加
	{
		auto endEnter = [this]() {EndEnter(); };
		auto endUpdate = [this]() {EndUpdate(); };
		auto endDraw = [this]() {EndDraw(); };
		auto endExit = dummy;

		m_pStateMachine.AddState(State::End, endEnter, endUpdate, endDraw, endExit);
	}


	// ステートセット
	SetStateChange(State::Init);
}

void MainScreen::Load()
{
	// 非同期のロードを行う
	m_pSceneMain->IsASynchronousLoad(true);

	// ゲームプレイ用のモデルをロードする
	ModelData::getInstance().LoadGamePlayModel();

	// 各クラスのハンドルロード
	m_pField->LoadHandle();
	m_pMainBackground->LoadHandle();
	m_pMainUI->LoadHandle();
	m_pPause->LoadHandle();
}

void MainScreen::SetStateChange(const State& state)
{
	// ステート代入
	m_state = state;

	// ステートセット
	m_pStateMachine.SetState(m_state);
}

void MainScreen::InitEnter()
{
	// ロード処理
	Load();
}

void MainScreen::InitUpdate()
{
	// ロードをしているかどうかを受け取る
	const bool isLoading = m_pSceneMain->GetLoading();

	// ロード中フラグがfalseの時、Normalステートに変更する
	if (!isLoading)
	{
		// ステートセット
		SetStateChange(State::Normal);
	}
}


void MainScreen::InitExit()
{
	// ポインタ渡し
	{
		// キャラクターマネージャーにポインタを送る
		m_pCharacterManager->SetMainScreenPointer(shared_from_this());
		m_pCharacterManager->SetFieldPointer(m_pField);
		m_pCharacterManager->SetMainParamPointer(m_pMainParam);
		m_pCharacterManager->SetMainUIPointer(m_pMainUI);

		// メインカメラにポインタを送る
		m_pMainCamera->SetMainParamPointer(m_pMainParam);

		// メインUIにポインタを送る
		m_pMainUI->SetMainParamPointer(m_pMainParam);

		// フィールドにポインタを送る
		m_pField->SetMainParamPointer(m_pMainParam);

		// ポーズに自身のポインタを送る
		m_pPause->SetMainScreenPointer(shared_from_this());
	}

	// 各クラスの初期化処理
	{
		m_pMainParam->Init();
		m_pMainBackground->Init();
		m_pField->Init();
		m_pMainCamera->Init();
		m_pCharacterManager->Init();
		m_pMainUI->Init();
		m_pPause->Init();
	}
}

void MainScreen::NormalEnter()
{
	// カメラ描画
	m_pMainCamera->Update();
}

void MainScreen::NormalUpdate()
{
	// 各クラスの更新処理
	m_pMainBackground->Update();
	m_pMainCamera->Update();
	m_pField->Update();
	m_pCharacterManager->Update();
	m_pMainUI->Update();
	m_pPause->Update();
}

void MainScreen::NormalDraw()
{
	// 各クラスの描画処理
	m_pMainBackground->Draw();
	m_pField->Draw();
	m_pCharacterManager->Draw();
	m_pMainUI->Draw();
}



void MainScreen::PauseUpdate()
{
	// ポーズ更新処理
	m_pPause->Update();
}

void MainScreen::PauseDraw()
{
	// 描画処理
	m_pMainBackground->Draw(true);
	m_pField->Draw();
	m_pCharacterManager->Draw();
	m_pMainUI->Draw();

	// ポーズ描画処理
	m_pPause->Draw();
}

void MainScreen::EndEnter()
{
	// シーンメインにフェードアウト処理をさせる
	m_pSceneMain->FadeOut(SceneMain::ChangeScene::Result);
}

void MainScreen::EndUpdate()
{
	// カメラ更新処理
	m_pMainCamera->Update();
}

void MainScreen::EndDraw()
{
	// 描画処理
	m_pMainBackground->Draw();
	m_pField->Draw();
	m_pCharacterManager->Draw();
	m_pMainUI->Draw();
}