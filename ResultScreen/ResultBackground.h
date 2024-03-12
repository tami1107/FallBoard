#pragma once
#include <vector>
#include "Vec2.h"
#include <memory>


class Field;
class ResultCharacter;
class ResultBackground
{
public:
	ResultBackground();
	virtual~ResultBackground();

	/// <summary>
	/// ���U���g�L�����N�^�[�|�C���^�擾
	/// </summary>
	/// <param name="resultCharacter">���U���g�L�����N�^�[�|�C���^</param>
	void SetResultCharacterPointer(std::shared_ptr<ResultCharacter>resultCharacter) { m_pResultCharacter = resultCharacter; }

	void Init();
	void Update();
	void Draw();

private:

	/// <summary>
	/// �G�t�F�N�g����������
	/// </summary>
	void InitEffect();

	/// <summary>
	/// �G�t�F�N�g�X�V����
	/// </summary>
	void UpdateEffect();

	/// <summary>
	/// �w�i�`�揈��
	/// </summary>
	void DrawBackground();

	/// <summary>
	/// ���U���g�e�L�X�g�`��
	/// </summary>
	void DrawResultText();

public:

	/// <summary>
	/// �n���h���̃��[�h����
	/// </summary>
	void LoadHandle();

	
	void DrawEffect();

	/// <summary>
	/// �����L���O�`�揈��
	/// </summary>
	void DrawRanking();

	/// <summary>
	/// �N���E���`�揈��
	/// </summary>
	void DrawCrown();

	/// <summary>
	/// ���̃V�[���̐����e�L�X�g��`��
	/// </summary>
	void DrawNextSceneText();
private:

	// �O���t�B�b�N
	struct Graphic
	{
		// �O���t�B�b�N�n���h��
		std::vector<int>handle;

		// ���W
		Vec2 pos = Vec2(0.0f, 0.0f);
	};

	// �G�t�F�N�g���
	struct EffectInfo
	{
		// �ړ���
		Vec2 vec = Vec2(0.0f, 0.0f);
		// ���W
		Vec2 pos = Vec2(0.0f, 0.0f);
	};


private:

	//////////////////////
	// �O���t�B�b�N�֘A //
	//////////////////////

	// �O���t�B�b�N���
	std::vector<Graphic>m_graph;


	//////////////
	// ���o�֘A //
	//////////////

	// �w�i�O���t�B�b�N�̈ړ���
	float m_backgroundMoveValue;

	// ���U���g�e�L�X�g�̃T�C���t���[��
	int m_resultTextSinFrame;

	// �N���E���O���t�B�b�N�̃T�C���t���[��
	int m_crownSinFrame;

	// �_�Ńt���[��
	int m_flashingFrame;

	// �G�t�F�N�g���
	std::vector<EffectInfo>m_effect;


	////////////////////////
	// �N���X�|�C���^�֘A //
	////////////////////////

	// �t�B�[���h�|�C���^
	std::shared_ptr<Field>m_pField;

	// ���U���g�L�����N�^�[�|�C���^
	std::shared_ptr<ResultCharacter>m_pResultCharacter;
};