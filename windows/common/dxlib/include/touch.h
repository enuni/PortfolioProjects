#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "type.h"

//#define __PRC__			//図形認識を入れる

#ifndef __TOUCH_WORK__
#define __TOUCH_WORK__

typedef struct _touch_work {
	s16 touchPoint;	//タッチイベント 場所が正確
	s16 touchDisp;	//タッチイベント 場所が不正確でも
	s16 first_x;	//現在のタッチイベントで最初にタッチした場所のx座標
	s16 first_y;	//現在のタッチイベントで最初にタッチした場所のy座標
	s16 release_x;	//現在のタッチイベントで放した場所のx座標
	s16 release_y;	//現在のタッチイベントで放した場所のy座標
	s16 x;	//現在タッチペンがタッチしている場所のx座標
	s16 y;	//現在タッチペンがタッチしている場所のy座標
	
	s16 slideStartX;
	s16 slideStartY;
} TOUCH_WORK;

#endif //__TOUCH_WORK__

#define TOUCH_TOUCH					0x0001	//タッチした瞬間
#define TOUCH_TOUCH_LONG			0x0002	//長押し
#define TOUCH_TOUCH_LONG_LONG		0x0004	//超長押し
#define TOUCH_DOUBLE_TOUCH			0x0008	//ダブルクリック
#define TOUCH_PRESS					0x0010	//タッチ中
#define TOUCH_PULSE					0x0020	//連続タッチ中一定時間ごとに発生
#define TOUCH_RELEASE				0x0040	//放した瞬間
#define TOUCH_RELEASE_LONG			0x0080	//放してから経過
#define TOUCH_RELEASE_LONG_LONG		0x0100	//放してから超経過

#define TOUCH_SLIDE_UP				0x0200	//上へスライド
#define TOUCH_SLIDE_DOWN			0x0400	//下へスライド
//---------------------------------------------------------------------------

void touchInit( void);
void touchTask( TOUCH_WORK * touchWork);
void touchSetTouchLongNum( u8 setNum);

#ifdef __PRC__
//図形認識の初期化 開始 内部heapAlloc()を呼び メモリを確保
//現状 パターンのリストは .cpp の静的なものをリンクする 将来的にファイル化予定
void touchStartPrc( void);
//終了処理 StartPrc() で確保した メモリを開放する
void touchEndPrc( void);
//touchTask()で 図形認識用のタッチの履歴を記録するフラグを設定
void touchSetPrcAppendPointFlag( BOOL flag);
//記録したタッチ履歴から kind で指定した任意のパターンの中で 最も近い文字を resultCharArray にいれる kind = (0xffffffffU)で全てのパターン
//有効な文字が認識できたら TRUE を返す
BOOL touchGetPrcBlock( char * resultCharArray, u32 kind);

//タッチ履歴を消す
void touchClearPrc( void);

//図形認識の入力範囲を設定する
void touchSetPrcArea( int x1, int y1, int x2, int y2);
//図形認識に入力されている点の個数を返す
int touchGetPrcPointN( void);
//図形認識に入力されている i 番目の x座標
int touchGetPrcPointX( int i);
//図形認識に入力されている i 番目の y座標
int touchGetPrcPointY( int i);
#endif //__PRC__
//---------------------------------------------------------------------------

#endif //__TOUCH_H__



