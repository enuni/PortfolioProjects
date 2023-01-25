
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "task.h"
#include "type.h"

//------------------------------------------------------------------------------

enum {
	ANIMATION_FLAGS_LOOP = 0x01,		//最後のフレームの次は0フレームに戻る
	ANIMATION_FLAGS_LAST_WAIT = 0x02,	//最後のフレームで待ち状態になる　最後のフレームの次のフレームでendCallBackFuncを呼ぶ　その後にendしてもendCallBackFuncは呼ばれない
	
	ANIMATION_FLAGS_MAX
};

enum {
	ANIMATION_LAYER_TYPE_OBJ_MAIN,
	ANIMATION_LAYER_TYPE_OBJ_SUB,
	ANIMATION_LAYER_TYPE_OBJ_SE,
	ANIMATION_LAYER_TYPE_ANIMATION,
	
	ANIMATION_LAYER_TYPE_OBJ_MAX
};

enum {
	ANIMATION_CHANGE_OBJ_ID_TYPE_LAYER,
	ANIMATION_CHANGE_OBJ_ID_TYPE_OBJ_ID,

	ANIMATION_CHANGE_OBJ_ID_TYPE_MAX
};

//AnimationConverter.rb　の　-cオプションで自動生成されるファイル
#include "animation_class.h"
#ifndef __ANIMATION_CLASS_H__
//animation_class.h　が用意できなかった時用　メンバーが変更されている事があるので注意する

typedef struct _animation_data {
	short dataOffset;
	short layerN;
} ANIMATION_DATA;

typedef struct _layer_data {
	short objId;
	short mainFlag;
	short keyFrameN;
} LAYER_DATA;

typedef struct _key_frame_data {
	short startFrame;
	short enableFrame;
	short x;
	short y;
	short tweenFlag;
	short acc;
} KEY_FRAME_DATA;

typedef struct _animation_arc_header {
	short dataOffset;
} ANIMATION_ARC_HEADER;

typedef struct _animation_arc_info {
	char * arcFileName;
	short animationOffset;
	short animationN;
} ANIMATION_ARC_INFO;

#endif //__ANIMATION_CLASS_H__

typedef struct _animation_disp_data {
	int x;
	int y;
	int wr;			//横率10000倍
	int hr;			//縦率10000倍
	int z;
	int angle;
	BOOL flipH;
	BOOL flipV;
	int r;
	int g;
	int b;
	int a;
} ANIMATION_DISP_DATA;

typedef struct _animation_arc_ctrl {
	BOOL loadFlag;
	u8 * buf;
} ANIMATION_ARC_CTRL;

typedef struct _animation_change_obj_id_data {
	int type;
	int dst;
	int src;
} ANIMATION_CHAGE_OBJ_ID_DATA;

typedef struct _animation {
	
	TASK * taskThis;
	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	void (* endCallBackFunc)( void *);	//デストラクタより1F早く呼ぶ
	void * endCallBackFuncArg;	//アニメが通常通り終了した場合何も表示しなかったフレーム
	//の最後でdestructorのコールバックを呼ぶが
	//何も表示しなかった瞬間に呼ぶ(通常priority = 64のタスク内)のでシームレスに次の表示が可能になる
	
	char fileName[256];
	int animationId;
	u8 * buf;
	int * keyFrameCnt;
	int frameCnt;
	
	int x;
	int y;
	int z;
	int alpha;
	
	/*
	//ANIMATION_CHAGE_OBJ_ID_DATA changeObjIdArray[]に変更
	int changeObjIdTypeArray[16];	//0なし　1置き換え　2シフト
	int changeObjIdLayerNoArray[16];	//操作レイヤー番号
	int changeObjIdNArray[16];	//操作値*/

	ANIMATION_CHAGE_OBJ_ID_DATA changeObjIdArray[32];
	int changeObjIdN;
	
	int changeObjIdAInATypeArray[16];	//0なし　1置き換え　2シフト
	int changeObjIdAInALayerNoArray[16];	//操作レイヤー番号
	int changeObjIdAInANArray[16];	//操作値
	
	BOOL loopFlag;
	BOOL waitFlag;
	BOOL lastWaitFlag;
	float speedRate;
	
	int vec;
	BOOL lastWaitCallBackLateFlag;
	BOOL lastWaitExecFlag;
	
	BOOL frameWaitFlag;
	int waitFrame;
	void (* frameWaitCallBackFunc)( void *);
	void * frameWaitCallBackFuncArg;
	
	BOOL hideFlag;
	
} ANIMATION;

//------------------------------------------------------------------------------

//初期化
void animationArcInit( void * arcInfo, int arcN);
//終了処理
void animationArcEnd( void);
//読み込み
BOOL animationLoadArc( int animationArcId);
//解放
BOOL animationReleaseArc( int animationArcId);

//起動したアニメの制御
void animationSetSpeedRate( ANIMATION * animationArg, float rate);
void animationEnd( ANIMATION * animationArg);
void animationWait( ANIMATION * animationArg);
void animationMove( ANIMATION * animationArg);
void animationBack( ANIMATION * animationArg);
void animationHide( ANIMATION * animationArg);
void animationVisible( ANIMATION * animationArg);
void animationFrameWait( ANIMATION * animationArg, int waitFrame, void (* frameWaitCallBackFunc)( void *), void * frameWaitCallBackFuncArg);
void animationChangeObjId( ANIMATION * animationArg, int type, int dst, int src);

//起動
TASK * animationExec( TASK * oya, int animationId, int x, int y, int z, int flags, void (* endCallBackFunc)( void *), void * endCallBackFuncArg);
//起動　低レベル　animationId指定
TASK * animationExecArc( TASK * oya, ANIMATION * animationArg);
//起動　低レベル　binファイル名直接指定
TASK * animationExecBinFile( TASK * oya, ANIMATION * animationArg);
//起動　低レベル
TASK * animationCreateTaskMain( TASK * oya, ANIMATION * animationArg);

//指定のレイヤーのフレーム目の表示情報をanimationDispDataに入れる
void animationGetDispData( ANIMATION_DISP_DATA * animationDispData, int animationId, ANIMATION_DISP_DATA * animationDispDataMaster, int layerNo, int frameCnt);

//指定のフレーム目のみ描画
void animationDisp( int animationId, ANIMATION_DISP_DATA * animationDispDataMaster, int frameCnt, BOOL stopFlag);

//パラメータ取得
s16 animationGetFrame( int animationId);
s16 animationGetLayerN( int animationId);
u8 * animationGetData( int animationId);

#endif //__ANIMATION_H__

