#include "StateMachine.h"

#include "CharacterManager.h"
#include "Player.h"
#include "Computer.h"
#include "CharacterBase.h"
#include "Pad.h"
#include "Field.h"
#include "GameData.h"
#include "Algorithm.h"
#include "MainScreen.h"
#include "EffectManager.h"
#include "MainUI.h"


CharacterManager::CharacterManager() :
	m_pCharacterBase(),
	m_pMainScreen(),
	m_pField(),
	m_pMainParam(),
	m_pAlgorithm(),
	m_pEffectManager(std::make_shared<EffectManager>()),
	m_pMainUI()
{
}

CharacterManager::~CharacterManager()
{
}

void CharacterManager::Init()
{
	// �L�����N�^�[����
	CreateCharacter();

	// ����������
	m_pEffectManager->Init();
}

void CharacterManager::Update()
{

	for (auto& character : m_pCharacterBase)
	{
		character->Update();
	}

	// �X�V����
	m_pEffectManager->Update();
}

void CharacterManager::Draw()
{
	for (auto& character : m_pCharacterBase)
	{
		character->Draw();
	}

	// �`�揈��
	m_pEffectManager->Draw();
}

void CharacterManager::CreateCharacter()
{
	// �L�����N�^�[�X�e�[�^�X���擾
	const std::vector<GameData::CharacterStatus> characterStatus = GameData::getInstance().GetCharData();

	// �R���s���[�^�J�E���g
	int computerCount = 0;

	for (int i = 0; i < static_cast<int>(characterStatus.size()); i++)
	{
		// �v���C���[���ǂ���
		if (characterStatus[i].isPlayer)
		{
			// �v���C���[�𐶐�����
			m_pCharacterBase.push_back(std::make_shared<Player>());
		}
		else
		{
			// �R���s���[�^�𐶐�����
			m_pCharacterBase.push_back(std::make_shared<Computer>());

			// �A���S���Y������
			m_pAlgorithm.push_back(std::make_shared<Algorithm>());

			// �t�B�[���h�|�C���^�𑗂�
			m_pAlgorithm[computerCount]->SetFieldPointer(m_pField);

			// ����������
			m_pAlgorithm[computerCount]->Init();

			// �A���S���Y���|�C���^�𑗂�
			m_pCharacterBase[i]->SetAlgorithmPointer(m_pAlgorithm[computerCount]);

			// �R���s���[�^�J�E���g�𑝂₷
			computerCount++;
		}

		// ���g�̃|�C���^�𑗂�
		m_pCharacterBase[i]->SetCharacterManagerPointer(shared_from_this());

		// �t�B�[���h�|�C���^�𑗂�
		m_pCharacterBase[i]->SetFieldPointer(m_pField);

		// �p�����[�^�|�C���^�𑗂�
		m_pCharacterBase[i]->SetMainParamPointer(m_pMainParam);

		// �G�t�F�N�g�|�C���^�𑗂�
		m_pCharacterBase[i]->SetEffectManagerPointer(m_pEffectManager);

		// UI�|�C���^�𑗂�
		m_pCharacterBase[i]->SetMainUIPointer(m_pMainUI);

		// �L�����f�[�^���擾
		m_pCharacterBase[i]->SetCharInfo(characterStatus[i]);

		// ��������
		m_pCharacterBase[i]->Init();
	}
}

void CharacterManager::WinnerDecision()
{
	// �����Ă���v���C���[�̔ԍ�
	const std::vector<int> alivePlayerNumber = AlivePlayerNumber();

	// �����Ă���v���C���[�̐�
	const int alivePlayerCount = static_cast<int>(alivePlayerNumber.size());

	// �����c���Ă���l����1�ȉ��̏ꍇ�A�����X�e�[�g
	if (alivePlayerCount <= 1)
	{
		m_pCharacterBase[alivePlayerNumber[0]]->GetIsWin();
	}
}

void CharacterManager::GameEnd()
{
	// �V�[����ύX����
	m_pMainScreen->SetStateChange(MainScreen::State::End);
}

std::vector<int> CharacterManager::AlivePlayerNumber()
{
	// �����c���Ă���ԍ�
	std::vector<int> alivePlayerNumber;

	// ���ׂẴv���C���[�����āA�����Ă���v���C���[�̔ԍ��𒲂ׂ�
	for (int i = 0; i < static_cast<int>(m_pCharacterBase.size()); i++)
	{
		if (m_pCharacterBase[i]->GetCharAlive())
		{
			// �����Ă���v���C���[�̔ԍ�������
			alivePlayerNumber.push_back(m_pCharacterBase[i]->GetCharNumber());
		}
	}

	// �����Ă���v���C���[�̔ԍ���Ԃ�
	return alivePlayerNumber;
}

bool CharacterManager::IsAllCP()
{
	for (int i = 0; i < static_cast<int>(m_pCharacterBase.size()); i++)
	{
		if (m_pCharacterBase[i]->GetIsPlayer()&& m_pCharacterBase[i]->GetCharAlive())
		{
			return false;
		}
	}

	return true;
}

std::vector<VECTOR> CharacterManager::AlivePos(const int charNumber)
{
	// �����Ă���v���C���[�̔ԍ�
	const std::vector<int> aliveNumber = AlivePlayerNumber();

	// �����Ă���L�����̍��W
	std::vector<VECTOR> alivePlayerPos;

	for (int i = 0; i < static_cast<int>(aliveNumber.size()); i++)
	{
		if (aliveNumber[i] == charNumber)
		{
			continue;
		}


		// �����Ă���v���C���[�̍��W����
		alivePlayerPos.push_back(m_pCharacterBase[i]->GetPos());
	}

	return alivePlayerPos;
}
