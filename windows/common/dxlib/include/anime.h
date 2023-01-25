
#ifndef __ANIME_H__
#define __ANIME_H__

#include "task_save.h"
#include "type.h"

//------------------------------------------------------------------------------

enum {
	ANIME_FLAGS_LOOP = 0x01,		//最後のフレームの次は0フレームに戻る
	ANIME_FLAGS_LAST_WAIT = 0x02,	//最後のフレームで待ち状態になる　最後のフレームの次のフレームでendCallBackFuncを呼ぶ　その後にendしてもendCallBackFuncは呼ばれない
	
	ANIME_FLAGS_MAX
};

enum {
	ANIME_LAYER_TYPE_OBJ_MAIN,
	ANIME_LAYER_TYPE_OBJ_SUB,
	ANIME_LAYER_TYPE_OBJ_SE,
	ANIME_LAYER_TYPE_ANIME,
	
	ANIME_LAYER_TYPE_OBJ_MAX
};

enum {
	ANIME_CHANGE_OBJ_ID_TYPE_LAYER,
	ANIME_CHANGE_OBJ_ID_TYPE_OBJ_ID,

	ANIME_CHANGE_OBJ_ID_TYPE_MAX
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

namespace nn {
namespace common {
namespace dxlib {

typedef struct _anime_disp_data {
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
} ANIME_DISP_DATA;

typedef struct _anime_arc_ctrl {
	BOOL loadFlag;
	u8 * buf;
} ANIME_ARC_CTRL;

typedef struct _anime_change_obj_id_data {
	int type;
	int dst;
	int src;
} ANIME_CHAGE_OBJ_ID_DATA;

class CAnime : public TASK_SAVE {
	
public:
	CAnime();
	CAnime( TASK_SAVE * oya);
	~CAnime();
	void task();
	void disp();
	void constructorCommon();

	static void arcInit( void * arcInfo, int arcN);		// 初期化
	static void arcEnd( void);							// 終了処理
	static BOOL loadArc( int animeArcId);				// 読み込み
	static BOOL releaseArc( int animeArcId);			// 解放
	static void animeDisp( int animeId, nn::common::dxlib::ANIME_DISP_DATA * animeDispDataMaster, int frameCnt, BOOL stopFlag);												// 指定のフレーム目の表示するだけ
	static void getDispData( nn::common::dxlib::ANIME_DISP_DATA * animeDispData, int animeId, nn::common::dxlib::ANIME_DISP_DATA * animeDispDataMaster, int layerNo, int frameCnt);	// 指定のレイヤーのフレーム目の表示情報をanimeDispDataに入れる
	
	static CAnime * exec( TASK_SAVE * oya, int animeId, int x, int y, int z, int flags, void (* endCallBackFunc)( void *), void * endCallBackFuncArg);	// 起動
	static CAnime * execArc( int animeId, CAnime * animeArg);																							// 起動　低レベル　animeId指定
	static CAnime * execBinFile( char * fileName, CAnime * animeArg);																					// 起動　低レベル　binファイル名直接指定
	
	//起動したアニメの制御
	void setSpeedRate( float rate);
	void end();
	void wait();
	void move();
	void back();
	void hide();
	void visible();
	void frameWait( int waitFrame, void (* frameWaitCallBackFunc)( void *), void * frameWaitCallBackFuncArg);
	void changeObjId( int type, int dst, int src);
	
	void disp( int animeId, nn::common::dxlib::ANIME_DISP_DATA * animeDispDataMaster, int frameCnt, BOOL stopFlag);	// 指定のフレーム目のみ描画

	//パラメータ取得
	static s16 getFrame( int animeId);
	static s16 getLayerN( int animeId);
	static u8 * getData( int animeId);
	
private:
	static void dump_anime_arc( int animeArcId);
	static s16 get_frame_n( u8 * animeBuf);
	static s16 get_layer_n( u8 * animeBuf);
	static int get_arc_offset( u8 * arcBuf, int arcId, int animeId);
	static int get_anime_arc_id( int animeId);
	static u8 * get_anime_data( int animeId);
	static u8 * get_layer_data( int animeId, int layerNo);
	static BOOL get_disp_data( nn::common::dxlib::ANIME_DISP_DATA * animeDispData, nn::common::dxlib::ANIME_DISP_DATA * animeDispDataMaster, int animeId, int layerNo, int frameCnt, int objW, int objH, float speedRate);
	void disp_anime( int animeId, ANIMATION_DATA * animeData, int frameCnt, int stepCnt, nn::common::dxlib::ANIME_DISP_DATA * animeDispDataMaster, BOOL stopFlag);
	
public:
	static void * sArcInfo;
	static int sArcN;
	static ANIME_ARC_CTRL * sArcCtrl;
	
	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	void (* endCallBackFunc)( void *);	// デストラクタより1F早く呼ぶ
	void * endCallBackFuncArg;			// アニメが通常通り終了した場合何も表示しなかったフレーム
										// の最後でdestructorのコールバックを呼ぶが
										// 何も表示しなかった瞬間に呼ぶ(通常priority = 64のタスク内)のでシームレスに次の表示が可能になる
	
	int animeId;
	u8 * buf;
	int * keyFrameCnt;
	int frameCnt;
	
	int x;
	int y;
	int z;
	int alpha;

	ANIME_CHAGE_OBJ_ID_DATA changeObjIdArray[32];
	int changeObjIdN;

	ANIME_CHAGE_OBJ_ID_DATA changeObjIdAinAArray[32];	// アニメ内アニメ用
	int changeObjIdAinAN;								// アニメ内アニメ用
	
	BOOL loopFlag;
	BOOL waitFlag;
	BOOL lastWaitFlag;
	float speedRate;
	
	int vec;
	BOOL lastWaitCallBackLateFlag;	// lastWaitFlag = TRUE時、endCallBackFuncの呼び出しを1フレーム遅らせるための内部制御フラグ
#if 0
	// たぶん、いらない
	BOOL lastWaitExecFlag;			// lastWaitFlag = TRUE時、endCallBackFuncが呼び出し済みならTRUE
#endif
	
	BOOL frameWaitFlag;							// あるフレームに来たら、自動停止するようにするフラグ
	int waitFrame;								// あるフレームに来たら、自動停止するフレーム数
	void (* frameWaitCallBackFunc)( void *);	// あるフレームに来た時の自動停止で自動停止した時のコールバック関数
	void * frameWaitCallBackFuncArg;			// あるフレームに来た時の自動停止で自動停止した時のコールバック関数の引数
	
	BOOL hideFlag;								// アニメの表示だけをしないフラグ
};

} // namespcae dxlib
} // namespace common
} // namespace nn

//------------------------------------------------------------------------------


#endif //__ANIME_H__

