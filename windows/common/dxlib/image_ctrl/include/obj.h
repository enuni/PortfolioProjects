#ifndef __OBJ_H__
#define __OBJ_H__

#include "type.h"

typedef struct _texture {
	char image_name[64];
	u16 s;	//ïù
	u16 t;	//èc
	u16 color;
} TEXTURE;

typedef struct _image {
	char image_name[64];
	u32 size;
	u16 color;
} IMAGE;

typedef struct _palette {
	char palette_name[64];
	u16 color;
} PALETTE;

typedef struct _obj {
	u16 u;
	u16 v;
	u16 w;
	u16 h;
	u16 texture_id;
} OBJ;

typedef struct _character {
	u16 u;
	u16 v;
	u8 w;
	u8 h;
	u16 image_id;
} CHARACTER;

typedef struct _bg {
	char bg_name[64];
	u16 color;
} BG;

#define TEXTURE_ID_MAIN_3D_COMMON_OFFSET (32)
#define PALETTE_3D_ID_MAIN_3D_COMMON_OFFSET (32)
#define OBJ_ID_MAIN_3D_COMMON_OFFSET (1024)
#define LAYOUT_ID_MAIN_3D_COMMON_OFFSET (1024)

#define IMAGE_ID_SUB_COMMON_OFFSET (32)
#define PALETTE_2D_ID_SUB_COMMON_OFFSET (32)
#define CHAR_ID_SUB_COMMON_OFFSET (1024)
#define LAYOUT_ID_SUB_COMMON_OFFSET (1024)

#endif	//__OBJ_H__
