#pragma once

namespace Pad
{
	// パッドの入力状態取得
	void Update();

	/// <summary>
	/// 押し下げ判定
	/// </summary>
	/// <param name="button">どのボタンか</param>
	/// <param name="padNo">パッドの番号/param>
	/// <returns>ボタンが押し下げられているかどうかを返す</returns>
	bool IsPress(int button, int padNo = 0);

	
	/// <summary>
	/// トリガー判定
	/// </summary>
	/// <param name="button">どのボタンか</param>
	/// <param name="padNo">パッドの番号</param>
	/// <returns>ボタンのトリガー判定を返す</returns>
	bool IsTrigger(int button, int padNo = 0);


	/// <summary>
	/// パッドの振動を開始する
	/// </summary>
	/// <param name="power">振動の強さ(0～1000)</param>
	/// <param name="frameTime">振動するフレーム数(値に-1.0fを代入した場合、StopVibration()が呼び出されるまで振動し続ける)</param>
	/// <param name="padNo">パッドの番号</param>
	void StartingVibration(const int& power, float frameTime = -1, int padNo = 0);

	/// <summary>
	/// パッドの振動を止める
	/// </summary>
	/// <param name="padNo"></param>
	void StopVibration(int padNo = 0);

}
