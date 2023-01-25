// BATTLE を置換
// battle を置換
// Battle を置換
// ts を置換
// quest を置換

#ifndef __BATTLE_H__
#define __BATTLE_H__

#include "task_save.h"
#include "master.h"
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
namespace battle {

enum STATE {
	STATE_NONE,
	STATE_PULL_START,
	STATE_PULL,
	STATE_SHOT,
	STATE_ENEMY,
	STATE_GAMEOVER,
	
	STATE_MAX
};

} // namespece battle
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct _battle_game_member {
	nn::ts::quest::CQuest * quest;
	
	int x;
	int y;
	int w;
	int h;

	nn::ts::quest::CEnemy * enemys[32];
	int enemyN;

	int enemyTurnCnt;
	int enmeyAttackCnt;

	nn::common::util::type::t2d::POINT_S32 itemPoints[16];
	int itemPointN;
	int itemN;
} BATTLE_GAME_MEMBER;

typedef struct _battle_link_member {
	nn::ts::common::CControl * ctrl;
	nn::ts::quest::CPull * pull;
	nn::ts::quest::CBattleItem * items[8];
} BATTLE_LINK_MEMBER;
	
class CBattle : public TASK_SAVE {
	
public:
	CBattle();
	CBattle( TASK_SAVE * oya);
	CBattle( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CBattle();
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

	static void pullDecideCB( void * argBattle, void * argPull);
	void pullDecide( void * argPull);

	static void shotEndCB( void * argBattle);
	void shotEnd();

	void startPull();

	void addFieldCollision( void * argCollisions);
	void addEnemyCollision( void * argCollisions);
	void addItemCollision( void * argCollisions);

	static void nextTurnEnemyCB( void * argBattle);
	void nextTurnEnemy();
	void startAttackEnemy();
	static void endAttackEnemyCB( void * argBattle);
	void endAttackEnemy();
	static void nextTurnCB( void * argBattle);
	void nextTurn();
	void updateBallMoveFrameEnd();
	bool isEnemyAnnihilation();
	void setState( int state);
	void updateWeakPoint();
	void dump();
	void shot( int angle);
	void checkNextBattle();
	void createItemPhase();
	void excludeItem( nn::ts::quest::CBattleItem * item);
	void updateTurnItemPhase();

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::quest::BATTLE_GAME_MEMBER gm;
	nn::ts::quest::BATTLE_LINK_MEMBER lm;
	
}; // CBattle
	 
} // namespece quest

} // namespace ts
} // namespace nn
	

#endif // __BATTLE_H__
