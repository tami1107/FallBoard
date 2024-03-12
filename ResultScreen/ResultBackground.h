#pragma once
#include <vector>
#include "Vec2.h"
#include <memory>


class Field;
class ResultCharacter;
class ResultBackground
{
public:
	ResultBackground();
	virtual~ResultBackground();

	/// <summary>
	/// リザルトキャラクターポインタ取得
	/// </summary>
	/// <param name="resultCharacter">リザルトキャラクターポインタ</param>
	void SetResultCharacterPointer(std::shared_ptr<ResultCharacter>resultCharacter) { m_pResultCharacter = resultCharacter; }

	void Init();
	void Update();
	void Draw();

private:

	/// <summary>
	/// エフェクト初期化処理
	/// </summary>
	void InitEffect();

	/// <summary>
	/// エフェクト更新処理
	/// </summary>
	void UpdateEffect();

	/// <summary>
	/// 背景描画処理
	/// </summary>
	void DrawBackground();

	/// <summary>
	/// リザルトテキスト描画
	/// </summary>
	void DrawResultText();

public:

	/// <summary>
	/// ハンドルのロード処理
	/// </summary>
	void LoadHandle();

	
	void DrawEffect();

	/// <summary>
	/// ランキング描画処理
	/// </summary>
	void DrawRanking();

	/// <summary>
	/// クラウン描画処理
	/// </summary>
	void DrawCrown();

	/// <summary>
	/// 次のシーンの説明テキストを描画
	/// </summary>
	void DrawNextSceneText();
private:

	// グラフィック
	struct Graphic
	{
		// グラフィックハンドル
		std::vector<int>handle;

		// 座標
		Vec2 pos = Vec2(0.0f, 0.0f);
	};

	// エフェクト情報
	struct EffectInfo
	{
		// 移動量
		Vec2 vec = Vec2(0.0f, 0.0f);
		// 座標
		Vec2 pos = Vec2(0.0f, 0.0f);
	};


private:

	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// グラフィック情報
	std::vector<Graphic>m_graph;


	//////////////
	// 演出関連 //
	//////////////

	// 背景グラフィックの移動量
	float m_backgroundMoveValue;

	// リザルトテキストのサインフレーム
	int m_resultTextSinFrame;

	// クラウングラフィックのサインフレーム
	int m_crownSinFrame;

	// 点滅フレーム
	int m_flashingFrame;

	// エフェクト情報
	std::vector<EffectInfo>m_effect;


	////////////////////////
	// クラスポインタ関連 //
	////////////////////////

	// フィールドポインタ
	std::shared_ptr<Field>m_pField;

	// リザルトキャラクターポインタ
	std::shared_ptr<ResultCharacter>m_pResultCharacter;
};