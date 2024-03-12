#include "Field.h"
#include "Pad.h"
#include "MainParam.h"
#include "GameData.h"
#include "Vec2.h"
#include "Sound.h"

namespace
{	
	// 盤面最小スケール
	constexpr int kBoardMinimumScale = 4;

	// スクエアスケール
	constexpr float kSquareScale = 0.96f;

}

namespace
{
	// サウンドファイル名
	const std::vector<std::string> kSoundFileName =
	{
		"fieldDrop",		// フィールド落下音
		"fieldColorChange"	// カラー変更音
	};

	// サウンド名
	enum class SoundName
	{
		Drap,		// フィールド落下音
		ColorChange,// カラー変更音
	};
}

Field::Field() :
	// フィールド情報関連↓
	m_filedInfo(),
	// ハンドル関連↓
	m_model(),
	// 座標関連↓
	m_criteriaPos(),
	// 盤面関連↓
	m_board(),
	m_boardScale(),
	m_boardScaleDownCount(),
	m_isDuringBoardScaleDown(),
	// クラスポインタ関連↓
	m_pMainParam(nullptr)
{
}

Field::~Field()
{
	// モデルの削除
	for (auto& handle : m_model)
	{
		MV1DeleteModel(handle.handle);
	}
}

void Field::Init(bool isUseParam)
{
	// 初期座標設定処理
	InitSetPos();

	// 盤面生成処理
	CreateBoard();

	// メインパラㇺを使用する場合、盤面スケールを送る
	if (isUseParam)
	{
		// 盤面スケールを送る
		m_pMainParam->SetBoardScale(m_boardScale);
	}

	// 盤面縮小回数の初期化
	m_boardScaleDownCount = 0;

	// 盤面縮小中かどうかの判定初期化
	m_isDuringBoardScaleDown = false;

	// サウンドロード
	Sound::getInstance().Load(kSoundFileName);
}

void Field::Update()
{
	// 盤面情報初期化
	BoardInfoInit();

	// 縮小するかどうか
	IsBoardScaleDown();

	// 盤面縮小が終了したかどうか
	BoardScaleDownEnd();

	// 落下処理
	Fall();

	// カラー処理
	ColorInterval();
}

void Field::Draw()
{
	// モデルの描画
	for (auto& board : m_board)
	{
		for (int i = 0; i < static_cast<int>(m_board.size()); i++)
		{
			// 表示しないならコンティニューする
			if (!board[i].isDisplay)continue;

			// 盤面を描画
			MV1DrawModel(m_model[board[i].number].handle);

			// スクエアを描画
			Draw_Lib::DrawCube3D_EvoLib(m_model[board[i].number].pos, VGet(m_filedInfo.modelScale * kSquareScale, m_filedInfo.modelScale * kSquareScale, m_filedInfo.modelScale * kSquareScale), 50,1);
		}
	}
}

void Field::LoadHandle(bool isASyncLoadFlag)
{
	// 非同期処理時、ここを通る
	if (isASyncLoadFlag)
	{
		// リソースの読み込み
		SetUseASyncLoadFlag(true);	// 非同期読み込みを行いたい
	}


	// フィールド情報を代入する
	m_filedInfo = MasterData::getInstance().GetFieldInfo();

	// 最大盤面スケール
	InitBoardScale();
	
	// 基礎モデルハンドルに3Dモデルを読み込む
	const int srcMHandle = MV1LoadModel(m_filedInfo.filePath.c_str());

	// レベルの代入
	GameData::Level level = GameData::getInstance().GetLevel();

	// モデル配列要素追加
	m_model.resize(m_boardScale * m_boardScale);

	// 行のカウント変数
	int lineCount = 0;

	// モデルロード
	for (int i = 0; i < m_boardScale * m_boardScale; i++)
	{
		// 基礎モデルハンドルにある3Dモデルを複製する
		m_model[i].handle = MV1DuplicateModel(srcMHandle);

		// モデルのサイズ変更
		MV1SetScale(m_model[i].handle, VGet(m_filedInfo.modelScale, m_filedInfo.modelScale, m_filedInfo.modelScale));

		// 列
		if (i % m_boardScale == 0)
		{
			lineCount++;
		}

		// 偶数か奇数かでモデルの色を変更する
		if ((i + lineCount) % 2 == 0)
		{
			// モデルの色を代入
			m_model[i].baseColor = m_filedInfo.color[static_cast<int>(level)].color_0;
		}
		else
		{
			// モデルの色を代入
			m_model[i].baseColor = m_filedInfo.color[static_cast<int>(level)].color_1;
		}

		// モデルに色をつける
		ChangeModelColor(i, m_model[i].baseColor);
	}

	// 基礎モデルを削除
	MV1DeleteModel(srcMHandle);

	// リソースの読み込み
	SetUseASyncLoadFlag(false);	// デフォルトに戻しておく
}

void Field::SetPlayerPresent(const VECTOR& currentPos)
{
	// 盤面セル情報取得
	const Cell boardCell = GetConvertPosToCell(currentPos);

	// プレイヤーがいることを入れる
	m_board[boardCell.x][boardCell.z].isPlayerPresent = true;
}

VECTOR Field::GetFieldCellCenterPos(const VECTOR& pos)
{
	// 座標をセルに変換
	const Cell cell = GetConvertPosToCell(pos);
	
	// モデル番号を代入
	const int modelNumber = m_board[cell.x][cell.z].number;

	// 座標代入
	const VECTOR convertPos = m_model[modelNumber].pos;

	// 変換された座標を返す
	return convertPos;
}


bool Field::GetIsBoardCellGround(const VECTOR& afterPos)
{
	// 盤面セル情報取得
	const Cell boardCell = GetConvertPosToCell(afterPos);

	// 範囲外のセルならばfalseを返す
	if (!IsCellRange(boardCell))
	{
		// falseを返し、処理を終了する
		return false;
	}

	// 落下中ではなく、消えていなければ、地面にいる
	const bool isGraund = !m_board[boardCell.x][boardCell.z].isFalling && m_board[boardCell.x][boardCell.z].isDisplay;

	// 地面にいるかを返す
	return isGraund;
}

bool Field::GetIsBoardCellFall(const VECTOR& currentPos)
{
	// 盤面セル情報取得
	const Cell boardCell = GetConvertPosToCell(currentPos);

	// 範囲外のセルならばtrueを返す
	if (!IsCellRange(boardCell))
	{
		return true;
	}

	// 落ちていて、消えていなければtrue
	const bool isFalling = m_board[boardCell.x][boardCell.z].isFalling && m_board[boardCell.x][boardCell.z].isDisplay;

	// マスが落下しているかどうかを返す
	return isFalling;
}

bool Field::GetIsBoardCellRange(const VECTOR& afterPos)
{
	// 盤面セル情報取得
	const Cell boardCell = GetConvertPosToCell(afterPos);

	// 範囲外のセルならばfalseを返す
	if (!IsCellRange(boardCell))
	{
		return false;
	}

	// trueフラグを返す
	return true;
}

bool Field::GetIsBoardCurrentCellRange(const VECTOR& pos)
{
	// 盤面セル情報取得
	const Cell boardCell = GetConvertPosToCell(pos);

	// 範囲外のセルならばfalseを返す
	if (!IsCurrentFieldCellRange(boardCell))
	{
		return false;
	}

	// trueフラグを返す
	return true;
}

void Field::AttackFallCollision(const VECTOR& pos, const Direction& direction, const int& playerNumber)
{
	// 盤面セル情報取得
	const Cell boardCell = GetConvertPosToCell(pos);

	// 向き情報を含んだセル
	Cell directionCell = boardCell;

	// セル情報を代入
	directionCell.x += kDirctonArray[static_cast<int>(direction)].x;
	directionCell.z += kDirctonArray[static_cast<int>(direction)].z;

	// セルが範囲外ならばここで処理を終了する
	if (!IsCellRange(directionCell))
	{
		return;
	}

	// 最初の番号
	int beginNumber = 0;

	// 最初の番号を決める
	if (direction == Direction::Top || direction == Direction::Bottom)
	{
		beginNumber = directionCell.z;
	}
	else
	{
		beginNumber = directionCell.x;
	}


	// 落下するセル
	std::vector<Cell>fallCell;

	// 判定を行うマスの数を数える
	if (direction == Direction::Top || direction == Direction::Right)
	{
		for (int i = beginNumber; i < static_cast<int>(m_board.size()); i++)
		{
			if (direction == Direction::Top)
			{
				directionCell.z = i;
			}
			else
			{
				directionCell.x = i;
			}

			// すでに落下している場合、コンティニューする
			if (m_board[directionCell.x][directionCell.z].isFalling ||
				m_board[directionCell.x][directionCell.z].isDelete)
			{
				continue;
			}

			// セルを代入
			fallCell.push_back(directionCell);
		}
	}
	else
	{
		for (int i = beginNumber; i >= 0; i--)
		{
			if (direction == Direction::Bottom)
			{
				directionCell.z = i;
			}
			else
			{
				directionCell.x = i;
			}

			// すでに落下している場合、コンティニューする
			if (m_board[directionCell.x][directionCell.z].isFalling ||
				m_board[directionCell.x][directionCell.z].isDelete)
			{
				continue;
			}

			// セルを代入
			fallCell.push_back(directionCell);
		}
	}

	// 落下するセルの個数
	const int fallCellValue = static_cast<int>(fallCell.size());

	// 基準落下インターバル
	const int baseFallInterval = fallCellValue * m_filedInfo.fallInterval;

	// 落下時間とカラー変更時間を計算
	for (int i = 0; i < fallCellValue; i++)
	{

		// カラー変更
		{
			// プレイヤーナンバーを代入する
			m_board[fallCell[i].x][fallCell[i].z].playerNumber = playerNumber;

			// カラーを変更する
			m_board[fallCell[i].x][fallCell[i].z].isChangeColor = true;

			// カラー変更インターバル代入
			m_board[fallCell[i].x][fallCell[i].z].changeColorInterval = m_filedInfo.fallInterval * i;

			// 落下判定中ならばコンティニューする
			if (m_board[fallCell[i].x][fallCell[i].z].isDrop)
			{
				continue;
			}
		}


		// 落下インターバル
		{
			// 落下判定
			m_board[fallCell[i].x][fallCell[i].z].isDrop = true;

			// 基礎落下インターバル代入
			m_board[fallCell[i].x][fallCell[i].z].fallInterval = baseFallInterval;

			// 落下インターバル代入
			m_board[fallCell[i].x][fallCell[i].z].fallInterval += m_filedInfo.differenceFallInterval * i;
		}
	}
}

VECTOR Field::GetSpawnPos()
{
	// 最大値を求める
	const int modelNum = static_cast<int>(m_model.size()) - 1;

	// ランダム値
	int number = 0;

	// スポーン出来るかどうか
	bool isSpawn = false;

	// スポーンカウント
	int processCount = 0;

	while (!isSpawn)
	{
		// 処理回数カウント
		processCount++;

		// 処理回数が盤面スケールの最大値を超えた場合、ランダムな値を取得し、while分を抜ける
		if (processCount > (m_boardScale * m_boardScale))
		{
			// ランダム値を取得
			number = GetRand(modelNum);
			
			break;
		}

		// ランダム値を取得
		number = GetRand(modelNum);

		// スポーン出来るかどうか取得
		isSpawn = IsSpawn(number);
	}

	
	// リスポーン位置
	VECTOR spawnPos = m_model[number].pos;

	// Y座標に0を代入
	spawnPos.y = 0.0f;
	
	// リスポーン座標を返す
	return spawnPos;
}

std::vector<std::vector<Field::AlgorithmCellInfo>> Field::GetCellInfo()
{
	// 要素数を持った二次元配列
	std::vector<std::vector<AlgorithmCellInfo>> cellInfo(m_boardScale, std::vector<AlgorithmCellInfo>(m_boardScale));

	// すべてのセルを確認し、判定を代入する
	for (int z = 0; z < static_cast<int>(cellInfo.size()); z++)
	{
		for (int x = 0; x < static_cast<int>(cellInfo[z].size()); x++)
		{
			// 障害物があるかどうか
			if (m_board[x][z].isDrop || !m_board[x][z].isDisplay)
			{
				// 障害物がある
				cellInfo[x][z].isObstacle = true;
			}

			// プレイヤーがいるかどうか
			if (m_board[x][z].isPlayerPresent)
			{
				// プレイヤーがいるかどうか
				cellInfo[x][z].isPlayerPresent = true;
			}
		}
	}

	// セル情報を返す
	return cellInfo;
}

void Field::InitSetPos()
{
	// 仮座標
	const VECTOR kTemporaryPos = VGet(
			-m_filedInfo.modelScale * (m_boardScale * 0.5f), 
			0.0f, 
			-m_filedInfo.modelScale * (m_boardScale * 0.5f));


	// 基準座標
	m_criteriaPos = VGet(
		kTemporaryPos.x + (m_filedInfo.modelScale * 0.5f),
		kTemporaryPos.y - (m_filedInfo.modelScale * 0.5f),
		kTemporaryPos.z + (m_filedInfo.modelScale * 0.5f));
}

void Field::InitBoardScale()
{
	// 最大盤面スケールを求める
	{
		// プレイ人数
		const int playNum = GameData::getInstance().GetGamePlayCount();

		// 盤面のスケールを求める
		m_boardScale = playNum * 2;
	}
}

void Field::CreateBoard()
{
	// ナンバー
	int number = 0;

	// 要素数を持った二次元配列
	std::vector<std::vector<BoardCell>> initBoard(m_boardScale, std::vector<BoardCell>(m_boardScale));

	// 要素数を持った二次元配列をコピー
	m_board = initBoard;

	// すべてのセルを確認し、座標を代入する
	for (int z = 0; z < static_cast<int>(m_board.size()); z++)
	{
		for (int x = 0; x < static_cast<int>(m_board[z].size()); x++)
		{
			// モデル座標計算
			{
				m_model[number].pos.x = (m_criteriaPos.x + (m_filedInfo.modelScale * x));
				m_model[number].pos.y = m_criteriaPos.y;
				m_model[number].pos.z = m_criteriaPos.z + (m_filedInfo.modelScale * z);
			}

			// 盤面情報を代入
			{
				// 盤面番号を代入
				m_board[x][z].number = number;

				// 落ちる判定の初期化
				m_board[x][z].isFalling = false;
			}

			// モデル座標を入れる
			MV1SetPosition(m_model[number].handle, m_model[number].pos);

			// ナンバーを増やす
			number++;
		}
	}
}

void Field::ChangeModelColor(const int& modelNumber, const VECTOR& color)
{
	// 色設定
	MV1SetMaterialDifColor(m_model[modelNumber].handle, 0, GetColorF(color.x, color.y, color.z, 1.0f));
}

void Field::BoardInfoInit()
{
	// すべてのセルを確認し、プレイヤーがいるかどうかの判定を初期化する
	for (auto& board : m_board)
	{
		for (int i = 0; i < static_cast<int>(m_board.size()); i++)
		{
			// プレイヤーがいるかどうかの判定初期化
			board[i].isPlayerPresent = false;
		}
	}
}

VECTOR Field::GetConvertCriteriaPos(const VECTOR& pos)
{
	// 基準座標
	const VECTOR criteriaPos = VGet(
		m_criteriaPos.x - (m_filedInfo.modelScale * 0.5f),
		0.0f,
		m_criteriaPos.z - (m_filedInfo.modelScale * 0.5f));

	// 変換座標を求める
	const VECTOR convertPos = VGet((pos.x - criteriaPos.x), pos.y, (pos.z - criteriaPos.z));

	// 変換した座標を返す
	return convertPos;
}


Field::Cell Field::GetConvertPosToCell(const VECTOR& convertPos)
{
	// フィールド座標
	const VECTOR fieldPos = GetConvertCriteriaPos(convertPos);

	// 盤面マス
	Cell boardCell = {};

	// 座標から盤面のマスを計算
	boardCell.x = static_cast<int>(fieldPos.x / m_filedInfo.modelScale);
	boardCell.z = static_cast<int>(fieldPos.z / m_filedInfo.modelScale);

	// マイナス値の場合値を返して処理を終了
	if (fieldPos.x <= 0.0f ||
		fieldPos.z <= 0.0f)
	{
		// 値を返す
		return Cell(-1, -1);
	}

	// 値を返す
	return boardCell;
}

bool Field::IsCellRange(const Cell& currentCell)
{
	// 範囲外か確認
	{
		if (currentCell.x < 0)return false;
		if (currentCell.x >= m_boardScale)return false;
		if (currentCell.z < 0)return false;
		if (currentCell.z >= m_boardScale)return false;
	}

	// 範囲外のtrueフラグを返す
	return true;
}

bool Field::IsCurrentFieldCellRange(const Cell& currentCell)
{
	// 現在の盤面スケール
	const int currentBoardScale = m_boardScale - (m_boardScaleDownCount * 2);

	// 範囲外か確認
	{
		if (currentCell.x < 0)return false;
		if (currentCell.x >= currentBoardScale)return false;
		if (currentCell.z < 0)return false;
		if (currentCell.z >= currentBoardScale)return false;
	}

	// 範囲外のtrueフラグを返す
	return true;
}

void Field::IsBoardScaleDown()
{
	// 落下回数を取得
	const int dropCount = m_pMainParam->GetBoardDropCount();

	// 落下中か、落下が0ならば処理を行わない
	if (m_isDuringBoardScaleDown || dropCount == 0)
	{
		return;
	}

	// 縮小フラグをtrueにする
	m_isDuringBoardScaleDown = true;

	// 盤面縮小処理
	BoardScaleDown();
}

void Field::BoardScaleDown()
{
	// 配列のはじめ
	const int arrayBegin = 0 + m_boardScaleDownCount;
	// 配列の終わり
	const int arrayEnd = m_boardScale - m_boardScaleDownCount;
	// 盤面スケール
	const int boardScale = arrayEnd - arrayBegin;

	// 盤面スケールが最小スケールよりも小さい場合、縮小を行わない
	if (boardScale <= kBoardMinimumScale)
	{
		m_pMainParam->BoardDropEnd();

		m_isDuringBoardScaleDown = false;

		return;
	}

	// 落下するセル
	std::vector<Cell>fallCell;

	// セル
	Cell cell;

	// セルの始まりと終わりをそれぞれ代入する
	cell.x = arrayBegin;
	cell.z = arrayEnd - 1;

	// 方向数
	const int directionNumber = 4;

	// 落下させるセルを調べる処理
	for (int i = 0; i < directionNumber; i++)
	{
		for (int j = arrayBegin; j < arrayEnd; j++)
		{
			if (cell.x < arrayBegin)
			{
				cell.x = arrayBegin;

				continue;
			}
			else if (cell.z < arrayBegin)
			{
				cell.z = arrayBegin;

				continue;
			}
			else if (cell.x > arrayEnd - 1)
			{
				cell.x = arrayEnd - 1;

				continue;

			}
			else if (cell.z > arrayEnd - 1)
			{
				cell.z = arrayEnd - 1;

				continue;
			}


			fallCell.push_back(cell);


			switch (i)
			{
			case 0:

				cell.x++;

				break;
			case 1:

				cell.z--;

				break;
			case 2:

				cell.x--;

				break;
			case 3:

				cell.z++;

				break;
			default:
				break;
			}
		}
	}


	// カラー変更最大インターバル
	const int changeColorMaxInterval = m_filedInfo.deleteFallColorInterval * static_cast<int>(fallCell.size());

	// カラー変更インターバルと落下インターバルを調べる処理
	for (int i = 0; i < static_cast<int>(fallCell.size()); i++)
	{
		// 盤面削除フラグをtrueにする
		m_board[fallCell[i].x][fallCell[i].z].isDelete = true;

		// 落下インターバルがtrueならば、コンティニューする
		if (m_board[fallCell[i].x][fallCell[i].z].isDrop)
		{
			continue;
		}

		// カラー変更
		{
			// カラーを変更する
			m_board[fallCell[i].x][fallCell[i].z].isChangeColor = true;

			// カラー変更インターバル代入
			m_board[fallCell[i].x][fallCell[i].z].changeColorInterval = m_filedInfo.deleteFallColorInterval * i;
		}

		// 落下インターバル
		{
			// 落下判定
			m_board[fallCell[i].x][fallCell[i].z].isDrop = true;

			// 落下インターバル代入
			m_board[fallCell[i].x][fallCell[i].z].fallInterval = m_filedInfo.differenceFallInterval + changeColorMaxInterval;
		}
	}

	// 盤面縮小カウントを増やす
	m_boardScaleDownCount++;
}

void Field::BoardScaleDownEnd()
{
	// 盤面の縮小処理が終わっていればここで処理を終了する
	if (!m_isDuringBoardScaleDown)
	{
		return;
	}


	// すべてのセルを調べ、削除・落下フラグがtrueならばここで処理を終了する
	for (auto& board : m_board)
	{
		for (int i = 0; i < static_cast<int>(m_board.size()); i++)
		{
			if (board[i].isDelete && board[i].isDrop)
			{
				return;
			}
		}
	}

	// メインパラㇺに盤面の落下処理が終了している事を送る
	m_pMainParam->BoardDropEnd();

	// 落下処理をしたことを送る
	m_pMainParam->SetIsDropBoard(true);

	// 盤面縮小フラグをfalseにする
	m_isDuringBoardScaleDown = false;
}

void Field::Fall()
{
	// 座標
	VECTOR pos = VGet(0.0f, 0.0f, 0.0f);

	// すべてのセルを確認し、落下インターバルを減らして落下させる
	for (auto& board : m_board)
	{
		for (int i = 0; i < static_cast<int>(m_board.size()); i++)
		{
			// 落下判定でなければコンティニューする
			if (!board[i].isDrop)continue;

			if (board[i].fallInterval == 0 &&
				!board[i].isFalling)
			{
				// 落下SEを再生する
				Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Drap)]);
			}

			// 落下インターバルが0以上ならインターバルを減らし、continueを行う
			if (board[i].fallInterval >= 0)
			{
				// 落下インターバルを減らす
				board[i].fallInterval--;

				continue;
			}
	
			// 落下フラグをtrueにする
			board[i].isFalling = true;


			// 盤面番号
			const int boardNumber = board[i].number;

			// モデルの座標を取得
			pos = m_model[boardNumber].pos;

			// 落下させる
			pos.y -= m_filedInfo.fallSpeed;

			// ある程度の位置まで落ちた場合、元の位置に戻す
			if (pos.y <= m_filedInfo.fallLowerLimitPos)
			{
				// 基準位置に戻す
				pos.y = m_criteriaPos.y;
				
				// 削除フラグがtrueならば、表示フラグをfalseにする
				if (board[i].isDelete)
				{
					// 表示しない
					board[i].isDisplay = false;
				}
				
				// 色をもとに戻す
				ChangeModelColor(boardNumber, m_model[boardNumber].baseColor);

				// 落下状態をfalseにする
				board[i].isFalling = false;
				board[i].isDrop = false;
			}

			// モデル座標代入
			m_model[boardNumber].pos = pos;

			// モデルの位置をセットする
			MV1SetPosition(m_model[boardNumber].handle, pos);
		}
	}
}

void Field::ColorInterval()
{
	// すべてのセルを確認し、カラーインターバルを減らしてカラーを変更する
	for (auto& board : m_board)
	{
		for (int i = 0; i < static_cast<int>(m_board.size()); i++)
		{
			// カラー変更フラグがfalseの場合、コンティニューする
			if (!board[i].isChangeColor)
			{
				continue;
			}

			// カラー変更インターバルを減らす
			board[i].changeColorInterval--;

			if (board[i].changeColorInterval <= 0)
			{
				board[i].changeColorInterval = 0;

				// 色変更
				ChangeModelColor(board[i].number, GameData::getInstance().GetPlayerColor(board[i].playerNumber));

				if (board[i].isDelete)
				{
					// 色変更
					ChangeModelColor(board[i].number, m_filedInfo.fallColor);

					// 落下SEを流す
					Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::ColorChange)]);
				}

				// カラー変更フラグをfalseにする
				board[i].isChangeColor = false;
			}
		}
	}
}

bool Field::IsSpawn(const int& number)
{
	// スポーン出来るかどうか
	bool isSpawn = false;

	// すべてのセルを確認し、リスポーンできる座標があるかを調べる
	for (int z = 0; z < static_cast<int>(m_board.size()); z++)
	{
		for (int x = 0; x < static_cast<int>(m_board[z].size()); x++)
		{
			if (m_board[x][z].number == number)
			{
				// プレイヤーがいなくて、落下している場合true
				const bool isBoardEmpty = !m_board[x][z].isPlayerPresent && 
					!m_board[x][z].isFalling && !m_board[x][z].isDelete;

				// スポーン出るか、フラグを調べ代入する
				isSpawn = isBoardEmpty && m_board[x][z].isDisplay;
			
				// スポーンフラグがtrueならばプレイヤーがいることにする
				if (isSpawn)
				{
					m_board[x][z].isPlayerPresent = true;
				}

				break;
			}
		}
	}

	// スポーンフラグを返す
	return isSpawn;
}

