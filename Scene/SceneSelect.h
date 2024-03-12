#pragma once
#include "SceneBase.h"


class SelectScreen;
class SceneSelect : public SceneBase
{
public:
	SceneSelect();

	virtual ~SceneSelect();


	virtual void Init() override;
	virtual void End() override {}

	virtual SceneBase* Update() override;
	virtual void Draw() override;


public:

	// �V�[���ύX��
	enum class ChangeScene
	{
		Main,		// ���C��
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
	/// <param name="changeScene">�t�F�[�h�A�E�g�I�����A�ǂ̃V�[���ɕύX���邩</param>
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

	////////////////////////
	// �N���X�|�C���^�֘A //
	////////////////////////

	//  �Z���N�g�X�N���[���̃N���X�|�C���^
	std::shared_ptr<SelectScreen>m_pSelectScreen;
};

