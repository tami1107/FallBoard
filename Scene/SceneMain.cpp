#include "StateMachine.h"// ������͈�ԏ�ɂ����Ă���

#include "SceneMain.h"
#include "SceneResult.h"
#include "Player.h"
#include "Sound.h"
#include "MainScreen.h"
#include "SceneSelect.h"
#include "SceneTitle.h"
#include "ModelData.h"

namespace
{
	// �t�F�[�h�C���A�E�g�̃X�s�[�h
	constexpr int kFadeSpeed = 6;
}

SceneMain::SceneMain():
	// �V�[���֘A��
	m_changeScene(),
	// �N���X�|�C���^�֘A��
	m_pMainScreen(std::make_shared<MainScreen>())
{
}

SceneMain::~SceneMain()
{
	// �T�E���h�̃A�����[�h
	Sound::getInstance().UnLoad();
}

void SceneMain::Init()
{
	// �t�F�[�h�̃X�s�[�h��������
	SetFadeSpeed(kFadeSpeed);

	////////////////////////////////////////////
	// ����扺����e�N���X�̏��������������� //
	////////////////////////////////////////////

	// ���g�̃|�C���^�����C���X�N���[���N���X�ɑ���
	m_pMainScreen->SetSceneMain(this);

	// ���C���X�N���[���̏���������
	m_pMainScreen->Init();
}

SceneBase* SceneMain::Update()
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

			// �V�[���ύX�����U���g�ȊO�Ȃ�΁A�v���C���f�����폜����
			if (m_changeScene != SceneMain::ChangeScene::Result)
			{
				ModelData::getInstance().DeleteGamePlayModel();
			}


			// m_changeScene�̒��g�ɂ���Ď��̃V�[����ύX����
			switch (m_changeScene)
			{
			case SceneMain::ChangeScene::CharSelect:

				// �V�[���Z���N�g�ֈڍs
				return (new SceneSelect);

				break;
			case SceneMain::ChangeScene::Main:

				// �V�[�����C���ֈڍs
				return (new SceneMain);

				break;
			case SceneMain::ChangeScene::Result:

				// �V�[�����U���g�ֈڍs
				return (new SceneResult);

				break;
			case SceneMain::ChangeScene::Title:

				// �V�[���^�C�g���ֈڍs
				return (new SceneTitle);

				break;

			default:
				// m_changeScene�̒��g����L��case�ȊO�ł��������p�����čs����悤�ɁA�V�[�����U���g�ֈڍs
				return (new SceneResult);

				break;
			}
		}
	}

	//////////////////////////////////////////
	// �����艺�Ɋe�N���X�̍X�V���������� //
	//////////////////////////////////////////


	// ���C���X�N���[���̍X�V����
	m_pMainScreen->Update();


	return this;
}

void SceneMain::Draw()
{
	// ���C���X�N���[���̕`�揈��
	m_pMainScreen->Draw();


	//////////////////////////////////////////
	// �������Ɋe�N���X�̕`�揈�������� //
	//////////////////////////////////////////

	// �t�F�[�h�C���A�E�g�̃t�B���^�[
	DrawFade();
}

void SceneMain::IsASynchronousLoad(bool isLoading)
{
	// ���[�h���s��
	LoadingInit(isLoading);
}

bool SceneMain::GetLoading()
{
	// ���[�h�����ǂ���
	const bool isLoading = IsLoadingProcess();

	// ���[�h���s���Ă��邩�ǂ�����Ԃ�
	return isLoading;
}

void SceneMain::FadeOut(ChangeScene changeScene)
{
	// ���݁A�t�F�[�h���s���Ă��Ȃ���΃t�F�[�h�A�E�g�̏����ݒ���s���A���s����
	if (!IsFading())
	{
		// �l����
		m_changeScene = changeScene;

		// �t�F�[�h�A�E�g�����s
		StartFadeOut();	
	}
}

void SceneMain::FadeSound()
{
	// �t�F�[�h�A�E�g���ł���΁A���̃t�F�[�h�ɍ��킹��BGM������������
	if (m_isFadeOut)
	{
		// �t�F�[�h�A�E�g���I���Ɠ����ɉ����~�܂�
		Sound::getInstance().ScreenFadeBGMStop(FadeBrightValue());
	}
}

