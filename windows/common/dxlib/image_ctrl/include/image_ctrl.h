#ifndef __IMAGE_CTRL_H__
#define __IMAGE_CTRL_H__

#include "type.h"

typedef struct _texture TEXTURE;

enum {
	IMAGE_CTRL_ASYNC_STATE_NONE,
	IMAGE_CTRL_ASYNC_STATE_LOAD,
	IMAGE_CTRL_ASYNC_STATE_ERROR,

	IMAGE_CTRL_ASYNC_STATE_MAX
};

enum {
	IMAGE_CTRL_STATE_NONE,
	IMAGE_CTRL_STATE_HEAP_LOAD,		//�q�[�v�i�������j�Ƀ��[�h��
	IMAGE_CTRL_STATE_HEAP_COMP,		//�q�[�v�i�������j�Ƀ��[�h����
	IMAGE_CTRL_STATE_VRAM_LOAD,		//VRAM�Ƀ��[�h��
	IMAGE_CTRL_STATE_ACTIVE,		//VRAM�Ƀ��[�h���� �\���\
	
	IMAGE_CTRL_STATE_MAX
};

enum {
	IMAGE_CTRL_ASYNC_CTRL_STATE_NONE,		//���ݔ񓯊����[�h�����Ă��Ȃ�
	IMAGE_CTRL_ASYNC_CTRL_STATE_IMAGE_HEAP,		//image���q�[�v�i�������j�Ƀ��[�h��
	IMAGE_CTRL_ASYNC_CTRL_STATE_IMAGE_HEAP_COMP,		//image���q�[�v�i�������j�Ƀ��[�h����
	IMAGE_CTRL_ASYNC_CTRL_STATE_IMAGE_VRAM,		//image��VRAM�Ƀ��[�h��
	IMAGE_CTRL_ASYNC_CTRL_STATE_IMAGE_VRAM_COMP,		//image��VARM�Ƀ��[�h��
	IMAGE_CTRL_ASYNC_CTRL_STATE_PALETTE_HEAP,		//palette���q�[�v�i�������j�Ƀ��[�h��
	IMAGE_CTRL_ASYNC_CTRL_STATE_PALETTE_HEAP_COMP,		//palette���q�[�v�i�������j�Ƀ��[�h����
	IMAGE_CTRL_ASYNC_CTRL_STATE_PALETTE_VRAM,		//palette��VRAM�Ƀ��[�h��
	IMAGE_CTRL_ASYNC_CTRL_STATE_COMP,		//palette��VARM�Ƀ��[�h����
	
	IMAGE_CTRL_SYNC_CTRL_STATE_MAX
};

enum {
	IMAGE_CTRL_ASYNC_CTRL_TYPE_NONE,
	IMAGE_CTRL_ASYNC_CTRL_TYPE_MAIN_3D,		//���C�����3d�p��image palette��ǂݍ��ݒ�
	IMAGE_CTRL_ASYNC_CTRL_TYPE_SUB_2D_BG_BG,		//�T�u���2d�w�i�p��image palette��ǂݍ��ݒ�
	IMAGE_CTRL_ASYNC_CTRL_TYPE_SUB_2D_OBJ,		//�T�u���2dobj�p��image palette��ǂݍ��ݒ�
	
	IMAGE_CTRL_ASYNC_CTRL_TYPE_MAX
};

//---------------------------------------------------------------------------

void image_ctrlInit();

void image_ctrlSetBgSubData( void * bgTop, void * paletteTop);
void image_ctrlSetTextureData( int index, void * textureTop, void * paletteTop, int idStart, int idEnd);
void image_ctrlSetImageData( int index, void * imageTop, void * paletteTop, int idStart, int idEnd);

//���t���[���ĂѕK�v�ȏ���������
void image_ctrlTask( void);
//image_ctrlTask()����Ăяo�����
void image_ctrlTaskAsyncCtrl( void);

//image�̔񓯊����[�h������Ԃ�
BOOL image_ctrlIsLoadAsyncActive( void);

//image�̔񓯊����[�h�̏�Ԃ�Ԃ� IMAGE_CTRL_ASYNC_STATE_�`
int image_ctrlGetLoadAsyncState( void);

int image_ctrlGetSub2dImageHandle( int imageId);
// imageId �̃T�u���2dobj�p��image�ƃf�t�H���g��palette��VRAM��ɂ��� �\���\����Ԃ�
BOOL image_ctrlIsSub2dObjImagePaletteActive( int imageId);
int image_ctrlGetSub2dObjImageColor( int imageId);
GXOamShape image_ctrlGetSub2dObjShape( int w, int h);
int image_ctrlGetSub2dObjPaletteNo( int imageId);
u32 image_ctrlGetSub2dObjImageVramOffset( int imageId);

int image_ctrlGetSub2dBgBgHandle( int bgId);
// bgId �̃T�u��ʔw�i�p��image�ƃf�t�H���g��palette��VRAM��ɂ��� �\���\����Ԃ�
BOOL image_ctrlIsSub2dBgBgImagePaletteActive( int bgId);
u8 image_ctrlGetSub2dBgBgPaletteNo( int paletteId);

int image_ctrlIsMain3dTextureHandle( int textureId);
// textureId �̃��C�����3d�p��image�ƃf�t�H���g��palette��VRAM��ɂ��� �\���\����Ԃ�
BOOL image_ctrlIsMain3dImagePaletteActive( int textureId);
int image_ctrlGetMain3dImageColor( int textureId);
GXTexSizeS image_ctrlGetMain3dImageS( int textureId);
GXTexSizeT image_ctrlGetMain3dImageT( int textureId);
u32 image_ctrlGetMain3dPaletteVramOffset( int paletteId);
u32 image_ctrlGetMain3dImageVramOffset( int textureId);

//---------------------------------------------------------------------------

//�c�[���ŏo�͂��� textureId �̃��C�����3d�p��image���q�[�v�i�������j�Ƀ��[�h���J�n���� �񓯊��m���u���b�N �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadMain3dImageHeapAsync( int textureId);
//�c�[���ŏo�͂��� textureId �̃��C�����3d�p��image��VRAM�Ƀ��[�h���J�n���� �񓯊��m���u���b�N
//���炩���߃q�[�v�i�������j�Ƀ��[�h���Ă����K�v������ �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadMain3dImageVramAsync( int textureId);
//�c�[���ŏo�͂��� paletteId �̃��C�����3d�p��palette���q�[�v�i�������j�Ƀ��[�h���J�n���� �񓯊��m���u���b�N �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadMain3dPaletteHeapAsync( int paletteId);
//�c�[���ŏo�͂��� paletteId �̃��C�����3d�p��palette��VRAM�Ƀ��[�h���J�n���� �񓯊��m���u���b�N
//���炩���߃q�[�v�i�������j�Ƀ��[�h���Ă����K�v������ �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadMain3dPaletteVramAsync( int paletteId);

//�c�[���ŏo�͂��� textureId �̃��C�����3d�p��image�ƃf�t�H���g��palette���q�[�v�i�������j��VRAM�ւ̈ꊇ���[�h���J�n����
//�񓯊��m���u���b�N �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadMain3dImagePaletteHeapVramAsync( int textureId);

//�c�[���ŏo�͂��� textureId �̃��C�����3d�p��image���q�[�v�i�������j�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadMain3dImageHeap( int textureId);
//�c�[���ŏo�͂��� paletteId �̃��C�����3d�p��palette���q�[�v�i�������j�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadMain3dPaletteHeap( int paletteId);
//�c�[���ŏo�͂��� textureId �̃��C�����3d�p��image��VRAM�Ƀ��[�h����
//���炩���߃q�[�v�i�������j�Ƀ��[�h���Ă����K�v������ ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadMain3dImageVram( int textureId);
//�c�[���ŏo�͂��� paletteId �̃��C�����3d�p��palette��VRAM�Ƀ��[�h����
//���炩���߃q�[�v�i�������j�Ƀ��[�h���Ă����K�v������ ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadMain3dPaletteVram( int paletteId);

//�c�[���ŏo�͂��� textureId �̃��C�����3d�p��image�ƃf�t�H���g��palette���q�[�v�i�������j��VRAM�ւ̈ꊇ���[�h���� ���������� TRUE ��Ԃ�  �u���b�N
BOOL image_ctrlLoadMain3dImagePaletteHeapVram( int textureId);
//�c�[���Ȃ��p
int image_ctrlLoadMain3dImagePaletteHeapVram( int index, char * textureFileName);

//image_ctrlLoadMain3d�`()�Ń��[�h���� textureId �̃��C�����3d�p��image���q�[�v�i�������j����J������
BOOL image_ctrlReleaseMain3dImageHeap( int textureId);
//image_ctrlLoadMain3d�`()�Ń��[�h���� paletteId �̃��C�����3d�p��palette���q�[�v�i�������j����J������
BOOL image_ctrlReleaseMain3dPaletteHeap( int paletteId);
//image_ctrlLoadMain3d�`()�Ń��[�h���� textureId �̃��C�����3d�p��image��VRAM����J������
BOOL image_ctrlReleaseMain3dImageVram( int textureId);
//image_ctrlLoadMain3d�`()�Ń��[�h���� paletteId �̃��C�����3d�p��palette��VRAM����J������
BOOL image_ctrlReleaseMain3dPaletteVram( int paletteId);

//image_ctrlLoadMain3d�`()�Ń��[�h���� textureId �̃��C�����3d�p��image�ƃf�t�H���g��palette���q�[�v�i�������j��VRAM����ꊇ�J������
BOOL image_ctrlReleaseMain3dImagePaletteHeapVram( int textureId);

//---------------------------------------------------------------------------

//�c�[���ŏo�͂��� bgId �̃T�u��ʔw�i�p��image���q�[�v�i�������j�Ƀ��[�h���J�n���� �񓯊��m���u���b�N �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadSub2dBgBgImageHeapAsync( int bgId);
//�c�[���ŏo�͂��� bgId �̃T�u��ʔw�i�p��Image��VRAM�Ƀ��[�h���J�n���� �񓯊��m���u���b�N
//���炩���߃q�[�v�i�������j�Ƀ��[�h���Ă����K�v������ �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadSub2dBgBgImageVramAsync( int bgId);
//�c�[���ŏo�͂��� paletteId �̃T�u��ʔw�i�p��palette���q�[�v�i�������j�Ƀ��[�h���J�n���� �񓯊��m���u���b�N �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadSub2dBgBgPaletteHeapAsync( int paletteId);
//�c�[���ŏo�͂��� paletteId �̃T�u��ʔw�i�p��palette��VRAM�Ƀ��[�h���J�n���� �񓯊��m���u���b�N
//���炩���߃q�[�v�i�������j�Ƀ��[�h���Ă����K�v������ �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadSub2dBgBgPaletteVramAsync( int paletteId);

//�c�[���ŏo�͂��� bgId �̃T�u��ʔw�i�p��image�ƃf�t�H���g��palette���q�[�v�i�������j��VRAM�ւ̈ꊇ���[�h���J�n����
//�񓯊��m���u���b�N �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadSub2dBgBgImagePaletteHeapVramAsync( int bgId);

//�c�[���ŏo�͂��� bgId �̃T�u��ʔw�i�p��image���q�[�v�i�������j�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadSub2dBgBgImageHeap( int bgId);
//�c�[���ŏo�͂��� paletteId �̃T�u��ʔw�i�p��palette���q�[�v�i�������j�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadSub2dBgBgPaletteHeap( int paletteId);
//�c�[���ŏo�͂��� bgId �̃T�u��ʔw�i�p��image��VRAM�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadSub2dBgBgImageVram( int bgId);
//�c�[���ŏo�͂��� paletteId �̃T�u��ʔw�i�p��palette��VRAM�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadSub2dBgBgPaletteVram( int paletteId);

//�c�[���ŏo�͂��� bgId �̃T�u��ʔw�i�p��image�ƃf�t�H���g��palette���q�[�v�i�������j��VRAM�ւ̈ꊇ�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadSub2dBgBgImagePaletteHeapVram( int bgId);

//image_ctrlLoadSub2dBgBg�`()�Ń��[�h���� bgId �̃T�u��ʔw�i�p��image���q�[�v�i�������j����J������
BOOL image_ctrlReleaseSub2dBgBgImageHeap( int bgId);
//image_ctrlLoadSub2dBgBg�`()�Ń��[�h���� paletteId �̃T�u��ʔw�i�p��palette���q�[�v�i�������j����J������
BOOL image_ctrlReleaseSub2dBgBgPaletteHeap( int paletteId);
//image_ctrlLoadSub2dBgBg�`()�Ń��[�h���� bgId �̃T�u��ʔw�i�p��image��VRAM����J������
BOOL image_ctrlReleaseSub2dBgBgImageVram( int bgId);
//image_ctrlLoadSub2dBgBg�`()�Ń��[�h���� paletteId �̃T�u��ʔw�i�p��palette����VRAM����J������
BOOL image_ctrlReleaseSub2dBgBgPaletteVram( int paletteId);

//image_ctrlLoadSub2dBgBg�`()�Ń��[�h���� bgId �̃T�u��ʔw�i�p��image�ƃf�t�H���g��palette���q�[�v�i�������j��VRAM����ꊇ�J������
BOOL image_ctrlReleaseSub2dBgBgImagePaletteHeapVram( int bgId);

//---------------------------------------------------------------------------

//�c�[���ŏo�͂��� imageId �̃T�u���obj�p��image���q�[�v�i�������j�Ƀ��[�h���J�n���� �񓯊��m���u���b�N �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadSub2dObjImageHeapAsync( int imageId);
//�c�[���ŏo�͂��� imageId �̃T�u���obj�p��Image��VRAM�Ƀ��[�h���J�n���� �񓯊��m���u���b�N
//���炩���߃q�[�v�i�������j�Ƀ��[�h���Ă����K�v������ �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadSub2dObjImageVramAsync( int imageId);
//�c�[���ŏo�͂��� paletteId �̃T�u���obj�p��palette���q�[�v�i�������j�Ƀ��[�h���J�n���� �񓯊��m���u���b�N �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadSub2dObjPaletteHeapAsync( int paletteId);
//�c�[���ŏo�͂��� paletteId �̃T�u���obj�p��palette��VRAM�Ƀ��[�h���J�n���� �񓯊��m���u���b�N
//���炩���߃q�[�v�i�������j�Ƀ��[�h���Ă����K�v������ �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadSub2dObjPaletteVramAsync( int paletteId);

//�c�[���ŏo�͂��� imageId �̃T�u���obj�p��image�ƃf�t�H���g��palette���q�[�v�i�������j��VRAM�ւ̈ꊇ���[�h���J�n����
//�񓯊��m���u���b�N �J�n�ɐ��������� TRUE ��Ԃ�
BOOL image_ctrlLoadSub2dObjImagePaletteHeapVramAsync( int imageId);

//�c�[���ŏo�͂��� imageId �̃T�u���obj�p��image���q�[�v�i�������j�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadSub2dObjImageHeap( int imageId);
//�c�[���ŏo�͂��� paletteId �̃T�u���obj�p��palette���q�[�v�i�������j�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadSub2dObjPaletteHeap( int paletteId);
//�c�[���ŏo�͂��� imageId �̃T�u���obj�p��image��VRAM�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadSub2dObjImageVram( int imageId);
//�c�[���ŏo�͂��� paletteId �̃T�u���obj�p��palette��VRAM�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadSub2dObjPaletteVram( int paletteId);

//�c�[���ŏo�͂��� imageId �̃T�u���obj�p��image�ƃf�t�H���g��palette���q�[�v�i�������j��VRAM�ւ̈ꊇ�Ƀ��[�h���� ���������� TRUE ��Ԃ� �u���b�N
BOOL image_ctrlLoadSub2dObjImagePaletteHeapVram( int imageId);

//image_ctrlLoadSub2dObj�`()�Ń��[�h���� imageId �̃T�u���obj�p��image���q�[�v�i�������j����J������
BOOL image_ctrlReleaseSub2dObjImageHeap( int imageId);
//image_ctrlLoadSub2dObj�`()�Ń��[�h���� paletteId �̃T�u���obj�p��palette���q�[�v�i�������j����J������
BOOL image_ctrlReleaseSub2dObjPaletteHeap( int paletteId);
//image_ctrlLoadSub2dObj�`()�Ń��[�h���� imageId �̃T�u���obj�p��image��VRAM����J������
BOOL image_ctrlReleaseSub2dObjImageVram( int imageId);
//image_ctrlLoadSub2dObj�`()�Ń��[�h���� paletteId �̃T�u���obj�p��palette����VRAM����J������
BOOL image_ctrlReleaseSub2dObjPaletteVram( int paletteId);

//image_ctrlLoadSub2dObj�`()�Ń��[�h���� imageId �̃T�u���obj�p��image�ƃf�t�H���g��palette���q�[�v�i�������j��VRAM����ꊇ�J������
BOOL image_ctrlReleaseSub2dObjImagePaletteHeapVram( int imageId);

// 101014 DXlib�}�X�N�@�\�ǉ�
BOOL image_ctrlLoadMain3dMask( int textureId);
BOOL image_ctrlLoadMain3dMaskAsync( int textureId);
BOOL image_ctrlReleaseMain3dMask( int textureId);

TEXTURE * image_ctrlGetTexture( int textureId);

#endif	//__IMAGE_CTRL_H__

