#include "EffectManager.h"
#include "AttackEffect.h"
#include "ResurrectionEffect.h"


EffectManager::EffectManager() :
	// グラフィック関連↓
	m_attackEffectInfo(),
	m_resurrectionEffectInfo(),
	// クラス関連↓
	m_pAttackEffect(),
	m_pResurrectionEffect(),
	m_pField(),
	m_pCharacterBase()
{
}

EffectManager::~EffectManager()
{
	// 攻撃エフェクトグラフィックハンドルを削除
	for (int i = 0; i < static_cast<int>(m_attackEffectInfo.size()); i++)
	{
		DeleteGraph(m_attackEffectInfo[i].handle);
	}

	// 復活エフェクトグラフィックハンドルを削除	
	DeleteGraph(m_resurrectionEffectInfo.handle);
}

void EffectManager::SetCharacterBasePointer(std::shared_ptr<CharacterBase> characterBase)
{
	// 取得したポインタをm_pCharacterBaseに入れ、要素を増やす
	m_pCharacterBase.push_back(characterBase);
}

void EffectManager::Init()
{
	// ハンドルのロード
	LoadHandle();
}

void EffectManager::Update()
{
	// 攻撃エフェクトの更新処理
	for (auto& attackEffect : m_pAttackEffect)
	{
		// 更新処理
		attackEffect->Update();
	}

	// 復活エフェクトの更新処理
	for (auto& resurrectionEffect : m_pResurrectionEffect)
	{
		// 更新処理
		resurrectionEffect->Update();
	}

	// エフェクトの削除
	DeleteEffect();
}

void EffectManager::Draw()
{
	// 攻撃エフェクトの描画処理
	for (auto& attackEffect : m_pAttackEffect)
	{
		// 描画処理
		attackEffect->Draw();
	}

	// 復活エフェクトの描画処理
	for (auto& resurrectionEffect : m_pResurrectionEffect)
	{
		// 描画処理
		resurrectionEffect->Draw();
	}
}

void EffectManager::CreateAttackEffect(const VECTOR& pos, const Direction& directon, const VECTOR& color, const int& charNum)
{
	// 攻撃エフェクトポインタを生成
	m_pAttackEffect.push_back(std::make_shared<AttackEffect>());

	// エフェクト設定
	AttackEffect::EffectSetting effectSetting;
	{
		// キャラナンバーに沿ったグラフィックハンドルを代入
		effectSetting.handle = m_attackEffectInfo[charNum].handle;

		// グラフィックスケール代入
		effectSetting.graphScale = m_attackEffectInfo[charNum].scale;

		// 座標を代入
		effectSetting.pos = pos;

		// 方向を代入
		effectSetting.directon = directon;

		// 色を代入
		effectSetting.color = color;
	}

	// 攻撃エフェクトポインタで使用する番号を求める
	const int useNumber = (static_cast<int>(m_pAttackEffect.size()) - 1);

	// フィールドポインタを送る
	m_pAttackEffect[useNumber]->SetFieldPointer(m_pField);

	// 攻撃エフェクトの初期化処理
	m_pAttackEffect[useNumber]->Init(effectSetting);
}

void EffectManager::CreateResurrectionEffect(const VECTOR& pos, const VECTOR& color, const int& charNum)
{
	// 復活エフェクトポインタを生成
	m_pResurrectionEffect.push_back(std::make_shared<ResurrectionEffect>());


	// エフェクト設定
	ResurrectionEffect::EffectSetting effectSetting;
	{
		// グラフィックハンドルを代入
		effectSetting.handle = m_resurrectionEffectInfo.handle;

		// グラフィックスケールを代入
		effectSetting.scale = m_resurrectionEffectInfo.scale;

		// 座標を代入
		effectSetting.pos = pos;

		// 色を代入
		effectSetting.color = color;
	}

	// 復活エフェクトポインタで使用する番号を求める
	const int useNumber = (static_cast<int>(m_pResurrectionEffect.size()) - 1);

	// フィールドポインタを送る
	m_pResurrectionEffect[useNumber]->SetFieldPointer(m_pField);

	// キャラクターベースポインタを送る
	m_pResurrectionEffect[useNumber]->SetCharacterBasePointer(m_pCharacterBase[charNum]);

	// 復活エフェクトの初期化処理
	m_pResurrectionEffect[useNumber]->Init(effectSetting);
}

void EffectManager::LoadHandle()
{
	// 攻撃エフェクトロード
	{

		// ファイルから読み取った攻撃エフェクト情報を取得
		std::vector<MasterData::FileReadAttackEffectInfo>  fileReadAttackEffectInfo =
			MasterData::getInstance().GetAttackEffectInfo();

		// 攻撃エフェクト情報の要素追加
		m_attackEffectInfo.resize(static_cast<int>(fileReadAttackEffectInfo.size()));

		// 使用する番号
		int useNumber = 0;

		// 攻撃エフェクトのグラフィックをロード
		for (int i = 0; i < static_cast<int>(fileReadAttackEffectInfo.size()); i++)
		{
			// 番号を取得
			useNumber = fileReadAttackEffectInfo[i].usePlayerNumber;

			// グラフィックのロード
			m_attackEffectInfo[useNumber].handle = LoadGraph(fileReadAttackEffectInfo[i].filePath.c_str());

			// グラフィックのスケール代入
			m_attackEffectInfo[useNumber].scale = fileReadAttackEffectInfo[i].scale;

		}

	}

	// 復活エフェクトロード
	{

		// ファイルから読み取った復活エフェクト情報を取得
		MasterData::FileReadResurrectionEffectInfo  fileReadResurrectionEffectInfo =
			MasterData::getInstance().GetResurrectionEffectInfo();

		// グラフィックのロード
		m_resurrectionEffectInfo.handle = LoadGraph(fileReadResurrectionEffectInfo.filePath.c_str());

		// グラフィックのスケール代入
		m_resurrectionEffectInfo.scale = fileReadResurrectionEffectInfo.scale;
	}


}

void EffectManager::DeleteEffect()
{
	// 攻撃エフェクトに削除フラグが立っているかどうかを調べ、trueだった場合要素を消す
	for (int i = 0; i < static_cast<int>(m_pAttackEffect.size()); i++)
	{
		// 削除フラグが立っているかどうかを調べる
		if (m_pAttackEffect[i]->GetIsDelete())
		{
			// 要素の削除
			m_pAttackEffect.erase(m_pAttackEffect.begin() + i);
		}
	}


	// 復活エフェクトに削除フラグが立っているかどうかを調べ、trueだった場合要素を消す
	for (int i = 0; i < static_cast<int>(m_pResurrectionEffect.size()); i++)
	{
		// 削除フラグが立っているかどうかを調べる
		if (m_pResurrectionEffect[i]->GetIsDelete())
		{
			// 要素の削除
			m_pResurrectionEffect.erase(m_pResurrectionEffect.begin() + i);
		}
	}
}