#include "MainBackground.h"
#include <DxLib.h>
#include "Game.h"
#include "EvoLib.h"
#include "GameData.h"

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

	// グラフィック情報
	const std::vector<GraphicInfo>kGraphicInfo =
	{
		{
			"Data/Graphic/BackGround/BackGround_0.png",

			// 座標
			Vec2(0.0f,0.0f),

			1,2,
		},

		{
			"Data/Graphic/BackGround/BackGround_1.png",

			// 座標
			Vec2(0.0f,0.0f),

			1,2,
		},

		{
			"Data/Graphic/BackGround/BackGround_2.png",

			// 座標
			Vec2(0.0f,0.0f),

			1,2,
		},
	};


	// グラフィックタイプ
	enum class GraphType
	{
		Stop,	// 停止
		Move	// 移動
	};


	// 画像の移動スピード
	constexpr float kBackgroundMoveSpeed = 1.0f;
}

MainBackground::MainBackground():
	// グラフィック関連↓
	m_graph(),
	m_level(),
	m_moveValue()
{
}

MainBackground::~MainBackground()
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

void MainBackground::Init()
{
	// 移動量の初期化
	m_moveValue = 0.0f;


	// 難易度を取得
	m_level = GameData::getInstance().GetLevel();
}

void MainBackground::Update()
{
}

void MainBackground::Draw(bool isPause)
{

	// 背景描画
	Draw_Lib::DrawSimpleBackground(m_graph[static_cast<int>(m_level)].handle[static_cast<int>(GraphType::Stop)], Vec2(Game::kScreenWidth, Game::kScreenHeight));


	// ポーズ中ならば、移動量を0にして背景を描画し、処理を終える
	if (isPause)
	{
		// 背景描画
		m_moveValue = Draw_Lib::SimpleLoopBackground(m_graph[static_cast<int>(m_level)].handle[static_cast<int>(GraphType::Move)], m_moveValue, 0.0f,
			Vec2(Game::kScreenWidth, Game::kScreenHeight),true);

		// ここで処理を終了する
		return;
	}

	// 動く背景描画
	m_moveValue = Draw_Lib::SimpleLoopBackground(m_graph[static_cast<int>(m_level)].handle[static_cast<int>(GraphType::Move)], m_moveValue, kBackgroundMoveSpeed,
		Vec2(Game::kScreenWidth, Game::kScreenHeight),true);
	
}

void MainBackground::LoadHandle()
{
	// リソースの読み込み
	SetUseASyncLoadFlag(true);	// 非同期読み込みを行いたい


	// 要素を増やす
	m_graph.resize(static_cast<int>(kGraphicInfo.size()));

	// グラフィックの数、処理を繰り返しグラフィックを挿入する
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