#ifndef __SOUND_NNS_H__
#define __SOUND_NNS_H__

#include "type.h"

//---------------------------------------------------------------------------

typedef struct _sound_group_info {
	int bgmStartNo;
	int bgmN;
	int seStartNo;
	int seN;
} SOUND_GROUP_INFO;

void sound_nnsInit( void * buf, u32 size, char * dataPath);
void sound_nnsInitDx( char ** bgmFileNameArray, char ** seFileNameArray, SOUND_GROUP_INFO * soundGroupInfo);
void sound_nnsDestroy(void);
void sound_nnsInitOnMemory( void * data);
void sound_nnsTask( void);
void sound_nnsTaskVoiceProgram( void);
BOOL sound_nnsIsVoiceProgramStop( void);
BOOL sound_nnsIsLoadAsyncActive( void);
void sound_nnsLoadGroupUseThread( int groupNo, void (* loadDestructorFunc)( void *), void * loadDestructorFuncArg);
void sound_nnsLoadGroup( int groupNo);
void sound_nnsLoadSeqUseThread( int seqNo, void (* loadDestructorFunc)( void *), void * loadDestructorFuncArg);
void sound_nnsLoadSeq( int seqNo);
void sound_nnsLoadSeqArc( int seqarcNo);
void sound_nnsLoadBank( int bankNo);
void sound_nnsHeapSaveState( void);
void sound_nnsHeapLoadState( void);
void sound_nnsHeapLoadStateBack( void);
u32 sound_nnsGetHeapFreeSize( void);
void sound_nnsPlaySe( int seNo);
void sound_nnsStopSeFade( int fadeFrame);
void sound_nnsStopSe( void);
void sound_nnsStopSeNo( int seNo);
void sound_nnsPlaySeCallBack( int SeNo, void (* callBackFunc)( void *), void * callBackFuncArg);
void sound_nnsPlayVoice( int voiceNo);
void sound_nnsStopVoice( void);
void sound_nnsPlayVoiceProgram( int voiceNoProgram[], int programNum);
void sound_nnsStopVoiceProgram( void);
void sound_nnsPlaySeq( int seqNo);
void sound_nnsStopSeq( void);
void sound_nnsPlayBgm( int bgmNo);
void sound_nnsStopBgm( void);
void sound_nnsStopBgmFade( int fadeFrame);

void sound_nnsSetVoicePlayerNo( int setNo);
void sound_nnsSetSeqArcNo( int setNo);

void sound_nnsSetVolumeSe( int volume);
void sound_nnsSetVolumeBgm( int volume);

BOOL sound_nnsPlaySeLoadCallBack( int groupNo, int seNo, void (* destructorFunc)( void *), void * destructorFuncArg);
BOOL sound_nnsPlaySeLoad( int groupNo, int seNo);
BOOL sound_nnsIsPlaySeLoadActive( void);

void sound_nnsPauseSeqDX( int seqNo);
void sound_nnsResumeSeqDX( int seqNo);
void sound_nnsPlaySeqTimeDX( int seqNo, int startMilliSec);
	
//---------------------------------------------------------------------------

#endif //__SOUND_NNS_H__

