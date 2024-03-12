#pragma once

#include <DxLib.h>
#include "EvoLib.h"
#include <array>

// シングルトンを使用したモデルデータクラス
class ModelData
{
private:
	ModelData() = default;
	virtual ~ModelData() = default;

	static ModelData* m_instance;

public:
	
	// コピーコンストラクタの禁止
	ModelData(const ModelData&) = delete;
	ModelData& operator=(const ModelData&) = delete;
	ModelData(ModelData&&) = delete;
	ModelData& operator=(ModelData&&) = delete;

	static ModelData& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new ModelData;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	// モデル
	struct Model
	{
		// ハンドル
		int handle = -1;

		// 座標
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);

		// 初期角度
		float initAngle = 0.0f;
	};

	// モデル描画データ
	struct DrawModelData
	{
		// プレイヤー番号
		int playerNumber = 0;

		// 角度
		int angle = 0;

		// 座標
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);
	};


	// フェイス
	enum class Face
	{
		Noramal,// 通常
		Smile,	// 笑顔
		Sulking,// 拗ねる
		Cry		// 泣く
	};

public:

	/// <summary>
	/// ロード処理
	/// </summary>
	void Load();

	/// <summary>
	/// アンロード処理
	/// </summary>
	void UnLoad();



	/// <summary>
	/// ゲームプレイ用のモデルロード処理
	/// </summary>
	void LoadGamePlayModel();

	/// <summary>
	/// ゲームプレイ用のモデルを削除処理
	/// </summary>
	void DeleteGamePlayModel();

	/// <summary>
	/// モデル描画処理
	/// </summary>
	/// <param name="drawModelData">モデル描画データ</param>
	void DrawModel(const DrawModelData& drawModelData);


	/// <summary>
	/// 表情変更処理
	/// </summary>
	/// <param name="face">フェイス情報</param>
	/// <param name="modelNum">モデル番号</param>
	void ChangeFace(const Face& face, const int& modelNum);

private:

	/// <summary>
	/// モデルセットアップ処理
	/// </summary>
	void ModelSetUp();

	/// <summary>
	/// モデルの回転処理
	/// </summary>
	/// <param name="typeNum">タイプ番号</param>
	/// <param name="angle">角度</param>
	void RotModel(const int& typeNum, const int& angle);

	/// <summary>
	/// 座標を移動させる
	/// </summary>
	/// <param name="typeNum">タイプ番号</param>
	/// <param name="pos">座標</param>
	void CoordinateMove(const int& typeNum, const VECTOR& pos);

private:

	// モデルスケール
	float m_modelScale = 0.0f;

	// 表情ハンドル
	std::vector<int>m_faceHandle;

	// モデル情報
	std::vector<Model>m_saveModel;

	// モデル一時保存
	std::vector<Model>m_playModel;
};