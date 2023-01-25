
#ifndef __FONT_H__
#define __FONT_H__

/*
#include <nnsys.h>
*/
#include "type.h"

//---------------------------------------------------------------------------

enum {
	FONT_COLOR_NONE,
	FONT_COLOR_WHITE,
	FONT_COLOR_BLACK,
	FONT_COLOR_RED,
	FONT_COLOR_GREEN,
	FONT_COLOR_BLUE,
	FONT_COLOR_YELLOW,
	FONT_COLOR_PINK,
	FONT_COLOR_AQUA,
	FONT_COLOR_DUMMY_1,//FONT_COLOR_LIME,
	FONT_COLOR_DUMMY_2,//FONT_COLOR_MAGENTA,
	FONT_COLOR_DUMMY_3,//FONT_COLOR_GRAY
	FONT_COLOR_DUMMY_4,//FONT_COLOR_PURPLE,
	FONT_COLOR_DUMMY_5,//FONT_COLOR_MAROON,
	FONT_COLOR_DUMMY_6,//FONT_COLOR_SILVER,
	FONT_COLOR_DUMMY_7,//FONT_COLOR_GOLD,
	
	FONT_COLOR_MAX
};

enum {
	FONT_COLOR_00 = FONT_COLOR_NONE,
	FONT_COLOR_01 = FONT_COLOR_WHITE,
	FONT_COLOR_02 = FONT_COLOR_BLACK,
	FONT_COLOR_03 = FONT_COLOR_GREEN,
	FONT_COLOR_04 = FONT_COLOR_BLUE,
	FONT_COLOR_05 = FONT_COLOR_BLACK,
	FONT_COLOR_06 = FONT_COLOR_YELLOW,
	FONT_COLOR_07 = FONT_COLOR_PINK,
	FONT_COLOR_08 = FONT_COLOR_AQUA,
	
	FONT_COLOR_NUM_MAX
};
//---------------------------------------------------------------------------
#if 0
//特に初期化処理なし
void fontInit( void);
#endif
void fontInitFont( NNSG2dFont * font, void * fontData);
void fontSetTypeSimple( int typeIndex, char * fontName, int pixelSize);
void fontDestructFont();

void fontTask( void);			//毎フレーム フォントの描画支持に先がけ呼ぶ
void fontSetAutoDispNoFlag( BOOL flag);			//フォント任意更新か毎フレーム自動更新か設定
void fontSetAutoDispNoDispFlag( void);			//fontSetNotAutoDisp()時のフォント更新支持

void fontMainBg1Init( void);
void fontMainBg1Reflect(void);
void fontMainClear( void);
void fontMainPuts( int x, int y, int colorNo, char * s);
void fontMainPrintfRight( int x, int y, int colorNo, char* fmt, ... );
void fontMainPutsCenter( int x, int y, int colorNo, char * s);
void fontMainPutsRight( int x, int y, int colorNo, char * s);
void fontMainPrintf( int x, int y, int colorNo, char* fmt, ... );
void fontMainPrintfCenter( int x, int y, int colorNo, char* fmt, ... );
void fontMainPrintfRight( int x, int y, int colorNo, char* fmt, ... );
void fontMainResetOffScreen( void);

void fontSubBg1Init(void);
void fontSubBg1Reflect( void);
void fontSubClear( void);
void fontSubPuts( int x, int y, int colorNo, char * s);
void fontSubPutsCenter( int x, int y, int colorNo, char * s);
void fontSubPutsRight( int x, int y, int colorNo, char * s);
void fontSubPrintf( int x, int y, int colorNo, char* fmt, ... );
void fontSubPrintfCenter( int x, int y, int colorNo, char* fmt, ... );
void fontSubPrintfRight( int x, int y, int colorNo, char* fmt, ... );
void fontSubResetOffScreen( void);

void * fontGetMainFontData(void);
void * fontGetSubFontData(void);

int fontMainPutc( int x, int y, int colorNo, u16 c);
int fontSubPutc( int x, int y, int colorNo, u16 c);
int fontGetMainH( void);
int fontGetSubH( void);

void fontSetStopFlag( BOOL flag);
void fontMainPrintfSystemMsg( int x, int y, int colorNo, char * fmt, ... );
void fontSubPrintfSystemMsg( int x, int y, int colorNo, char * fmt, ... );

void fontPrintfType( int x, int y, int type, int color, char * fmt, ...);
void fontPutsType( int x, int y, int type, int color, char * s);
int fontPutcType( int x, int y, int type, int color, u16 c);
GXRgb fontColorNo2Color( int colorNo);

//そのフレームの表示の座標オフセットを設定する
void fontSetOffset( int hOffset, int vOffset);

void fontShow( void);
//---------------------------------------------------------------------------
#endif //__FONT_H__

