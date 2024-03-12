#pragma once
#include <memory>
#include <vector>
#include "EvoLib.h"
#include <DxLib.h>
#include "MasterData.h"

class Field;
class AttackEffect;
class ResurrectionEffect;
class CharacterBase;
class EffectManager
{	
public:

	EffectManager();
	virtual ~EffectManager();

	
	/// <summary>
	/// �t�B�[���h�N���X�|�C���^�̎擾
	/// </summary>
	/// <param name="field">�t�B�[���h�|�C���^</param>
	void SetFieldPointer(std::shared_ptr<Field>field) { m_pField = field; }

	/// <summary>
	/// �L�����N�^�[�x�[�X�N���X�|�C���^�̎擾
	/// </summary>
	/// <param name="characterBase">�L�����N�^�[�x�[�X�|�C���^</param>
	void SetCharacterBasePointer(std::shared_ptr<CharacterBase>characterBase);


	void Init();
	void Update();
	void Draw();

public:

	/// <summary>
	/// �U���G�t�F�N�g����
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="directon">����</param>
	/// <param name="color">�F</param>
	/// <param name="charNum">�L�����ԍ�</param>
	void CreateAttackEffect(const VECTOR& pos, const Direction& directon,
		const VECTOR& color, const int& charNum);

	/// <summary>
	/// �����G�t�F�N�g����
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="color">�F</param>
	/// <param name="charNum">�L�����ԍ�</param>
	void CreateResurrectionEffect(const VECTOR& pos,const VECTOR& color, const int& charNum);

private:

	// �G�t�F�N�g���
	struct EffectInfo
	{
		// �G�t�F�N�g�n���h��
		int handle = -1;

		// �O���t�B�b�N�X�P�[��
		float scale = 0.0f;
	};


private:

	/// <summary>
	/// �n���h���̃��[�h
	/// </summary>
	void LoadHandle();

	/// <summary>
	/// �G�t�F�N�g�̍폜
	/// </summary>
	void DeleteEffect();

private:

	//////////////////////
	// �O���t�B�b�N�֘A //
	//////////////////////


	// �U���G�t�F�N�g���
	std::vector<EffectInfo> m_attackEffectInfo;

	// �����G�t�F�N�g���
	EffectInfo m_resurrectionEffectInfo;


	////////////////////////
	// �N���X�|�C���^�֘A //
	////////////////////////

	// �U���G�t�F�N�g�N���X�|�C���^
	std::vector<std::shared_ptr<AttackEffect>>m_pAttackEffect;

	// �����G�t�F�N�g�N���X�|�C���^
	std::vector<std::shared_ptr<ResurrectionEffect>>m_pResurrectionEffect;

	// �t�B�[���h�N���X�|�C���^
	std::shared_ptr<Field>m_pField;

	// �L�����N�^�[�x�[�X�|�C���^
	std::vector<std::shared_ptr<CharacterBase>>m_pCharacterBase;
};