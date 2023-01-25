
#ifndef __FS_H__
#define __FS_H__
/*
#include <nitro.h>
#include <nnsys.h>
*/
#include "type.h"

void fsTask( void);

void fsInitSimple( void);
void fsInitUseDma( u32 defaultDmaNo);
void fsInitUseAllocator( NNSFndAllocator * allocator);
void fsInitUseDmaUseAllocator( u32 defaultDmaNo, NNSFndAllocator * allocator);

BOOL fsIsFileAsyncBusyEx( FSFile * file);
BOOL fsLoadFileAsyncExStart( void * buf,  FSFile * file,char * fileName, int readSize);
BOOL fsLoadFileAsyncExEnd( FSFile * file);

BOOL fsIsFileAsyncBusy( void);
BOOL fsIsLoadAsyncActive( void);
BOOL fsLoadOverlayAsync( MIProcessor target, FSOverlayID overlay_id, void (* asyncDestructorFunc)( void *), void * asyncDestructorFuncArg);
BOOL fsLoadFileAsync( void * buf, char * fileName, int readSize, void (* asyncDestructorFunc)( void *), void * asyncDestructorFuncArg);
BOOL fsLoadFileAsyncStart( void * buf, char * fileName, int readSize);
BOOL fsLoadFileAsyncEnd( void);

BOOL fsLoadFileAsyncExStart( FSFile * file, char * fileName, int readSize);
BOOL fsLoadFileAsyncExEnd( FSFile * file);
BOOL fsLoadFile( void * buf, char * fileName, int readSize);

u32 fsGetFileSize( char * fileName);

BOOL fsLoadFileAllocSimple( void ** buf, char * fileName);

#endif //__FS_H__


#ifndef __FS_H__
#define __FS_H__

#include <nitro.h>
#include <nnsys.h>

void fsTask( void);

void fsInitSimple( void);
void fsInitUseDma( u32 defaultDmaNo);
void fsInitUseAllocator( NNSFndAllocator* allocator);
void fsInitUseDmaUseAllocator( u32 defaultDmaNo, NNSFndAllocator* allocator);

BOOL fsIsFileAsyncBusyEx( FSFile * file);
BOOL fsLoadFileAsyncExStart( void * buf,  FSFile * file,char * fileName, int readSize);
BOOL fsLoadFileAsyncExEnd( FSFile * file);

BOOL fsIsFileAsyncBusy( void);
BOOL fsIsLoadAsyncActive( void);
BOOL fsLoadOverlayAsync( MIProcessor target, FSOverlayID overlay_id, void (* asyncDestructorFunc)( void *), void * asyncDestructorFuncArg);
BOOL fsLoadFileAsync( void * buf, char * fileName, int readSize, void (* asyncDestructorFunc)( void *), void * asyncDestructorFuncArg);
BOOL fsLoadFileAsyncStart( void * buf, char * fileName, int readSize);
BOOL fsLoadFileAsyncEnd( void);

BOOL fsLoadFileAsyncExStart( FSFile * file, char * fileName, int readSize);
BOOL fsLoadFileAsyncExEnd( FSFile * file);
BOOL fsLoadFile( void * buf, char * fileName, int readSize);

u32 fsGetFileSize( char * fileName);

#endif //__FS_H__

