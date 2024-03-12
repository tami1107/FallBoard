#pragma once
#include <vector>
#include "Vec2.h"
#include "GameData.h"

class MainBackground
{
public:
	MainBackground();
	virtual ~MainBackground();

	void Init();
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="isPause">ポーズ中かどうか</param>
	void Draw(bool isPause = false);

public:

	// ハンドルのロード
	void LoadHandle();

private:

	// グラフィック
	struct Graphic
	{
		// グラフィックハンドル
		std::vector<int>handle;

		// 座標
		Vec2 pos = Vec2(0.0f, 0.0f);
	};


private:

	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// グラフィック
	std::vector<Graphic>m_graph;

	// 難易度
	GameData::Level m_level;

	// 移動量
	float m_moveValue;
};

