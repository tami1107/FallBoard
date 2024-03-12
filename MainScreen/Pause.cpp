#include "StateMachine.h"// ※一番上に書く
#include "Pause.h"
#include "Pad.h"
#include <DxLib.h>
#include "MainScreen.h"
#include "Game.h"
#include "EvoLib.h"
#include "SoundSetting.h"
#include "Sound.h"


namespace
{
	// グラフィック
	struct GraphicInfo
	{
		// ファイルパス
		const char* filePath = "";

		// 座標
		Vec2 pos = Vec2(0.0f, 0.0f);

		// グラフィックの分割数
		const int div_x = 0;
		const int div_y = 0;
	};


	// グラフィックのファイルパス
	const std::vector<GraphicInfo>kGraphicInfo
	{
		// 背景
		{
			// グラフィックファイルパス
			"Data/Graphic/Pause/frame.png",

			// 座標
			Game::kScreenCenterPos,

			// グラフィックの分割数
			1,1,
		},

		// 選択テキスト
		{
			// グラフィックファイルパス
			"Data/Graphic/Pause/pauseSelect.png",

			// 座標
			Vec2(Game::kScreenCenterPos.x - 180, Game::kScreenCenterPos.y - 230),

			// グラフィックの分割数
			1,5,
		},

		// 矢印
		{
			// グラフィックファイルパス
			"Data/Graphic/Pause/arrow.png",

			// 座標
			Vec2(Game::kScreenCenterPos.x - 250, Game::kScreenCenterPos.y - 235),

			// グラフィックの分割数
			1,1,
		},
	};

	// グラフィック幅
	constexpr int kPauseSelectGraphDistance = 100;

	// グラフィック名
	enum class GraphName
	{
		Frame,		// フレーム
		SerectText,	// セレクトテキスト
		Arrow,		// 矢印
	};
}

namespace
{
	// 選択項目
	enum class SelectedItem
	{
		BackToGame,			// ゲームに戻る
		RetryFromBeginning,	// 初めからやりなおす
		BackSelectScene,	// セレクトシーンに戻る
		SoundSetting,		// 音量調整
		BackTitleScene,		// タイトルシーンに戻る
		SelectedItemNum		// 選択項目の合計
	};


	// 選択時のカラー
	const Draw_Lib::Color kSelectColor = { 255.0f, 0.0f, 0.0f };

	// 項目選択時の移動インターバルの最大値
	constexpr int kIntervalMaximumValue = 15;

	// サインカーブフレーム
	constexpr int kSinMaxFrameValue= 150;

	// サインカーブの大きさ
	constexpr int kSinMaxValue = 5;
}

namespace
{
	// サウンドファイル名
	const std::vector<std::string> kSoundFileName =
	{
		"pause",			// ポーズ音
		"determination",	// 決定音
		"choose",			// 選択音
		"cancel",			// キャンセル音
	};

	// サウンド名
	enum class SoundName
	{
		Pause,			// ポーズ音
		Determination,	// 決定音
		Select,			// セレクト音
		Cancel,			// キャンセル音
	};
}


Pause::Pause():
	// グラフィック関連↓
	m_graph(),
	// セレクト関連↓
	m_selectNum(),
	m_selectInterval(),
	m_isScreenChange(),
	m_isSoundSetting(),
	// サインカーブ関連
	m_sinFrame(),
	// ステート関連↓
	m_state(),
	// クラスポインタ関連↓
	m_pMainScreen()
{
}

Pause::~Pause()
{
	// グラフィックの削除
	for (int i = 0; i < static_cast<int>(m_graph.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_graph[i].handle.size()); j++)
		{
			DeleteGraph(m_graph[i].handle[j]);
		}
	}
}

void Pause::Init()
{
	// ステートマシン初期設定
	InitStateMachine();

	// 変数の初期化
	{
		m_selectNum = static_cast<int>(SelectedItem::BackToGame);
		m_selectInterval = 0;
		m_isScreenChange = false;
		m_sinFrame = 0;
	}

	// サウンドロード
	Sound::getInstance().Load(kSoundFileName);
}

void Pause::Update()
{
	// ボタンを押したとき、ポーズするかどうか
	if (Pad::IsTrigger(PAD_INPUT_8))
	{
		// ステートがNoramalだった場合、ステートをPauseに変更する
		if (m_state == State::Normal)
		{
			// ポーズSEを再生
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Pause)]);

			// Pauseステートセット
			SetStateChange(State::Pause);

			// メインスクリーンにPauseステートに変更することを送る
			m_pMainScreen->SetStateChange(MainScreen::State::Pause);
		}
		else
		{
			// キャンセルSEを再生
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);

			// Normalステートセット
			SetStateChange(State::Normal);
			
			// メインスクリーンにNormalステートに変更することを送る
			m_pMainScreen->SetStateChange(MainScreen::State::Normal);
		}
	}

	// ステートマシンの更新処理
	m_pStateMachine.Update();
}

void Pause::Draw()
{
	// ステートマシンの描画処理
	m_pStateMachine.Draw();
}

void Pause::PauseEnter()
{
	// 初期化
	m_isSoundSetting = false;
}

void Pause::PauseUpdate()
{
	// シーン変更中ならばここで処理を終了する
	if (m_isScreenChange)return;

	// ポーズ選択処理
	PauseSelect();

	// 選択項目決定処理
	SelectDetermination();

	// サウンド設定更新処理
	UpdateSoundSetting();
}

void Pause::PauseDraw()
{
	// ポーズウィンドウ描画処理
	PauseDrawWindow();

	// ポーズ選択グラフィック描画処理
	PauseDrawSelectGraph();

	// サウンド設定描画処理
	DrawSoundSetting();
}

void Pause::PauseExit()
{
	// 選択番号の初期化
	m_selectNum = static_cast<int>(SelectedItem::BackToGame);
}

void Pause::SetStateChange(const State& state)
{
	// ステート代入
	m_state = state;

	// ステートセット
	m_pStateMachine.SetState(m_state);
}

void Pause::LoadHandle()
{
	// リソースの読み込み
	SetUseASyncLoadFlag(true);	// 非同期読み込みを行いたい

	// m_graphの要素をkGraphicInfoの分、増やす
	m_graph.resize(static_cast<int>(kGraphicInfo.size()));

	// グラフィック情報をkGraphicInfoの分、代入する
	for (int i = 0; i < static_cast<int>(kGraphicInfo.size()); i++)
	{
		// グラフィック情報代入
		Load_Lib::DivGraphInfo graphInfo = Load_Lib::LoadDivGraph_EvoLib(kGraphicInfo[i].filePath, kGraphicInfo[i].div_x, kGraphicInfo[i].div_y);

		// グラフィック代入
		m_graph[i].handle = graphInfo.handle;

		// 座標代入
		m_graph[i].pos = kGraphicInfo[i].pos;
	}

	// リソースの読み込み
	SetUseASyncLoadFlag(false);	// デフォルトに戻しておく
}

void Pause::InitStateMachine()
{
	// ステートマシンの初期化、Entry
	auto dummy = []() {};

	// 通常処理をステートマシンに追加
	{
		auto normalEnter = dummy;
		auto normalUpdate = dummy;
		auto normalDraw = dummy;
		auto normalExit = dummy;

		// 処理をセットする
		m_pStateMachine.AddState(State::Normal, normalEnter, normalUpdate, normalDraw, normalExit);
	}

	// ポーズ処理をステートマシンに追加
	{
		auto pauseEnter = [this]() {PauseEnter(); };
		auto pauseUpdate = [this]() {PauseUpdate(); };
		auto pauseDraw = [this]() {PauseDraw(); };
		auto pauseExit = [this]() {PauseExit(); };

		// 処理をセットする
		m_pStateMachine.AddState(State::Pause, pauseEnter, pauseUpdate, pauseDraw, pauseExit);
	}

	// ステートセット
	SetStateChange(State::Normal);
}

void Pause::PauseSelect()
{
	// サウンド設定フラグがtrueの場合、処理を終了する
	if (m_isSoundSetting)
	{
		return;
	}

	// 選択の移動インターバル処理
	{
		// セレクトインターバルの値を減らす
		m_selectInterval--;

		// セレクトインターバルの値が0以下の場合、0を代入する
		if (m_selectInterval <= 0)
		{
			m_selectInterval = 0;
		}
	}

	// インターバルが0以上ならばここで処理を終了する
	if (m_selectInterval != 0)return;

	if (Pad::IsPress(PAD_INPUT_DOWN))
	{
		// 値をループさせる
		m_selectNum = Calculation_Lib::SelectLoopNumber(0, static_cast<int>(SelectedItem::SelectedItemNum), m_selectNum);

		// 選択SEを再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Select)]);

		// m_selectIntervalにkIntervalMaximumValueの値を代入することによって、m_selectIntervalの値が0になるまでm_selectNumberを変更できなくする
		m_selectInterval = kIntervalMaximumValue;
	}
	else if (Pad::IsPress(PAD_INPUT_UP))
	{
		// 値を逆ループさせる
		m_selectNum = Calculation_Lib::SelectLoopNumber(0, static_cast<int>(SelectedItem::SelectedItemNum), m_selectNum, true);

		// 選択SEを再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Select)]);

		// m_selectIntervalにkIntervalMaximumValueの値を代入することによって、m_selectIntervalの値が0になるまでm_selectNumberを変更できなくする
		m_selectInterval = kIntervalMaximumValue;
	}
}

void Pause::PauseDrawWindow()
{

	// ポーズウィンドウセロファン(黒い)
	SetDrawBlendMode(DX_BLENDMODE_MULA, 100);

	// 画面を全体的に暗くする
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight,
		0x000000, true);

	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	// ポーズフレームを描画
	DrawRotaGraphF(
		m_graph[static_cast<int>(GraphName::Frame)].pos.x, 
		m_graph[static_cast<int>(GraphName::Frame)].pos.y,
		1.0, 0.0, m_graph[static_cast<int>(GraphName::Frame)].handle[0], true);
}

void Pause::PauseDrawSelectGraph()
{
	// サインカーブ
	{
		// サインフレームの値を増やす
		m_sinFrame++;

		// サインフレームの値がkSinMaxFrameValueの値を超えた場合、0を代入する
		if (m_sinFrame >= kSinMaxFrameValue)
		{
			m_sinFrame = 0;
		}
	}


	// 選択テキストのグラフィックの数、処理を繰り返す
	for (int i = 0; i < static_cast<int>(m_graph[static_cast<int>(GraphName::SerectText)].handle.size()); i++)
	{
		// 選択番号とiが同じ場合、continueを行う
		if (m_selectNum == i)
		{
			continue;
		}

	
		// 選択テキストを描画する
		DrawGraphF(
			m_graph[static_cast<int>(GraphName::SerectText)].pos.x,
			m_graph[static_cast<int>(GraphName::SerectText)].pos.y + (kPauseSelectGraphDistance * i),
			m_graph[static_cast<int>(GraphName::SerectText)].handle[i], true);
	}

	// SetDrawBrightを使用して、色を変更する
	SetDrawBright(static_cast<int>(kSelectColor.r), static_cast<int>(kSelectColor.g), static_cast<int>(kSelectColor.b));

	// サインカーブの値を求める
	const int sinValue = Calculation_Lib::SineCurve(m_sinFrame, kSinMaxFrameValue, kSinMaxValue);

	// 選択テキストを描画する
	DrawGraphF(
		m_graph[static_cast<int>(GraphName::SerectText)].pos.x, 
		(m_graph[static_cast<int>(GraphName::SerectText)].pos.y + (kPauseSelectGraphDistance * m_selectNum) + sinValue), 
		m_graph[static_cast<int>(GraphName::SerectText)].handle[m_selectNum], true);

	// SetDrawBrightを使用して、色をもとに戻す
	SetDrawBright(255, 255, 255);


	// 矢印を描画する
	DrawGraphF(
		m_graph[static_cast<int>(GraphName::Arrow)].pos.x, 
		(m_graph[static_cast<int>(GraphName::Arrow)].pos.y + (kPauseSelectGraphDistance * m_selectNum)), 
		m_graph[static_cast<int>(GraphName::Arrow)].handle[0], true);

}

void Pause::SelectDetermination()
{
	// 決定ボタンかサウンド設定フラグがtrueの場合、処理を終了する
	if (!Pad::IsPress(PAD_INPUT_1) || m_isSoundSetting == true)
	{
		return;
	}


	// 決定SEを再生する
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);

	// 選択番号から選択項目を調べる
	const SelectedItem selectedItem = SelectedItem(m_selectNum);


	// 選択項目によって処理を変更する
	switch (selectedItem)
	{
	case SelectedItem::BackToGame:

		// Normalステートセット
		SetStateChange(State::Normal);

		// メインスクリーンにNormalステート情報を送る
		m_pMainScreen->SetStateChange(MainScreen::State::Normal);

		break;
	case SelectedItem::RetryFromBeginning:

		// メインスクリーンへメインシーンに変更する情報を送る
		m_pMainScreen->ChangeScene(SceneMain::ChangeScene::Main);

		// シーン変更フラグをtrueにする
		m_isScreenChange = true;

		break;
	case SelectedItem::BackSelectScene:

		// メインスクリーンへシーンセレクトに変更する情報を送る
		m_pMainScreen->ChangeScene(SceneMain::ChangeScene::CharSelect);

		// シーン変更フラグをtrueにする
		m_isScreenChange = true;

		break;
	case SelectedItem::SoundSetting:

		// 音量設定フラグをtrueにする
		m_isSoundSetting = true;

		break;

	case SelectedItem::BackTitleScene:

		// メインスクリーンへタイトルシーンに変更する情報を送る
		m_pMainScreen->ChangeScene(SceneMain::ChangeScene::Title);

		// シーン変更フラグをtrueにする
		m_isScreenChange = true;

		break;
	default:
		break;
	}
}

void Pause::UpdateSoundSetting()
{
	// 音量設定中でなければ、処理を行わない
	if (!m_isSoundSetting)return;

	// サウンド設定を閉じる
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		// キャンセルSEを再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);

		// サウンド設定フラグをfalseにする
		m_isSoundSetting = false;
	}

	// 更新
	SoundSetting::getInstance().Update();
}

void Pause::DrawSoundSetting()
{
	// 音量設定中でなければ処理を行わない
	if (!m_isSoundSetting)return;

	// サウンド設定画面を描画する
	SoundSetting::getInstance().Draw();
}
