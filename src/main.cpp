/*
	������������������������������������������������������������������������������������������������������������������������
	�������������������������������������������������������������A����������������������������������������������������������
	������������������������������������������������������������������������������������������������������������������������
	���������������������������������ʂ��悢�B������������������������������������������������������������������������������
	������������������������������������������������������������������������������������������������������������������������

	�@�@�@�@�@�@�����@�@���������@���@�@���@�������@�������@�@���������@���@�@�@���@�@�������@���@�@�@���@�@����
	�@�@�@�@�@���@�@�@�@���@�@�@�@���@���@�@�@���@�@���@�@���@���@�@�@�@�����@�@���@���@�@�@�@���@�@�@���@���@�@��
	�@�@�@�@�@���@�����@�������@�@�����@�@�@�@���@�@�������@�@�������@�@���@���@���@�@�����@�@�����������@��������
	�@�@�@�@�@���@�@���@���@�@�@�@���@���@�@�@���@�@���@�@���@���@�@�@�@���@�@�����@�@�@�@���@���@�@�@���@���@�@��
	�@�@�@�@�@�@�����@�@���������@���@�@���@�������@���@�@���@���������@���@�@�@���@�������@�@���@�@�@���@���@�@�� 68k

�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�`�@��D���Ȓ��A��68k�̂��߂Ɂ@�`												  
�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
	 ������������������������������������������������������������������������������������������������������������������������
	 �������������ˌ���������������������������������������������������������������������������������������������������������
	 ������������������������������������������������������������������������������������������������������������������������

     // �ǂ����Ɂu���v�u�A�v�u�ˁv���B��Ă����I�T���Ă݂悤�I
*/
#define _CRTDBG_MAP_ALLOC
#include "DxLib.h"

#include "Keyboard.hpp"
#include "SceneMng.hpp"
#include "DebugMode.hpp"
#include "Sound.hpp"

#include <memory>
#include <time.h>
#include <DirectXMath.h>

constexpr int SC_W = 640;
constexpr int SC_H = 480;

static int  FrameStartTime;			// 60fps�Œ��p
static int  FPS;
static bool ScSizeFrag = true;		// ��ʃ��[�h�ύX�p
static bool quit       = false;		// �����I���t���O

void QuitGame();					// �Q�[���I���`�B�֐�

void message_box()
{
	int flag;

	flag = MessageBox(
		NULL,
		TEXT("�t���X�N���[�����[�h�ŋN�����܂����H\n�iF5�L�[�ł��ύX�ł��܂��B�j"),
		TEXT("�X�N���[���ݒ�"),
		MB_YESNOCANCEL | MB_ICONQUESTION);
	
	if (flag == IDNO)
	{
		ScSizeFrag = true;
//		SetUseBackBufferTransColorFlag(true); // �S���������t���O
	}
	if (flag == IDCANCEL)	QuitGame();
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if !defined(_DEBUG)
	message_box();																				// �E�B���h�E�X�^�C������
#endif
	if (DirectX::XMVerifyCPUSupport() == FALSE)
	{
		MessageBox(NULL, "CPU is not XMVerifyCPUSupport.", "ERROR", MB_OK);
		return -1;
	}
	SetWindowIconID(444);
	SetGraphMode(SC_W, SC_H, 32), ChangeWindowMode(ScSizeFrag), DxLib_Init();					// �E�B���h�E�����ݒ�(VGA),DxLib�N��
	SetMainWindowText("���A��");																// �^�C�g����ݒ�
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);											// ��ʃ��[�h�ύX��A�f�ރ����������Z�b�g���Ȃ�
	SetWaitVSyncFlag(FALSE);																	// ���������M����҂��Ȃ�
	FrameStartTime = GetNowCount();																// �J�n���Ԃ�ݒ�
	FPS = 60;
	Sound::Load();
	std::unique_ptr<SceneMng> sceneMng(new SceneMng);											// �V�[���Ǘ�
	SRand((unsigned)time(NULL));																// �����V�[�h
	LoadPauseGraph("GRAPH/Cover.png");															// ��A�N�e�B�u��Ԏ��̉摜

	// �Q�[��--------------------------------------------------------------------------------------
	while (ProcessMessage() == 0 && SetDrawScreen(DX_SCREEN_BACK) == 0 && ClearDrawScreen() == 0 && !quit)
	{
		while (GetNowCount() - FrameStartTime < 1000 / FPS) {}									// 1/60 �b�܂ő҂�
		FrameStartTime = GetNowCount();															// ���݂̃J�E���g��ۑ�

		sceneMng->Update();																		// ���݂̃V�[���f�[�^���X�V
		Keyboard::Instance()->Update();															// �L�[���̓f�[�^�X�V

		if (Keyboard::Instance()->isPush(KEY_INPUT_ESCAPE))	break;								// [Esc]�ŋ^�������I��
		if (Keyboard::Instance()->isPush(KEY_INPUT_F5))
			ChangeWindowMode(ScSizeFrag = !ScSizeFrag);											// ��ʃ��[�h�ύX
		if (Keyboard::Instance()->isPush(KEY_INPUT_F))	FPS = 30;								// �������
		if (Keyboard::Instance()->isPush(KEY_INPUT_G))	FPS = 60;								// ����

		sceneMng->Draw();																		// ���݂̃V�[����`��
		DebugMode::Draw();

		ScreenFlip();	//�@����ʂ�\��ʂɔ��f
	}
	Sound::Delete();
	LoadPauseGraph(NULL);
	DxLib_End();																				// �c�w���C�u�����g�p�̏I������
	return 0;																					// �\�t�g�̏I�� 
}


void QuitGame(){
	quit = true;
}