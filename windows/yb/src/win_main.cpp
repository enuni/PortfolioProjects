 
#include "DxLib.h"

#include <io.h>
#include <fcntl.h>

#include "dxsdkver.h"
#include "assert.h"

#include "nn_common.h"

extern void main_loopMain( void);
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

#ifdef _DEBUG
	// DirectXバージョン確認 DXSDK_Jun10
	assert(_DXSDK_PRODUCT_MAJOR == 9 && _DXSDK_PRODUCT_MINOR == 29 && _DXSDK_BUILD_MAJOR == 1962);
#endif
#ifdef _DEBUG
	// コンソールの表示
	int hConsole;
	AllocConsole();
	hConsole = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	*stdout = *_fdopen(hConsole, "w");
	setvbuf(stdout, NULL, _IONBF, 0);
	//ログ表示レベル設定 このレベル以下のログを出力する
	::gLogViewLevel = 100;
#endif

	(void)ChangeWindowMode( TRUE);		// ウィンドウモードにする
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
	
	if(SetMainWindowText( "ゆっくりボウルDX Ver.1.13") == -1) {

	}
	
	if(DxLib_Init() == -1) {		// ＤＸライブラリ初期化処理
		return -1;
	}

	SetDXArchiveKeyString( "n2n_agrippa_yb");
	
	if(SetTransColor( 255, 0, 255) == -1) {		// 抜き色を設定
		
	}

	SetWaitVSyncFlag( FALSE);
	SetDrawScreen( DX_SCREEN_BACK);		// 描画先を裏画面に設定
	
#if 1
	main_loopMain();
#else
	// テストコード
	SetDrawScreen( DX_SCREEN_FRONT);		// 描画先を裏画面に設定
	DrawPixel( 320 , 240 , 0xffff ) ;	// 点を打つ
	
	WaitKey() ;				// キー入力待ち
#endif

	DxLib_End();		// ＤＸライブラリ使用の終了処理

	return 0;

}
