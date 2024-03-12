#pragma once
#include <memory>
#include <vector>
#include "EvoLib.h"
#include <DxLib.h>
#include <tuple>
#include "Field.h"

class Field;
class Algorithm
{
public:

	Algorithm();
	virtual ~Algorithm();

	/// <summary>
	/// フィールドクラスポインタの取得
	/// </summary>
	/// <param name="field">フィールドクラスポインタ</param>
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }


	void Init();
	void Update();

public:

	// 盤面
	struct AlgorithmBoard
	{
		bool isOpen = false;			// 開いているか
		bool isClose = false;			// 閉じているか
		bool isUse = false;				// 使っているか
		bool isObstacle = false;		// 障害物はあるか
		bool isPlayerPresent = false;	// プレイヤーがいるか
		int cost = 0;					// コスト
	};

	// 目的地
	struct Coordinate
	{
		// ゴール座標
		VECTOR goalPos = VECTOR();
		// 向き
		Direction direction = {};
		// 進んだかどうか
		bool isMove = false;
	};

	// 攻撃
	struct Attack
	{
		// 攻撃できるかどうか
		bool isAttack = false;

		// 向き
		Direction direction = {};
	};

	// コスト
	struct Cost
	{
		// コストの値
		int value = 0;

		// セル
		Field::Cell cell = {};

		// 向き
		Direction direction = {};
	};

public:

	/// <summary>
	/// ターゲットの方向を求める
	/// </summary>
	/// <param name="currentPos">現在の座標</param>
	/// <param name="targetPos">ターゲットの座標</param>
	/// <param name="isCell">セルかどうか</param>
	/// <returns>ターゲットの方向</returns>
	Direction TargetDirection(VECTOR currentPos, VECTOR targetPos, bool isCell = false);

	/// <summary>
	/// 攻撃できるかどうか
	/// </summary>
	/// <param name="currentPos">現在の座標</param>
	/// <param name="otherPlayerPos">他プレイヤーの座標</param>
	/// <returns>攻撃できるかどうか、向き</returns>
	Attack IsAssailable(const VECTOR& currentPos, const std::vector<VECTOR>& otherPlayerPos);

	/// <summary>
	/// まわりが障害物かどうか
	/// </summary>
	/// <param name="currentPos"></param>
	/// <returns>まわりが障害物かどうかのフラグ</returns>
	bool IsAroundObstacle(const VECTOR& currentPos);

	/// <summary>
	/// 障害物があるかどうか
	/// </summary>
	/// <param name="pos">調べる座標</param>
	/// <returns>障害物があるかどうかのフラグ</returns>
	bool IsObstacle(const VECTOR& pos);

	/// <summary>
	/// プレイヤーがいるかどうか
	/// </summary>
	/// <param name="currentPos">現在の位置</param>
	/// <param name="playerPos">他プレイヤーの座標</param>
	/// <returns>他プレイヤーがいるかどうかを返す</returns>
	bool IsPlayerPresent(const VECTOR& currentPos, const std::vector<VECTOR>& playerPos);

	/// <summary>
	/// 近くの安全なセルを探す
	/// </summary>
	/// <param name="currentPos">現在の座標</param>
	/// <returns>近くの安全なセル座標</returns>
	VECTOR FindCellSafeClose(const VECTOR& currentPos);

	/// <summary>
	/// 現在の座標を目的地にする
	/// </summary>
	/// <param name="currentPos">現在座標</param>
	/// <returns>目的地座標</returns>
	VECTOR CurrentPosDestination(const VECTOR& currentPos);

	/// <summary>
	/// ランダムに進む
	/// </summary>
	/// <param name="currentPos">現在の座標</param>
	void RandomMove(const VECTOR& currentPos);

	/// <summary>
	/// 現在のセルからゴールセルまでのコストを求める
	/// </summary>
	/// <param name="currentCell">現在のセル</param>
	/// <param name="goalCell">ゴールセル</param>
	/// <returns>算出したコスト</returns>
	int RangeCost(const Field::Cell& currentCell, const Field::Cell& goalCell);

	/// <summary>
	/// 進む座標を返す
	/// </summary>
	/// <param name="currentPos">現在の座標</param>
	/// <returns>進む座標</returns>
	VECTOR MoveCoordinate(const VECTOR& currentPos);



private:

	/// <summary>
	/// 盤面スケール初期設定
	/// </summary>
	void InitSetBoardScale();

	/// <summary>
	/// アルゴリズムを行うための盤面情報
	/// </summary>
	void BoardInfo();

	/// <summary>
	/// セルが範囲内かどうか
	/// </summary>
	/// <param name="currentCell">現在のセル</param>
	/// <returns>範囲内かどうか</returns>
	bool IsRange(const Field::Cell& currentCell);

	/// <summary>
	/// 障害物があるかどうか
	/// </summary>
	/// <param name="currentCell">現在のセル</param>
	/// <returns>障害物があるかどうか</returns>
	bool IsObstacle(const Field::Cell& currentCell);

	/// <summary>
	/// 最短距離の座標を探す
	/// </summary>
	/// <param name="searchPos">検索座標</param>
	/// <returns>最短距離の座標</returns>
	VECTOR ShortestDistance(const VECTOR& currentPos, const std::vector<VECTOR>& searchPos);

	/// <summary>
	/// コストの構造型を昇順ソートして返す
	/// </summary>
	/// <param name="">コストの構造型</param>
	void SortFactorAscendingOrder(std::vector<Cost>& cost);

	/// <summary>
	/// 座標をセルに変換
	/// </summary>
	/// <param name="currentPos">現在座標</param>
	/// <returns>セル</returns>
	Field::Cell ConvertPosToCell(const VECTOR& currentPos);

	/// <summary>
	/// セルを座標に変換
	/// </summary>
	/// <param name="currentCell">現在のセル</param>
	/// <returns>変換座標</returns>
	VECTOR ConvertCellToPos(const Field::Cell& currentCell);


public:

	//////////////
	// 盤面関連 //
	//////////////

	// 盤面
	std::vector<std::vector<AlgorithmBoard>>m_algorithmBoard;

	// 盤面スケール
	int m_boardScale;


	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// フィールドクラスポインタ
	std::shared_ptr<Field>m_pField;
};