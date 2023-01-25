#ifndef __FONT_DATA_H__
#define __FONT_DATA_H__

typedef struct _font_common_data {
	char * fontName;
	int fontSize;
	int imageFileN;
	int imageW;
	int imageH;
	int hankakuFontW;
	int zenkakuFontW;
	int fontH;
} FONT_COMMON_DATA;

typedef struct _font_data {
	unsigned char fontNo;
	unsigned short x;
	unsigned short y;
	unsigned char w;
	unsigned char h;
} FONT_DATA;

#endif //__FONT_DATA_H__
