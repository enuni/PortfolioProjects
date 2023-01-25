// MASTER_SPARK を置換
// master_spark を置換
// MasterSpark を置換
// ts を置換
// effect を置換

#ifndef __MASTER_SPARK_H__
#define __MASTER_SPARK_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
extern MASTER * master;
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace master_spark {

enum STATE {
	STATE_NONE,
	STATE_FIRE,
	STATE_WAIT,
	STATE_FADE_OUT,
	
	STATE_MAX
};

enum FRAME_N {
	FRAME_N_FIRE = 10,
	FRAME_N_WAIT = 30,
	FRAME_N_FADE_OUT = 10,

	FRAME_N_MAX
};

} // namespece master_spark
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace effect {

typedef struct _master_spark_param {
	int w;
	int h;
	int x;
	int y;
	int angle;
	int blank;
	bool cameraFlag;

	void (* endCBFunc)(void *);
	void * endCBFuncArg;
} MASTER_SPARK_PARAM;

typedef struct _master_spark_game_membar {
	MASTER_SPARK_PARAM param;
	int dispX;
	int dispY;
	int stateFrameCnt;
} MASTER_SPARK_GAME_MEMBER;

typedef struct _master_spark_disp_membar {
	int objIdMasterSpark;
	int objIdMasterSparkBase;
} MASTER_SPARK_DISP_MEMBER;
	
class CMasterSpark : public TASK_SAVE {
	
public:
	CMasterSpark();
	CMasterSpark( TASK_SAVE * oya);
	CMasterSpark( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CMasterSpark();
	void constructCommon();					// メンバー0クリアなどnewしたらconstructorで必ずやる事
	void task();
	void disp();
	
	void setMember( void * owner);			// 自分を作ったTASKから必要なパラメータをもらう void *型は宣言から変えても良いし、関数内で型変換しても良い
	void setMemberLoad( void * owner);		// setMember()のロード時版、ロードして不要なものはセットしない
	void setMemberCommon( void * owner);	// setMember()とsetMemberLoad()の共通処理
	void start();							// setMember()後にやるTask立ち上げ処理
	void startLoad();						// start()のロード時版
	void startCommon();						// start()とstartLoad()の共通処理
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();


	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	static nn::ts::effect::MASTER_SPARK_DISP_MEMBER dm;
	MASTER_SPARK_GAME_MEMBER gm;
	
}; // CMasterSpark
	 
} // namespece effect

} // namespace ts
} // namespace nn
	

#endif // __MASTER_SPARK_H__
