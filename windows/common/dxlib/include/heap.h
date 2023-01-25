#ifndef __HEAP_H__
#define __HEAP_H__

/*
#include <nitro.h>
*/

#include "type.h"

OSHeapHandle heapInit( void);
void heapDispFreeSize( void);
void * heapAlloc( u32 size);
void heapFree( void * ptr);
void heapSaveState( void);
void heapLoadState( void);
void heapCheckStart( void);
void heapCheckDisp( void);

#endif	//__HEAP_H__

