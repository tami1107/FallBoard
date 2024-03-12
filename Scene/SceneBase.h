#pragma once
#include "StateMachine.h"// ������͈�ԏ�ɂ����Ă���
#include <memory>

// �Q�[���V�[�����N���X
class Loading;
class SceneBase
{
public:
	// �t�F�[�h���x
	static constexpr int kFadeSpeed = 5;
	static constexpr int kVolumeSpeed = 5;

public:
	SceneBase();
	virtual ~SceneBase() {};

	// �N���X�|�C���^�̎擾
	void setLoadingPointer(std::shared_ptr<Loading>loading) { m_pLoading = loading; }


	virtual void Init() {}
	virtual void End() {}

	virtual SceneBase* Update() { return this; }
	virtual void Draw() {}


	// ���[�h�֘A
	void LoadingInit(bool isLoading = false);
	bool IsLoadingProcess();




	// �t�F�[�h�֘A
	void UpdateFade();// �t�F�[�h�X�V
	void DrawFade(int fadeColor = 0x000000) const;// �t�F�[�h�`��
	bool IsFadingIn() const;// �t�F�[�h�C����
	bool IsFadingOut() const;// �t�F�[�h�A�E�g��
	bool IsFading() const { return IsFadingIn() || IsFadingOut(); }// �t�F�[�h�C��or�A�E�g��

	void StartFadeOut();// �t�F�[�h�A�E�g�J�n

	// �t�F�[�h�X�s�[�h�̕ύX
	void SetFadeSpeed(int fadeSetUpSpeed = kFadeSpeed);

	// �t�F�[�h�̒l��Ԃ�
	int FadeBrightValue() { return m_fadeBright; }

	// �V�[���I��
	virtual bool isEnd() { return false; }

private:
	// �N���X�|�C���^
	std::shared_ptr<Loading>m_pLoading;

	

protected:
	// �t�F�[�h�֘A����
	int m_fadeBright;
	int m_fadeSpeed;
	int m_fadeSetUpSpeed;


	bool m_isFadeOut;// �t�F�[�h�A�E�g���s���Ă��邩�ǂ���

	bool m_isBackScene;

	bool m_isLoading;// ���[�h���Ă��邩�ǂ���


};