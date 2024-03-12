#pragma once

#include <array>
#include <vector>
#include <string>
#include <map>
#include <DxLib.h>
#include "EvoLib.h"

class MasterData
{

private:
	MasterData() = default;
	virtual ~MasterData() = default;

	static MasterData* m_instance;

public:
	// コピーコンストラクタの禁止
	MasterData(const MasterData&) = delete;
	MasterData& operator=(const MasterData&) = delete;
	MasterData(MasterData&&) = delete;
	MasterData& operator=(MasterData&&) = delete;

	static MasterData& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new MasterData;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	// ファイルから読み取るキャラクターステータス
	struct FileReadCharcterStatus
	{
		// ライフ
		int lifePoint = 0;

		// 移動速度
		float moveSpeed = 0.0f;

		// 攻撃硬直時間
		int attackRigorTime = 0;

		// 復帰時の無敵時間
		int invincibleTime = 0;

		// カラー
		std::vector<VECTOR>color;
	};

	// ファイルから読み取る攻撃エフェクト情報
	struct FileReadAttackEffectInfo
	{
		// ファイルパス
		std::string filePath = "";

		// グラフィックスケール
		float scale = 0.0f;

		// 使用するプレイヤー番号
		int usePlayerNumber = 0;
	};

	// ファイルから読み取る復活エフェクト情報
	struct FileReadResurrectionEffectInfo
	{
		// ファイルパス
		std::string filePath = "";

		// グラフィックスケール
		float scale = 0.0f;

	};

	// カラー情報
	struct ColorInfo
	{
		VECTOR color_0;// カラー1
		VECTOR color_1;// カラー2
	};


	// ファイルから読み取るフィールド情報
	struct FileReadFieldInfo
	{
		// ファイルパス
		std::string filePath = "";

		// モデルスケール
		float modelScale = 0.0f;

		// 落下インターバル
		int fallInterval = 0;

		// 落下間隔
		int differenceFallInterval = 0;

		// 盤面縮小落下カラーインターバル
		int deleteFallColorInterval = 0;

		// 落下スピード
		float fallSpeed = 0.0f;

		// 落下下限位置
		float fallLowerLimitPos = 0.0f;

		// 落下時の色
		VECTOR fallColor = VECTOR();

		// 盤面の色
		std::vector<ColorInfo>color;
	};

	// ファイルロードモデル情報
	struct FileReadModelInfo
	{
		// ファイルパス
		std::string filePath = "";

		// モデル向き
		float radian = 0.0f;

		// モデルスケール
		float scale = 1.0f;

		// モデルY座標
		float pos_y = 0.0f;

		// モーションが使えるかどうか
		bool isUseMotion = false;
	};

public:


	/// <summary>
	/// データのロード処理
	/// </summary>
	void LoadData();

	/// <summary>
	/// キャラクターステータスを返す
	/// </summary>
	/// <returns>キャラクターステータス</returns>
	FileReadCharcterStatus GetCharcterStatus() { return m_charcterStatus; }

	/// <summary>
	/// 攻撃エフェクト情報を返す
	/// </summary>
	/// <returns>攻撃エフェクト情報</returns>
	std::vector<FileReadAttackEffectInfo> GetAttackEffectInfo() { return m_attackEffectInfo; }

	/// <summary>
	/// 復活エフェクト情報を返す
	/// </summary>
	/// <returns>復活エフェクト情報</returns>
	FileReadResurrectionEffectInfo GetResurrectionEffectInfo() { return m_resurrectionEffectInfo; }


	/// <summary>
	/// フィールド情報を返す
	/// </summary>
	/// <returns>フィールド情報</returns>
	FileReadFieldInfo GetFieldInfo() { return m_fieldInfo; }

	/// <summary>
	/// モデル情報を返す
	/// </summary>
	/// <returns>モデル情報</returns>
	std::vector<FileReadModelInfo> GetModelInfo() { return m_modelInfo; }

private:

	/// <summary>
	/// キャラクターステータスのロード処理
	/// </summary>
	void LoadCharcterStatus();

	/// <summary>
	/// 攻撃エフェクト情報のロード処理
	/// </summary>
	void LoadAtackEffectInfo();

	/// <summary>
	/// 復活エフェクト情報のロード処理
	/// </summary>
	void LoadResurrectionEffectInfo();

	/// <summary>
	/// フィールド情報のロード処理
	/// </summary>
	void LoadFieldInfo();

	/// <summary>
	/// モデル情報のロード処理
	/// </summary>
	void LoadModelInfo();


private:

	////////////////////
	// 保存データ関連 //
	////////////////////

	// キャラクターステータス情報
	FileReadCharcterStatus m_charcterStatus;

	// 攻撃エフェクト情報
	std::vector<FileReadAttackEffectInfo> m_attackEffectInfo;

	// 復活エフェクト情報
	FileReadResurrectionEffectInfo m_resurrectionEffectInfo;

	// フィールド情報
	FileReadFieldInfo m_fieldInfo;

	// モデル情報
	std::vector<FileReadModelInfo> m_modelInfo;
};