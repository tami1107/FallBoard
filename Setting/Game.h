#pragma once
#include "Vec2.h"


namespace Game
{
	// ウインドウモード設定
	constexpr bool kWindowMode = true;

	// ウインドウ名
	const char* const kTitleText = "FallBoard";

	// ウインドウサイズ
	constexpr int kScreenWidth = 1800;
	constexpr int kScreenHeight = 900;


	// カラーモード
	constexpr int kColorDepth = 32;		// 32 or 16

	// ウィンドウの中心座標
	const Vec2 kScreenCenterPos = Vec2((static_cast<float>(kScreenWidth)) * 0.5f, (static_cast<float>(kScreenHeight)) * 0.5f);
};