#ifndef __BACKUP_H__
#define __BACKUP_H__

enum {
	BACKUP_RESULT_COMP,
	BACKUP_RESULT_BREAK,
	BACKUP_RESULT_BUSY,
	BACKUP_RESULT_ERROR,
	
	BAKCUP_RESULT_MAX
};

BOOL backupInit( BOOL (* cardPulledOutCallBack)( void));
void backupTask( void);
BOOL backupIsSaveLoad( void);
BOOL backupSave( u32 dstAdress, u8 * srcBuf, u32 size, BOOL crcFlag);
BOOL backupLoad( u32 srcAdress, u8 * dstBuf, u32 size, BOOL crcFlag);
BOOL backupSaveAsync( u32 dstAdress, u8 * srcBuf, u32 size, BOOL crcFlag, void (* asyncCallBackFunc)( void *), void * asyncCallBackFuncArg);
BOOL backupLoadAsync( u32 srcAdress, u8 * dstBuf, u32 size, BOOL crcFlag, void (* asyncCallBackFunc)( void *), void * asyncCallBackFuncArg);
int backupGetResult( void);
u16 backupGetCrc( u8 * buf, int size);

#endif //__BACKUP_H__

