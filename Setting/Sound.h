#pragma once
#include "game.h"

#include <array>
#include <vector>
#include <string>
#include <map>

// シングルトンを使用したサウンドクラス
class Sound
{
public:
	enum Id
	{
		Bgm,
	};
private:
	Sound() = default;
	virtual ~Sound() = default;

	static Sound* m_instance;

public:
	// コピーコンストラクタの禁止
	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;
	Sound(Sound&&) = delete;
	Sound& operator=(Sound&&) = delete;

	static Sound& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new Sound;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	// サウンドタイプ情報
	enum class SoundTypeInfo
	{
		BGM,// BGM
		SE,	// SE
	};


private:

	// ファイル
	enum class File
	{
		FileName,		// ファイル名
		FileExtension,	// 拡張子
		SoundType,		// サウンドのタイプ
		SoundVolume,	// サウンドの音量
	};

	// サウンドデータ
	struct Data
	{
		bool isBgm = false;			// これがBGMか効果音か
		float volRate = 0.0f;		// ボリューム調整
		int maxVolume = 0;			// 最大ボリューム
		int soundVolume = 0;		// サウンドボリューム
		int settingSoundVolume = 0;	// 設定したサウンドボリューム
		int handle = -1;			// サウンドハンドル
		std::string filePath;		// ファイルパス
	};

	
public:
	// 初期処理
	void Init();

	/// <summary>
	/// サウンドロード
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="ASyncLoad">非同期処理を行うかどうか</param>
	void Load(const std::vector<std::string>& fileName, bool ASyncLoad = false);

	/// <summary>
	/// サウンドアンロード
	/// </summary>
	void UnLoad();

	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="filename">ファイル名</param>
	void Play(const std::string& filename);

	/// <summary>
	/// 画面のフェードに合わせてBGMを消す
	/// </summary>
	/// <param name="fadeValue">これを書く→FadeBrightValue()</param>
	void ScreenFadeBGMStop(const int& fadeValue);

	/// <summary>
	/// サウンド音量設定
	/// </summary>
	/// <param name="soundType">サウンドタイプ</param>
	/// <param name="soundPercentVolume">サウンドパーセントボリューム</param>
	void SetSoundVolume(const SoundTypeInfo& soundType, const int& soundPercentVolume);

private:
	// 音声データ
	std::map<std::string, Data>	m_data;
};

