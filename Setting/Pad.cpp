#include "DxLib.h"
#include "Pad.h"
#include <vector>
#include "EvoLib.h"

namespace
{
	// ログ記録フレーム数
	constexpr int kLogNum = 16;
	// パッド最大数
	constexpr int kMaxPad = 4;
	// 使用するパッド名
	constexpr int kUsePadData[kMaxPad] =
	{
		DX_INPUT_KEY_PAD1,
		DX_INPUT_PAD2,
		DX_INPUT_PAD3,
		DX_INPUT_PAD4,
	};

	// 入力ログ	0が最新の状態
	int padLog[kMaxPad][kLogNum];
}

namespace Pad
{
	// パッドの入力状態取得
	void Update()
	{
		for (int padNo = 0; padNo < kMaxPad; padNo++)
		{
			// 現在のパッドの状態を取得
			int padState = GetJoypadInputState(kUsePadData[padNo]);

			// ログの更新
			for (int i = kLogNum - 1; i >= 1; i--)
			{
				padLog[padNo][i] = padLog[padNo][i - 1];
			}

			
			// 最新の状態
			padLog[padNo][0] = padState;
		}
	}

	// 押し下げ判定
	bool IsPress(int button, int padNo)
	{
		return (padLog[padNo][0] & button);
	}
	// トリガー判定
	bool IsTrigger(int button, int padNo)
	{
		bool isNow = (padLog[padNo][0] & button);	// 現在の状態
		bool isLast = (padLog[padNo][1] & button);	// １フレーム前の状態
		return (isNow && !isLast);
	}

	// 振動
	void StartingVibration(const int& power, float frameTime, int padNo)
	{
		// ミリ秒単位を求める
		int millisecondUnit = Convert_Lib::ConvertingFrameCountToMillisecondUnit(static_cast<int>(frameTime));

		// フレームタイムが-1.0fならばミリ秒単位も-1にする
		if (frameTime == -1.0f)
		{
			millisecondUnit = -1;
		}

		// 振動を行う
		StartJoypadVibration(kUsePadData[padNo], power, millisecondUnit, -1);
	}

	// 振動を止める
	void StopVibration(int padNo)
	{
		// パッドの振動を止める
		StopJoypadVibration(kUsePadData[padNo], -1);
	}
}