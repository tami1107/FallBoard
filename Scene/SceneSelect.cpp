#include "SceneSelect.h"
#include "Pad.h"
#include "Game.h"
#include "Sound.h"
#include "SelectScreen.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include <DxLib.h>

namespace
{
	// �t�F�[�h�C���A�E�g�̃X�s�[�h
	constexpr int kFadeSpeed = 6;
}

SceneSelect::SceneSelect():
	// �V�[���ύX�֘A��
	m_changeScene(),
	// �N���X�|�C���^�֘A��
    m_pSelectScreen(std::make_shared<SelectScreen>())
{
}

SceneSelect::~SceneSelect()
{
	// �T�E���h�̃A�����[�h
	Sound::getInstance().UnLoad();
}

void SceneSelect::Init()
{
	// �t�F�[�h�̃X�s�[�h��������
	SetFadeSpeed(kFadeSpeed);


	////////////////////////////////////////////
	// ����扺����e�N���X�̏��������������� //
	////////////////////////////////////////////

	// ���g�̃|�C���^���Z���N�g�X�N���[���N���X�ɑ���
	m_pSelectScreen->SetSceneCharSelectPointer(this);

	// �Z���N�g�X�N���[���̏���������
	m_pSelectScreen->Init();
}

SceneBase* SceneSelect::Update()
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

			// m_changeScene�̒��g�ɂ���Ď��̃V�[����ύX����
			switch (m_changeScene)
			{
			case SceneSelect::ChangeScene::Main:

				// �V�[�����C���ֈڍs
				return (new SceneMain);

				break;

			case SceneSelect::ChangeScene::Title:

				// �V�[���^�C�g���ֈڍs
				return (new SceneTitle);

				break;

			default:

				// m_changeScene�̒��g����L��case�ȊO�ł��������p�����čs����悤�ɁA�V�[�����C���ֈڍs
				return (new SceneMain);

				break;
			}
		}
	}


	//////////////////////////////////////////
	// �����艺�Ɋe�N���X�̍X�V���������� //
	//////////////////////////////////////////


	// �Z���N�g�X�N���[���̍X�V����
	m_pSelectScreen->Update();

    return this;
}

void SceneSelect::Draw()
{
	// �L�����Z���N�g�X�N���[���̍X�V����
	m_pSelectScreen->Draw();



	//////////////////////////////////////////
	// �������Ɋe�N���X�̍X�V���������� //
	//////////////////////////////////////////

	// �t�F�[�h�C���A�E�g�̃t�B���^�[
	DrawFade();
}

void SceneSelect::IsASynchronousLoad(bool isLoading)
{
	// ���[�h���s��
	LoadingInit(isLoading);
}

bool SceneSelect::GetLoading()
{
	// ���[�h���s���Ă��邩�ǂ���
	const bool isLoading = IsLoadingProcess();

	// ���[�h���s���Ă��邩�ǂ�����Ԃ�
	return isLoading;
}

void SceneSelect::FadeOut(ChangeScene changeScene)
{
	// ���݁A�t�F�[�h���s���Ă��Ȃ���΃t�F�[�h�A�E�g�̏����ݒ���s���A���s����
	if (!IsFading())
	{
		// �ǂ̃V�[���ɕύX���邩��������
		m_changeScene = changeScene;

		// �t�F�[�h�A�E�g�����s
		StartFadeOut();
	}
}

void SceneSelect::FadeSound()
{
	// �t�F�[�h�A�E�g���ł���΁A���̃t�F�[�h�ɍ��킹��BGM������������
	if (m_isFadeOut)
	{
		// �t�F�[�h�A�E�g���I���Ɠ����ɉ����~�܂�
		Sound::getInstance().ScreenFadeBGMStop(FadeBrightValue());
	}
}