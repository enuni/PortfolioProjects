// PULL を置換
// pull を置換
// Pull を置換
// ts を置換
// quest を置換

#ifndef __PULL_H__
#define __PULL_H__

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
namespace pull {

enum STATE {
	STATE_NONE,
	STATE_IDLE,
	STATE_PULL,
	
	STATE_MAX
};

} // namespece pull
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct _pull_game_member {
	int decideAngle;
	double nowAngle;
	double nowPullLength;
	int dispX;
	int dispY;
} PULL_GAME_MEMBER;

typedef struct _pull_disp_membar {
	int objIdArrorw;
} PULL_DISP_MEMBER;

typedef struct _pull_link_member {
	int decideCBFuncId;
	void * decideCBFuncOwner;
} PULL_LINK_MEMBER;

class CPull : public TASK_SAVE {
	
public:
	CPull();
	CPull( TASK_SAVE * oya);
	CPull( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CPull();
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

	void taskIdle();
	void taskPull();

	void dump();

	void decide( int angle);

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;


	static nn_common::DATARECODE dataRecode[];

	nn::ts::quest::PULL_GAME_MEMBER gm;
	static nn::ts::quest::PULL_DISP_MEMBER dm;
	nn::ts::quest::PULL_LINK_MEMBER lm;
	
}; // CPull
	 
} // namespece quest

} // namespace ts
} // namespace nn
	

#endif // __PULL_H__
