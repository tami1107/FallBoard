#include "SoundSetting.h"
#include "EvoLib.h"
#include "Game.h"
#include "Pad.h"
#include "Sound.h"

// シングルトン
SoundSetting* SoundSetting::m_instance = nullptr;

namespace
{

	// グラフィックl
	struct GraphicInfo
	{
		// ファイルパス
		const char* filePath = "";

		// 座標
		Vec2 pos = Vec2(0.0f, 0.0f);

		// グラフィックの分割数
		const int div_x = 0;
		const int div_y = 0;

	};

	// グラフィックのファイルパス
	const std::vector<GraphicInfo> kGraphicInfo
	{
		// 背景
		{
			// グラフィックファイルパス
			"Data/Graphic/SoundSetting/background.png",

			// 座標
			Vec2(Game::kScreenCenterPos.x,Game::kScreenCenterPos.y-50),

		
			// グラフィックの分割数
			1,1,
		},

		// フレーム
		{
			// グラフィックファイルパス
			"Data/Graphic/SoundSetting/frame.png",

			// 座標
			Vec2(Game::kScreenCenterPos.x,Game::kScreenCenterPos.y),

			// グラフィックの分割数
			1,2,
		},

		// テキスト
		{
			// グラフィックファイル
			"Data/Graphic/SoundSetting/text.png",

			// 座標
			Vec2(Game::kScreenCenterPos.x - 300.0f,Game::kScreenCenterPos.y),


			// グラフィックの分割数
			1,2,
		},

		// スピーカー
		{
			// グラフィックファイルパス
			"Data/Graphic/SoundSetting/speaker.png",

			// 座標
			Vec2(Game::kScreenCenterPos.x - 120.0f,Game::kScreenCenterPos.y),

			// グラフィックの分割数
			2,1,
		},

		// 音量バー
		{
			// グラフィックファイルパス
			"Data/Graphic/SoundSetting/bar.png",

			// 座標
			Vec2(Game::kScreenCenterPos.x,Game::kScreenCenterPos.y),


			// グラフィックの分割数
			1,2,
		},

	};

	// グラフィック名
	enum class GraphName
	{
		Background,	// 背景
		Frame,		// フレーム
		Text,		// テキスト
		Speaker,	// スピーカー
		SoundBar,	// サウンドバー
	};

	// グラフィック幅
	constexpr int kGraphDistance = 90;


	// 音量バーの幅
	constexpr int kSoundBarDistance = 85;
}

namespace
{
	// 音量の最大
	constexpr int kSoundMaxNum = 5;

	// 項目選択時の移動インターバルの最大値
	constexpr int kIntervalMaximumValue = 10;

	// 選択項目の最大数
	constexpr int kSelectMaxNum = 2;
}

namespace
{
	// サウンドファイル名
	const std::vector<std::string> kSoundFileName =
	{
		"determination",// 決定音
	};

	// サウンド名
	enum class SoundName
	{
		Determination,	// 決定音
	};
}

void SoundSetting::Init()
{
	// ハンドルのロード
	LoadHandle();

	// 変数初期化
	m_bgmValue = kSoundMaxNum;
	m_seValue = kSoundMaxNum;
	m_seValue = kSoundMaxNum;
	m_selectNum = 0;
	m_selectInterval = kIntervalMaximumValue;

	// サウンドロード
	Sound::getInstance().Load(kSoundFileName);
}

void SoundSetting::UnLoad()
{
	// グラフィックの削除
	for (int i = 0; i < static_cast<int>(m_graph.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_graph[i].handle.size()); j++)
		{
			DeleteGraph(m_graph[i].handle[j]);
		}
	}

	// サウンドロード
	Sound::getInstance().UnLoad();
}

void SoundSetting::Update()
{
	// 選択
	Select();

	// 音量調整
	VolumeAdjustment();
}

void SoundSetting::Draw()
{
	// 画面を暗くする
	{
		// ポーズウィンドウセロファン(黒い)
		SetDrawBlendMode(DX_BLENDMODE_MULA, 100);

		// 全体的に暗くする
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight,
			0x000000, true);

		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}


	// 背景描画
	{
		DrawRotaGraphF(
			kGraphicInfo[static_cast<int>(GraphName::Background)].pos.x,
			kGraphicInfo[static_cast<int>(GraphName::Background)].pos.y,
			1.0, 0.0, m_graph[static_cast<int>(GraphName::Background)].handle[0], true);
	}


	{
		if (m_selectNum == 1)
		{
			// 透明度を設定する
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		}
		else
		{
			// 枠
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Frame)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Frame)].pos.y - kGraphDistance,
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Frame)].handle[1], true);
			}

			// SetDrawBrightを使用して、カラーをセットする
			SetDrawBright(255, 100, 0);
		}
	

		// テキスト
		{
			DrawRotaGraphF(
				kGraphicInfo[static_cast<int>(GraphName::Text)].pos.x,
				kGraphicInfo[static_cast<int>(GraphName::Text)].pos.y - kGraphDistance,
				1.0, 0.0, m_graph[static_cast<int>(GraphName::Text)].handle[0], true);
		}

		// スピーカー
		{
			if (m_bgmValue != 0)
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.y - kGraphDistance, 
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Speaker)].handle[0], true);
			}
			else
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.y - kGraphDistance, 
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Speaker)].handle[1], true);
			}
		}

		// 音量
		{
			for (int i = kSoundMaxNum - 1; i >= 0; --i)
			{
				if ((m_bgmValue) <= i)
				{
					DrawRotaGraphF(
						kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.x + (kSoundBarDistance * i), 
						kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.y - kGraphDistance,
						0.8, 0.0, m_graph[static_cast<int>(GraphName::SoundBar)].handle[1], true);

					continue;
				}
			}

			for (int i = 0; i < m_bgmValue; i++)
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.x + (kSoundBarDistance * i),
					kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.y - kGraphDistance,
					0.8, 0.0, m_graph[static_cast<int>(GraphName::SoundBar)].handle[0], true);
			}
		}

		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// SetDrawBrightを使用し、カラーをもとに戻す
		SetDrawBright(255, 255, 255);
	}

	{
		if (m_selectNum == 0)
		{
			// 透明度を設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		}
		else
		{

			// 枠
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Frame)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Frame)].pos.y + kGraphDistance,
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Frame)].handle[1], true);
			}

			// カラーを設定
			SetDrawBright(255, 100, 0);
		}

		// テキスト
		{
			DrawRotaGraphF(
				kGraphicInfo[static_cast<int>(GraphName::Text)].pos.x,
				kGraphicInfo[static_cast<int>(GraphName::Text)].pos.y + kGraphDistance,
				1.0, 0.0, m_graph[static_cast<int>(GraphName::Text)].handle[1], true);
		}

		// スピーカー
		{
			if (m_seValue != 0)
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.y + kGraphDistance,
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Speaker)].handle[0], true);
			}
			else
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.x,
					kGraphicInfo[static_cast<int>(GraphName::Speaker)].pos.y + kGraphDistance,
					1.0, 0.0, m_graph[static_cast<int>(GraphName::Speaker)].handle[1], true);
			}
			
		}

		// 音量バー
		{

			for (int i = kSoundMaxNum - 1; i >= 0; --i)
			{
				if ((m_seValue) <= i)
				{
					DrawRotaGraphF(
						kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.x + (kSoundBarDistance * i),
						kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.y + kGraphDistance,
						0.8, 0.0, m_graph[static_cast<int>(GraphName::SoundBar)].handle[1], true);

					continue;
				}
			}

			for (int i = 0; i < m_seValue; i++)
			{
				DrawRotaGraphF(
					kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.x + (kSoundBarDistance * i),
					kGraphicInfo[static_cast<int>(GraphName::SoundBar)].pos.y + kGraphDistance,
					0.8, 0.0, m_graph[static_cast<int>(GraphName::SoundBar)].handle[0], true);
			}
		}

		// 描画ブレンドモードをノーブレンドにする
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// SetDrawBrightを使用し、カラーをもとに戻す
		SetDrawBright(255, 255, 255);
	}
}

void SoundSetting::LoadHandle()
{
	// グラフィック変数の要素を増やす
	m_graph.resize(static_cast<int>(kGraphicInfo.size()));

	// ハンドルのロード
	for (int i = 0; i < static_cast<int>(kGraphicInfo.size()); i++)
	{
		// グラフィック情報代入
		Load_Lib::DivGraphInfo graphInfo = Load_Lib::LoadDivGraph_EvoLib(kGraphicInfo[i].filePath, kGraphicInfo[i].div_x, kGraphicInfo[i].div_y);

		// グラフィック代入
		m_graph[i].handle = graphInfo.handle;

		// 座標代入
		m_graph[i].pos = kGraphicInfo[i].pos;
	}

}

void SoundSetting::Select()
{
	// 選択の移動インターバル処理
	{
		m_selectInterval--;

		if (m_selectInterval <= 0)
		{
			m_selectInterval = 0;
		}
	}

	// インターバルが0以上ならばここで処理を終了する
	if (m_selectInterval != 0)return;

	if (Pad::IsPress(PAD_INPUT_DOWN))
	{
		// 値をループさせる
		m_selectNum = Calculation_Lib::SelectLoopNumber(0, kSelectMaxNum, m_selectNum);

		// 選択SEを流す
		Sound::getInstance().Play(kSoundFileName[0]);

		// m_selectIntervalにkIntervalMaximumValueの値を代入することによって、m_selectIntervalの値が0になるまでm_selectNumberを変更できなくする
		m_selectInterval = kIntervalMaximumValue;
	}
	else if (Pad::IsPress(PAD_INPUT_UP))
	{
		// 値を逆ループさせる
		m_selectNum = Calculation_Lib::SelectLoopNumber(0, kSelectMaxNum, m_selectNum, true);

		// 選択SEを流す
		Sound::getInstance().Play(kSoundFileName[0]);

		// m_selectIntervalにkIntervalMaximumValueの値を代入することによって、m_selectIntervalの値が0になるまでm_selectNumberを変更できなくする
		m_selectInterval = kIntervalMaximumValue;
	}

}

void SoundSetting::VolumeAdjustment()
{

	// パーセントボリューム変数
	int percentValue = 0;

	if (m_selectNum == 0)
	{
		

		if (Pad::IsTrigger(PAD_INPUT_RIGHT))
		{
			// 値をループさせる
			m_bgmValue = Calculation_Lib::SelectLoopNumber(0, kSoundMaxNum + 1, m_bgmValue);			
		
			// パーセントボリュームを調べる
			percentValue = Convert_Lib::PercentageRate(static_cast<float>(kSoundMaxNum), static_cast<float>(m_bgmValue));

			// 音量を設定する
			Sound::getInstance().SetSoundVolume(Sound::SoundTypeInfo::BGM, percentValue);

			// 決定音を再生
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);
		}
		else if (Pad::IsTrigger(PAD_INPUT_LEFT))
		{
			// 値を逆ループさせる
			m_bgmValue = Calculation_Lib::SelectLoopNumber(0, kSoundMaxNum + 1, m_bgmValue, true);
		
			// パーセントボリュームを調べる
			percentValue = Convert_Lib::PercentageRate(static_cast<float>(kSoundMaxNum), static_cast<float>(m_bgmValue));

			// 音量を設定する
			Sound::getInstance().SetSoundVolume(Sound::SoundTypeInfo::BGM, percentValue);

			// 決定音を再生
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);
		}
	}
	else
	{
		if (Pad::IsTrigger(PAD_INPUT_RIGHT))
		{

			// 値をループさせる
			m_seValue = Calculation_Lib::SelectLoopNumber(0, kSoundMaxNum + 1, m_seValue);

			// パーセントボリュームを調べる
			percentValue = Convert_Lib::PercentageRate(static_cast<float>(kSoundMaxNum), static_cast<float>(m_seValue));

			// 音量を設定する
			Sound::getInstance().SetSoundVolume(Sound::SoundTypeInfo::SE, percentValue);


			// 決定音を再生
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);
		}
		else if (Pad::IsTrigger(PAD_INPUT_LEFT))
		{
			// 値を逆ループさせる
			m_seValue = Calculation_Lib::SelectLoopNumber(0, kSoundMaxNum + 1, m_seValue, true);
			
			// パーセントボリュームを調べる
			percentValue = Convert_Lib::PercentageRate(static_cast<float>(kSoundMaxNum), static_cast<float>(m_seValue));

			// 音量を設定する
			Sound::getInstance().SetSoundVolume(Sound::SoundTypeInfo::SE, percentValue);


			// 決定音を再生
			Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::Determination)]);
		}
	}

}
