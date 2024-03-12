#include "MainBackground.h"
#include <DxLib.h>
#include "Game.h"
#include "EvoLib.h"
#include "GameData.h"

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

	// �O���t�B�b�N���
	const std::vector<GraphicInfo>kGraphicInfo =
	{
		{
			"Data/Graphic/BackGround/BackGround_0.png",

			// ���W
			Vec2(0.0f,0.0f),

			1,2,
		},

		{
			"Data/Graphic/BackGround/BackGround_1.png",

			// ���W
			Vec2(0.0f,0.0f),

			1,2,
		},

		{
			"Data/Graphic/BackGround/BackGround_2.png",

			// ���W
			Vec2(0.0f,0.0f),

			1,2,
		},
	};


	// �O���t�B�b�N�^�C�v
	enum class GraphType
	{
		Stop,	// ��~
		Move	// �ړ�
	};


	// �摜�̈ړ��X�s�[�h
	constexpr float kBackgroundMoveSpeed = 1.0f;
}

MainBackground::MainBackground():
	// �O���t�B�b�N�֘A��
	m_graph(),
	m_level(),
	m_moveValue()
{
}

MainBackground::~MainBackground()
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

void MainBackground::Init()
{
	// �ړ��ʂ̏�����
	m_moveValue = 0.0f;


	// ��Փx���擾
	m_level = GameData::getInstance().GetLevel();
}

void MainBackground::Update()
{
}

void MainBackground::Draw(bool isPause)
{

	// �w�i�`��
	Draw_Lib::DrawSimpleBackground(m_graph[static_cast<int>(m_level)].handle[static_cast<int>(GraphType::Stop)], Vec2(Game::kScreenWidth, Game::kScreenHeight));


	// �|�[�Y���Ȃ�΁A�ړ��ʂ�0�ɂ��Ĕw�i��`�悵�A�������I����
	if (isPause)
	{
		// �w�i�`��
		m_moveValue = Draw_Lib::SimpleLoopBackground(m_graph[static_cast<int>(m_level)].handle[static_cast<int>(GraphType::Move)], m_moveValue, 0.0f,
			Vec2(Game::kScreenWidth, Game::kScreenHeight),true);

		// �����ŏ������I������
		return;
	}

	// �����w�i�`��
	m_moveValue = Draw_Lib::SimpleLoopBackground(m_graph[static_cast<int>(m_level)].handle[static_cast<int>(GraphType::Move)], m_moveValue, kBackgroundMoveSpeed,
		Vec2(Game::kScreenWidth, Game::kScreenHeight),true);
	
}

void MainBackground::LoadHandle()
{
	// ���\�[�X�̓ǂݍ���
	SetUseASyncLoadFlag(true);	// �񓯊��ǂݍ��݂��s������


	// �v�f�𑝂₷
	m_graph.resize(static_cast<int>(kGraphicInfo.size()));

	// �O���t�B�b�N�̐��A�������J��Ԃ��O���t�B�b�N��}������
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