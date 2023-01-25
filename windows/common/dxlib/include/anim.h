// 第3世代アニメライブラリ disp.h .cpp 使用版

#ifndef __ANIM_H__
#define __ANIM_H__

#include "task_save.h"
#include "type.h"

//------------------------------------------------------------------------------

enum {
	ANIM_FLAGS_LOOP = 0x01,		//最後のフレームの次は0フレームに戻る
	ANIM_FLAGS_LAST_WAIT = 0x02,	//最後のフレームで待ち状態になる　最後のフレームの次のフレームでendCallBackFuncを呼ぶ　その後にendしてもendCallBackFuncは呼ばれない
	ANIM_FLAGS_CAMERA = 0x04,	// dispのcameraFlag default=false
	
	ANIM_FLAGS_MAX
};

enum {
	ANIM_LAYER_TYPE_OBJ_MAIN,
	ANIM_LAYER_TYPE_OBJ_SUB,
	ANIM_LAYER_TYPE_OBJ_SE,
	ANIM_LAYER_TYPE_ANIM,
	
	ANIM_LAYER_TYPE_OBJ_MAX
};

enum {
	ANIM_CHANGE_OBJ_ID_TYPE_LAYER,
	ANIM_CHANGE_OBJ_ID_TYPE_OBJ_ID,

	ANIM_CHANGE_OBJ_ID_TYPE_MAX
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
namespace dxlib {

typedef struct _anim_disp_data {
	int objId;
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
	int blendMode;
} ANIM_DISP_DATA;

typedef struct _anim_arc_ctrl {
	BOOL loadFlag;
	u8 * buf;
} ANIM_ARC_CTRL;

typedef struct _anim_change_obj_id_data {
	int type;
	int dst;
	int src;
} ANIM_CHAGE_OBJ_ID_DATA;

class CAnim : public TASK_SAVE {
	
public:
	CAnim();
	CAnim( TASK_SAVE * oya);								// TASKとして実行したかったらNULLでもこっちでnew
	CAnim( TASK_SAVE * oya, nn_common::SaveManager * m);
	~CAnim();
	void task();
	//void disp();
	void constructorCommon();

	static void arcInit( void * arcInfo, int arcN);		// 初期化
	static void arcEnd( void);							// 終了処理
	static BOOL loadArc( int animArcId);				// 読み込み
	static BOOL releaseArc( int animArcId);				// 解放
	static void animDispSimpleShift( int animId, int layoutId, int frameCnt,
		BOOL stopFlag);		// 指定のフレーム目の表示するだけ stopFlag == TRUE でSE無効
	static void animDisp( int animId, nn::dxlib::ANIM_DISP_DATA * animDispDataMaster,
		int frameCnt, BOOL stopFlag);	// 指定のフレーム目の表示するだけ stopFlag == TRUE でSE無効
	static BOOL getDispData( nn::dxlib::ANIM_DISP_DATA * animDispData, int animId,
		nn::dxlib::ANIM_DISP_DATA * animDispDataMaster,int layerNo,
		int frameCnt);	// 指定のレイヤーのフレーム目の表示情報をanimDispDataに入れる
	
	static CAnim * exec( TASK_SAVE * oya, int animId, int x, int y, int z, int flags,
		void (* endCallBackFunc)( void *), void * endCallBackFuncArg);	// 起動
	static CAnim * execArc( int animId, CAnim * animArg);				// 起動　低レベル　animId指定
	static CAnim * execBinFile( char * fileName, CAnim * animArg);		// 起動　低レベル　binファイル名直接指定
	static CAnim * execSimple( TASK_SAVE * oya, int animId, int layoutId, int flags,
		void (* endCallBackFunc)( void *), void * endCallBackFuncArg);	// 起動 x y z alpha をlayoutIdから持ってくる
	static CAnim * execAlpha( TASK_SAVE * oya, int animId, int x, int y, int z, int alpha, int flags,
		void (* endCallBackFunc)( void *), void * endCallBackFuncArg);	// 起動 α値指定版
	static CAnim * execSave( TASK_SAVE * oya, nn_common::SaveManager * saveManager,
		int animId, int x, int y, int z, int alpha, int flags,
		void (* endCallBackFunc)( void *), void * endCallBackFuncArg);	// 起動 α値指定版

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
	
	//void disp( int animId, nn::dxlib::ANIM_DISP_DATA * animDispDataMaster, int frameCnt, BOOL stopFlag);	// 指定のフレーム目のみ描画

	//パラメータ取得
	static s16 getFrame( int animId);
	static s16 getLayerN( int animId);
	static u8 * getData( int animId);

	static int utilCalcMove( int start, int end, int acc, int frame, int now);		// acc=0最大加速 acc=33等速 acc=66最大減速
	static float utilCalcMove( float start, float end, int acc, int frame, int now);
	
private:
	static void dump_anim_arc( int animArcId);
	static s16 get_frame_n( u8 * animBuf);
	static s16 get_layer_n( u8 * animBuf);
	static int get_arc_offset( u8 * arcBuf, int arcId, int animId);
	static int get_anim_arc_id( int animId);
	static u8 * get_anim_data( int animId);
	static u8 * get_layer_data( int animId, int layerNo);
	static BOOL get_disp_data( nn::dxlib::ANIM_DISP_DATA * animDispData,
		nn::dxlib::ANIM_DISP_DATA * animDispDataMaster, int animId, int layerNo, int frameCnt,
		int objW, int objH, float speedRate);
	void disp_anim( int animId, ANIMATION_DATA * animData, int frameCnt, int stepCnt,
		nn::dxlib::ANIM_DISP_DATA * animDispDataMaster, BOOL stopFlag);
	
public:
	static void * sArcInfo;
	static int sArcN;
	static ANIM_ARC_CTRL * sArcCtrl;
	
	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	void (* endCallBackFunc)( void *);	// デストラクタより1F早く呼ぶ
	void * endCallBackFuncArg;			// アニメが通常通り終了した場合何も表示しなかったフレーム
										// の最後でdestructorのコールバックを呼ぶが
										// 何も表示しなかった瞬間に呼ぶ(通常priority = 64のタスク内)のでシームレスに次の表示が可能になる
	
	// タスクセーブ対応
	virtual void loadPostProc();
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();

	int animId;
	u8 * buf;
	int * keyFrameCnt;
	int frameCnt;
	
	int x;
	int y;
	int z;
	int alpha;

	ANIM_CHAGE_OBJ_ID_DATA changeObjIdArray[32];
	int changeObjIdN;

	ANIM_CHAGE_OBJ_ID_DATA changeObjIdAinAArray[32];	// アニメ内アニメ用
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
	bool cameraFlag;
	
	// タスクセーブ対応
	static nn_common::DATARECODE dataRecode[];
};

} // namespcae dxlib
} // namespace nn

//------------------------------------------------------------------------------


#endif //__ANIM_H__

