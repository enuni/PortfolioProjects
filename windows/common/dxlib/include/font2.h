
#ifndef __FONT2_H__
#define __FONT2_H__

#include "type.h"
#include "font_data.h"

//---------------------------------------------------------------------------

namespace nn {
namespace dxlib {

namespace e {
namespace font2 {

enum {
	TEXTURE_IMAGE_N = 256
};

enum FONT_TYPE {
	FONT_TYPE_NONE,
	FONT_TYPE_HANKAKU,
	FONT_TYPE_ZENKAKU,
	FONT_TYPE_KANJI1,
	FONT_TYPE_KANJI2_1,
	FONT_TYPE_KANJI2_2,

	FONT_TYPE_MAX
};

} // namespace font2
} // namespace e

class CFont {
public:
	CFont();
	~CFont();
	void constructorCommon();
	void init( FONT_COMMON_DATA * fontCommonData,
		char ** imageFileNames,
		FONT_DATA * hankaku,
		FONT_DATA * zenkaku,
		FONT_DATA * kanji1,
		FONT_DATA * kanji2_1,
		FONT_DATA * kanji2_2
	);
	int putc( unsigned short c, int x, int y, int z, bool cameraFlag);
	int putcColor( unsigned short c, int x, int y, int z,
		int rgb, int alpha, int blend, bool cameraFlag
	);
	int putsColor(const char * s, int x, int y, int z,
		int rgb, int alpha, int blend, bool cameraFlag
	);
	
	FONT_DATA * getFontData( unsigned short c);
	static unsigned short char2short( char * s);
	static void getFontTypeAndIndex( nn::dxlib::e::font2::FONT_TYPE * type,
		unsigned short * index, unsigned short c
	);
	int getStringDispW( const char * s);
	static u16 getDispCharacterUpdateS( const char ** s);
	
	FONT_COMMON_DATA * fontCommonData;
	char ** imageFileNames;
	FONT_DATA * hankaku;
	FONT_DATA * zenkaku;
	FONT_DATA * kanji1;
	FONT_DATA * kanji2_1;
	FONT_DATA * kanji2_2;
	
	int textureId[nn::dxlib::e::font2::TEXTURE_IMAGE_N];

	char imageFileDirectory[256];
	
};

//---------------------------------------------------------------------------
} // namespace dxlib
} // nn
//---------------------------------------------------------------------------
#endif //__FONT2_H__

