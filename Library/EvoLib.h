#pragma once
#include <DxLib.h>
#include <vector>
#include "Vec2.h"
#include <string>


// 向き
enum class Direction
{
	Top,	// 上
	Bottom,	// 下
	Left,	// 左
	Right,	// 右
	DirectionNum,// 方向数
};

// 球情報
struct Sphere
{
	// 座標
	VECTOR pos = { 0.0f,0.0f,0.0f };
	// 半径
	float radius = 0.0f;
};

// FPS
class FPS_Lib
{
public:

	// DXLibを60FPSで動かす
	static constexpr int Dxlib_60FPS = 16667;
	// FPS
	static constexpr int FPS = 60;


	// FPSの取得
	static int GetFPS_EvoLib();
};

// ロード
class Load_Lib
{
public:
	// 分割数
	struct DivNum
	{
		// 横の分割数
		const int x = 1;
		// 縦の分割数
		const int y = 1;
	};


	// グラフィック情報
	struct GraphicInfo
	{
		// ファイルパス
		const char* filePath = "";

		// 座標
		Vec2 pos = Vec2(0.0f, 0.0f);

		// グラフィックスケール
		float scale = 1.0f;

		// グラフィックの分割数
		DivNum div = DivNum();
	};


	// グラフィック分割情報
	struct DivGraphInfo
	{
		// ハンドル
		std::vector<int>handle;

		// グラフィックのスケール
		Vec2 scale = Vec2(0.0f, 0.0f);
	};

public:

	/// <summary>
	/// グラフィックの分割
	/// </summary>
	/// <param name="filePath">グラフィックのファイルパス</param>
	/// <param name="div_x">横向きに対する分割数</param>
	/// <param name="div_y">縦向きに対する分割数</param>
	/// <returns></returns>
	static DivGraphInfo LoadDivGraph_EvoLib(const char* filePath, const int& div_x, const int& div_y);

	/// <summary>
	/// グラフィックの分割　改定版
	/// </summary>
	/// <param name="filePath">グラフィックのファイルパス</param>
	/// <param name="div">縦横の分割数</param>
	/// <returns>分割されたグラフィックハンドルを返す</returns>
	static std::vector<int> LoadDivGraph_EvoLib_Revision(const char* filePath, const DivNum& div);
};



// 変換
class Convert_Lib
{
public:

	// 向きに対応した角度
	static constexpr float TopAngle = 270.0f;	// 上
	static constexpr float BottomAngle = 90.0f;	// 下
	static constexpr float LeftAngle = 180.0f;	// 左
	static constexpr float RightAngle = 0.0f;	// 右

public:

	/// <summary>
	/// カラーを255に変換
	/// </summary>
	/// <param name="color"></param>
	/// <returns></returns>
	static VECTOR ColorCodeTo255(const VECTOR& color);


	/// <summary>
	/// 最大値と変換値を使い、パーセントを求める
	/// </summary>
	/// <param name="maxValue">最大値</param>
	/// <param name="convertValue">変換値</param>
	/// <returns>パーセンテージ</returns>
	static int PercentageRate(const float& maxValue, const float& convertValue);


	/// <summary>
	/// 値をパーセント変換
	/// </summary>
	/// <param name="maxValue">最大値</param>
	/// <param name="percentage">パーセント</param>
	/// <returns>パーセンテージ</returns>
	static int ConvertingValueToPercent(const int& maxValue, const float& percentage);


	/// <summary>
	/// パーセントから値に変換
	/// </summary>
	/// <param name="maxValue">最大値</param>
	/// <param name="percentage">パーセント</param>
	/// <returns>変換された値</returns>
	static int ConvertingFromPercentToValue(const int& maxValue, const float& percentage);


	/// <summary>
	/// 角度をラジアンに変換
	/// </summary>
	/// <param name="angle">角度</param>
	/// <returns>変換したラジアンを返す</returns>
	static float ConvertAngleToRadian(const float& angle);


	/// <summary>
	/// ラジアンを角度に変換
	/// </summary>
	/// <param name="radian">ラジアン</param>
	/// <returns>変換した角度を返す</returns>
	static float ConvertRadianToAngle(const float& radian);


	/// <summary>
	/// 向き情報を角度に変換
	/// </summary>
	/// <param name="direction">向き</param>
	/// <returns>角度</returns>
	static float ConvertDirectionToAngle(const Direction& direction);


	/// <summary>
	/// 角度情報を方向情報に変換
	/// </summary>
	/// <param name="angle">角度</param>
	/// <param name="addSubValue">増減値</param>
	/// <returns>方向情報</returns>
	static Direction ConvertAngleToDirection(const float& angle, const float addSubValue = 0.0f);


	/// <summary>
	/// フレームを秒に変換
	/// </summary>
	/// <param name="frame">フレーム</param>
	/// <param name="fps">FPS</param>
	/// <returns>秒</returns>
	static int ConvertFrameToSeconds(const int& frame, const int& fps = 0);

	/// <summary>
	/// フレームを秒に変換(改訂版)
	/// </summary>
	/// <param name="frame">フレーム</param>
	/// <param name="isInteger">整数値を返すかどうか</param>
	/// <param name="fps">FPS</param>
	/// <returns>変換された秒数を返す</returns>
	static float ConvertFrameToSeconds_Revision(const int& frame, bool isInteger = true, int fps = 0);


	/// <summary>
	/// 球情報へ変換
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="radius">半径</param>
	/// <returns>球情報</returns>
	static Sphere ConvertSphereInfo(const VECTOR& pos, const float& radius);


	/// <summary>
	/// string型からfloat型に変換
	/// </summary>
	/// <param name="num">string型の数字</param>
	/// <returns>float型の数字</returns>
	static float ConvertingStringToFloat(const std::string& stringNum);

	/// <summary>
	/// float型からstring型に変換
	/// </summary>
	/// <param name="num">float型の数字</param>
	/// <returns>string型の数字</returns>
	static std::string ConvertingFloatToString(const float& num);

	/// <summary>
	/// フレームをミリ秒単位に変換
	/// </summary>
	/// <param name="frame">フレーム</param>
	/// <returns>ミリ秒単位を返す</returns>
	static int ConvertingFrameCountToMillisecondUnit(const int& frame);

};

class Collision_Lib
{
public:

	/// <summary>
	/// 球の衝突判定
	/// </summary>
	/// <param name="sphere1">球情報1</param>
	/// <param name="sphere2">球情報2</param>
	/// <returns>衝突したかどうかを返す</returns>
	static bool SphereCollision(const Sphere& sphere1, const Sphere& sphere2);
};


class Calculation_Lib
{
public:

	// 計算(足し算、引き算)
	enum class Calculation
	{
		Addition,	// 足し算
		Subtraction,// 引き算
	};


public:

	/// <summary>
	/// サインカーブの値を求める
	/// </summary>
	/// <param name="currentFrame">現在のフレーム</param>
	/// <param name="maxFrame">最大フレーム</param>
	/// <param name="maxValue">サインカーブの最大値</param>
	/// <param name="isAbs">absを使用するかどうか</param>
	/// <returns></returns>
	static int SineCurve(int currentFrame, int maxFrame, int maxValue, bool isAbs = false);


	/// <summary>
	/// 2Dの振動
	/// </summary>
	/// <param name="currentFrame">現在のフレーム</param>
	/// <param name="maxFrame">振動させる最大フレーム</param>
	/// <param name="vibrationWidth">振動幅</param>
	/// <returns>2Dの振動値</returns>
	static Vec2 VibrationValue(int currentFrame, const int& maxFrame, const int& vibrationWidth);


	/// <summary>
	/// ループ番号
	/// </summary>
	/// <param name="minNumber">最小値</param>
	/// <param name="maxNumber">最大値</param>
	/// <param name="currentNumber">現在の値</param>
	/// <param name="isInverseLoop">逆ループするかどうか</param>
	/// <returns>変換された番号</returns>
	static int SelectLoopNumber(const int& minNumber, const int& maxNumber, const int& currentNumber, bool isInverseLoop = false);


	/// <summary>
	/// ある位置を基準とし、幅を考慮したグラフィックの座標を返す
	/// </summary>
	/// <param name="handle">グラフィックハンドル</param>
	/// <param name="num">グラフィックの個数</param>
	/// <param name="graphScale">グラフィックスケール</param>
	/// <param name="centerPos">基準とする座標</param>
	/// <param name="distancePos">幅とする座標</param>
	/// <param name="isSide">横かどうか</param>
	/// <returns>幅を考慮したグラフィック座標を返す</returns>
	static std::vector<Vec2> GraphicWidthCoordinate(const int& handle, const int& num, const float& graphScale, const Vec2& centerPos, const Vec2& distancePos, bool isSide = true);


	/// <summary>
	/// 座標を均等に並べる
	/// </summary>
	/// <param name="num">均等に並べる数</param>
	/// <param name="centerPos">基準とする位置</param>
	/// <param name="distancePos">幅とする座標</param>
	/// <param name="isSide">横に並べるかどうか</param>
	/// <returns>均等に並べた座標</returns>
	static std::vector<Vec2> SortCoordinateEqually(const int& num, const Vec2& centerPos, const Vec2& distancePos, bool isSide = true);


	/// <summary>
	/// ターゲットの範囲内かどうかの判定
	/// </summary>
	/// <param name="currentValue">現在の値</param>
	/// <param name="targetValue">ターゲットの値</param>
	/// <param name="addSubValue">加減算値</param>
	/// <returns>ターゲットの範囲内かどうか</returns>
	static bool IsTargetRangeValue(const float& currentValue, const float& targetValue, const float& addSubValue);


	/// <summary>
	/// 角度を加減算して、360度を超えたり、0度以下にならないように計算を行う
	/// </summary>
	/// <param name="currentAngle">現在の角度</param>
	/// <param name="addSubValue">加減算値</param>
	/// <param name="calculation">足し算か引き算かどうか</param>
	/// <param name="isInteger">整数値かどうか</param>
	/// <param name="isAdjustment">調整値を加えるかどうか</param>
	/// <returns>加減算を終えた角度を返す</returns>
	static float AngleLimitValueNotExceedToCalculate(const float& currentAngle, const float& addSubValue, Calculation_Lib::Calculation calculation, bool isInteger = true, bool isAdjustment = true);

};


class Draw_Lib
{
public:

	// ループの方向
	enum class LoopDirection
	{
		Go_Top,		// 上方向にループする
		Go_Under,	// 下方向にループする
		Go_Left,	// 左方向にループする
		Go_Right,	// 右方向にループする
	};


	struct Color
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
	};


public:

	/// <summary>
	/// 簡易背景描画
	/// </summary>
	/// <param name="graphHandle">画像ハンドル</param>
	/// <param name="screenSize">画面サイズ</param>
	/// <param name="isFlipImage">画像を反転するかどうか</param>
	static void DrawSimpleBackground(const int& graphHandle, const Vec2& screenSize, bool isFlipImage = false);

	/// <summary>
	/// 簡易背景ループ
	/// </summary>
	/// <param name="graphHandle">グラフィックのハンドル</param>
	/// <param name="currentFrame">現在の移動量</param>
	/// <param name="moveSpeed">背景の移動スピード</param>
	/// <param name="screenSize">画面サイズ</param>
	/// <param name="isSupportedImageInLoop">使用する画像がループに対応しているかどうか</param>
	/// <param name="loopDirection">ループする方向</param>
	/// <returns>現在の移動量を返す</returns>
	static float SimpleLoopBackground(const int& graphHandle, const float currentMoveValue,float moveSpeed, const Vec2 screenSize, bool isSupportedImageInLoop = false, LoopDirection loopDirection = LoopDirection::Go_Left);

	/// <summary>
	/// 2Dグラフィックを3D空間に描画する　※graphHandle で指定する画像は８以上の２のｎ乗のピクセルサイズである必要があります
	/// </summary>
	/// <param name="graphHandle">グラフィックのハンドル</param>
	/// <param name="graphScale">グラフィックのスケール</param>
	/// <param name="angle">角度</param>
	/// <param name="pos">座標</param>
	/// <param name="color">カラーの値(最大値:1.0f)</param>
	static void DrawGprah3D_EvoLib(const int& graphHandle, const float& graphScale, const VECTOR& angle, const VECTOR& pos, Color color = { 1.0f,1.0f,1.0f });

	/// <summary>
	/// 3D空間にキューブを描画する
	/// </summary>
	/// <param name="centerPos">中心座標</param>
	/// <param name="cubeScale">一辺の長さ</param>
	/// <param name="alphaValue">透明度</param>
	/// <param name="lineThickness">線の太さ</param>
	/// <param name="color">カラーの値(最大値:1.0f)</param>
	static void DrawCube3D_EvoLib(const VECTOR& centerPos, const VECTOR& oneSideLength, int alphaValue = 255, int lineThickness = 1, Color color = { 0.0f,1.0f, 0.0f });
};


class File_Lib
{
public:

	// 読み取るタイプ
	enum class LoadType
	{
		Noramal,// 通常
		SkipFirstLine,	// 一行目をスキップする
		SkipOneColumn,	// 各一列目をスキップする
		DoubleSkip,		// 一行目と各一列目をスキップする
	};


private:

	/// <summary>
	/// 文字列を分割して返す
	static std::vector<std::string> Split(const std::string& input, const char& delimiter);
public:

	/// <summary>
	/// ファイルが存在するかどうかを確認
	/// </summary>
	/// <param name="name">ファイル名</param>
	/// <returns>そのファイルが存在するかどうかを返す</returns>
	static bool IsFileExist(const std::string& name);

	
	/// <summary>
	/// CSVファイルを読み込む
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="isLoadFirstLine">最初の一行を読み込むかどうか</param>
	/// <returns>読み込んだ文字列を返す</returns>
	static std::vector<std::vector<std::string>> CsvFileLoading(const std::string& filePath, bool isLoadFirstLine = true);

	/// <summary>
	/// CSVファイルを読み込む(改訂版)
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="isLoadFirstLine">最初の一行を読み込むかどうか</param>
	/// <param name="isLoadOneColumn">最初の一列を読み込むかどうか</param>
	/// <returns>読み込んだ文字列を返す</returns>
	static std::vector<std::vector<std::string>> CsvFileLoading_Revision(const std::string& filePath, LoadType loadType = LoadType::Noramal);

	/// <summary>
	/// 簡易CSVファイル書き込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="writingText">書き込むテキスト</param>
	static void SimpleCsvFileWriting(const std::string& fileName, const std::string& writingText);
};