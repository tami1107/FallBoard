#include "StateMachine.h"// ※一番上に書く
#include "MainUI.h"
#include "MainParam.h"
#include <DxLib.h>
#include "GameData.h"
#include "Game.h"
#include "EvoLib.h"
#include "Sound.h"



// カウント
namespace CountInfo
{
	// カウントのグラフィックファイルパス
	const std::vector<const char*>kFilePath =
	{
		"Data/UI/CountDown/start.png",
		"Data/UI/CountDown/count_1.png",
		"Data/UI/CountDown/count_2.png",
		"Data/UI/CountDown/count_3.png",
	};

	// カウントグラフィックのデフォルトサイズ
	constexpr float kGraphScale = 0.8f;

	// カウントグラフィックの最大サイズ
	constexpr float kGraphMaxSize = kGraphScale * 2.0f;

	// 透明度の値
	constexpr int kAlphaValue = 255;
}


// ライフ
namespace LifeInfo
{
	// ライフのグラフィックファイルパス
	const std::vector<const char*>kFilePath =
	{
		"Data/UI/Life/underLifeBar.png",
		"Data/UI/Life/topLifeBar.png",
		"Data/UI/Life/lifeFrame.png",
		"Data/UI/Life/lifeGage.png",
	};

	// グラフィック名
	enum class GraphName
	{
		UnderLifeBar,	// ライフバー下
		TopLifeBar,		// ライフバー上
		LifeFrame,		// ライフフレーム
		LifeGage		// ライフゲージ
	};

	// グラフィックのデフォルトスケール
	constexpr float kGraphScale = 1.0f;

	// グラフィック幅
	constexpr int kGraphDistance = 130;

	// グラフィックの高さ
	constexpr int kGraphHeight = 80;

	// ライフの位置
	constexpr float kLifeGraphBasePos = -33.0f;

	// ライフのグラフィック幅
	constexpr float kLifeGraphDistance = 25.0f;

	// 点滅回数
	constexpr int kFlashingCount = 3;

	// 点滅スピード
	constexpr int kFlashingSpeed = 90;

	// 振動秒数
	constexpr int kVibrationFrame = kFlashingSpeed * kFlashingCount;

	// 振動幅
	constexpr int kVibrationWidth = 30;
}

namespace
{
	// グラフィック
	struct GraphicInfo
	{
		// ファイルパス
		const char* filePath = "";

		// グラフィックの分割数
		const int div_x = 0;
		const int div_y = 0;
	};
}

// タグ
namespace TagInfo
{
	// グラフィックのファイルパス
	const std::vector<GraphicInfo>kGraphFilePath=
	{
		{
			// タグ
			"Data/UI/Tag/playerNum.png",

			// グラフィックの分割数
			1,5,
		},

		{
			// タグのフレーム
			"Data/UI/Tag/playerNumFrame.png",

			// グラフィックの分割数
			1,5,
		},
	};

	// 調整Y座標
	constexpr float kAdjustmentPos_Y = 10.0f;

	// グラフィックスケール
	constexpr float kGraphScale = 5.0f;
}

// フェイス
namespace FaceInfo
{
	// グラフィックのファイルパス
	const std::vector<GraphicInfo>kGraphFilePath =
	{
		{
			// フェイスグラフィック
			"Data/UI/Life/face.png",

			// グラフィックの分割数
			2,1,
		},
	};

	// グラフィック名
	enum class GraphName
	{
		Face	// フェイス
	};

	// グラフィックタイプ名
	enum class GraphTypeName
	{
		Noramal,// 通常
		Die		// 死亡
	};

	// フェイスグラフィックサイズ
	constexpr float kGraphScale = 0.3f;

	// 調整Y座標
	constexpr float kAdjustmentPos_Y = -90.0f;
}

// 説明
namespace Operation
{
	// 説明グラフィックのファイルパス
	const char* const kGraphFilePath = "Data/Graphic/BackGround/operationDescription.png";

	// 初期座標
	const Vec2 kInitPos = Vec2(300.0f, 400.0f);

	// グラフィックスケール
	constexpr float kGraphScale = 0.8f;
}

namespace
{
	// サウンドファイル名
	const std::vector<std::string> kSoundFileName =
	{
		"countStart",	// カウントスタート音
		"countEnd",		// カウント終了音
		"mainBGM"		// メインBGM
	};

	// サウンド名
	enum class SoundName
	{
		CountStert,	// カウントスタート音
		CountEnd,	// カウント終了音
		MainBGM,	// メインBGM
	};
}


MainUI::MainUI():
	// カウント関連↓
	m_count(),
	m_operationGraphHandle(),
	// ライフ関連↓
	m_life(),
	m_face(),
	// タグ関連↓
	m_tag(),
	// クラス関連
	m_pMainParam()
{
}

MainUI::~MainUI()
{
	// カウントグラフィックの削除
	for (auto& handle : m_count.handle)
	{
		// グラフィックの削除
		DeleteGraph(handle);
	}
	
	// ライフグラフィックの削除
	for (auto& handle : m_life.handle)
	{
		// グラフィックの削除
		DeleteGraph(handle);
	}
	
	// タググラフィックの削除
	for (int i = 0; i < static_cast<int>(m_tag.size()); i++)
	{
		for (int j = 0; j < static_cast<int>(m_tag[i].handle.size()); j++)
		{
			// グラフィックの削除
			DeleteGraph(m_tag[i].handle[j]);
		}
	}

	// 説明グラフィックの削除
	DeleteGraph(m_operationGraphHandle);
}

void MainUI::Init()
{
	// ステートマシンの初期化処理
	InitStateMachine();

	// ライフ初期化処理
	LifeInit();

	// サウンドロード処理
	Sound::getInstance().Load(kSoundFileName);
}

void MainUI::Update()
{
	// ステートマシンの更新処理
	m_pStateMachine.Update();

	// ライフ更新処理
	LifeUpdate();
}

void MainUI::Draw()
{
	// ステートマシンの描画処理
	m_pStateMachine.Draw();

	// ライフ描画処理
	LifeDraw();
}

void MainUI::CountEnter()
{
	// アルファ値の代入
	m_count.alphaValue = CountInfo::kAlphaValue;

	// グラフィックスケールの代入
	m_count.scaleValue = CountInfo::kGraphScale;
}

void MainUI::CountUpdate()
{
	// フレーム数の代入
	m_count.frame = m_pMainParam->CountDown();

	// フレームを秒数に変換
	m_count.second = Convert_Lib::ConvertFrameToSeconds(m_count.frame);

	// フレーム数が4,3,2の値の場合、カウントスタートのSEを再生
	if (m_count.frame == (FPS_Lib::FPS * 4) ||
		m_count.frame == (FPS_Lib::FPS * 3) ||
		m_count.frame == (FPS_Lib::FPS * 2))
	{
		// カウントダウンのSEを流す
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::CountStert)]);
	}
	// フレーム数が1の値の場合、カウントエンドのSEを再生
	if (m_count.frame == (FPS_Lib::FPS * 1))
	{
		// カウントダウンのSEを流す
		Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::CountEnd)]);
	}

	// グラフィックのサイズ、アルファ値
	{
		// グラフィックの計算に用いるフレーム値
		const int graphFrame = m_count.frame % FPS_Lib::FPS;

		// グラフィックを動かすフレーム
		const int graphDriveFrame = static_cast<int>(FPS_Lib::FPS * 0.5f);

		// グラフィックを動かすかどうか
		const bool isGraphDrive = (graphFrame <= graphDriveFrame);

		
		// グラフィックを動かすフラグがtrueの時、動かす
		if (isGraphDrive)
		{
			// アルファ値の減少値
			const int alphaSubValue = CountInfo::kAlphaValue / graphDriveFrame;

			// アルファ値を減らす
			m_count.alphaValue -= alphaSubValue;

			// デフォルトスケール値とマックススケール値の差
			const float differenceScale = CountInfo::kGraphMaxSize - CountInfo::kGraphScale;

			// スケール値の加算値
			const float scaleAddValue = differenceScale / graphDriveFrame;

			// サイズを増やす
			m_count.scaleValue += scaleAddValue;
		}
		else
		{
			// アルファ値の代入
			m_count.alphaValue = CountInfo::kAlphaValue;

			// サイズの代入
			m_count.scaleValue = CountInfo::kGraphScale;
		}
	}

	// 0フレーム以上ならば処理を終了する
	if (m_count.frame > 0)
	{
		return;
	}

	// BGMを流す
	Sound::getInstance().Play(kSoundFileName[static_cast<int>(SoundName::MainBGM)]);

	// ステートセット
	m_pStateMachine.SetState(State::Normal);
}

void MainUI::CountDraw()
{

	// アルファブレンドをかける
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_count.alphaValue);

	// 秒数がハンドルの数以下の場合のみ描画
	if (m_count.second < static_cast<int>(m_count.handle.size()))
	{
		DrawRotaGraphF(Game::kScreenCenterPos.x, Game::kScreenCenterPos.y,
			m_count.scaleValue, 0.0,
			m_count.handle[m_count.second], true);
	}

	// 通常描画に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	// オプション画像を描画する
	DrawRotaGraphF(
		Operation::kInitPos.x,
		Operation::kInitPos.y,
		static_cast<double>(Operation::kGraphScale),
		0.0f,
		m_operationGraphHandle, true);

}

void MainUI::InitStateMachine()
{
	// ステートマシンの初期化、Entry
	auto dummy = []() {};

	// カウント処理をステートマシンに追加
	{
		auto countEnter = [this]() {CountEnter(); };
		auto countUpdate = [this]() {CountUpdate(); };
		auto countDraw = [this]() {CountDraw(); };
		auto countExit = dummy;

		m_pStateMachine.AddState(State::Count, countEnter, countUpdate, countDraw, countExit);
	}

	// 通常処理をステートマシンに追加
	{
		auto normalEnter = dummy;
		auto normalUpdate = dummy;
		auto normalDraw = dummy;
		auto normalExit = dummy;

		m_pStateMachine.AddState(State::Normal, normalEnter, normalUpdate, normalDraw, normalExit);
	}


	// カウントステートセット
	m_pStateMachine.SetState(State::Count);
}

void MainUI::LoadHandle()
{
	// リソースの読み込み
	SetUseASyncLoadFlag(true);	// 非同期読み込みを行いたい

	// カウントダウンのハンドルロード
	{
		// ハンドルのロード
		for (auto& handle : CountInfo::kFilePath)
		{
			m_count.handle.push_back(LoadGraph(handle));
		}
	}

	// ライフのハンドルロード
	{
		// ハンドルのロード
		for (auto& handle : LifeInfo::kFilePath)
		{
			m_life.handle.push_back(LoadGraph(handle));
		}
	}

	// タグ変数をタグのグラフィック数分、要素を増やす
	m_tag.resize(static_cast<int>(TagInfo::kGraphFilePath.size()));

	// グラフィックのロード
	for (int i = 0; i < static_cast<int>(TagInfo::kGraphFilePath.size()); i++)
	{
		// グラフィック情報代入
		Load_Lib::DivGraphInfo graphInfo = Load_Lib::LoadDivGraph_EvoLib(TagInfo::kGraphFilePath[i].filePath, TagInfo::kGraphFilePath[i].div_x, TagInfo::kGraphFilePath[i].div_y);

		// グラフィック代入
		m_tag[i].handle = graphInfo.handle;
	}


	// フェイス変数をフェイスのグラフィック数分、要素を増やす
	m_face.resize(static_cast<int>(FaceInfo::kGraphFilePath.size()));

	// グラフィックのロード
	for (int i = 0; i < static_cast<int>(FaceInfo::kGraphFilePath.size()); i++)
	{
		// グラフィック情報代入
		Load_Lib::DivGraphInfo graphInfo = Load_Lib::LoadDivGraph_EvoLib(FaceInfo::kGraphFilePath[i].filePath, FaceInfo::kGraphFilePath[i].div_x, FaceInfo::kGraphFilePath[i].div_y);

		// グラフィック代入
		m_face[i].handle = graphInfo.handle;
	}

	// 説明グラフィックのロード
	m_operationGraphHandle = LoadGraph(Operation::kGraphFilePath);


	// リソースの読み込み
	SetUseASyncLoadFlag(false);	// デフォルトに戻しておく
}

void MainUI::LifeInit()
{
	// プレイ人数取得
	const int playCount = GameData::getInstance().GetGamePlayCount();

	// 最大体力を取得
	m_life.maxLifePoint = GameData::getInstance().GetMaxLifePoint();

	// プレイ人数分の点滅変数初期化
	for (int i = 0; i < playCount; i++)
	{
		// 0を入れ要素を増やす
		m_life.flashingFrame.push_back(0);
		m_life.flashingCount.push_back(0);
	}

	// ライフポイントの配列要素追加
	m_life.lifePoint.resize(playCount);

	// ライフグラフィックの位置
	{
		// ライフの座標の配列要素追加
		m_life.graphicPos.resize(playCount);

		// 中心座標を軸としたグラフィックの座標を求める
		m_life.graphicPos = Calculation_Lib::GraphicWidthCoordinate(m_life.handle[static_cast<int>(LifeInfo::GraphName::UnderLifeBar)], 
			playCount, LifeInfo::kGraphScale,Vec2(Game::kScreenCenterPos.x, LifeInfo::kGraphHeight), Vec2(LifeInfo::kGraphDistance, 0.0f));
	}
}

void MainUI::LifeUpdate()
{
	// ライフの値を取得
	for (int i = 0; i < static_cast<int> (m_life.lifePoint.size()); i++)
	{
		// ライフ代入
		m_life.lifePoint[i] = m_pMainParam->GetCharLife(i);
	}
}

void MainUI::LifeDraw()
{
	// カラー
	VECTOR color = {};

	// 基準座標
	std::vector<Vec2>basePos = {};

	// ライフゲージ座標
	Vec2 lifeGagePos = {};


	// 点滅フラグ
	bool isFlashing = false;

	// ライフ変数分、処理を繰り返し、画像を描画する
	for (int lifeSize = 0; lifeSize < static_cast<int> (m_life.graphicPos.size()); lifeSize++)
	{
		// カラー取得
		color = Convert_Lib::ColorCodeTo255(GameData::getInstance().GetPlayerColor(lifeSize));

		// 要素
		basePos.push_back(Vec2());

		// 点滅しているかどうかを調べる
		isFlashing = m_life.flashingFrame[lifeSize] != 0;

		// 点滅している場合は、グラフィックを揺らす座標を求める
		if (isFlashing)
		{
			basePos[lifeSize] = Calculation_Lib::VibrationValue(m_life.flashingFrame[lifeSize], LifeInfo::kVibrationFrame, LifeInfo::kVibrationWidth);
		}

		// 座標代入
		basePos[lifeSize] += m_life.graphicPos[lifeSize];


		// ライフバー
		{

			// SetDrawBrightを使用しカラー設定する
			SetDrawBright(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z));

			// 点滅フラグがtrueの場合、アルファブレンドを使用する
			if (isFlashing)
			{
				// 透明度を設定
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			}

			// ライフポイントが0の場合、アルファブレンドを使用する
			if (m_life.lifePoint[lifeSize] == 0)
			{
				// 透明度を設定
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			}

			// ライフバー下を描画する
			DrawRotaGraphF(basePos[lifeSize].x, basePos[lifeSize].y,
				LifeInfo::kGraphScale, 0.0, m_life.handle[static_cast<int>(LifeInfo::GraphName::UnderLifeBar)], true);

			// SetDrawBrightを使用して、カラー設定をもとに戻す
			SetDrawBright(255, 255, 255);

			// ライフバー上を描画する
			DrawRotaGraphF(basePos[lifeSize].x, basePos[lifeSize].y,
				LifeInfo::kGraphScale, 0.0, m_life.handle[static_cast<int>(LifeInfo::GraphName::TopLifeBar)], true);

			// 描画ブレンドモードをノーブレンドにする
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		// ライフゲージ
		{
			// ライフゲージ座標を代入
			lifeGagePos = basePos[lifeSize];

			// ライフゲージをライフポイント分処理を繰り返し、描画する
			for (int lifePoint = 0; lifePoint < m_life.maxLifePoint; lifePoint++)
			{
				// ライフゲージのX座標を求める
				lifeGagePos.x = (basePos[lifeSize].x + LifeInfo::kLifeGraphBasePos) + (LifeInfo::kLifeGraphDistance * lifePoint);

				// SetDrawBrightを使用して、カラー設定をもとに戻す
				SetDrawBright(255, 255, 255);

				// ライフゲージ描画
				if (lifePoint < m_life.lifePoint[lifeSize])
				{
					DrawRotaGraphF(lifeGagePos.x, lifeGagePos.y,
						LifeInfo::kGraphScale, 0.0, m_life.handle[static_cast<int>(LifeInfo::GraphName::LifeGage)], true);
				}

				// ダメージを受けているかどうかを取得する
				if (m_pMainParam->GetIsCharDamage(lifeSize))
				{
					// 振動値を増やす
					m_life.flashingFrame[lifeSize]++;

					// 振動値がkFlashingCountを超えた場合、振動変数を初期化する
					if (m_life.flashingCount[lifeSize] >= LifeInfo::kFlashingCount)
					{
						// 変数の初期化
						m_life.flashingFrame[lifeSize] = 0;
						m_life.flashingCount[lifeSize] = 0;

						// メインパラムにfalseフラグを代入する
						m_pMainParam->SetIsCharDamage(lifeSize, false);
					}

					// アルファ値を求める
					const int alphaValue = Calculation_Lib::SineCurve(m_life.flashingFrame[lifeSize], LifeInfo::kFlashingSpeed, 255, true);

					// アルファ値が0の場合、点滅回数変数を増やす
					if (alphaValue == 0)
					{
						// 点滅回数変数を増やす
						m_life.flashingCount[lifeSize]++;
					}

					// 点滅座標を求める
					const float flashingPos_x = (basePos[lifeSize].x + LifeInfo::kLifeGraphBasePos) + (LifeInfo::kLifeGraphDistance * (m_life.lifePoint[lifeSize]));

					// 透明度を設定
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaValue);

					// ライフゲージを描画
					DrawRotaGraphF(flashingPos_x, lifeGagePos.y,
						LifeInfo::kGraphScale, 0.0, m_life.handle[static_cast<int>(LifeInfo::GraphName::LifeGage)], true);

					// 透明度を設定
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	
				}

				// SetDrawBrightを使用し、カラーを設定する
				SetDrawBright(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z));

				// ライフフレームを描画
				DrawRotaGraphF(lifeGagePos.x, lifeGagePos.y,
					LifeInfo::kGraphScale, 0.0, m_life.handle[static_cast<int>(LifeInfo::GraphName::LifeFrame)], true);
			}

			

			// フェイス
			{
				// フェイス座標に基準座標を代入
				Vec2 facePos = basePos[lifeSize];

				// フェイス座標に調整座標を代入する
				facePos.x += FaceInfo::kAdjustmentPos_Y;

				// グラフィックタイプ
				FaceInfo::GraphTypeName graphType = FaceInfo::GraphTypeName::Noramal;

				// ライフが0の場合、グラフィックタイプを死亡にする
				if (m_life.lifePoint[lifeSize] == 0)
				{
					graphType = FaceInfo::GraphTypeName::Die;
				}

				// SetDrawBrightを使用し、カラーを設定する
				SetDrawBright(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z));

				// フェイスを描画
				DrawRotaGraphF(facePos.x, facePos.y,
					FaceInfo::kGraphScale, 0.0, 
					m_face[static_cast<int>(FaceInfo::GraphName::Face)].handle[static_cast<int>(graphType)], true);
			}

			// SetDrawBrightを使用して、カラー設定をもとに戻す
			SetDrawBright(255, 255, 255);
		}
	}
}

void MainUI::CharacterTagDraw(const bool& isPlayer, const int& playerNum, VECTOR pos, VECTOR color)
{
	// キャラクタータグ名
	enum class CharacterTagName
	{
		Player_1,	// プレイヤー１
		Player_2,	// プレイヤー２
		Player_3,	// プレイヤー３
		Player_4,	// プレイヤー４
		CP,			// コンピュータ
	};

	// 描画情報名
	enum class DrawInfoName
	{
		Noramal,// 通常描画
		Frame	// フレームを描画
	};


	// Y座標調整
	pos.y += TagInfo::kAdjustmentPos_Y;

	// カラー調整
	color = Convert_Lib::ColorCodeTo255(color);

	// キャラクタータグの番号
	int characterTagNumber = static_cast<int>(CharacterTagName(playerNum));

	// タグをつけるキャラクターがプレイヤーでない場合、CPのキャラクタータグ番号を代入する
	if (!isPlayer)
	{
		characterTagNumber = static_cast<int>(CharacterTagName::CP);
	}

	// SetDrawBrightを使用し、グラフィックに色を付ける
	SetDrawBright(static_cast<int>(color.x), static_cast<int>(color.y), static_cast<int>(color.z));

	// ３Ｄ空間上に通常画像を描画
	DrawBillboard3D(pos, 0.5f, 0.5f, TagInfo::kGraphScale, 0.0f, m_tag[static_cast<int>(DrawInfoName::Noramal)].handle[characterTagNumber], true);

	// 色をもとに戻す 
	SetDrawBright(255, 255, 255);

	// ３Ｄ空間上にフレーム画像を描画
	DrawBillboard3D(pos, 0.5f, 0.5f, TagInfo::kGraphScale, 0.0f, m_tag[static_cast<int>(DrawInfoName::Frame)].handle[characterTagNumber], true);
}


