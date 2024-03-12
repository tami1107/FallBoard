#include "GameData.h"
#include "ModelData.h"
#include "MasterData.h"

// シングルトン
GameData* GameData::m_instance = nullptr;


void GameData::SetCharData(const int& playNum, const Level& level)
{
	// 初期化
	m_characterStatus.clear();

	// キャラクターステータスを取得する
	const MasterData::FileReadCharcterStatus charcterStatus = MasterData::getInstance().GetCharcterStatus();

	// 最大体力
	{
		// 最大体力
		m_maxLifePoint = charcterStatus.lifePoint;

		// ライフポイントが0以下の場合、1に変更する
		if (m_maxLifePoint <= 0)
		{
			m_maxLifePoint = 1;
		}
		// ライフポイントが6以上の場合、6に変更する
		else if(m_maxLifePoint > 6)
		{
			m_maxLifePoint = 6;
		}
	}

	// 難易度取得
	m_level = level;

	// 一時保存キャラデータ
	CharacterStatus saveCharacterStatus;

	// データ入力
	for (int i = 0; i < kPlayerMaxCount; i++)
	{

		// プレイヤーかどうかを判定
		if (playNum > i)
		{
			saveCharacterStatus.isPlayer = true;
		}
		else
		{
			saveCharacterStatus.isPlayer = false;
		}

		// キャラクター番号を代入
		saveCharacterStatus.charNumber = i;

		// キャラクターカラーを代入
		saveCharacterStatus.color = charcterStatus.color[i];

		// 最大体力を代入
		saveCharacterStatus.lifePoint = m_maxLifePoint;

		// 移動速度を代入
		saveCharacterStatus.moveSpeed = charcterStatus.moveSpeed;

		// 攻撃硬直時間を代入
		saveCharacterStatus.attackRigorTime = charcterStatus.attackRigorTime;

		// 復帰時の無敵時間を代入
		saveCharacterStatus.invincibleTime = charcterStatus.invincibleTime;

		// ランキングを代入
		saveCharacterStatus.rank = 1;

		// m_characterStatusの要素を増やし、代入
		m_characterStatus.push_back(saveCharacterStatus);
	}
}

void GameData::SetCharRank(const int& charNumber, const int& rank)
{
	// ランキングを入れる
	m_characterStatus[charNumber].rank = rank;
}


