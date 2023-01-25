
#ifndef __SETUP_H__
#define __SETUP_H__

#include "task.h"
#include "master.h"

typedef struct _setup_class {
	u16 state;
	u16 cnt;
	TASK * taskThis;
	MASTER * master;
	
	int textureMainLoadArray[4];
	int textureMainLoadN;
	int textureSubBgLoadArray[4];
	int textureSubBgLoadN;
	int textureSubObjLoadArray[4];
	int textureSubObjLoadN;
	int loadCnt;
	BOOL (* setupFunc[4])(void *);
	void * setupFuncArg[4];
	int setupFuncN;
	
	void (* destructorFunc)( void *);
	void * destructorFuncArg;
} SETUP_CLASS;

TASK * setupCreateTaskMain( TASK * oya, SETUP_CLASS * setupArg);

#endif //__SETUP_H__

