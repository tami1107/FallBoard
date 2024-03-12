#include "StateMachine.h"// ����ԏ�ɏ���
#include "Pause.h"
#include "Pad.h"
#include <DxLib.h>
#include "MainScreen.h"
#include "Game.h"
#include "EvoLib.h"
#include "SoundSetting.h"
#include "Sound.h"


namespace
{
	// �O���t�B�b�N
	struct GraphicInfo
	{
		// �t�@�C���p�X
		const char* filePath = "";

		// ���W
		Vec2 pos = Vec2(0.0f, 0.0f);

		// �O���t�B�b�N�̕�����
		const int div_x = 0;
		const int div_y = 0;
	};


	// �O���t�B�b�N�̃t�@�C���p�X
	const std::vector<GraphicInfo>kGraphicInfo
	{
		// �w�i
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Pause/frame.png",

			// ���W
			Game::kScreenCenterPos,

			// �O���t�B�b�N�̕�����
			1,1,
		},

		// �I���e�L�X�g
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Pause/pauseSelect.png",

			// ���W
			Vec2(Game::kScreenCenterPos.x - 180, Game::kScreenCenterPos.y - 230),

			// �O���t�B�b�N�̕�����
			1,5,
		},

		// ���
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Pause/arrow.png",

			// ���W
			Vec2(Game::kScreenCenterPos.x - 250, Game::kScreenCenterPos.y - 235),

			// �O���t�B�b�N�̕�����
			1,1,
		},
	};

	// �O���t�B�b�N��
	constexpr int kPauseSelectGraphDistance = 100;

	// �O���t�B�b�N��
	enum class GraphName
	{
		Frame,		// �t���[��
		SerectText,	// �Z���N�g�e�L�X�g
		Arrow,		// ���
	};
}

namespace
{
	// �I������
	enum class SelectedItem
	{
		BackToGame,			// �Q�[���ɖ߂�
		RetryFromBeginning,	// ���߂�����Ȃ���
		BackSelectScene,	// �Z���N�g�V�[���ɖ߂�
		SoundSetting,		// ���ʒ���
		BackTitleScene,		// �^�C�g���V�[���ɖ߂�
		SelectedItemNum		// �I�����ڂ̍��v
	};


	// �I�����̃J���[
	const Draw_Lib::Color kSelectColor = { 255.0f, 0.0f, 0.0f };

	// ���ڑI�����̈ړ��C���^�[�o���̍ő�l
	constexpr int kIntervalMaximumValue = 15;

	// �T�C���J�[�u�t���[��
	constexpr int kSinMaxFrameValue= 150;

	// �T�C���J�[�u�̑傫��
	constexpr int kSinMaxValue = 5;
}

namespace
{
	// �T�E���h�t�@�C����
	const std::vector<std::string> kSoundFileName =
	{
		"pause",			// �|�[�Y��
		"determination",	// ���艹
		"choose",			// �I����
		"cancel",			// �L�����Z����
	};

	// �T�E���h��
	enum class SoundName
	{
		Pause,			// �|�[�Y��
		Determination,	// ���艹
		Select,			// �Z���N�g��
		Cancel,			// �L�����Z����
	};
}


Pause::Pause():
	// �O���t�B�b�N�֘A��
	m_graph(),
	// �Z���N�g�֘A��
	m_selectNum(),
	m_selectInterval(),
	m_isScreenChange(),
	m_isSoundSetting(),
	// �T�C���J�[�u�֘A
	m_sinFrame(),
	// �X�e�[�g�֘A��
	m_state(),
	// �N���X�|�C���^�֘A��
	m_pMainScreen()
{
}

Pause::~Pause()
{
	// �O���t�B�b�N�̍폜
	for (int i = 0; i < static_cast<int>(m_graph.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_graph[i].handle.size()); j++)
		{
			DeleteGraph(m_graph[i].handle[j]);
		}
	}
}

void Pause::Init()
{
	// �X�e�[�g�}�V�������ݒ�
	InitStateMachine();

	// �ϐ��̏�����
	{
		m_selectNum = static_cast<int>(SelectedItem::BackToGame);
		m_selectInterval = 0;
		m_isScreenChange = false;
		m_sinFrame = 0;
	}

	// �T�E���h���[�h
	Sound::getInstance().Load(kSoundFileName);
}

void Pause::Update()
{
	// �{�^�����������Ƃ��A�|�[�Y���邩�ǂ���
	if (Pad::IsTrigger(PAD_INPUT_8))
	{
		// �X�e�[�g��Noramal�������ꍇ�A�X�e�[�g��Pause�ɕύX����
		if (m_state == State::Normal)
		{
			// �|�[�YSE���Đ�
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Pause)]);

			// Pause�X�e�[�g�Z�b�g
			SetStateChange(State::Pause);

			// ���C���X�N���[����Pause�X�e�[�g�ɕύX���邱�Ƃ𑗂�
			m_pMainScreen->SetStateChange(MainScreen::State::Pause);
		}
		else
		{
			// �L�����Z��SE���Đ�
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);

			// Normal�X�e�[�g�Z�b�g
			SetStateChange(State::Normal);
			
			// ���C���X�N���[����Normal�X�e�[�g�ɕύX���邱�Ƃ𑗂�
			m_pMainScreen->SetStateChange(MainScreen::State::Normal);
		}
	}

	// �X�e�[�g�}�V���̍X�V����
	m_pStateMachine.Update();
}

void Pause::Draw()
{
	// �X�e�[�g�}�V���̕`�揈��
	m_pStateMachine.Draw();
}

void Pause::PauseEnter()
{
	// ������
	m_isSoundSetting = false;
}

void Pause::PauseUpdate()
{
	// �V�[���ύX���Ȃ�΂����ŏ������I������
	if (m_isScreenChange)return;

	// �|�[�Y�I������
	PauseSelect();

	// �I�����ڌ��菈��
	SelectDetermination();

	// �T�E���h�ݒ�X�V����
	UpdateSoundSetting();
}

void Pause::PauseDraw()
{
	// �|�[�Y�E�B���h�E�`�揈��
	PauseDrawWindow();

	// �|�[�Y�I���O���t�B�b�N�`�揈��
	PauseDrawSelectGraph();

	// �T�E���h�ݒ�`�揈��
	DrawSoundSetting();
}

void Pause::PauseExit()
{
	// �I��ԍ��̏�����
	m_selectNum = static_cast<int>(SelectedItem::BackToGame);
}

void Pause::SetStateChange(const State& state)
{
	// �X�e�[�g���
	m_state = state;

	// �X�e�[�g�Z�b�g
	m_pStateMachine.SetState(m_state);
}

void Pause::LoadHandle()
{
	// ���\�[�X�̓ǂݍ���
	SetUseASyncLoadFlag(true);	// �񓯊��ǂݍ��݂��s������

	// m_graph�̗v�f��kGraphicInfo�̕��A���₷
	m_graph.resize(static_cast<int>(kGraphicInfo.size()));

	// �O���t�B�b�N����kGraphicInfo�̕��A�������
	for (int i = 0; i < static_cast<int>(kGraphicInfo.size()); i++)
	{
		// �O���t�B�b�N�����
		Load_Lib::DivGraphInfo graphInfo = Load_Lib::LoadDivGraph_EvoLib(kGraphicInfo[i].filePath, kGraphicInfo[i].div_x, kGraphicInfo[i].div_y);

		// �O���t�B�b�N���
		m_graph[i].handle = graphInfo.handle;

		// ���W���
		m_graph[i].pos = kGraphicInfo[i].pos;
	}

	// ���\�[�X�̓ǂݍ���
	SetUseASyncLoadFlag(false);	// �f�t�H���g�ɖ߂��Ă���
}

void Pause::InitStateMachine()
{
	// �X�e�[�g�}�V���̏������AEntry
	auto dummy = []() {};

	// �ʏ폈�����X�e�[�g�}�V���ɒǉ�
	{
		auto normalEnter = dummy;
		auto normalUpdate = dummy;
		auto normalDraw = dummy;
		auto normalExit = dummy;

		// �������Z�b�g����
		m_pStateMachine.AddState(State::Normal, normalEnter, normalUpdate, normalDraw, normalExit);
	}

	// �|�[�Y�������X�e�[�g�}�V���ɒǉ�
	{
		auto pauseEnter = [this]() {PauseEnter(); };
		auto pauseUpdate = [this]() {PauseUpdate(); };
		auto pauseDraw = [this]() {PauseDraw(); };
		auto pauseExit = [this]() {PauseExit(); };

		// �������Z�b�g����
		m_pStateMachine.AddState(State::Pause, pauseEnter, pauseUpdate, pauseDraw, pauseExit);
	}

	// �X�e�[�g�Z�b�g
	SetStateChange(State::Normal);
}

void Pause::PauseSelect()
{
	// �T�E���h�ݒ�t���O��true�̏ꍇ�A�������I������
	if (m_isSoundSetting)
	{
		return;
	}

	// �I���̈ړ��C���^�[�o������
	{
		// �Z���N�g�C���^�[�o���̒l�����炷
		m_selectInterval--;

		// �Z���N�g�C���^�[�o���̒l��0�ȉ��̏ꍇ�A0��������
		if (m_selectInterval <= 0)
		{
			m_selectInterval = 0;
		}
	}

	// �C���^�[�o����0�ȏ�Ȃ�΂����ŏ������I������
	if (m_selectInterval != 0)return;

	if (Pad::IsPress(PAD_INPUT_DOWN))
	{
		// �l�����[�v������
		m_selectNum = Calculation_Lib::SelectLoopNumber(0, static_cast<int>(SelectedItem::SelectedItemNum), m_selectNum);

		// �I��SE���Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Select)]);

		// m_selectInterval��kIntervalMaximumValue�̒l�������邱�Ƃɂ���āAm_selectInterval�̒l��0�ɂȂ�܂�m_selectNumber��ύX�ł��Ȃ�����
		m_selectInterval = kIntervalMaximumValue;
	}
	else if (Pad::IsPress(PAD_INPUT_UP))
	{
		// �l���t���[�v������
		m_selectNum = Calculation_Lib::SelectLoopNumber(0, static_cast<int>(SelectedItem::SelectedItemNum), m_selectNum, true);

		// �I��SE���Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Select)]);

		// m_selectInterval��kIntervalMaximumValue�̒l�������邱�Ƃɂ���āAm_selectInterval�̒l��0�ɂȂ�܂�m_selectNumber��ύX�ł��Ȃ�����
		m_selectInterval = kIntervalMaximumValue;
	}
}

void Pause::PauseDrawWindow()
{

	// �|�[�Y�E�B���h�E�Z���t�@��(����)
	SetDrawBlendMode(DX_BLENDMODE_MULA, 100);

	// ��ʂ�S�̓I�ɈÂ�����
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight,
		0x000000, true);

	// �`��u�����h���[�h���m�[�u�����h�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	// �|�[�Y�t���[����`��
	DrawRotaGraphF(
		m_graph[static_cast<int>(GraphName::Frame)].pos.x, 
		m_graph[static_cast<int>(GraphName::Frame)].pos.y,
		1.0, 0.0, m_graph[static_cast<int>(GraphName::Frame)].handle[0], true);
}

void Pause::PauseDrawSelectGraph()
{
	// �T�C���J�[�u
	{
		// �T�C���t���[���̒l�𑝂₷
		m_sinFrame++;

		// �T�C���t���[���̒l��kSinMaxFrameValue�̒l�𒴂����ꍇ�A0��������
		if (m_sinFrame >= kSinMaxFrameValue)
		{
			m_sinFrame = 0;
		}
	}


	// �I���e�L�X�g�̃O���t�B�b�N�̐��A�������J��Ԃ�
	for (int i = 0; i < static_cast<int>(m_graph[static_cast<int>(GraphName::SerectText)].handle.size()); i++)
	{
		// �I��ԍ���i�������ꍇ�Acontinue���s��
		if (m_selectNum == i)
		{
			continue;
		}

	
		// �I���e�L�X�g��`�悷��
		DrawGraphF(
			m_graph[static_cast<int>(GraphName::SerectText)].pos.x,
			m_graph[static_cast<int>(GraphName::SerectText)].pos.y + (kPauseSelectGraphDistance * i),
			m_graph[static_cast<int>(GraphName::SerectText)].handle[i], true);
	}

	// SetDrawBright���g�p���āA�F��ύX����
	SetDrawBright(static_cast<int>(kSelectColor.r), static_cast<int>(kSelectColor.g), static_cast<int>(kSelectColor.b));

	// �T�C���J�[�u�̒l�����߂�
	const int sinValue = Calculation_Lib::SineCurve(m_sinFrame, kSinMaxFrameValue, kSinMaxValue);

	// �I���e�L�X�g��`�悷��
	DrawGraphF(
		m_graph[static_cast<int>(GraphName::SerectText)].pos.x, 
		(m_graph[static_cast<int>(GraphName::SerectText)].pos.y + (kPauseSelectGraphDistance * m_selectNum) + sinValue), 
		m_graph[static_cast<int>(GraphName::SerectText)].handle[m_selectNum], true);

	// SetDrawBright���g�p���āA�F�����Ƃɖ߂�
	SetDrawBright(255, 255, 255);


	// ����`�悷��
	DrawGraphF(
		m_graph[static_cast<int>(GraphName::Arrow)].pos.x, 
		(m_graph[static_cast<int>(GraphName::Arrow)].pos.y + (kPauseSelectGraphDistance * m_selectNum)), 
		m_graph[static_cast<int>(GraphName::Arrow)].handle[0], true);

}

void Pause::SelectDetermination()
{
	// ����{�^�����T�E���h�ݒ�t���O��true�̏ꍇ�A�������I������
	if (!Pad::IsPress(PAD_INPUT_1) || m_isSoundSetting == true)
	{
		return;
	}


	// ����SE���Đ�����
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);

	// �I��ԍ�����I�����ڂ𒲂ׂ�
	const SelectedItem selectedItem = SelectedItem(m_selectNum);


	// �I�����ڂɂ���ď�����ύX����
	switch (selectedItem)
	{
	case SelectedItem::BackToGame:

		// Normal�X�e�[�g�Z�b�g
		SetStateChange(State::Normal);

		// ���C���X�N���[����Normal�X�e�[�g���𑗂�
		m_pMainScreen->SetStateChange(MainScreen::State::Normal);

		break;
	case SelectedItem::RetryFromBeginning:

		// ���C���X�N���[���փ��C���V�[���ɕύX������𑗂�
		m_pMainScreen->ChangeScene(SceneMain::ChangeScene::Main);

		// �V�[���ύX�t���O��true�ɂ���
		m_isScreenChange = true;

		break;
	case SelectedItem::BackSelectScene:

		// ���C���X�N���[���փV�[���Z���N�g�ɕύX������𑗂�
		m_pMainScreen->ChangeScene(SceneMain::ChangeScene::CharSelect);

		// �V�[���ύX�t���O��true�ɂ���
		m_isScreenChange = true;

		break;
	case SelectedItem::SoundSetting:

		// ���ʐݒ�t���O��true�ɂ���
		m_isSoundSetting = true;

		break;

	case SelectedItem::BackTitleScene:

		// ���C���X�N���[���փ^�C�g���V�[���ɕύX������𑗂�
		m_pMainScreen->ChangeScene(SceneMain::ChangeScene::Title);

		// �V�[���ύX�t���O��true�ɂ���
		m_isScreenChange = true;

		break;
	default:
		break;
	}
}

void Pause::UpdateSoundSetting()
{
	// ���ʐݒ蒆�łȂ���΁A�������s��Ȃ�
	if (!m_isSoundSetting)return;

	// �T�E���h�ݒ�����
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		// �L�����Z��SE���Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);

		// �T�E���h�ݒ�t���O��false�ɂ���
		m_isSoundSetting = false;
	}

	// �X�V
	SoundSetting::getInstance().Update();
}

void Pause::DrawSoundSetting()
{
	// ���ʐݒ蒆�łȂ���Ώ������s��Ȃ�
	if (!m_isSoundSetting)return;

	// �T�E���h�ݒ��ʂ�`�悷��
	SoundSetting::getInstance().Draw();
}
