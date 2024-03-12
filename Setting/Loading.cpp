#include "StateMachine.h"// ������͈�ԏ�ɂ����Ă���
#include "Loading.h"
#include <DxLib.h>
#include "Game.h"
#include "EvoLib.h"

namespace
{
	// �Œ���̃��[�h�t���[��
	constexpr int kMinimumFrame = 60;

	// �t�F�[�h�X�s�[�h
	constexpr int kFadeSpeed = 30;

	// �t�F�[�h�ő�l
	constexpr int kFadeMaxValue = 255;

	// �t�F�[�h�J���[
	constexpr int kFadeColor= 0x000000;
}

// �O���t�B�b�N
namespace Graph
{
	// �O���t�B�b�N�t�@�C���p�X
	const char* const kFilePath = "Data/Graphic/Load/loadText.png";

	// �O���t�B�b�N�̍��W
	const Vec2 kPos = Vec2(1500.0f, 800.0f);

	// �O���t�B�b�N�X�P�[��
	constexpr float kGraphScale = 1.0f;

	// �T�C���J�[�u�t���[��
	constexpr int kSinMaxFrameValue = 150;

	// �T�C���J�[�u�̑傫��
	constexpr int kSinMaxValue = 20;
}

Loading::Loading():
	// �O���t�B�b�N�֘A��
	m_handle(),
	m_sinFrame(),
	m_fadeBright(),
	// ���[�h�֘A��
	m_frameCount(),
	m_isMinimumFrame(),
	m_isLoading()
{
}

Loading::~Loading()
{
	// �O���t�B�b�N�̍폜
	DeleteGraph(m_handle);
}

void Loading::Init()
{
	// ���[�h�n���h��
	LoadHanele();

	// �ϐ��̏�����
	m_frameCount = 0;
	m_isMinimumFrame = false;
	m_isLoading = true;
	m_fadeBright = 0;
	m_sinFrame = 0;
}

void Loading::Update()
{
	// �t�F�[�h�l�����Z
	UpdateFadeBright();

	// ���[�h�t���[������
	LoadFrame();

	// �񓯊������̐��𐔂���
	ASyncLoadingCount();
}

void Loading::Draw()
{
	// �����x��ݒ肷��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);

	// ��ʂ𕢂�Box��`�悷��
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kFadeColor, true);
	
	// ���[�h�O���t�B�b�N��`�悷��
	DrawLoadGaph();

	// �����x�����ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Loading::LoadHanele()
{
	// �O���t�B�b�N�����[�h����
	m_handle = LoadGraph(Graph::kFilePath);
}

void Loading::UpdateFadeBright()
{
	// �t�F�[�h�l�����Z
	m_fadeBright += kFadeSpeed;

	// �t�F�[�h�l��kFadeMaxValue�𒴂����ꍇ�AkFadeMaxValue�̒l��������
	if (m_fadeBright >= kFadeMaxValue)
	{
		m_fadeBright = kFadeMaxValue;
	}
}

bool Loading::GetIsLoading()
{
	// �ŒჍ�[�h�t���[���𒴂��Ă��āA�񓯊����������Ă��Ȃ���΁Afalse��Ԃ�
	if (m_isMinimumFrame && !m_isLoading)
	{
		return false;
	}

	// true��Ԃ�
	return true;
}

void Loading::LoadFrame()
{
	// �t���[���̉��Z
	m_frameCount++;

	// �t���[�����Œ���̃��[�h�t���[���𒴂��Ă�����t���O��true�ɂ���
	if (m_frameCount >= kMinimumFrame)
	{
		m_isMinimumFrame = true;
	}
}

void Loading::ASyncLoadingCount()
{
	// �񓯊��������Ă�����𐔂���
	const int ASyncLoadCount = GetASyncLoadNum();

	// �񓯊��������Ă������0�Ȃ�΁A�t���O��false�ɂ���
	if (ASyncLoadCount == 0)
	{
		m_isLoading = false;
	}
}

void Loading::DrawLoadGaph()
{
	// �T�C���t���[��
	{
		// �T�C���t���[�������Z����
		m_sinFrame++;

		// �T�C���t���[����kSinMaxFrameValue�𒴂����ꍇ�A0��������
		if (m_sinFrame >= Graph::kSinMaxFrameValue)
		{
			m_sinFrame = 0;
		}
	}

	// �T�C���l�����߂�
	const int sinValue = Calculation_Lib::SineCurve(m_sinFrame, Graph::kSinMaxFrameValue, Graph::kSinMaxValue);

	// �O���t�B�b�N��`�悷��
	DrawRotaGraphF(Graph::kPos.x, Graph::kPos.y + sinValue, Graph::kGraphScale, 0.0, m_handle, true);
}
