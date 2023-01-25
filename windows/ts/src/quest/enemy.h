// ENEMY を置換
// enemy を置換
// Enemy を置換
// ts を置換
// quest を置換

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "game_char.h"
#include "master.h"
#include "direct.h"

#include "ts_common.h"

#include "nn_common.h"
#include "nn_common2.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
extern MASTER * master;
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace enemy {

enum STATE {
	STATE_NONE,
	STATE_IDLE,
	STATE_COLLISION,
	STATE_ATTACK_DIRECT,
	STATE_ATTACK,
	STATE_DIRECT,
	STATE_ESCAPE,
	
	STATE_MAX
};

enum ATTACK_CATEGORY {
	ATTACK_CATEGORY_EXPLOSION,
	ATTACK_CATEGORY_HOMING,
	ATTACK_CATEGORY_LASER_2,
	ATTACK_CATEGORY_MACHINGUN_DOWN,
	ATTACK_CATEGORY_LASER_4,
};

} // namespece enemy
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct _enemy_attack {
	int x;
	int y;
	int category;
	int firstTurn;
	int resetTurn;
} ENEMY_ATTACK;

typedef struct _enemy_status { 
	int hp;
	int gaugeCnt;
	int damageHp;
	bool hitSkipFlag;
	int attackTurns[8];
} ENEMY_STATUS;

typedef struct _enemy_param {
	nn::ts::common::TS_HIT hits[32];
	int hitN;
	int maxHP[8];
	int gaugeN;
	bool bossFlag;
	nn::ts::quest::ENEMY_ATTACK attacks[8];
	int attackN;
} ENEMY_PARAME;

typedef struct _enemy_game_member {
	int enemyID;
	nn::ts::quest::ENEMY_PARAME param;
	nn::ts::quest::ENEMY_STATUS status;
	int stateFrameCnt;
	unsigned int collsionCtrlCnt;
} ENEMY_GAME_MEMBER;

typedef struct _enemy_parts {
	int layoutIdTurns[8];
	int layoutIdTurnN;
	int layoutIdHits[4];
	int layoutIdHitN;
	int layoutIdWeakPoints[4];
	int layoutIdWeakPointN;
} ENEMY_PARTS;

typedef struct _enemy_disp_membar {
	int * objIdEnemys;
	int objIdDameNumber;
	nn::ts::quest::ENEMY_PARTS * partss;
	int animeIdEnemyIn;
	int animeIdBossExplosionSamll;
	int animeIdBossEscape;
} ENEMY_DISP_MEMBER;

typedef struct _enemy_link_membar {
	nn::ts::quest::CQuest * quest;
	int endAttackCBFuncId;
	void * endAttackCBFuncOwner;
	int endDirectCBFuncId;
	void * endDirectCBFuncOwner;
	
	nn::ts::quest::CDamageDisp * dd;
} ENEMY_LINK_MEMBER;
	
class CEnemy : public nn::common::CGameChar {
	
public:
	CEnemy();
	CEnemy( TASK_SAVE * oya);
	CEnemy( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CEnemy();
	void constructCommon();					// メンバー0クリアなどnewしたらconstructorで必ずやる事
	void task();
	void disp();
	
	void setMember( void * owner);			// 自分を作ったTASKから必要なパラメータをもらう void *型は宣言から変えても良いし、関数内で型変換しても良い
	void setMemberLoad( void * owner);		// setMember()のロード時版、ロードして不要なものはセットしない
	void setMemberCommon( void * owner);	// setMember()とsetMemberLoad()の共通処理
	void start();							// setMember()後にやるTask立ち上げ処理
	void startLoad();						// start()のロード時版
	void startCommon();						// start()とstartLoad()の共通処理
	void collision( int damage);
	void deleteDamageDisp();
	static void finDeleteDamgeDispCB( void * argEnemy);
	void finDeleteDamgeDisp();
	void shotEnd();
	void damage( int damage);
	int turn();
	static void attackCB( void * argEnemy);
	void attack();
	static void endAttackCB( void * argEnemy);
	void endAttack();
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	
	static void endDirectCB( void * argEnmey);
	void endDirect();
	void updateIfDead();

	static void nextDeadCB( void * argEnemy);
	void nextDead();

	void updateWeakPoint();

	void dump();

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::quest::ENEMY_GAME_MEMBER gm;
	static nn::ts::quest::ENEMY_DISP_MEMBER dm;
	nn::ts::quest::ENEMY_LINK_MEMBER lm;
	nn::dxlib::direct::CDispDirect * dd;
	
}; // CEnemy
	 
} // namespece quest

} // namespace ts
} // namespace nn
	

#endif // __ENEMY_H__
