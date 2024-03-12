#pragma once

#include <memory>
#include <DxLib.h>
#include "EvoLib.h"
#include "ModelData.h"
#include "Field.h"
#include <array>
#include "GameData.h"

// ステートマシン
template <class TState> class StateMachine;

class Field;
class MainParam;
class Algorithm;
class CharacterManager;
class EffectManager;
class MainUI;
class CharacterBase : public std::enable_shared_from_this<CharacterBase>
{

public:

	// ステート
	enum class State
	{
		// 登場処理
		Pop,
		// 通常状態
		Normal,
		// 攻撃状態
		Attack,
		// 落下状態
		Fall,
		// 死亡状態
		Death,
		// 勝利状態
		Win,
	};


protected:

	// 移動向き
	struct MoveDirection
	{
		// 前
		Direction before = Direction::Right;

		// 現在
		Direction current = Direction::Right;

		// 後
		Direction after = Direction::Right;
	};

	// 方向情報
	struct DirectionInfo
	{
		// 向き
		MoveDirection direction = MoveDirection();

		// 転換向き
		MoveDirection changeDirection = MoveDirection();

		// 方向転換中かどうか
		bool isChangeDirection = false;

		// 右を向いていたかどうか
		bool isRightDirection = false;

		// 上を向いていたかどうか
		bool isTopDirection = false;

		// 角度
		int modelAngle = 0;
	};

	// 無敵情報
	struct InvincibleInfo
	{
		// 無敵かどうか
		bool isInvincible = false;

		// 無敵時間
		int invincibleFrame = 0;

		// 復活時のセル
		Field::Cell resurrectionCell = Field::Cell();
	};


public:
	CharacterBase();
	virtual ~CharacterBase();

	/// <summary>
	/// フィールドポインタを取得
	/// </summary>
	/// <param name="field">フィールドポインタ</param>
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }

	/// <summary>
	/// メインパラムポインタを取得
	/// </summary>
	/// <param name="mainParam">メインパラムポインタ</param>
	void SetMainParamPointer(std::shared_ptr<MainParam>mainParam) { m_pMainParam = mainParam; }

	/// <summary>
	/// アルゴリズムポインタを取得
	/// </summary>
	/// <param name="algorithm">アルゴリズムポインタ</param>
	void SetAlgorithmPointer(std::shared_ptr<Algorithm>algorithm) { m_pAlgorithm = algorithm; }

	/// <summary>
	/// キャラクターマネージャーポインタを取得
	/// </summary>
	/// <param name="characterManager">キャラクターマネージャーポインタ</param>
	void SetCharacterManagerPointer(std::shared_ptr<CharacterManager>characterManager) { m_pCharacterManager = characterManager; }

	/// <summary>
	/// エフェクトマネージャーポインタを取得
	/// </summary>
	/// <param name="effectManager">エフェクトマネージャーポインタ</param>
	void SetEffectManagerPointer(std::shared_ptr<EffectManager>effectManager) { m_pEffectManager = effectManager; }

	/// <summary>
	/// メインUIポインタを取得
	/// </summary>
	/// <param name="mainUI">メインUIポインタ</param>
	void SetMainUIPointer(std::shared_ptr<MainUI>mainUI) { m_pMainUI = mainUI; }

		
	virtual void Init();
	virtual void Update();
	virtual void Draw();

public:

	// 登場処理
	void PopEnter();
	void PopUpdate();

	// 通常処理
	void NormalUpdate();

	// 通常処理
	void AttackEnter();
	void AttackUpdate();

	// 落下状態
	void FallEnter();
	void FallUpdate();
	void FallExit();

	// 死亡状態
	void DeathEnter();

	// 勝利状態
	void WinEnter();
	void WinUpdate();


	/// <summary>
	/// キャラクター情報を取得
	/// </summary>
	/// <param name="charData">キャラクター情報</param>
	void SetCharInfo(const GameData::CharacterStatus& charData) { m_charData = charData; }

	/// <summary>
	/// キャラが生きているかどうかを返す
	/// </summary>
	/// <returns>キャラが生きているかどうかのフラグ</returns>
	bool GetCharAlive();

	/// <summary>
	/// 自身がプレイヤーかどうかを返す
	/// </summary>
	/// <returns></returns>
	bool GetIsPlayer() { return m_charData.isPlayer; }

	/// <summary>
	/// キャラクター番号を返す
	/// </summary>
	/// <returns>キャラクター番号</returns>
	int GetCharNumber() const { return m_charData.charNumber; }

	/// <summary>
	/// 無敵時間中かどうかを返す
	/// </summary>
	/// <returns>無敵時間中かどうかのフラグ</returns>
	bool GetIsInvincible(){return m_invincibleInfo.isInvincible;}

	/// <summary>
	/// 勝利したかどうかを受け取る
	/// </summary>
	void GetIsWin() { m_isWin = true; }

	/// <summary>
	/// ステートの変更
	/// </summary>
	/// <param name="state">ステート</param>
	void SetStateChange(const State& state);

	/// <summary>
	/// ステートを返す
	/// </summary>
	/// <returns>ステート</returns>
	State GetState() { return m_state; }

	/// <summary>
	/// 自身の座標を返す
	/// </summary>
	/// <returns>座標</returns>
	VECTOR GetPos() { return m_pos; }

protected:

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void InitSet(){};

	/// <summary>
	/// 操作処理
	/// </summary>
	virtual void Operation() = 0;

	/// <summary>
	/// 移動処理
	/// </summary>
	virtual void Move() = 0;

	/// <summary>
	/// 攻撃開始処理
	/// </summary>
	virtual void BeginningAttack() = 0;

	/// <summary>
	/// 攻撃処理
	/// </summary>
	virtual void Attack();

	/// <summary>
	/// 攻撃インターバル処理
	/// </summary>
	virtual void AttackInterval();

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	virtual void Collision();

	/// <summary>
	/// 盤面の当たり判定
	/// </summary>
	virtual void BoardCollision();

	/// <summary>
	/// 落下判定
	/// </summary>
	virtual void Fall();

	/// <summary>
	/// 生きているキャラクターの座標を取得
	/// </summary>
	/// <returns>生きているキャラクターの座標</returns>
	std::vector<VECTOR> GetAlivePos();


private:


	/// <summary>
	/// 無敵処理
	/// </summary>
	void Invincible();

	/// <summary>
	/// モデルの角度処理
	/// </summary>
	void ModelAngle();

	/// <summary>
	/// モデル描画処理
	/// </summary>
	void DrawModel();

	/// <summary>
	/// ランダム方向処理
	/// </summary>
	void RandomDirection();

	/// <summary>
	/// 勝利モーション処理
	/// </summary>
	void WinMotion();

protected:

	////////////////////////////
	// キャラクターデータ関連 //
	////////////////////////////
	
	GameData::CharacterStatus m_charData;// キャラデータ

	//////////////
	// 方向関連 //
	//////////////

	// 方向情報
	DirectionInfo m_directionInfo;

	//////////////
	// 攻撃関連 //
	//////////////

	// 攻撃モーションインターバル
	int m_attackMotionInterval;

	//////////////
	// 復帰関連 //
	//////////////

	// 無敵情報
	InvincibleInfo m_invincibleInfo;

	//////////////
	// 座標関連 //
	//////////////

	// 座標
	VECTOR m_pos;

	// 移動ベクトル
	VECTOR m_vec;

	
	//////////////////
	// ステート関連 //
	//////////////////

	// ステート
	State m_state;

	// 勝利したかどうか
	bool m_isWin;

	////////////////////
	// シーン変更関連 //
	////////////////////

	// ゲーム終了フレーム
	int m_gameEndFrame;

	////////////////
	// クラス関連 //
	////////////////

	StateMachine<State>m_pStateMachine;
	std::shared_ptr<Field>m_pField;
	std::shared_ptr<MainParam>m_pMainParam;
	std::shared_ptr<Algorithm>m_pAlgorithm;
	std::shared_ptr<CharacterManager>m_pCharacterManager;
	std::shared_ptr<EffectManager>m_pEffectManager;
	std::shared_ptr<MainUI>m_pMainUI;
};

