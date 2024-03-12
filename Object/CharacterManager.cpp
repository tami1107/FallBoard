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
	// キャラクター生成
	CreateCharacter();

	// 初期化処理
	m_pEffectManager->Init();
}

void CharacterManager::Update()
{

	for (auto& character : m_pCharacterBase)
	{
		character->Update();
	}

	// 更新処理
	m_pEffectManager->Update();
}

void CharacterManager::Draw()
{
	for (auto& character : m_pCharacterBase)
	{
		character->Draw();
	}

	// 描画処理
	m_pEffectManager->Draw();
}

void CharacterManager::CreateCharacter()
{
	// キャラクターステータスを取得
	const std::vector<GameData::CharacterStatus> characterStatus = GameData::getInstance().GetCharData();

	// コンピュータカウント
	int computerCount = 0;

	for (int i = 0; i < static_cast<int>(characterStatus.size()); i++)
	{
		// プレイヤーかどうか
		if (characterStatus[i].isPlayer)
		{
			// プレイヤーを生成する
			m_pCharacterBase.push_back(std::make_shared<Player>());
		}
		else
		{
			// コンピュータを生成する
			m_pCharacterBase.push_back(std::make_shared<Computer>());

			// アルゴリズム生成
			m_pAlgorithm.push_back(std::make_shared<Algorithm>());

			// フィールドポインタを送る
			m_pAlgorithm[computerCount]->SetFieldPointer(m_pField);

			// 初期化処理
			m_pAlgorithm[computerCount]->Init();

			// アルゴリズムポインタを送る
			m_pCharacterBase[i]->SetAlgorithmPointer(m_pAlgorithm[computerCount]);

			// コンピュータカウントを増やす
			computerCount++;
		}

		// 自身のポインタを送る
		m_pCharacterBase[i]->SetCharacterManagerPointer(shared_from_this());

		// フィールドポインタを送る
		m_pCharacterBase[i]->SetFieldPointer(m_pField);

		// パラメータポインタを送る
		m_pCharacterBase[i]->SetMainParamPointer(m_pMainParam);

		// エフェクトポインタを送る
		m_pCharacterBase[i]->SetEffectManagerPointer(m_pEffectManager);

		// UIポインタを送る
		m_pCharacterBase[i]->SetMainUIPointer(m_pMainUI);

		// キャラデータを取得
		m_pCharacterBase[i]->SetCharInfo(characterStatus[i]);

		// 初期処理
		m_pCharacterBase[i]->Init();
	}
}

void CharacterManager::WinnerDecision()
{
	// 生きているプレイヤーの番号
	const std::vector<int> alivePlayerNumber = AlivePlayerNumber();

	// 生きているプレイヤーの数
	const int alivePlayerCount = static_cast<int>(alivePlayerNumber.size());

	// 生き残っている人数が1以下の場合、勝利ステート
	if (alivePlayerCount <= 1)
	{
		m_pCharacterBase[alivePlayerNumber[0]]->GetIsWin();
	}
}

void CharacterManager::GameEnd()
{
	// シーンを変更する
	m_pMainScreen->SetStateChange(MainScreen::State::End);
}

std::vector<int> CharacterManager::AlivePlayerNumber()
{
	// 生き残っている番号
	std::vector<int> alivePlayerNumber;

	// すべてのプレイヤーを見て、生きているプレイヤーの番号を調べる
	for (int i = 0; i < static_cast<int>(m_pCharacterBase.size()); i++)
	{
		if (m_pCharacterBase[i]->GetCharAlive())
		{
			// 生きているプレイヤーの番号を入れる
			alivePlayerNumber.push_back(m_pCharacterBase[i]->GetCharNumber());
		}
	}

	// 生きているプレイヤーの番号を返す
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
	// 生きているプレイヤーの番号
	const std::vector<int> aliveNumber = AlivePlayerNumber();

	// 生きているキャラの座標
	std::vector<VECTOR> alivePlayerPos;

	for (int i = 0; i < static_cast<int>(aliveNumber.size()); i++)
	{
		if (aliveNumber[i] == charNumber)
		{
			continue;
		}


		// 生きているプレイヤーの座標を代入
		alivePlayerPos.push_back(m_pCharacterBase[i]->GetPos());
	}

	return alivePlayerPos;
}
