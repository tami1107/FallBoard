#include "SceneBase.h"
#include "Loading.h"
#include <DxLib.h>
#include "Game.h"

namespace
{
	// フェード速度
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

	// ロード処理中かどうかを返す
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
/// フェード描画
/// </summary>
/// <param name="fadeColor">フェード時の色を入れる</param>
void SceneBase::DrawFade(int fadeColor) const
{
	if (m_isLoading)return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#if false
	// フェードスピードの描画
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
/// フェードスピードを設定する
/// </summary>
/// <param name="fadeSpeed">フェードスピードの値(値を入れなかった場合、デフォルト値が入る)</param>
void SceneBase::SetFadeSpeed(int fadeSpeed)
{
	// フェードのスピードを設定する
	m_fadeSetUpSpeed = fadeSpeed;

	// 設定したフェードのマイナススピードを代入する
	m_fadeSpeed = -m_fadeSetUpSpeed;
}

