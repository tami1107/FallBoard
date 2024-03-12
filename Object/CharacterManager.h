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

	// クラスポインタ取得
	void SetMainScreenPointer(std::shared_ptr<MainScreen>mainScreen) { m_pMainScreen = mainScreen; }
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }
	void SetMainParamPointer(std::shared_ptr<MainParam>mainParam) { m_pMainParam = mainParam; }
	void SetMainUIPointer(std::shared_ptr<MainUI>mainUI) { m_pMainUI = mainUI; }


	void Init();
	void Update();
	void Draw();

public:

	// 生きているキャラの座標を返す
	std::vector<VECTOR>AlivePos(const int charNumber);

	// 勝者が決まったかどうか
	void WinnerDecision();

	// ゲーム終了
	void GameEnd();

	// すべてCPかどうか
	bool IsAllCP();
private:

	// キャラクター生成
	void CreateCharacter();

	
	// 生きているプレイヤーの番号
	std::vector<int> AlivePlayerNumber();



private:
	
	
	
	// クラスポインタ
	std::vector <std::shared_ptr<CharacterBase>>m_pCharacterBase;
	std::shared_ptr<MainScreen>m_pMainScreen;
	std::shared_ptr<Field>m_pField;
	std::shared_ptr<MainParam>m_pMainParam;
	std::shared_ptr<EffectManager>m_pEffectManager;
	std::shared_ptr<MainUI>m_pMainUI;
	std::vector <std::shared_ptr<Algorithm>>m_pAlgorithm;
};

