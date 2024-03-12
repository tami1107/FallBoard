#pragma once
#include <vector>
#include "MainScreen.h"


class MainParam
{
public:

	// 登場
	struct Pop
	{
		// カウントダウン
		int countDownFrame = 0;
	};

	// キャラパラメータ
	struct CharParam
	{
		// 体力
		int lifePoint = 0;

		// ダメージを受けたかどうか
		bool isDamage = false;

		// 死んだかどうか
		bool isDie = false;
	};

	// 盤面
	struct Board
	{
		// 盤面スケール
		int scale = 0;

		// 落下させる回数
		int dropCount = 0;

		// 落下処理を行ったかどうか
		bool isDrap = false;

	};

public:

	MainParam();
	virtual ~MainParam();

	void Init();

public:

	/// <summary>
	/// カウントダウン処理
	/// </summary>
	/// <returns>カウントを返す</returns>
	int CountDown();

	/// <summary>
	/// カウントダウンが終わったかどうかを返す
	/// </summary>
	/// <returns>カウントダウン終了フラグ</returns>
	bool GetIsCountDownEnd();
	

	/// <summary>
	/// キャラクターの体力を取得
	/// </summary>
	/// <param name="charNumber">キャラクターナンバー</param>
	/// <param name="lifePoint">体力</param>
	void SetCharLife(const int& charNumber, const int& lifePoint);

	/// <summary>
	/// キャラクターの体力を返す
	/// </summary>
	/// <param name="charNumber">キャラクターナンバー</param>
	/// <returns>ナンバーに対応した、体力を返す</returns>
	int GetCharLife(const int& charNumber) { return m_charInfo[charNumber].lifePoint; }

	/// <summary>
	/// キャラクターがダメージを受けたかどうかを取得
	/// </summary>
	/// <param name="charNumber">キャラクターナンバー</param>
	/// <param name="isDamage">ダメージフラグ</param>
	void SetIsCharDamage(const int& charNumber, const bool& isDamage);

	/// <summary>
	/// キャラクターがダメージを受けたかどうかを返す
	/// </summary>
	/// <param name="charNumber">キャラクターナンバー</param>
	/// <returns>ナンバーに対応した、ダメージフラグを返す</returns>
	bool GetIsCharDamage(const int& charNumber) { return m_charInfo[charNumber].isDamage; }
	



	/// <summary>
	/// 盤面のスケールを取得
	/// </summary>
	/// <param name="boardScale">盤面のスケール</param>
	void SetBoardScale(const int& boardScale) { m_board.scale = boardScale; }
	
	/// <summary>
	/// 盤面のスケールを返す
	/// </summary>
	/// <returns>盤面のスケール</returns>
	int GetBoardScale() { return  m_board.scale; }

	/// <summary>
	/// 盤面を落下させる回数
	/// </summary>
	void BoardDropCount();

	/// <summary>
	/// 盤面落下終了
	/// </summary>
	void BoardDropEnd();

	/// <summary>
	/// 落下回数を返す
	/// </summary>
	/// <returns>盤面の落下回数</returns>
	int GetBoardDropCount() { return m_board.dropCount; }

	/// <summary>
	/// 落下処理をした事を取得
	/// </summary>
	/// <param name="isDrap">落下フラグ</param>
	void SetIsDropBoard(const bool& isDrap) { m_board.isDrap = isDrap; }

	/// <summary>
	/// 落下処理をしたかどうかを返す
	/// </summary>
	/// <returns>落下しているフラグ</returns>
	bool GetIsDropBoard() {return m_board.isDrap; }



	/// <summary>
	/// ステートをセット
	/// </summary>
	/// <param name="state">ステート</param>
	void SetState(const MainScreen::State& state) { m_state = state; }


	/// <summary>
	/// ステートを返す
	/// </summary>
	/// <returns>ステート</returns>
	MainScreen::State GetState() { return m_state; }


private:

	
	/// <summary>
	/// 順位を計算する
	/// </summary>
	/// <returns>順位</returns>
	int RankCalculation();

private:

	//////////////
	// 登場関連 //
	//////////////

	// 登場関連
	Pop m_pop;


	//////////////////////////////
	// キャラパラメータ情報関連 //
	//////////////////////////////

	// キャラパラメータ情報
	std::vector<CharParam>m_charInfo;


	//////////////
	// 盤面関連 //
	//////////////

	// 盤面
	Board m_board;


	//////////////////
	// ステート関連 //
	//////////////////

	// ステート
	MainScreen::State m_state;
};

