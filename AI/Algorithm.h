#pragma once
#include <memory>
#include <vector>
#include "EvoLib.h"
#include <DxLib.h>
#include <tuple>
#include "Field.h"

class Field;
class Algorithm
{
public:

	Algorithm();
	virtual ~Algorithm();

	/// <summary>
	/// �t�B�[���h�N���X�|�C���^�̎擾
	/// </summary>
	/// <param name="field">�t�B�[���h�N���X�|�C���^</param>
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }


	void Init();
	void Update();

public:

	// �Ֆ�
	struct AlgorithmBoard
	{
		bool isOpen = false;			// �J���Ă��邩
		bool isClose = false;			// ���Ă��邩
		bool isUse = false;				// �g���Ă��邩
		bool isObstacle = false;		// ��Q���͂��邩
		bool isPlayerPresent = false;	// �v���C���[�����邩
		int cost = 0;					// �R�X�g
	};

	// �ړI�n
	struct Coordinate
	{
		// �S�[�����W
		VECTOR goalPos = VECTOR();
		// ����
		Direction direction = {};
		// �i�񂾂��ǂ���
		bool isMove = false;
	};

	// �U��
	struct Attack
	{
		// �U���ł��邩�ǂ���
		bool isAttack = false;

		// ����
		Direction direction = {};
	};

	// �R�X�g
	struct Cost
	{
		// �R�X�g�̒l
		int value = 0;

		// �Z��
		Field::Cell cell = {};

		// ����
		Direction direction = {};
	};

public:

	/// <summary>
	/// �^�[�Q�b�g�̕��������߂�
	/// </summary>
	/// <param name="currentPos">���݂̍��W</param>
	/// <param name="targetPos">�^�[�Q�b�g�̍��W</param>
	/// <param name="isCell">�Z�����ǂ���</param>
	/// <returns>�^�[�Q�b�g�̕���</returns>
	Direction TargetDirection(VECTOR currentPos, VECTOR targetPos, bool isCell = false);

	/// <summary>
	/// �U���ł��邩�ǂ���
	/// </summary>
	/// <param name="currentPos">���݂̍��W</param>
	/// <param name="otherPlayerPos">���v���C���[�̍��W</param>
	/// <returns>�U���ł��邩�ǂ����A����</returns>
	Attack IsAssailable(const VECTOR& currentPos, const std::vector<VECTOR>& otherPlayerPos);

	/// <summary>
	/// �܂�肪��Q�����ǂ���
	/// </summary>
	/// <param name="currentPos"></param>
	/// <returns>�܂�肪��Q�����ǂ����̃t���O</returns>
	bool IsAroundObstacle(const VECTOR& currentPos);

	/// <summary>
	/// ��Q�������邩�ǂ���
	/// </summary>
	/// <param name="pos">���ׂ���W</param>
	/// <returns>��Q�������邩�ǂ����̃t���O</returns>
	bool IsObstacle(const VECTOR& pos);

	/// <summary>
	/// �v���C���[�����邩�ǂ���
	/// </summary>
	/// <param name="currentPos">���݂̈ʒu</param>
	/// <param name="playerPos">���v���C���[�̍��W</param>
	/// <returns>���v���C���[�����邩�ǂ�����Ԃ�</returns>
	bool IsPlayerPresent(const VECTOR& currentPos, const std::vector<VECTOR>& playerPos);

	/// <summary>
	/// �߂��̈��S�ȃZ����T��
	/// </summary>
	/// <param name="currentPos">���݂̍��W</param>
	/// <returns>�߂��̈��S�ȃZ�����W</returns>
	VECTOR FindCellSafeClose(const VECTOR& currentPos);

	/// <summary>
	/// ���݂̍��W��ړI�n�ɂ���
	/// </summary>
	/// <param name="currentPos">���ݍ��W</param>
	/// <returns>�ړI�n���W</returns>
	VECTOR CurrentPosDestination(const VECTOR& currentPos);

	/// <summary>
	/// �����_���ɐi��
	/// </summary>
	/// <param name="currentPos">���݂̍��W</param>
	void RandomMove(const VECTOR& currentPos);

	/// <summary>
	/// ���݂̃Z������S�[���Z���܂ł̃R�X�g�����߂�
	/// </summary>
	/// <param name="currentCell">���݂̃Z��</param>
	/// <param name="goalCell">�S�[���Z��</param>
	/// <returns>�Z�o�����R�X�g</returns>
	int RangeCost(const Field::Cell& currentCell, const Field::Cell& goalCell);

	/// <summary>
	/// �i�ލ��W��Ԃ�
	/// </summary>
	/// <param name="currentPos">���݂̍��W</param>
	/// <returns>�i�ލ��W</returns>
	VECTOR MoveCoordinate(const VECTOR& currentPos);



private:

	/// <summary>
	/// �ՖʃX�P�[�������ݒ�
	/// </summary>
	void InitSetBoardScale();

	/// <summary>
	/// �A���S���Y�����s�����߂̔Ֆʏ��
	/// </summary>
	void BoardInfo();

	/// <summary>
	/// �Z�����͈͓����ǂ���
	/// </summary>
	/// <param name="currentCell">���݂̃Z��</param>
	/// <returns>�͈͓����ǂ���</returns>
	bool IsRange(const Field::Cell& currentCell);

	/// <summary>
	/// ��Q�������邩�ǂ���
	/// </summary>
	/// <param name="currentCell">���݂̃Z��</param>
	/// <returns>��Q�������邩�ǂ���</returns>
	bool IsObstacle(const Field::Cell& currentCell);

	/// <summary>
	/// �ŒZ�����̍��W��T��
	/// </summary>
	/// <param name="searchPos">�������W</param>
	/// <returns>�ŒZ�����̍��W</returns>
	VECTOR ShortestDistance(const VECTOR& currentPos, const std::vector<VECTOR>& searchPos);

	/// <summary>
	/// �R�X�g�̍\���^�������\�[�g���ĕԂ�
	/// </summary>
	/// <param name="">�R�X�g�̍\���^</param>
	void SortFactorAscendingOrder(std::vector<Cost>& cost);

	/// <summary>
	/// ���W���Z���ɕϊ�
	/// </summary>
	/// <param name="currentPos">���ݍ��W</param>
	/// <returns>�Z��</returns>
	Field::Cell ConvertPosToCell(const VECTOR& currentPos);

	/// <summary>
	/// �Z�������W�ɕϊ�
	/// </summary>
	/// <param name="currentCell">���݂̃Z��</param>
	/// <returns>�ϊ����W</returns>
	VECTOR ConvertCellToPos(const Field::Cell& currentCell);


public:

	//////////////
	// �Ֆʊ֘A //
	//////////////

	// �Ֆ�
	std::vector<std::vector<AlgorithmBoard>>m_algorithmBoard;

	// �ՖʃX�P�[��
	int m_boardScale;


	////////////////////////
	// �N���X�|�C���^�֘A //
	////////////////////////

	// �t�B�[���h�N���X�|�C���^
	std::shared_ptr<Field>m_pField;
};