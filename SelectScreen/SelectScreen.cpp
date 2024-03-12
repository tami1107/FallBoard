#include "SelectScreen.h"
#include <DxLib.h>
#include "Game.h"
#include <array>
#include "EvoLib.h"
#include "Pad.h"
#include "GameData.h"
#include "Sound.h"


namespace
{
	// グラフィック名
	enum class GraphicName
	{
		Frame,				// 枠
		Title_1,			// 選択タイトル①
		Title_2,			// 選択タイトル②
		PlayerCount,		// プレイヤー人数
		SelectPlayerCount,	// 選択プレイヤー人数
		Level,				// 難易度
		SelectLevel,		// 難易度選択
		Operation_1,		// 操作説明①
		Operation_2,		// 操作説明②
		Background,			// 背景
		PadCountDescription,// パッド数の説明
		LevelDescription,	// 難易度説明
	};

	// グラフィック同士の幅
	const float kGraphicToEachOtherWidth = (Game::kScreenCenterPos.x * 2);

	// 選択タイトルグラフィックのY座標
	constexpr float kSelectTitleGraph_PosY = -340.0f;

	// 人数選択グラフィックの座標
	const Vec2 kPlayerCountGraph = Vec2(-10.0f, 10.0f);

	// 難易度グラフィックのY座標
	constexpr float kLevelGraph_PosY = -70.0f;

	// 操作説明グラフィックのY座標
	constexpr float kOperationDescriptionGraph_PosY = 270.0f;

	// パッドおよび難易度説明グラフィックのY座標
	constexpr float kDescriptionGraph_PosY = 100.0f;


	// グラフィックのファイルパス
	const std::vector<Load_Lib::GraphicInfo>kGraphicInfo
	{
		// 枠
		{
			// グラフィックファイルパス
			"Data/Graphic/Select/frame.png",

			// 座標(X,Y)
			Vec2(
				Game::kScreenCenterPos.x,
				Game::kScreenCenterPos.y
				),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,1),
		},

		// 選択タイトル①
		{
			// グラフィックファイルパス
			"Data/Graphic/Select/selectTitle_1.png",

			// 座標(X,Y)
			Vec2(
				Game::kScreenCenterPos.x,
				Game::kScreenCenterPos.y + kSelectTitleGraph_PosY
				),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,1),
		},

		// 選択タイトル②
		{
			// グラフィックファイルパス
			"Data/Graphic/Select/selectTitle_2.png",

			// 座標(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kGraphicToEachOtherWidth,
				Game::kScreenCenterPos.y + kSelectTitleGraph_PosY
				),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,1),
		
		},

		// プレイヤー人数
		{
			// グラフィックファイルパス
			"Data/Graphic/Select/playerCount.png",

			// 座標(X,Y)
			Vec2(	
				Game::kScreenCenterPos.x + kPlayerCountGraph.x,
				Game::kScreenCenterPos.y + kPlayerCountGraph.y
				),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,4),
		},

		// プレイヤー人数選択
		{
			// グラフィックファイルパス
			"Data/Graphic/Select/playerCountSelect.png",

			// 座標(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kPlayerCountGraph.x,
				Game::kScreenCenterPos.y + kPlayerCountGraph.y
				),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,4),
		},

		// 難易度
		{
			// グラフィックファイルパス
			"Data/Graphic/Select/level.png",

			// 座標(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kGraphicToEachOtherWidth,
				Game::kScreenCenterPos.y + kLevelGraph_PosY
				),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,3),
		},

		// 難易度選択
		{
			// グラフィックファイルパス
			"Data/Graphic/Select/levelSelect.png",

			// 座標(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kGraphicToEachOtherWidth,
				Game::kScreenCenterPos.y + kLevelGraph_PosY
				),

			// グラフィックスケール
			1.1f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,3),
		},

		// 操作説明①
		{
			// グラフィックファイルパス
			"Data/Graphic/Select/operation_1.png",

			// 座標(X,Y)
			Vec2(
				Game::kScreenCenterPos.x,
				Game::kScreenCenterPos.y + kOperationDescriptionGraph_PosY
				),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,1),
		},

		// 操作説明②
		{
			// グラフィックファイルパス
			"Data/Graphic/Select/operation_2.png",

			// 座標(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kGraphicToEachOtherWidth,
				Game::kScreenCenterPos.y + kOperationDescriptionGraph_PosY
				),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,1),
		},

		// 背景
		{
			// 背景
			"Data/Graphic/Select/background.png",

			// 座標(X,Y)
			Vec2(0,0),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,1),
		},

		// パッド数の説明
		{
			// テキスト
			"Data/Graphic/Select/padCountDescription.png",

			// 座標(X,Y)
			Vec2(
				Game::kScreenCenterPos.x,
				Game::kScreenCenterPos.y + kDescriptionGraph_PosY
				),


			// グラフィックスケール
			1.0f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,1),
		},

		// 難易度説明
		{
			// テキスト
			"Data/Graphic/Select/levelDescription.png",

			// 座標(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kGraphicToEachOtherWidth,
				Game::kScreenCenterPos.y + kDescriptionGraph_PosY
				),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数(横,縦)
			Load_Lib::DivNum(1,3),
		},
	};

	// 背景移動スピード
	constexpr float kBackgroundMoveSpeed = 0.5f;
}

namespace
{
	// グラフィックの移動スピード
	constexpr float kGraphMoveSpeed = 60.0f;

	// 項目選択時の移動インターバルの最大値
	constexpr int kIntervalMaximumValue = 15;

	// プレイヤー最大人数
	constexpr int kSelectMaxNum = 4;

	// プレイヤー人数グラフィックの幅
	const Vec2 kPlayerCountGraphDistance = Vec2(25.0f, 0.0f);

	// 難易度選択の最大値
	constexpr int kLevelMaxNum = 3;

	// 難易度選択グラフィックの幅
	const Vec2 kLevelGraphDistance = Vec2(80.0f, 0.0f);

	// サインカーブフレーム
	constexpr int kSinMaxFrameValue = 150;

	// サインカーブの大きさ
	constexpr int kSinMaxValue = 10;
}

namespace
{
	// サウンドファイル名
	std::vector<std::string> kSoundFileName =
	{
		"selectBGM",
		"determination",
		"choose",
		"cancel",
	};

	// サウンド名
	enum class SoundName
	{
		BGM,			// BGM
		Determination,	// 決定音
		Selection,		// 選択音
		Cancel,			// キャンセル音
	};
}

SelectScreen::SelectScreen():
	// グラフィック関連↓
	m_graph(),
	m_basePos(),
	m_targetPos(),
	m_moveValue(),
	// セレクト関連↓
	m_playCountSelectNum(),
	m_levelSelectNum(),
	m_selectInterval(),
	// 演出関連↓
	m_sinFrame(),
	// シーン関連↓
	m_changeScene(),
	// ステート関連↓
	m_state(),
	// クラスポインタ↓
	m_pSceneCharSelect(nullptr),
	m_pStateMachine()
{
}

SelectScreen::~SelectScreen()
{
	// 使用したグラフィックの削除
	for (int i = 0; i < static_cast<int>(m_graph.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_graph[i].handle.size()); j++)
		{
			DeleteGraph(m_graph[i].handle[j]);
		}
	}
}

void SelectScreen::Init()
{
	// 変数の初期化
	{
		m_playCountSelectNum = 0;
		m_levelSelectNum = 0;
		m_selectInterval = 0;
		m_basePos = Vec2(0.0f, 0.0f);
		m_targetPos = Vec2(0.0f, 0.0f);
		m_moveValue = 0.0f;
		m_sinFrame = 0;
	}
	

	// ステートマシンの初期設定処理
	InitStateMachine();

	// グラフィックのロード処理
	LoadHandle();

	

	// サウンドのロード
	Sound::getInstance().Load(kSoundFileName);

	// BGMを再生する
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::BGM)]);
}

void SelectScreen::Update()
{
	// ステートマシンの更新処理
	m_pStateMachine.Update();

	// 選択のインターバル処理
	SelectInterval();

	// グラフィックの移動処理
	MoveGraph();
}

void SelectScreen::Draw()
{
	// ステートマシンの更新処理
	m_pStateMachine.Draw();

	// 背景描画処理
	DrawBackGround();
}

void SelectScreen::PlayCountSelectEnter()
{
	// ターゲット座標決定
	m_targetPos = Vec2(0.0f, 0.0f);
}

void SelectScreen::PlayCountSelectUpdate()
{
	// プレイヤー人数選択
	PlayCountSelect();


	// PADの1ボタンを押したとき、ステートをLevelSelectに変更する
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		// 決定音を再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);

		// ステートをLevelSelectに変更する
		SetStateChange(State::LevelSelect);
	}

	// PADの2ボタンを押したとき、シーン変更変数をタイトルに設定して、ステートをEndに変更する
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		// キャンセル音を再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);

		// シーン変更変数をタイトルに設定する
		m_changeScene = SceneSelect::ChangeScene::Title;

		// ステートをEndに変更する
		SetStateChange(State::End);
	}
}

void SelectScreen::LevelSelectEnter()
{
	// ターゲット座標決定
	m_targetPos = Vec2(-kGraphicToEachOtherWidth, 0.0f);
}

void SelectScreen::LevelSelectUpdate()
{
	// 難易度選択
	LevelSelect();

	// PADの2ボタンを押したとき、ステートをPlayCountSelectに変更する
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		// キャンセル音を再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);

		// ステートをPlayCountSelectに変更する
		SetStateChange(State::PlayCountSelect);
	}

	// PADの1ボタンを押したとき、シーン変更変数をメインに設定して、ステートをEndに変更する
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		// 決定音を再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);

		// シーン変更変数をメインに設定する
		m_changeScene = SceneSelect::ChangeScene::Main;

		// ステートをEndに変更する
		SetStateChange(State::End);
	}
}

void SelectScreen::EndEnter()
{
	// ゲームデータにプレイ人数と難易度を送る
	GameData::getInstance().SetCharData(m_playCountSelectNum + 1,GameData::Level(m_levelSelectNum));
}

void SelectScreen::EndUpdate()
{
	// フェードアウト
	m_pSceneCharSelect->FadeOut(m_changeScene);
}

void SelectScreen::InitStateMachine()
{

	// ステートマシンの初期化、Entry
	auto dummy = []() {};

	// プレイヤー人数選択処理をステートマシンに追加
	{
		auto Enter = [this]() {PlayCountSelectEnter(); };
		auto Update = [this]() {PlayCountSelectUpdate(); };

		m_pStateMachine.AddState(State::PlayCountSelect, Enter, Update, dummy, dummy);
	}

	// プレイヤー人数選択処理をステートマシンに追加
	{
		auto Enter = [this]() {LevelSelectEnter(); };
		auto Update = [this]() {LevelSelectUpdate(); };

		m_pStateMachine.AddState(State::LevelSelect, Enter, Update, dummy, dummy);
	}


	// 終了処理をステートマシンに追加
	{
		auto Enter = [this]() {EndEnter(); };
		auto Update = [this]() {EndUpdate(); };

		m_pStateMachine.AddState(State::End, Enter, Update, dummy, dummy);
	}


	// ステートセット
	SetStateChange(State::PlayCountSelect);
}

void SelectScreen::LoadHandle()
{
	// グラフィック
	SelectScreen::Graphic graphic;

	// グラフィックをロードする
	for (int i = 0; i < static_cast<int>(kGraphicInfo.size()); i++)
	{
		// 分割されたグラフィックをロードする
		std::vector<int>divHandle = Load_Lib::LoadDivGraph_EvoLib_Revision(kGraphicInfo[i].filePath, kGraphicInfo[i].div);

		// グラフィックの代入
		graphic.handle = divHandle;

		// 座標代入
		graphic.pos = kGraphicInfo[i].pos;

		// グラフィックを入れ、要素を増やす
		m_graph.push_back(graphic);
	}
}

void SelectScreen::SelectInterval()
{
	// 選択インターバルを減らす
	m_selectInterval--;

	//! 選択インターバルの値が0以下の場合、0にする
	if (m_selectInterval <= 0)
	{
		// 値を0にする
		m_selectInterval = 0;
	}
}

void SelectScreen::PlayCountSelect()
{
	// インターバルが0以上ならばここで処理を終了する
	if (m_selectInterval != 0)return;

	
	if (Pad::IsPress(PAD_INPUT_RIGHT))// PADの右ボタンを押したとき、m_playCountSelectNumの値を減らす
	{
		// 選択音を再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Selection)]);

		// 値をループさせる
		m_playCountSelectNum = Calculation_Lib::SelectLoopNumber(0, kSelectMaxNum, m_playCountSelectNum);

		// m_selectIntervalにkIntervalMaximumValueの値を代入することによって、m_selectIntervalの値が0になるまでm_selectNumberを変更できなくする
		m_selectInterval = kIntervalMaximumValue;
	}
	else if (Pad::IsPress(PAD_INPUT_LEFT))// PADの左ボタンを押したとき、m_playCountSelectNumの値を増やす
	{
		// 選択音を再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Selection)]);

		// 値を逆ループさせる
		m_playCountSelectNum = Calculation_Lib::SelectLoopNumber(0, kSelectMaxNum, m_playCountSelectNum, true);

		// m_selectIntervalにkIntervalMaximumValueの値を代入することによって、m_selectIntervalの値が0になるまでm_selectNumberを変更できなくする
		m_selectInterval = kIntervalMaximumValue;
	}
}

void SelectScreen::LevelSelect()
{
	// インターバルが0以上ならばここで処理を終了する
	if (m_selectInterval != 0)return;



	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		// 選択音を再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Selection)]);

		// 値をループさせる
		m_levelSelectNum = Calculation_Lib::SelectLoopNumber(0, kLevelMaxNum, m_levelSelectNum);

		// m_selectIntervalにkIntervalMaximumValueの値を代入することによって、m_selectIntervalの値が0になるまでm_selectNumberを変更できなくする
		m_selectInterval = kIntervalMaximumValue;
	}
	else if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		// 選択音を再生する
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Selection)]);

		// 値を逆ループさせる
		m_levelSelectNum = Calculation_Lib::SelectLoopNumber(0, kLevelMaxNum, m_levelSelectNum, true);

		// m_selectIntervalにkIntervalMaximumValueの値を代入することによって、m_selectIntervalの値が0になるまでm_selectNumberを変更できなくする
		m_selectInterval = kIntervalMaximumValue;
	}
}

void SelectScreen::MoveGraph()
{
	// m_basePosがm_targetPosの範囲内かどうかを調べ、範囲内ならば値を代入する
	if (Calculation_Lib::IsTargetRangeValue(m_basePos.x, m_targetPos.x, kGraphMoveSpeed))
	{
		m_basePos.x = m_targetPos.x;

		return;
	}

	// 加算するかどうか
	bool isAdd = false;

	// ベースよりもターゲット座標のほうが大きかった場合加算する
	if (m_basePos.x < m_targetPos.x)
	{
		isAdd = true;
	}

	// 加算するかどうかで処理を変更する
	if (isAdd)
	{
		m_basePos.x += kGraphMoveSpeed;
	}
	else
	{
		m_basePos.x -= kGraphMoveSpeed;
	}
}

void SelectScreen::DrawBackGround()
{

	// 背景描画
	{
		// ループする背景を描画する
		m_moveValue = Draw_Lib::SimpleLoopBackground(m_graph[(int)GraphicName::Background].handle[0], m_moveValue,
			kBackgroundMoveSpeed, Vec2(Game::kScreenWidth, Game::kScreenHeight), true, Draw_Lib::LoopDirection::Go_Top);
	}

	// 枠描画
	{
		// 座標
		const Vec2 pos = Vec2(
			m_graph[(int)GraphicName::Frame].pos.x + m_basePos.x,
			m_graph[(int)GraphicName::Frame].pos.y + m_basePos.y);

		// スケール
		const float scale = kGraphicInfo[(int)GraphicName::Frame].scale;

		// 枠描画1枚目
		DrawRotaGraph((int)pos.x, (int)pos.y, (double)scale, 0.0, m_graph[(int)GraphicName::Frame].handle[0], true);

		// 背景描画2枚目
		DrawRotaGraph((int)(pos.x + kGraphicToEachOtherWidth), (int)pos.y, (double)scale, 0.0, m_graph[(int)GraphicName::Frame].handle[0], true);
	}

	// タイトル描画
	{
		// 座標
		Vec2 pos = Vec2(
			m_graph[(int)GraphicName::Title_1].pos.x + m_basePos.x,
			m_graph[(int)GraphicName::Title_1].pos.y + m_basePos.y);

		// スケール
		float scale = kGraphicInfo[(int)GraphicName::Title_1].scale;


		// タイトル1枚目の描画
		{
			DrawRotaGraph((int)pos.x, (int)pos.y, (double)scale, 0.0, m_graph[(int)GraphicName::Title_1].handle[0], true);
		}
		

		// 座標再設定
		pos = Vec2(
			m_graph[(int)GraphicName::Title_2].pos.x + m_basePos.x,
			m_graph[(int)GraphicName::Title_2].pos.y + m_basePos.y);

		// スケール再設定
		scale = kGraphicInfo[(int)GraphicName::Title_2].scale;

		// タイトル2枚目の描画
		{
			DrawRotaGraph((int)pos.x, (int)pos.y, (double)scale, 0.0, m_graph[(int)GraphicName::Title_2].handle[0], true);
		}
	}


	// アルファブレンドをかける
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

	// プレイヤー人数選択
	{
		// 座標取得
		std::vector<Vec2>pos = Calculation_Lib::GraphicWidthCoordinate(m_graph[(int)GraphicName::PlayerCount].handle[0], kSelectMaxNum,
			kGraphicInfo[(int)GraphicName::PlayerCount].scale, m_graph[(int)GraphicName::PlayerCount].pos, kPlayerCountGraphDistance);

		for (int i = 0; i < kSelectMaxNum; i++)
		{
			if (i == m_playCountSelectNum)
			{
				continue;
			}


			// 人数グラフィックの描画
			DrawRotaGraphF(
				pos[i].x + m_basePos.x, 
				pos[i].y + m_basePos.y, 
				kGraphicInfo[(int)GraphicName::PlayerCount].scale, 0.0, m_graph[(int)GraphicName::PlayerCount].handle[i], true);
		}
	}


	// 強さ選択
	{
		// 座標取得
		std::vector<Vec2>pos = Calculation_Lib::GraphicWidthCoordinate(m_graph[(int)GraphicName::Level].handle[0], kLevelMaxNum,
			kGraphicInfo[(int)GraphicName::Level].scale, m_graph[(int)GraphicName::Level].pos, kLevelGraphDistance);

		for (int i = 0; i < kLevelMaxNum; i++)
		{
			if (i == m_levelSelectNum)
			{
				continue;
			}

			// 強さのグラフィックを描画
			DrawRotaGraphF(
				pos[i].x + m_basePos.x, 
				pos[i].y + m_basePos.y, 
				kGraphicInfo[(int)GraphicName::Level].scale, 0.0, m_graph[(int)GraphicName::Level].handle[i], true);
		}

	}

	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



	// サインカーブ
	{
		m_sinFrame++;

		if (m_sinFrame >= kSinMaxFrameValue)
		{
			m_sinFrame = 0;
		}
	}
	
	// サインカーブの値を求める
	const int sinValue = Calculation_Lib::SineCurve(m_sinFrame, kSinMaxFrameValue, kSinMaxValue);


	// 操作説明
	{
		// 背景描画1枚目
		DrawRotaGraphF(m_graph[(int)GraphicName::Operation_1].pos.x + m_basePos.x, m_graph[(int)GraphicName::Operation_1].pos.y + m_basePos.y,
			kGraphicInfo[(int)GraphicName::Operation_1].scale, 0.0, m_graph[(int)GraphicName::Operation_1].handle[0], true);

		// 背景描画2枚目
		DrawRotaGraphF(m_graph[(int)GraphicName::Operation_2].pos.x + m_basePos.x, m_graph[(int)GraphicName::Operation_2].pos.y + m_basePos.y,
			kGraphicInfo[(int)GraphicName::Operation_2].scale, 0.0, m_graph[(int)GraphicName::Operation_2].handle[0], true);
	}

	{

		// 座標取得
		std::vector<Vec2>pos = Calculation_Lib::GraphicWidthCoordinate(m_graph[(int)GraphicName::PlayerCount].handle[0], kSelectMaxNum,
			kGraphicInfo[(int)GraphicName::PlayerCount].scale, m_graph[(int)GraphicName::PlayerCount].pos, kPlayerCountGraphDistance);

		{
			// 人数
			DrawRotaGraphF((pos[m_playCountSelectNum].x + m_basePos.x), (pos[m_playCountSelectNum].y + m_basePos.y) + sinValue,
				kGraphicInfo[(int)GraphicName::PlayerCount].scale, 0.0, m_graph[(int)GraphicName::PlayerCount].handle[m_playCountSelectNum], true);

			// 選択人数
			DrawRotaGraphF((pos[m_playCountSelectNum].x + m_basePos.x), (pos[m_playCountSelectNum].y + m_basePos.y) + sinValue,
				kGraphicInfo[(int)GraphicName::SelectPlayerCount].scale, 0.0, m_graph[(int)GraphicName::SelectPlayerCount].handle[m_playCountSelectNum], true);
		}


		// 座標取得
		pos = Calculation_Lib::GraphicWidthCoordinate(m_graph[(int)GraphicName::Level].handle[0], kLevelMaxNum,
			kGraphicInfo[(int)GraphicName::SelectLevel].scale, m_graph[(int)GraphicName::Level].pos, kLevelGraphDistance);

		{
			// 強さ
			DrawRotaGraphF((pos[m_levelSelectNum].x + m_basePos.x), (pos[m_levelSelectNum].y + m_basePos.y) + sinValue, 
				kGraphicInfo[(int)GraphicName::SelectLevel].scale, 0.0, m_graph[(int)GraphicName::Level].handle[m_levelSelectNum], true);

			DrawRotaGraphF((pos[m_levelSelectNum].x + m_basePos.x), (pos[m_levelSelectNum].y + m_basePos.y) + sinValue,
				kGraphicInfo[(int)GraphicName::SelectLevel].scale, 0.0, m_graph[(int)GraphicName::SelectLevel].handle[m_levelSelectNum], true);
		}
	}


	// 説明テキスト
	{
		// プレイヤー人数が1人以外の場合、描画
		if (m_playCountSelectNum != 0)
		{

			DrawRotaGraphF((m_graph[(int)GraphicName::PadCountDescription].pos.x + m_basePos.x), (m_graph[(int)GraphicName::PadCountDescription].pos.y), 
				kGraphicInfo[(int)GraphicName::PadCountDescription].scale, 0.0, m_graph[(int)GraphicName::PadCountDescription].handle[0], true);
		}

		// 難易度説明
		DrawRotaGraphF((m_graph[(int)GraphicName::LevelDescription].pos.x + m_basePos.x), (m_graph[(int)GraphicName::LevelDescription].pos.y),
			kGraphicInfo[(int)GraphicName::LevelDescription].scale, 0.0, m_graph[11].handle[m_levelSelectNum], true);
	}

}

void SelectScreen::SetStateChange(const State state)
{
	// ステート代入
	m_state = state;

	// ステートセット
	m_pStateMachine.SetState(m_state);
}
