#include "StateMachine.h"// ����ԏ�ɏ���
#include "MainScreen.h"
#include <DxLib.h>
#include "Pad.h"
#include "Sound.h"
#include "MainBackground.h"
#include "MainCamera.h"
#include "Field.h"
#include "CharacterManager.h"
#include "ModelData.h"
#include "MainParam.h"
#include "MainUI.h"
#include "GameData.h"
#include "Pause.h"

MainScreen::MainScreen():
	// �X�e�[�g�֘A��
	m_state(),
	// �N���X�|�C���^�֘A��
	m_pSceneMain(nullptr),
	m_pMainBackground(std::make_shared<MainBackground>()),
	m_pMainCamera(std::make_shared<MainCamera>()),
	m_pField(std::make_shared<Field>()),
	m_pCharacterManager(std::make_shared<CharacterManager>()),
	m_pMainParam(std::make_shared<MainParam>()),
	m_pMainUI(std::make_shared<MainUI>()),
	m_pPause(std::make_shared<Pause>())
{
}

MainScreen::~MainScreen()
{
	// �|�C���^�̍폜
	m_pSceneMain = nullptr;
	delete m_pSceneMain;
}

void MainScreen::Init()
{
	// �X�e�[�g�}�V������������
	InitStateMachine();
}

void MainScreen::Update()
{
	// �X�e�[�g�}�V���̍X�V����
	m_pStateMachine.Update();
}

void MainScreen::Draw()
{
	// �X�e�[�g�}�V���̕`�揈��
	m_pStateMachine.Draw();
}

void MainScreen::InitStateMachine()
{
	// �X�e�[�g�}�V���̏������AEntry
	auto dummy = []() {};

	// �������������X�e�[�g�}�V���ɒǉ�
	{
		auto initEnter = [this]() {InitEnter(); };
		auto initUpdate = [this]() {InitUpdate(); };
		auto initDraw = dummy;
		auto initExit = [this]() {InitExit(); };

		m_pStateMachine.AddState(State::Init, initEnter, initUpdate, initDraw, initExit);
	}

	// �ʏ폈�����X�e�[�g�}�V���ɒǉ�
	{
		auto normalEnter = [this]() {NormalEnter(); };
		auto normalUpdate = [this]() {NormalUpdate(); };
		auto normalDraw = [this]() {NormalDraw(); };
		auto normalExit = dummy;

		m_pStateMachine.AddState(State::Normal, normalEnter, normalUpdate, normalDraw, normalExit);
	}

	// �|�[�Y�������X�e�[�g�}�V���ɒǉ�
	{
		auto pauseEnter = dummy;
		auto pauseUpdate = [this]() {PauseUpdate(); };
		auto pauseDraw = [this]() {PauseDraw(); };
		auto pauseExit = dummy;

		m_pStateMachine.AddState(State::Pause, pauseEnter, pauseUpdate, pauseDraw, pauseExit);
	}


	// �I���������X�e�[�g�}�V���ɒǉ�
	{
		auto endEnter = [this]() {EndEnter(); };
		auto endUpdate = [this]() {EndUpdate(); };
		auto endDraw = [this]() {EndDraw(); };
		auto endExit = dummy;

		m_pStateMachine.AddState(State::End, endEnter, endUpdate, endDraw, endExit);
	}


	// �X�e�[�g�Z�b�g
	SetStateChange(State::Init);
}

void MainScreen::Load()
{
	// �񓯊��̃��[�h���s��
	m_pSceneMain->IsASynchronousLoad(true);

	// �Q�[���v���C�p�̃��f�������[�h����
	ModelData::getInstance().LoadGamePlayModel();

	// �e�N���X�̃n���h�����[�h
	m_pField->LoadHandle();
	m_pMainBackground->LoadHandle();
	m_pMainUI->LoadHandle();
	m_pPause->LoadHandle();
}

void MainScreen::SetStateChange(const State& state)
{
	// �X�e�[�g���
	m_state = state;

	// �X�e�[�g�Z�b�g
	m_pStateMachine.SetState(m_state);
}

void MainScreen::InitEnter()
{
	// ���[�h����
	Load();
}

void MainScreen::InitUpdate()
{
	// ���[�h�����Ă��邩�ǂ������󂯎��
	const bool isLoading = m_pSceneMain->GetLoading();

	// ���[�h���t���O��false�̎��ANormal�X�e�[�g�ɕύX����
	if (!isLoading)
	{
		// �X�e�[�g�Z�b�g
		SetStateChange(State::Normal);
	}
}


void MainScreen::InitExit()
{
	// �|�C���^�n��
	{
		// �L�����N�^�[�}�l�[�W���[�Ƀ|�C���^�𑗂�
		m_pCharacterManager->SetMainScreenPointer(shared_from_this());
		m_pCharacterManager->SetFieldPointer(m_pField);
		m_pCharacterManager->SetMainParamPointer(m_pMainParam);
		m_pCharacterManager->SetMainUIPointer(m_pMainUI);

		// ���C���J�����Ƀ|�C���^�𑗂�
		m_pMainCamera->SetMainParamPointer(m_pMainParam);

		// ���C��UI�Ƀ|�C���^�𑗂�
		m_pMainUI->SetMainParamPointer(m_pMainParam);

		// �t�B�[���h�Ƀ|�C���^�𑗂�
		m_pField->SetMainParamPointer(m_pMainParam);

		// �|�[�Y�Ɏ��g�̃|�C���^�𑗂�
		m_pPause->SetMainScreenPointer(shared_from_this());
	}

	// �e�N���X�̏���������
	{
		m_pMainParam->Init();
		m_pMainBackground->Init();
		m_pField->Init();
		m_pMainCamera->Init();
		m_pCharacterManager->Init();
		m_pMainUI->Init();
		m_pPause->Init();
	}
}

void MainScreen::NormalEnter()
{
	// �J�����`��
	m_pMainCamera->Update();
}

void MainScreen::NormalUpdate()
{
	// �e�N���X�̍X�V����
	m_pMainBackground->Update();
	m_pMainCamera->Update();
	m_pField->Update();
	m_pCharacterManager->Update();
	m_pMainUI->Update();
	m_pPause->Update();
}

void MainScreen::NormalDraw()
{
	// �e�N���X�̕`�揈��
	m_pMainBackground->Draw();
	m_pField->Draw();
	m_pCharacterManager->Draw();
	m_pMainUI->Draw();
}



void MainScreen::PauseUpdate()
{
	// �|�[�Y�X�V����
	m_pPause->Update();
}

void MainScreen::PauseDraw()
{
	// �`�揈��
	m_pMainBackground->Draw(true);
	m_pField->Draw();
	m_pCharacterManager->Draw();
	m_pMainUI->Draw();

	// �|�[�Y�`�揈��
	m_pPause->Draw();
}

void MainScreen::EndEnter()
{
	// �V�[�����C���Ƀt�F�[�h�A�E�g������������
	m_pSceneMain->FadeOut(SceneMain::ChangeScene::Result);
}

void MainScreen::EndUpdate()
{
	// �J�����X�V����
	m_pMainCamera->Update();
}

void MainScreen::EndDraw()
{
	// �`�揈��
	m_pMainBackground->Draw();
	m_pField->Draw();
	m_pCharacterManager->Draw();
	m_pMainUI->Draw();
}