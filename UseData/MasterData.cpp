#include "MasterData.h"
#include "EvoLib.h"
#include "string"

// キャラクター
namespace Character
{
	// キャラクターステータスのファイルパス
	const char* const kStatusFilePath = "Data/csv/ChangeableData/CharacterStatus.csv";

	// キャラクターカラーのファイルパス
	const char* const kColorFilePath = "Data/csv/ChangeableData/CharacterColor.csv";

	// キャラクターモデル情報ファイルパス
	const char* const kModelInfoFilePath = "Data/csv/ChangeableData/ModelInformation.csv";
}

// エフェクト
namespace Effect
{
	// 攻撃エフェクト情報のファイルパス
	const char* const kAttackEffectInfoFilePath = "Data/csv/ChangeableData/AttackEffectInfo.csv";

	// 復活エフェクト情報のファイルパス
	const char* const kResurrectionEffectPath = "Data/csv/ChangeableData/ResurrectionEffectInfo.csv";
}

// フィールド
namespace Field
{
	// フィールドモデル情報ファイルパス
	const char* const kModelInfoFilePath = "Data/csv/ChangeableData/FieldCellInfo.csv";

	// フィールドステータスのファイルパス
	const char* const kStatusFilePath = "Data/csv/ChangeableData/FieldStatus.csv";

	// フィールドモデルカラーのファイルパス
	const char* const kModelColorFilePath = "Data/csv/ChangeableData/FieldColor.csv";
}


MasterData* MasterData::m_instance = nullptr;



void MasterData::LoadData()
{
	// キャラクターステータスのロード処理
	LoadCharcterStatus();

	// 攻撃エフェクト情報のロード処理
	LoadAtackEffectInfo();

	// 復活エフェクト情報のロード処理
	LoadResurrectionEffectInfo();

	// フィールド情報のロード処理
	LoadFieldInfo();

	// モデル情報のロード処理
	LoadModelInfo();
}

void MasterData::LoadCharcterStatus()
{
	// キャラステート名
	enum class CharStateName
	{
		HitPoint,		// 最大体力
		MoveSpeed,		// 移動スピード
		AttackRigorTime,// 攻撃硬直時間
		InvincibleTime,	// 復帰時の無敵時間
		Color,			// 色
	};

	// キャラクターステータスデータ取得
	const std::vector<std::vector<std::string>>charcterStatusData = File_Lib::CsvFileLoading_Revision(Character::kStatusFilePath, File_Lib::LoadType::SkipOneColumn);

	// プレイヤーの最大体力
	m_charcterStatus.lifePoint = static_cast<int> (Convert_Lib::ConvertingStringToFloat(charcterStatusData[static_cast<int>(CharStateName::HitPoint)][0]));

	// 移動速度
	m_charcterStatus.moveSpeed = Convert_Lib::ConvertingStringToFloat(charcterStatusData[static_cast<int>(CharStateName::MoveSpeed)][0]);

	// 攻撃硬直時間
	m_charcterStatus.attackRigorTime = static_cast<int> (Convert_Lib::ConvertingStringToFloat(charcterStatusData[static_cast<int>(CharStateName::AttackRigorTime)][0]));

	// 復帰時の無敵時間
	m_charcterStatus.invincibleTime = static_cast<int> (Convert_Lib::ConvertingStringToFloat(charcterStatusData[static_cast<int>(CharStateName::InvincibleTime)][0]));



	// カラー名
	enum class ColorName
	{
		Red,	// 赤
		Green,	// 緑
		Blue,	// 青
	};

	// キャラクターカラーデータ取得
	const std::vector<std::vector<std::string>>charcterColorData = File_Lib::CsvFileLoading_Revision(Character::kColorFilePath, File_Lib::LoadType::DoubleSkip);

	// カラー
	VECTOR color;

	// キャラクターカラーを代入
	for (int i = 0; i < static_cast<int>(charcterColorData.size()); i++)
	{
		// カラーを代入
		color.x = Convert_Lib::ConvertingStringToFloat(charcterColorData[i][static_cast<int>(ColorName::Red)]);
		color.y = Convert_Lib::ConvertingStringToFloat(charcterColorData[i][static_cast<int>(ColorName::Green)]);
		color.z = Convert_Lib::ConvertingStringToFloat(charcterColorData[i][static_cast<int>(ColorName::Blue)]);

		// m_charcterStatusにカラー情報を入れる
		m_charcterStatus.color.push_back(color);
	}
}

void MasterData::LoadAtackEffectInfo()
{
	// 攻撃エフェクト情報名
	enum class AttackEffectInfoName
	{
		FilePath,		// グラフィックファイルパス
		GraphScale,		// グラフィックスケール
		UsePlayerNumber,// 使用するプレイヤー番号
	};

	// 攻撃エフェクトデータ取得
	const std::vector<std::vector<std::string>>attackEffectInfoData = File_Lib::CsvFileLoading_Revision(Effect::kAttackEffectInfoFilePath, File_Lib::LoadType::SkipFirstLine);

	// 行数を代入
	const int lineCount = static_cast<int>(attackEffectInfoData.size());
	
	// 一時保存
	FileReadAttackEffectInfo saveAttackEffectInfo;
	
	// 行数分、処理を繰り返す
	for (int i = 0; i < lineCount; i++)
	{
		// ファイルパス代入
		saveAttackEffectInfo.filePath = attackEffectInfoData[i][static_cast<int> (AttackEffectInfoName::FilePath)];
			
		// グラフィックスケール代入
		saveAttackEffectInfo.scale =
			Convert_Lib::ConvertingStringToFloat(attackEffectInfoData[i][static_cast<int> (AttackEffectInfoName::GraphScale)]);

		// 使用するプレイヤー番号を代入
		saveAttackEffectInfo.usePlayerNumber =
			static_cast<int>(Convert_Lib::ConvertingStringToFloat(attackEffectInfoData[i][static_cast<int> (AttackEffectInfoName::UsePlayerNumber)]));

		// 一時保存したデータを要素に追加する
		m_attackEffectInfo.push_back(saveAttackEffectInfo);
	}
}

void MasterData::LoadResurrectionEffectInfo()
{
	// 復活エフェクト情報名
	enum class ResurrectionEffectInfoName
	{
		FilePath,		// グラフィックファイルパス
		GraphScale,		// グラフィックスケール
	};


	// 復活エフェクトデータ取得
	const std::vector<std::vector<std::string>>resurrectionEffectInfoData = File_Lib::CsvFileLoading_Revision(Effect::kResurrectionEffectPath, File_Lib::LoadType::SkipFirstLine);

	// 行数
	const int lineNum = 0;

	// ファイルパス代入
	m_resurrectionEffectInfo.filePath = 
		resurrectionEffectInfoData[lineNum][static_cast<int> (ResurrectionEffectInfoName::FilePath)];

	// グラフィックスケール代入
	m_resurrectionEffectInfo.scale =
		Convert_Lib::ConvertingStringToFloat(resurrectionEffectInfoData[lineNum][static_cast<int> (ResurrectionEffectInfoName::GraphScale)]);

}

void MasterData::LoadFieldInfo()
{
	// モデル情報
	{
		// フィールドモデル情報名
		enum class FieldModelInfoName
		{
			FilePath,				// ファイルパス
			ModelScale,				// モデルスケール
		};

		// フィールドモデル情報取得
		const std::vector<std::vector<std::string>>fieldModelInfoData = File_Lib::CsvFileLoading_Revision(Field::kModelInfoFilePath, File_Lib::LoadType::SkipFirstLine);

		// 行数
		const int lineNum = 0;


		// フィールドのモデルパスを代入
		m_fieldInfo.filePath = fieldModelInfoData[lineNum][static_cast<int>(FieldModelInfoName::FilePath)];

		// モデルのスケールを取得
		m_fieldInfo.modelScale = Convert_Lib::ConvertingStringToFloat(fieldModelInfoData[lineNum][static_cast<int>(FieldModelInfoName::ModelScale)]);
	}

	// フィールドステータス
	{
		// フィールドステータス名
		enum class FieldStatusName
		{
			FallInterval,			// 落下インターバル
			DifferenceFallInterval,	// 落下間隔
			DeleteFallColorInterval,// 盤面落下カラーインターバル
			FallSpeed,				// 落下スピード
			FallLowerLimitPos,		// 落下下限位置
		};

		// フィールドステータス情報取得
		const std::vector<std::vector<std::string>>fieldStatusData = File_Lib::CsvFileLoading_Revision(Field::kStatusFilePath, File_Lib::LoadType::SkipOneColumn);

		// 列数
		const int columnNum = 0;

		// 落下インターバル代入
		m_fieldInfo.fallInterval = static_cast<int>(Convert_Lib::ConvertingStringToFloat(fieldStatusData[static_cast<int>(FieldStatusName::FallInterval)][columnNum]));

		// 落下間隔
		m_fieldInfo.differenceFallInterval= static_cast<int>(Convert_Lib::ConvertingStringToFloat(fieldStatusData[static_cast<int>(FieldStatusName::DifferenceFallInterval)][columnNum]));

		// 盤面縮小落下カラーインターバル
		m_fieldInfo.deleteFallColorInterval = static_cast<int>(Convert_Lib::ConvertingStringToFloat(fieldStatusData[static_cast<int>(FieldStatusName::DeleteFallColorInterval)][columnNum]));

		// 落下スピード
		m_fieldInfo.fallSpeed = Convert_Lib::ConvertingStringToFloat(fieldStatusData[static_cast<int>(FieldStatusName::FallSpeed)][columnNum]);

		// 落下下限位置
		m_fieldInfo.fallLowerLimitPos = Convert_Lib::ConvertingStringToFloat(fieldStatusData[static_cast<int>(FieldStatusName::FallLowerLimitPos)][columnNum]);
	}

	// カラー
	{

		// カラー名
		enum class ColorName
		{
			Red,	// 赤
			Green,	// 緑
			Blue,	// 青
		};

		// フィールドカラー情報取得
		const std::vector<std::vector<std::string>>fieldColorInfoData = File_Lib::CsvFileLoading_Revision(Field::kModelColorFilePath, File_Lib::LoadType::DoubleSkip);

		// 列数
		const int columnNum = 0;

		// 一時保存カラー
		std::vector<VECTOR>saveColor;

		// 一時保存カラーの要素を増やす
		saveColor.resize(static_cast<int>(fieldColorInfoData.size()));

		// キャラクターカラーを代入
		for (int i = 0; i < static_cast<int>(fieldColorInfoData.size()); i++)
		{
			// カラーを代入
			saveColor[i].x = Convert_Lib::ConvertingStringToFloat(fieldColorInfoData[i][static_cast<int>(ColorName::Red)]);
			saveColor[i].y = Convert_Lib::ConvertingStringToFloat(fieldColorInfoData[i][static_cast<int>(ColorName::Green)]);
			saveColor[i].z = Convert_Lib::ConvertingStringToFloat(fieldColorInfoData[i][static_cast<int>(ColorName::Blue)]);
		}

		// 落下時のグラフィックカラーを代入
		m_fieldInfo.fallColor = saveColor[0];
	
		
		// 一時保存カラー情報
		ColorInfo saveColorInfo = ColorInfo();

		// HACK:コードが雑
		{
			saveColorInfo.color_0 = saveColor[1];
			saveColorInfo.color_1 = saveColor[2];

			m_fieldInfo.color.push_back(saveColorInfo);

			saveColorInfo.color_0 = saveColor[3];
			saveColorInfo.color_1 = saveColor[4];

			m_fieldInfo.color.push_back(saveColorInfo);

			saveColorInfo.color_0 = saveColor[5];
			saveColorInfo.color_1 = saveColor[6];

			m_fieldInfo.color.push_back(saveColorInfo);
		}
	}
}

void MasterData::LoadModelInfo()
{
	// モデル情報名
	enum class FieldStatusName
	{
		FilePath,	//ファイルパス
		Radian,		// モデルの向き
		Scale,		// スケール
		Pos_y,		// Y座標
		IsUseMotion,// モーションが使えるかどうか
	};

	// ロードしたテキスト
	std::vector<std::vector<std::string>> loadText = File_Lib::CsvFileLoading("Data/csv/ModelInformation.csv");

	// モデル情報
	std::vector<FileReadModelInfo> modelInfo;

	// モデル情報
	modelInfo.resize(static_cast<int>(loadText.size()));

	for (int i = 0; i < static_cast<int>(modelInfo.size()); i++)
	{
		// ファイルパス代入
		modelInfo[i].filePath = loadText[i][static_cast<int>(FieldStatusName::FilePath)];

		// モデルの向き
		modelInfo[i].radian = Convert_Lib::ConvertingStringToFloat(loadText[i][static_cast<int>(FieldStatusName::Radian)]);

		// モデルのスケール
		modelInfo[i].scale = Convert_Lib::ConvertingStringToFloat(loadText[i][static_cast<int>(FieldStatusName::Scale)]);

		// モデルのY座標
		modelInfo[i].pos_y = Convert_Lib::ConvertingStringToFloat(loadText[i][static_cast<int>(FieldStatusName::Pos_y)]);

		// モーションが使えるかどうか
		if (Convert_Lib::ConvertingStringToFloat(loadText[i][static_cast<int>(FieldStatusName::IsUseMotion)]) == 0)
		{
			modelInfo[i].isUseMotion = false;
		}
		else
		{
			modelInfo[i].isUseMotion = true;
		}
	}

	// モデル情報を代入する
	m_modelInfo = modelInfo;
}
