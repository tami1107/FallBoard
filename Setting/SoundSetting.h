#pragma once
#include <vector>
#include "Vec2.h"

class SoundSetting
{

private:
	SoundSetting() = default;
	virtual ~SoundSetting() = default;

	static SoundSetting* m_instance;

public:
	// コピーコンストラクタの禁止
	SoundSetting(const SoundSetting&) = delete;
	SoundSetting& operator=(const SoundSetting&) = delete;
	SoundSetting(SoundSetting&&) = delete;
	SoundSetting& operator=(SoundSetting&&) = delete;

	static SoundSetting& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new SoundSetting;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	// 初期化処理
	void Init();

	// 削除
	void UnLoad();

	// 更新
	void Update();

	// 描画
	void Draw();

	
private:

	// ハンドルのロード
	void LoadHandle();

	// 選択
	void Select();

	// 音量調整
	void VolumeAdjustment();

private:

	// グラフィック
	struct Graphic
	{
		// グラフィックハンドル
		std::vector<int>handle;

		// 座標
		Vec2 pos = Vec2(0.0f, 0.0f);
	};

private:

	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// グラフィック
	std::vector<Graphic>m_graph;

	//////////////
	// 音量関連 //
	//////////////


	// BGM音量
	int m_bgmValue = 0;

	// SE音量
	int m_seValue = 0;

	//////////////////
	// セレクト関連 //
	//////////////////

	// 選択番号
	int m_selectNum = 0;

	// 選択フレーム
	int m_selectInterval = 0;

};

