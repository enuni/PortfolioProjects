
#ifndef __TYPE_H__
#define __TYPE_H__

#include "DxLib.h"

#pragma warning(disable:4200)	// �\���̓��̃T�C�Y0�̔z��

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef long s32;
typedef unsigned long u32;
typedef long long s64;
typedef unsigned long long u64;
typedef int BOOL;

//-----------------------------------------------------------------------------
typedef int GXOamShape;
typedef int GXTexSizeS;
typedef int GXTexSizeT;

typedef enum
{
    GX_TEXPLTTCOLOR0_USE = 0,
    GX_TEXPLTTCOLOR0_TRNS = 1
}
GXTexPlttColor0;

// x1B5G5R5 format
typedef int GXRgb;
#define GX_RGB(r, g, b)           GetColor( r * 8, g * 8, b * 8)

// A1B5G5R5 format
typedef u16 GXRgba;
#define GX_RGBA_R_SHIFT           (0)
#define GX_RGBA_R_MASK            (0x001f)
#define GX_RGBA_G_SHIFT           (5)
#define GX_RGBA_G_MASK            (0x03e0)
#define GX_RGBA_B_SHIFT           (10)
#define GX_RGBA_B_MASK            (0x7c00)
#define GX_RGBA_A_SHIFT           (15)
#define GX_RGBA_A_MASK            (0x8000)
#define GX_RGBA(r, g, b, a)       ((GXRgba)(((r) << GX_RGBA_R_SHIFT) | \
                                            ((g) << GX_RGBA_G_SHIFT) | \
                                            ((b) << GX_RGBA_B_SHIFT) | \
                                            ((a) << GX_RGBA_A_SHIFT)))

typedef enum
{
    GX_OAM_MODE_NORMAL = 0,
    GX_OAM_MODE_XLU = 1,
    GX_OAM_MODE_OBJWND = 2,
    GX_OAM_MODE_BITMAPOBJ = 3
}
GXOamMode;

#define GX_OAM_ATTR01_RSENABLE_SHIFT                      8
#define GX_OAM_ATTR01_HF_SHIFT                            28
#define GX_OAM_ATTR01_VF_SHIFT                            29

typedef enum
{
    GX_OAM_EFFECT_NONE =
        (0 << GX_OAM_ATTR01_RSENABLE_SHIFT) | (0 << GX_OAM_ATTR01_HF_SHIFT) | (0 <<
                                                                               GX_OAM_ATTR01_VF_SHIFT),
    GX_OAM_EFFECT_FLIP_H =
        (0 << GX_OAM_ATTR01_RSENABLE_SHIFT) | (1 << GX_OAM_ATTR01_HF_SHIFT) | (0 <<
                                                                               GX_OAM_ATTR01_VF_SHIFT),
    GX_OAM_EFFECT_FLIP_V =
        (0 << GX_OAM_ATTR01_RSENABLE_SHIFT) | (0 << GX_OAM_ATTR01_HF_SHIFT) | (1 <<
                                                                               GX_OAM_ATTR01_VF_SHIFT),
    GX_OAM_EFFECT_FLIP_HV =
        (0 << GX_OAM_ATTR01_RSENABLE_SHIFT) | (1 << GX_OAM_ATTR01_HF_SHIFT) | (1 <<
                                                                               GX_OAM_ATTR01_VF_SHIFT),
    GX_OAM_EFFECT_AFFINE = (1 << GX_OAM_ATTR01_RSENABLE_SHIFT),
    GX_OAM_EFFECT_NODISPLAY = (2 << GX_OAM_ATTR01_RSENABLE_SHIFT),
    GX_OAM_EFFECT_AFFINE_DOUBLE = (3 << GX_OAM_ATTR01_RSENABLE_SHIFT)
}
GXOamEffect;

//-----------------------------------------------------------------------------

#if 1
#define OS_Printf printf
#endif
#if 0
#define OS_Printf printfDx
#endif
#if 0
#define OS_Printf nn_commonLogPrintf
#endif

//-----------------------------------------------------------------------------

typedef int OSHeapHandle;

//-----------------------------------------------------------------------------

typedef struct NNSFndAllocator NNSFndAllocator;
typedef void*   (*NNSFndFuncAllocatorAlloc)(
                    NNSFndAllocator*    pAllocator,
                    u32                 size);
typedef void    (*NNSFndFuncAllocatorFree)(
                    NNSFndAllocator*    pAllocator,
                    void*               memBlock);
typedef struct NNSFndAllocatorFunc NNSFndAllocatorFunc;
struct NNSFndAllocatorFunc
{
    NNSFndFuncAllocatorAlloc    pfAlloc;
    NNSFndFuncAllocatorFree     pfFree;
};
struct NNSFndAllocator
{
    NNSFndAllocatorFunc const * pFunc;
    void*                       pHeap;
    u32                         heapParam1;
    u32                         heapParam2;
};

//-----------------------------------------------------------------------------

struct FSFile;
typedef enum
{
    MI_PROCESSOR_ARM9 = 0,             // Main processor
    MI_PROCESSOR_ARM7 = 1              // Sub processor
}
MIProcessor;

//-----------------------------------------------------------------------------

typedef u32 FSOverlayID;

typedef struct
{
    u32     offset;
    u32     length;
}
CARDRomRegion;
/* overlay static initializer function */
typedef void (*FSOverlayInitFunc) (void);
typedef struct
{
/* protected: */
    u32     id;                        /* own overlay ID (equal to index of OVT element) */
    u8     *ram_address;               /* start-address of overlay */
    u32     ram_size;                  /* size of [text+data] section */
    u32     bss_size;                  /* size of bss section */
    /* static initializer function table */
    FSOverlayInitFunc *sinit_init;
    FSOverlayInitFunc *sinit_init_end;
    u32     file_id;                   /* index as FAT of "rom" archive */
    u32     compressed:24;             /* size of commpressed 'ram_size' */
    u32     flag:8;                    /* multi-purpose flag */
}
FSOverlayInfoHeader;
/*
 * overlay module information structure.
 * (FSOverlayInfoHeader and some extensions)
 */
typedef struct
{
/* private: */
    FSOverlayInfoHeader header;
    MIProcessor target;                /* target processor */
    CARDRomRegion file_pos;            /* region in CARD-ROM */
}
FSOverlayInfo;

//-----------------------------------------------------------------------------

//---- masked value
#define PAD_PLUS_KEY_MASK       0x00f0 // mask : cross keys
#define PAD_BUTTON_MASK         0x2f0f // mask : buttons
#define PAD_DEBUG_BUTTON_MASK   0x2000 // mask : debug button
#define PAD_ALL_MASK            0x2fff // mask : all buttons
#define PAD_RCNTPORT_MASK       0x2c00 // mask : factors ARM7 can read from RCNT register
#define PAD_KEYPORT_MASK        0x03ff // mask : factors ARM7/9 can read from KEY register

#define PAD_DETECT_FOLD_MASK    0x8000 // mask : folding

//---- button and key
#define PAD_BUTTON_A            0x0001 // A
#define PAD_BUTTON_B            0x0002 // B
#define PAD_BUTTON_SELECT       0x0004 // SELECT
#define PAD_BUTTON_START        0x0008 // START
#define PAD_KEY_RIGHT           0x0010 // RIGHT of cross key
#define PAD_KEY_LEFT            0x0020 // LEFT  of cross key
#define PAD_KEY_UP              0x0040 // UP    of cross key
#define PAD_KEY_DOWN            0x0080 // DOWN  of cross key
#define PAD_BUTTON_R            0x0100 // R
#define PAD_BUTTON_L            0x0200 // L
#define PAD_BUTTON_X            0x0400 // X
#define PAD_BUTTON_Y            0x0800 // Y
#define PAD_BUTTON_DEBUG        0x2000 // Debug button

//-----------------------------------------------------------------------------

typedef struct NNSG2dCharWidths
{
    s8 left;                            // �����̍��X�y�[�X�̕�
    u8 glyphWidth;                      // �����̃O���t��
    s8 charWidth;                       // �����̕� = ���X�y�[�X�� + �O���t�� + �E�X�y�[�X��
}
NNSG2dCharWidths;
typedef struct NNSG2dFontWidth
{
    u16                     indexBegin;     // widthTable �̍ŏ��̃G���g�����Ή�����O���t�C���f�b�N�X
    u16                     indexEnd;       // widthTable �̍Ō�̃G���g�����Ή�����O���t�C���f�b�N�X
    struct NNSG2dFontWidth* pNext;          // ���� NNSG2dFontWidth �ւ̃|�C���^
    NNSG2dCharWidths        widthTable[];   // �����̔z��
}
NNSG2dFontWidth;
typedef struct NNSG2dFontCodeMap
{
    u16                         ccodeBegin;     // ���̃u���b�N���S�����镶���R�[�h�͈͂̍ŏ��̕����R�[�h
    u16                         ccodeEnd;       // ���̃u���b�N���S�����镶���R�[�h�͈͂̍Ō�̕����R�[�h
    u16                         mappingMethod;  // �}�b�s���O���@ (NNSG2dFontMappingMethod�^)
    u16                         reserved;       // �\��
    struct NNSG2dFontCodeMap*   pNext;          // ���� NNSG2dFontCodeMap �ւ̃|�C���^
    u16                         mapInfo[];      // �����R�[�h�}�b�s���O��� ��̓I�ȓ��e�� mappingMethod �Ɉ˂�
}
NNSG2dFontCodeMap;
typedef struct NNSG2dFontGlyph
{
    u8  cellWidth;          // �O���t�C���[�W1���C���̃s�N�Z����
    u8  cellHeight;         // �O���t�C���[�W�̃��C����
    u16 cellSize;           // 1�O���t������̃O���t�C���[�W�f�[�^�T�C�Y
    s8  baselinePos;        // (*)�O���t�C���[�W��[����x�[�X���C���܂ł̋���
    u8  maxCharWidth;       // (*)�ő啶����
    u8  bpp;                // �O���t�C���[�W1�s�N�Z���������bit��
    u8  flags;              // NNSG2dFontGlyphFlag

    u8  glyphTable[];       // �O���t�C���[�W�̔z��
}
NNSG2dFontGlyph;
typedef u16 (*NNSiG2dSplitCharCallback)(const void** ppChar);
typedef struct NNSG2dFontInformation
{
    u8                  fontType;       // �O���t�f�[�^�^�C�v (NNSG2dFontType�^)
    s8                  linefeed;       // (*)== leading
    u16                 alterCharIndex; // (*)�t�H���g�Ɋ܂܂�Ȃ������p�̃O���t�̃O���t�C���f�b�N�X
    NNSG2dCharWidths    defaultWidth;   // (*)���������������Ȃ��O���t�p�̕��������
    u8                  encoding;       // �Ή����镶����G���R�[�f�B���O (NNSG2dFontEncoding�^)
    NNSG2dFontGlyph*    pGlyph;         // �B��� NNSG2dFontGlyph �ւ̃|�C���^
    NNSG2dFontWidth*    pWidth;         // �ŏ��� NNSG2dFontWidth �ւ̃|�C���^
    NNSG2dFontCodeMap*  pMap;           // �ŏ��� NNSG2dFontCodeMap �ւ̃|�C���^
}
NNSG2dFontInformation;
typedef struct NNSG2dFont
{
    NNSG2dFontInformation*   pRes;          // �W�J�ς݃t�H���g���\�[�X�ւ̃|�C���^
    NNSiG2dSplitCharCallback cbCharSpliter; // ������G���R�[�f�B���O�����R�[���o�b�N�ւ̃|�C���^
}
NNSG2dFont;

#define PI (3.1415926535897932384626433832795)

//-----------------------------------------------------------------------------

#endif //__TYPE_H__

