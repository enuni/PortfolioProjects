#include <time.h>

#include "nn_common.h"

#include "heap.h"
#include "draw.h"
#include "font.h"
#include "sound_nns.h"

#ifdef __SOUND__
#include "sound_data.sadl"
#endif //__SOUND__

#include "init.h"

//---------------------------------------------------------------------------

#ifdef __SOUND__
#define SOUND_HEAP_SIZE ((1024) * 768)

static void * sSoundHeap = NULL;

#ifdef __CHILD__
extern u32 sound_data_sdat[];
#endif //__CHILD__
#endif //__SOUND__

//---------------------------------------------------------------------------

#ifdef __SOUND__
static SOUND_GROUP_INFO sSoundGroupInfoArray[] = {
	{ 0, 3, 0, 16, },
};

static char * sBgmFileNameArray[] = {
	"bgm01.mp3",
	"bgm02.mp3",
	"bgm03.mp3",
};

static char * sSeFileNameArray[] = {
	"se01.wav",
	"se02.wav",
	"se03.wav",
	"se04.wav",
	"se05.wav",
	"se06.wav",
	"se07.wav",
	"se08.wav",
	"se09.wav",
	"se10.wav",
	"se11.wav",
	"se12.wav",
	"se13.wav",
	"se14.wav",
	"se15.wav",
	"se16.wav",
};
#endif //__SOUND__

//---------------------------------------------------------------------------

void initSystem( void) {
	
}

//---------------------------------------------------------------------------

void initMain( void) {
	
	//描画初期化
	drawInit();
	drawMainInit();
	drawMain3dInit();
	drawMain2dInit();
	drawSub2dInit();

	//サウンド初期化
#ifdef __SOUND__
#ifndef __CHILD__
	sSoundHeap = heapAlloc( SOUND_HEAP_SIZE);
	sound_nnsInitDx( sBgmFileNameArray, sSeFileNameArray, sSoundGroupInfoArray);
	sound_nnsLoadGroup( GROUP_ALL);
	sound_nnsLoadSeq( BGM_INTERVAL);
	sound_nnsLoadSeq( BGM_DUNGEONOPENING);
	sound_nnsLoadSeq( BGM_BOSS);
	sound_nnsHeapSaveState();
	sound_nnsSetVolumeBgm( 64);
	sound_nnsSetVolumeSe( 64);
	OS_Printf( "sound_nnsGetHeapFreeSize = %d\n", sound_nnsGetHeapFreeSize());
#else //__CHILD__
	sound_nnsInitOnMemory( sound_data_sdat);
#endif //__CHILD__
#endif //__SOUND__
	
	//フォント初期化
#if 1
	fontInitFont( NULL, NULL);
	fontSubBg1Init();
	fontMainBg1Init();
#endif
	
	// 出力先設定
#ifdef __CHILD__
	drawSetDispSubMain();
#else //__CHILD__
	drawSetDispMainSub();
#endif //__CHILD__
	
	drawStartDisplay();
	
}

//---------------------------------------------------------------------------

void initRand( void) {

	unsigned int seed;

	seed = (unsigned int)time( NULL);

	::n_commonRandInit( seed);
	::n_commonRandInitCpu( seed);
	::n_commonRandInitDirect( seed);
	
}

//---------------------------------------------------------------------------

BOOL initCardEject( void) {

	return TRUE;
	
}

//---------------------------------------------------------------------------

void initVBlankIntr( void) {
	
}

//---------------------------------------------------------------------------

void initCardSetThreadPriority( u32 priority) {

	(void)priority;

}

//---------------------------------------------------------------------------

