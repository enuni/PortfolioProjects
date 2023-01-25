#ifndef __DISP_COMMON_H__
#define __DISP_COMMON_H__

#include "task.h"
/*
#include "master.h"
*/

#include "obj.h"
#include "layout.h"

#define DISP_COMMON_RGB( r, g, b) ((unsigned long)(((unsigned char)(b)|((unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(r))<<16)))
#define DISP_COMMON_R( rgb) (rgb >> 16 & 0xff)
#define DISP_COMMON_G( rgb) (rgb >> 8 & 0xff)
#define DISP_COMMON_B( rgb) (rgb & 0xff)

#define RGB_WHITE (GX_RGB( 31, 31, 31))
#define RGB_BLACK_HLAF (GX_RGB( 16, 16, 16))
#define RGB_BLACK (GX_RGB( 0, 0, 0))
#define RGB_RED (GX_RGB( 31, 0, 0))
#define RGB_GREEN (GX_RGB( 0, 31, 0))
#define RGB_BLUE (GX_RGB( 0, 0, 31))

#define ALPHA_DEFAULT (31)
#define ROTATE_DEFAULT (0)

enum {
	DISP_COMMON_BLEND_MODE_NONE,	//なし
	DISP_COMMON_BLEND_MODE_ALPHA,	//αブレンド
	DISP_COMMON_BLEND_MODE_ADD,	//加算ブレンド
	DISP_COMMON_BLEND_MODE_SUB,	//減算ブレンド
	DISP_COMMON_BLEND_MODE_MULA,	//乗算ブレンド
	DISP_COMMON_BLEND_MODE_INVSRC,	//反転ブレンド
	
	DISP_COMMON_BLEND_MODE_MAX
};

//---------------------------------------------------------------------------

typedef struct _sub_obj_z_buf {
	s16 objId;
	s16 x;
	s16 y;
	s16 z;
	s8 alphaHalfFlag;
	s8 flipVFlag;
	s8 flipHFlag;
} SUB_2D_OBJ_Z_BUF;

typedef struct _disp_obj DISP_OBJ;

//---------------------------------------------------------------------------

//オブジェ表示　最も一般的なもの
void disp_commonObj( int objId, int x, int y, int z);
//オブジェ表示　レイアウト指定版　 x y z座標をずらすことが可能
void disp_commonObjShift( int layoutId, int xShift, int yShift, int zShift);
//オブジェ表示　レイアウト指定版　指定したレイアウト情報のうち表示 オブジェの変更が可能　rgba各値は(0～31)
void disp_commonObjSimpleColor( int layoutId, GXRgb rgb, int alpha);
//オブジェ表示　レイアウト指定版　最も簡単なもの
void disp_commonObjSimple( int layoutId);
//オブジェ表示　レイアウト指定版　指定したレイアウト情報のうち表示 オブジェの変更が可能
void disp_commonObjChangeObjId( int layoutId, int objId);
//オブジェ表示　レイアウト指定版　指定したレイアウト情報のうち表示 オブジェの変更 x y z座標をずらすことが可能
void disp_commonObjChangeShift( int layoutId, int objId, int xShift, int yShift, int zShift);
//オブジェ表示　座標色アルファ指定版　rgba各値は(0～31)
void disp_commonObjColor( int objId, int x, int y, int z, GXRgb rgb, u16 alpha);
//オブジェ表示　レイアウト指定表示拡張版　rgba各値は(0～31)
void disp_commonObjLayoutEx( int layoutId, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
//オブジェ表示　中心座標版　x yはオブジェの中心座標　rgba各値は(0～31)
void disp_commonObjCenter( int objId, int x, int y, int w, int h, int z, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
//オブジェ表示　拡張版　rgba各値は(0～31)
void disp_commonObjEx( int objId, int x, int y, int z, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
//オブジェ表示　全機能版　plygonIdはwinの場合0にする　下位ライブラリの仕様上 縦横の拡大率が異なる場合回転 フリップは無効になる　rgba各値は(0～31)
void disp_commonObjFull( int polygonId, int objId, int x, int y, int z, int w, int h, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
//オブジェ表示　全機能版　x yはオブジェの中心座標　plygonIdはwinの場合0にする　下位ライブラリの仕様上 縦横の拡大率が異なる場合回転 フリップは無効になる　rgba各値は(0～31)
void disp_commonObjCenterFull( int polygonId, int objId, int x, int y, int z, int w, int h, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
//オブジェ表示　Dx版　blendはenum DISP_COMMON_BLEND_MODE参照　ブレンドパラメータはalpha使用　rgba各値は(0～31)
void disp_commonObjDxFull( int objId, int x, int y, int z, int w, int h, u16 r, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend);
//オブジェ表示　Dx版　x yはオブジェの中心座標　blendはenum DISP_COMMON_BLEND_MODE参照　ブレンドパラメータはalpha使用　rgba各値は(0～31)
void disp_commonObjDxCenterFull( int objId, int x, int y, int z, int w, int h, u16 r, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend);
//オブジェ表示 中心座標版 の最も一般的な物
void disp_commonObjCenterBasic( int objId, int x, int y, int z);
//オブジェ表示 中心座標版 の座標色アルファ指定版　rgba各値は(0～31)
void disp_commonObjCenterColor( int objId, int x, int y, int z, GXRgb rgb, u16 alpha);
//そのフレームの表示の座標オフセットを設定する
void disp_commonSetOffset( int hOffset, int vOffset);

//カメラを有効にすると表示の原点が「カメラ中心(x, y) - カメラ表示領域の-1/2(w / 2, h / 2)」になる
//オフセット disp_commonSetOffset() と併用可
//カメラを有効無効を設定
void disp_commonSetCameraFlag( BOOL flag);
//カメラ表示領域を設定
void disp_commonSetCameraSize( int w, int h);
//カメラの中心座標を設定
void disp_commonSetCameraPos( int x, int y);

//オブジェIDからオブジェ情報を得る
OBJ * disp_commonGetObj( int id);
//レイアウトIDからレイアウト情報を得る
LAYOUT * disp_commonGetLayoutMain3d( int id);

//オブジェ情報の設定
void disp_commonSetObjData( int index, void * objTop, int idStart, int idEnd);
//レイアウト情報の設定
void disp_commonSetLayoutMain3dData( int index, void * layoutTop, int idStart, int idEnd);

///////////////////////////////////////////////////////////////////////////////
//disp_commonObj～と同じもの 互換性のために存在
void disp_commonMain3dObj( int objId, int x, int y, int z);
void disp_commonMain3dObjShift( int layoutId, int xShift, int yShift, int zShift);
void disp_commonMain3dObjSimpleColor( int layoutId, GXRgb color, int alpha);
void disp_commonMain3dObjSimple( int layoutId);
void disp_commonMain3dObjChangeObjId( int layoutId, int objId);
void disp_commonMain3dObjChangeShift( int layoutId, int objId, int xShift, int yShift, int zShift);
void disp_commonMain3dObjColor( int objId, int x, int y, int z, GXRgb rgb, u16 alpha);
void disp_commonMain3dObjLayoutEx( int layoutId, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
void disp_commonMain3dObjCenter( int objId, int x, int y, int w, int h, int z, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
void disp_commonMain3dObjEx( int objId, int x, int y, int z, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
void disp_commonMain3dObjFull( int polygonId, int objId, int x, int y, int z, int w, int h, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
void disp_commonMain3dObjCenterFull( int polygonId, int objId, int x, int y, int z, int w, int h, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
void disp_commonShowMain3dObj( void);
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//以下未整理
//メイン画面に n の値を 0～9その後-の連続したオブジェの0の objId のオブジェで 1桁目が x y zの位置から 上位桁まで w の間隔で表示する
void disp_commonMain3dObjNumber( int n , int objId, int x, int y, int z, int w);
void disp_commonMain3dObjNumberPlumi( int n, int objId, int x, int y, int z, int w);
void disp_commonMain3dObjNumberZero( int n, int figureCnt, int objId, int x, int y, int z, int w);
void disp_commonMain3dObjNumberColor( int n, int objId, int x, int y, int z, int w, GXRgb color, u16 alpha);
void disp_commonMain3dObjNumberZeroColor( int n, int figureCnt, int objId, int x, int y, int z, int w, GXRgb color, u16 alpha);
void disp_commonMain3dObjNumberCenter( int n, int objId, int xCenter, int y, int z, int w);

void disp_commonMain3dBoardSimple( int layoutId, GXRgb rgb);
//コの字順
void disp_commonMain3dBoardSimpleColor4( int layoutId, GXRgb * rgb);
void disp_commonMain3dBoard( int x, int y, int z, int w, int h, GXRgb rgb, u16 alpha);
//コの字順
void disp_commonMain3dBoardColor4( int x, int y, int z, int w, int h, GXRgb * rgb, u16 alpha);
//コの字順
void disp_commonMain3dBoardFull( int id, int x, int y, int z, int w, int h, u16 rot, GXRgb * rgb, u16 alpha);
void disp_commonMain3dBoardCenter( int x, int y, int z, int w, int h, u16 rot, GXRgb rgb, u16 alpha);

void disp_commonGetHCenterLineUpPosMain3d( int layoutId, int objId, int maxN, int n, int * xP, int *yP);
void disp_commonGetVCenterLineUpPosMain3d( int layoutId, int objId, int maxN, int n, int * xP, int *yP);
void disp_commonGetHvCenterLineUpPosMain3d( int layoutId, int objId, int maxHN, int maxVN, int hN, int vN, int * xP, int * yP);

void disp_commonMain3dLine( int x1, int y1, int x2, int y2, int z, int w, GXRgb color, int alpha);

void disp_commonFontMainPutsCenter( int layoutId, int colorNo, char * argS);



CHARACTER * disp_commonGetChar( int id);
LAYOUT * disp_commonGetLayoutSub( int id);

void disp_commonSetCharData( int index, void * charTop, int idStart, int idEnd);
void disp_commonSetLayoutSubData( int index, void * layoutTop, int idStart, int idEnd);

// nは 1以上 objId 一～九 の連番 objId 十百千万 の連番
void disp_commonSub2dObjNumberKanji( int n, int objIdNum, int objIdUnit, int x, int y, int z, int w);

//サブ画面に n の値を 0～9その後-の連続したオブジェの0の objId のオブジェで 1桁目が x y zの位置から 上位桁まで w の間隔で表示する
void disp_commonSub2dObjNumber( int n, int objId, int x, int y, int z, int w);

//disp_commonSub2dObjNumber()を figureCnt の桁数で 足りない桁には 0を入れて 表示する
void disp_commonSub2dObjNumberZero( int n, int figureCnt, int objId, int x, int y, int z, int w);

void disp_commonSub2dObj( int charId, int x, int y, int z);
void disp_commonSub2dObjShift( int layoutId, int xShift, int yShift, int zShift);
void disp_commonSub2dObjSimple( int layoutId);
void disp_commonSub2dObjEx( int charId, int x, int y, int z, BOOL flipH, BOOL flipV);
void disp_commonShowSub2dObj( void);

void disp_commonSub2dBg( int bgId);
void disp_commonShowSub2dBg( void);

void disp_commonPanic( const char * format, ...);

// 101014 DXlibマスク機能追加
void disp_commonStartMask();
void disp_commonEndMask();
void disp_commonMain3dMask( int texture_id, int x, int y, int z);
void disp_commonMain3dMaskRelease( int z);

// 101029
// 任意のテキスチャー部分切り出し描画
void disp_commonObjUv( int textureId, int u, int v, int w, int h, int x, int y, int z);

// 101223
void disp_commonObjUvFull( int textureId, int u, int v, int w, int h, int x, int y, int z,
						  u16 r, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend
);

// 101129
// レイアウト指定版　layoutIdから、layoutIdShift分x y z座標をずらす
void disp_commonObjSimpleShift( int layoutId, int layoutIdShift);
void disp_commonObjSimpleColorShift( int layoutId, int layoutIdShift, GXRgb color, int alpha);
// n の値を 0～9その後-の連続したオブジェの0の objId のオブジェで 1桁目が x y zの位置から 上位桁まで w の間隔で表示する
// layoutIdShift分のx y z座標をずらす
void disp_commonObjNumberSimple( int n, int layoutIdLowDigit, int layoutIdHighDigit);
void disp_commonObjNumberSimpleColor( int n, int layoutIdLowDigit, int layoutIdHighDigit, GXRgb color, u16 alpha);
void disp_commonObjNumberSimpleZeroColor( int n, int figureCnt, int layoutIdLowDigit, int layoutIdHighDigit, GXRgb color, u16 alpha);
void disp_commonObjNumberSimpleShift( int n, int layoutIdLowDigit, int layoutIdHighDigit, int layoutIdShift);

// fpsを の値を 0～9その後-、その後.の連続したオブジェの0の objId のオブジェで FPSが x y zの位置から 上位桁まで w の間隔で XX.XXFPS の形式で表示する
void disp_commonObjFps( double fps, int objIdNumber, int objIdFps, int x, int y, int z, int w);

void disp_commonObjDxHandleSimple( int handle, int layoutId);
void disp_commonObjDxHandleFull( int handle, int x, int y, int z, int w, int h, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend);
void disp_commonObjDxHandleCenterFull( int handle, int x, int y, int z, int w, int h, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend);

// 111125
void disp_commonObjDxScaleCenterFull( int objId, int x, int y, int z, double hScale, double vScale, u16 r, BOOL flipH, BOOL flipV, GXRgb color, u16 alpha, int blend);

// 111128
void disp_commonObjUvWhFull( int textureId, int u, int v, int uw, int vh, int x, int y, int z,
						  int w, int h, u16 r, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend
);

// 120320
void disp_common3dModelSimple( void * draw3d, int modelId,
							double x, double y, double z, int dispZ
);

// 120526
void disp_commonCallback( void (* func)( void *), void * funcArg, int z);

void disp_commonBoardSimple( int layoutId, GXRgb color);

// 130204
int disp_commonGetObjZBufCnt();
DISP_OBJ * disp_commonGetObjZBuf( int index);
void disp_commonShowMain3dObjEnd();
void disp_commonShowMain3dObj( DISP_OBJ * objZBuf);

#endif //__DISP_COMMON_H__