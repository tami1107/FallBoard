#pragma once
#include <Dxlib.h>
#include <vector>
#include "ModelData.h"

class ResultCharacter
{
public:
	ResultCharacter();

	virtual~ResultCharacter();

	void Init();
	void Update();
	void Draw();

	/// <summary>
	/// ���f���̍��W��Ԃ�
	/// </summary>
	/// <returns>���f���̍��W</returns>
	std::vector<VECTOR> GetModelPos() {return m_pos; }

private:

	/// <summary>
	/// �������W����
	/// </summary>
	void InitPos();

	/// <summary>
	/// ���[�V�����X�V����
	/// </summary>
	void UpdateMotion();

	/// <summary>
	/// ���f���`�揈��
	/// </summary>
	void DrawModel();

private:

	////////////////
	// ���f���֘A //
	////////////////

	// ���W
	std::vector<VECTOR>m_pos;

	// ���f���f�[�^�ݒ�
	std::vector<ModelData::DrawModelData> m_modelData;

	////////////////////
	// ���[�V�����֘A //
	////////////////////

	// �p�x
	int m_angle;
};