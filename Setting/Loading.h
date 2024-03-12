#pragma once


class Loading
{
public:
	Loading();
	virtual ~Loading();

	void Init();
	void Update();
	void Draw();

	

	/// <summary>
	/// ���[�h�����ǂ�����Ԃ�
	/// </summary>
	/// <returns>���[�h�����ǂ���</returns>
	bool GetIsLoading();


private:

	/// <summary>
	/// �n���h���̃��[�h����
	/// </summary>
	void LoadHanele();

	/// <summary>
	/// �t�F�[�h�l�X�V����
	/// </summary>
	void UpdateFadeBright();

	/// <summary>
	/// ���[�h�t���[������
	/// </summary>
	void LoadFrame();

	/// <summary>
	/// �񓯊��������𐔂��鏈��
	/// </summary>
	void ASyncLoadingCount();

	/// <summary>
	/// ���[�h�O���t�B�b�N��`�悷��
	/// </summary>
	void DrawLoadGaph();

public:

	// �X�e�[�g
	enum class State
	{
		FadeIn,	// �t�F�[�h�C��
	};

private:

	//////////////////////
	// �O���t�B�b�N�֘A //
	//////////////////////

	// �n���h��
	int m_handle;

	// �T�C���t���[��
	int m_sinFrame;

	// �t�F�[�h�l
	int m_fadeBright;

	////////////////
	// ���[�h�֘A //
	////////////////

	// �t���[���J�E���g
	int m_frameCount;

	// �Œ���̃��[�h�t���[���𒴂������ǂ���
	bool m_isMinimumFrame;

	// ���[�h�����ǂ���
	bool m_isLoading;
};