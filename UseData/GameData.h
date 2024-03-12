#pragma once
#include "Pad.h"
#include "ModelData.h"
#include <DxLib.h>
#include <vector>


// シングルトンを使用したゲームデータクラス
class GameData
{
public:

	// キャラクターステータス
	struct CharacterStatus
	{
		// プレイヤーかどうか
		bool isPlayer = true;

		// キャラクター番号
		int charNumber = 0;

		// 色
		VECTOR color = VGet(0.0f, 0.0f, 0.0f);

		// 体力
		int lifePoint = 0;

		// 移動速度
		float moveSpeed = 0;

		// 攻撃硬直時間
		int attackRigorTime = 0;

		// 復帰時の無敵時間
		int invincibleTime = 0;

		// ランキング
		int rank = 0;
	};


protected:

	// 使用モデル番号
	static constexpr int kUseModelNumber = 1;

	// プレイヤーの最大数
	static constexpr int kPlayerMaxCount = 4;

private:
	
	GameData() = default;

	virtual ~GameData() = default;

	static GameData* m_instance;

public:

	// コピーコンストラクタの禁止
	GameData(const GameData&) = delete;
	GameData& operator=(const GameData&) = delete;
	GameData(GameData&&) = delete;
	GameData& operator=(GameData&&) = delete;

	static GameData& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new GameData;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	// 難易度
	enum class Level
	{
		Easy,	// 簡単
		Noramal,// 普通
		Hard,	// 難しい
	};

public:

	/// <summary>
	/// キャラデータをセットする
	/// </summary>
	/// <param name="playerNum">プレイヤーの人数</param>
	/// <param name="level">難易度</param>
	void SetCharData(const int& playerNum, const Level& level);

	/// <summary>
	/// キャラデータを返す
	/// </summary>
	/// <returns>キャラデータ</returns>
	std::vector<CharacterStatus> GetCharData() { return m_characterStatus; }

	/// <summary>
	/// ゲームプレイ人数を返す
	/// </summary>
	/// <returns>ゲームプレイ人数</returns>
	int GetGamePlayCount() { return kPlayerMaxCount; }

	/// <summary>
	/// キャラの最大体力を返す
	/// </summary>
	/// <returns></returns>
	int GetMaxLifePoint() { return m_maxLifePoint; }

	/// <summary>
	/// 使用するモデルの番号を返す
	/// </summary>
	/// <returns>使用するモデルの番号</returns>
	int GetModelNumber() { return kUseModelNumber; }

	/// <summary>
	/// プレイヤーカラーを返す
	/// </summary>
	/// <param name="charNumber">キャラクターナンバー</param>
	/// <returns>プレイヤーカラー</returns>
	VECTOR GetPlayerColor(const int& charNumber){ return m_characterStatus[charNumber].color; }


	/// <summary>
	/// 順位を受け取る
	/// </summary>
	/// <param name="charNumber">キャラ番号</param>
	/// <param name="rank">順位/param>
	void SetCharRank(const int& charNumber, const int& rank);

	/// <summary>
	/// キャラクターのランクを取得
	/// </summary>
	/// <param name="charNumber">キャラの番号</param>
	/// <returns></returns>
	int GetCharRank(const int& charNumber) { return m_characterStatus[charNumber].rank; }

	/// <summary>
	/// 難易度を返す
	/// </summary>
	/// <returns>難易度</returns>
	Level GetLevel() { return m_level; }

private:

	// 最大体力
	int m_maxLifePoint = 0;

	// 難易度
	Level m_level = Level::Easy;

	// キャラクターステータス
	std::vector<CharacterStatus>m_characterStatus;
};

