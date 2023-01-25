
#include "DxLib.h"

#include <io.h>
#include <fcntl.h>
#include <boost/version.hpp>

extern void main_loopMain( void);

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

#if _DEBUG
	// �R���\�[���̕\��
	int hConsole;
	AllocConsole();
	hConsole = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	*stdout = *_fdopen(hConsole, "w");
	setvbuf(stdout, NULL, _IONBF, 0);
#endif

	printf("boost version=%lu\n", BOOST_VERSION);
	if(BOOST_VERSION != 106400) {
		printf("missmatched version in boost.\n");
	}

	(void)ChangeWindowMode( TRUE);		// �E�B���h�E���[�h�ɂ���
	if(SetGraphMode( 854, 480, 32) != DX_CHANGESCREEN_OK) {

	}
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
	
	if(SetMainWindowText( "�����Փˌ� C93 1.01") == -1) {

	}
	
	if(DxLib_Init() == -1) {		// �c�w���C�u��������������
		return -1;
	}

	SetDXArchiveKeyString( "kotomi0715");
	
	if(SetTransColor( 255, 0, 255) == -1) {		// �����F��ݒ�
		
	}
	
	SetDrawScreen( DX_SCREEN_BACK);		// �`���𗠉�ʂɐݒ�
	
#if 1
	main_loopMain();
#else
	DrawPixel( 320 , 240 , 0xffffff ) ;	// �_��ł�
	ScreenFlip();
	
	WaitKey() ;				// �L�[���͑҂�
#endif

	DxLib_End();		// �c�w���C�u�����g�p�̏I������

	return 0;

}
