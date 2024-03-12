#include "SceneBase.h"
#include "Loading.h"
#include <DxLib.h>
#include "Game.h"

namespace
{
	// �t�F�[�h���x
	constexpr int kFadeSpeed = 5;
}

SceneBase::SceneBase() :
	m_fadeBright(255),
	m_fadeSpeed(-kFadeSpeed),
	m_fadeSetUpSpeed(kFadeSpeed),
	m_isFadeOut(false),
	m_isBackScene(false),
	m_isLoading(false)
{
}

void SceneBase::LoadingInit(bool isLoading)
{
	if (!isLoading)return;

	m_isLoading = true;

	m_pLoading = std::make_shared<Loading>();
	m_pLoading->Init();
}

bool SceneBase::IsLoadingProcess()
{
	m_pLoading->Update();
	m_pLoading->Draw();

	// ���[�h���������ǂ�����Ԃ�
	m_isLoading = m_pLoading->GetIsLoading();

	return m_isLoading;
}

void SceneBase::UpdateFade()
{
	if (m_isLoading)return;


	m_fadeBright += m_fadeSpeed;
	if (m_fadeBright >= 255)
	{
		m_fadeBright = 255;
		if (m_fadeSpeed > 0)
		{
			m_fadeSpeed = 0;
		}
	}
	if (m_fadeBright <= 0)
	{
		m_fadeBright = 0;
		if (m_fadeSpeed < 0)
		{
			m_fadeSpeed = 0;
		}
	}
}

/// <summary>
/// �t�F�[�h�`��
/// </summary>
/// <param name="fadeColor">�t�F�[�h���̐F������</param>
void SceneBase::DrawFade(int fadeColor) const
{
	if (m_isLoading)return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#if false
	// �t�F�[�h�X�s�[�h�̕`��
	DrawFormatString(0, 15 * 2, 0xff0000, "m_fadeSpeed=%d", m_fadeSpeed);

	DrawFormatString(0, 15 * 3, 0xff0000, "m_fadeBright=%d", m_fadeBright);
#endif
}

bool SceneBase::IsFadingIn() const
{
	if (m_fadeSpeed < 0)	return true;
	return false;
}

bool SceneBase::IsFadingOut() const
{
	if (m_fadeSpeed > 0)	return true;
	return false;
}

void SceneBase::StartFadeOut()
{
	m_fadeSpeed = m_fadeSetUpSpeed;
}

/// <summary>
/// �t�F�[�h�X�s�[�h��ݒ肷��
/// </summary>
/// <param name="fadeSpeed">�t�F�[�h�X�s�[�h�̒l(�l�����Ȃ������ꍇ�A�f�t�H���g�l������)</param>
void SceneBase::SetFadeSpeed(int fadeSpeed)
{
	// �t�F�[�h�̃X�s�[�h��ݒ肷��
	m_fadeSetUpSpeed = fadeSpeed;

	// �ݒ肵���t�F�[�h�̃}�C�i�X�X�s�[�h��������
	m_fadeSpeed = -m_fadeSetUpSpeed;
}

