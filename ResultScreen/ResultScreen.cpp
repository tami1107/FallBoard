#include "StateMachine.h"// ����ԏ�ɏ���
#include "ResultScreen.h"
#include "Pad.h"
#include "GameData.h"
#include "ResultCharacter.h"
#include "ResultCamera.h"
#include "ResultBackground.h"
#include <DxLib.h>
#include "Sound.h"

namespace
{
	// �T�E���h�t�@�C����
	const std::vector<std::string> kSoundFileName =
	{
		"win",			// ������
		"resultBGM",	// ���U���gBGM
		"determination"	// ���艹
	};

	// �T�E���h��
	enum class SoundName
	{
		Win,			// ������
		ResultBGM,		// ���U���gBGM
		Determination,	// ���艹
	};
}

ResultScreen::ResultScreen() :
	// �X�e�[�g�֘A��
	m_state(),
	// �N���X�|�C���^�֘A��
	m_pSceneResult(nullptr),
	m_pResultCharacter(std::make_shared<ResultCharacter>()),
	m_pResultCamera(std::make_shared<ResultCamera>()),
	m_pResultBackground(std::make_shared<ResultBackground>())
{
}

ResultScreen::~ResultScreen()
{
	// �|�C���^�̍폜
	m_pSceneResult = nullptr;
	delete m_pSceneResult;

	// ���f���폜
	ModelData::getInstance().DeleteGamePlayModel();

	// �T�E���h�A�����[�h
	Sound::getInstance().UnLoad();
}

void ResultScreen::Init()
{
	// �X�e�[�g����������
	InitState();

	// ���U���g�o�b�N�O���E���h�Ƀ��U���g�L�����N�^�[�|�C���^�𑗂�
	m_pResultBackground->SetResultCharacterPointer(m_pResultCharacter);

	// ���[�h����
	Load();

	

	// ����SE���Đ�����
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Win)]);

	// ���U���gBGM���Đ�����
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::ResultBGM)]);
}

void ResultScreen::Update()
{
	// �X�e�[�g�}�V���̍X�V����
	m_pStateMachine.Update();
	// ���f���X�V����
	m_pResultCharacter->Update();
	// �J�����X�V����
	m_pResultCamera->Update();
	// �w�i�X�V����
	m_pResultBackground->Update();
}

void ResultScreen::Draw()
{
	// �w�i�`�揈��
	m_pResultBackground->Draw();

	// ���f���`�揈��
	m_pResultCharacter->Draw();

	// �����L���O�`�揈��
	m_pResultBackground->DrawRanking();

	// �N���E���`�揈��
	m_pResultBackground->DrawCrown();

	// �G�t�F�N�g�`�揈��
	m_pResultBackground->DrawEffect();

	// ���̃V�[���̐����e�L�X�g�`�揈��
	m_pResultBackground->DrawNextSceneText();
}

void ResultScreen::NormalEnter()
{
	// �e�N���X�̏���������
	m_pResultCharacter->Init();
	m_pResultCamera->Init();
	m_pResultBackground->Init();
}

void ResultScreen::NormalUpdate()
{
	// �{�^�����������Ƃ��A�t�F�[�h�C�����łȂ���΃X�e�[�g��ύX����
	if (Pad::IsTrigger(PAD_INPUT_1)&&
		!m_pSceneResult->IsFadingIn())
	{
		// ���艹���Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);

		// End�X�e�[�g���Z�b�g
		m_pStateMachine.SetState(State::End);
	}
}


void ResultScreen::EndEnter()
{
	// �t�F�[�h�A�E�g����
	m_pSceneResult->FadeOut();
}

void ResultScreen::Load()
{
	// �T�E���h���[�h
	Sound::getInstance().Load(kSoundFileName);
}

void ResultScreen::InitState()
{
	// �X�e�[�g�}�V���̏������AEntry
	auto dummy = []() {};

	// �ʏ폈�����X�e�[�g�}�V���ɒǉ�
	{
		auto normalEnter = [this]() {NormalEnter(); };
		auto normalUpdate = [this]() {NormalUpdate(); };
		auto normalDraw = dummy;
		auto normalExit = dummy;

		// �X�e�[�g�ݒ�
		m_pStateMachine.AddState(State::Normal, normalEnter, normalUpdate, normalDraw, normalExit);
	}

	// �I���������X�e�[�g�}�V���ɒǉ�
	{
		auto endEnter = [this]() {EndEnter(); };
		auto endUpdate = dummy;
		auto endDraw = dummy;
		auto endExit = dummy;

		// �X�e�[�g�ݒ�
		m_pStateMachine.AddState(State::End, endEnter, endUpdate, endDraw, endExit);
	}

	// �X�e�[�g�Z�b�g
	SetStateChange(State::Normal);
}

void ResultScreen::SetStateChange(const State& state)
{
	// �X�e�[�g���
	m_state = state;

	// �X�e�[�g�Z�b�g
	m_pStateMachine.SetState(m_state);
}
