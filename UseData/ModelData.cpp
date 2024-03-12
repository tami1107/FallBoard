#include "ModelData.h"
#include <vector>
#include "GameData.h"
#include "MasterData.h"

// �V���O���g��
ModelData* ModelData::m_instance = nullptr;

namespace
{
	// �e�N�X�`���t�@�C���p�X
	std::vector<const char*>kTextureFileName
	{
		"Data/Char/model_1/Texture.jpg",
		"Data/Char/model_1/Texture_1.jpg",
		"Data/Char/model_1/Texture_2.jpg",
		"Data/Char/model_1/Texture_3.jpg",
	};
}

void ModelData::Load()
{
	// ���f���Z�b�g�A�b�v
	ModelSetUp();

	// �t�F�C�X�e�N�X�`�������[�h
	for (auto& path : kTextureFileName)
	{
		m_faceHandle.push_back(LoadGraph(path));
	}
}

void ModelData::UnLoad()
{
	// �^�C�v�̍��v��for�������[�v����
	for (int i = 0; i < static_cast<int>(m_saveModel.size()); i++)
	{
		// ���f���n���h���̍폜
		MV1DeleteModel(m_saveModel[i].handle);
	}
}

void ModelData::LoadGamePlayModel()
{
	// ���\�[�X�̓ǂݍ���
	SetUseASyncLoadFlag(true);	// �񓯊��ǂݍ��݂��s������

	// ���f���̌�
	const int modelCount = GameData::getInstance().GetGamePlayCount();
	
	// �g�p���郂�f���̔ԍ�
	const int useModelNumber = GameData::getInstance().GetModelNumber();
	
	// �v�f�𑝂₷
	m_playModel.resize(modelCount);

	// �J���[
	VECTOR color;

	// ���f������������
	for (int i = 0; i < modelCount; i++)
	{
		// ���f���𕡐�
		m_playModel[i].handle = MV1DuplicateModel(m_saveModel[useModelNumber].handle);

		// ���f���̃X�P�[���ύX
		MV1SetScale(m_playModel[i].handle, VGet(m_modelScale, m_modelScale, m_modelScale));

		// ���f���̏����p�x����
		m_playModel[i].initAngle = m_saveModel[useModelNumber].initAngle;

		// ���f���̍�������
		m_playModel[i].pos.y = m_saveModel[useModelNumber].pos.y;
		
		// �v���C���[�J���[���擾
		color = GameData::getInstance().GetPlayerColor(i);

		// �F�ݒ�
		MV1SetMaterialDifColor(m_playModel[i].handle, 0, GetColorF(color.x, color.y, color.z, 1.0f));

		// �A�E�g���C��
		MV1SetMaterialOutLineWidth(m_playModel[i].handle, 0, 0.1f);
	}

	// ���\�[�X�̓ǂݍ���
	SetUseASyncLoadFlag(false);	// �f�t�H���g�ɖ߂��Ă���
}

void ModelData::DeleteGamePlayModel()
{
	// ���f�����폜����
	for (auto& model : m_playModel)
	{
		MV1DeleteModel(model.handle);
	}
}

void ModelData::DrawModel(const DrawModelData& drawModelData)
{
	// ���f������]������
	RotModel(drawModelData.playerNumber, drawModelData.angle);

	// ���W��ύX����
	CoordinateMove(drawModelData.playerNumber, drawModelData.pos);

	// ���f���̕`��
	MV1DrawModel(m_playModel[drawModelData.playerNumber].handle);
}

void ModelData::ChangeFace(const Face& face,const int& modelNum)
{
	// �e�N�X�`����ύX����
	MV1SetTextureGraphHandle(m_playModel[modelNum].handle, 0, m_faceHandle[static_cast<int>(face)], false);
}

void ModelData::ModelSetUp()
{
	// ���f�������擾
	std::vector<MasterData::FileReadModelInfo> modelInfo = MasterData::getInstance().GetModelInfo();

	// �v�f���𑝂₷
	m_saveModel.resize(static_cast<int>(modelInfo.size()));

	// �^�C�v�̍��v��for�������[�v����
	for (int i = 0; i < static_cast<int>(m_saveModel.size()); i++)
	{
		// ���[�h���f��
		m_saveModel[i].handle = MV1LoadModel(modelInfo[i].filePath.c_str());

		// ���f���̃X�P�[���擾
		m_modelScale = modelInfo[i].scale;

		// ���W�A������p�x�����߂�
		float angle = Convert_Lib::ConvertRadianToAngle(modelInfo[i].radian);

		// �p�x������������߂�
		const Direction direction = Convert_Lib::ConvertAngleToDirection(angle, 1.0f);

		// ��������p�x�����߂�
		angle = Convert_Lib::ConvertDirectionToAngle(direction);

		// �����p�x�ϐ�
		float initAngle = 0.0f;

		// �p�x�ɂ���ď�����ύX����
		if (angle == 0.0f)
		{
			initAngle = Convert_Lib::ConvertDirectionToAngle(Direction::Bottom);
		}
		else if (angle == 90.0f)
		{
			initAngle = Convert_Lib::ConvertDirectionToAngle(Direction::Right);
		}
		else if (angle == 180.0f)
		{
			initAngle = Convert_Lib::ConvertDirectionToAngle(Direction::Top);
		}
		else if (angle == 270.0f)
		{
			initAngle = Convert_Lib::ConvertDirectionToAngle(Direction::Left);
		}

		// ���f���̏����p�x����
		m_saveModel[i].initAngle = initAngle;
		// ���f���̍������
		m_saveModel[i].pos.y = modelInfo[i].pos_y;
	}
}

void ModelData::RotModel(const int& typeNum, const int& angle)
{
	// ���f���̏����p�x������
	const int modelAngle = angle - static_cast<int>(m_playModel[typeNum].initAngle);

	// �p�x�����W�A���ɕϊ�
	const float rad = Convert_Lib::ConvertAngleToRadian(static_cast<float>(modelAngle));

	// Y���𒆐S�Ƀ��f������]������
	MV1SetRotationXYZ(m_playModel[typeNum].handle, VGet(0.0f, rad, 0.0f));
}

void ModelData::CoordinateMove(const int& typeNum, const VECTOR& pos)
{
	// ���W�Ɗ���W�𑫂�
	const VECTOR addPos = VAdd(m_playModel[typeNum].pos, pos);

	// ���f���ɍ��W������
	MV1SetPosition(m_playModel[typeNum].handle, addPos);
}