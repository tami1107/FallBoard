#include "TitleScreen.h"
#include <DxLib.h>

namespace
{
	// �t�@�C���p�X
	const char* const kFilePath = "Data/Graphic/Title/titleMovie.mp4";

	// ����̍Đ��t���[��
	constexpr int kMovePlayMaxFrame = 6600;
}


TitleScreen::TitleScreen():
	// �O���t�B�b�N�֘A��
	m_handle(),
	m_frame()
{
}

TitleScreen::~TitleScreen()
{
	// �O���t�B�b�N�̍폜
	DeleteGraph(m_handle);
}

void TitleScreen::Init()
{
	// �O���t�B�b�N�̃��[�h
	m_handle = LoadGraph(kFilePath);

	// �t���[��������
	m_frame = 0;

	// ���[�r�[���Đ���Ԃɂ��܂�
	PlayMovieToGraph(m_handle);
}

void TitleScreen::Update()
{
	// �t���[�������Z����
	m_frame++;

	// �l��kMovePlayMaxFrame�̒l�𒴂����ꍇ�A0��������
	if (m_frame > kMovePlayMaxFrame)
	{
		// ���[�r�[�̍Đ��ʒu��0�b�ڂɕύX
		SeekMovieToGraph(m_handle, 0);

		// 0����
		m_frame = 0;
	}
}

void TitleScreen::Draw()
{
	// ����̍Đ�
	DrawGraph(0, 0, m_handle, false);
}
