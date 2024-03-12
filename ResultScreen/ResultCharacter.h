#pragma once
#include <Dxlib.h>
#include <vector>
#include "ModelData.h"

class ResultCharacter
{
public:
	ResultCharacter();

	virtual~ResultCharacter();

	void Init();
	void Update();
	void Draw();

	/// <summary>
	/// モデルの座標を返す
	/// </summary>
	/// <returns>モデルの座標</returns>
	std::vector<VECTOR> GetModelPos() {return m_pos; }

private:

	/// <summary>
	/// 初期座標処理
	/// </summary>
	void InitPos();

	/// <summary>
	/// モーション更新処理
	/// </summary>
	void UpdateMotion();

	/// <summary>
	/// モデル描画処理
	/// </summary>
	void DrawModel();

private:

	////////////////
	// モデル関連 //
	////////////////

	// 座標
	std::vector<VECTOR>m_pos;

	// モデルデータ設定
	std::vector<ModelData::DrawModelData> m_modelData;

	////////////////////
	// モーション関連 //
	////////////////////

	// 角度
	int m_angle;
};