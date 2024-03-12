#include "Algorithm.h"
#include <algorithm>
#include "Vec2.h"
#include "EvoLib.h"


Algorithm::Algorithm():
	// 盤面関連↓
	m_algorithmBoard(),
	m_boardScale(),
	// クラスポインタ関連↓
	m_pField()
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::Init()
{
	// 盤面スケール初期処理
	InitSetBoardScale();
}

void Algorithm::Update()
{
	// 盤面情報の初期化
	BoardInfo();
}

Direction Algorithm::TargetDirection(VECTOR currentPos, VECTOR targetPos, bool isCell)
{

	// セルがtrueならば、座標をセルに変換してセル座標に変換する
	if (isCell)
	{
		// 現在のセル
		const Field::Cell currentCell = ConvertPosToCell(currentPos);
		// ターゲットのセル
		const Field::Cell targetCell = ConvertPosToCell(targetPos);


		// セル座標に変換
		{
			// 現在の座標をセル座標に変換
			currentPos = ConvertCellToPos(currentCell);

			// ターゲット座標をセル座標に変換
			targetPos = ConvertCellToPos(targetCell);
		}
	}

	// 方向情報
	Direction direction = {};

	// 上下
	if (currentPos.z != targetPos.z)
	{
		if (currentPos.z < targetPos.z)
		{
			direction = Direction::Top;
		}
		else
		{
			direction = Direction::Bottom;
		}
	}
	// 左右
	if (currentPos.x != targetPos.x)
	{
		if (currentPos.x < targetPos.x)
		{
			direction = Direction::Right;
		}
		else
		{
			direction = Direction::Left;
		}
	}

	// 方向情報を返す
	return direction;
}

Algorithm::Attack Algorithm::IsAssailable(const VECTOR& currentPos, const std::vector<VECTOR>& otherPlayerPos)
{
	// 攻撃要素
	Attack attack = {};

	// 自分のいる位置に障害物があれば攻撃しない
	if (IsObstacle(currentPos))
	{
		return attack;
	}

	// 現在座標をセルに変換
	const Field::Cell currentCell = ConvertPosToCell(currentPos);

	// 他プレイヤーのセル
	Field::Cell otherPlayerCell = {};

	// コストの構造体
	std::vector<Cost>cost;

	// 代入用のコスト構造体
	Cost saveCost = {};

	// 他プレイヤーと同じセルにいるかどうかを調べる
	for (auto& pos : otherPlayerPos)
	{
		// 他プレイヤーの座標をセルに変換
		otherPlayerCell = ConvertPosToCell(pos);

		// 現在のセルと他プレイヤーのセルが片方でも同じ場合、コストを調べる
		if (currentCell.x == otherPlayerCell.x ||
			currentCell.z == otherPlayerCell.z)
		{
			// 代入用のコスト構造体
			{
				// コスト計算
				saveCost.value = RangeCost(currentCell, otherPlayerCell);

				// 方向を求める
				saveCost.direction = TargetDirection(currentPos, pos, true);
			}

			// 代入
			cost.push_back(saveCost);
		}
	}


	// コストの要素が0の場合、ここで処理を終了する
	if (static_cast<int>(cost.size()) == 0)
	{
		// 攻撃要素を返す
		return attack;
	}


	// コストをソートする(昇順)
	SortFactorAscendingOrder(cost);


	// 攻撃要素に変換
	{
		// 最小コストの方向を攻撃方向に変換
		attack.direction = cost.front().direction;

		// 攻撃できる
		attack.isAttack = true;
	}


	// 攻撃要素を返す
	return attack;
}

bool Algorithm::IsAroundObstacle(const VECTOR& currentPos)
{

	// 現在座標をセルに変換
	const Field::Cell currentCell = ConvertPosToCell(currentPos);

	// 方向配列を代入
	Field::Cell dirctonArray[static_cast<int>(Direction::DirectionNum)];

	// フィールドのセル情報を取得
	const std::vector<std::vector<Field::AlgorithmCellInfo>>fieldCell = m_pField->GetCellInfo();


	// 上下左右の方向分処理を回し、周りに障害物がないかどうかを調べる
	for (int i = 0; i < static_cast<int>(Direction::DirectionNum); i++)
	{
		// 配列を代入
		dirctonArray[i] = currentCell;

		// 方向配列に変換
		dirctonArray[i].x += Field::kDirctonArray[i].x;
		dirctonArray[i].z += Field::kDirctonArray[i].z;


		// 範囲外ならばコンティニューする
		if (!IsRange(dirctonArray[i]))
		{
			continue;
		}

		// オブジェクトがあれば、falseを返す
		if (!fieldCell[dirctonArray[i].x][dirctonArray[i].z].isObstacle)
		{
			return false;
		}	
	}

	// true判定を返す
	return true;
}

bool Algorithm::IsObstacle(const VECTOR& pos)
{
	// 調べる座標をセルに変換
	const Field::Cell destinationCell = ConvertPosToCell(pos);

	// フィールドのセル情報を取得
	const std::vector<std::vector<Field::AlgorithmCellInfo>>fieldCell = m_pField->GetCellInfo();

	// 障害物がある場合、trueで返す
	if (fieldCell[destinationCell.x][destinationCell.z].isObstacle)
	{
		return true;
	}

	// false判定を返す
	return false;
}

bool Algorithm::IsPlayerPresent(const VECTOR& currentPos, const std::vector<VECTOR>& playerPos)
{
	// 現在座標をセルに変換
	const Field::Cell currentCell = ConvertPosToCell(currentPos);

	// 他プレイヤーのセル
	Field::Cell playerCell = {};

	// 他プレイヤーと同じセルにいるかどうかを調べる
	for (auto& pos : playerPos)
	{
		// 他プレイヤーの座標をセルに変換
		playerCell = ConvertPosToCell(pos);

		// 現在のセルと他プレイヤーのセルが同じ座標だった場合、trueを返す
		if (currentCell.x == playerCell.x &&
			currentCell.z == playerCell.z)
		{
			return true;
		}
	}

	// false判定を返す
	return false;
}

VECTOR Algorithm::FindCellSafeClose(const VECTOR& currentPos)
{
	// 現在座標をセルに変換
	const Field::Cell currentCell = ConvertPosToCell(currentPos);

	// 方向配列
	Field::Cell dirctonArray = {};

	// 検索した座標
	std::vector<VECTOR>searchPos;

	// フィールドのセル情報を取得
	const std::vector<std::vector<Field::AlgorithmCellInfo>>fieldCell = m_pField->GetCellInfo();

	// 再検索するかどうか
	bool isSearchAgain = true;

	// 検索回数
	int searchCount = 0;

	// 検索範囲外かどうか
	bool isSearchRange = false;


	// 再検索フラグがtrueの時検索を行う
	while (isSearchAgain)
	{
		// 検索回数を増やす
		searchCount++;

		// 検索するセルのスケール
		const int searchCellScale = searchCount + 2;

		// 検索するセルのスケールが、盤面のスケールよりも大きかった場合、breakする
		if (searchCellScale > m_boardScale)
		{
			isSearchRange = true;
			break;
		}

		// 8方向をすべて調べる
		for (int i = 0; i < Field::kDirectionNumber; i++)
		{
			// 現在のセルを代入
			dirctonArray = currentCell;

			// 方向配列に変換
			dirctonArray.x += (Field::kDirctonArray[i].x * searchCount);
			dirctonArray.z += (Field::kDirctonArray[i].z * searchCount);


			// 範囲外ならばコンティニューする
			if (!IsRange(dirctonArray))
			{
				continue;
			}

			// 障害物があればコンティニューする
			if (fieldCell[dirctonArray.x][dirctonArray.z].isObstacle)
			{
				continue;
			}

			// 座標を入れる
			searchPos.push_back(ConvertCellToPos(dirctonArray));

			break;
		}

		// 要素が無ければ、再検索フラグをtrueにして、あればfalseにする
		if (static_cast<int>(searchPos.size()) == 0)
		{
			isSearchAgain = true;
		}
		else
		{
			isSearchAgain = false;
		}
	}

	// 検索範囲外ならば現在の座標を返す
	if (isSearchRange)
	{
		return currentPos;
	}

	// 検索した座標を返す
	return searchPos.front();
}

VECTOR Algorithm::CurrentPosDestination(const VECTOR& currentPos)
{
	// 現在座標をセルに変換
	const Field::Cell currentCell = ConvertPosToCell(currentPos);

	// セルを座標に変換
	const VECTOR destinationPos = ConvertCellToPos(currentCell);

	// セルを返す
	return destinationPos;
}

void Algorithm::RandomMove(const VECTOR& currentPos)
{

	// 現在座標をセルに変換
	const Field::Cell startCell = ConvertPosToCell(currentPos);

	// 盤面初期化
	BoardInfo();

	// ランダム方向を取得
	const int randDirection = GetRand((static_cast<int>(Direction::DirectionNum)) - 1);

	// ランダムなセル
	Field::Cell randCell = {};

	// ランダムなセルを代入する
	randCell.x = startCell.x + Field::kDirctonArray[randDirection].x;
	randCell.z = startCell.z + Field::kDirctonArray[randDirection].z;

	// 範囲内ならば使う判定にする
	if (IsRange(randCell))
	{
		m_algorithmBoard[randCell.x][randCell.z].isUse = true;
	}
}


int Algorithm::RangeCost(const Field::Cell& currentCell, const Field::Cell& goalCell)
{
	// 絶対値
	Field::Cell absoluteValue = Field::Cell();

	// 絶対値を求める
	{
		absoluteValue.x = std::abs(currentCell.x - goalCell.x);
		absoluteValue.z = std::abs(currentCell.z - goalCell.z);
	}

	// コストを求める
	const int cost = absoluteValue.x + absoluteValue.z;

	// 値を返す
	return cost;
}

VECTOR Algorithm::MoveCoordinate(const VECTOR& currentPos)
{

	// 現在座標をセルに変換
	const Field::Cell startCell = ConvertPosToCell(currentPos);

	// 方向配列を代入
	Field::Cell dirctonArray[static_cast<int>(Direction::DirectionNum)];

	// 目的地の座標に現在の座標を代入
	VECTOR coordinatePos = currentPos;


	// 方向分処理を回し、目的地とする座標を求める
	for (int i = 0; i < static_cast<int>(Direction::DirectionNum); i++)
	{
		// 配列を代入
		dirctonArray[i] = startCell;

		// 方向配列に変換
		dirctonArray[i].x += Field::kDirctonArray[i].x;
		dirctonArray[i].z += Field::kDirctonArray[i].z;


		// 範囲外の場合、コンティニューする
		if (!IsRange(dirctonArray[i]))
		{
			continue;
		}

		// 使うセルの場合、ここで処理を終了する
		if (m_algorithmBoard[dirctonArray[i].x][dirctonArray[i].z].isUse)
		{
			// セルを座標に変換
			coordinatePos = ConvertCellToPos(dirctonArray[i]);

			break;
		}
	}


	// 盤面を使わない判定にする
	m_algorithmBoard[startCell.x][startCell.z].isUse = false;

	
	// 目的地とする座標を返す
	return coordinatePos;
}

void Algorithm::InitSetBoardScale()
{
	// 盤面スケールを代入
	m_boardScale = m_pField->GetBoardScale();

	// 要素数を持った二次元配列
	const std::vector<std::vector<AlgorithmBoard>> m_initBoard(m_boardScale, std::vector<AlgorithmBoard>(m_boardScale));

	// 配列情報をコピー
	m_algorithmBoard = m_initBoard;
}

void Algorithm::BoardInfo()
{
	// フィールドのセル情報を取得
	const std::vector<std::vector<Field::AlgorithmCellInfo>>fieldCell = m_pField->GetCellInfo();

	// 初期化変数
	const AlgorithmBoard initAlgorithmBoard;

	
	// すべての盤面を調べ、フラグを代入する
	for (int z = 0; z < static_cast<int>(m_algorithmBoard.size()); z++)
	{
		for (int x = 0; x < static_cast<int>(m_algorithmBoard[z].size()); x++)
		{
			// 変数初期化
			m_algorithmBoard[x][z] = initAlgorithmBoard;

			// 障害物があるかどうか
			m_algorithmBoard[x][z].isObstacle = fieldCell[x][z].isObstacle;

			// プレイヤーがいるかどうか
			m_algorithmBoard[x][z].isPlayerPresent = fieldCell[x][z].isPlayerPresent;
	
		}
	}
}

bool Algorithm::IsRange(const Field::Cell& currentCell)
{
	// 範囲外か確認
	{
		if (currentCell.x < 0)return false;
		if (currentCell.x >= m_boardScale)return false;
		if (currentCell.z < 0)return false;
		if (currentCell.z >= m_boardScale)return false;
	}

	// 判定を返す
	return true;
}

bool Algorithm::IsObstacle(const Field::Cell &currentCell)
{
	// 障害物がある場合、trueで返す
	if (m_algorithmBoard[currentCell.x][currentCell.z].isObstacle)
	{
		return true;
	}

	// 判定を返す
	return false;
}

VECTOR Algorithm::ShortestDistance(const VECTOR& currentPos, const std::vector<VECTOR>& searchPos)
{
	// ベクトルのスケール
	std::vector<float> vecScale;

	// ベクトルのスケールを求める
	for (auto& search : searchPos)
	{
		// 距離を求める
		VECTOR vel = VSub(search, currentPos);

		// 求めた距離をノマライズすることにより距離の長さを１とみる
		vel = VNorm(vel);

		// ベクトルのスケールを代入
		vecScale.push_back(VSize(vel));
	}

	// 番号
	int number = 0;

	// 最小スケールを代入
	float minScale = vecScale.front();

	// 最小スケールを求める
	for (int i = 0; i < static_cast<int>(vecScale.size()); i++)
	{
		if (minScale > vecScale[i])
		{
			number = i;
			minScale = vecScale[i];
		}
	}

	// 座標を返す
	return searchPos[number];
}

void Algorithm::SortFactorAscendingOrder(std::vector<Cost>& cost)
{
	// 値をソートする
	std::sort(cost.begin(), cost.end(), [](const Cost& a, const Cost& b) 
	{
		// ソートされた値を返す
		return a.value < b.value;
	});
}

Field::Cell Algorithm::ConvertPosToCell(const VECTOR& currentPos)
{
	// セルを取得
	const Field::Cell cellInfo = m_pField->GetConvertPosToCell(currentPos);

	// セルに代入
	Field::Cell cell = {};
	cell.x = cellInfo.x;
	cell.z = cellInfo.z;

	// 値を返す
	return cell;
}

VECTOR Algorithm::ConvertCellToPos(const Field::Cell& currentCell)
{
	// 変換座標
	VECTOR convertPos = VECTOR();

	// 値を代入する
	convertPos.x = m_pField->GetCriteriaPos().x + (m_pField->GetFieldCellModelScale() * currentCell.x);
	convertPos.z = m_pField->GetCriteriaPos().z + (m_pField->GetFieldCellModelScale() * currentCell.z);

	// 値を返す
	return convertPos;
}