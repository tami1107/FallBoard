#include "MainParam.h"
#include <DxLib.h>
#include "EvoLib.h"
#include "GameData.h"


namespace
{
	// カウントダウン秒数
	constexpr int kCountDownSecond = 4;

	// カウントダウンフレーム(0秒も入れたいので +1 を行う)
	constexpr int kCountDownFrame = FPS_Lib::FPS * (kCountDownSecond + 1);
}

MainParam::MainParam() :
	// 登場関連↓
	m_pop(),
	// キャラパラメータ関連↓
	m_charInfo(),
	// 盤面関連↓
	m_board(),
	// ステート関連↓
	m_state()
{
}

MainParam::~MainParam()
{
}

void MainParam::Init()
{
	// カウントダウンのフレームを代入
	m_pop.countDownFrame = kCountDownFrame;

	// プレイ人数分要素を増やす
	m_charInfo.resize(GameData::getInstance().GetGamePlayCount());
}

int MainParam::CountDown()
{
	// カウントダウンフレームを減らす
	m_pop.countDownFrame--;

	// カウントダウンフレームが0以下の場合、ここで処理を終了する。
	if (m_pop.countDownFrame <= 0)
	{
		m_pop.countDownFrame = 0;
	}

	// フレームを返す
	return m_pop.countDownFrame;
}

bool MainParam::GetIsCountDownEnd()
{
	// FPSを代入
	const int fps = FPS_Lib::FPS;

	// ゲームスタートフレーム
	const int gameStartFrame = static_cast<int>(fps * 0.5f);

	// カウントダウンのフレームがゲームスタートフレーム以下の場合、カウントダウン終了とみなす
	if (m_pop.countDownFrame <= gameStartFrame)
	{
		return true;
	}

	// フラグを返す
	return false;
}

void MainParam::SetCharLife(const int& charNumber, const int& lifePoint)
{
	// 体力の代入
	m_charInfo[charNumber].lifePoint = lifePoint;


	// 体力が0以下の場合、死亡判定
	if (m_charInfo[charNumber].lifePoint <= 0)
	{
		// 順位計算
		const int rank = RankCalculation();

		// キャラの順位を送る
		GameData::getInstance().SetCharRank(charNumber, rank);

		// 死亡判定をtrueにする
		m_charInfo[charNumber].isDie = true;
	}
}

void MainParam::SetIsCharDamage(const int& charNumber, const bool& isDamage)
{
	// ダメージフラグを代入
	 m_charInfo[charNumber].isDamage = isDamage; 
}


void MainParam::BoardDropCount()
{
	// 落下させる回数を増やす
	m_board.dropCount++;
}

void MainParam::BoardDropEnd()
{
	// 落下させる回数を減らす
	m_board.dropCount--;

	// 落下させる回数が0以下の場合、0にする
	if (m_board.dropCount <= 0)
	{
		m_board.dropCount = 0;
	}
}

int MainParam::RankCalculation()
{
	// 死んでいる人数
	int dieCount = 0;

	// プレイヤー人数
	const int playCount = static_cast<int>(m_charInfo.size());

	// 死んでいる人数を数える
	for (int i = 0; i < playCount; i++)
	{
		if (m_charInfo[i].isDie)
		{
			dieCount++;
		}
	}

	// プレイ人数から死んでいる人数を引いてランキングを求める
	const int rank = playCount - dieCount;

	// ランクを返す
	return rank;
}