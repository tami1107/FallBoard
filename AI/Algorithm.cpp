#include "Algorithm.h"
#include <algorithm>
#include "Vec2.h"
#include "EvoLib.h"


Algorithm::Algorithm():
	// �Ֆʊ֘A��
	m_algorithmBoard(),
	m_boardScale(),
	// �N���X�|�C���^�֘A��
	m_pField()
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::Init()
{
	// �ՖʃX�P�[����������
	InitSetBoardScale();
}

void Algorithm::Update()
{
	// �Ֆʏ��̏�����
	BoardInfo();
}

Direction Algorithm::TargetDirection(VECTOR currentPos, VECTOR targetPos, bool isCell)
{

	// �Z����true�Ȃ�΁A���W���Z���ɕϊ����ăZ�����W�ɕϊ�����
	if (isCell)
	{
		// ���݂̃Z��
		const Field::Cell currentCell = ConvertPosToCell(currentPos);
		// �^�[�Q�b�g�̃Z��
		const Field::Cell targetCell = ConvertPosToCell(targetPos);


		// �Z�����W�ɕϊ�
		{
			// ���݂̍��W���Z�����W�ɕϊ�
			currentPos = ConvertCellToPos(currentCell);

			// �^�[�Q�b�g���W���Z�����W�ɕϊ�
			targetPos = ConvertCellToPos(targetCell);
		}
	}

	// �������
	Direction direction = {};

	// �㉺
	if (currentPos.z != targetPos.z)
	{
		if (currentPos.z < targetPos.z)
		{
			direction = Direction::Top;
		}
		else
		{
			direction = Direction::Bottom;
		}
	}
	// ���E
	if (currentPos.x != targetPos.x)
	{
		if (currentPos.x < targetPos.x)
		{
			direction = Direction::Right;
		}
		else
		{
			direction = Direction::Left;
		}
	}

	// ��������Ԃ�
	return direction;
}

Algorithm::Attack Algorithm::IsAssailable(const VECTOR& currentPos, const std::vector<VECTOR>& otherPlayerPos)
{
	// �U���v�f
	Attack attack = {};

	// �����̂���ʒu�ɏ�Q��������΍U�����Ȃ�
	if (IsObstacle(currentPos))
	{
		return attack;
	}

	// ���ݍ��W���Z���ɕϊ�
	const Field::Cell currentCell = ConvertPosToCell(currentPos);

	// ���v���C���[�̃Z��
	Field::Cell otherPlayerCell = {};

	// �R�X�g�̍\����
	std::vector<Cost>cost;

	// ����p�̃R�X�g�\����
	Cost saveCost = {};

	// ���v���C���[�Ɠ����Z���ɂ��邩�ǂ����𒲂ׂ�
	for (auto& pos : otherPlayerPos)
	{
		// ���v���C���[�̍��W���Z���ɕϊ�
		otherPlayerCell = ConvertPosToCell(pos);

		// ���݂̃Z���Ƒ��v���C���[�̃Z�����Е��ł������ꍇ�A�R�X�g�𒲂ׂ�
		if (currentCell.x == otherPlayerCell.x ||
			currentCell.z == otherPlayerCell.z)
		{
			// ����p�̃R�X�g�\����
			{
				// �R�X�g�v�Z
				saveCost.value = RangeCost(currentCell, otherPlayerCell);

				// ���������߂�
				saveCost.direction = TargetDirection(currentPos, pos, true);
			}

			// ���
			cost.push_back(saveCost);
		}
	}


	// �R�X�g�̗v�f��0�̏ꍇ�A�����ŏ������I������
	if (static_cast<int>(cost.size()) == 0)
	{
		// �U���v�f��Ԃ�
		return attack;
	}


	// �R�X�g���\�[�g����(����)
	SortFactorAscendingOrder(cost);


	// �U���v�f�ɕϊ�
	{
		// �ŏ��R�X�g�̕������U�������ɕϊ�
		attack.direction = cost.front().direction;

		// �U���ł���
		attack.isAttack = true;
	}


	// �U���v�f��Ԃ�
	return attack;
}

bool Algorithm::IsAroundObstacle(const VECTOR& currentPos)
{

	// ���ݍ��W���Z���ɕϊ�
	const Field::Cell currentCell = ConvertPosToCell(currentPos);

	// �����z�����
	Field::Cell dirctonArray[static_cast<int>(Direction::DirectionNum)];

	// �t�B�[���h�̃Z�������擾
	const std::vector<std::vector<Field::AlgorithmCellInfo>>fieldCell = m_pField->GetCellInfo();


	// �㉺���E�̕������������񂵁A����ɏ�Q�����Ȃ����ǂ����𒲂ׂ�
	for (int i = 0; i < static_cast<int>(Direction::DirectionNum); i++)
	{
		// �z�����
		dirctonArray[i] = currentCell;

		// �����z��ɕϊ�
		dirctonArray[i].x += Field::kDirctonArray[i].x;
		dirctonArray[i].z += Field::kDirctonArray[i].z;


		// �͈͊O�Ȃ�΃R���e�B�j���[����
		if (!IsRange(dirctonArray[i]))
		{
			continue;
		}

		// �I�u�W�F�N�g������΁Afalse��Ԃ�
		if (!fieldCell[dirctonArray[i].x][dirctonArray[i].z].isObstacle)
		{
			return false;
		}	
	}

	// true�����Ԃ�
	return true;
}

bool Algorithm::IsObstacle(const VECTOR& pos)
{
	// ���ׂ���W���Z���ɕϊ�
	const Field::Cell destinationCell = ConvertPosToCell(pos);

	// �t�B�[���h�̃Z�������擾
	const std::vector<std::vector<Field::AlgorithmCellInfo>>fieldCell = m_pField->GetCellInfo();

	// ��Q��������ꍇ�Atrue�ŕԂ�
	if (fieldCell[destinationCell.x][destinationCell.z].isObstacle)
	{
		return true;
	}

	// false�����Ԃ�
	return false;
}

bool Algorithm::IsPlayerPresent(const VECTOR& currentPos, const std::vector<VECTOR>& playerPos)
{
	// ���ݍ��W���Z���ɕϊ�
	const Field::Cell currentCell = ConvertPosToCell(currentPos);

	// ���v���C���[�̃Z��
	Field::Cell playerCell = {};

	// ���v���C���[�Ɠ����Z���ɂ��邩�ǂ����𒲂ׂ�
	for (auto& pos : playerPos)
	{
		// ���v���C���[�̍��W���Z���ɕϊ�
		playerCell = ConvertPosToCell(pos);

		// ���݂̃Z���Ƒ��v���C���[�̃Z�����������W�������ꍇ�Atrue��Ԃ�
		if (currentCell.x == playerCell.x &&
			currentCell.z == playerCell.z)
		{
			return true;
		}
	}

	// false�����Ԃ�
	return false;
}

VECTOR Algorithm::FindCellSafeClose(const VECTOR& currentPos)
{
	// ���ݍ��W���Z���ɕϊ�
	const Field::Cell currentCell = ConvertPosToCell(currentPos);

	// �����z��
	Field::Cell dirctonArray = {};

	// �����������W
	std::vector<VECTOR>searchPos;

	// �t�B�[���h�̃Z�������擾
	const std::vector<std::vector<Field::AlgorithmCellInfo>>fieldCell = m_pField->GetCellInfo();

	// �Č������邩�ǂ���
	bool isSearchAgain = true;

	// ������
	int searchCount = 0;

	// �����͈͊O���ǂ���
	bool isSearchRange = false;


	// �Č����t���O��true�̎��������s��
	while (isSearchAgain)
	{
		// �����񐔂𑝂₷
		searchCount++;

		// ��������Z���̃X�P�[��
		const int searchCellScale = searchCount + 2;

		// ��������Z���̃X�P�[�����A�Ֆʂ̃X�P�[�������傫�������ꍇ�Abreak����
		if (searchCellScale > m_boardScale)
		{
			isSearchRange = true;
			break;
		}

		// 8���������ׂĒ��ׂ�
		for (int i = 0; i < Field::kDirectionNumber; i++)
		{
			// ���݂̃Z������
			dirctonArray = currentCell;

			// �����z��ɕϊ�
			dirctonArray.x += (Field::kDirctonArray[i].x * searchCount);
			dirctonArray.z += (Field::kDirctonArray[i].z * searchCount);


			// �͈͊O�Ȃ�΃R���e�B�j���[����
			if (!IsRange(dirctonArray))
			{
				continue;
			}

			// ��Q��������΃R���e�B�j���[����
			if (fieldCell[dirctonArray.x][dirctonArray.z].isObstacle)
			{
				continue;
			}

			// ���W������
			searchPos.push_back(ConvertCellToPos(dirctonArray));

			break;
		}

		// �v�f��������΁A�Č����t���O��true�ɂ��āA�����false�ɂ���
		if (static_cast<int>(searchPos.size()) == 0)
		{
			isSearchAgain = true;
		}
		else
		{
			isSearchAgain = false;
		}
	}

	// �����͈͊O�Ȃ�Ό��݂̍��W��Ԃ�
	if (isSearchRange)
	{
		return currentPos;
	}

	// �����������W��Ԃ�
	return searchPos.front();
}

VECTOR Algorithm::CurrentPosDestination(const VECTOR& currentPos)
{
	// ���ݍ��W���Z���ɕϊ�
	const Field::Cell currentCell = ConvertPosToCell(currentPos);

	// �Z�������W�ɕϊ�
	const VECTOR destinationPos = ConvertCellToPos(currentCell);

	// �Z����Ԃ�
	return destinationPos;
}

void Algorithm::RandomMove(const VECTOR& currentPos)
{

	// ���ݍ��W���Z���ɕϊ�
	const Field::Cell startCell = ConvertPosToCell(currentPos);

	// �Ֆʏ�����
	BoardInfo();

	// �����_���������擾
	const int randDirection = GetRand((static_cast<int>(Direction::DirectionNum)) - 1);

	// �����_���ȃZ��
	Field::Cell randCell = {};

	// �����_���ȃZ����������
	randCell.x = startCell.x + Field::kDirctonArray[randDirection].x;
	randCell.z = startCell.z + Field::kDirctonArray[randDirection].z;

	// �͈͓��Ȃ�Ύg������ɂ���
	if (IsRange(randCell))
	{
		m_algorithmBoard[randCell.x][randCell.z].isUse = true;
	}
}


int Algorithm::RangeCost(const Field::Cell& currentCell, const Field::Cell& goalCell)
{
	// ��Βl
	Field::Cell absoluteValue = Field::Cell();

	// ��Βl�����߂�
	{
		absoluteValue.x = std::abs(currentCell.x - goalCell.x);
		absoluteValue.z = std::abs(currentCell.z - goalCell.z);
	}

	// �R�X�g�����߂�
	const int cost = absoluteValue.x + absoluteValue.z;

	// �l��Ԃ�
	return cost;
}

VECTOR Algorithm::MoveCoordinate(const VECTOR& currentPos)
{

	// ���ݍ��W���Z���ɕϊ�
	const Field::Cell startCell = ConvertPosToCell(currentPos);

	// �����z�����
	Field::Cell dirctonArray[static_cast<int>(Direction::DirectionNum)];

	// �ړI�n�̍��W�Ɍ��݂̍��W����
	VECTOR coordinatePos = currentPos;


	// �������������񂵁A�ړI�n�Ƃ�����W�����߂�
	for (int i = 0; i < static_cast<int>(Direction::DirectionNum); i++)
	{
		// �z�����
		dirctonArray[i] = startCell;

		// �����z��ɕϊ�
		dirctonArray[i].x += Field::kDirctonArray[i].x;
		dirctonArray[i].z += Field::kDirctonArray[i].z;


		// �͈͊O�̏ꍇ�A�R���e�B�j���[����
		if (!IsRange(dirctonArray[i]))
		{
			continue;
		}

		// �g���Z���̏ꍇ�A�����ŏ������I������
		if (m_algorithmBoard[dirctonArray[i].x][dirctonArray[i].z].isUse)
		{
			// �Z�������W�ɕϊ�
			coordinatePos = ConvertCellToPos(dirctonArray[i]);

			break;
		}
	}


	// �Ֆʂ��g��Ȃ�����ɂ���
	m_algorithmBoard[startCell.x][startCell.z].isUse = false;

	
	// �ړI�n�Ƃ�����W��Ԃ�
	return coordinatePos;
}

void Algorithm::InitSetBoardScale()
{
	// �ՖʃX�P�[������
	m_boardScale = m_pField->GetBoardScale();

	// �v�f�����������񎟌��z��
	const std::vector<std::vector<AlgorithmBoard>> m_initBoard(m_boardScale, std::vector<AlgorithmBoard>(m_boardScale));

	// �z������R�s�[
	m_algorithmBoard = m_initBoard;
}

void Algorithm::BoardInfo()
{
	// �t�B�[���h�̃Z�������擾
	const std::vector<std::vector<Field::AlgorithmCellInfo>>fieldCell = m_pField->GetCellInfo();

	// �������ϐ�
	const AlgorithmBoard initAlgorithmBoard;

	
	// ���ׂĂ̔Ֆʂ𒲂ׁA�t���O��������
	for (int z = 0; z < static_cast<int>(m_algorithmBoard.size()); z++)
	{
		for (int x = 0; x < static_cast<int>(m_algorithmBoard[z].size()); x++)
		{
			// �ϐ�������
			m_algorithmBoard[x][z] = initAlgorithmBoard;

			// ��Q�������邩�ǂ���
			m_algorithmBoard[x][z].isObstacle = fieldCell[x][z].isObstacle;

			// �v���C���[�����邩�ǂ���
			m_algorithmBoard[x][z].isPlayerPresent = fieldCell[x][z].isPlayerPresent;
	
		}
	}
}

bool Algorithm::IsRange(const Field::Cell& currentCell)
{
	// �͈͊O���m�F
	{
		if (currentCell.x < 0)return false;
		if (currentCell.x >= m_boardScale)return false;
		if (currentCell.z < 0)return false;
		if (currentCell.z >= m_boardScale)return false;
	}

	// �����Ԃ�
	return true;
}

bool Algorithm::IsObstacle(const Field::Cell &currentCell)
{
	// ��Q��������ꍇ�Atrue�ŕԂ�
	if (m_algorithmBoard[currentCell.x][currentCell.z].isObstacle)
	{
		return true;
	}

	// �����Ԃ�
	return false;
}

VECTOR Algorithm::ShortestDistance(const VECTOR& currentPos, const std::vector<VECTOR>& searchPos)
{
	// �x�N�g���̃X�P�[��
	std::vector<float> vecScale;

	// �x�N�g���̃X�P�[�������߂�
	for (auto& search : searchPos)
	{
		// ���������߂�
		VECTOR vel = VSub(search, currentPos);

		// ���߂��������m�}���C�Y���邱�Ƃɂ�苗���̒������P�Ƃ݂�
		vel = VNorm(vel);

		// �x�N�g���̃X�P�[������
		vecScale.push_back(VSize(vel));
	}

	// �ԍ�
	int number = 0;

	// �ŏ��X�P�[������
	float minScale = vecScale.front();

	// �ŏ��X�P�[�������߂�
	for (int i = 0; i < static_cast<int>(vecScale.size()); i++)
	{
		if (minScale > vecScale[i])
		{
			number = i;
			minScale = vecScale[i];
		}
	}

	// ���W��Ԃ�
	return searchPos[number];
}

void Algorithm::SortFactorAscendingOrder(std::vector<Cost>& cost)
{
	// �l���\�[�g����
	std::sort(cost.begin(), cost.end(), [](const Cost& a, const Cost& b) 
	{
		// �\�[�g���ꂽ�l��Ԃ�
		return a.value < b.value;
	});
}

Field::Cell Algorithm::ConvertPosToCell(const VECTOR& currentPos)
{
	// �Z�����擾
	const Field::Cell cellInfo = m_pField->GetConvertPosToCell(currentPos);

	// �Z���ɑ��
	Field::Cell cell = {};
	cell.x = cellInfo.x;
	cell.z = cellInfo.z;

	// �l��Ԃ�
	return cell;
}

VECTOR Algorithm::ConvertCellToPos(const Field::Cell& currentCell)
{
	// �ϊ����W
	VECTOR convertPos = VECTOR();

	// �l��������
	convertPos.x = m_pField->GetCriteriaPos().x + (m_pField->GetFieldCellModelScale() * currentCell.x);
	convertPos.z = m_pField->GetCriteriaPos().z + (m_pField->GetFieldCellModelScale() * currentCell.z);

	// �l��Ԃ�
	return convertPos;
}