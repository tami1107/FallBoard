#pragma once
#include <memory>
#include <vector>
#include <DxLib.h>
#include <iostream>


class MainScreen;
class Field;
class MainParam;
class Algorithm;
class Player;
class Computer;
class CharacterBase;
class EffectManager;
class MainUI;
class CharacterManager : public std::enable_shared_from_this<CharacterManager>
{
public:

	CharacterManager();
	virtual ~CharacterManager();

	// �N���X�|�C���^�擾
	void SetMainScreenPointer(std::shared_ptr<MainScreen>mainScreen) { m_pMainScreen = mainScreen; }
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }
	void SetMainParamPointer(std::shared_ptr<MainParam>mainParam) { m_pMainParam = mainParam; }
	void SetMainUIPointer(std::shared_ptr<MainUI>mainUI) { m_pMainUI = mainUI; }


	void Init();
	void Update();
	void Draw();

public:

	// �����Ă���L�����̍��W��Ԃ�
	std::vector<VECTOR>AlivePos(const int charNumber);

	// ���҂����܂������ǂ���
	void WinnerDecision();

	// �Q�[���I��
	void GameEnd();

	// ���ׂ�CP���ǂ���
	bool IsAllCP();
private:

	// �L�����N�^�[����
	void CreateCharacter();

	
	// �����Ă���v���C���[�̔ԍ�
	std::vector<int> AlivePlayerNumber();



private:
	
	
	
	// �N���X�|�C���^
	std::vector <std::shared_ptr<CharacterBase>>m_pCharacterBase;
	std::shared_ptr<MainScreen>m_pMainScreen;
	std::shared_ptr<Field>m_pField;
	std::shared_ptr<MainParam>m_pMainParam;
	std::shared_ptr<EffectManager>m_pEffectManager;
	std::shared_ptr<MainUI>m_pMainUI;
	std::vector <std::shared_ptr<Algorithm>>m_pAlgorithm;
};

