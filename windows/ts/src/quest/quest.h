// QUEST ��u��
// quest ��u��
// Quest ��u��
// ts ��u��
// quest ��u��

#ifndef __QUEST_H__
#define __QUEST_H__

#include <boost/property_tree/ptree.hpp>

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// �O���錾
#include "class_list.h"
extern MASTER * master;
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace quest {

enum STATE {
	STATE_NONE,
	STATE_MOVE,
	STATE_BATTLE,
	
	STATE_MAX
};

enum MOVE_STATE {
	MOVE_IN_BALL_0,
	MOVE_IN_BALL_1,
	MOVE_IN_BALL_2,
	MOVE_IN_BALL_3,
	MOVE_TALK,
	MOVE_STATE_BG,
	MOVE_STATE_IN_ENEMY,
	MOVE_STATE_IN_FRAME,
	MOVE_STATE_TEXT,
	MOVE_STATE_TUTORIAL,

	MOVE_STATE_MAX
};

} // namespece quest
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct _quest_status {
	int turn;
	int hp;
	int damageHp;
	int maxHp;
	int battleCnt;
} QUEST_STATUS;

typedef struct {
	nn::ts::quest::QUEST_STATUS status;
} QUEST_GAME_MEMBER;

typedef struct {
	int layoutIdCharUpTop;
	int layoutIdCharUpOffset;
	int * objIdCharUps;
	int layoutIdField;
	int layoutIdHpBarBoss;
	int layoutIdHpBar;

	int layoutIdHpBarBosFrame;
	int layoutIdHpBarFrame;
	int layoutIdStage;
	int layoutIdStageNum;
	int layoutIdBoss;
	int layoutIdHp;
	int layoutIdFieldBg;
	int layoutIdFieldOuts[4];
	
	int * objIdCharFulls;

	int animeIdBattleFrameIn;
	int animeIdStageStart;
	int animeIdWin;
} QUEST_DISP_MEMBER;

typedef struct {
	int moveState;
} QUEST_CTRL_MEMBER;

typedef struct _quest_link_member {
	nn::dxlib::CAnim * animeFrame;
	nn::ts::quest::CBattle * battle;
	nn::ts::quest::CBall * balls[4];
	nn::ts::quest::CEnemy * boss;
	nn::ts::quest::CBattleBg * battleBg;
	nn::ts::quest::CQuestControl * questCtrl;
	nn::ts::common::CControl * ctrl;
} QUEST_LINK_MEMBER;
	
class CQuest : public TASK_SAVE {
	
public:
	CQuest();
	CQuest( TASK_SAVE * oya);
	CQuest( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CQuest();
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

	void addBallCollision( void * argCollisions);
	void deleteDamageDispBall();
	void getNearBallPos( double * nearX, double * nearY,
		double x, double y);
	
	static void nextMoveCB( void * argQuest);
	void nextMove();
	void taskMove();
	void setupBg();
	void nextBattle();
	void setupBattle();
	void setupBoss();
	static void writeXML();
	void readXML();
	void readXMLEnemy( boost::property_tree::ptree& xmlEnemy);
	void readXMLEnemyName( nn::ts::quest::CEnemy * enemy,
		boost::property_tree::ptree& xmlEnemyName);
	void readXMLEnemyAttack( nn::ts::quest::CEnemy * enemy,
		boost::property_tree::ptree& xmlEnemyAttack);
	void readXMLBoss( boost::property_tree::ptree& xmlBoss);
	void dump();
	void addTe( int angle, bool spell);
	void setupCamera();
	void recoveryHPNextBattle();
	void syncDisplayHP();
	void damage( int damage);
	void recovery( int percent);
	
	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::quest::QUEST_GAME_MEMBER gm;
	static nn::ts::quest::QUEST_DISP_MEMBER dm;
	nn::ts::quest::QUEST_CTRL_MEMBER cm;
	nn::ts::quest::QUEST_LINK_MEMBER lm;
	
}; // CQuest
	 
} // namespece quest

} // namespace ts
} // namespace nn
	

#endif // __QUEST_H__
