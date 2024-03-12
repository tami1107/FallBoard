#pragma once
class TitleScreen
{
public:
	TitleScreen();
	virtual ~TitleScreen();

	void Init();
	void Update();
	void Draw();

private:

	//////////////////////
	// グラフィック関連 //
	//////////////////////

	// ハンドル
	int m_handle;

	// フレーム
	int m_frame;
};