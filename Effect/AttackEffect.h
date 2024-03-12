#pragma once
#include <memory>
#include <vector>
#include "EvoLib.h"
#include <DxLib.h>


class Field;
class AttackEffect
{
public:

	AttackEffect();
	virtual ~AttackEffect();


	/// <summary>
	/// �t�B�[���h�|�C���^���擾
	/// </summary>
	/// <param name="field">�t�B�[���h�|�C���^</param>
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }


	void Update();
	void Draw();


	/// <summary>
	/// �폜�t���O��Ԃ�
	/// </summary>
	/// <returns>�폜�t���O/returns>
	bool GetIsDelete() { return m_isDelete; }

private:
	
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// ���W�ϊ�����
	/// </summary>
	void ConvertPos();

	/// <summary>
	/// �c���`�揈��
	/// </summary>
	void DrawAfterEffect();

public:

	// �G�t�F�N�g�ݒ�
	struct EffectSetting
	{
		// �O���t�B�b�N�n���h��
		int handle = -1;

		// �O���t�B�b�N�X�P�[��
		float graphScale = 0.0f;

		// ���W
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);

		// ����
		Direction directon = Direction::Right;

		// �p�x
		float angle = 0.0f;

		// �F
		VECTOR color = VGet(1.0f, 1.0f, 1.0f);

		// �ړ��X�s�[�h
		float moveSpeed = 0.0f;
	};



public:

	/// <summary>
	/// �G�t�F�N�g�����ݒ�
	/// </summary>
	/// <param name="effectSetting">�G�t�F�N�g�ݒ�l</param>
	void Init(const EffectSetting& effectSetting);

private:

	////////////////////
	// �G�t�F�N�g�֘A //
	////////////////////

	// �G�t�F�N�g���
	EffectSetting m_effectInfo;

	// �폜���邩�ǂ����̃t���O
	bool m_isDelete;

	// �c�����W
	std::vector<VECTOR>m_afterPos;

	////////////////////////
	// �N���X�|�C���^�֘A //
	////////////////////////

	// �t�B�[���h�N���X�|�C���^
	std::shared_ptr<Field>m_pField;
};