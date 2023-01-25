
#ifndef __DRAW_H__
#define __DRAW_H__

#include "type.h"

void drawInit( void);
void drawTask( void);
void drawMainInit( void);
void drawMain3dInit( void);
void drawMain2dInit( void);
void drawSub2dInit( void);

void drawStartDisplay( void);

void drawMain2dAllDCFlushRange( void);
void drawSub2dAllDCFlushRange( void);
void drawMainSub2dAllDCFlushRange( void);

void drawMain2dAllTransReset( void);
void drawSub2dAllTransReset( void);
void drawMainSub2dAllTransReset( void);

void drawMain3dSetVRAMImage( void * src, u32 dstAdress, u32 size);
void drawMain3dSetVRAMPalette( void * src, u32 dstAdress, u32 size);
void drawMain3dSetTextureTexture256( GXTexSizeS s, GXTexSizeT t, u32 textureOffset, u32 paletteOffset, GXTexPlttColor0 color0Mode);
void drawMain3dSetTexturePalette256( u32 paletteOffset);
void drawMain3dSetTextureTexture16( GXTexSizeS s, GXTexSizeT t, u32 textureOffset, u32 paletteOffset, GXTexPlttColor0 color0Mode);
void drawMain3dSetTexturePalette16( u32 paletteOffset);

void drawMain3dSpriteXYCornerSimple( int x, int y, int u, int v, int s, int t);
void drawMain3dSpriteXYCornerFull( int id, int x, int y, int z, int w, int h, int u, int v, int s, int t, u16 rot, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
void drawMain3dSpriteXYCenterSimple( int x, int y, int u, int v, int s, int t);
void drawMain3dSpriteXYCenterFull( int id, int x, int y, int z, int w, int h, int u, int v, int s, int t, u16 rot, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);

void drawMain3dSpriteBoardXYCornerSimple( int x, int y, int z, int w, int h, u16 alpha, GXRgb rgb);
void drawMain3dSpriteBoardXYCornerFull( int id, int x, int y, int z, int w, int h, u16 rot, u16 alpha, GXRgb rgbLeftUp, GXRgb rgbRightUp, GXRgb rgbRightDown, GXRgb rgbLeftDown);
void drawMain3dSpriteBoardXYCenterSimple( int x, int y, int z, int w, int h, u16 alpha, GXRgb rgb);
void drawMain3dSpriteBoardXYCenterFull( int id, int x, int y, int z, int w, int h, u16 rot, u16 alpha, GXRgb rgbLeftUp, GXRgb rgbRightUp, GXRgb rgbRightDown, GXRgb rgbLeftDown);

void drawMain3dReset( void);

void drawMain2dObj16( int x, int y, GXOamShape shape, int charSrcOffset, int priority, int paletteNo, GXOamMode mode, BOOL mosaic, GXOamEffect effect);
void drawMain2dObjCountReset( void);
void drawMain2dOamDCFlushRange( void);
void drawMain2dOamTrans( void);
void drawMain2dSetVRAMObjImage( void * src, u32 dstAdress, u32 size);
void drawMain2dSetVRAMObjPalette( void * src, u32 dstAdress, u32 size);

void drawMain2dSetVRAMBg1Image( void * src, u32 dstAdress, u32 size);
void drawMain2dSetVRAMBg2Image( void * src, u32 dstAdress, u32 size);
void drawMain2dSetVRAMBgPalette( void * src, u32 dstAdress, u32 size);
void drawMain2dBg1( int x, int y, int src, int paletteNo, BOOL flipH, BOOL flipV);
void drawMain2dBg2( int x, int y, int src, int paletteNo, BOOL flipH, BOOL flipV);
void drawMain2dBg1DCFlushRange( void);
void drawMain2dBg2DCFlushRange( void);
void drawMain2dBg1Trans( void);
void drawMain2dBg2Trans( void);
void drawMain2dBg1Reset( void);
void drawMain2dBg2Reset( void);
void drawMain2dBg1Offset( int x, int y);
void drawMain2dBg2Offset( int x, int y);

void drawSub2dObj256( int x, int y, GXOamShape shape, int charSrcOffset, int priority, int paletteNo, GXOamMode mode, BOOL mosainc, GXOamEffect effect);
void drawSub2dObj16( int x, int y, GXOamShape shape, int charSrcOffset, int priority, int paletteNo, GXOamMode mode, BOOL mosaic, GXOamEffect effect);
void drawSub2dObjCountReset( void);
void drawSub2dOamDCFlushRange( void);
void drawSub2dOamTrans( void);
void drawSub2dSetVRAMObjImage( void * src, u32 dstAdress, u32 size);
void drawSub2dSetVRAMObjExtPalette( void * src, u32 dstAdress, u32 size);
void drawSub2dSetVRAMObjPalette( void * src, u32 dstAdress, u32 size);

void drawSub2dSetVRAMBg0Image( void * src, u32 dstAdress, u32 size);
void drawSub2dSetVRAMBg1Image( void * src, u32 dstAdress, u32 size);
void drawSub2dSetVRAMBg2Image( void * src, u32 dstAdress, u32 size);
void drawSub2dSetVRAMBg3Image( void * src, u32 dstAdress, u32 size);

void drawSub2dSetVRAMBgExtPalette( void * src, u32 dstAdress, u32 size);
void drawSub2dSetVRAMBg0ExtPalette( void * src, u32 dstAdress, u32 size);
void drawSub2dSetVRAMBg1ExtPalette( void * src, u32 dstAdress, u32 size);
void drawSub2dSetVRAMBg2ExtPalette( void * src, u32 dstAdress, u32 size);
void drawSub2dSetVRAMBg3ExtPalette( void * src, u32 dstAdress, u32 size);
void drawSub2dSetVRAMBgPalette( void * src, u32 dstAdress, u32 size);

void drawSub2dBg0( int x, int y, int src, int paletteNo, BOOL flipH, BOOL flipV);
void drawSub2dBg1( int x, int y, int src, int paletteNo, BOOL flipH, BOOL flipV);
void drawSub2dBg2( int x, int y, int src, int paletteNo, BOOL flipH, BOOL flipV);
void drawSub2dBg3( int x, int y, int src);

void drawSub2dBg0DCFlushRange( void);
void drawSub2dBg1DCFlushRange( void);
void drawSub2dBg2DCFlushRange( void);
void drawSub2dBg3DCFlushRange( void);

void drawSub2dBg0Trans( void);
void drawSub2dBg1Trans( void);
void drawSub2dBg2Trans( void);
void drawSub2dBg3Trans( void);

void drawSub2dBg0Reset( void);
void drawSub2dBg1Reset( void);
void drawSub2dBg2Reset( void);
void drawSub2dBg3Reset( void);

void drawSub2dBg0Offset( int offsetH, int offsetV);
void drawSub2dBg1Offset( int offsetH, int offsetV);
void drawSub2dBg2Offset( int offsetH, int offsetV);
/*アフィンBGのオフセットはG2S_SetBG3Affine()で設定
void drawSub2dBg3Offset( int offsetH, int offsetV);
*/
void drawMainAllBrightness( int brightness);
void drawMain3dObjBrightness( int brightness);

void drawSub2dAllBrightness( int brightness);
void drawSub2dBrightness( int plane, int brightness);
void drawSub2dObjBrightness( int brightness);
void drawSub2dBg0Brightness( int brightness);
void drawSub2dBg1Brightness( int brightness);
void drawSub2dBg2Brightness( int brightness);
void drawSub2dBg3Brightness( int brightness);

void drawSub2dBg3Affine( int rotate, int percent, int offsetH, int offsetV);

GXOamShape drawGetGXOamShape( int w, int h);
void drawSetDispMainSub( void);
void drawSetDispSubMain( void);
BOOL drawGetDispMainSubFlag( void);
void drawSwapBuffers( void);
void drawWaitVBlankIntr( void);

#endif	//__DRAW_H__

