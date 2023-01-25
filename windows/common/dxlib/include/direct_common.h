
#ifndef __DIRECT_COMMON_H__
#define __DIRECT_COMMON_H__

#include "task.h"
#include "master.h"

//---------------------------------------------------------------------------

typedef struct _slide_se_data {
	int scene;
	int frame;
	int seNo;
} SLIDE_SE_DATA;

typedef struct _slide_data {
	TASK * taskThis;
	MASTER * master;
	
	u16 state;
	u16 cnt;
	u16 scene;
	
	int objId[32][32];
	float x[32];
	float y[32];
	int z[32][32];
	float a[32];
	u16 frame[32];
	int xKey[32][32];
	int yKey[32][32];
	int aKey[32][32];
	float xVar[32][32];
	float yVar[32][32];
	float aVar[32][32];
	int objN;
	int sceneN;
	u16 skipScene[32];
	
	int modeAccDec[32][32];	//0í èÌ 1â¡ë¨ 2å∏ë¨
	float xAccDec[32][32];
	float yAccDec[32][32];
	
	BOOL objIdVarFlag;
	BOOL zVarFlag;
	int aVarMode[32];	//031Ç≈å≈íË 1èâä˙ílÇ≈å≈íË 2ïœâª
	
	SLIDE_SE_DATA se[8];
	int seN;
	
	void (* destructorFunc)( void *);
	void * destructorFuncArg;
} SLIDE_DATA;

enum WAIT_DESTRUCT_STATE {
	WAIT_DESTRUCT_STATE_NORMAL,
	WAIT_DESTRUCT_STATE_SKIP,

	WAIT_DESTRUCT_STATE_MAX
};

enum WAIT_MODE {
	WAIT_MODE_DEFAULT,
	WAIT_MODE_SKIP,

	WAI_MODE_MAX
};

typedef struct _direct_common_wait {
	
	TASK * taskThis;
	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	u16 cnt;
	int frame;
	int mode;

	int state;

	bool (* isSkipFunc)( void *);
	void * isSkipFuncArg;

	void (* destructorFuncSkip)( void *, int);
	void * destructorFuncSkipArg;
	
} DIRECT_COMMON_WAIT;

TASK * direct_commonCreateTaskWhiteFadeOutMainSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);
TASK * direct_commonCreateTaskWhiteFadeInMainSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);
TASK * direct_commonCreateTaskBlackFadeOutMainSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);
TASK * direct_commonCreateTaskBlackFadeInMainSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);

TASK * direct_commonCreateTaskWhiteFadeOutSubSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);
TASK * direct_commonCreateTaskWhiteFadeInSubSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);
TASK * direct_commonCreateTaskBlackFadeOutSubSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);
TASK * direct_commonCreateTaskBlackFadeInSubSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);

TASK * direct_commonCreateTaskWhiteFadeOutAllSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);
TASK * direct_commonCreateTaskWhiteFadeInAllSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);
TASK * direct_commonCreateTaskBlackFadeOutAllSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);
TASK * direct_commonCreateTaskBlackFadeInAllSimple( TASK * oya, void (* destructorFunc)(void *), void * destructorFuncArg);

TASK * direct_commonCreateTaskFadeSimple( TASK * oya, s8 * brightnessTbl, BOOL main, BOOL sub, void (* destructorFunc)(void *), void * destructorFuncArg);

TASK * direct_commonCreateTaskSlide( TASK * oya, SLIDE_DATA * slideData);

TASK * direct_commonWaitSimple( TASK * oya, int waitFrame, void (* destructorFunc)( void *), void * destructorFuncArg);
TASK * direct_commonCreateTaskWait( TASK * oya, DIRECT_COMMON_WAIT * waitArg);
TASK * direct_commonWaitSkip( TASK * oya, int waitFrame, void (* destructorFunc)( void *, int state), void * destructorFuncArg, bool (* isSkipFunc)( void *), void * isSkipFuncArg);

//---------------------------------------------------------------------------

#endif //__DIRECT_COMMON_H__

