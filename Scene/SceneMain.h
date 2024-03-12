#pragma once

#include "SceneBase.h"
#include <memory>
#include "Vec2.h"


class MainScreen;
class SceneMain : public SceneBase
{
public:
	SceneMain();

	virtual ~SceneMain();


	virtual void Init() override;
	virtual void End() override {}

	virtual SceneBase* Update() override;
	virtual void Draw() override;

public:

	// �X�e�[�g��
	enum class State
	{
		Fade,			// �t�F�[�h
		Initialization, // ������
		Normal,			// �ʏ�
	};

	// �V�[���ύX��
	enum class ChangeScene
	{
		CharSelect,	// �L�����Z���N�g
		Main,		// ���C��
		Result,		// ���U���g
		Title,		// �^�C�g��
	};


public:

	/// <summary>
	/// ���[�h���s�����ǂ���
	/// </summary>
	/// <param name="isLoading">���[�h���s�����ǂ���</param>
	void IsASynchronousLoad(bool isLoading = false);

	/// <summary>
	/// ���[�f�B���O���������Ă��邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns>���[�f�B���O���������Ă��邩�ǂ�����Ԃ�</returns>
	bool GetLoading();

	/// <summary>
	/// �t�F�[�h�A�E�g���s��
	/// </summary>
	/// <param name="changeScene">�t�F�[�h�A�E�g���A�ǂ̃V�[���ɕύX���邩</param>
	void FadeOut(ChangeScene changeScene);

private:

	/// <summary>
	/// �t�F�[�h�A�E�g���A���񂾂񉹂��������Ȃ�T�E���h����
	/// </summary>
	void FadeSound();

private:

	////////////////
	// �V�[���֘A //
	////////////////

	// �V�[���ύX
	ChangeScene m_changeScene;

	////////////////////
	// �N���X�|�C���^ //
	////////////////////

	// ���C���X�N���[���̃N���X�|�C���^
	std::shared_ptr<MainScreen>m_pMainScreen;
};