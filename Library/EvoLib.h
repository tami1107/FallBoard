#pragma once
#include <DxLib.h>
#include <vector>
#include "Vec2.h"
#include <string>


// ����
enum class Direction
{
	Top,	// ��
	Bottom,	// ��
	Left,	// ��
	Right,	// �E
	DirectionNum,// ������
};

// �����
struct Sphere
{
	// ���W
	VECTOR pos = { 0.0f,0.0f,0.0f };
	// ���a
	float radius = 0.0f;
};

// FPS
class FPS_Lib
{
public:

	// DXLib��60FPS�œ�����
	static constexpr int Dxlib_60FPS = 16667;
	// FPS
	static constexpr int FPS = 60;


	// FPS�̎擾
	static int GetFPS_EvoLib();
};

// ���[�h
class Load_Lib
{
public:
	// ������
	struct DivNum
	{
		// ���̕�����
		const int x = 1;
		// �c�̕�����
		const int y = 1;
	};


	// �O���t�B�b�N���
	struct GraphicInfo
	{
		// �t�@�C���p�X
		const char* filePath = "";

		// ���W
		Vec2 pos = Vec2(0.0f, 0.0f);

		// �O���t�B�b�N�X�P�[��
		float scale = 1.0f;

		// �O���t�B�b�N�̕�����
		DivNum div = DivNum();
	};


	// �O���t�B�b�N�������
	struct DivGraphInfo
	{
		// �n���h��
		std::vector<int>handle;

		// �O���t�B�b�N�̃X�P�[��
		Vec2 scale = Vec2(0.0f, 0.0f);
	};

public:

	/// <summary>
	/// �O���t�B�b�N�̕���
	/// </summary>
	/// <param name="filePath">�O���t�B�b�N�̃t�@�C���p�X</param>
	/// <param name="div_x">�������ɑ΂��镪����</param>
	/// <param name="div_y">�c�����ɑ΂��镪����</param>
	/// <returns></returns>
	static DivGraphInfo LoadDivGraph_EvoLib(const char* filePath, const int& div_x, const int& div_y);

	/// <summary>
	/// �O���t�B�b�N�̕����@�����
	/// </summary>
	/// <param name="filePath">�O���t�B�b�N�̃t�@�C���p�X</param>
	/// <param name="div">�c���̕�����</param>
	/// <returns>�������ꂽ�O���t�B�b�N�n���h����Ԃ�</returns>
	static std::vector<int> LoadDivGraph_EvoLib_Revision(const char* filePath, const DivNum& div);
};



// �ϊ�
class Convert_Lib
{
public:

	// �����ɑΉ������p�x
	static constexpr float TopAngle = 270.0f;	// ��
	static constexpr float BottomAngle = 90.0f;	// ��
	static constexpr float LeftAngle = 180.0f;	// ��
	static constexpr float RightAngle = 0.0f;	// �E

public:

	/// <summary>
	/// �J���[��255�ɕϊ�
	/// </summary>
	/// <param name="color"></param>
	/// <returns></returns>
	static VECTOR ColorCodeTo255(const VECTOR& color);


	/// <summary>
	/// �ő�l�ƕϊ��l���g���A�p�[�Z���g�����߂�
	/// </summary>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="convertValue">�ϊ��l</param>
	/// <returns>�p�[�Z���e�[�W</returns>
	static int PercentageRate(const float& maxValue, const float& convertValue);


	/// <summary>
	/// �l���p�[�Z���g�ϊ�
	/// </summary>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="percentage">�p�[�Z���g</param>
	/// <returns>�p�[�Z���e�[�W</returns>
	static int ConvertingValueToPercent(const int& maxValue, const float& percentage);


	/// <summary>
	/// �p�[�Z���g����l�ɕϊ�
	/// </summary>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="percentage">�p�[�Z���g</param>
	/// <returns>�ϊ����ꂽ�l</returns>
	static int ConvertingFromPercentToValue(const int& maxValue, const float& percentage);


	/// <summary>
	/// �p�x�����W�A���ɕϊ�
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <returns>�ϊ��������W�A����Ԃ�</returns>
	static float ConvertAngleToRadian(const float& angle);


	/// <summary>
	/// ���W�A�����p�x�ɕϊ�
	/// </summary>
	/// <param name="radian">���W�A��</param>
	/// <returns>�ϊ������p�x��Ԃ�</returns>
	static float ConvertRadianToAngle(const float& radian);


	/// <summary>
	/// ���������p�x�ɕϊ�
	/// </summary>
	/// <param name="direction">����</param>
	/// <returns>�p�x</returns>
	static float ConvertDirectionToAngle(const Direction& direction);


	/// <summary>
	/// �p�x����������ɕϊ�
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <param name="addSubValue">�����l</param>
	/// <returns>�������</returns>
	static Direction ConvertAngleToDirection(const float& angle, const float addSubValue = 0.0f);


	/// <summary>
	/// �t���[����b�ɕϊ�
	/// </summary>
	/// <param name="frame">�t���[��</param>
	/// <param name="fps">FPS</param>
	/// <returns>�b</returns>
	static int ConvertFrameToSeconds(const int& frame, const int& fps = 0);

	/// <summary>
	/// �t���[����b�ɕϊ�(������)
	/// </summary>
	/// <param name="frame">�t���[��</param>
	/// <param name="isInteger">�����l��Ԃ����ǂ���</param>
	/// <param name="fps">FPS</param>
	/// <returns>�ϊ����ꂽ�b����Ԃ�</returns>
	static float ConvertFrameToSeconds_Revision(const int& frame, bool isInteger = true, int fps = 0);


	/// <summary>
	/// �����֕ϊ�
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="radius">���a</param>
	/// <returns>�����</returns>
	static Sphere ConvertSphereInfo(const VECTOR& pos, const float& radius);


	/// <summary>
	/// string�^����float�^�ɕϊ�
	/// </summary>
	/// <param name="num">string�^�̐���</param>
	/// <returns>float�^�̐���</returns>
	static float ConvertingStringToFloat(const std::string& stringNum);

	/// <summary>
	/// float�^����string�^�ɕϊ�
	/// </summary>
	/// <param name="num">float�^�̐���</param>
	/// <returns>string�^�̐���</returns>
	static std::string ConvertingFloatToString(const float& num);

	/// <summary>
	/// �t���[�����~���b�P�ʂɕϊ�
	/// </summary>
	/// <param name="frame">�t���[��</param>
	/// <returns>�~���b�P�ʂ�Ԃ�</returns>
	static int ConvertingFrameCountToMillisecondUnit(const int& frame);

};

class Collision_Lib
{
public:

	/// <summary>
	/// ���̏Փ˔���
	/// </summary>
	/// <param name="sphere1">�����1</param>
	/// <param name="sphere2">�����2</param>
	/// <returns>�Փ˂������ǂ�����Ԃ�</returns>
	static bool SphereCollision(const Sphere& sphere1, const Sphere& sphere2);
};


class Calculation_Lib
{
public:

	// �v�Z(�����Z�A�����Z)
	enum class Calculation
	{
		Addition,	// �����Z
		Subtraction,// �����Z
	};


public:

	/// <summary>
	/// �T�C���J�[�u�̒l�����߂�
	/// </summary>
	/// <param name="currentFrame">���݂̃t���[��</param>
	/// <param name="maxFrame">�ő�t���[��</param>
	/// <param name="maxValue">�T�C���J�[�u�̍ő�l</param>
	/// <param name="isAbs">abs���g�p���邩�ǂ���</param>
	/// <returns></returns>
	static int SineCurve(int currentFrame, int maxFrame, int maxValue, bool isAbs = false);


	/// <summary>
	/// 2D�̐U��
	/// </summary>
	/// <param name="currentFrame">���݂̃t���[��</param>
	/// <param name="maxFrame">�U��������ő�t���[��</param>
	/// <param name="vibrationWidth">�U����</param>
	/// <returns>2D�̐U���l</returns>
	static Vec2 VibrationValue(int currentFrame, const int& maxFrame, const int& vibrationWidth);


	/// <summary>
	/// ���[�v�ԍ�
	/// </summary>
	/// <param name="minNumber">�ŏ��l</param>
	/// <param name="maxNumber">�ő�l</param>
	/// <param name="currentNumber">���݂̒l</param>
	/// <param name="isInverseLoop">�t���[�v���邩�ǂ���</param>
	/// <returns>�ϊ����ꂽ�ԍ�</returns>
	static int SelectLoopNumber(const int& minNumber, const int& maxNumber, const int& currentNumber, bool isInverseLoop = false);


	/// <summary>
	/// ����ʒu����Ƃ��A�����l�������O���t�B�b�N�̍��W��Ԃ�
	/// </summary>
	/// <param name="handle">�O���t�B�b�N�n���h��</param>
	/// <param name="num">�O���t�B�b�N�̌�</param>
	/// <param name="graphScale">�O���t�B�b�N�X�P�[��</param>
	/// <param name="centerPos">��Ƃ�����W</param>
	/// <param name="distancePos">���Ƃ�����W</param>
	/// <param name="isSide">�����ǂ���</param>
	/// <returns>�����l�������O���t�B�b�N���W��Ԃ�</returns>
	static std::vector<Vec2> GraphicWidthCoordinate(const int& handle, const int& num, const float& graphScale, const Vec2& centerPos, const Vec2& distancePos, bool isSide = true);


	/// <summary>
	/// ���W���ϓ��ɕ��ׂ�
	/// </summary>
	/// <param name="num">�ϓ��ɕ��ׂ鐔</param>
	/// <param name="centerPos">��Ƃ���ʒu</param>
	/// <param name="distancePos">���Ƃ�����W</param>
	/// <param name="isSide">���ɕ��ׂ邩�ǂ���</param>
	/// <returns>�ϓ��ɕ��ׂ����W</returns>
	static std::vector<Vec2> SortCoordinateEqually(const int& num, const Vec2& centerPos, const Vec2& distancePos, bool isSide = true);


	/// <summary>
	/// �^�[�Q�b�g�͈͓̔����ǂ����̔���
	/// </summary>
	/// <param name="currentValue">���݂̒l</param>
	/// <param name="targetValue">�^�[�Q�b�g�̒l</param>
	/// <param name="addSubValue">�����Z�l</param>
	/// <returns>�^�[�Q�b�g�͈͓̔����ǂ���</returns>
	static bool IsTargetRangeValue(const float& currentValue, const float& targetValue, const float& addSubValue);


	/// <summary>
	/// �p�x�������Z���āA360�x�𒴂�����A0�x�ȉ��ɂȂ�Ȃ��悤�Ɍv�Z���s��
	/// </summary>
	/// <param name="currentAngle">���݂̊p�x</param>
	/// <param name="addSubValue">�����Z�l</param>
	/// <param name="calculation">�����Z�������Z���ǂ���</param>
	/// <param name="isInteger">�����l���ǂ���</param>
	/// <param name="isAdjustment">�����l�������邩�ǂ���</param>
	/// <returns>�����Z���I�����p�x��Ԃ�</returns>
	static float AngleLimitValueNotExceedToCalculate(const float& currentAngle, const float& addSubValue, Calculation_Lib::Calculation calculation, bool isInteger = true, bool isAdjustment = true);

};


class Draw_Lib
{
public:

	// ���[�v�̕���
	enum class LoopDirection
	{
		Go_Top,		// ������Ƀ��[�v����
		Go_Under,	// �������Ƀ��[�v����
		Go_Left,	// �������Ƀ��[�v����
		Go_Right,	// �E�����Ƀ��[�v����
	};


	struct Color
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
	};


public:

	/// <summary>
	/// �ȈՔw�i�`��
	/// </summary>
	/// <param name="graphHandle">�摜�n���h��</param>
	/// <param name="screenSize">��ʃT�C�Y</param>
	/// <param name="isFlipImage">�摜�𔽓]���邩�ǂ���</param>
	static void DrawSimpleBackground(const int& graphHandle, const Vec2& screenSize, bool isFlipImage = false);

	/// <summary>
	/// �ȈՔw�i���[�v
	/// </summary>
	/// <param name="graphHandle">�O���t�B�b�N�̃n���h��</param>
	/// <param name="currentFrame">���݂̈ړ���</param>
	/// <param name="moveSpeed">�w�i�̈ړ��X�s�[�h</param>
	/// <param name="screenSize">��ʃT�C�Y</param>
	/// <param name="isSupportedImageInLoop">�g�p����摜�����[�v�ɑΉ����Ă��邩�ǂ���</param>
	/// <param name="loopDirection">���[�v�������</param>
	/// <returns>���݂̈ړ��ʂ�Ԃ�</returns>
	static float SimpleLoopBackground(const int& graphHandle, const float currentMoveValue,float moveSpeed, const Vec2 screenSize, bool isSupportedImageInLoop = false, LoopDirection loopDirection = LoopDirection::Go_Left);

	/// <summary>
	/// 2D�O���t�B�b�N��3D��Ԃɕ`�悷��@��graphHandle �Ŏw�肷��摜�͂W�ȏ�̂Q�̂���̃s�N�Z���T�C�Y�ł���K�v������܂�
	/// </summary>
	/// <param name="graphHandle">�O���t�B�b�N�̃n���h��</param>
	/// <param name="graphScale">�O���t�B�b�N�̃X�P�[��</param>
	/// <param name="angle">�p�x</param>
	/// <param name="pos">���W</param>
	/// <param name="color">�J���[�̒l(�ő�l:1.0f)</param>
	static void DrawGprah3D_EvoLib(const int& graphHandle, const float& graphScale, const VECTOR& angle, const VECTOR& pos, Color color = { 1.0f,1.0f,1.0f });

	/// <summary>
	/// 3D��ԂɃL���[�u��`�悷��
	/// </summary>
	/// <param name="centerPos">���S���W</param>
	/// <param name="cubeScale">��ӂ̒���</param>
	/// <param name="alphaValue">�����x</param>
	/// <param name="lineThickness">���̑���</param>
	/// <param name="color">�J���[�̒l(�ő�l:1.0f)</param>
	static void DrawCube3D_EvoLib(const VECTOR& centerPos, const VECTOR& oneSideLength, int alphaValue = 255, int lineThickness = 1, Color color = { 0.0f,1.0f, 0.0f });
};


class File_Lib
{
public:

	// �ǂݎ��^�C�v
	enum class LoadType
	{
		Noramal,// �ʏ�
		SkipFirstLine,	// ��s�ڂ��X�L�b�v����
		SkipOneColumn,	// �e���ڂ��X�L�b�v����
		DoubleSkip,		// ��s�ڂƊe���ڂ��X�L�b�v����
	};


private:

	/// <summary>
	/// ������𕪊����ĕԂ�
	static std::vector<std::string> Split(const std::string& input, const char& delimiter);
public:

	/// <summary>
	/// �t�@�C�������݂��邩�ǂ������m�F
	/// </summary>
	/// <param name="name">�t�@�C����</param>
	/// <returns>���̃t�@�C�������݂��邩�ǂ�����Ԃ�</returns>
	static bool IsFileExist(const std::string& name);

	
	/// <summary>
	/// CSV�t�@�C����ǂݍ���
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="isLoadFirstLine">�ŏ��̈�s��ǂݍ��ނ��ǂ���</param>
	/// <returns>�ǂݍ��񂾕������Ԃ�</returns>
	static std::vector<std::vector<std::string>> CsvFileLoading(const std::string& filePath, bool isLoadFirstLine = true);

	/// <summary>
	/// CSV�t�@�C����ǂݍ���(������)
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="isLoadFirstLine">�ŏ��̈�s��ǂݍ��ނ��ǂ���</param>
	/// <param name="isLoadOneColumn">�ŏ��̈���ǂݍ��ނ��ǂ���</param>
	/// <returns>�ǂݍ��񂾕������Ԃ�</returns>
	static std::vector<std::vector<std::string>> CsvFileLoading_Revision(const std::string& filePath, LoadType loadType = LoadType::Noramal);

	/// <summary>
	/// �Ȉ�CSV�t�@�C����������
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="writingText">�������ރe�L�X�g</param>
	static void SimpleCsvFileWriting(const std::string& fileName, const std::string& writingText);
};