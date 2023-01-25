#ifndef __DISP_H__
#define __DISP_H__

#include "type.h"
#include "obj.h"
#include "layout.h"

#define DISP_GET_COLOR( r, g, b) GetColor( r, g, b)
#define DISP_WHITE (DISP_GET_COLOR( 255, 255, 255))
#define DISP_BLACK (DISP_GET_COLOR( 0, 0, 0))
#define DISP_BLACK_HLAF (DISP_GET_COLOR( 128, 128, 128))
#define DISP_RED   (DISP_GET_COLOR( 255,   0,   0))
#define DISP_GREEN (DISP_GET_COLOR(   0, 255,   0))
#define DISP_BLUE  (DISP_GET_COLOR(   0,   0, 255))

enum {
	DISP_BLEND_MODE_NONE,	//なし
	DISP_BLEND_MODE_ALPHA,	//αブレンド
	DISP_BLEND_MODE_ADD,	//加算ブレンド
	DISP_BLEND_MODE_SUB,	//減算ブレンド
	DISP_BLEND_MODE_MULA,	//乗算ブレンド
	DISP_BLEND_MODE_INVSRC,	//反転ブレンド

	DISP_BLEND_MODE_MAX
};

enum {
	DISP_ALPHA_DEFAULT = 255
};

enum {
	DISP_DEBUG_CHECK_STATE_NONE,
	DISP_DEBUG_CHECK_STATE_ID,

	DISP_DEBUG_CHECK_STATE_MAX
};

enum {
	DISP_NUMBER_OPTION_COMMA = 0x0001,

	DISP_NUMBER_OPTION_MAX
};


typedef struct _disp_obj DISP_OBJ;
struct _disp_obj {
	u16 category;
	s32 id;
	s16 u;
	s16 v;
	s16 uw;
	s16 vh;
	s16 x;
	s16 y;
	s16 z;
	s16 w;
	s16 h;
	u16 rot;
	u16 alpha;
	BOOL flipH;
	BOOL flipV;
	GXRgb rgbLeftUp;
	GXRgb rgbRightUp;
	GXRgb rgbRightDown;
	GXRgb rgbLeftDown;
	int blend;
	DISP_OBJ * next;
	void * draw3d;
	double x3d;
	double y3d;
	double z3d;
	double time;
	int modelAnimeIndex;
	double scale;
	void (* func)(void *);
	void * funcArg;
	bool externalShowFlag;
};

namespace nn {
namespace dxlib {
namespace disp {

//オブジェ表示　最も一般的なもの
void obj( int objId, int x, int y, int z, bool cameraFlag = true);
//オブジェ表示　Dx版　x yはオブジェの中心座標　blendはenum DISP_COMMON_BLEND_MODE参照　ブレンドパラメータはalpha使用　rgba各値は(0～31)
void objCenterFull( int objId, int x, int y, int z, int w, int h,
	int r, BOOL flipH, BOOL flipV, int rgb,
	int alpha, int blend, bool cameraFlag = true
);
void objCenter( int objId, int x, int y, int z, bool cameraFlag = true);
void objColor( int objId, int x, int y, int z, int rgb, int alpha, bool cameraFlag = true);
void objColorCenter( int objId, int x, int y, int z, int rgb, int alpha, bool cameraFlag = true);

void objFull( int objId, int x, int y, int z, int w, int h,
	int r, BOOL flipH, BOOL flipV, int rgb,
	int alpha, int blend, bool cameraFlag = true
);
void objSimple( int layoutId, bool cameraFlag = true);
void objNumberSimple( int n, int figureCnt,
	int layoutIdLowDigit, int layoutIdHighDigit, bool cameraFlag = true
);
void objNumber( int n, int figureCnt, int objId,
	int x, int y, int z, int w, bool cameraFlag = true
);
void objNumberSimpleShift( int n, int figureCnt,
	int layoutIdBase,
	int layoutIdLowDigit, int layoutIdHighDigit, bool cameraFlag = true
);
void objNumberColor(  int n, int figureCnt, int objId,
	int x, int y, int z, int w,
	int rgb, int alpha, int blend,
	bool cameraFlag = true
);
void objNumberSimpleShiftColor( int n, int figureCnt,
	int layoutIdBase,
	int layoutIdLowDigit, int layoutIdHighDigit,
	int rgb, int alpha, int blend,
	bool cameraFlag = true
);
void objSimpleColor( int layoutId, int rgb, int alpha, int blendMode, bool cameraFlag);

void boardFull( int x, int y, int z, int w, int h, int r,
	int rgb, int alpha, int blend, bool cameraFlag = true
);
void boardCenterFull( int x, int y, int z, int w, int h, int r,
	int rgb, int alpha, int blend, bool cameraFlag = true
);
void boardSimple( int layoutId, int bleandMode, bool cameraFlag);

void setObjData( int index, void * objTop, int idStart, int idEnd);
void setLayoutData( int index, void * layoutTop, int idStart, int idEnd);

void show( void);

void setCameraArea( int w, int h);
void setCameraPos( int x, int y);
void setCameraFlag( bool flag);
int getCameraStartX();
int getCameraStartY();

void objUv( int textureId, int u, int v, int w, int h,
	int x, int y, int z, bool cameraFlag = true
);
void objUvFull( int textureId, int u, int v, int w, int h,
	int x, int y, int z,
	int r, BOOL flipH, BOOL flipV,
	int rgb, int alpha, int blend,
	bool cameraFlag = true
);
void objUvWhFull( int textureId, int u, int v, int uw, int vh,
	int x, int y, int z,
	int w, int h, int r, BOOL flipH, BOOL flipV,
	int rgb, int alpha, int blend,
	bool cameraFlag = true
);

void objSimpleShift( int layoutId, int layoutIdShift, bool cameraFlag = true);
void objShift( int layoutId, int xShift, int yShift, int zShift, bool cameraFlag = true);
void objSimpleShiftChangeObj( int objId, int layoutId, int layoutIdShift, bool cameraFlag = true);
void objSimpleChangeObj( int objId, int layoutId, bool cameraFlag = true);
void objShiftChangeObj( int objId, int layoutId, int xShift, int yShift, int zShift, bool cameraFlag = true);

int getDispObjCnt();
DISP_OBJ * getDispObj( int index);
void showEnd();
void showObj( DISP_OBJ * dispObj);

void model( void * draw3d, int modelId, int modelAnimeIndex,
		   double x, double y, double z, int dispZ,
		   double scale, double time, GXRgb lightDefColor
);

LAYOUT * getLayout( int id);
OBJ * getObj( int id);

void line( int x1, int y1, int x2, int y2, int z, int w,
		  int rgb, int alpha, int blend, bool cameraFlag = true
);

void task();
void setBrightness( int brightness);

void objHandleSimple( int handle, int layoutId, bool cameraFlag = true);
void objHandleSimpleColor( int handle, int layoutId, int rgb, int alpha, int blendMode, bool cameraFlag = true);
void objHandleFull( int handle,
	int x, int y, int z, int w, int h, int r, BOOL flipH, BOOL flipV,
	GXRgb rgb, u16 alpha, int blend, bool cameraFlag = true
);
void objHandleCenterFull( int handle,
	int x, int y, int z, int w, int h, int r, BOOL flipH, BOOL flipV,
	GXRgb rgb, u16 alpha, int blend, bool cameraFlag = true
);

void objCntInc();

void setTrapObjId( int objId);

void setCameraPosSimple( int x, int y, int dispW, int dispH, int mapW, int mapH);

void circleCenter( int x, int y, int z, int r,
	int rgb, int alpha, int blend, bool cameraFlag
);

void objNumberCenter( int n, int figureCnt, int objId,
	int x, int y, int z,
	int blank, double extRate, GXRgb rgb, int alpha, int blend,
	unsigned int option, bool cameraFlag = true
);

} //namesapce disp
} //namesapce dxlib
} //namespace nn

#endif //__DISP_H__
