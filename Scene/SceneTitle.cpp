#include "SceneTitle.h"
#include "SceneSelect.h"
#include "Sound.h"
#include "Pad.h"
#include "TitleScreen.h"
#include <DxLib.h>


// �T�E���h
namespace SoundFile
{
	// �T�E���h�t�@�C����
	std::vector<std::string> kSoundFileName =
	{
		"taitleBGM",	// �^�C�g��BGM
		"determination",// ���艹
	};

	// �T�E���h��
	enum class SoundName
	{
		TaitleBGM,		// �^�C�g��BGM
		Determination	// ���艹
	};
}

SceneTitle::SceneTitle():
	// �N���X�|�C���^�֘A��
	m_pTitleScreen(std::make_shared<TitleScreen>())
{
}

SceneTitle::~SceneTitle()
{
	// �T�E���h�̃A�����[�h
	Sound::getInstance().UnLoad();
}

void SceneTitle::Init()
{
	// �t�F�[�h�̃X�s�[�h��������
	SetFadeSpeed();

	// �T�E���h�����[�h����
	Sound::getInstance().Load(SoundFile::kSoundFileName);

	// �^�C�g��BGM���Đ�����
	Sound::getInstance().Play(SoundFile::kSoundFileName[static_cast<int>(SoundFile::SoundName::TaitleBGM)]);

	////////////////////////////////////////////
	// ����扺����e�N���X�̏��������������� //
	////////////////////////////////////////////

	// �^�C�g���X�N���[���̏���������
	m_pTitleScreen->Init();
}

SceneBase* SceneTitle::Update()
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
			return (new SceneSelect);
		}
	}

	// �{�^�����������Ƃ��A�t�F�[�h�A�E�g�������s��
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		// �t�F�[�h�A�E�g����
		FadeOut();
	}

	//////////////////////////////////////////
	// �����艺�Ɋe�N���X�̍X�V���������� //
	//////////////////////////////////////////

	// �^�C�g���X�N���[���X�V����
	m_pTitleScreen->Update();

	return this;
}

void SceneTitle::Draw()
{
	// �^�C�g���X�N���[���`�揈��
	m_pTitleScreen->Draw();

	//////////////////////////////////////////
	// �������Ɋe�N���X�̕`�揈�������� //
	//////////////////////////////////////////

	// �t�F�[�h�C���A�E�g�̃t�B���^�[
	SceneBase::DrawFade();
}

void SceneTitle::IsASynchronousLoad(bool isLoading)
{
	// ���[�h���s��
	LoadingInit(isLoading);
}

bool SceneTitle::GetLoading()
{
	// ���[�h�����ǂ���
	const bool isLoading = IsLoadingProcess();

	// ���[�h���s���Ă��邩�ǂ�����Ԃ�
	return isLoading;
}

void SceneTitle::FadeOut()
{
	// ���݁A�t�F�[�h���s���Ă��Ȃ���΃t�F�[�h�A�E�g�̏����ݒ���s���A���s����
	if (!IsFading())
	{
		// ���艹���Đ�����
		Sound::getInstance().Play(SoundFile::kSoundFileName[static_cast<int>(SoundFile::SoundName::Determination)]);

		// �t�F�[�h�A�E�g�����s
		StartFadeOut();
	}
}

void SceneTitle::FadeSound()
{
	// �t�F�[�h�A�E�g���ł���΁A���̃t�F�[�h�ɍ��킹��BGM������������
	if (m_isFadeOut)
	{
		// �t�F�[�h�A�E�g���I���Ɠ����ɉ����~�܂�
		Sound::getInstance().ScreenFadeBGMStop(FadeBrightValue());
	}
}
