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
	DISP_COMMON_BLEND_MODE_NONE,	//�Ȃ�
	DISP_COMMON_BLEND_MODE_ALPHA,	//���u�����h
	DISP_COMMON_BLEND_MODE_ADD,	//���Z�u�����h
	DISP_COMMON_BLEND_MODE_SUB,	//���Z�u�����h
	DISP_COMMON_BLEND_MODE_MULA,	//��Z�u�����h
	DISP_COMMON_BLEND_MODE_INVSRC,	//���]�u�����h
	
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

//�I�u�W�F�\���@�ł���ʓI�Ȃ���
void disp_commonObj( int objId, int x, int y, int z);
//�I�u�W�F�\���@���C�A�E�g�w��Ł@ x y z���W�����炷���Ƃ��\
void disp_commonObjShift( int layoutId, int xShift, int yShift, int zShift);
//�I�u�W�F�\���@���C�A�E�g�w��Ł@�w�肵�����C�A�E�g���̂����\�� �I�u�W�F�̕ύX���\�@rgba�e�l��(0�`31)
void disp_commonObjSimpleColor( int layoutId, GXRgb rgb, int alpha);
//�I�u�W�F�\���@���C�A�E�g�w��Ł@�ł��ȒP�Ȃ���
void disp_commonObjSimple( int layoutId);
//�I�u�W�F�\���@���C�A�E�g�w��Ł@�w�肵�����C�A�E�g���̂����\�� �I�u�W�F�̕ύX���\
void disp_commonObjChangeObjId( int layoutId, int objId);
//�I�u�W�F�\���@���C�A�E�g�w��Ł@�w�肵�����C�A�E�g���̂����\�� �I�u�W�F�̕ύX x y z���W�����炷���Ƃ��\
void disp_commonObjChangeShift( int layoutId, int objId, int xShift, int yShift, int zShift);
//�I�u�W�F�\���@���W�F�A���t�@�w��Ł@rgba�e�l��(0�`31)
void disp_commonObjColor( int objId, int x, int y, int z, GXRgb rgb, u16 alpha);
//�I�u�W�F�\���@���C�A�E�g�w��\���g���Ł@rgba�e�l��(0�`31)
void disp_commonObjLayoutEx( int layoutId, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
//�I�u�W�F�\���@���S���W�Ł@x y�̓I�u�W�F�̒��S���W�@rgba�e�l��(0�`31)
void disp_commonObjCenter( int objId, int x, int y, int w, int h, int z, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
//�I�u�W�F�\���@�g���Ł@rgba�e�l��(0�`31)
void disp_commonObjEx( int objId, int x, int y, int z, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
//�I�u�W�F�\���@�S�@�\�Ł@plygonId��win�̏ꍇ0�ɂ���@���ʃ��C�u�����̎d�l�� �c���̊g�嗦���قȂ�ꍇ��] �t���b�v�͖����ɂȂ�@rgba�e�l��(0�`31)
void disp_commonObjFull( int polygonId, int objId, int x, int y, int z, int w, int h, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
//�I�u�W�F�\���@�S�@�\�Ł@x y�̓I�u�W�F�̒��S���W�@plygonId��win�̏ꍇ0�ɂ���@���ʃ��C�u�����̎d�l�� �c���̊g�嗦���قȂ�ꍇ��] �t���b�v�͖����ɂȂ�@rgba�e�l��(0�`31)
void disp_commonObjCenterFull( int polygonId, int objId, int x, int y, int z, int w, int h, u16 rotate, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha);
//�I�u�W�F�\���@Dx�Ł@blend��enum DISP_COMMON_BLEND_MODE�Q�Ɓ@�u�����h�p�����[�^��alpha�g�p�@rgba�e�l��(0�`31)
void disp_commonObjDxFull( int objId, int x, int y, int z, int w, int h, u16 r, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend);
//�I�u�W�F�\���@Dx�Ł@x y�̓I�u�W�F�̒��S���W�@blend��enum DISP_COMMON_BLEND_MODE�Q�Ɓ@�u�����h�p�����[�^��alpha�g�p�@rgba�e�l��(0�`31)
void disp_commonObjDxCenterFull( int objId, int x, int y, int z, int w, int h, u16 r, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend);
//�I�u�W�F�\�� ���S���W�� �̍ł���ʓI�ȕ�
void disp_commonObjCenterBasic( int objId, int x, int y, int z);
//�I�u�W�F�\�� ���S���W�� �̍��W�F�A���t�@�w��Ł@rgba�e�l��(0�`31)
void disp_commonObjCenterColor( int objId, int x, int y, int z, GXRgb rgb, u16 alpha);
//���̃t���[���̕\���̍��W�I�t�Z�b�g��ݒ肷��
void disp_commonSetOffset( int hOffset, int vOffset);

//�J������L���ɂ���ƕ\���̌��_���u�J�������S(x, y) - �J�����\���̈��-1/2(w / 2, h / 2)�v�ɂȂ�
//�I�t�Z�b�g disp_commonSetOffset() �ƕ��p��
//�J������L��������ݒ�
void disp_commonSetCameraFlag( BOOL flag);
//�J�����\���̈��ݒ�
void disp_commonSetCameraSize( int w, int h);
//�J�����̒��S���W��ݒ�
void disp_commonSetCameraPos( int x, int y);

//�I�u�W�FID����I�u�W�F���𓾂�
OBJ * disp_commonGetObj( int id);
//���C�A�E�gID���烌�C�A�E�g���𓾂�
LAYOUT * disp_commonGetLayoutMain3d( int id);

//�I�u�W�F���̐ݒ�
void disp_commonSetObjData( int index, void * objTop, int idStart, int idEnd);
//���C�A�E�g���̐ݒ�
void disp_commonSetLayoutMain3dData( int index, void * layoutTop, int idStart, int idEnd);

///////////////////////////////////////////////////////////////////////////////
//disp_commonObj�`�Ɠ������� �݊����̂��߂ɑ���
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
//�ȉ�������
//���C����ʂ� n �̒l�� 0�`9���̌�-�̘A�������I�u�W�F��0�� objId �̃I�u�W�F�� 1���ڂ� x y z�̈ʒu���� ��ʌ��܂� w �̊Ԋu�ŕ\������
void disp_commonMain3dObjNumber( int n , int objId, int x, int y, int z, int w);
void disp_commonMain3dObjNumberPlumi( int n, int objId, int x, int y, int z, int w);
void disp_commonMain3dObjNumberZero( int n, int figureCnt, int objId, int x, int y, int z, int w);
void disp_commonMain3dObjNumberColor( int n, int objId, int x, int y, int z, int w, GXRgb color, u16 alpha);
void disp_commonMain3dObjNumberZeroColor( int n, int figureCnt, int objId, int x, int y, int z, int w, GXRgb color, u16 alpha);
void disp_commonMain3dObjNumberCenter( int n, int objId, int xCenter, int y, int z, int w);

void disp_commonMain3dBoardSimple( int layoutId, GXRgb rgb);
//�R�̎���
void disp_commonMain3dBoardSimpleColor4( int layoutId, GXRgb * rgb);
void disp_commonMain3dBoard( int x, int y, int z, int w, int h, GXRgb rgb, u16 alpha);
//�R�̎���
void disp_commonMain3dBoardColor4( int x, int y, int z, int w, int h, GXRgb * rgb, u16 alpha);
//�R�̎���
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

// n�� 1�ȏ� objId ��`�� �̘A�� objId �\�S�疜 �̘A��
void disp_commonSub2dObjNumberKanji( int n, int objIdNum, int objIdUnit, int x, int y, int z, int w);

//�T�u��ʂ� n �̒l�� 0�`9���̌�-�̘A�������I�u�W�F��0�� objId �̃I�u�W�F�� 1���ڂ� x y z�̈ʒu���� ��ʌ��܂� w �̊Ԋu�ŕ\������
void disp_commonSub2dObjNumber( int n, int objId, int x, int y, int z, int w);

//disp_commonSub2dObjNumber()�� figureCnt �̌����� ����Ȃ����ɂ� 0������ �\������
void disp_commonSub2dObjNumberZero( int n, int figureCnt, int objId, int x, int y, int z, int w);

void disp_commonSub2dObj( int charId, int x, int y, int z);
void disp_commonSub2dObjShift( int layoutId, int xShift, int yShift, int zShift);
void disp_commonSub2dObjSimple( int layoutId);
void disp_commonSub2dObjEx( int charId, int x, int y, int z, BOOL flipH, BOOL flipV);
void disp_commonShowSub2dObj( void);

void disp_commonSub2dBg( int bgId);
void disp_commonShowSub2dBg( void);

void disp_commonPanic( const char * format, ...);

// 101014 DXlib�}�X�N�@�\�ǉ�
void disp_commonStartMask();
void disp_commonEndMask();
void disp_commonMain3dMask( int texture_id, int x, int y, int z);
void disp_commonMain3dMaskRelease( int z);

// 101029
// �C�ӂ̃e�L�X�`���[�����؂�o���`��
void disp_commonObjUv( int textureId, int u, int v, int w, int h, int x, int y, int z);

// 101223
void disp_commonObjUvFull( int textureId, int u, int v, int w, int h, int x, int y, int z,
						  u16 r, BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend
);

// 101129
// ���C�A�E�g�w��Ł@layoutId����AlayoutIdShift��x y z���W�����炷
void disp_commonObjSimpleShift( int layoutId, int layoutIdShift);
void disp_commonObjSimpleColorShift( int layoutId, int layoutIdShift, GXRgb color, int alpha);
// n �̒l�� 0�`9���̌�-�̘A�������I�u�W�F��0�� objId �̃I�u�W�F�� 1���ڂ� x y z�̈ʒu���� ��ʌ��܂� w �̊Ԋu�ŕ\������
// layoutIdShift����x y z���W�����炷
void disp_commonObjNumberSimple( int n, int layoutIdLowDigit, int layoutIdHighDigit);
void disp_commonObjNumberSimpleColor( int n, int layoutIdLowDigit, int layoutIdHighDigit, GXRgb color, u16 alpha);
void disp_commonObjNumberSimpleZeroColor( int n, int figureCnt, int layoutIdLowDigit, int layoutIdHighDigit, GXRgb color, u16 alpha);
void disp_commonObjNumberSimpleShift( int n, int layoutIdLowDigit, int layoutIdHighDigit, int layoutIdShift);

// fps�� �̒l�� 0�`9���̌�-�A���̌�.�̘A�������I�u�W�F��0�� objId �̃I�u�W�F�� FPS�� x y z�̈ʒu���� ��ʌ��܂� w �̊Ԋu�� XX.XXFPS �̌`���ŕ\������
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