#pragma once
#include <vector>
#include <DxLib.h>
#include <array>
#include "EvoLib.h"
#include <memory>
#include "Algorithm"
#include "MasterData.h"

class MainParam;
class Field
{
public:

	// セル
	struct Cell
	{
		// セルX
		int x = 0;
		// セルY
		int z = 0;
	};

	// 方向数
	static constexpr int kDirectionNumber = 8;

	// 8方向のセル
	static constexpr Cell kDirctonArray[kDirectionNumber]
	{
		// 上
		Cell(0,  1),
		// 下
		Cell(0, -1),
		// 左
		Cell(-1,  0),
		// 右
		Cell(1,  0),
		// 左上
		Cell(-1, 1),
		// 右上
		Cell(1,  1),
		// 左下
		Cell(-1, -1),
		// 右下
		Cell(1, -1),
	};




	// 盤面マス
	struct BoardCell
	{
		// 盤面番号
		int number = 0;

		// プレイヤーナンバー
		int playerNumber = 0;

		// 色を変更するかどうか
		bool isChangeColor = false;

		// カラー変更インターバル
		int changeColorInterval = 0;

		// 落下するかどうか
		bool isDrop = false;

		// 落下中かどうか
		bool isFalling = false;

		// 落下インターバル
		int fallInterval = 0;

		// 盤面を消すかどうか
		bool isDelete = false;

		// 表示するかどうか
		bool isDisplay = true;

		// プレイヤーがいるかどうか
		bool isPlayerPresent = false;
	};

	// モデル情報
	struct Model
	{
		// モデルハンドル
		int handle = -1;

		// ベースカラー
		VECTOR baseColor = VGet(0.0f, 0.0f, 0.0f);

		// 座標
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);
	};



	// セル情報
	struct AlgorithmCellInfo
	{
		bool isObstacle = false;// 障害物はあるか
		bool isPlayerPresent = false;// プレイヤーがいるか
	};

public:
	Field();
	virtual ~Field();

	/// <summary>
	/// メインパラㇺポインタを取得
	/// </summary>
	/// <param name="mainParam">メインパラㇺポインタ</param>
	void SetMainParamPointer(std::shared_ptr<MainParam>mainParam) { m_pMainParam = mainParam; }

	/// <summary>
	/// メインパラㇺを使用するかどうか
	/// </summary>
	/// <param name="isUseParam">メインパラㇺ使用フラグ</param>
	void Init(bool isUseParam = true);
	void Update();
	void Draw();

public:

	/// <summary>
	/// ハンドルのロード処理
	/// </summary>
	/// <param name="isASyncLoadFlag">非同期処理を行うかどうか</param>
	void LoadHandle(bool isASyncLoadFlag = true);

	/// <summary>
	/// 自分のいる位置を入れる処理
	/// </summary>
	/// <param name="currentPos">現在の座標</param>
	void SetPlayerPresent(const VECTOR& currentPos);

	/// <summary>
	/// フィールドのセルを中心とした座標を返す処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <returns>フィールドのセルを中心とした座標</returns>
	VECTOR GetFieldCellCenterPos(const VECTOR& pos);


	/// <summary>
	/// 自分のいる位置マスが地面なのかどうかを返す処理
	/// </summary>
	/// <param name="afterPos">移動後座標</param>
	/// <returns>地面にいるかどうか</returns>
	bool GetIsBoardCellGround(const VECTOR& afterPos);

	/// <summary>
	/// 自分のいる位置のマスが落下しているかどうかを返す処理
	/// </summary>
	/// <param name="pos">現在座標</param>
	/// <returns>自分のいるマスの落下情報</returns>
	bool GetIsBoardCellFall(const VECTOR& currentPos);

	/// <summary>
	/// 自分のいる位置マスが範囲内かどうかを返す処理
	/// </summary>
	/// <param name="afterPos">移動後座標</param>
	/// <returns>自分のいる位置マスが範囲内かどうか</returns>
	bool GetIsBoardCellRange(const VECTOR& afterPos);

	/// <summary>
	/// 自分のいる位置マスが現在のセルの範囲内かどうかを返す処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <returns>範囲内かどうか</returns>
	bool GetIsBoardCurrentCellRange(const VECTOR& pos);


	/// <summary>
	/// 攻撃落下判定処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="direction">向き</param>
	/// <param name="playerNumber">プレイヤーナンバー</param>
	void AttackFallCollision(const VECTOR& pos, const Direction& direction, const int& playerNumber);


	/// <summary>
	/// スポーン位置を返す処理
	/// </summary>
	/// <returns>スポーン位置</returns>
	VECTOR GetSpawnPos();


	/// <summary>
	/// 盤面のスケールを返す処理
	/// </summary>
	/// <returns>盤面スケール</returns>
	int GetBoardScale() { return m_boardScale; }

	/// <summary>
	/// フィールドモデルのスケールを返す処理
	/// </summary>
	/// <returns>フィールドモデルのスケール</returns>
	float GetFieldCellModelScale() {return m_filedInfo.modelScale;}

	/// <summary>
	/// 落下スピードを返す処理
	/// </summary>
	/// <returns>落下スピード</returns>
	float GetFallSpeed() { return m_filedInfo.fallSpeed; }

	/// <summary>
	/// 落下下限位置を返す
	/// </summary>
	/// <returns>落下下限位置</returns>
	float GetFallLowerLimitPos() { return m_filedInfo.fallLowerLimitPos; }

	/// <summary>
	/// セル情報を返す処理
	/// </summary>
	/// <returns>セル情報</returns>
	std::vector<std::vector<AlgorithmCellInfo>> GetCellInfo();


	/// <summary>
	/// 基準座標を返す処理
	/// </summary>
	/// <returns>基準座標</returns>
	VECTOR GetCriteriaPos() { return m_criteriaPos; }

	/// <summary>
	/// 基準座標に変換する処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <returns>変換された座標</returns>
	VECTOR GetConvertCriteriaPos(const VECTOR& pos);

	/// <summary>
	/// 座標をセルに変換する処理
	/// </summary>
	/// <param name="currentPos">変換を行う座標</param>
	/// <returns>座標を変換したセル</returns>
	Cell GetConvertPosToCell(const VECTOR& convertPos);

	/// <summary>
	/// 削除カラーインターバルを返す処理
	/// </summary>
	/// <returns>削除カラーインターバル</returns>
	int GetDeleteFallColorInterval() { return m_filedInfo.deleteFallColorInterval; }

private:

	/// <summary>
	/// 初期座標設定処理
	/// </summary>
	void InitSetPos();

	/// <summary>
	/// 盤面スケール初期処理
	/// </summary>
	void InitBoardScale();

	/// <summary>
	/// 盤面生成処理
	/// </summary>
	void CreateBoard();

	/// <summary>
	/// モデルカラー変更処理
	/// </summary>
	/// <param name="modelNumber">モデル番号</param>
	/// <param name="color">カラー</param>
	void ChangeModelColor(const int& modelNumber, const VECTOR& color);

	/// <summary>
	/// 盤面情報初期化処理
	/// </summary>
	void BoardInfoInit();


	/// <summary>
	/// セルが範囲内かどうかを返す処理
	/// </summary>
	/// <param name="currentCell">現在のセル</param>
	/// <returns>範囲内かどうか</returns>
	bool IsCellRange(const Cell& currentCell);

	/// <summary>
	/// 現在のセルの範囲内かどうかを返す処理
	/// </summary>
	/// <param name="currentCell">現在のセル</param>
	/// <returns>範囲内かどうか</returns>
	bool IsCurrentFieldCellRange(const Cell& currentCell);


	/// <summary>
	/// 盤面を縮小するかどうかを調べる処理
	/// </summary>
	void IsBoardScaleDown();

	/// <summary>
	/// 盤面縮小処理
	/// </summary>
	void BoardScaleDown();

	/// <summary>
	/// 盤面縮小が終了したかどうかを調べる処理
	/// </summary>
	void BoardScaleDownEnd();

	/// <summary>
	/// 落下処理
	/// </summary>
	void Fall();

	/// <summary>
	/// カラーインターバル処理
	/// </summary>
	void ColorInterval();

	/// <summary>
	/// リスポーン出来るかどうかを調べる処理
	/// </summary>
	/// <param name="number">番号</param>
	/// <returns>リスポーンできるかどうか</returns>
	bool IsSpawn(const int& number);

private:

	////////////////////////
	// フィールド情報関連 //
	////////////////////////
	
	// フィールド情報
	MasterData::FileReadFieldInfo m_filedInfo;

	//////////////////
	// ハンドル関連 //
	//////////////////

	// モデルハンドル
	std::vector<Model> m_model;

	//////////////
	// 座標関連 //
	//////////////

	// 基準座標
	VECTOR m_criteriaPos;

	//////////////
	// 盤面関連 //
	//////////////

	// 盤面情報
	std::vector<std::vector<BoardCell>> m_board;

	// 盤面スケール
	int m_boardScale;

	// 盤面縮小回数
	int m_boardScaleDownCount;

	// 盤面が縮小中かどうか
	bool m_isDuringBoardScaleDown;

	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// メインパラㇺポインタ
	std::shared_ptr<MainParam>m_pMainParam;
};

