#include "SelectScreen.h"
#include <DxLib.h>
#include "Game.h"
#include <array>
#include "EvoLib.h"
#include "Pad.h"
#include "GameData.h"
#include "Sound.h"


namespace
{
	// �O���t�B�b�N��
	enum class GraphicName
	{
		Frame,				// �g
		Title_1,			// �I���^�C�g���@
		Title_2,			// �I���^�C�g���A
		PlayerCount,		// �v���C���[�l��
		SelectPlayerCount,	// �I���v���C���[�l��
		Level,				// ��Փx
		SelectLevel,		// ��Փx�I��
		Operation_1,		// ��������@
		Operation_2,		// ��������A
		Background,			// �w�i
		PadCountDescription,// �p�b�h���̐���
		LevelDescription,	// ��Փx����
	};

	// �O���t�B�b�N���m�̕�
	const float kGraphicToEachOtherWidth = (Game::kScreenCenterPos.x * 2);

	// �I���^�C�g���O���t�B�b�N��Y���W
	constexpr float kSelectTitleGraph_PosY = -340.0f;

	// �l���I���O���t�B�b�N�̍��W
	const Vec2 kPlayerCountGraph = Vec2(-10.0f, 10.0f);

	// ��Փx�O���t�B�b�N��Y���W
	constexpr float kLevelGraph_PosY = -70.0f;

	// ��������O���t�B�b�N��Y���W
	constexpr float kOperationDescriptionGraph_PosY = 270.0f;

	// �p�b�h����ѓ�Փx�����O���t�B�b�N��Y���W
	constexpr float kDescriptionGraph_PosY = 100.0f;


	// �O���t�B�b�N�̃t�@�C���p�X
	const std::vector<Load_Lib::GraphicInfo>kGraphicInfo
	{
		// �g
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Select/frame.png",

			// ���W(X,Y)
			Vec2(
				Game::kScreenCenterPos.x,
				Game::kScreenCenterPos.y
				),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,1),
		},

		// �I���^�C�g���@
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Select/selectTitle_1.png",

			// ���W(X,Y)
			Vec2(
				Game::kScreenCenterPos.x,
				Game::kScreenCenterPos.y + kSelectTitleGraph_PosY
				),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,1),
		},

		// �I���^�C�g���A
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Select/selectTitle_2.png",

			// ���W(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kGraphicToEachOtherWidth,
				Game::kScreenCenterPos.y + kSelectTitleGraph_PosY
				),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,1),
		
		},

		// �v���C���[�l��
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Select/playerCount.png",

			// ���W(X,Y)
			Vec2(	
				Game::kScreenCenterPos.x + kPlayerCountGraph.x,
				Game::kScreenCenterPos.y + kPlayerCountGraph.y
				),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,4),
		},

		// �v���C���[�l���I��
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Select/playerCountSelect.png",

			// ���W(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kPlayerCountGraph.x,
				Game::kScreenCenterPos.y + kPlayerCountGraph.y
				),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,4),
		},

		// ��Փx
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Select/level.png",

			// ���W(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kGraphicToEachOtherWidth,
				Game::kScreenCenterPos.y + kLevelGraph_PosY
				),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,3),
		},

		// ��Փx�I��
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Select/levelSelect.png",

			// ���W(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kGraphicToEachOtherWidth,
				Game::kScreenCenterPos.y + kLevelGraph_PosY
				),

			// �O���t�B�b�N�X�P�[��
			1.1f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,3),
		},

		// ��������@
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Select/operation_1.png",

			// ���W(X,Y)
			Vec2(
				Game::kScreenCenterPos.x,
				Game::kScreenCenterPos.y + kOperationDescriptionGraph_PosY
				),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,1),
		},

		// ��������A
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Select/operation_2.png",

			// ���W(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kGraphicToEachOtherWidth,
				Game::kScreenCenterPos.y + kOperationDescriptionGraph_PosY
				),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,1),
		},

		// �w�i
		{
			// �w�i
			"Data/Graphic/Select/background.png",

			// ���W(X,Y)
			Vec2(0,0),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,1),
		},

		// �p�b�h���̐���
		{
			// �e�L�X�g
			"Data/Graphic/Select/padCountDescription.png",

			// ���W(X,Y)
			Vec2(
				Game::kScreenCenterPos.x,
				Game::kScreenCenterPos.y + kDescriptionGraph_PosY
				),


			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,1),
		},

		// ��Փx����
		{
			// �e�L�X�g
			"Data/Graphic/Select/levelDescription.png",

			// ���W(X,Y)
			Vec2(
				Game::kScreenCenterPos.x + kGraphicToEachOtherWidth,
				Game::kScreenCenterPos.y + kDescriptionGraph_PosY
				),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����(��,�c)
			Load_Lib::DivNum(1,3),
		},
	};

	// �w�i�ړ��X�s�[�h
	constexpr float kBackgroundMoveSpeed = 0.5f;
}

namespace
{
	// �O���t�B�b�N�̈ړ��X�s�[�h
	constexpr float kGraphMoveSpeed = 60.0f;

	// ���ڑI�����̈ړ��C���^�[�o���̍ő�l
	constexpr int kIntervalMaximumValue = 15;

	// �v���C���[�ő�l��
	constexpr int kSelectMaxNum = 4;

	// �v���C���[�l���O���t�B�b�N�̕�
	const Vec2 kPlayerCountGraphDistance = Vec2(25.0f, 0.0f);

	// ��Փx�I���̍ő�l
	constexpr int kLevelMaxNum = 3;

	// ��Փx�I���O���t�B�b�N�̕�
	const Vec2 kLevelGraphDistance = Vec2(80.0f, 0.0f);

	// �T�C���J�[�u�t���[��
	constexpr int kSinMaxFrameValue = 150;

	// �T�C���J�[�u�̑傫��
	constexpr int kSinMaxValue = 10;
}

namespace
{
	// �T�E���h�t�@�C����
	std::vector<std::string> kSoundFileName =
	{
		"selectBGM",
		"determination",
		"choose",
		"cancel",
	};

	// �T�E���h��
	enum class SoundName
	{
		BGM,			// BGM
		Determination,	// ���艹
		Selection,		// �I����
		Cancel,			// �L�����Z����
	};
}

SelectScreen::SelectScreen():
	// �O���t�B�b�N�֘A��
	m_graph(),
	m_basePos(),
	m_targetPos(),
	m_moveValue(),
	// �Z���N�g�֘A��
	m_playCountSelectNum(),
	m_levelSelectNum(),
	m_selectInterval(),
	// ���o�֘A��
	m_sinFrame(),
	// �V�[���֘A��
	m_changeScene(),
	// �X�e�[�g�֘A��
	m_state(),
	// �N���X�|�C���^��
	m_pSceneCharSelect(nullptr),
	m_pStateMachine()
{
}

SelectScreen::~SelectScreen()
{
	// �g�p�����O���t�B�b�N�̍폜
	for (int i = 0; i < static_cast<int>(m_graph.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_graph[i].handle.size()); j++)
		{
			DeleteGraph(m_graph[i].handle[j]);
		}
	}
}

void SelectScreen::Init()
{
	// �ϐ��̏�����
	{
		m_playCountSelectNum = 0;
		m_levelSelectNum = 0;
		m_selectInterval = 0;
		m_basePos = Vec2(0.0f, 0.0f);
		m_targetPos = Vec2(0.0f, 0.0f);
		m_moveValue = 0.0f;
		m_sinFrame = 0;
	}
	

	// �X�e�[�g�}�V���̏����ݒ菈��
	InitStateMachine();

	// �O���t�B�b�N�̃��[�h����
	LoadHandle();

	

	// �T�E���h�̃��[�h
	Sound::getInstance().Load(kSoundFileName);

	// BGM���Đ�����
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::BGM)]);
}

void SelectScreen::Update()
{
	// �X�e�[�g�}�V���̍X�V����
	m_pStateMachine.Update();

	// �I���̃C���^�[�o������
	SelectInterval();

	// �O���t�B�b�N�̈ړ�����
	MoveGraph();
}

void SelectScreen::Draw()
{
	// �X�e�[�g�}�V���̍X�V����
	m_pStateMachine.Draw();

	// �w�i�`�揈��
	DrawBackGround();
}

void SelectScreen::PlayCountSelectEnter()
{
	// �^�[�Q�b�g���W����
	m_targetPos = Vec2(0.0f, 0.0f);
}

void SelectScreen::PlayCountSelectUpdate()
{
	// �v���C���[�l���I��
	PlayCountSelect();


	// PAD��1�{�^�����������Ƃ��A�X�e�[�g��LevelSelect�ɕύX����
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		// ���艹���Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);

		// �X�e�[�g��LevelSelect�ɕύX����
		SetStateChange(State::LevelSelect);
	}

	// PAD��2�{�^�����������Ƃ��A�V�[���ύX�ϐ����^�C�g���ɐݒ肵�āA�X�e�[�g��End�ɕύX����
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		// �L�����Z�������Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);

		// �V�[���ύX�ϐ����^�C�g���ɐݒ肷��
		m_changeScene = SceneSelect::ChangeScene::Title;

		// �X�e�[�g��End�ɕύX����
		SetStateChange(State::End);
	}
}

void SelectScreen::LevelSelectEnter()
{
	// �^�[�Q�b�g���W����
	m_targetPos = Vec2(-kGraphicToEachOtherWidth, 0.0f);
}

void SelectScreen::LevelSelectUpdate()
{
	// ��Փx�I��
	LevelSelect();

	// PAD��2�{�^�����������Ƃ��A�X�e�[�g��PlayCountSelect�ɕύX����
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		// �L�����Z�������Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Cancel)]);

		// �X�e�[�g��PlayCountSelect�ɕύX����
		SetStateChange(State::PlayCountSelect);
	}

	// PAD��1�{�^�����������Ƃ��A�V�[���ύX�ϐ������C���ɐݒ肵�āA�X�e�[�g��End�ɕύX����
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		// ���艹���Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);

		// �V�[���ύX�ϐ������C���ɐݒ肷��
		m_changeScene = SceneSelect::ChangeScene::Main;

		// �X�e�[�g��End�ɕύX����
		SetStateChange(State::End);
	}
}

void SelectScreen::EndEnter()
{
	// �Q�[���f�[�^�Ƀv���C�l���Ɠ�Փx�𑗂�
	GameData::getInstance().SetCharData(m_playCountSelectNum + 1,GameData::Level(m_levelSelectNum));
}

void SelectScreen::EndUpdate()
{
	// �t�F�[�h�A�E�g
	m_pSceneCharSelect->FadeOut(m_changeScene);
}

void SelectScreen::InitStateMachine()
{

	// �X�e�[�g�}�V���̏������AEntry
	auto dummy = []() {};

	// �v���C���[�l���I���������X�e�[�g�}�V���ɒǉ�
	{
		auto Enter = [this]() {PlayCountSelectEnter(); };
		auto Update = [this]() {PlayCountSelectUpdate(); };

		m_pStateMachine.AddState(State::PlayCountSelect, Enter, Update, dummy, dummy);
	}

	// �v���C���[�l���I���������X�e�[�g�}�V���ɒǉ�
	{
		auto Enter = [this]() {LevelSelectEnter(); };
		auto Update = [this]() {LevelSelectUpdate(); };

		m_pStateMachine.AddState(State::LevelSelect, Enter, Update, dummy, dummy);
	}


	// �I���������X�e�[�g�}�V���ɒǉ�
	{
		auto Enter = [this]() {EndEnter(); };
		auto Update = [this]() {EndUpdate(); };

		m_pStateMachine.AddState(State::End, Enter, Update, dummy, dummy);
	}


	// �X�e�[�g�Z�b�g
	SetStateChange(State::PlayCountSelect);
}

void SelectScreen::LoadHandle()
{
	// �O���t�B�b�N
	SelectScreen::Graphic graphic;

	// �O���t�B�b�N�����[�h����
	for (int i = 0; i < static_cast<int>(kGraphicInfo.size()); i++)
	{
		// �������ꂽ�O���t�B�b�N�����[�h����
		std::vector<int>divHandle = Load_Lib::LoadDivGraph_EvoLib_Revision(kGraphicInfo[i].filePath, kGraphicInfo[i].div);

		// �O���t�B�b�N�̑��
		graphic.handle = divHandle;

		// ���W���
		graphic.pos = kGraphicInfo[i].pos;

		// �O���t�B�b�N�����A�v�f�𑝂₷
		m_graph.push_back(graphic);
	}
}

void SelectScreen::SelectInterval()
{
	// �I���C���^�[�o�������炷
	m_selectInterval--;

	//! �I���C���^�[�o���̒l��0�ȉ��̏ꍇ�A0�ɂ���
	if (m_selectInterval <= 0)
	{
		// �l��0�ɂ���
		m_selectInterval = 0;
	}
}

void SelectScreen::PlayCountSelect()
{
	// �C���^�[�o����0�ȏ�Ȃ�΂����ŏ������I������
	if (m_selectInterval != 0)return;

	
	if (Pad::IsPress(PAD_INPUT_RIGHT))// PAD�̉E�{�^�����������Ƃ��Am_playCountSelectNum�̒l�����炷
	{
		// �I�������Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Selection)]);

		// �l�����[�v������
		m_playCountSelectNum = Calculation_Lib::SelectLoopNumber(0, kSelectMaxNum, m_playCountSelectNum);

		// m_selectInterval��kIntervalMaximumValue�̒l�������邱�Ƃɂ���āAm_selectInterval�̒l��0�ɂȂ�܂�m_selectNumber��ύX�ł��Ȃ�����
		m_selectInterval = kIntervalMaximumValue;
	}
	else if (Pad::IsPress(PAD_INPUT_LEFT))// PAD�̍��{�^�����������Ƃ��Am_playCountSelectNum�̒l�𑝂₷
	{
		// �I�������Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Selection)]);

		// �l���t���[�v������
		m_playCountSelectNum = Calculation_Lib::SelectLoopNumber(0, kSelectMaxNum, m_playCountSelectNum, true);

		// m_selectInterval��kIntervalMaximumValue�̒l�������邱�Ƃɂ���āAm_selectInterval�̒l��0�ɂȂ�܂�m_selectNumber��ύX�ł��Ȃ�����
		m_selectInterval = kIntervalMaximumValue;
	}
}

void SelectScreen::LevelSelect()
{
	// �C���^�[�o����0�ȏ�Ȃ�΂����ŏ������I������
	if (m_selectInterval != 0)return;



	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		// �I�������Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Selection)]);

		// �l�����[�v������
		m_levelSelectNum = Calculation_Lib::SelectLoopNumber(0, kLevelMaxNum, m_levelSelectNum);

		// m_selectInterval��kIntervalMaximumValue�̒l�������邱�Ƃɂ���āAm_selectInterval�̒l��0�ɂȂ�܂�m_selectNumber��ύX�ł��Ȃ�����
		m_selectInterval = kIntervalMaximumValue;
	}
	else if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		// �I�������Đ�����
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Selection)]);

		// �l���t���[�v������
		m_levelSelectNum = Calculation_Lib::SelectLoopNumber(0, kLevelMaxNum, m_levelSelectNum, true);

		// m_selectInterval��kIntervalMaximumValue�̒l�������邱�Ƃɂ���āAm_selectInterval�̒l��0�ɂȂ�܂�m_selectNumber��ύX�ł��Ȃ�����
		m_selectInterval = kIntervalMaximumValue;
	}
}

void SelectScreen::MoveGraph()
{
	// m_basePos��m_targetPos�͈͓̔����ǂ����𒲂ׁA�͈͓��Ȃ�Βl��������
	if (Calculation_Lib::IsTargetRangeValue(m_basePos.x, m_targetPos.x, kGraphMoveSpeed))
	{
		m_basePos.x = m_targetPos.x;

		return;
	}

	// ���Z���邩�ǂ���
	bool isAdd = false;

	// �x�[�X�����^�[�Q�b�g���W�̂ق����傫�������ꍇ���Z����
	if (m_basePos.x < m_targetPos.x)
	{
		isAdd = true;
	}

	// ���Z���邩�ǂ����ŏ�����ύX����
	if (isAdd)
	{
		m_basePos.x += kGraphMoveSpeed;
	}
	else
	{
		m_basePos.x -= kGraphMoveSpeed;
	}
}

void SelectScreen::DrawBackGround()
{

	// �w�i�`��
	{
		// ���[�v����w�i��`�悷��
		m_moveValue = Draw_Lib::SimpleLoopBackground(m_graph[(int)GraphicName::Background].handle[0], m_moveValue,
			kBackgroundMoveSpeed, Vec2(Game::kScreenWidth, Game::kScreenHeight), true, Draw_Lib::LoopDirection::Go_Top);
	}

	// �g�`��
	{
		// ���W
		const Vec2 pos = Vec2(
			m_graph[(int)GraphicName::Frame].pos.x + m_basePos.x,
			m_graph[(int)GraphicName::Frame].pos.y + m_basePos.y);

		// �X�P�[��
		const float scale = kGraphicInfo[(int)GraphicName::Frame].scale;

		// �g�`��1����
		DrawRotaGraph((int)pos.x, (int)pos.y, (double)scale, 0.0, m_graph[(int)GraphicName::Frame].handle[0], true);

		// �w�i�`��2����
		DrawRotaGraph((int)(pos.x + kGraphicToEachOtherWidth), (int)pos.y, (double)scale, 0.0, m_graph[(int)GraphicName::Frame].handle[0], true);
	}

	// �^�C�g���`��
	{
		// ���W
		Vec2 pos = Vec2(
			m_graph[(int)GraphicName::Title_1].pos.x + m_basePos.x,
			m_graph[(int)GraphicName::Title_1].pos.y + m_basePos.y);

		// �X�P�[��
		float scale = kGraphicInfo[(int)GraphicName::Title_1].scale;


		// �^�C�g��1���ڂ̕`��
		{
			DrawRotaGraph((int)pos.x, (int)pos.y, (double)scale, 0.0, m_graph[(int)GraphicName::Title_1].handle[0], true);
		}
		

		// ���W�Đݒ�
		pos = Vec2(
			m_graph[(int)GraphicName::Title_2].pos.x + m_basePos.x,
			m_graph[(int)GraphicName::Title_2].pos.y + m_basePos.y);

		// �X�P�[���Đݒ�
		scale = kGraphicInfo[(int)GraphicName::Title_2].scale;

		// �^�C�g��2���ڂ̕`��
		{
			DrawRotaGraph((int)pos.x, (int)pos.y, (double)scale, 0.0, m_graph[(int)GraphicName::Title_2].handle[0], true);
		}
	}


	// �A���t�@�u�����h��������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

	// �v���C���[�l���I��
	{
		// ���W�擾
		std::vector<Vec2>pos = Calculation_Lib::GraphicWidthCoordinate(m_graph[(int)GraphicName::PlayerCount].handle[0], kSelectMaxNum,
			kGraphicInfo[(int)GraphicName::PlayerCount].scale, m_graph[(int)GraphicName::PlayerCount].pos, kPlayerCountGraphDistance);

		for (int i = 0; i < kSelectMaxNum; i++)
		{
			if (i == m_playCountSelectNum)
			{
				continue;
			}


			// �l���O���t�B�b�N�̕`��
			DrawRotaGraphF(
				pos[i].x + m_basePos.x, 
				pos[i].y + m_basePos.y, 
				kGraphicInfo[(int)GraphicName::PlayerCount].scale, 0.0, m_graph[(int)GraphicName::PlayerCount].handle[i], true);
		}
	}


	// �����I��
	{
		// ���W�擾
		std::vector<Vec2>pos = Calculation_Lib::GraphicWidthCoordinate(m_graph[(int)GraphicName::Level].handle[0], kLevelMaxNum,
			kGraphicInfo[(int)GraphicName::Level].scale, m_graph[(int)GraphicName::Level].pos, kLevelGraphDistance);

		for (int i = 0; i < kLevelMaxNum; i++)
		{
			if (i == m_levelSelectNum)
			{
				continue;
			}

			// �����̃O���t�B�b�N��`��
			DrawRotaGraphF(
				pos[i].x + m_basePos.x, 
				pos[i].y + m_basePos.y, 
				kGraphicInfo[(int)GraphicName::Level].scale, 0.0, m_graph[(int)GraphicName::Level].handle[i], true);
		}

	}

	// �`��u�����h���[�h���m�[�u�����h�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);



	// �T�C���J�[�u
	{
		m_sinFrame++;

		if (m_sinFrame >= kSinMaxFrameValue)
		{
			m_sinFrame = 0;
		}
	}
	
	// �T�C���J�[�u�̒l�����߂�
	const int sinValue = Calculation_Lib::SineCurve(m_sinFrame, kSinMaxFrameValue, kSinMaxValue);


	// �������
	{
		// �w�i�`��1����
		DrawRotaGraphF(m_graph[(int)GraphicName::Operation_1].pos.x + m_basePos.x, m_graph[(int)GraphicName::Operation_1].pos.y + m_basePos.y,
			kGraphicInfo[(int)GraphicName::Operation_1].scale, 0.0, m_graph[(int)GraphicName::Operation_1].handle[0], true);

		// �w�i�`��2����
		DrawRotaGraphF(m_graph[(int)GraphicName::Operation_2].pos.x + m_basePos.x, m_graph[(int)GraphicName::Operation_2].pos.y + m_basePos.y,
			kGraphicInfo[(int)GraphicName::Operation_2].scale, 0.0, m_graph[(int)GraphicName::Operation_2].handle[0], true);
	}

	{

		// ���W�擾
		std::vector<Vec2>pos = Calculation_Lib::GraphicWidthCoordinate(m_graph[(int)GraphicName::PlayerCount].handle[0], kSelectMaxNum,
			kGraphicInfo[(int)GraphicName::PlayerCount].scale, m_graph[(int)GraphicName::PlayerCount].pos, kPlayerCountGraphDistance);

		{
			// �l��
			DrawRotaGraphF((pos[m_playCountSelectNum].x + m_basePos.x), (pos[m_playCountSelectNum].y + m_basePos.y) + sinValue,
				kGraphicInfo[(int)GraphicName::PlayerCount].scale, 0.0, m_graph[(int)GraphicName::PlayerCount].handle[m_playCountSelectNum], true);

			// �I��l��
			DrawRotaGraphF((pos[m_playCountSelectNum].x + m_basePos.x), (pos[m_playCountSelectNum].y + m_basePos.y) + sinValue,
				kGraphicInfo[(int)GraphicName::SelectPlayerCount].scale, 0.0, m_graph[(int)GraphicName::SelectPlayerCount].handle[m_playCountSelectNum], true);
		}


		// ���W�擾
		pos = Calculation_Lib::GraphicWidthCoordinate(m_graph[(int)GraphicName::Level].handle[0], kLevelMaxNum,
			kGraphicInfo[(int)GraphicName::SelectLevel].scale, m_graph[(int)GraphicName::Level].pos, kLevelGraphDistance);

		{
			// ����
			DrawRotaGraphF((pos[m_levelSelectNum].x + m_basePos.x), (pos[m_levelSelectNum].y + m_basePos.y) + sinValue, 
				kGraphicInfo[(int)GraphicName::SelectLevel].scale, 0.0, m_graph[(int)GraphicName::Level].handle[m_levelSelectNum], true);

			DrawRotaGraphF((pos[m_levelSelectNum].x + m_basePos.x), (pos[m_levelSelectNum].y + m_basePos.y) + sinValue,
				kGraphicInfo[(int)GraphicName::SelectLevel].scale, 0.0, m_graph[(int)GraphicName::SelectLevel].handle[m_levelSelectNum], true);
		}
	}


	// �����e�L�X�g
	{
		// �v���C���[�l����1�l�ȊO�̏ꍇ�A�`��
		if (m_playCountSelectNum != 0)
		{

			DrawRotaGraphF((m_graph[(int)GraphicName::PadCountDescription].pos.x + m_basePos.x), (m_graph[(int)GraphicName::PadCountDescription].pos.y), 
				kGraphicInfo[(int)GraphicName::PadCountDescription].scale, 0.0, m_graph[(int)GraphicName::PadCountDescription].handle[0], true);
		}

		// ��Փx����
		DrawRotaGraphF((m_graph[(int)GraphicName::LevelDescription].pos.x + m_basePos.x), (m_graph[(int)GraphicName::LevelDescription].pos.y),
			kGraphicInfo[(int)GraphicName::LevelDescription].scale, 0.0, m_graph[11].handle[m_levelSelectNum], true);
	}

}

void SelectScreen::SetStateChange(const State state)
{
	// �X�e�[�g���
	m_state = state;

	// �X�e�[�g�Z�b�g
	m_pStateMachine.SetState(m_state);
}
