#include "StateMachine.h"// ※これは一番上においておく
#include "Loading.h"
#include <DxLib.h>
#include "Game.h"
#include "EvoLib.h"

namespace
{
	// 最低限のロードフレーム
	constexpr int kMinimumFrame = 60;

	// フェードスピード
	constexpr int kFadeSpeed = 30;

	// フェード最大値
	constexpr int kFadeMaxValue = 255;

	// フェードカラー
	constexpr int kFadeColor= 0x000000;
}

// グラフィック
namespace Graph
{
	// グラフィックファイルパス
	const char* const kFilePath = "Data/Graphic/Load/loadText.png";

	// グラフィックの座標
	const Vec2 kPos = Vec2(1500.0f, 800.0f);

	// グラフィックスケール
	constexpr float kGraphScale = 1.0f;

	// サインカーブフレーム
	constexpr int kSinMaxFrameValue = 150;

	// サインカーブの大きさ
	constexpr int kSinMaxValue = 20;
}

Loading::Loading():
	// グラフィック関連↓
	m_handle(),
	m_sinFrame(),
	m_fadeBright(),
	// ロード関連↓
	m_frameCount(),
	m_isMinimumFrame(),
	m_isLoading()
{
}

Loading::~Loading()
{
	// グラフィックの削除
	DeleteGraph(m_handle);
}

void Loading::Init()
{
	// ロードハンドル
	LoadHanele();

	// 変数の初期化
	m_frameCount = 0;
	m_isMinimumFrame = false;
	m_isLoading = true;
	m_fadeBright = 0;
	m_sinFrame = 0;
}

void Loading::Update()
{
	// フェード値を加算
	UpdateFadeBright();

	// ロードフレーム処理
	LoadFrame();

	// 非同期処理の数を数える
	ASyncLoadingCount();
}

void Loading::Draw()
{
	// 透明度を設定する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);

	// 画面を覆うBoxを描画する
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kFadeColor, true);
	
	// ロードグラフィックを描画する
	DrawLoadGaph();

	// 透明度を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Loading::LoadHanele()
{
	// グラフィックをロードする
	m_handle = LoadGraph(Graph::kFilePath);
}

void Loading::UpdateFadeBright()
{
	// フェード値を加算
	m_fadeBright += kFadeSpeed;

	// フェード値がkFadeMaxValueを超えた場合、kFadeMaxValueの値を代入する
	if (m_fadeBright >= kFadeMaxValue)
	{
		m_fadeBright = kFadeMaxValue;
	}
}

bool Loading::GetIsLoading()
{
	// 最低ロードフレームを超えていて、非同期処理をしていなければ、falseを返す
	if (m_isMinimumFrame && !m_isLoading)
	{
		return false;
	}

	// trueを返す
	return true;
}

void Loading::LoadFrame()
{
	// フレームの加算
	m_frameCount++;

	// フレームが最低限のロードフレームを超えていたらフラグをtrueにする
	if (m_frameCount >= kMinimumFrame)
	{
		m_isMinimumFrame = true;
	}
}

void Loading::ASyncLoadingCount()
{
	// 非同期処理している個数を数える
	const int ASyncLoadCount = GetASyncLoadNum();

	// 非同期処理している個数が0ならば、フラグをfalseにする
	if (ASyncLoadCount == 0)
	{
		m_isLoading = false;
	}
}

void Loading::DrawLoadGaph()
{
	// サインフレーム
	{
		// サインフレームを加算する
		m_sinFrame++;

		// サインフレームがkSinMaxFrameValueを超えた場合、0を代入する
		if (m_sinFrame >= Graph::kSinMaxFrameValue)
		{
			m_sinFrame = 0;
		}
	}

	// サイン値を求める
	const int sinValue = Calculation_Lib::SineCurve(m_sinFrame, Graph::kSinMaxFrameValue, Graph::kSinMaxValue);

	// グラフィックを描画する
	DrawRotaGraphF(Graph::kPos.x, Graph::kPos.y + sinValue, Graph::kGraphScale, 0.0, m_handle, true);
}
