#include "ModelData.h"
#include <vector>
#include "GameData.h"
#include "MasterData.h"

// シングルトン
ModelData* ModelData::m_instance = nullptr;

namespace
{
	// テクスチャファイルパス
	std::vector<const char*>kTextureFileName
	{
		"Data/Char/model_1/Texture.jpg",
		"Data/Char/model_1/Texture_1.jpg",
		"Data/Char/model_1/Texture_2.jpg",
		"Data/Char/model_1/Texture_3.jpg",
	};
}

void ModelData::Load()
{
	// モデルセットアップ
	ModelSetUp();

	// フェイステクスチャをロード
	for (auto& path : kTextureFileName)
	{
		m_faceHandle.push_back(LoadGraph(path));
	}
}

void ModelData::UnLoad()
{
	// タイプの合計分for分をループする
	for (int i = 0; i < static_cast<int>(m_saveModel.size()); i++)
	{
		// モデルハンドルの削除
		MV1DeleteModel(m_saveModel[i].handle);
	}
}

void ModelData::LoadGamePlayModel()
{
	// リソースの読み込み
	SetUseASyncLoadFlag(true);	// 非同期読み込みを行いたい

	// モデルの個数
	const int modelCount = GameData::getInstance().GetGamePlayCount();
	
	// 使用するモデルの番号
	const int useModelNumber = GameData::getInstance().GetModelNumber();
	
	// 要素を増やす
	m_playModel.resize(modelCount);

	// カラー
	VECTOR color;

	// モデル情報を代入する
	for (int i = 0; i < modelCount; i++)
	{
		// モデルを複製
		m_playModel[i].handle = MV1DuplicateModel(m_saveModel[useModelNumber].handle);

		// モデルのスケール変更
		MV1SetScale(m_playModel[i].handle, VGet(m_modelScale, m_modelScale, m_modelScale));

		// モデルの初期角度を代入
		m_playModel[i].initAngle = m_saveModel[useModelNumber].initAngle;

		// モデルの高さを代入
		m_playModel[i].pos.y = m_saveModel[useModelNumber].pos.y;
		
		// プレイヤーカラーを取得
		color = GameData::getInstance().GetPlayerColor(i);

		// 色設定
		MV1SetMaterialDifColor(m_playModel[i].handle, 0, GetColorF(color.x, color.y, color.z, 1.0f));

		// アウトライン
		MV1SetMaterialOutLineWidth(m_playModel[i].handle, 0, 0.1f);
	}

	// リソースの読み込み
	SetUseASyncLoadFlag(false);	// デフォルトに戻しておく
}

void ModelData::DeleteGamePlayModel()
{
	// モデルを削除する
	for (auto& model : m_playModel)
	{
		MV1DeleteModel(model.handle);
	}
}

void ModelData::DrawModel(const DrawModelData& drawModelData)
{
	// モデルを回転させる
	RotModel(drawModelData.playerNumber, drawModelData.angle);

	// 座標を変更する
	CoordinateMove(drawModelData.playerNumber, drawModelData.pos);

	// モデルの描画
	MV1DrawModel(m_playModel[drawModelData.playerNumber].handle);
}

void ModelData::ChangeFace(const Face& face,const int& modelNum)
{
	// テクスチャを変更する
	MV1SetTextureGraphHandle(m_playModel[modelNum].handle, 0, m_faceHandle[static_cast<int>(face)], false);
}

void ModelData::ModelSetUp()
{
	// モデル情報を取得
	std::vector<MasterData::FileReadModelInfo> modelInfo = MasterData::getInstance().GetModelInfo();

	// 要素数を増やす
	m_saveModel.resize(static_cast<int>(modelInfo.size()));

	// タイプの合計分for分をループする
	for (int i = 0; i < static_cast<int>(m_saveModel.size()); i++)
	{
		// ロードモデル
		m_saveModel[i].handle = MV1LoadModel(modelInfo[i].filePath.c_str());

		// モデルのスケール取得
		m_modelScale = modelInfo[i].scale;

		// ラジアンから角度を求める
		float angle = Convert_Lib::ConvertRadianToAngle(modelInfo[i].radian);

		// 角度から方向を求める
		const Direction direction = Convert_Lib::ConvertAngleToDirection(angle, 1.0f);

		// 方向から角度を求める
		angle = Convert_Lib::ConvertDirectionToAngle(direction);

		// 初期角度変数
		float initAngle = 0.0f;

		// 角度によって処理を変更する
		if (angle == 0.0f)
		{
			initAngle = Convert_Lib::ConvertDirectionToAngle(Direction::Bottom);
		}
		else if (angle == 90.0f)
		{
			initAngle = Convert_Lib::ConvertDirectionToAngle(Direction::Right);
		}
		else if (angle == 180.0f)
		{
			initAngle = Convert_Lib::ConvertDirectionToAngle(Direction::Top);
		}
		else if (angle == 270.0f)
		{
			initAngle = Convert_Lib::ConvertDirectionToAngle(Direction::Left);
		}

		// モデルの初期角度を代入
		m_saveModel[i].initAngle = initAngle;
		// モデルの高さ代入
		m_saveModel[i].pos.y = modelInfo[i].pos_y;
	}
}

void ModelData::RotModel(const int& typeNum, const int& angle)
{
	// モデルの初期角度を引く
	const int modelAngle = angle - static_cast<int>(m_playModel[typeNum].initAngle);

	// 角度をラジアンに変換
	const float rad = Convert_Lib::ConvertAngleToRadian(static_cast<float>(modelAngle));

	// Y軸を中心にモデルを回転させる
	MV1SetRotationXYZ(m_playModel[typeNum].handle, VGet(0.0f, rad, 0.0f));
}

void ModelData::CoordinateMove(const int& typeNum, const VECTOR& pos)
{
	// 座標と基準座標を足す
	const VECTOR addPos = VAdd(m_playModel[typeNum].pos, pos);

	// モデルに座標を入れる
	MV1SetPosition(m_playModel[typeNum].handle, addPos);
}