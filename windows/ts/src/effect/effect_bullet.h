// EFFECT_BULLET を置換
// bullet を置換
// Bullet を置換
// ts を置換
// effect を置換

#ifndef __EFFECT_BULLET_H__
#define __EFFECT_BULLET_H__

#include "task_save.h"
#include "game_char.h"
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
namespace effect_bullet {

enum STATE {
	STATE_NONE,
	STATE_ACTIVE,
	STATE_FIRE,
	
	STATE_MAX
};

enum TYPE {
	TYPE_STRAIGHT,
	TYPE_HOMING,
	
	TYPE_MAX
};

} // namespece effect_bullet
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace effect {

typedef struct _bullet_param {
	int x;
	int y;
	int angle;
	nn::common::CGameChar * target;
	double speed;
	double angleSpeed;
	int type;
	int attribute;
	double speedAdd;
	double angleSpeedAdd;

	void (* moveCBFunc)( void *, void *, double, double, double, double);
	void * moveCBuncArg;
	void (* hitCBFunc)(void *, void *);
	void * hitCBFuncArg;
	void (* endCBFunc)(void *);
	void * endCBFuncArg;

	nn::common::def::graphics2d::RECT liveRect;
} BULLET_PARAM;

typedef struct _bullet_game_membar {
	nn::ts::effect::BULLET_PARAM param;
	double x;
	double y;
	double angle;
	double speed;
	double angleSpeed;
} BULLET_GAME_MEMBER;

typedef struct _bullet_disp_membar {
	int objIdHoming[5];
	int objIdNormal[5];
} BULLET_DISP_MEMBER;
	
class CBullet : public TASK_SAVE {
	
public:
	CBullet();
	CBullet( TASK_SAVE * oya);
	CBullet( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CBullet();
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

	nn::ts::effect::BULLET_GAME_MEMBER gm;
	static nn::ts::effect::BULLET_DISP_MEMBER dm;
	
}; // CBullet
	 
} // namespece effect

} // namespace ts
} // namespace nn
	

#endif // __EFFECT_BULLET_H__
