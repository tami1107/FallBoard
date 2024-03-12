#include "TitleScreen.h"
#include <DxLib.h>

namespace
{
	// ファイルパス
	const char* const kFilePath = "Data/Graphic/Title/titleMovie.mp4";

	// 動画の再生フレーム
	constexpr int kMovePlayMaxFrame = 6600;
}


TitleScreen::TitleScreen():
	// グラフィック関連↓
	m_handle(),
	m_frame()
{
}

TitleScreen::~TitleScreen()
{
	// グラフィックの削除
	DeleteGraph(m_handle);
}

void TitleScreen::Init()
{
	// グラフィックのロード
	m_handle = LoadGraph(kFilePath);

	// フレーム初期化
	m_frame = 0;

	// ムービーを再生状態にします
	PlayMovieToGraph(m_handle);
}

void TitleScreen::Update()
{
	// フレームを加算する
	m_frame++;

	// 値がkMovePlayMaxFrameの値を超えた場合、0を代入する
	if (m_frame > kMovePlayMaxFrame)
	{
		// ムービーの再生位置を0秒目に変更
		SeekMovieToGraph(m_handle, 0);

		// 0を代入
		m_frame = 0;
	}
}

void TitleScreen::Draw()
{
	// 動画の再生
	DrawGraph(0, 0, m_handle, false);
}
