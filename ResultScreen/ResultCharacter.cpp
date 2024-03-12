#include "ResultCharacter.h"
#include "GameData.h"
#include "Game.h"
#include "EvoLib.h"
#include "Vec2.h"
#include "ModelData.h"

namespace
{
	// 座標の幅
	const Vec2 kDistancePos = Vec2(15.0f, 0.0);
	
	// ランキング座標
	constexpr float kRankAdjustmentPos_Z = 8.0f;

	// 初期モデル角度
	constexpr int kInitModelAngle = 90;

	// モデル回転スピード
	constexpr int kRotationSpeed = 3;
}

ResultCharacter::ResultCharacter():
	// モデル関連↓
	m_pos(),
	m_modelData(),
	// モーション関連↓
	m_angle()
{
}

ResultCharacter::~ResultCharacter()
{
}

void ResultCharacter::Init()
{
	// 初期座標
	InitPos();

	// モデル初期角度を代入
	m_angle = kInitModelAngle;
}

void ResultCharacter::Update()
{
	// モーション更新処理
	UpdateMotion();
}

void ResultCharacter::Draw()
{
	// モデル描画処理
	DrawModel();
}

void ResultCharacter::InitPos()
{
	// プレイ人数を取得
	const int playCount = GameData::getInstance().GetGamePlayCount();

	// プレイ人数分、要素を増やす
	m_pos.resize(playCount);

	// 幅を考慮した座標を代入
	const std::vector<Vec2> distancePos = Calculation_Lib::SortCoordinateEqually(playCount, Vec2(0.0f, 0.0f), kDistancePos);

	// 座標の代入
	for (int i = 0; i < playCount; i++)
	{
		// 求めた座標を代入する
		m_pos[i] = VGet(distancePos[i].x, 0.0f, 0.0f);
	}

	// プレイ人数分、要素を増やす
	m_modelData.resize(playCount);

	// ランキング
	int rank = 0;

	// ランキングの座標
	float rankPos_z = 0.0f;

	for (int i = 0; i < static_cast<int>(m_pos.size()); i++)
	{
		// モデルの向きを代入
		m_modelData[i].angle = kInitModelAngle;

		// モデルの番号を代入
		m_modelData[i].playerNumber = i;

		// 座標を代入
		m_modelData[i].pos = m_pos[i];

		// ランキング取得
		rank = (GameData::getInstance().GetCharRank(i));
		
		// モデルのZ座標を求める
		m_modelData[i].pos.z += (kRankAdjustmentPos_Z * (rank -1));

		// 座標を代入する
		m_pos[i] = m_modelData[i].pos;

	

		// フェイス変数
		ModelData::Face face = ModelData::Face::Sulking;

		// ランクによってフェイスを変更する
		switch (rank)
		{
		case 1:
			// Smileを代入
			face = ModelData::Face::Smile;

			break;
		
		case 3:
			// Sulkingを代入
			face = ModelData::Face::Sulking;

			break;
		case 4:
			// Cryを代入
			face = ModelData::Face::Cry;
			break;

		default:
			break;
		}

		// ランクが2以外ならば表情を変更する
		if (rank != 2)
		{
			// 表情変更
			ModelData::getInstance().ChangeFace(face, i);
		}
	}
}

void ResultCharacter::UpdateMotion()
{
	// m_angleをkRotationSpeed分、加算する
	m_angle += kRotationSpeed;

	// 値が360を超えた場合、0を代入する
	if (m_angle >= 360)
	{
		m_angle = 0;
	}
}

void ResultCharacter::DrawModel()
{
	// モデルデータサイズを代入する
	const int num = static_cast<int>(m_modelData.size());

	// ランキングを入れる変数
	int rank = 0;

	// モデルを描画する
	for (int i = 0; i < num; i++)
	{
		// ランキング取得
		rank = GameData::getInstance().GetCharRank(i);

		// ランクの値が1の場合、モデルデータのアングルに加算されたアングルを代入する
		if (rank == 1)
		{
			// アングルを代入する
			m_modelData[i].angle = m_angle;
		}
	
		// モデル描画
		ModelData::getInstance().DrawModel(m_modelData[i]);
	}
}
