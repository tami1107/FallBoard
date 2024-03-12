#include "ResultBackground.h"
#include "Game.h"
#include "EvoLib.h"
#include "GameData.h"
#include "Field.h"
#include "ResultCharacter.h"

namespace
{
	// �O���t�B�b�N�̃t�@�C���p�X
	const std::vector<Load_Lib::GraphicInfo>kGraphicInfo
	{
		// �w�i
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Result/backGround.png",

			// ���W
			Vec2(0 ,0),

			// �O���t�B�b�N�X�P�[��
			0.0f,

			// �O���t�B�b�N�̕�����
			{1,1},
		},

		// ���U���g�e�L�X�g
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Result/resultText.png",

			// ���W
			Vec2(Game::kScreenCenterPos.x, 180.0f),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����
			{1,1},
		},

		// �����L���O
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Result/rank.png",

			// ���W
			Vec2(-5.0f,10.0f),

			// �O���t�B�b�N�X�P�[��
			5.0f,

			// �O���t�B�b�N�̕�����
			{4,1},
		},

		// �N���E��
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Result/crown.png",

			// ���W
			Vec2(Game::kScreenCenterPos.x ,Game::kScreenCenterPos.y + 6.0f),

			// �O���t�B�b�N�X�P�[��
			0.8f,

			// �O���t�B�b�N�̕�����
			{1,1},
		},

		// ���̃V�[���̐����O���t�B�b�N
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/Result/nextSceneText.png",

			// ���W
			Vec2(Game::kScreenCenterPos.x , 800.0f),

			// �O���t�B�b�N�X�P�[��
			1.0f,

			// �O���t�B�b�N�̕�����
			{1,1},
		},
	};

	// �O���t�B�b�N��
	enum class GraphName
	{
		BackGround,		// �w�i
		ResultText,		// ���U���g�e�L�X�g
		Rank,			// �����L���O
		Crown,			// �N���E��
		NextSceneText,	// ���̃V�[���ւ̐����e�L�X�g
	};

	
}


// �w�i
namespace Background
{
	// �ړ��X�s�[�h
	constexpr float kMoveSpeed = 1.0f;
}

// �G�t�F�N�g
namespace Effect
{
	// �G�t�F�N�g�̌�
	constexpr int kCount = 255;

	// �����_���l
	constexpr int kRandValue = 20;
}

// ���U���g�e�L�X�g
namespace ResultText
{
	// �T�C���J�[�u�t���[��
	constexpr int kSinMaxFrameValue = 150;

	// �T�C���J�[�u�̑傫��
	constexpr int kSinMaxValue = 5;
}

// �N���E��
namespace Crown
{
	// �T�C���J�[�u�t���[��
	constexpr int kSinMaxFrameValue = 100;

	// �T�C���J�[�u�̑傫��
	constexpr int kSinMaxValue = 20;
}

// ���̃V�[���ւ̐����e�L�X�g
namespace NextSceneText
{
	// �t���b�V���t���[��
	constexpr int kFlashingMaxFrameValue = 100;

	// �t���b�V���̑傫��
	constexpr int kFlashingSinMaxValue = 255;
}

ResultBackground::ResultBackground():
	// �O���t�B�b�N�֘A��
	m_graph(),
	// ���o�֘A��
	m_backgroundMoveValue(),
	m_resultTextSinFrame(),
	m_crownSinFrame(),
	m_flashingFrame(),
	m_effect(),
	// �N���X�|�C���^�֘A��
	m_pField(std::make_shared<Field>()),
	m_pResultCharacter()
{
}

ResultBackground::~ResultBackground()
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

void ResultBackground::Init()
{
	// �O���t�B�b�N�̃��[�h
	LoadHandle();

	// �t�B�[���h�n���h���̃��[�h
	m_pField->LoadHandle(false);

	// �t�B�[���h����������
	m_pField->Init(false);

	// �l�̏�����
	m_backgroundMoveValue = 0.0f;
	m_resultTextSinFrame = 0;
	m_crownSinFrame = 0;
	m_flashingFrame = 0;

	// �G�t�F�N�g����������
	InitEffect();
}

void ResultBackground::Update()
{
	// �G�t�F�N�g�X�V����
	UpdateEffect();
}

void ResultBackground::Draw()
{
	// �w�i�`�揈��
	DrawBackground();

	// �Ֆʕ`�揈��
	m_pField->Draw();

	// ���U���g�e�L�X�g�`�揈��
	DrawResultText();
}

void ResultBackground::LoadHandle()
{
	// �O���t�B�b�N�ϐ���kGraphicInfo�̐��A�v�f�𑝂₷
	m_graph.resize(static_cast<int>(kGraphicInfo.size()));

	// �ϐ��ɃO���t�B�b�N��������
	for (int i = 0; i < static_cast<int>(kGraphicInfo.size()); i++)
	{
		// �O���t�B�b�N�����
		Load_Lib::DivGraphInfo graphInfo = 
			Load_Lib::LoadDivGraph_EvoLib(kGraphicInfo[i].filePath, kGraphicInfo[i].div.x, kGraphicInfo[i].div.y);

		// �O���t�B�b�N���
		m_graph[i].handle = graphInfo.handle;

		// ���W���
		m_graph[i].pos = kGraphicInfo[i].pos;
	}
}

void ResultBackground::InitEffect()
{
	// �G�t�F�N�g�̗v�f��kEffectNum���A���₷
	m_effect.resize(Effect::kCount);

	// ���ׂẴG�t�F�N�g�ɒl��������
	for (auto& effect : m_effect)
	{
		effect.pos.x = static_cast<float>(GetRand(Game::kScreenWidth));
		effect.pos.y = static_cast<float>(-GetRand(Game::kScreenHeight));

		effect.vec.x = 0.0f;
		effect.vec.y = static_cast<float>(GetRand(Effect::kRandValue) + Effect::kRandValue);
		effect.vec.y /= 10.0f;
	}
}

void ResultBackground::UpdateEffect()
{
	// �G�t�F�N�g�𓮂���
	for (auto& effect : m_effect)
	{
		// �ړ��ʕ��A���W�����Z����
		effect.pos += effect.vec;

		// �G�t�F�N�gY���W����ʊO�܂Ői�񂾏ꍇ�A���������s��
		if (effect.pos.y > Game::kScreenHeight)
		{
			effect.pos.x = static_cast<float>(GetRand(Game::kScreenWidth));
			effect.pos.y = 0.0f;

			effect.vec.x = 0.0f;
			effect.vec.y = static_cast<float>(GetRand(Effect::kRandValue) + Effect::kRandValue);
			effect.vec.y /= 10.0f;
		}
	}
}

void ResultBackground::DrawBackground()
{
	// �����w�i��`�悷��
	m_backgroundMoveValue =
		Draw_Lib::SimpleLoopBackground(
			m_graph[static_cast<int>(GraphName::BackGround)].handle[0],
			m_backgroundMoveValue,
			Background::kMoveSpeed,
			Vec2(Game::kScreenWidth, Game::kScreenHeight),
			true,
			Draw_Lib::LoopDirection::Go_Top);
}

void ResultBackground::DrawResultText()
{
	// �T�C���J�[�u
	{
		// �T�C���t���[�������Z����
		m_resultTextSinFrame++;

		// �T�C���t���[����kSinMaxFrameValue�̒l�𒴂����ꍇ�A0��������
		if (m_resultTextSinFrame >= ResultText::kSinMaxFrameValue)
		{
			m_resultTextSinFrame = 0;
		}
	}

	// �T�C���l�����߂�
	const int sinValue = Calculation_Lib::SineCurve(m_resultTextSinFrame, ResultText::kSinMaxFrameValue, ResultText::kSinMaxValue);

	// ���U���g�e�L�X�g�`��
	DrawRotaGraphF(
		m_graph[static_cast<int>(GraphName::ResultText)].pos.x,
		m_graph[static_cast<int>(GraphName::ResultText)].pos.y + sinValue,
		static_cast<double>(kGraphicInfo[static_cast<int>(GraphName::ResultText)].scale),
		0.0,
		m_graph[static_cast<int>(GraphName::ResultText)].handle[0],
		true);
}

void ResultBackground::DrawEffect()
{
	// �G�t�F�N�g��`��
	for (auto& effect : m_effect)
	{
		// ���������߂�
		const float rate = (effect.vec.length() / 4.0f);

		// �G�t�F�N�g��`�悷��
		DrawCircleAA(
			effect.pos.x,
			effect.pos.y,
			4.0f * rate,
			32,
			GetColor(
				static_cast <int>(255.0f * rate),
				static_cast <int>(255.0f * rate),
				static_cast <int>(32 * rate)),
			true);
	}
}

void ResultBackground::DrawRanking()
{
	
	// ���f���̍��W���擾
	const std::vector<VECTOR>modelPos = m_pResultCharacter->GetModelPos();

	// �����N������ϐ�
	int rank = 0;

	// �����L���O�O���t�B�b�N��`�悷��
	for (int i = 0; i < static_cast<int>(modelPos.size()); i++)
	{
		// �L�����N�^�[�̃����N���擾
		rank = GameData::getInstance().GetCharRank(i);

		// �R�c��ԏ�Ƀ����L���O�摜��`��
		DrawBillboard3D(
			VGet(
				modelPos[i].x + m_graph[static_cast<int>(GraphName::Rank)].pos.x,
				modelPos[i].y + m_graph[static_cast<int>(GraphName::Rank)].pos.y,
				modelPos[i].z),
			0.5f, 
			0.5f, 
			kGraphicInfo[static_cast<int>(GraphName::Rank)].scale,
			0.0f, 
			m_graph[static_cast<int>(GraphName::Rank)].handle[rank - 1],
			true);
	}
}

void ResultBackground::DrawCrown()
{

	// �T�C���J�[�u
	{
		// �T�C���t���[�������Z����
		m_crownSinFrame++;

		// �T�C���t���[����kSinMaxFrameValue�𒴂����ꍇ�A0��������
		if (m_crownSinFrame >= Crown::kSinMaxFrameValue)
		{
			m_crownSinFrame = 0;
		}
	}

		
	// �T�C���l�����߂�
	const int sinValue = Calculation_Lib::SineCurve(m_crownSinFrame, Crown::kSinMaxFrameValue, Crown::kSinMaxValue);

	// �v���C�l����������
	const int playCount = GameData::getInstance().GetGamePlayCount();

	// ���W�擾
	std::vector<Vec2>pos = 
		Calculation_Lib::GraphicWidthCoordinate(
			m_graph[static_cast<int>(GraphName::Crown)].handle[0],
			playCount,
			kGraphicInfo[static_cast<int>(GraphName::Crown)].scale, 
			Game::kScreenCenterPos, 
			Vec2(190.0f, 0.0f));

	// �����L���O������ϐ�
	int rank = 0;

	for (int i = 0; i < playCount; i++)
	{
		// �����N���擾
		rank = GameData::getInstance().GetCharRank(i);

		// �����N��1�ȊO�Ȃ�΁Acontinue����
		if (rank != 1)continue;

		// �N���E����`�悷��
		DrawRotaGraphF(
			pos[i].x,
			pos[i].y + sinValue,
			static_cast<double>(kGraphicInfo[static_cast<int>(GraphName::Crown)].scale),
			0.0, 
			m_graph[static_cast<int>(GraphName::Crown)].handle[0], true);
	}
}

void ResultBackground::DrawNextSceneText()
{
	// �_�Ńt���[�������Z����
	m_flashingFrame++;

	// �_�Ńt���[����kFlashingMaxFrameValue�𒴂����ꍇ�A0��������
	if (m_flashingFrame >= NextSceneText::kFlashingMaxFrameValue)
	{
		m_flashingFrame = 0;
	}

	// �����x�����߂�
	const int alphaValue = Calculation_Lib::SineCurve(m_flashingFrame, NextSceneText::kFlashingMaxFrameValue, NextSceneText::kFlashingSinMaxValue, true);

	// �����x��ݒ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);

	// �O���t�B�b�N��`�悷��
	DrawRotaGraphF(
		kGraphicInfo[static_cast<int>(GraphName::NextSceneText)].pos.x,
		kGraphicInfo[static_cast<int>(GraphName::NextSceneText)].pos.y,
		static_cast<double>(kGraphicInfo[static_cast<int>(GraphName::NextSceneText)].scale),
		0.0,
		m_graph[static_cast<int>(GraphName::NextSceneText)].handle[0],
		true);

	// �����x�����Ƃɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}