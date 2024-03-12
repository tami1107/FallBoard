#include <DxLib.h>
#include "Game.h"

#include "SceneManager.h"

#include "Sound.h"
#include "ModelData.h"
#include "EvoLib.h"
#include "SoundSetting.h"
#include "MasterData.h"


// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// window���[�h�ݒ�
	ChangeWindowMode(Game::kWindowMode);
	// �E�C���h�E���ݒ�
	SetMainWindowText(Game::kTitleText);
	// 
	SetWindowSizeChangeEnableFlag(true);
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);




	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}



	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// �Q�[���f�[�^�̃��[�h
	MasterData::getInstance().LoadData();

	// �T�E���h�̏�������
	Sound::getInstance().Init();

	// �T�E���h�ݒ�̏�������
	SoundSetting::getInstance().Init();

	// ���f���̃��[�h(�񓯊��ǂݍ���)
	ModelData::getInstance().Load();


	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBufferFlag(TRUE);		// �y�o�b�t�@���g�p����
	SetUseBackCulling(TRUE);		// �o�b�N�J�����O���s��

	// �ŏ��̃V�[���̏�����
	SceneManager scene;
	scene.Init();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
		ClearDrawScreen();

		scene.Update();
		scene.Draw();

		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < FPS_Lib::Dxlib_60FPS)
		{
		}
	}
	// �T�E���h�̏I��
	Sound::getInstance().UnLoad();
	Sound::destroy();

	ModelData::getInstance().UnLoad();

	SoundSetting::getInstance().UnLoad();


	scene.End();			// �V�[���̏I������

	DxLib_End();			// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}