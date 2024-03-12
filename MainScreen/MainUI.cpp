#include "StateMachine.h"// ����ԏ�ɏ���
#include "MainUI.h"
#include "MainParam.h"
#include <DxLib.h>
#include "GameData.h"
#include "Game.h"
#include "EvoLib.h"
#include "Sound.h"



// �J�E���g
namespace CountInfo
{
	// �J�E���g�̃O���t�B�b�N�t�@�C���p�X
	const std::vector<const char*>kFilePath =
	{
		"Data/UI/CountDown/start.png",
		"Data/UI/CountDown/count_1.png",
		"Data/UI/CountDown/count_2.png",
		"Data/UI/CountDown/count_3.png",
	};

	// �J�E���g�O���t�B�b�N�̃f�t�H���g�T�C�Y
	constexpr float kGraphScale = 0.8f;

	// �J�E���g�O���t�B�b�N�̍ő�T�C�Y
	constexpr float kGraphMaxSize = kGraphScale * 2.0f;

	// �����x�̒l
	constexpr int kAlphaValue = 255;
}


// ���C�t
namespace LifeInfo
{
	// ���C�t�̃O���t�B�b�N�t�@�C���p�X
	const std::vector<const char*>kFilePath =
	{
		"Data/UI/Life/underLifeBar.png",
		"Data/UI/Life/topLifeBar.png",
		"Data/UI/Life/lifeFrame.png",
		"Data/UI/Life/lifeGage.png",
	};

	// �O���t�B�b�N��
	enum class GraphName
	{
		UnderLifeBar,	// ���C�t�o�[��
		TopLifeBar,		// ���C�t�o�[��
		LifeFrame,		// ���C�t�t���[��
		LifeGage		// ���C�t�Q�[�W
	};

	// �O���t�B�b�N�̃f�t�H���g�X�P�[��
	constexpr float kGraphScale = 1.0f;

	// �O���t�B�b�N��
	constexpr int kGraphDistance = 130;

	// �O���t�B�b�N�̍���
	constexpr int kGraphHeight = 80;

	// ���C�t�̈ʒu
	constexpr float kLifeGraphBasePos = -33.0f;

	// ���C�t�̃O���t�B�b�N��
	constexpr float kLifeGraphDistance = 25.0f;

	// �_�ŉ�
	constexpr int kFlashingCount = 3;

	// �_�ŃX�s�[�h
	constexpr int kFlashingSpeed = 90;

	// �U���b��
	constexpr int kVibrationFrame = kFlashingSpeed * kFlashingCount;

	// �U����
	constexpr int kVibrationWidth = 30;
}

namespace
{
	// �O���t�B�b�N
	struct GraphicInfo
	{
		// �t�@�C���p�X
		const char* filePath = "";

		// �O���t�B�b�N�̕�����
		const int div_x = 0;
		const int div_y = 0;
	};
}

// �^�O
namespace TagInfo
{
	// �O���t�B�b�N�̃t�@�C���p�X
	const std::vector<GraphicInfo>kGraphFilePath=
	{
		{
			// �^�O
			"Data/UI/Tag/playerNum.png",

			// �O���t�B�b�N�̕�����
			1,5,
		},

		{
			// �^�O�̃t���[��
			"Data/UI/Tag/playerNumFrame.png",

			// �O���t�B�b�N�̕�����
			1,5,
		},
	};

	// ����Y���W
	constexpr float kAdjustmentPos_Y = 10.0f;

	// �O���t�B�b�N�X�P�[��
	constexpr float kGraphScale = 5.0f;
}

// �t�F�C�X
namespace FaceInfo
{
	// �O���t�B�b�N�̃t�@�C���p�X
	const std::vector<GraphicInfo>kGraphFilePath =
	{
		{
			// �t�F�C�X�O���t�B�b�N
			"Data/UI/Life/face.png",

			// �O���t�B�b�N�̕�����
			2,1,
		},
	};

	// �O���t�B�b�N��
	enum class GraphName
	{
		Face	// �t�F�C�X
	};

	// �O���t�B�b�N�^�C�v��
	enum class GraphTypeName
	{
		Noramal,// �ʏ�
		Die		// ���S
	};

	// �t�F�C�X�O���t�B�b�N�T�C�Y
	constexpr float kGraphScale = 0.3f;

	// ����Y���W
	constexpr float kAdjustmentPos_Y = -90.0f;
}

// ����
namespace Operation
{
	// �����O���t�B�b�N�̃t�@�C���p�X
	const char* const kGraphFilePath = "Data/Graphic/BackGround/operationDescription.png";

	// �������W
	const Vec2 kInitPos = Vec2(300.0f, 400.0f);

	// �O���t�B�b�N�X�P�[��
	constexpr float kGraphScale = 0.8f;
}

namespace
{
	// �T�E���h�t�@�C����
	const std::vector<std::string> kSoundFileName =
	{
		"countStart",	// �J�E���g�X�^�[�g��
		"countEnd",		// �J�E���g�I����
		"mainBGM"		// ���C��BGM
	};

	// �T�E���h��
	enum class SoundName
	{
		CountStert,	// �J�E���g�X�^�[�g��
		CountEnd,	// �J�E���g�I����
		MainBGM,	// ���C��BGM
	};
}


MainUI::MainUI():
	// �J�E���g�֘A��
	m_count(),
	m_operationGraphHandle(),
	// ���C�t�֘A��
	m_life(),
	m_face(),
	// �^�O�֘A��
	m_tag(),
	// �N���X�֘A
	m_pMainParam()
{
}

MainUI::~MainUI()
{
	// �J�E���g�O���t�B�b�N�̍폜
	for (auto& handle : m_count.handle)
	{
		// �O���t�B�b�N�̍폜
		DeleteGraph(handle);
	}
	
	// ���C�t�O���t�B�b�N�̍폜
	for (auto& handle : m_life.handle)
	{
		// �O���t�B�b�N�̍폜
		DeleteGraph(handle);
	}
	
	// �^�O�O���t�B�b�N�̍폜
	for (int i = 0; i < static_cast<int>(m_tag.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_tag[i].handle.size()); j++)
		{
			// �O���t�B�b�N�̍폜
			DeleteGraph(m_tag[i].handle[j]);
		}
	}

	// �����O���t�B�b�N�̍폜
	DeleteGraph(m_operationGraphHandle);
}

void MainUI::Init()
{
	// �X�e�[�g�}�V���̏���������
	InitStateMachine();

	// ���C�t����������
	LifeInit();

	// �T�E���h���[�h����
	Sound::getInstance().Load(kSoundFileName);
}

void MainUI::Update()
{
	// �X�e�[�g�}�V���̍X�V����
	m_pStateMachine.Update();

	// ���C�t�X�V����
	LifeUpdate();
}

void MainUI::Draw()
{
	// �X�e�[�g�}�V���̕`�揈��
	m_pStateMachine.Draw();

	// ���C�t�`�揈��
	LifeDraw();
}

void MainUI::CountEnter()
{
	// �A���t�@�l�̑��
	m_count.alphaValue = CountInfo::kAlphaValue;

	// �O���t�B�b�N�X�P�[���̑��
	m_count.scaleValue = CountInfo::kGraphScale;
}

void MainUI::CountUpdate()
{
	// �t���[�����̑��
	m_count.frame = m_pMainParam->CountDown();

	// �t���[����b���ɕϊ�
	m_count.second = Convert_Lib::ConvertFrameToSeconds(m_count.frame);

	// �t���[������4,3,2�̒l�̏ꍇ�A�J�E���g�X�^�[�g��SE���Đ�
	if (m_count.frame == (FPS_Lib::FPS * 4) ||
		m_count.frame == (FPS_Lib::FPS * 3) ||
		m_count.frame == (FPS_Lib::FPS * 2))
	{
		// �J�E���g�_�E����SE�𗬂�
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::CountStert)]);
	}
	// �t���[������1�̒l�̏ꍇ�A�J�E���g�G���h��SE���Đ�
	if (m_count.frame == (FPS_Lib::FPS * 1))
	{
		// �J�E���g�_�E����SE�𗬂�
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::CountEnd)]);
	}

	// �O���t�B�b�N�̃T�C�Y�A�A���t�@�l
	{
		// �O���t�B�b�N�̌v�Z�ɗp����t���[���l
		const int graphFrame = m_count.frame % FPS_Lib::FPS;

		// �O���t�B�b�N�𓮂����t���[��
		const int graphDriveFrame = static_cast<int>(FPS_Lib::FPS * 0.5f);

		// �O���t�B�b�N�𓮂������ǂ���
		const bool isGraphDrive = (graphFrame <= graphDriveFrame);

		
		// �O���t�B�b�N�𓮂����t���O��true�̎��A������
		if (isGraphDrive)
		{
			// �A���t�@�l�̌����l
			const int alphaSubValue = CountInfo::kAlphaValue / graphDriveFrame;

			// �A���t�@�l�����炷
			m_count.alphaValue -= alphaSubValue;

			// �f�t�H���g�X�P�[���l�ƃ}�b�N�X�X�P�[���l�̍�
			const float differenceScale = CountInfo::kGraphMaxSize - CountInfo::kGraphScale;

			// �X�P�[���l�̉��Z�l
			const float scaleAddValue = differenceScale / graphDriveFrame;

			// �T�C�Y�𑝂₷
			m_count.scaleValue += scaleAddValue;
		}
		else
		{
			// �A���t�@�l�̑��
			m_count.alphaValue = CountInfo::kAlphaValue;

			// �T�C�Y�̑��
			m_count.scaleValue = CountInfo::kGraphScale;
		}
	}

	// 0�t���[���ȏ�Ȃ�Ώ������I������
	if (m_count.frame > 0)
	{
		return;
	}

	// BGM�𗬂�
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::MainBGM)]);

	// �X�e�[�g�Z�b�g
	m_pStateMachine.SetState(State::Normal);
}

void MainUI::CountDraw()
{

	// �A���t�@�u�����h��������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_count.alphaValue);

	// �b�����n���h���̐��ȉ��̏ꍇ�̂ݕ`��
	if (m_count.second < static_cast<int>(m_count.handle.size()))
	{
		DrawRotaGraphF(Game::kScreenCenterPos.x, Game::kScreenCenterPos.y,
			m_count.scaleValue, 0.0,
			m_count.handle[m_count.second], true);
	}

	// �ʏ�`��ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	// �I�v�V�����摜��`�悷��
	DrawRotaGraphF(
		Operation::kInitPos.x,
		Operation::kInitPos.y,
		static_cast<double>(Operation::kGraphScale),
		0.0f,
		m_operationGraphHandle, true);

}

void MainUI::InitStateMachine()
{
	// �X�e�[�g�}�V���̏������AEntry
	auto dummy = []() {};

	// �J�E���g�������X�e�[�g�}�V���ɒǉ�
	{
		auto countEnter = [this]() {CountEnter(); };
		auto countUpdate = [this]() {CountUpdate(); };
		auto countDraw = [this]() {CountDraw(); };
		auto countExit = dummy;

		m_pStateMachine.AddState(State::Count, countEnter, countUpdate, countDraw, countExit);
	}

	// �ʏ폈�����X�e�[�g�}�V���ɒǉ�
	{
		auto normalEnter = dummy;
		auto normalUpdate = dummy;
		auto normalDraw = dummy;
		auto normalExit = dummy;

		m_pStateMachine.AddState(State::Normal, normalEnter, normalUpdate, normalDraw, normalExit);
	}


	// �J�E���g�X�e�[�g�Z�b�g
	m_pStateMachine.SetState(State::Count);
}

void MainUI::LoadHandle()
{
	// ���\�[�X�̓ǂݍ���
	SetUseASyncLoadFlag(true);	// �񓯊��ǂݍ��݂��s������

	// �J�E���g�_�E���̃n���h�����[�h
	{
		// �n���h���̃��[�h
		for (auto& handle : CountInfo::kFilePath)
		{
			m_count.handle.push_back(LoadGraph(handle));
		}
	}

	// ���C�t�̃n���h�����[�h
	{
		// �n���h���̃��[�h
		for (auto& handle : LifeInfo::kFilePath)
		{
			m_life.handle.push_back(LoadGraph(handle));
		}
	}

	// �^�O�ϐ����^�O�̃O���t�B�b�N�����A�v�f�𑝂₷
	m_tag.resize(static_cast<int>(TagInfo::kGraphFilePath.size()));

	// �O���t�B�b�N�̃��[�h
	for (int i = 0; i < static_cast<int>(TagInfo::kGraphFilePath.size()); i++)
	{
		// �O���t�B�b�N�����
		Load_Lib::DivGraphInfo graphInfo = Load_Lib::LoadDivGraph_EvoLib(TagInfo::kGraphFilePath[i].filePath, TagInfo::kGraphFilePath[i].div_x, TagInfo::kGraphFilePath[i].div_y);

		// �O���t�B�b�N���
		m_tag[i].handle = graphInfo.handle;
	}


	// �t�F�C�X�ϐ����t�F�C�X�̃O���t�B�b�N�����A�v�f�𑝂₷
	m_face.resize(static_cast<int>(FaceInfo::kGraphFilePath.size()));

	// �O���t�B�b�N�̃��[�h
	for (int i = 0; i < static_cast<int>(FaceInfo::kGraphFilePath.size()); i++)
	{
		// �O���t�B�b�N�����
		Load_Lib::DivGraphInfo graphInfo = Load_Lib::LoadDivGraph_EvoLib(FaceInfo::kGraphFilePath[i].filePath, FaceInfo::kGraphFilePath[i].div_x, FaceInfo::kGraphFilePath[i].div_y);

		// �O���t�B�b�N���
		m_face[i].handle = graphInfo.handle;
	}

	// �����O���t�B�b�N�̃��[�h
	m_operationGraphHandle = LoadGraph(Operation::kGraphFilePath);


	// ���\�[�X�̓ǂݍ���
	SetUseASyncLoadFlag(false);	// �f�t�H���g�ɖ߂��Ă���
}

void MainUI::LifeInit()
{
	// �v���C�l���擾
	const int playCount = GameData::getInstance().GetGamePlayCount();

	// �ő�̗͂��擾
	m_life.maxLifePoint = GameData::getInstance().GetMaxLifePoint();

	// �v���C�l�����̓_�ŕϐ�������
	for (int i = 0; i < playCount; i++)
	{
		// 0�����v�f�𑝂₷
		m_life.flashingFrame.push_back(0);
		m_life.flashingCount.push_back(0);
	}

	// ���C�t�|�C���g�̔z��v�f�ǉ�
	m_life.lifePoint.resize(playCount);

	// ���C�t�O���t�B�b�N�̈ʒu
	{
		// ���C�t�̍��W�̔z��v�f�ǉ�
		m_life.graphicPos.resize(playCount);

		// ���S���W�����Ƃ����O���t�B�b�N�̍��W�����߂�
		m_life.graphicPos = Calculation_Lib::GraphicWidthCoordinate(m_life.handle[static_cast<int>(LifeInfo::GraphName::UnderLifeBar)], 
			playCount, LifeInfo::kGraphScale,Vec2(Game::kScreenCenterPos.x, LifeInfo::kGraphHeight), Vec2(LifeInfo::kGraphDistance, 0.0f));
	}
}

void MainUI::LifeUpdate()
{
	// ���C�t�̒l���擾
	for (int i = 0; i < static_cast<int> (m_life.lifePoint.size()); i++)
	{
		// ���C�t���
		m_life.lifePoint[i] = m_pMainParam->GetCharLife(i);
	}
}

void MainUI::LifeDraw()
{
	// �J���[
	VECTOR color = {};

	// ����W
	std::vector<Vec2>basePos = {};

	// ���C�t�Q�[�W���W
	Vec2 lifeGagePos = {};


	// �_�Ńt���O
	bool isFlashing = false;

	// ���C�t�ϐ����A�������J��Ԃ��A�摜��`�悷��
	for (int lifeSize = 0; lifeSize < static_cast<int> (m_life.graphicPos.size()); lifeSize++)
	{
		// �J���[�擾
		color = Convert_Lib::ColorCodeTo255(GameData::getInstance().GetPlayerColor(lifeSize));

		// �v�f
		basePos.push_back(Vec2());

		// �_�ł��Ă��邩�ǂ����𒲂ׂ�
		isFlashing = m_life.flashingFrame[lifeSize] != 0;

		// �_�ł��Ă���ꍇ�́A�O���t�B�b�N��h�炷���W�����߂�
		if (isFlashing)
		{
			basePos[lifeSize] = Calculation_Lib::VibrationValue(m_life.flashingFrame[lifeSize], LifeInfo::kVibrationFrame, LifeInfo::kVibrationWidth);
		}

		// ���W���
		basePos[lifeSize] += m_life.graphicPos[lifeSize];


		// ���C�t�o�[
		{

			// SetDrawBright���g�p���J���[�ݒ肷��
			SetDrawBright(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z));

			// �_�Ńt���O��true�̏ꍇ�A�A���t�@�u�����h���g�p����
			if (isFlashing)
			{
				// �����x��ݒ�
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			}

			// ���C�t�|�C���g��0�̏ꍇ�A�A���t�@�u�����h���g�p����
			if (m_life.lifePoint[lifeSize] == 0)
			{
				// �����x��ݒ�
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			}

			// ���C�t�o�[����`�悷��
			DrawRotaGraphF(basePos[lifeSize].x, basePos[lifeSize].y,
				LifeInfo::kGraphScale, 0.0, m_life.handle[static_cast<int>(LifeInfo::GraphName::UnderLifeBar)], true);

			// SetDrawBright���g�p���āA�J���[�ݒ�����Ƃɖ߂�
			SetDrawBright(255, 255, 255);

			// ���C�t�o�[���`�悷��
			DrawRotaGraphF(basePos[lifeSize].x, basePos[lifeSize].y,
				LifeInfo::kGraphScale, 0.0, m_life.handle[static_cast<int>(LifeInfo::GraphName::TopLifeBar)], true);

			// �`��u�����h���[�h���m�[�u�����h�ɂ���
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		// ���C�t�Q�[�W
		{
			// ���C�t�Q�[�W���W����
			lifeGagePos = basePos[lifeSize];

			// ���C�t�Q�[�W�����C�t�|�C���g���������J��Ԃ��A�`�悷��
			for (int lifePoint = 0; lifePoint < m_life.maxLifePoint; lifePoint++)
			{
				// ���C�t�Q�[�W��X���W�����߂�
				lifeGagePos.x = (basePos[lifeSize].x + LifeInfo::kLifeGraphBasePos) + (LifeInfo::kLifeGraphDistance * lifePoint);

				// SetDrawBright���g�p���āA�J���[�ݒ�����Ƃɖ߂�
				SetDrawBright(255, 255, 255);

				// ���C�t�Q�[�W�`��
				if (lifePoint < m_life.lifePoint[lifeSize])
				{
					DrawRotaGraphF(lifeGagePos.x, lifeGagePos.y,
						LifeInfo::kGraphScale, 0.0, m_life.handle[static_cast<int>(LifeInfo::GraphName::LifeGage)], true);
				}

				// �_���[�W���󂯂Ă��邩�ǂ������擾����
				if (m_pMainParam->GetIsCharDamage(lifeSize))
				{
					// �U���l�𑝂₷
					m_life.flashingFrame[lifeSize]++;

					// �U���l��kFlashingCount�𒴂����ꍇ�A�U���ϐ�������������
					if (m_life.flashingCount[lifeSize] >= LifeInfo::kFlashingCount)
					{
						// �ϐ��̏�����
						m_life.flashingFrame[lifeSize] = 0;
						m_life.flashingCount[lifeSize] = 0;

						// ���C���p������false�t���O��������
						m_pMainParam->SetIsCharDamage(lifeSize, false);
					}

					// �A���t�@�l�����߂�
					const int alphaValue = Calculation_Lib::SineCurve(m_life.flashingFrame[lifeSize], LifeInfo::kFlashingSpeed, 255, true);

					// �A���t�@�l��0�̏ꍇ�A�_�ŉ񐔕ϐ��𑝂₷
					if (alphaValue == 0)
					{
						// �_�ŉ񐔕ϐ��𑝂₷
						m_life.flashingCount[lifeSize]++;
					}

					// �_�ō��W�����߂�
					const float flashingPos_x = (basePos[lifeSize].x + LifeInfo::kLifeGraphBasePos) + (LifeInfo::kLifeGraphDistance * (m_life.lifePoint[lifeSize]));

					// �����x��ݒ�
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);

					// ���C�t�Q�[�W��`��
					DrawRotaGraphF(flashingPos_x, lifeGagePos.y,
						LifeInfo::kGraphScale, 0.0, m_life.handle[static_cast<int>(LifeInfo::GraphName::LifeGage)], true);

					// �����x��ݒ�
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	
				}

				// SetDrawBright���g�p���A�J���[��ݒ肷��
				SetDrawBright(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z));

				// ���C�t�t���[����`��
				DrawRotaGraphF(lifeGagePos.x, lifeGagePos.y,
					LifeInfo::kGraphScale, 0.0, m_life.handle[static_cast<int>(LifeInfo::GraphName::LifeFrame)], true);
			}

			

			// �t�F�C�X
			{
				// �t�F�C�X���W�Ɋ���W����
				Vec2 facePos = basePos[lifeSize];

				// �t�F�C�X���W�ɒ������W��������
				facePos.x += FaceInfo::kAdjustmentPos_Y;

				// �O���t�B�b�N�^�C�v
				FaceInfo::GraphTypeName graphType = FaceInfo::GraphTypeName::Noramal;

				// ���C�t��0�̏ꍇ�A�O���t�B�b�N�^�C�v�����S�ɂ���
				if (m_life.lifePoint[lifeSize] == 0)
				{
					graphType = FaceInfo::GraphTypeName::Die;
				}

				// SetDrawBright���g�p���A�J���[��ݒ肷��
				SetDrawBright(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z));

				// �t�F�C�X��`��
				DrawRotaGraphF(facePos.x, facePos.y,
					FaceInfo::kGraphScale, 0.0, 
					m_face[static_cast<int>(FaceInfo::GraphName::Face)].handle[static_cast<int>(graphType)], true);
			}

			// SetDrawBright���g�p���āA�J���[�ݒ�����Ƃɖ߂�
			SetDrawBright(255, 255, 255);
		}
	}
}

void MainUI::CharacterTagDraw(const bool& isPlayer, const int& playerNum, VECTOR pos, VECTOR color)
{
	// �L�����N�^�[�^�O��
	enum class CharacterTagName
	{
		Player_1,	// �v���C���[�P
		Player_2,	// �v���C���[�Q
		Player_3,	// �v���C���[�R
		Player_4,	// �v���C���[�S
		CP,			// �R���s���[�^
	};

	// �`����
	enum class DrawInfoName
	{
		Noramal,// �ʏ�`��
		Frame	// �t���[����`��
	};


	// Y���W����
	pos.y += TagInfo::kAdjustmentPos_Y;

	// �J���[����
	color = Convert_Lib::ColorCodeTo255(color);

	// �L�����N�^�[�^�O�̔ԍ�
	int characterTagNumber = static_cast<int>(CharacterTagName(playerNum));

	// �^�O������L�����N�^�[���v���C���[�łȂ��ꍇ�ACP�̃L�����N�^�[�^�O�ԍ���������
	if (!isPlayer)
	{
		characterTagNumber = static_cast<int>(CharacterTagName::CP);
	}

	// SetDrawBright���g�p���A�O���t�B�b�N�ɐF��t����
	SetDrawBright(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z));

	// �R�c��ԏ�ɒʏ�摜��`��
	DrawBillboard3D(pos, 0.5f, 0.5f, TagInfo::kGraphScale, 0.0f, m_tag[static_cast<int>(DrawInfoName::Noramal)].handle[characterTagNumber], true);

	// �F�����Ƃɖ߂� 
	SetDrawBright(255, 255, 255);

	// �R�c��ԏ�Ƀt���[���摜��`��
	DrawBillboard3D(pos, 0.5f, 0.5f, TagInfo::kGraphScale, 0.0f, m_tag[static_cast<int>(DrawInfoName::Frame)].handle[characterTagNumber], true);
}


