#ifndef __ANIMATION_CLASS_H__
#define __ANIMATION_CLASS_H__

typedef struct _key_frame_data {
	short startFrame;
	short enableFrame;
	short x;
	short y;
	short ctrlX;
	short ctrlY;
	short wr;
	short hr;
	short r;
	short g;
	short b;
	short a;
	short blendMode;
	short tweenFlag;
	short acc;
	short angle;
} KEY_FRAME_DATA;

typedef struct _layer_data {
	short objId;
	short layerType;
	short keyFrameN;
} LAYER_DATA;

typedef struct _animation_data {
	short dataOffset;
	short frame;
	short layerN;
} ANIMATION_DATA;

typedef struct _animation_arc_header {
	int dataOffset;
} ANIMATION_ARC_HEADER;

typedef struct _animation_arc_info {
	char * arcFileName;
	short animationOffset;
	short animationN;
} ANIMATION_ARC_INFO;


#endif //__ANIMATION_CLASS_H__
