#pragma once
class TitleScreen
{
public:
	TitleScreen();
	virtual ~TitleScreen();

	void Init();
	void Update();
	void Draw();

private:

	//////////////////////
	// �O���t�B�b�N�֘A //
	//////////////////////

	// �n���h��
	int m_handle;

	// �t���[��
	int m_frame;
};