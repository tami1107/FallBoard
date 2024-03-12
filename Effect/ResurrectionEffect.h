#pragma once
#include <memory>
#include <vector>
#include "EvoLib.h"
#include <DxLib.h>

class CharacterBase;
class Field;
class ResurrectionEffect
{
public:
	ResurrectionEffect();
	virtual ~ResurrectionEffect();

	/// <summary>
	/// �t�B�[���h�|�C���^���擾
	/// </summary>
	/// <param name="field">�t�B�[���h�|�C���^</param>
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }

	/// <summary>
	/// �L�����N�^�[�x�[�X�|�C���^���擾
	/// </summary>
	/// <param name="characterBase">�L�����N�^�[�x�[�X�|�C���^</param>
	void SetCharacterBasePointer(std::shared_ptr<CharacterBase>characterBase) { m_pCharacterBase = characterBase; }



	void Update();
	void Draw();


	/// <summary>
	/// �폜�t���O��Ԃ�
	/// </summary>
	/// <returns>�폜�t���O</returns>
	bool GetIsDelete() { return m_isDelete; }


public:

	// �G�t�F�N�g�ݒ�
	struct EffectSetting
	{
		// �O���t�B�b�N�n���h��
		int handle = -1;

		// �O���t�B�b�N�X�P�[��
		float scale = 0.0f;

		// ���W
		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);

		// �F
		VECTOR color = VGet(1.0f, 1.0f, 1.0f);
	};

	/// <summary>
	/// �����ݒ�
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

	////////////////////////
	// �N���X�|�C���^�֘A //
	////////////////////////

	// �N���X�|�C���^
	std::shared_ptr<Field>m_pField;
	std::shared_ptr<CharacterBase>m_pCharacterBase;
};

