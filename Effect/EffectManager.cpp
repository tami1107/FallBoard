#include "EffectManager.h"
#include "AttackEffect.h"
#include "ResurrectionEffect.h"


EffectManager::EffectManager() :
	// �O���t�B�b�N�֘A��
	m_attackEffectInfo(),
	m_resurrectionEffectInfo(),
	// �N���X�֘A��
	m_pAttackEffect(),
	m_pResurrectionEffect(),
	m_pField(),
	m_pCharacterBase()
{
}

EffectManager::~EffectManager()
{
	// �U���G�t�F�N�g�O���t�B�b�N�n���h�����폜
	for (int i = 0; i < static_cast<int>(m_attackEffectInfo.size()); i++)
	{
		DeleteGraph(m_attackEffectInfo[i].handle);
	}

	// �����G�t�F�N�g�O���t�B�b�N�n���h�����폜	
	DeleteGraph(m_resurrectionEffectInfo.handle);
}

void EffectManager::SetCharacterBasePointer(std::shared_ptr<CharacterBase> characterBase)
{
	// �擾�����|�C���^��m_pCharacterBase�ɓ���A�v�f�𑝂₷
	m_pCharacterBase.push_back(characterBase);
}

void EffectManager::Init()
{
	// �n���h���̃��[�h
	LoadHandle();
}

void EffectManager::Update()
{
	// �U���G�t�F�N�g�̍X�V����
	for (auto& attackEffect : m_pAttackEffect)
	{
		// �X�V����
		attackEffect->Update();
	}

	// �����G�t�F�N�g�̍X�V����
	for (auto& resurrectionEffect : m_pResurrectionEffect)
	{
		// �X�V����
		resurrectionEffect->Update();
	}

	// �G�t�F�N�g�̍폜
	DeleteEffect();
}

void EffectManager::Draw()
{
	// �U���G�t�F�N�g�̕`�揈��
	for (auto& attackEffect : m_pAttackEffect)
	{
		// �`�揈��
		attackEffect->Draw();
	}

	// �����G�t�F�N�g�̕`�揈��
	for (auto& resurrectionEffect : m_pResurrectionEffect)
	{
		// �`�揈��
		resurrectionEffect->Draw();
	}
}

void EffectManager::CreateAttackEffect(const VECTOR& pos, const Direction& directon, const VECTOR& color, const int& charNum)
{
	// �U���G�t�F�N�g�|�C���^�𐶐�
	m_pAttackEffect.push_back(std::make_shared<AttackEffect>());

	// �G�t�F�N�g�ݒ�
	AttackEffect::EffectSetting effectSetting;
	{
		// �L�����i���o�[�ɉ������O���t�B�b�N�n���h������
		effectSetting.handle = m_attackEffectInfo[charNum].handle;

		// �O���t�B�b�N�X�P�[�����
		effectSetting.graphScale = m_attackEffectInfo[charNum].scale;

		// ���W����
		effectSetting.pos = pos;

		// ��������
		effectSetting.directon = directon;

		// �F����
		effectSetting.color = color;
	}

	// �U���G�t�F�N�g�|�C���^�Ŏg�p����ԍ������߂�
	const int useNumber = (static_cast<int>(m_pAttackEffect.size()) - 1);

	// �t�B�[���h�|�C���^�𑗂�
	m_pAttackEffect[useNumber]->SetFieldPointer(m_pField);

	// �U���G�t�F�N�g�̏���������
	m_pAttackEffect[useNumber]->Init(effectSetting);
}

void EffectManager::CreateResurrectionEffect(const VECTOR& pos, const VECTOR& color, const int& charNum)
{
	// �����G�t�F�N�g�|�C���^�𐶐�
	m_pResurrectionEffect.push_back(std::make_shared<ResurrectionEffect>());


	// �G�t�F�N�g�ݒ�
	ResurrectionEffect::EffectSetting effectSetting;
	{
		// �O���t�B�b�N�n���h������
		effectSetting.handle = m_resurrectionEffectInfo.handle;

		// �O���t�B�b�N�X�P�[������
		effectSetting.scale = m_resurrectionEffectInfo.scale;

		// ���W����
		effectSetting.pos = pos;

		// �F����
		effectSetting.color = color;
	}

	// �����G�t�F�N�g�|�C���^�Ŏg�p����ԍ������߂�
	const int useNumber = (static_cast<int>(m_pResurrectionEffect.size()) - 1);

	// �t�B�[���h�|�C���^�𑗂�
	m_pResurrectionEffect[useNumber]->SetFieldPointer(m_pField);

	// �L�����N�^�[�x�[�X�|�C���^�𑗂�
	m_pResurrectionEffect[useNumber]->SetCharacterBasePointer(m_pCharacterBase[charNum]);

	// �����G�t�F�N�g�̏���������
	m_pResurrectionEffect[useNumber]->Init(effectSetting);
}

void EffectManager::LoadHandle()
{
	// �U���G�t�F�N�g���[�h
	{

		// �t�@�C������ǂݎ�����U���G�t�F�N�g�����擾
		std::vector<MasterData::FileReadAttackEffectInfo>  fileReadAttackEffectInfo =
			MasterData::getInstance().GetAttackEffectInfo();

		// �U���G�t�F�N�g���̗v�f�ǉ�
		m_attackEffectInfo.resize(static_cast<int>(fileReadAttackEffectInfo.size()));

		// �g�p����ԍ�
		int useNumber = 0;

		// �U���G�t�F�N�g�̃O���t�B�b�N�����[�h
		for (int i = 0; i < static_cast<int>(fileReadAttackEffectInfo.size()); i++)
		{
			// �ԍ����擾
			useNumber = fileReadAttackEffectInfo[i].usePlayerNumber;

			// �O���t�B�b�N�̃��[�h
			m_attackEffectInfo[useNumber].handle = LoadGraph(fileReadAttackEffectInfo[i].filePath.c_str());

			// �O���t�B�b�N�̃X�P�[�����
			m_attackEffectInfo[useNumber].scale = fileReadAttackEffectInfo[i].scale;

		}

	}

	// �����G�t�F�N�g���[�h
	{

		// �t�@�C������ǂݎ���������G�t�F�N�g�����擾
		MasterData::FileReadResurrectionEffectInfo  fileReadResurrectionEffectInfo =
			MasterData::getInstance().GetResurrectionEffectInfo();

		// �O���t�B�b�N�̃��[�h
		m_resurrectionEffectInfo.handle = LoadGraph(fileReadResurrectionEffectInfo.filePath.c_str());

		// �O���t�B�b�N�̃X�P�[�����
		m_resurrectionEffectInfo.scale = fileReadResurrectionEffectInfo.scale;
	}


}

void EffectManager::DeleteEffect()
{
	// �U���G�t�F�N�g�ɍ폜�t���O�������Ă��邩�ǂ����𒲂ׁAtrue�������ꍇ�v�f������
	for (int i = 0; i < static_cast<int>(m_pAttackEffect.size()); i++)
	{
		// �폜�t���O�������Ă��邩�ǂ����𒲂ׂ�
		if (m_pAttackEffect[i]->GetIsDelete())
		{
			// �v�f�̍폜
			m_pAttackEffect.erase(m_pAttackEffect.begin() + i);
		}
	}


	// �����G�t�F�N�g�ɍ폜�t���O�������Ă��邩�ǂ����𒲂ׁAtrue�������ꍇ�v�f������
	for (int i = 0; i < static_cast<int>(m_pResurrectionEffect.size()); i++)
	{
		// �폜�t���O�������Ă��邩�ǂ����𒲂ׂ�
		if (m_pResurrectionEffect[i]->GetIsDelete())
		{
			// �v�f�̍폜
			m_pResurrectionEffect.erase(m_pResurrectionEffect.begin() + i);
		}
	}
}