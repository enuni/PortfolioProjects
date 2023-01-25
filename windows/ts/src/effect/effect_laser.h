// LASER を置換
// laser を置換
// Laser を置換
// ts を置換
// effect を置換

#ifndef __EFFECT_LASER_H__
#define __EFFECT_LASER_H__

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
namespace effect_laser {

enum STATE {
	STATE_NONE,
	STATE_FIRE,
	STATE_WAIT,
	STATE_FADE_OUT,
	
	STATE_MAX
};
enum FRAME_N {
	FRAME_N_WAIT = 30,
	FRAME_N_FADE_OUT = 10,

	FRAME_N_MAX
};

} // namespece effect_laser
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace effect {

typedef struct _laser_param {
	int w;
	int h;
	int x;
	int y;
	int angle;
	int blank;
	bool cameraFlag;
	int speed;
	int atribute;

	void (* landingCBFunc)(void *);
	void * landingCBFuncArg;
	void (* endCBFunc)(void *);
	void * endCBFuncArg;
} LASER_PARAM;

typedef struct _laser_game_membar {
	nn::ts::effect::LASER_PARAM param;
	int dispW;
	int dispX;
	int dispY;
	int stateFrameCnt;
} LASER_GAME_MEMBER;

typedef struct _laser_disp_membar {
	int objIdLaser[5];
} LASER_DISP_MEMBER;
	
class CLaser : public TASK_SAVE {
	
public:
	CLaser();
	CLaser( TASK_SAVE * oya);
	CLaser( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CLaser();
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

	nn::ts::effect::LASER_GAME_MEMBER gm;
	static nn::ts::effect::LASER_DISP_MEMBER dm;
	
}; // CLaser
	 
} // namespece effect

} // namespace ts
} // namespace nn
	

#endif // __EFFECT_LASER_H__
