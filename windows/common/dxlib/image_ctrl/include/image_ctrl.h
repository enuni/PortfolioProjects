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
	IMAGE_CTRL_STATE_HEAP_LOAD,		//ヒープ（メモリ）にロード中
	IMAGE_CTRL_STATE_HEAP_COMP,		//ヒープ（メモリ）にロード完了
	IMAGE_CTRL_STATE_VRAM_LOAD,		//VRAMにロード中
	IMAGE_CTRL_STATE_ACTIVE,		//VRAMにロード完了 表示可能
	
	IMAGE_CTRL_STATE_MAX
};

enum {
	IMAGE_CTRL_ASYNC_CTRL_STATE_NONE,		//現在非同期ロードをしていない
	IMAGE_CTRL_ASYNC_CTRL_STATE_IMAGE_HEAP,		//imageをヒープ（メモリ）にロード中
	IMAGE_CTRL_ASYNC_CTRL_STATE_IMAGE_HEAP_COMP,		//imageをヒープ（メモリ）にロード完了
	IMAGE_CTRL_ASYNC_CTRL_STATE_IMAGE_VRAM,		//imageをVRAMにロード中
	IMAGE_CTRL_ASYNC_CTRL_STATE_IMAGE_VRAM_COMP,		//imageをVARMにロード中
	IMAGE_CTRL_ASYNC_CTRL_STATE_PALETTE_HEAP,		//paletteをヒープ（メモリ）にロード中
	IMAGE_CTRL_ASYNC_CTRL_STATE_PALETTE_HEAP_COMP,		//paletteをヒープ（メモリ）にロード完了
	IMAGE_CTRL_ASYNC_CTRL_STATE_PALETTE_VRAM,		//paletteをVRAMにロード中
	IMAGE_CTRL_ASYNC_CTRL_STATE_COMP,		//paletteをVARMにロード完了
	
	IMAGE_CTRL_SYNC_CTRL_STATE_MAX
};

enum {
	IMAGE_CTRL_ASYNC_CTRL_TYPE_NONE,
	IMAGE_CTRL_ASYNC_CTRL_TYPE_MAIN_3D,		//メイン画面3d用のimage paletteを読み込み中
	IMAGE_CTRL_ASYNC_CTRL_TYPE_SUB_2D_BG_BG,		//サブ画面2d背景用のimage paletteを読み込み中
	IMAGE_CTRL_ASYNC_CTRL_TYPE_SUB_2D_OBJ,		//サブ画面2dobj用のimage paletteを読み込み中
	
	IMAGE_CTRL_ASYNC_CTRL_TYPE_MAX
};

//---------------------------------------------------------------------------

void image_ctrlInit();

void image_ctrlSetBgSubData( void * bgTop, void * paletteTop);
void image_ctrlSetTextureData( int index, void * textureTop, void * paletteTop, int idStart, int idEnd);
void image_ctrlSetImageData( int index, void * imageTop, void * paletteTop, int idStart, int idEnd);

//毎フレーム呼び必要な処理をする
void image_ctrlTask( void);
//image_ctrlTask()から呼び出される
void image_ctrlTaskAsyncCtrl( void);

//imageの非同期ロード中かを返す
BOOL image_ctrlIsLoadAsyncActive( void);

//imageの非同期ロードの状態を返す IMAGE_CTRL_ASYNC_STATE_～
int image_ctrlGetLoadAsyncState( void);

int image_ctrlGetSub2dImageHandle( int imageId);
// imageId のサブ画面2dobj用のimageとデフォルトのpaletteがVRAM上にあり 表示可能かを返す
BOOL image_ctrlIsSub2dObjImagePaletteActive( int imageId);
int image_ctrlGetSub2dObjImageColor( int imageId);
GXOamShape image_ctrlGetSub2dObjShape( int w, int h);
int image_ctrlGetSub2dObjPaletteNo( int imageId);
u32 image_ctrlGetSub2dObjImageVramOffset( int imageId);

int image_ctrlGetSub2dBgBgHandle( int bgId);
// bgId のサブ画面背景用のimageとデフォルトのpaletteがVRAM上にあり 表示可能かを返す
BOOL image_ctrlIsSub2dBgBgImagePaletteActive( int bgId);
u8 image_ctrlGetSub2dBgBgPaletteNo( int paletteId);

int image_ctrlIsMain3dTextureHandle( int textureId);
// textureId のメイン画面3d用のimageとデフォルトのpaletteがVRAM上にあり 表示可能かを返す
BOOL image_ctrlIsMain3dImagePaletteActive( int textureId);
int image_ctrlGetMain3dImageColor( int textureId);
GXTexSizeS image_ctrlGetMain3dImageS( int textureId);
GXTexSizeT image_ctrlGetMain3dImageT( int textureId);
u32 image_ctrlGetMain3dPaletteVramOffset( int paletteId);
u32 image_ctrlGetMain3dImageVramOffset( int textureId);

//---------------------------------------------------------------------------

//ツールで出力した textureId のメイン画面3d用のimageをヒープ（メモリ）にロードを開始する 非同期ノンブロック 開始に成功したら TRUE を返す
BOOL image_ctrlLoadMain3dImageHeapAsync( int textureId);
//ツールで出力した textureId のメイン画面3d用のimageをVRAMにロードを開始する 非同期ノンブロック
//あらかじめヒープ（メモリ）にロードしておく必要がある 開始に成功したら TRUE を返す
BOOL image_ctrlLoadMain3dImageVramAsync( int textureId);
//ツールで出力した paletteId のメイン画面3d用のpaletteをヒープ（メモリ）にロードを開始する 非同期ノンブロック 開始に成功したら TRUE を返す
BOOL image_ctrlLoadMain3dPaletteHeapAsync( int paletteId);
//ツールで出力した paletteId のメイン画面3d用のpaletteをVRAMにロードを開始する 非同期ノンブロック
//あらかじめヒープ（メモリ）にロードしておく必要がある 開始に成功したら TRUE を返す
BOOL image_ctrlLoadMain3dPaletteVramAsync( int paletteId);

//ツールで出力した textureId のメイン画面3d用のimageとデフォルトのpaletteをヒープ（メモリ）とVRAMへの一括ロードを開始する
//非同期ノンブロック 開始に成功したら TRUE を返す
BOOL image_ctrlLoadMain3dImagePaletteHeapVramAsync( int textureId);

//ツールで出力した textureId のメイン画面3d用のimageをヒープ（メモリ）にロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadMain3dImageHeap( int textureId);
//ツールで出力した paletteId のメイン画面3d用のpaletteをヒープ（メモリ）にロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadMain3dPaletteHeap( int paletteId);
//ツールで出力した textureId のメイン画面3d用のimageをVRAMにロードする
//あらかじめヒープ（メモリ）にロードしておく必要がある 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadMain3dImageVram( int textureId);
//ツールで出力した paletteId のメイン画面3d用のpaletteをVRAMにロードする
//あらかじめヒープ（メモリ）にロードしておく必要がある 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadMain3dPaletteVram( int paletteId);

//ツールで出力した textureId のメイン画面3d用のimageとデフォルトのpaletteをヒープ（メモリ）とVRAMへの一括ロードする 成功したら TRUE を返す  ブロック
BOOL image_ctrlLoadMain3dImagePaletteHeapVram( int textureId);
//ツールなし用
int image_ctrlLoadMain3dImagePaletteHeapVram( int index, char * textureFileName);

//image_ctrlLoadMain3d～()でロードした textureId のメイン画面3d用のimageをヒープ（メモリ）から開放する
BOOL image_ctrlReleaseMain3dImageHeap( int textureId);
//image_ctrlLoadMain3d～()でロードした paletteId のメイン画面3d用のpaletteをヒープ（メモリ）から開放する
BOOL image_ctrlReleaseMain3dPaletteHeap( int paletteId);
//image_ctrlLoadMain3d～()でロードした textureId のメイン画面3d用のimageをVRAMから開放する
BOOL image_ctrlReleaseMain3dImageVram( int textureId);
//image_ctrlLoadMain3d～()でロードした paletteId のメイン画面3d用のpaletteをVRAMから開放する
BOOL image_ctrlReleaseMain3dPaletteVram( int paletteId);

//image_ctrlLoadMain3d～()でロードした textureId のメイン画面3d用のimageとデフォルトのpaletteをヒープ（メモリ）とVRAMから一括開放する
BOOL image_ctrlReleaseMain3dImagePaletteHeapVram( int textureId);

//---------------------------------------------------------------------------

//ツールで出力した bgId のサブ画面背景用のimageをヒープ（メモリ）にロードを開始する 非同期ノンブロック 開始に成功したら TRUE を返す
BOOL image_ctrlLoadSub2dBgBgImageHeapAsync( int bgId);
//ツールで出力した bgId のサブ画面背景用のImageをVRAMにロードを開始する 非同期ノンブロック
//あらかじめヒープ（メモリ）にロードしておく必要がある 開始に成功したら TRUE を返す
BOOL image_ctrlLoadSub2dBgBgImageVramAsync( int bgId);
//ツールで出力した paletteId のサブ画面背景用のpaletteをヒープ（メモリ）にロードを開始する 非同期ノンブロック 開始に成功したら TRUE を返す
BOOL image_ctrlLoadSub2dBgBgPaletteHeapAsync( int paletteId);
//ツールで出力した paletteId のサブ画面背景用のpaletteをVRAMにロードを開始する 非同期ノンブロック
//あらかじめヒープ（メモリ）にロードしておく必要がある 開始に成功したら TRUE を返す
BOOL image_ctrlLoadSub2dBgBgPaletteVramAsync( int paletteId);

//ツールで出力した bgId のサブ画面背景用のimageとデフォルトのpaletteをヒープ（メモリ）とVRAMへの一括ロードを開始する
//非同期ノンブロック 開始に成功したら TRUE を返す
BOOL image_ctrlLoadSub2dBgBgImagePaletteHeapVramAsync( int bgId);

//ツールで出力した bgId のサブ画面背景用のimageをヒープ（メモリ）にロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadSub2dBgBgImageHeap( int bgId);
//ツールで出力した paletteId のサブ画面背景用のpaletteをヒープ（メモリ）にロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadSub2dBgBgPaletteHeap( int paletteId);
//ツールで出力した bgId のサブ画面背景用のimageをVRAMにロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadSub2dBgBgImageVram( int bgId);
//ツールで出力した paletteId のサブ画面背景用のpaletteをVRAMにロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadSub2dBgBgPaletteVram( int paletteId);

//ツールで出力した bgId のサブ画面背景用のimageとデフォルトのpaletteをヒープ（メモリ）とVRAMへの一括にロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadSub2dBgBgImagePaletteHeapVram( int bgId);

//image_ctrlLoadSub2dBgBg～()でロードした bgId のサブ画面背景用のimageをヒープ（メモリ）から開放する
BOOL image_ctrlReleaseSub2dBgBgImageHeap( int bgId);
//image_ctrlLoadSub2dBgBg～()でロードした paletteId のサブ画面背景用のpaletteをヒープ（メモリ）から開放する
BOOL image_ctrlReleaseSub2dBgBgPaletteHeap( int paletteId);
//image_ctrlLoadSub2dBgBg～()でロードした bgId のサブ画面背景用のimageをVRAMから開放する
BOOL image_ctrlReleaseSub2dBgBgImageVram( int bgId);
//image_ctrlLoadSub2dBgBg～()でロードした paletteId のサブ画面背景用のpaletteををVRAMから開放する
BOOL image_ctrlReleaseSub2dBgBgPaletteVram( int paletteId);

//image_ctrlLoadSub2dBgBg～()でロードした bgId のサブ画面背景用のimageとデフォルトのpaletteをヒープ（メモリ）とVRAMから一括開放する
BOOL image_ctrlReleaseSub2dBgBgImagePaletteHeapVram( int bgId);

//---------------------------------------------------------------------------

//ツールで出力した imageId のサブ画面obj用のimageをヒープ（メモリ）にロードを開始する 非同期ノンブロック 開始に成功したら TRUE を返す
BOOL image_ctrlLoadSub2dObjImageHeapAsync( int imageId);
//ツールで出力した imageId のサブ画面obj用のImageをVRAMにロードを開始する 非同期ノンブロック
//あらかじめヒープ（メモリ）にロードしておく必要がある 開始に成功したら TRUE を返す
BOOL image_ctrlLoadSub2dObjImageVramAsync( int imageId);
//ツールで出力した paletteId のサブ画面obj用のpaletteをヒープ（メモリ）にロードを開始する 非同期ノンブロック 開始に成功したら TRUE を返す
BOOL image_ctrlLoadSub2dObjPaletteHeapAsync( int paletteId);
//ツールで出力した paletteId のサブ画面obj用のpaletteをVRAMにロードを開始する 非同期ノンブロック
//あらかじめヒープ（メモリ）にロードしておく必要がある 開始に成功したら TRUE を返す
BOOL image_ctrlLoadSub2dObjPaletteVramAsync( int paletteId);

//ツールで出力した imageId のサブ画面obj用のimageとデフォルトのpaletteをヒープ（メモリ）とVRAMへの一括ロードを開始する
//非同期ノンブロック 開始に成功したら TRUE を返す
BOOL image_ctrlLoadSub2dObjImagePaletteHeapVramAsync( int imageId);

//ツールで出力した imageId のサブ画面obj用のimageをヒープ（メモリ）にロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadSub2dObjImageHeap( int imageId);
//ツールで出力した paletteId のサブ画面obj用のpaletteをヒープ（メモリ）にロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadSub2dObjPaletteHeap( int paletteId);
//ツールで出力した imageId のサブ画面obj用のimageをVRAMにロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadSub2dObjImageVram( int imageId);
//ツールで出力した paletteId のサブ画面obj用のpaletteをVRAMにロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadSub2dObjPaletteVram( int paletteId);

//ツールで出力した imageId のサブ画面obj用のimageとデフォルトのpaletteをヒープ（メモリ）とVRAMへの一括にロードする 成功したら TRUE を返す ブロック
BOOL image_ctrlLoadSub2dObjImagePaletteHeapVram( int imageId);

//image_ctrlLoadSub2dObj～()でロードした imageId のサブ画面obj用のimageをヒープ（メモリ）から開放する
BOOL image_ctrlReleaseSub2dObjImageHeap( int imageId);
//image_ctrlLoadSub2dObj～()でロードした paletteId のサブ画面obj用のpaletteをヒープ（メモリ）から開放する
BOOL image_ctrlReleaseSub2dObjPaletteHeap( int paletteId);
//image_ctrlLoadSub2dObj～()でロードした imageId のサブ画面obj用のimageをVRAMから開放する
BOOL image_ctrlReleaseSub2dObjImageVram( int imageId);
//image_ctrlLoadSub2dObj～()でロードした paletteId のサブ画面obj用のpaletteををVRAMから開放する
BOOL image_ctrlReleaseSub2dObjPaletteVram( int paletteId);

//image_ctrlLoadSub2dObj～()でロードした imageId のサブ画面obj用のimageとデフォルトのpaletteをヒープ（メモリ）とVRAMから一括開放する
BOOL image_ctrlReleaseSub2dObjImagePaletteHeapVram( int imageId);

// 101014 DXlibマスク機能追加
BOOL image_ctrlLoadMain3dMask( int textureId);
BOOL image_ctrlLoadMain3dMaskAsync( int textureId);
BOOL image_ctrlReleaseMain3dMask( int textureId);

TEXTURE * image_ctrlGetTexture( int textureId);

#endif	//__IMAGE_CTRL_H__

