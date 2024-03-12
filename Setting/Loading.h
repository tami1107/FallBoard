#pragma once


class Loading
{
public:
	Loading();
	virtual ~Loading();

	void Init();
	void Update();
	void Draw();

	

	/// <summary>
	/// ロード中かどうかを返す
	/// </summary>
	/// <returns>ロード中かどうか</returns>
	bool GetIsLoading();


private:

	/// <summary>
	/// ハンドルのロード処理
	/// </summary>
	void LoadHanele();

	/// <summary>
	/// フェード値更新処理
	/// </summary>
	void UpdateFadeBright();

	/// <summary>
	/// ロードフレーム処理
	/// </summary>
	void LoadFrame();

	/// <summary>
	/// 非同期処理数を数える処理
	/// </summary>
	void ASyncLoadingCount();

	/// <summary>
	/// ロードグラフィックを描画する
	/// </summary>
	void DrawLoadGaph();

public:

	// ステート
	enum class State
	{
		FadeIn,	// フェードイン
	};

private:

	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// ハンドル
	int m_handle;

	// サインフレーム
	int m_sinFrame;

	// フェード値
	int m_fadeBright;

	////////////////
	// ロード関連 //
	////////////////

	// フレームカウント
	int m_frameCount;

	// 最低限のロードフレームを超えたかどうか
	bool m_isMinimumFrame;

	// ロード中かどうか
	bool m_isLoading;
};