#include "EvoLib.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <array>


VECTOR Convert_Lib::ColorCodeTo255(const VECTOR& color)
{

    // カラー変数
    VECTOR rgb_255 = {};

    // パーセンテージ変数
    float percentage = 0.0f;
    
    // 赤
    {
        percentage = static_cast<float>(PercentageRate(1.0f, color.x));

        rgb_255.x = static_cast<float>(ConvertingValueToPercent(255, percentage));
    }
    // 緑
    {
        percentage = static_cast<float>(PercentageRate(1.0f, color.y));


        rgb_255.y = static_cast<float>(ConvertingValueToPercent(255, percentage));
    }
    // 青
    {
        percentage = static_cast<float>(PercentageRate(1.0f, color.z));


        rgb_255.z = static_cast<float>(ConvertingValueToPercent(255, percentage));
    }
    
    // カラー変数を返す
    return rgb_255;
}

int Convert_Lib::PercentageRate(const float& maxValue, const float& convertValue)
{
    
    // パーセントの小数値を求める
    const float decimal = convertValue / maxValue;

    // パーセントに変換
    const int percentage = static_cast<int>(decimal * 100);

    // パーセントを返す
    return percentage;
}

int Convert_Lib::ConvertingValueToPercent(const int& maxValue, const float& percentage)
{
    // パーセンテージに対する値計算
    const int convertValue = static_cast<int>(maxValue * percentage * 0.01);

    // 変換値を返す
    return convertValue;
}

int Convert_Lib::ConvertingFromPercentToValue(const int& maxValue, const float& percentage)
{

    // パーセントを小数に変換
    const float decimal = percentage * 0.01f;

    // 値を求める
    const int value = static_cast<int>(maxValue * decimal);

    // 値を返す
    return value;
}

float Convert_Lib::ConvertAngleToRadian(const float& angle)
{
    // 角度をラジアンに変換
    const float radian = angle * (DX_PI_F / 180.0f);

    // ラジアンを返す
    return radian;
}

float Convert_Lib::ConvertRadianToAngle(const float& radian)
{
    // ラジアンから角度に変換
    const float angle = radian * (180.0f / DX_PI_F);

    // 角度を返す
    return angle;
}

float Convert_Lib::ConvertDirectionToAngle(const Direction& direction)
{

    // 向き角度
    float directionAngle = 0.0f;

    // 向きによって代入する角度を変更する
    switch (direction)
    {
    case Direction::Top:

        directionAngle = TopAngle;
        break;
    case Direction::Bottom:

        directionAngle = BottomAngle;
        break;
    case Direction::Left:

        directionAngle = LeftAngle;
        break;
    case Direction::Right:

        directionAngle = RightAngle;
        break;
    default:
        break;
    }

    // 方向角度を返す
    return directionAngle;
}

Direction Convert_Lib::ConvertAngleToDirection(const float& angle, const float addSubValue)
{
    
    // 角度情報
    float directionAngle[5];

    directionAngle[0] = RightAngle;
    directionAngle[1] = BottomAngle;
    directionAngle[2] = LeftAngle;
    directionAngle[3] = TopAngle;
    directionAngle[4] = 360.0f;


    // 方向情報
    Direction direction;

    for (int i = 0; i < 5; i++)
    {
        if (Calculation_Lib::IsTargetRangeValue(angle, directionAngle[i], addSubValue))
        {
            switch (i)
            {
            case 0:
                direction = Direction::Right;
                break;
            case 1:
                direction = Direction::Bottom;
                break;
            case 2:
                direction = Direction::Left;
                break;
            case 3:
                direction = Direction::Top;
                break;
            case 4:
                direction = Direction::Right;
                break;
            default:
                break;
            }

            break;
        }
    }

    // 方向情報を返す
    return direction;
}

int Convert_Lib::ConvertFrameToSeconds(const int& frame, const int& fps)
{
  
    // 秒
    int second = 0;

    if (fps == 0)
    {
        second = frame / FPS_Lib::FPS;
    }
    else
    {
        second = frame / fps;
    }

    // 秒数を返す
    return second;
}

float Convert_Lib::ConvertFrameToSeconds_Revision(const int& frame, bool isInteger, int fps)
{
    // 秒
    float second = 0;

    // FPSを考慮して計算する
    if (fps == 0)
    {
        second = static_cast<float>(frame / FPS_Lib::FPS);
    }
    else
    {
        second = static_cast<float>(frame / fps);
    }

    // 整数値フラグがtrueの場合、整数値を返す
    if (isInteger)
    {
        // 整数値へ変換
        const int integerNum = static_cast<int> (second);

        return static_cast<float> (integerNum);
    }

    // 秒数を返す
    return second;
}


Sphere Convert_Lib::ConvertSphereInfo(const VECTOR& pos, const float& radius)
{
    // 球情報
    const Sphere sphere = { pos, radius };

    // 球情報を返す
    return sphere;
}

float Convert_Lib::ConvertingStringToFloat(const std::string& stringNum)
{
    // string型の数字をfloat型に変換する
    const float num = std::stof(stringNum);

    // 値を返す
    return num;
}

std::string Convert_Lib::ConvertingFloatToString(const float& num)
{
    // float型をstring型に変換
    const std::string stringNum = std::to_string(num);

    // 変換したstring型の数字を返す
    return stringNum;
}

int Convert_Lib::ConvertingFrameCountToMillisecondUnit(const int& frame)
{
    // フレーム数から秒数を求める
    const float seconds = ConvertFrameToSeconds_Revision(frame);

    // 秒数からミリ秒単位を求める
    int millisecondUnit = static_cast<int>(seconds * 1000.0f);

    // ミリ秒単位を返す
    return millisecondUnit;
}

int FPS_Lib::GetFPS_EvoLib()
{
    // 現在のFPSを取得する
    const float fps = GetFPS();

    // 取得したFPSを返す
    return static_cast<int>(fps);
}

bool Collision_Lib::SphereCollision(const Sphere& sphere1, const Sphere& sphere2)
{
    // 衝突したかどうか
    bool isCollision = false;


    // 球の衝突判定
    {
        const float dx = sphere1.pos.x - sphere2.pos.x;
        const float dy = sphere1.pos.y - sphere2.pos.y;
        const float dz = sphere1.pos.z - sphere2.pos.z;


        const float dr = (dx * dx) + (dy * dy) + (dz * dz);	// A²＝B²＋C²＋D²

        const float ar = sphere1.radius + sphere2.radius;		// 球の大きさ
        const float dl = ar * ar;

        if (dr <= dl)
        {
            // 衝突している
            isCollision = true;
        }
    }

    // 衝突判定を返す
    return isCollision;
}

int Calculation_Lib::SineCurve(int currentFrame, int maxFrame, int maxValue, bool isAbs)
{
    // ABSを使用する場合、最大フレームを２倍する
    if (isAbs)
    {
        maxFrame *= 2;
    }

    // 現在のフレームを最大フレームで割る
    currentFrame %= maxFrame;

    // サイン値を求める
    int sinValue = static_cast<int>(sin(DX_PI_F * 2.0f / maxFrame * currentFrame) * maxValue);

    // ABSを使用する場合、絶対値を求める
    if (isAbs)
    {
        // 絶対値を求める
        sinValue = std::abs(sinValue);
    }

    // サイン値を返す
    return sinValue;
}

Vec2 Calculation_Lib::VibrationValue(int currentFrame, const int& maxFrame, const int& vibrationWidth)
{

    // 現在のフレームに最大フレームから現在のフレームを引いた値を代入する
    currentFrame = maxFrame - currentFrame;

    // 現在のフレームが０以下だった場合、0を代入
    if (currentFrame <= 0)
    {
        currentFrame = 0;
    }


    // x,yをマイナスするかしないか
    int x = GetRand(1);
    int y = GetRand(1);

    // パーセンテージを求める
    const float percentage = static_cast<float>(Convert_Lib::PercentageRate(static_cast<float>(maxFrame), static_cast<float>(currentFrame)));

    // 小数化
    const float decimal = percentage * 0.01f;

    // 振動幅(計算)
    int afterVibrationWidth = static_cast<int>(vibrationWidth * decimal);


    Vec2 vibrationPos = {};


    // ランダム値で座標を変換する
    vibrationPos.x = static_cast<float>(GetRand(afterVibrationWidth));
    vibrationPos.y = static_cast<float>(GetRand(afterVibrationWidth));


    // x or yが0の時、位置をマイナスする
    if (x == 0)vibrationPos.x *= -1;
    if (y == 0)vibrationPos.y *= -1;

    // 振動値を返す
    return vibrationPos;
}

int Calculation_Lib::SelectLoopNumber(const int& minNumber, const int& maxNumber, const int& currentNumber, bool isInverseLoop)
{
    // 現在の番号を代入
    int number = currentNumber;

    // 逆ループするかどうかで処理を変更する
    if (isInverseLoop)
    {
        // 現在の選択項目を一つ上にずらす(逆ループする)
        number = (currentNumber + (maxNumber - 1)) % maxNumber; 
    }
    else
    {
        // 現在の選択項目を一つ下にずらす(ループする)
        number = (currentNumber + 1) % maxNumber; 
    }

    // 求めた番号を返す
    return number;
}

std::vector<Vec2> Calculation_Lib::GraphicWidthCoordinate(const int& handle, const int& num, const float& graphScale, const Vec2& centerPos, const Vec2& distancePos, bool isSide)
{
    // 座標
    std::vector<Vec2> pos;

    // グラフィックの個数分、座標変数を増やす
    pos.resize(num);

    // 一時的なグラフィックのサイズ格納変数
    int sizeX = 0;
    int sizeY = 0;

    // グラフィックサイズを取得
    GetGraphSize(handle, &sizeX, &sizeY);

    // グラフィックのサイズ格納変数
    Vec2 size;

    // サイズを代入
    size.x = static_cast<float>(sizeX);
    size.y = static_cast<float>(sizeY);


    // 調整グラフィックサイズをかける
    size *= graphScale;
    size += distancePos;

    // 基準座標
    Vec2 basePos = {};

    // 偶数判定
    const bool isEvenNumber = (num % 2 == 0);

    // 偶数か奇数かでベース座標を変更する
    if (isEvenNumber)
    {
        basePos = centerPos;

        basePos -= (size * 0.5);
    }
    else
    {
        basePos = centerPos;
    }


    int n = num;

    if (isEvenNumber)
    {
        n--;
    }

    n /= 2;



    // 一時保存座標
    Vec2 savePos;

    // ナンバー変数
    int number = 0;

    // カウント変数
    int count = 0;

    for (int i = 0; i < num; i++)
    {
        // 一時保存座標にベース座標を代入
        savePos = basePos;

        // ナンバー変数初期化
        number = 0;

        // iが奇数が偶数かで代入する座標を変更する
        if (i % 2 == 0)
        {
            if (isSide)
            {
                savePos.x -= size.x * count;
            }
            else
            {
                savePos.y -= size.y * count;
            }
            
            number -= count;
        }
        else
        {
            if (isSide)
            {
                savePos.x += size.x * count;
            }
            else
            {
                savePos.y += size.y * count;
            }

            number += count;
        }

        // 座標代入
        pos[number + n] = savePos;

        if (i % 2 == 0)
        {
            count++;
        }
    }

    // 値を返す
    return pos;
}

std::vector<Vec2> Calculation_Lib::SortCoordinateEqually(const int& num, const Vec2& centerPos, const Vec2& distancePos, bool isSide)
{
    // 座標
    std::vector<Vec2> pos;

    // 座標の要素を増やす
    pos.resize(num);

    // サイズ変数に幅とする座標を代入する
    const Vec2 size = distancePos;

    // 基準座標
    Vec2 basePos = {};

    // 偶数判定
    const bool isEvenNumber = (num % 2 == 0);

    // 偶数か奇数かでベース座標を変更する
    if (isEvenNumber)
    {
        basePos = centerPos;

        basePos -= (size * 0.5);
    }
    else
    {
        basePos = centerPos;
    }


    int n = num;

    if (isEvenNumber)
    {
        n--;
    }

    n /= 2;



    // 一時保存座標
    Vec2 savePos;

    // ナンバー変数
    int number = 0;
    // カウント変数
    int count = 0;

    for (int i = 0; i < num; i++)
    {
        // 一時保存座標にベース座標を代入
        savePos = basePos;

        // ナンバー変数初期化
        number = 0;

        // iが奇数か偶数かで一時保存座標の値を変更する
        if (i % 2 == 0)
        {
            if (isSide)
            {
                savePos.x -= size.x * count;
            }
            else
            {
                savePos.y -= size.y * count;
            }

            number -= count;
        }
        else
        {
            if (isSide)
            {
                savePos.x += size.x * count;
            }
            else
            {
                savePos.y += size.y * count;
            }

            number += count;
        }

        // 座標代入
        pos[number + n] = savePos;

        if (i % 2 == 0)
        {
            count++;
        }
    }

    // 値を返す
    return pos;
}

bool Calculation_Lib::IsTargetRangeValue(const float& currentValue, const float& targetValue, const float& addSubValue)
{
    // 値を補正
    if (currentValue <= (targetValue + addSubValue) &&
        currentValue >= (targetValue - addSubValue))
    {
        // true判定を返す
        return true;
    }

    // false判定を返す
    return false;
}

float Calculation_Lib::AngleLimitValueNotExceedToCalculate(const float& currentAngle, const float& addSubValue, Calculation_Lib::Calculation calculation, bool isInteger, bool isAdjustment)
{
    // 現在の角度を代入
    float angle = currentAngle;

    // 調整値
    float adjustmentValue = 0.0f;

    // 加算の場合、360度を超えないようにする
    if (calculation == Calculation::Addition)
    {
        // 角度に加減算値を加える
        angle += addSubValue;

        if (angle >= 360.0f)
        {
            // アングルを0度にする
            angle = 0.0f;

            // 調整値に現在のアングルから360.0f引いた数を求める
            adjustmentValue = angle - 360.0f;
        }
    }
    // 減算の場合、0度を下回らないようにする
    else
    {
        // 角度を加減算値分減らす
        angle -= addSubValue;

        if (angle <= 0.0f)
        {
            // 調整値に現在のアングルを代入する
            adjustmentValue = angle;

            // アングルを360度にする
            angle = 360.0f;
        }
    }

    // 調整値を使用する場合、調整値分加算を行う
    if (isAdjustment)
    {
        // アングルを調整値分、加算する
        angle += adjustmentValue;
    }

    // 整数値フラグがtrueならば整数値を返す
    if (isInteger)
    {
        // 整数値へ変換
        const int integerNum = static_cast<int>(angle);

        // 求めた角度を返す
        return static_cast<float>(integerNum);
    }


    // 求めた角度を返す
    return angle;
}

std::vector<std::string> File_Lib::Split(const std::string& input, const char& delimiter)
{
    std::istringstream stream(input);	// stringをgetlineで使えるデータに変換？
    std::string field;					// 分割した文字列1つ文を格納する？
    std::vector<std::string> result;	// 分割後の文字列の配列

    while (getline(stream, field, delimiter))
    {
        result.push_back(field);
    }

    // 分割した文字列を返す
    return result;
}

bool File_Lib::IsFileExist(const std::string& name)
{
    return std::filesystem::is_regular_file(name);
}

std::vector<std::vector<std::string>> File_Lib::CsvFileLoading(const std::string& filePath, bool isLoadFirstLine)
{
    // ファイル情報の読み込み
    std::ifstream ifs(filePath);
    std::string line;


    // 行カウント
    int lineCount = 0;

    // 読み込んだテキスト
    std::vector<std::vector<std::string>>loadText;
    


    while (getline(ifs, line))
    {
        // 行カウントを加算
        lineCount++;


        // 一行目の処理を飛ばす
        if (lineCount <= 1 && !isLoadFirstLine)continue;

        // csvデータ1行を','で複数の文字列に分割
        std::vector<std::string>strvec = File_Lib::Split(line, ',');

        if (static_cast<int>(strvec.size()) == 0)
        {
            break;
        }


        loadText.push_back(strvec);
    }



    return loadText;
}

std::vector<std::vector<std::string>> File_Lib::CsvFileLoading_Revision(const std::string& filePath, LoadType loadType)
{
    // ファイル情報の読み込み
    std::ifstream ifs(filePath);
    std::string line;


    // 行カウント
    int lineCount = 0;

    // 読み込んだテキスト
    std::vector<std::vector<std::string>>loadText;

    while (getline(ifs, line))
    {
        // 行カウントを加算
        lineCount++;


        // 一行目の処理を飛ばす
        if (lineCount <= 1 && loadType == File_Lib::LoadType::SkipFirstLine ||
            lineCount <= 1 && loadType == File_Lib::LoadType::DoubleSkip)
        {
            continue;
        }

            

        // csvデータ1行を','で複数の文字列に分割
        std::vector<std::string>	strvec = File_Lib::Split(line, ',');

        if (static_cast<int>(strvec.size()) == 0)
        {
            break;
        }

        // 一列目をスキップする
        if (loadType == File_Lib::LoadType::SkipOneColumn||
            loadType == File_Lib::LoadType::DoubleSkip)
        {
            strvec.erase(strvec.begin());
        }

        loadText.push_back(strvec);
    }



    return loadText;
}

void File_Lib::SimpleCsvFileWriting(const std::string& fileName, const std::string& writingText)
{
    std::ofstream writing_file;
    writing_file.open(fileName, std::ios::out);
   
    writing_file << writingText << std::endl;

    writing_file.close();

}

Load_Lib::DivGraphInfo Load_Lib::LoadDivGraph_EvoLib(const char* filePath, const int& div_x, const int& div_y)
{
  

    int wide = 0;       // グラフィックの横幅
    int height = 0;     // グラフィックの縦幅
    int graphic = -1;   // グラフィックの代入


    // プレイヤーのグラフィックサイズを取得
    {
        graphic = LoadGraph(filePath);
        GetGraphSize(graphic, &wide, &height);
    }


    // 分割数合計
    const int divNum = div_x * div_y;

    // グラフィック情報
    DivGraphInfo graphInfo;

    // スケールを代入
    graphInfo.scale.x = static_cast<float>(wide);
    graphInfo.scale.y = static_cast<float>(height);



    // 分割した画像をハンドルに入れる
    {
        // ハンドル
        int* handle = new int[divNum];

        // 分割した画像を入れる
        LoadDivGraph(filePath, divNum,
            div_x, div_y,
            wide / div_x, height / div_y, handle);

        for (int i = 0; i < divNum; i++)
        {
            // グラフィックを代入
            graphInfo.handle.push_back(handle[i]);
        }

        // メモリの開放
        delete[] handle;
    }

    return graphInfo;
}

std::vector<int> Load_Lib::LoadDivGraph_EvoLib_Revision(const char* filePath, const DivNum& div)
{

    int wide = 0;       // グラフィックの横幅
    int height = 0;     // グラフィックの縦幅
    int graphic = -1;   // グラフィックの代入


    // プレイヤーのグラフィックサイズを取得
    {
        graphic = LoadGraph(filePath);
        GetGraphSize(graphic, &wide, &height);
    }


    // 分割数合計
    const int divNum = div.x * div.y;

    // 分割されたグラフィックハンドル
    std::vector<int> graphHandle;

    // 分割した画像をハンドルに入れる
    {
        // ハンドル
        int* handle = new int[divNum];

        // 分割した画像を入れる
        LoadDivGraph(filePath, divNum,
            div.x, div.y,
            wide / div.x, height / div.y, handle);

        for (int i = 0; i < divNum; i++)
        {
            // グラフィックを代入
            graphHandle.push_back(handle[i]);
        }

        // メモリの開放
        delete[] handle;
    }

    // グラフィック情報
    return graphHandle;
}

void Draw_Lib::DrawSimpleBackground(const int& graphHandle, const Vec2& screenSize, bool isFlipImage)
{

    // 各座標
    Vec2 pos_1;
    Vec2 pos_2;
    Vec2 pos_3;
    Vec2 pos_4;

    // 座標に値を代入
    pos_1 = Vec2(0, 0);
    pos_2 = Vec2(screenSize.x, 0);
    pos_3 = Vec2(pos_2.x, screenSize.y);
    pos_4 = Vec2(0, screenSize.y);

    // 反転する場合の処理
    if (isFlipImage)
    {
        pos_1.x = pos_2.x;
        pos_2.x = pos_1.x;
        pos_3.x = pos_4.x;
        pos_4.x = pos_3.x;
    }

    // 描画
    DrawModiGraphF(
        pos_1.x, pos_1.y,
        pos_2.x, pos_2.y,
        pos_3.x, pos_3.y,
        pos_4.x, pos_4.y,
        graphHandle, true);

    // 処理を終了する
    return;
}

float Draw_Lib::SimpleLoopBackground(const int& graphHandle, float currentMoveValue, float moveSpeed, const Vec2 screenSize, bool isSupportedImageInLoop, LoopDirection loopDirection)
{
    // 座標格納変数
    Vec2 pos;

    if (loopDirection == Draw_Lib::LoopDirection::Go_Left ||
        loopDirection == Draw_Lib::LoopDirection::Go_Top)
    {
        // 移動スピード分、移動量に減算する
        currentMoveValue -= moveSpeed;


        // 最小移動量
        float moveMinValue = 0.0f;

        if (loopDirection == Draw_Lib::LoopDirection::Go_Left)
        {
            moveMinValue = -screenSize.x;
        }
        else
        {
            moveMinValue = -screenSize.y;
        }


        if (!isSupportedImageInLoop)
        {
            moveMinValue *= 2;
        }


        // 現在の移動量が最大移動量よりも小さかった場合、初期化を行う
        if (currentMoveValue <= moveMinValue)
        {
            currentMoveValue = 0.0f;
        }

    }
    else
    {
        // 移動スピード分、移動量に加算する
        currentMoveValue += moveSpeed;

        // 最大移動量
        float moveMaxValue = 0.0f;

        if (loopDirection == Draw_Lib::LoopDirection::Go_Right)
        {
            moveMaxValue = screenSize.x;
        }
        else
        {
            moveMaxValue = screenSize.y;
        }

        if (!isSupportedImageInLoop)
        {
            moveMaxValue *= 2;
        }

        // 現在の移動量が最大移動量よりも小さかった場合、初期化を行う
        if (currentMoveValue >= moveMaxValue)
        {
            currentMoveValue = 0.0f;
        }
    }


    Vec2 pos_1;
    Vec2 pos_2;
    Vec2 pos_3;
    Vec2 pos_4;

    if (loopDirection == Draw_Lib::LoopDirection::Go_Left ||
        loopDirection == Draw_Lib::LoopDirection::Go_Right)
    {

        pos_1 = Vec2(currentMoveValue, 0);
        pos_2 = Vec2((screenSize.x + currentMoveValue), 0);
        pos_3 = Vec2(pos_2.x, screenSize.y);
        pos_4 = Vec2(currentMoveValue, screenSize.y);
    }
    else
    {
        pos_1 = Vec2(0, currentMoveValue);
        pos_2 = Vec2(screenSize.x, currentMoveValue);
        pos_3 = Vec2(screenSize.x, (screenSize.y + currentMoveValue));
        pos_4 = Vec2(0, pos_3.y);
    }

    
    DrawModiGraphF(pos_1.x, pos_1.y, 
        pos_2.x, pos_2.y,
        pos_3.x, pos_3.y, 
        pos_4.x, pos_4.y,
        graphHandle, true);

    switch (loopDirection)
    {
    case Draw_Lib::LoopDirection::Go_Top:

        pos_1.y += screenSize.y;
        pos_2.y += screenSize.y;
        pos_3.y += screenSize.y;
        pos_4.y += screenSize.y;

        break;
    case Draw_Lib::LoopDirection::Go_Under:

        pos_1.y -= screenSize.y;
        pos_2.y -= screenSize.y;
        pos_3.y -= screenSize.y;
        pos_4.y -= screenSize.y;

        break;
    case Draw_Lib::LoopDirection::Go_Left:

        pos_1.x += screenSize.x;
        pos_2.x += screenSize.x;
        pos_3.x += screenSize.x;
        pos_4.x += screenSize.x;

        break;
    case Draw_Lib::LoopDirection::Go_Right:

        pos_1.x -= screenSize.x;
        pos_2.x -= screenSize.x;
        pos_3.x -= screenSize.x;
        pos_4.x -= screenSize.x;

        break;
    default:
        break;
    }
 

    // 反転用の座標
    const Vec2 reversePos_1 = pos_1;
    const Vec2 reversePos_2 = pos_2;
    const Vec2 reversePos_3 = pos_3;
    const Vec2 reversePos_4 = pos_4;

    if (!isSupportedImageInLoop)
    {

        if (loopDirection == Draw_Lib::LoopDirection::Go_Left ||
            loopDirection == Draw_Lib::LoopDirection::Go_Right)
        {
            pos_1.x = reversePos_2.x;
            pos_2.x = reversePos_1.x;
            pos_3.x = reversePos_4.x;
            pos_4.x = reversePos_3.x;
        }
        else
        {
            pos_1.y = reversePos_4.y;
            pos_2.y = reversePos_3.y;
            pos_3.y = reversePos_2.y;
            pos_4.y = reversePos_1.y;
        }
    }

    DrawModiGraphF(pos_1.x, pos_1.y,
        pos_2.x, pos_2.y,
        pos_3.x, pos_3.y,
        pos_4.x, pos_4.y,
        graphHandle, true);


    if (!isSupportedImageInLoop)
    {

        switch (loopDirection)
        {
        case Draw_Lib::LoopDirection::Go_Top:

            pos_1.y = (reversePos_1.y + screenSize.y);
            pos_2.y = (reversePos_2.y + screenSize.y);
            pos_3.y = (reversePos_3.y + screenSize.y);
            pos_4.y = (reversePos_4.y + screenSize.y);

            break;
        case Draw_Lib::LoopDirection::Go_Under:

            pos_1.y = (reversePos_1.y - screenSize.y);
            pos_2.y = (reversePos_2.y - screenSize.y);
            pos_3.y = (reversePos_3.y - screenSize.y);
            pos_4.y = (reversePos_4.y - screenSize.y);

            break;
        case Draw_Lib::LoopDirection::Go_Left:

            pos_1.x = (reversePos_1.x + screenSize.x);
            pos_2.x = (reversePos_2.x + screenSize.x);
            pos_3.x = (reversePos_3.x + screenSize.x);
            pos_4.x = (reversePos_4.x + screenSize.x);

            break;
        case Draw_Lib::LoopDirection::Go_Right:

            pos_1.x = (reversePos_1.x - screenSize.x);
            pos_2.x = (reversePos_2.x - screenSize.x);
            pos_3.x = (reversePos_3.x - screenSize.x);
            pos_4.x = (reversePos_4.x - screenSize.x);

            break;
        default:
            break;
        }
      

        DrawModiGraphF(pos_1.x, pos_1.y,
            pos_2.x, pos_2.y,
            pos_3.x, pos_3.y,
            pos_4.x, pos_4.y,
            graphHandle, true);
    }

    // グラフィックの移動量を返す
    return currentMoveValue;
}

void Draw_Lib::DrawGprah3D_EvoLib(const int& graphHandle, const float& graphScale, const VECTOR& angle, const VECTOR& pos, const Color color)
{
    // 立体表示
    VERTEX3D vertex[6];

    // ２ポリゴン分の頂点の座標と法線以外のデータをセット
    {
        vertex[0].dif = GetColorU8(0, 0, 0, 255);
        vertex[0].spc = GetColorU8(0, 0, 0, 0);
        vertex[0].u = 0.0f;
        vertex[0].v = 0.0f;
        vertex[0].su = 0.0f;
        vertex[0].sv = 0.0f;

        vertex[1].dif = GetColorU8(0, 0, 0, 255);
        vertex[1].spc = GetColorU8(0, 0, 0, 0);
        vertex[1].u = 1.0f;
        vertex[1].v = 0.0f;
        vertex[1].su = 0.0f;
        vertex[1].sv = 0.0f;

        vertex[2].dif = GetColorU8(0, 0, 0, 255);
        vertex[2].spc = GetColorU8(0, 0, 0, 0);
        vertex[2].u = 0.0f;
        vertex[2].v = 1.0f;
        vertex[2].su = 0.0f;
        vertex[2].sv = 0.0f;

        vertex[3].dif = GetColorU8(0, 0, 0, 255);
        vertex[3].spc = GetColorU8(0, 0, 0, 0);
        vertex[3].u = 1.0f;
        vertex[3].v = 1.0f;
        vertex[3].su = 0.0f;
        vertex[3].sv = 0.0f;

        vertex[4] = vertex[2];
        vertex[5] = vertex[1];
    }

    // 座標、角度をセット
    {
        MATRIX TransformMatrix;

        // ラジアン
        VECTOR radian;

        // 角度をラジアンに変換
        radian.x = Convert_Lib::ConvertAngleToRadian(angle.x);
        radian.y = Convert_Lib::ConvertAngleToRadian(angle.y);
        radian.z = Convert_Lib::ConvertAngleToRadian(angle.z);

        // 回転( x, y, z軸回転の順 )＋座標移動行列の作成
        TransformMatrix = MGetRotX(radian.x);
        TransformMatrix = MMult(TransformMatrix, MGetRotY(radian.y));
        TransformMatrix = MMult(TransformMatrix, MGetRotZ(radian.z));
        TransformMatrix = MMult(TransformMatrix, MGetTranslate(VGet(0.0f, 0.0f, 0.0f)));

        // 行列を使ってワールド座標を算出
        vertex[0].pos = VTransform(VGet(pos.x + -graphScale, pos.y + graphScale, pos.z), TransformMatrix);
        vertex[1].pos = VTransform(VGet(pos.x + graphScale, pos.y + graphScale, pos.z), TransformMatrix);
        vertex[2].pos = VTransform(VGet(pos.x + -graphScale, pos.y + -graphScale, pos.z), TransformMatrix);
        vertex[3].pos = VTransform(VGet(pos.x + graphScale, pos.y + -graphScale, pos.z), TransformMatrix);

        vertex[4].pos = vertex[2].pos;
        vertex[5].pos = vertex[1].pos;

        // 行列を使って法線を算出
        vertex[0].norm = VTransformSR(VGet(0.0f, 0.0f, -1.0f), TransformMatrix);
        vertex[1].norm = vertex[0].norm;
        vertex[2].norm = vertex[0].norm;
        vertex[3].norm = vertex[0].norm;
        vertex[4].norm = vertex[0].norm;
        vertex[5].norm = vertex[0].norm;
    }



    MATERIALPARAM Material;

    // 色
    {
        Material.Diffuse = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
        Material.Specular = GetColorF(1.0f, 1.0f, 1.0f, 0.0f);
        Material.Ambient = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
        Material.Emissive = GetColorF(color.r, color.g, color.b, 0.0f);
        Material.Power = 20.0f;


        SetMaterialParam(Material);
    }


    // 2Dポリゴンの描画
    DrawPolygon3D(vertex, 2, graphHandle, true);


    // 初期マテリアル
    MATERIALPARAM initMaterial = {};
   

    // ここでマテリアルのパラメータを元に戻す
    SetMaterialParam(initMaterial);
}

void Draw_Lib::DrawCube3D_EvoLib(const VECTOR& centerPos, const VECTOR& oneSideLength, int alphaValue, int lineThickness, Color color)
{

    // アルファブレンドをかける
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);

    // 半分の一辺の値
    VECTOR halfOneSideLength = VScale(oneSideLength, 0.5f);


    VECTOR start = centerPos;
    VECTOR end = centerPos;

    for (int i = 0; i < lineThickness; i++)
    {

        float shiftValue = i * 0.05f;

        {
            start = VAdd(centerPos,VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x -= halfOneSideLength.x;
            start.y += halfOneSideLength.y;
            start.z -= halfOneSideLength.z;

            end.x += halfOneSideLength.x;
            end.y += halfOneSideLength.y;
            end.z -= halfOneSideLength.z;

            DrawLine3D(start, end, GetColor(0, 0, 0));
        }

        {
            start = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x -= halfOneSideLength.x;
            start.y -= halfOneSideLength.y;
            start.z -= halfOneSideLength.z;

            end.x += halfOneSideLength.x;
            end.y -= halfOneSideLength.y;
            end.z -= halfOneSideLength.z;


            DrawLine3D(start, end, GetColor(0, 0, 0));
        }

        {
            start = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x -= halfOneSideLength.x;
            start.y -= halfOneSideLength.y;
            start.z += halfOneSideLength.z;

            end.x += halfOneSideLength.x;
            end.y -= halfOneSideLength.y;
            end.z += halfOneSideLength.z;


            DrawLine3D(start, end, GetColor(0, 0, 0));
        }

        {
            start = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x -= halfOneSideLength.x;
            start.y += halfOneSideLength.y;
            start.z += halfOneSideLength.z;

            end.x += halfOneSideLength.x;
            end.y += halfOneSideLength.y;
            end.z += halfOneSideLength.z;


            DrawLine3D(start, end, GetColor(0, 0, 0));
        }

        {
            start = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x += halfOneSideLength.x;
            start.y += halfOneSideLength.y;
            start.z -= halfOneSideLength.z;

            end.x += halfOneSideLength.x;
            end.y += halfOneSideLength.y;
            end.z += halfOneSideLength.z;


            DrawLine3D(start, end, GetColor(0, 0, 0));
        }

        {
            start = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x += halfOneSideLength.x;
            start.y -= halfOneSideLength.y;
            start.z -= halfOneSideLength.z;

            end.x += halfOneSideLength.x;
            end.y -= halfOneSideLength.y;
            end.z += halfOneSideLength.z;


            DrawLine3D(start, end, GetColor(0, 0, 0));
        }

        {
            start = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x -= halfOneSideLength.x;
            start.y -= halfOneSideLength.y;
            start.z -= halfOneSideLength.z;

            end.x -= halfOneSideLength.x;
            end.y -= halfOneSideLength.y;
            end.z += halfOneSideLength.z;


            DrawLine3D(start, end, GetColor(0, 0, 0));
        }

        {
            start = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x -= halfOneSideLength.x;
            start.y += halfOneSideLength.y;
            start.z -= halfOneSideLength.z;

            end.x -= halfOneSideLength.x;
            end.y += halfOneSideLength.y;
            end.z += halfOneSideLength.z;


            DrawLine3D(start, end, GetColor(0, 0, 0));
        }

        {
            start = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x += halfOneSideLength.x;
            start.y += halfOneSideLength.y;
            start.z -= halfOneSideLength.z;

            end.x += halfOneSideLength.x;
            end.y -= halfOneSideLength.y;
            end.z -= halfOneSideLength.z;


            DrawLine3D(start, end, GetColor(0, 0, 0));
        }

        {
            start = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x += halfOneSideLength.x;
            start.y += halfOneSideLength.y;
            start.z += halfOneSideLength.z;

            end.x += halfOneSideLength.x;
            end.y -= halfOneSideLength.y;
            end.z += halfOneSideLength.z;


            DrawLine3D(start, end, GetColor(0, 0, 0));
        }

        {
            start = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x -= halfOneSideLength.x;
            start.y += halfOneSideLength.y;
            start.z += halfOneSideLength.z;

            end.x -= halfOneSideLength.x;
            end.y -= halfOneSideLength.y;
            end.z += halfOneSideLength.z;


            DrawLine3D(start, end, GetColor(0, 0, 0));
        }


        {
            start = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));
            end = VAdd(centerPos, VGet(shiftValue, shiftValue, shiftValue));


            start.x -= halfOneSideLength.x;
            start.y += halfOneSideLength.y;
            start.z -= halfOneSideLength.z;

            end.x -= halfOneSideLength.x;
            end.y -= halfOneSideLength.y;
            end.z -= halfOneSideLength.z;


            DrawLine3D(start, end, GetColor(0, 0, 0));
        }
    }

    // 通常描画に戻す
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
