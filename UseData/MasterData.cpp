#include "MasterData.h"
#include "EvoLib.h"
#include "string"

// �L�����N�^�[
namespace Character
{
	// �L�����N�^�[�X�e�[�^�X�̃t�@�C���p�X
	const char* const kStatusFilePath = "Data/csv/ChangeableData/CharacterStatus.csv";

	// �L�����N�^�[�J���[�̃t�@�C���p�X
	const char* const kColorFilePath = "Data/csv/ChangeableData/CharacterColor.csv";

	// �L�����N�^�[���f�����t�@�C���p�X
	const char* const kModelInfoFilePath = "Data/csv/ChangeableData/ModelInformation.csv";
}

// �G�t�F�N�g
namespace Effect
{
	// �U���G�t�F�N�g���̃t�@�C���p�X
	const char* const kAttackEffectInfoFilePath = "Data/csv/ChangeableData/AttackEffectInfo.csv";

	// �����G�t�F�N�g���̃t�@�C���p�X
	const char* const kResurrectionEffectPath = "Data/csv/ChangeableData/ResurrectionEffectInfo.csv";
}

// �t�B�[���h
namespace Field
{
	// �t�B�[���h���f�����t�@�C���p�X
	const char* const kModelInfoFilePath = "Data/csv/ChangeableData/FieldCellInfo.csv";

	// �t�B�[���h�X�e�[�^�X�̃t�@�C���p�X
	const char* const kStatusFilePath = "Data/csv/ChangeableData/FieldStatus.csv";

	// �t�B�[���h���f���J���[�̃t�@�C���p�X
	const char* const kModelColorFilePath = "Data/csv/ChangeableData/FieldColor.csv";
}


MasterData* MasterData::m_instance = nullptr;



void MasterData::LoadData()
{
	// �L�����N�^�[�X�e�[�^�X�̃��[�h����
	LoadCharcterStatus();

	// �U���G�t�F�N�g���̃��[�h����
	LoadAtackEffectInfo();

	// �����G�t�F�N�g���̃��[�h����
	LoadResurrectionEffectInfo();

	// �t�B�[���h���̃��[�h����
	LoadFieldInfo();

	// ���f�����̃��[�h����
	LoadModelInfo();
}

void MasterData::LoadCharcterStatus()
{
	// �L�����X�e�[�g��
	enum class CharStateName
	{
		HitPoint,		// �ő�̗�
		MoveSpeed,		// �ړ��X�s�[�h
		AttackRigorTime,// �U���d������
		InvincibleTime,	// ���A���̖��G����
		Color,			// �F
	};

	// �L�����N�^�[�X�e�[�^�X�f�[�^�擾
	const std::vector<std::vector<std::string>>charcterStatusData = File_Lib::CsvFileLoading_Revision(Character::kStatusFilePath, File_Lib::LoadType::SkipOneColumn);

	// �v���C���[�̍ő�̗�
	m_charcterStatus.lifePoint = static_cast<int> (Convert_Lib::ConvertingStringToFloat(charcterStatusData[static_cast<int>(CharStateName::HitPoint)][0]));

	// �ړ����x
	m_charcterStatus.moveSpeed = Convert_Lib::ConvertingStringToFloat(charcterStatusData[static_cast<int>(CharStateName::MoveSpeed)][0]);

	// �U���d������
	m_charcterStatus.attackRigorTime = static_cast<int> (Convert_Lib::ConvertingStringToFloat(charcterStatusData[static_cast<int>(CharStateName::AttackRigorTime)][0]));

	// ���A���̖��G����
	m_charcterStatus.invincibleTime = static_cast<int> (Convert_Lib::ConvertingStringToFloat(charcterStatusData[static_cast<int>(CharStateName::InvincibleTime)][0]));



	// �J���[��
	enum class ColorName
	{
		Red,	// ��
		Green,	// ��
		Blue,	// ��
	};

	// �L�����N�^�[�J���[�f�[�^�擾
	const std::vector<std::vector<std::string>>charcterColorData = File_Lib::CsvFileLoading_Revision(Character::kColorFilePath, File_Lib::LoadType::DoubleSkip);

	// �J���[
	VECTOR color;

	// �L�����N�^�[�J���[����
	for (int i = 0; i < static_cast<int>(charcterColorData.size()); i++)
	{
		// �J���[����
		color.x = Convert_Lib::ConvertingStringToFloat(charcterColorData[i][static_cast<int>(ColorName::Red)]);
		color.y = Convert_Lib::ConvertingStringToFloat(charcterColorData[i][static_cast<int>(ColorName::Green)]);
		color.z = Convert_Lib::ConvertingStringToFloat(charcterColorData[i][static_cast<int>(ColorName::Blue)]);

		// m_charcterStatus�ɃJ���[��������
		m_charcterStatus.color.push_back(color);
	}
}

void MasterData::LoadAtackEffectInfo()
{
	// �U���G�t�F�N�g���
	enum class AttackEffectInfoName
	{
		FilePath,		// �O���t�B�b�N�t�@�C���p�X
		GraphScale,		// �O���t�B�b�N�X�P�[��
		UsePlayerNumber,// �g�p����v���C���[�ԍ�
	};

	// �U���G�t�F�N�g�f�[�^�擾
	const std::vector<std::vector<std::string>>attackEffectInfoData = File_Lib::CsvFileLoading_Revision(Effect::kAttackEffectInfoFilePath, File_Lib::LoadType::SkipFirstLine);

	// �s������
	const int lineCount = static_cast<int>(attackEffectInfoData.size());
	
	// �ꎞ�ۑ�
	FileReadAttackEffectInfo saveAttackEffectInfo;
	
	// �s�����A�������J��Ԃ�
	for (int i = 0; i < lineCount; i++)
	{
		// �t�@�C���p�X���
		saveAttackEffectInfo.filePath = attackEffectInfoData[i][static_cast<int> (AttackEffectInfoName::FilePath)];
			
		// �O���t�B�b�N�X�P�[�����
		saveAttackEffectInfo.scale =
			Convert_Lib::ConvertingStringToFloat(attackEffectInfoData[i][static_cast<int> (AttackEffectInfoName::GraphScale)]);

		// �g�p����v���C���[�ԍ�����
		saveAttackEffectInfo.usePlayerNumber =
			static_cast<int>(Convert_Lib::ConvertingStringToFloat(attackEffectInfoData[i][static_cast<int> (AttackEffectInfoName::UsePlayerNumber)]));

		// �ꎞ�ۑ������f�[�^��v�f�ɒǉ�����
		m_attackEffectInfo.push_back(saveAttackEffectInfo);
	}
}

void MasterData::LoadResurrectionEffectInfo()
{
	// �����G�t�F�N�g���
	enum class ResurrectionEffectInfoName
	{
		FilePath,		// �O���t�B�b�N�t�@�C���p�X
		GraphScale,		// �O���t�B�b�N�X�P�[��
	};


	// �����G�t�F�N�g�f�[�^�擾
	const std::vector<std::vector<std::string>>resurrectionEffectInfoData = File_Lib::CsvFileLoading_Revision(Effect::kResurrectionEffectPath, File_Lib::LoadType::SkipFirstLine);

	// �s��
	const int lineNum = 0;

	// �t�@�C���p�X���
	m_resurrectionEffectInfo.filePath = 
		resurrectionEffectInfoData[lineNum][static_cast<int> (ResurrectionEffectInfoName::FilePath)];

	// �O���t�B�b�N�X�P�[�����
	m_resurrectionEffectInfo.scale =
		Convert_Lib::ConvertingStringToFloat(resurrectionEffectInfoData[lineNum][static_cast<int> (ResurrectionEffectInfoName::GraphScale)]);

}

void MasterData::LoadFieldInfo()
{
	// ���f�����
	{
		// �t�B�[���h���f�����
		enum class FieldModelInfoName
		{
			FilePath,				// �t�@�C���p�X
			ModelScale,				// ���f���X�P�[��
		};

		// �t�B�[���h���f�����擾
		const std::vector<std::vector<std::string>>fieldModelInfoData = File_Lib::CsvFileLoading_Revision(Field::kModelInfoFilePath, File_Lib::LoadType::SkipFirstLine);

		// �s��
		const int lineNum = 0;


		// �t�B�[���h�̃��f���p�X����
		m_fieldInfo.filePath = fieldModelInfoData[lineNum][static_cast<int>(FieldModelInfoName::FilePath)];

		// ���f���̃X�P�[�����擾
		m_fieldInfo.modelScale = Convert_Lib::ConvertingStringToFloat(fieldModelInfoData[lineNum][static_cast<int>(FieldModelInfoName::ModelScale)]);
	}

	// �t�B�[���h�X�e�[�^�X
	{
		// �t�B�[���h�X�e�[�^�X��
		enum class FieldStatusName
		{
			FallInterval,			// �����C���^�[�o��
			DifferenceFallInterval,	// �����Ԋu
			DeleteFallColorInterval,// �Ֆʗ����J���[�C���^�[�o��
			FallSpeed,				// �����X�s�[�h
			FallLowerLimitPos,		// ���������ʒu
		};

		// �t�B�[���h�X�e�[�^�X���擾
		const std::vector<std::vector<std::string>>fieldStatusData = File_Lib::CsvFileLoading_Revision(Field::kStatusFilePath, File_Lib::LoadType::SkipOneColumn);

		// ��
		const int columnNum = 0;

		// �����C���^�[�o�����
		m_fieldInfo.fallInterval = static_cast<int>(Convert_Lib::ConvertingStringToFloat(fieldStatusData[static_cast<int>(FieldStatusName::FallInterval)][columnNum]));

		// �����Ԋu
		m_fieldInfo.differenceFallInterval= static_cast<int>(Convert_Lib::ConvertingStringToFloat(fieldStatusData[static_cast<int>(FieldStatusName::DifferenceFallInterval)][columnNum]));

		// �Ֆʏk�������J���[�C���^�[�o��
		m_fieldInfo.deleteFallColorInterval = static_cast<int>(Convert_Lib::ConvertingStringToFloat(fieldStatusData[static_cast<int>(FieldStatusName::DeleteFallColorInterval)][columnNum]));

		// �����X�s�[�h
		m_fieldInfo.fallSpeed = Convert_Lib::ConvertingStringToFloat(fieldStatusData[static_cast<int>(FieldStatusName::FallSpeed)][columnNum]);

		// ���������ʒu
		m_fieldInfo.fallLowerLimitPos = Convert_Lib::ConvertingStringToFloat(fieldStatusData[static_cast<int>(FieldStatusName::FallLowerLimitPos)][columnNum]);
	}

	// �J���[
	{

		// �J���[��
		enum class ColorName
		{
			Red,	// ��
			Green,	// ��
			Blue,	// ��
		};

		// �t�B�[���h�J���[���擾
		const std::vector<std::vector<std::string>>fieldColorInfoData = File_Lib::CsvFileLoading_Revision(Field::kModelColorFilePath, File_Lib::LoadType::DoubleSkip);

		// ��
		const int columnNum = 0;

		// �ꎞ�ۑ��J���[
		std::vector<VECTOR>saveColor;

		// �ꎞ�ۑ��J���[�̗v�f�𑝂₷
		saveColor.resize(static_cast<int>(fieldColorInfoData.size()));

		// �L�����N�^�[�J���[����
		for (int i = 0; i < static_cast<int>(fieldColorInfoData.size()); i++)
		{
			// �J���[����
			saveColor[i].x = Convert_Lib::ConvertingStringToFloat(fieldColorInfoData[i][static_cast<int>(ColorName::Red)]);
			saveColor[i].y = Convert_Lib::ConvertingStringToFloat(fieldColorInfoData[i][static_cast<int>(ColorName::Green)]);
			saveColor[i].z = Convert_Lib::ConvertingStringToFloat(fieldColorInfoData[i][static_cast<int>(ColorName::Blue)]);
		}

		// �������̃O���t�B�b�N�J���[����
		m_fieldInfo.fallColor = saveColor[0];
	
		
		// �ꎞ�ۑ��J���[���
		ColorInfo saveColorInfo = ColorInfo();

		// HACK:�R�[�h���G
		{
			saveColorInfo.color_0 = saveColor[1];
			saveColorInfo.color_1 = saveColor[2];

			m_fieldInfo.color.push_back(saveColorInfo);

			saveColorInfo.color_0 = saveColor[3];
			saveColorInfo.color_1 = saveColor[4];

			m_fieldInfo.color.push_back(saveColorInfo);

			saveColorInfo.color_0 = saveColor[5];
			saveColorInfo.color_1 = saveColor[6];

			m_fieldInfo.color.push_back(saveColorInfo);
		}
	}
}

void MasterData::LoadModelInfo()
{
	// ���f�����
	enum class FieldStatusName
	{
		FilePath,	//�t�@�C���p�X
		Radian,		// ���f���̌���
		Scale,		// �X�P�[��
		Pos_y,		// Y���W
		IsUseMotion,// ���[�V�������g���邩�ǂ���
	};

	// ���[�h�����e�L�X�g
	std::vector<std::vector<std::string>> loadText = File_Lib::CsvFileLoading("Data/csv/ModelInformation.csv");

	// ���f�����
	std::vector<FileReadModelInfo> modelInfo;

	// ���f�����
	modelInfo.resize(static_cast<int>(loadText.size()));

	for (int i = 0; i < static_cast<int>(modelInfo.size()); i++)
	{
		// �t�@�C���p�X���
		modelInfo[i].filePath = loadText[i][static_cast<int>(FieldStatusName::FilePath)];

		// ���f���̌���
		modelInfo[i].radian = Convert_Lib::ConvertingStringToFloat(loadText[i][static_cast<int>(FieldStatusName::Radian)]);

		// ���f���̃X�P�[��
		modelInfo[i].scale = Convert_Lib::ConvertingStringToFloat(loadText[i][static_cast<int>(FieldStatusName::Scale)]);

		// ���f����Y���W
		modelInfo[i].pos_y = Convert_Lib::ConvertingStringToFloat(loadText[i][static_cast<int>(FieldStatusName::Pos_y)]);

		// ���[�V�������g���邩�ǂ���
		if (Convert_Lib::ConvertingStringToFloat(loadText[i][static_cast<int>(FieldStatusName::IsUseMotion)]) == 0)
		{
			modelInfo[i].isUseMotion = false;
		}
		else
		{
			modelInfo[i].isUseMotion = true;
		}
	}

	// ���f������������
	m_modelInfo = modelInfo;
}
