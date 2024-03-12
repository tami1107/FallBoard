#pragma once
#include "Vec2.h"


namespace Game
{
	// �E�C���h�E���[�h�ݒ�
	constexpr bool kWindowMode = true;

	// �E�C���h�E��
	const char* const kTitleText = "FallBoard";

	// �E�C���h�E�T�C�Y
	constexpr int kScreenWidth = 1800;
	constexpr int kScreenHeight = 900;


	// �J���[���[�h
	constexpr int kColorDepth = 32;		// 32 or 16

	// �E�B���h�E�̒��S���W
	const Vec2 kScreenCenterPos = Vec2((static_cast<float>(kScreenWidth)) * 0.5f, (static_cast<float>(kScreenHeight)) * 0.5f);
};