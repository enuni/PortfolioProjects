#ifndef __SLEEP_H__
#define __SLEEP_H__

#include "type.h"

//初期化
void sleepInit( void);
//毎フレーム呼び出す
void sleepTask( void);
//スリープに入る前のコールバックを登録する
void sleepAddSleeStartCallBack( void (* func)( void *), void * arg);
//スリープから復帰したコールバック関数を登録する
void sleepAddSleepEndCallBack( void (* func)( void *), void * arg);
//起動 スリープ LCD-OFFから復帰時 マイクをオンにした時間をセット
//（スリープ LCD-OFFから復帰時はコールバック SleepTask()から呼ばれる）
void sleepSetSleepEndMilliSec( void);
//起動 スリープ LCD-OFFから復帰時 マイクをオンにしてから経過したミリ秒を返す
u64 sleepGetSleepEndMilliSec( void);
//スリープに入るかどうかのフラグをセットする TRUE = 入らない FALSE = 入る
void sleepSetSleepWaitFlag( BOOL flag);
//スリープモードが通信中 LCDOFF状態かどうか
BOOL sleepIsModeNet( void);

void sleepBackLightOffAsync( void);
void sleepBackLightOnAsync( void);

#endif //__SLEEP_H__

