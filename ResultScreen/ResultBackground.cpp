#include "ResultBackground.h"
#include "Game.h"
#include "EvoLib.h"
#include "GameData.h"
#include "Field.h"
#include "ResultCharacter.h"

namespace
{
	// グラフィックのファイルパス
	const std::vector<Load_Lib::GraphicInfo>kGraphicInfo
	{
		// 背景
		{
			// グラフィックファイルパス
			"Data/Graphic/Result/backGround.png",

			// 座標
			Vec2(0 ,0),

			// グラフィックスケール
			0.0f,

			// グラフィックの分割数
			{1,1},
		},

		// リザルトテキスト
		{
			// グラフィックファイルパス
			"Data/Graphic/Result/resultText.png",

			// 座標
			Vec2(Game::kScreenCenterPos.x, 180.0f),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数
			{1,1},
		},

		// ランキング
		{
			// グラフィックファイルパス
			"Data/Graphic/Result/rank.png",

			// 座標
			Vec2(-5.0f,10.0f),

			// グラフィックスケール
			5.0f,

			// グラフィックの分割数
			{4,1},
		},

		// クラウン
		{
			// グラフィックファイルパス
			"Data/Graphic/Result/crown.png",

			// 座標
			Vec2(Game::kScreenCenterPos.x ,Game::kScreenCenterPos.y + 6.0f),

			// グラフィックスケール
			0.8f,

			// グラフィックの分割数
			{1,1},
		},

		// 次のシーンの説明グラフィック
		{
			// グラフィックファイルパス
			"Data/Graphic/Result/nextSceneText.png",

			// 座標
			Vec2(Game::kScreenCenterPos.x , 800.0f),

			// グラフィックスケール
			1.0f,

			// グラフィックの分割数
			{1,1},
		},
	};

	// グラフィック名
	enum class GraphName
	{
		BackGround,		// 背景
		ResultText,		// リザルトテキスト
		Rank,			// ランキング
		Crown,			// クラウン
		NextSceneText,	// 次のシーンへの説明テキスト
	};

	
}


// 背景
namespace Background
{
	// 移動スピード
	constexpr float kMoveSpeed = 1.0f;
}

// エフェクト
namespace Effect
{
	// エフェクトの個数
	constexpr int kCount = 255;

	// ランダム値
	constexpr int kRandValue = 20;
}

// リザルトテキスト
namespace ResultText
{
	// サインカーブフレーム
	constexpr int kSinMaxFrameValue = 150;

	// サインカーブの大きさ
	constexpr int kSinMaxValue = 5;
}

// クラウン
namespace Crown
{
	// サインカーブフレーム
	constexpr int kSinMaxFrameValue = 100;

	// サインカーブの大きさ
	constexpr int kSinMaxValue = 20;
}

// 次のシーンへの説明テキスト
namespace NextSceneText
{
	// フラッシュフレーム
	constexpr int kFlashingMaxFrameValue = 100;

	// フラッシュの大きさ
	constexpr int kFlashingSinMaxValue = 255;
}

ResultBackground::ResultBackground():
	// グラフィック関連↓
	m_graph(),
	// 演出関連↓
	m_backgroundMoveValue(),
	m_resultTextSinFrame(),
	m_crownSinFrame(),
	m_flashingFrame(),
	m_effect(),
	// クラスポインタ関連↓
	m_pField(std::make_shared<Field>()),
	m_pResultCharacter()
{
}

ResultBackground::~ResultBackground()
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

void ResultBackground::Init()
{
	// グラフィックのロード
	LoadHandle();

	// フィールドハンドルのロード
	m_pField->LoadHandle(false);

	// フィールド初期化処理
	m_pField->Init(false);

	// 値の初期化
	m_backgroundMoveValue = 0.0f;
	m_resultTextSinFrame = 0;
	m_crownSinFrame = 0;
	m_flashingFrame = 0;

	// エフェクト初期化処理
	InitEffect();
}

void ResultBackground::Update()
{
	// エフェクト更新処理
	UpdateEffect();
}

void ResultBackground::Draw()
{
	// 背景描画処理
	DrawBackground();

	// 盤面描画処理
	m_pField->Draw();

	// リザルトテキスト描画処理
	DrawResultText();
}

void ResultBackground::LoadHandle()
{
	// グラフィック変数をkGraphicInfoの数、要素を増やす
	m_graph.resize(static_cast<int>(kGraphicInfo.size()));

	// 変数にグラフィックを代入する
	for (int i = 0; i < static_cast<int>(kGraphicInfo.size()); i++)
	{
		// グラフィック情報代入
		Load_Lib::DivGraphInfo graphInfo = 
			Load_Lib::LoadDivGraph_EvoLib(kGraphicInfo[i].filePath, kGraphicInfo[i].div.x, kGraphicInfo[i].div.y);

		// グラフィック代入
		m_graph[i].handle = graphInfo.handle;

		// 座標代入
		m_graph[i].pos = kGraphicInfo[i].pos;
	}
}

void ResultBackground::InitEffect()
{
	// エフェクトの要素をkEffectNum分、増やす
	m_effect.resize(Effect::kCount);

	// すべてのエフェクトに値を代入する
	for (auto& effect : m_effect)
	{
		effect.pos.x = static_cast<float>(GetRand(Game::kScreenWidth));
		effect.pos.y = static_cast<float>(-GetRand(Game::kScreenHeight));

		effect.vec.x = 0.0f;
		effect.vec.y = static_cast<float>(GetRand(Effect::kRandValue) + Effect::kRandValue);
		effect.vec.y /= 10.0f;
	}
}

void ResultBackground::UpdateEffect()
{
	// エフェクトを動かす
	for (auto& effect : m_effect)
	{
		// 移動量分、座標を加算する
		effect.pos += effect.vec;

		// エフェクトY座標が画面外まで進んだ場合、初期化を行う
		if (effect.pos.y > Game::kScreenHeight)
		{
			effect.pos.x = static_cast<float>(GetRand(Game::kScreenWidth));
			effect.pos.y = 0.0f;

			effect.vec.x = 0.0f;
			effect.vec.y = static_cast<float>(GetRand(Effect::kRandValue) + Effect::kRandValue);
			effect.vec.y /= 10.0f;
		}
	}
}

void ResultBackground::DrawBackground()
{
	// 動く背景を描画する
	m_backgroundMoveValue =
		Draw_Lib::SimpleLoopBackground(
			m_graph[static_cast<int>(GraphName::BackGround)].handle[0],
			m_backgroundMoveValue,
			Background::kMoveSpeed,
			Vec2(Game::kScreenWidth, Game::kScreenHeight),
			true,
			Draw_Lib::LoopDirection::Go_Top);
}

void ResultBackground::DrawResultText()
{
	// サインカーブ
	{
		// サインフレームを加算する
		m_resultTextSinFrame++;

		// サインフレームがkSinMaxFrameValueの値を超えた場合、0を代入する
		if (m_resultTextSinFrame >= ResultText::kSinMaxFrameValue)
		{
			m_resultTextSinFrame = 0;
		}
	}

	// サイン値を求める
	const int sinValue = Calculation_Lib::SineCurve(m_resultTextSinFrame, ResultText::kSinMaxFrameValue, ResultText::kSinMaxValue);

	// リザルトテキスト描画
	DrawRotaGraphF(
		m_graph[static_cast<int>(GraphName::ResultText)].pos.x,
		m_graph[static_cast<int>(GraphName::ResultText)].pos.y + sinValue,
		static_cast<double>(kGraphicInfo[static_cast<int>(GraphName::ResultText)].scale),
		0.0,
		m_graph[static_cast<int>(GraphName::ResultText)].handle[0],
		true);
}

void ResultBackground::DrawEffect()
{
	// エフェクトを描画
	for (auto& effect : m_effect)
	{
		// 割合を求める
		const float rate = (effect.vec.length() / 4.0f);

		// エフェクトを描画する
		DrawCircleAA(
			effect.pos.x,
			effect.pos.y,
			4.0f * rate,
			32,
			GetColor(
				static_cast <int>(255.0f * rate),
				static_cast <int>(255.0f * rate),
				static_cast <int>(32 * rate)),
			true);
	}
}

void ResultBackground::DrawRanking()
{
	
	// モデルの座標を取得
	const std::vector<VECTOR>modelPos = m_pResultCharacter->GetModelPos();

	// ランクを入れる変数
	int rank = 0;

	// ランキンググラフィックを描画する
	for (int i = 0; i < static_cast<int>(modelPos.size()); i++)
	{
		// キャラクターのランクを取得
		rank = GameData::getInstance().GetCharRank(i);

		// ３Ｄ空間上にランキング画像を描画
		DrawBillboard3D(
			VGet(
				modelPos[i].x + m_graph[static_cast<int>(GraphName::Rank)].pos.x,
				modelPos[i].y + m_graph[static_cast<int>(GraphName::Rank)].pos.y,
				modelPos[i].z),
			0.5f, 
			0.5f, 
			kGraphicInfo[static_cast<int>(GraphName::Rank)].scale,
			0.0f, 
			m_graph[static_cast<int>(GraphName::Rank)].handle[rank - 1],
			true);
	}
}

void ResultBackground::DrawCrown()
{

	// サインカーブ
	{
		// サインフレームを加算する
		m_crownSinFrame++;

		// サインフレームがkSinMaxFrameValueを超えた場合、0を代入する
		if (m_crownSinFrame >= Crown::kSinMaxFrameValue)
		{
			m_crownSinFrame = 0;
		}
	}

		
	// サイン値を求める
	const int sinValue = Calculation_Lib::SineCurve(m_crownSinFrame, Crown::kSinMaxFrameValue, Crown::kSinMaxValue);

	// プレイ人数を代入する
	const int playCount = GameData::getInstance().GetGamePlayCount();

	// 座標取得
	std::vector<Vec2>pos = 
		Calculation_Lib::GraphicWidthCoordinate(
			m_graph[static_cast<int>(GraphName::Crown)].handle[0],
			playCount,
			kGraphicInfo[static_cast<int>(GraphName::Crown)].scale, 
			Game::kScreenCenterPos, 
			Vec2(190.0f, 0.0f));

	// ランキングを入れる変数
	int rank = 0;

	for (int i = 0; i < playCount; i++)
	{
		// ランクを取得
		rank = GameData::getInstance().GetCharRank(i);

		// ランクが1以外ならば、continueする
		if (rank != 1)continue;

		// クラウンを描画する
		DrawRotaGraphF(
			pos[i].x,
			pos[i].y + sinValue,
			static_cast<double>(kGraphicInfo[static_cast<int>(GraphName::Crown)].scale),
			0.0, 
			m_graph[static_cast<int>(GraphName::Crown)].handle[0], true);
	}
}

void ResultBackground::DrawNextSceneText()
{
	// 点滅フレームを加算する
	m_flashingFrame++;

	// 点滅フレームがkFlashingMaxFrameValueを超えた場合、0を代入する
	if (m_flashingFrame >= NextSceneText::kFlashingMaxFrameValue)
	{
		m_flashingFrame = 0;
	}

	// 透明度を求める
	const int alphaValue = Calculation_Lib::SineCurve(m_flashingFrame, NextSceneText::kFlashingMaxFrameValue, NextSceneText::kFlashingSinMaxValue, true);

	// 透明度を設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);

	// グラフィックを描画する
	DrawRotaGraphF(
		kGraphicInfo[static_cast<int>(GraphName::NextSceneText)].pos.x,
		kGraphicInfo[static_cast<int>(GraphName::NextSceneText)].pos.y,
		static_cast<double>(kGraphicInfo[static_cast<int>(GraphName::NextSceneText)].scale),
		0.0,
		m_graph[static_cast<int>(GraphName::NextSceneText)].handle[0],
		true);

	// 透明度をもとに戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}