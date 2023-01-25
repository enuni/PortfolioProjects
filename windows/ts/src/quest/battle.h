// BATTLE ��u��
// battle ��u��
// Battle ��u��
// ts ��u��
// quest ��u��

#ifndef __BATTLE_H__
#define __BATTLE_H__

#include "task_save.h"
#include "master.h"
#include "nn_common2.h"
//---------------------------------------------------------------------------
// �O���錾
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
	void constructCommon();					// �����o�[0�N���A�Ȃ�new������constructor�ŕK����鎖
	void task();
	void disp();
	
	void setMember( void * owner);			// �����������TASK����K�v�ȃp�����[�^�����炤 void *�^�͐錾����ς��Ă��ǂ����A�֐����Ō^�ϊ����Ă��ǂ�
	void setMemberLoad( void * owner);		// setMember()�̃��[�h���ŁA���[�h���ĕs�v�Ȃ��̂̓Z�b�g���Ȃ�
	void setMemberCommon( void * owner);	// setMember()��setMemberLoad()�̋��ʏ���
	void start();							// setMember()��ɂ��Task�����グ����
	void startLoad();						// start()�̃��[�h����
	void startCommon();						// start()��startLoad()�̋��ʏ���
	
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
