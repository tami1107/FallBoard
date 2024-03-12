#include "Sound.h"
#include <DxLib.h>

#include <cassert>
#include <fstream>
#include <sstream>
#include "EvoLib.h"

Sound* Sound::m_instance = nullptr;

namespace
{
	// サウンドのファイルパス
	std::string  kDataFilePaht = "Data/Sound/";

	// 音量の最大値
	constexpr int kSoundMaxValue = 255;

	// inputした文字列をdelimiterで分割して
	// 分割後の文字列を返す
	std::vector<std::string> split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);	// stringをgetlineで使えるデータに変換？
		std::string field;					// 分割した文字列1つ文を格納する？
		std::vector<std::string> result;	// 分割後の文字列の配列

		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}
}

void Sound::Init()
{
	// ファイル情報の読み込み
	std::ifstream ifs("Data/csv/ChangeableData/Sound.csv");
	std::string line;

	// 行カウント
	int lineCount = 0;

	while (getline(ifs, line))
	{
		// 行カウントを加算
		lineCount++;

		// 一行目は説明のため処理を飛ばす
		if (lineCount <= 1)continue;


		//printfDx("%s\n", line.c_str());	// test 1行表示
		// csvデータ1行を','で複数の文字列に分割
		std::vector<std::string>	strvec = split(line, ',');

		// strvec[0]	: ファイル名	string
		// strvec[1]	: 拡張子		string
		// strvec[2]	: BGMフラグ		int(boolに変換したい)
		// strvec[3]	: 音量			float

		// 文字列を適切なデータ型に変換して格納
		Data data;

		// ファイル名を代入
		std::string fileName = strvec[static_cast<int>(File::FileName)];
		std::string fileExtension = strvec[static_cast<int>(File::FileExtension)];
		data.filePath = kDataFilePaht + fileName + fileExtension;


		// BGMかどうかを判定
		if (strvec[static_cast<int>(File::SoundType)] == "BGM")
		{
			data.isBgm = true;
		}
		else
		{
			data.isBgm = false;
		}

		// ボリューム調整
		data.volRate = stof(strvec[static_cast<int>(File::SoundVolume)]);

		// 最大ボリューム
		data.maxVolume = Convert_Lib::ConvertingValueToPercent(kSoundMaxValue, data.volRate);
		
		// 最大ボリュームをボリュームに代入
		data.soundVolume = data.maxVolume;

		// ボリュームを設定ボリュームに代入
		data.settingSoundVolume = data.soundVolume;

		// ハンドルの初期化
		data.handle = -1;

		m_data[strvec[0]] = data;
	}
}

void Sound::Load(const std::vector<std::string>& fileName, bool ASyncLoad)
{
	// 非同期処理を行うかどうか
	if (ASyncLoad)
	{
		// リソースの読み込み
		SetUseASyncLoadFlag(true);	// 非同期読み込みを行いたい
	}

	// サウンドの読み込み
	for (auto& file : fileName)
	{
		// ハンドルを代入する
		m_data[file].handle = LoadSoundMem(m_data[file].filePath.c_str());

		// 音量を代入する
		m_data[file].soundVolume = m_data[file].settingSoundVolume;

		// 音量設定
		ChangeVolumeSoundMem(m_data[file].soundVolume, m_data[file].handle);
	}

	// リソースの読み込み
	SetUseASyncLoadFlag(false);	// デフォルトに戻しておく
}

void Sound::UnLoad()
{
	// すべてのハンドルを削除
	InitSoundMem();
}

void Sound::Play(const std::string& filename)
{
	// サウンドのプレイタイプ
	int playType = 0;

	if (m_data[filename].isBgm)
	{
		playType = DX_PLAYTYPE_LOOP;
	}
	else
	{
		playType = DX_PLAYTYPE_BACK;
	}

	// 音量設定
	ChangeVolumeSoundMem(m_data[filename].soundVolume, m_data[filename].handle);

	// サウンドを鳴らす
	PlaySoundMem(m_data[filename].handle, playType);
}

void Sound::ScreenFadeBGMStop(const int& fadeValue)
{
	// フェードの最大値
	const int fadeMaxValue = 255;

	// フェード値の計算
	const int fadeValueReverse = fadeMaxValue - fadeValue;

	// フェード値を百分率で計算
	const int fadePercent= Convert_Lib::PercentageRate(static_cast<float>(fadeMaxValue), static_cast<float>(fadeValueReverse));

	// サウンドの読み込み
	for (auto& data : m_data)
	{
		// BGMでなかった場合、コンティニューする
		if (!data.second.isBgm)continue;

		// サウンドボリュームを求める
		data.second.soundVolume = Convert_Lib::ConvertingFromPercentToValue(data.second.settingSoundVolume, static_cast<float>(fadePercent));
		
		// 音量設定
		ChangeVolumeSoundMem(data.second.soundVolume, data.second.handle);
	}
}

void Sound::SetSoundVolume(const SoundTypeInfo& soundType, const int& soundPercentVolume)
{
	// データ内のサウンドタイプ
	SoundTypeInfo dataSoundType = SoundTypeInfo::BGM;

	// サウンドの読み込み
	for (auto& data : m_data)
	{
		// そのサウンドがBGMかSEかを調べ、サウンドタイプに代入する
		if (data.second.isBgm)
		{
			dataSoundType = SoundTypeInfo::BGM;
		}
		else
		{
			dataSoundType = SoundTypeInfo::SE;
		}

		// サウンドタイプが同じではない場合、continueする
		if (soundType != dataSoundType)continue;

		// サウンド音量を調べる
		const int soundVolume = Convert_Lib::ConvertingFromPercentToValue(data.second.maxVolume, static_cast<float>(soundPercentVolume));

		// 設定ボリュームに調べたサウンドボリュームを代入する
		data.second.settingSoundVolume = soundVolume;

		// ボリュームに設定ボリュームを代入する
		data.second.soundVolume = data.second.settingSoundVolume;

		// 音量設定
		ChangeVolumeSoundMem(data.second.soundVolume, data.second.handle);
	}
}

