#pragma once
#include "SceneBase.h"
#include <memory>

class ResultScreen;
class SceneResult : public SceneBase
{
public:
	SceneResult();

	virtual ~SceneResult();

	virtual void Init() override;
	virtual void End() override {}

	virtual SceneBase* Update() override;
	virtual void Draw() override;


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
	/// �t�F�[�h�A�E�g����
	/// </summary>
	void FadeOut();

private:

	/// <summary>
	/// �t�F�[�h���̃T�E���h����
	/// </summary>
	void FadeSound();

private:

	////////////////////////
	// �N���X�|�C���^�֘A //
	////////////////////////

	// ���U���g�V�[���|�C���^
	std::shared_ptr<ResultScreen>m_pResultScreen;
};

