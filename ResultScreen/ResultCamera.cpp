#include "ResultCamera.h"
#include <DxLib.h>

namespace
{
	// カメラ初期位置
	const VECTOR kCameraInitPos = VGet(0.0f, 10.0f, -30.0f);

	// カメラニアー
	constexpr float kCameraNear = 5.0f;

	// カメラファー
	constexpr float kCameraFar = 1000.0f;
}

ResultCamera::ResultCamera()
{
}

ResultCamera::~ResultCamera()
{
}

void ResultCamera::Init()
{
	// カメラの 手前クリップ距離と 奥クリップ距離を設定する
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void ResultCamera::Update()
{
	// カメラの回転角度
	SetCameraPositionAndAngle(kCameraInitPos, 0.0f, 0.0f, 0.0f);
}
