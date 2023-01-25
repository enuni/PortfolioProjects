// DAMAGE_DISP を置換
// damage_disp を置換
// DamageDisp を置換
// ts を置換
// quest を置換

#ifndef __DAMAGE_DISP_H__
#define __DAMAGE_DISP_H__

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
namespace damage_disp {

enum STATE {
	STATE_NONE,
	STATE_ACTIVE,
	STATE_DISP,
	
	STATE_MAX
};

} // namespece damage_disp
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct _damage_disp_disp_membar {
	int objIdNumberTop;
} DAMGE_DISP_DISP_MEMBER;

typedef struct _damage_disp_param {
	int moveFrameN;
	int exFrameN;
	int startX;
	int startY;
	int endX;
	int endY;
	int z;
	int blank;
	double startExRate;
	double endExRate;
	int dmageFrameN;
	int dispFrameN;

	GXRgb rgb;
	int alpha;
	int blend;
	
	void (* deleteCBFunc)(void * deleteCBFuncArg);
	void * deleteCBFuncArg;
} DAMAGE_DISP_PARAM;

typedef struct _damage_disp_game_member {
	int moveFrameCnt;
	int exFrameCnt;
	int dispDamge;
	int damage;
	int addDamage;
	int dispFrameCnt;
	bool deleteFlag;

	nn::ts::quest::DAMAGE_DISP_PARAM param;
} DAMAGE_DISP_GAME_MEMBER;
	
class CDamageDisp : public TASK_SAVE {
	
public:
	CDamageDisp();
	CDamageDisp( TASK_SAVE * oya);
	CDamageDisp( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CDamageDisp();
	void constructCommon();					// メンバー0クリアなどnewしたらconstructorで必ずやる事
	void task();
	void disp();
	
	void setMember( void * owner);			// 自分を作ったTASKから必要なパラメータをもらう void *型は宣言から変えても良いし、関数内で型変換しても良い
	void setMemberLoad( void * owner);		// setMember()のロード時版、ロードして不要なものはセットしない
	void setMemberCommon( void * owner);	// setMember()とsetMemberLoad()の共通処理
	void start();							// setMember()後にやるTask立ち上げ処理
	void startLoad();						// start()のロード時版
	void startCommon();						// start()とstartLoad()の共通処理
	void damage( int damage);
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();


	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	static nn::ts::quest::DAMGE_DISP_DISP_MEMBER dm;
	nn::ts::quest::DAMAGE_DISP_GAME_MEMBER gm;
	
}; // CDamageDisp
	 
} // namespece quest

} // namespace ts
} // namespace nn
	

#endif // __DAMAGE_DISP_H__
