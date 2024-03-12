#include <DxLib.h>
#include "Game.h"

#include "SceneManager.h"

#include "Sound.h"
#include "ModelData.h"
#include "EvoLib.h"
#include "SoundSetting.h"
#include "MasterData.h"


// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// windowモード設定
	ChangeWindowMode(Game::kWindowMode);
	// ウインドウ名設定
	SetMainWindowText(Game::kTitleText);
	// 
	SetWindowSizeChangeEnableFlag(true);
	// 画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);




	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}



	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// ゲームデータのロード
	MasterData::getInstance().LoadData();

	// サウンドの初期処理
	Sound::getInstance().Init();

	// サウンド設定の初期処理
	SoundSetting::getInstance().Init();

	// モデルのロード(非同期読み込み)
	ModelData::getInstance().Load();


	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBufferFlag(TRUE);		// Ｚバッファを使用する
	SetUseBackCulling(TRUE);		// バックカリングを行う

	// 最初のシーンの初期化
	SceneManager scene;
	scene.Init();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		scene.Update();
		scene.Draw();

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < FPS_Lib::Dxlib_60FPS)
		{
		}
	}
	// サウンドの終了
	Sound::getInstance().UnLoad();
	Sound::destroy();

	ModelData::getInstance().UnLoad();

	SoundSetting::getInstance().UnLoad();


	scene.End();			// シーンの終了処理

	DxLib_End();			// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}