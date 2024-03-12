#include "ResultCamera.h"
#include <DxLib.h>

namespace
{
	// �J���������ʒu
	const VECTOR kCameraInitPos = VGet(0.0f, 10.0f, -30.0f);

	// �J�����j�A�[
	constexpr float kCameraNear = 5.0f;

	// �J�����t�@�[
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
	// �J������ ��O�N���b�v������ ���N���b�v������ݒ肷��
	SetCameraNearFar(kCameraNear, kCameraFar);
}

void ResultCamera::Update()
{
	// �J�����̉�]�p�x
	SetCameraPositionAndAngle(kCameraInitPos, 0.0f, 0.0f, 0.0f);
}
