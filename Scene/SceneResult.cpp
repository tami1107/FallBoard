#include "SceneResult.h"
#include "Pad.h"
#include "SceneSelect.h"
#include "Game.h"
#include "ResultScreen.h"
#include "Sound.h"
#include <DxLib.h>


SceneResult::SceneResult() :
	// �N���X�|�C���^�֘A��
	m_pResultScreen(std::make_shared<ResultScreen>())
{
}

SceneResult::~SceneResult()
{
	// �T�E���h�̃A�����[�h
	Sound::getInstance().UnLoad();
}

void SceneResult::Init()
{
	// ���g�̃|�C���^�𑗂�
	m_pResultScreen->SetSceneMain(this);

    // �t�F�[�h�̃X�s�[�h��������
    SetFadeSpeed();


	////////////////////////////////////////////
	// ����扺����e�N���X�̏��������������� //
	////////////////////////////////////////////

	// ���U���g�̏���������
	m_pResultScreen->Init();
}

SceneBase* SceneResult::Update()
{
	// �t�F�[�h�C���A�E�g���Ă���
	if (IsFading())
	{
		m_isFadeOut = IsFadingOut();
		SceneBase::UpdateFade();

		// �t�F�[�h���̃T�E���h����
		FadeSound();

		// �t�F�[�h�A�E�g�I����
		if (!IsFading() && m_isFadeOut && !m_isBackScene)
		{
			// �V�[���Z���N�g�փV�[�����ڍs����
			return (new SceneSelect);
		}
	}

	//////////////////////////////////////////
	// �����艺�Ɋe�N���X�̍X�V���������� //
	//////////////////////////////////////////

	// ���U���g�̍X�V����
	m_pResultScreen->Update();


    return this;
}

void SceneResult::Draw()
{
	// ���U���g�̍X�V����
	m_pResultScreen->Draw();

	//////////////////////////////////////////
	// �������Ɋe�N���X�̕`�揈�������� //
	//////////////////////////////////////////

	// �t�F�[�h�C���A�E�g�̃t�B���^�[
	DrawFade();
}

void SceneResult::IsASynchronousLoad(bool isLoading)
{
	// ���[�h���s��
	LoadingInit(isLoading);
}

bool SceneResult::GetLoading()
{
	// ���[�h�����ǂ���
	const bool isLoading = IsLoadingProcess();

	// ���[�h���s���Ă��邩�ǂ�����Ԃ�
	return isLoading;
}

void SceneResult::FadeOut()
{
	// ���݁A�t�F�[�h���s���Ă��Ȃ���΃t�F�[�h�A�E�g�̏����ݒ���s���A���s����
	if (!IsFading())
	{
		// �t�F�[�h�A�E�g�����s
		StartFadeOut();
	}
}

void SceneResult::FadeSound()
{
	// �t�F�[�h�A�E�g���ł���΁A���̃t�F�[�h�ɍ��킹��BGM������������
	if (m_isFadeOut)
	{
		// �t�F�[�h�A�E�g���I���Ɠ����ɉ����~�܂�
		Sound::getInstance().ScreenFadeBGMStop(FadeBrightValue());
	}
}
