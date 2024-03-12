#include "MainCamera.h"
#include "EvoLib.h"
#include "Pad.h"
#include "MainParam.h"

namespace
{
	// カメラニアー
	constexpr float kCameraNear = 20.0f;

	// カメラファー
	constexpr float kCameraFar = 1000.0f;

	// カメラ基準位置
	const VECTOR kCameraBasePos = VGet(0.0f, 50.0f, -30.0f);

	// カメラ調整座標
	constexpr float kAdjustmentCameraPos = 10.0f;

	// カメラ角度
	constexpr int kCameraAngle = 60;

	// カメラスピード
	constexpr float kCameraSpeed = 0.5f;
}


MainCamera::MainCamera():
	// 座標関連↓
	m_cameraPos(),
	m_targetPos(),
	// 角度関連↓
	m_cameraAngle(),
	// 処理フラグ関連↓
	m_isProcessTagetPos(),
	// ポインタ関連↓
	m_pMainParam(nullptr)
{
}

MainCamera::~MainCamera()
{
}

void MainCamera::Init()
{
	// カメラの 手前クリップ距離と 奥クリップ距離を設定する
	SetCameraNearFar(kCameraNear, kCameraFar);

	// カメラ位置計算
	{
		// フィールドの盤面スケールを代入
		const int fieldScale = m_pMainParam->GetBoardScale();

		// スケールを計算
		const int scale = (fieldScale - 4);

		// カメラスケールを求める
		const int cameraScale = scale / 2;




		// カメラ調整座標を求める
		const float adjustmentCoordinate = kAdjustmentCameraPos * cameraScale;

		// カメラ基準座標を代入
		m_cameraPos = kCameraBasePos;


		m_cameraPos = VScale(kCameraBasePos, (kAdjustmentCameraPos * 0.1));


		// カメラ調整座標を加算する
		m_cameraPos.y += adjustmentCoordinate;
		m_cameraPos.z -= adjustmentCoordinate;


		// ターゲット座標にカメラ座標を代入
		m_targetPos = m_cameraPos;
	}

	// カメラ角度代入
	m_cameraAngle = kCameraAngle;

	// 処理フラグ初期化
	m_isProcessTagetPos = false;
}

void MainCamera::Update()
{
	// カメラ移動
	Move();

	// カメラ角度
	CameraAngle();
}

void MainCamera::Move()
{
	// 落下処理を行っていなければここで処理を終了する
	if (!m_pMainParam->GetIsDropBoard())return;

	// ターゲットカメラ処理が終了していなければ、処理を行う
	if (!m_isProcessTagetPos)
	{
		// ターゲットカメラ処理に値を入れる
		m_targetPos.y -= kAdjustmentCameraPos;
		m_targetPos.z += kAdjustmentCameraPos;

		// カメラ処理判定をtrueにする
		m_isProcessTagetPos = true;
	}


	// カメラを手前に動かす
	m_cameraPos.y -= kCameraSpeed;
	m_cameraPos.z += kCameraSpeed;


	// カメラの座標がターゲット座標より下回っていた場合、ターゲット座標を代入する
	if (m_cameraPos.y <= m_targetPos.y &&
		m_cameraPos.z <= m_targetPos.z)
	{
		// ターゲット座標を代入
		m_cameraPos.y = m_targetPos.y;
		m_cameraPos.z = m_targetPos.z;

		// カメラ処理判定をflaseにする
		m_isProcessTagetPos = false;

		// メインパラㇺにfalseの落下処理フラグを送る
		m_pMainParam->SetIsDropBoard(false);
	}
}

void MainCamera::CameraAngle()
{
	// ラジアンを求める
	const float cameraRad = static_cast<float>(Convert_Lib::ConvertAngleToRadian(static_cast<float>(m_cameraAngle)));

	// カメラの回転角度
	SetCameraPositionAndAngle(m_cameraPos, cameraRad, 0.0f, 0.0f);
}
