// ATTACK_EXPLOSION を置換
// explosion を置換
// Explosion を置換
// ts を置換
// attack を置換

#ifndef __ATTACK_EXPLOSION_H__
#define __ATTACK_EXPLOSION_H__

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
namespace explosion {

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

} // namespece explosion
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace attack {

typedef struct _explosion_param {
	int x;
	int y;
	int r;
	int damage;
	unsigned int hitTargetBit;
	nn::ts::quest::CBattle * battle;
	nn::ts::quest::CQuest * quest;
	nn::ts::quest::CBall * ball;

	void (* endCBFunc)(void *);
	void * endCBFuncArg;
} EXPLOSION_PARAM;

typedef struct _explosion_game_membar {
	EXPLOSION_PARAM param;
} EXPLOSION_GAME_MEMBER;
	
class CExplosion : public TASK_SAVE {
	
public:
	CExplosion();
	CExplosion( TASK_SAVE * oya);
	CExplosion( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CExplosion();
	void constructCommon();					// メンバー0クリアなどnewしたらconstructorで必ずやる事
	void task();
	void disp();
	
	void setMember( void * owner);			// 自分を作ったTASKから必要なパラメータをもらう void *型は宣言から変えても良いし、関数内で型変換しても良い
	void setMemberLoad( void * owner);		// setMember()のロード時版、ロードして不要なものはセットしない
	void setMemberCommon( void * owner);	// setMember()とsetMemberLoad()の共通処理
	void start();							// setMember()後にやるTask立ち上げ処理
	void startLoad();						// start()のロード時版
	void startCommon();						// start()とstartLoad()の共通処理
	static void endEffectExplosionCB( void * argLaser);
	void endEffectExplosion();
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();

	static void landingCB( void * argExplosion);
	void landing();

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;
	
	MASTER * master;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::attack::EXPLOSION_GAME_MEMBER gm;
	
}; // CExplosion
	 
} // namespece attack

} // namespace ts
} // namespace nn
	

#endif // __ATTACK_EXPLOSION_H__
