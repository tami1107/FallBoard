#include "SoundSetting.h"
#include "EvoLib.h"
#include "Game.h"
#include "Pad.h"
#include "Sound.h"

// �V���O���g��
SoundSetting* SoundSetting::m_instance = nullptr;

namespace
{

	// �O���t�B�b�Nl
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
	const std::vector<GraphicInfo> kGraphicInfo
	{
		// �w�i
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/SoundSetting/background.png",

			// ���W
			Vec2(Game::kScreenCenterPos.x,Game::kScreenCenterPos.y-50),

		
			// �O���t�B�b�N�̕�����
			1,1,
		},

		// �t���[��
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/SoundSetting/frame.png",

			// ���W
			Vec2(Game::kScreenCenterPos.x,Game::kScreenCenterPos.y),

			// �O���t�B�b�N�̕�����
			1,2,
		},

		// �e�L�X�g
		{
			// �O���t�B�b�N�t�@�C��
			"Data/Graphic/SoundSetting/text.png",

			// ���W
			Vec2(Game::kScreenCenterPos.x - 300.0f,Game::kScreenCenterPos.y),


			// �O���t�B�b�N�̕�����
			1,2,
		},

		// �X�s�[�J�[
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/SoundSetting/speaker.png",

			// ���W
			Vec2(Game::kScreenCenterPos.x - 120.0f,Game::kScreenCenterPos.y),

			// �O���t�B�b�N�̕�����
			2,1,
		},

		// ���ʃo�[
		{
			// �O���t�B�b�N�t�@�C���p�X
			"Data/Graphic/SoundSetting/bar.png",

			// ���W
			Vec2(Game::kScreenCenterPos.x,Game::kScreenCenterPos.y),


			// �O���t�B�b�N�̕�����
			1,2,
		},

	};

	// �O���t�B�b�N��
	enum class GraphName
	{
		Background,	// �w�i
		Frame,		// �t���[��
		Text,		// �e�L�X�g
		Speaker,	// �X�s�[�J�[
		SoundBar,	// �T�E���h�o�[
	};

	// �O���t�B�b�N��
	constexpr int kGraphDistance = 90;


	// ���ʃo�[�̕�
	constexpr int kSoundBarDistance = 85;
}

namespace
{
	// ���ʂ̍ő�
	constexpr int kSoundMaxNum = 5;

	// ���ڑI�����̈ړ��C���^�[�o���̍ő�l
	constexpr int kIntervalMaximumValue = 10;

	// �I�����ڂ̍ő吔
	constexpr int kSelectMaxNum = 2;
}

namespace
{
	// �T�E���h�t�@�C����
	const std::vector<std::string> kSoundFileName =
	{
		"determination",// ���艹
	};

	// �T�E���h��
	enum class SoundName
	{
		Determination,	// ���艹
	};
}

void SoundSetting::Init()
{
	// �n���h���̃��[�h
	LoadHandle();

	// �ϐ�������
	m_bgmValue = kSoundMaxNum;
	m_seValue = kSoundMaxNum;
	m_seValue = kSoundMaxNum;
	m_selectNum = 0;
	m_selectInterval = kIntervalMaximumValue;

	// �T�E���h���[�h
	Sound::getInstance().Load(kSoundFileName);
}

void SoundSetting::UnLoad()
{
	// �O���t�B�b�N�̍폜
	for (int i = 0; i < static_cast<int>(m_graph.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_graph[i].handle.size()); j++)
		{
			DeleteGraph(m_graph[i].handle[j]);
		}
	}

	// �T�E���h���[�h
	Sound::getInstance().UnLoad();
}

void SoundSetting::Update()
{
	// �I��
	Select();

	// ���ʒ���
	VolumeAdjustment();
}

void SoundSetting::Draw()
{
	// ��ʂ��Â�����
	{
		// �|�[�Y�E�B���h�E�Z���t�@��(����)
		SetDrawBlendMode(DX_BLENDMODE_MULA, 100);

		// �S�̓I�ɈÂ�����
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight,
			0x000000, true);

		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}


	// �w�i�`��
	{
		DrawRotaGraphF(
			kGraphicInfo[static_cast<int>(GraphName::Background)].pos.x,
			kGraphicInfo[static_cast<int>(GraphName::Background)].pos.y,
			1.0, 0.0, m_graph[static_cast<int>(GraphName::Background)].handle[0], true);
	}


	{
		if (m_selectNum == 1)
		{
			// �����x��ݒ肷��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		}
		else
		{
			// �g
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Frame)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Frame)].pos.y - kGraphDistance,
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Frame)].handle[1], true);
			}

			// SetDrawBright���g�p���āA�J���[���Z�b�g����
			SetDrawBright(255, 100, 0);
		}
	

		// �e�L�X�g
		{
			DrawRotaGraphF(
				kGraphicInfo[static_cast<int>(GraphName::Text)].pos.x,
				kGraphicInfo[static_cast<int>(GraphName::Text)].pos.y - kGraphDistance,
				1.0, 0.0, m_graph[static_cast<int>(GraphName::Text)].handle[0], true);
		}

		// �X�s�[�J�[
		{
			if (m_bgmValue != 0)
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.y - kGraphDistance, 
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Speaker)].handle[0], true);
			}
			else
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.y - kGraphDistance, 
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Speaker)].handle[1], true);
			}
		}

		// ����
		{
			for (int i = kSoundMaxNum - 1; i >= 0; --i)
			{
				if ((m_bgmValue) <= i)
				{
					DrawRotaGraphF(
						kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.x + (kSoundBarDistance * i), 
						kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.y - kGraphDistance,
						0.8, 0.0, m_graph[static_cast<int>(GraphName::SoundBar)].handle[1], true);

					continue;
				}
			}

			for (int i = 0; i < m_bgmValue; i++)
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.x + (kSoundBarDistance * i),
					kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.y - kGraphDistance,
					0.8, 0.0, m_graph[static_cast<int>(GraphName::SoundBar)].handle[0], true);
			}
		}

		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// SetDrawBright���g�p���A�J���[�����Ƃɖ߂�
		SetDrawBright(255, 255, 255);
	}

	{
		if (m_selectNum == 0)
		{
			// �����x��ݒ�
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		}
		else
		{

			// �g
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Frame)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Frame)].pos.y + kGraphDistance,
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Frame)].handle[1], true);
			}

			// �J���[��ݒ�
			SetDrawBright(255, 100, 0);
		}

		// �e�L�X�g
		{
			DrawRotaGraphF(
				kGraphicInfo[static_cast<int>(GraphName::Text)].pos.x,
				kGraphicInfo[static_cast<int>(GraphName::Text)].pos.y + kGraphDistance,
				1.0, 0.0, m_graph[static_cast<int>(GraphName::Text)].handle[1], true);
		}

		// �X�s�[�J�[
		{
			if (m_seValue != 0)
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.y + kGraphDistance,
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Speaker)].handle[0], true);
			}
			else
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.y + kGraphDistance,
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Speaker)].handle[1], true);
			}
			
		}

		// ���ʃo�[
		{

			for (int i = kSoundMaxNum - 1; i >= 0; --i)
			{
				if ((m_seValue) <= i)
				{
					DrawRotaGraphF(
						kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.x + (kSoundBarDistance * i),
						kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.y + kGraphDistance,
						0.8, 0.0, m_graph[static_cast<int>(GraphName::SoundBar)].handle[1], true);

					continue;
				}
			}

			for (int i = 0; i < m_seValue; i++)
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.x + (kSoundBarDistance * i),
					kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.y + kGraphDistance,
					0.8, 0.0, m_graph[static_cast<int>(GraphName::SoundBar)].handle[0], true);
			}
		}

		// �`��u�����h���[�h���m�[�u�����h�ɂ���
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// SetDrawBright���g�p���A�J���[�����Ƃɖ߂�
		SetDrawBright(255, 255, 255);
	}
}

void SoundSetting::LoadHandle()
{
	// �O���t�B�b�N�ϐ��̗v�f�𑝂₷
	m_graph.resize(static_cast<int>(kGraphicInfo.size()));

	// �n���h���̃��[�h
	for (int i = 0; i < static_cast<int>(kGraphicInfo.size()); i++)
	{
		// �O���t�B�b�N�����
		Load_Lib::DivGraphInfo graphInfo = Load_Lib::LoadDivGraph_EvoLib(kGraphicInfo[i].filePath, kGraphicInfo[i].div_x, kGraphicInfo[i].div_y);

		// �O���t�B�b�N���
		m_graph[i].handle = graphInfo.handle;

		// ���W���
		m_graph[i].pos = kGraphicInfo[i].pos;
	}

}

void SoundSetting::Select()
{
	// �I���̈ړ��C���^�[�o������
	{
		m_selectInterval--;

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
		m_selectNum = Calculation_Lib::SelectLoopNumber(0, kSelectMaxNum, m_selectNum);

		// �I��SE�𗬂�
		Sound::getInstance().Play(kSoundFileName[0]);

		// m_selectInterval��kIntervalMaximumValue�̒l�������邱�Ƃɂ���āAm_selectInterval�̒l��0�ɂȂ�܂�m_selectNumber��ύX�ł��Ȃ�����
		m_selectInterval = kIntervalMaximumValue;
	}
	else if (Pad::IsPress(PAD_INPUT_UP))
	{
		// �l���t���[�v������
		m_selectNum = Calculation_Lib::SelectLoopNumber(0, kSelectMaxNum, m_selectNum, true);

		// �I��SE�𗬂�
		Sound::getInstance().Play(kSoundFileName[0]);

		// m_selectInterval��kIntervalMaximumValue�̒l�������邱�Ƃɂ���āAm_selectInterval�̒l��0�ɂȂ�܂�m_selectNumber��ύX�ł��Ȃ�����
		m_selectInterval = kIntervalMaximumValue;
	}

}

void SoundSetting::VolumeAdjustment()
{

	// �p�[�Z���g�{�����[���ϐ�
	int percentValue = 0;

	if (m_selectNum == 0)
	{
		

		if (Pad::IsTrigger(PAD_INPUT_RIGHT))
		{
			// �l�����[�v������
			m_bgmValue = Calculation_Lib::SelectLoopNumber(0, kSoundMaxNum + 1, m_bgmValue);			
		
			// �p�[�Z���g�{�����[���𒲂ׂ�
			percentValue = Convert_Lib::PercentageRate(static_cast<float>(kSoundMaxNum), static_cast<float>(m_bgmValue));

			// ���ʂ�ݒ肷��
			Sound::getInstance().SetSoundVolume(Sound::SoundTypeInfo::BGM, percentValue);

			// ���艹���Đ�
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);
		}
		else if (Pad::IsTrigger(PAD_INPUT_LEFT))
		{
			// �l���t���[�v������
			m_bgmValue = Calculation_Lib::SelectLoopNumber(0, kSoundMaxNum + 1, m_bgmValue, true);
		
			// �p�[�Z���g�{�����[���𒲂ׂ�
			percentValue = Convert_Lib::PercentageRate(static_cast<float>(kSoundMaxNum), static_cast<float>(m_bgmValue));

			// ���ʂ�ݒ肷��
			Sound::getInstance().SetSoundVolume(Sound::SoundTypeInfo::BGM, percentValue);

			// ���艹���Đ�
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);
		}
	}
	else
	{
		if (Pad::IsTrigger(PAD_INPUT_RIGHT))
		{

			// �l�����[�v������
			m_seValue = Calculation_Lib::SelectLoopNumber(0, kSoundMaxNum + 1, m_seValue);

			// �p�[�Z���g�{�����[���𒲂ׂ�
			percentValue = Convert_Lib::PercentageRate(static_cast<float>(kSoundMaxNum), static_cast<float>(m_seValue));

			// ���ʂ�ݒ肷��
			Sound::getInstance().SetSoundVolume(Sound::SoundTypeInfo::SE, percentValue);


			// ���艹���Đ�
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);
		}
		else if (Pad::IsTrigger(PAD_INPUT_LEFT))
		{
			// �l���t���[�v������
			m_seValue = Calculation_Lib::SelectLoopNumber(0, kSoundMaxNum + 1, m_seValue, true);
			
			// �p�[�Z���g�{�����[���𒲂ׂ�
			percentValue = Convert_Lib::PercentageRate(static_cast<float>(kSoundMaxNum), static_cast<float>(m_seValue));

			// ���ʂ�ݒ肷��
			Sound::getInstance().SetSoundVolume(Sound::SoundTypeInfo::SE, percentValue);


			// ���艹���Đ�
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);
		}
	}

}
