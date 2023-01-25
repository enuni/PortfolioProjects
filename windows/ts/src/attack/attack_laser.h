// LASER を置換
// laser を置換
// Laser を置換
// ts を置換
// attack を置換

#ifndef __ATTACK_LASER_H__
#define __ATTACK_LASER_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace attack_laser {

enum STATE {
	STATE_NONE,
	STATE_ACTIVE,
	
	STATE_MAX
};

enum LASER_TYPE {
	LASER_TYPE_MASTER_SPARK,
	LASER_TYPE_REFLECTION,

	LASER_TYPE_MAX
};

} // namespece attack_laser
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace attack {

typedef struct _laser_param {
	int x;
	int y;
	int angle;
	int width;
	int damage;
	nn::ts::quest::CQuest * quest;
	int type;
	int reflectionLaserN;				// 設定した本数のレーザーが出る反射回数は設定値 -1
	int attribute;
	unsigned int hitTargetBit;

	void (* endCBFunc)(void *);
	void * endCBFuncArg;
} LASER_PARAM;

typedef struct _laser_game_membar {
	LASER_PARAM param;
	int reflectionCnt;
	int reflectionX;
	int reflectionY;
	int reflectionAngle;
	
	int x;
	int y;
	int angle;
	int len;
	int laserCnt;
} LASER_GAME_MEMBER;
	
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
	
	static void damgeCB( void * argLaser);
	void damage();
	static void endEffectLaserCB( void * argLaser);
	void endEffectLaser();
	static void landingCB( void * argLaser);
	void landing();
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	void nextReflection( int x, int y, int angle);

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;
	
	MASTER * master;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::attack::LASER_GAME_MEMBER gm;
	
}; // CLaser
	 
} // namespece attack

} // namespace ts
} // namespace nn
	

#endif // __ATTACK_LASER_H__
