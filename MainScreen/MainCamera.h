#pragma once
#include <DxLib.h>
#include <memory>


class MainParam;
class MainCamera
{
public:
	MainCamera();
	virtual ~MainCamera();

	/// <summary>
	/// メインパラㇺポインタを取得
	/// </summary>
	/// <param name="mainParam">メインパラㇺポインタ</param>
	void SetMainParamPointer(std::shared_ptr<MainParam>mainParam) { m_pMainParam = mainParam; }


	void Init();
	void Update();



private:

	/// <summary>
	/// カメラ移動
	/// </summary>
	void Move();

	/// <summary>
	/// カメラ角度
	/// </summary>
	void CameraAngle();

private:

	//////////////
	// 座標関連 //
	//////////////

	// カメラ座標
	VECTOR m_cameraPos;

	// ターゲット座標
	VECTOR m_targetPos;

	//////////////
	// 角度関連 //
	//////////////

	// カメラ角度
	int m_cameraAngle;

	////////////////////
	// 処理フラグ関連 //
	////////////////////

	// ターゲット位置を処理したかどうか
	bool m_isProcessTagetPos;


	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// メインパラㇺポインタ
	std::shared_ptr<MainParam>m_pMainParam;

};

