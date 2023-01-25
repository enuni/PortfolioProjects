#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include "type.h"

typedef struct _layout {
	u8 type;
	s16 x;
	s16 y;
	u16 w;
	u16 h;
	s16 z;			//-255Å`+255
	s16 r;			//0Å`255
	s16 g;			//0Å`255
	s16 b;			//0Å`255
	u8 alpha;		//0Å`255
	u16 scale;		//1Å` ìôî{=100
	u16 rotate;		//0Å`359
	BOOL flip_h;
	BOOL flip_v;
	u16 obj_id;
} LAYOUT;

typedef struct _script_layout {
	int obj_id;
	int x;
	int y;
	int z;
} SCRIPT_LAYOUT;

#define LAYOUT_ID_MAIN_3D_COMMON_OFFSET (1024)

#endif	//__LAYOUT_H__

