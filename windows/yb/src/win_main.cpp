 
#include "DxLib.h"

#include <io.h>
#include <fcntl.h>

#include "dxsdkver.h"
#include "assert.h"

#include "nn_common.h"

extern void main_loopMain( void);
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

#ifdef _DEBUG
	// DirectX�o�[�W�����m�F DXSDK_Jun10
	assert(_DXSDK_PRODUCT_MAJOR == 9 && _DXSDK_PRODUCT_MINOR == 29 && _DXSDK_BUILD_MAJOR == 1962);
#endif
#ifdef _DEBUG
	// �R���\�[���̕\��
	int hConsole;
	AllocConsole();
	hConsole = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	*stdout = *_fdopen(hConsole, "w");
	setvbuf(stdout, NULL, _IONBF, 0);
	//���O�\�����x���ݒ� ���̃��x���ȉ��̃��O���o�͂���
	::gLogViewLevel = 100;
#endif

	(void)ChangeWindowMode( TRUE);		// �E�B���h�E���[�h�ɂ���
	if(SetGraphMode( 640, 480, 32) != DX_CHANGESCREEN_OK) {

	}
#ifdef _DEBUG
	FILE * fp = fopen( "init.txt", "r");
	if(fp) {
		char buf[256];
		fgets( buf, sizeof( buf), fp);
		int win_x = atoi( buf);
		fgets( buf, sizeof( buf), fp);
		int win_y = atoi( buf);
		//printf( "win_x = %d win_y = %d\n", win_x, win_y);
		SetWindowInitPosition( win_x, win_y);
		fclose( fp);
	}
#endif
	
	if(SetMainWindowText( "�������{�E��DX Ver.1.13") == -1) {

	}
	
	if(DxLib_Init() == -1) {		// �c�w���C�u��������������
		return -1;
	}

	SetDXArchiveKeyString( "n2n_agrippa_yb");
	
	if(SetTransColor( 255, 0, 255) == -1) {		// �����F��ݒ�
		
	}

	SetWaitVSyncFlag( FALSE);
	SetDrawScreen( DX_SCREEN_BACK);		// �`���𗠉�ʂɐݒ�
	
#if 1
	main_loopMain();
#else
	// �e�X�g�R�[�h
	SetDrawScreen( DX_SCREEN_FRONT);		// �`���𗠉�ʂɐݒ�
	DrawPixel( 320 , 240 , 0xffff ) ;	// �_��ł�
	
	WaitKey() ;				// �L�[���͑҂�
#endif

	DxLib_End();		// �c�w���C�u�����g�p�̏I������

	return 0;

}
