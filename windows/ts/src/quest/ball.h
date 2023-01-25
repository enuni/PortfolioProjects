// BALL を置換
// ball を置換
// Ball を置換
// tsを置換
// quest を置換

#ifndef __BALL_H__
#define __BALL_H__

#include "game_char.h"
#include "master.h"

#include "nn_common2.h"
#include "direct.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
extern MASTER * master;
//---------------------------------------------------------------------------
namespace nn {
namespace ts{

// enum
namespace e {
namespace ball {

enum STATE {
	STATE_NONE,
	STATE_IDLE,
	STATE_SHOT,
	STATE_MOVE,
	STATE_DIRECT,
	
	STATE_MAX
};

} // namespece ball
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct _ball_status {
	double x;
	double y;
	double speed;
} BALL_STATUS;

typedef struct _ball_param {
	int ap;
	int sp;
	int hp;
} BALL_PARAM;

typedef struct _ball_game_member {
	int charID;
	nn::ts::quest::BALL_STATUS status;
	nn::ts::quest::BALL_PARAM param;
	int shotAngle;
	double decSpeed;
	int turn;
	bool friendComboFlag;
} BALL_GAME_MEMBER;

typedef struct _ball_disp_membar {
	int * objIdBalls;
	int * objIdBallBgs;
	int animeIdBallIn;
} BALL_DISP_MEMBER;

typedef struct _ball_shot_member {

} BALL_SHOT_MEMBER;

typedef struct _ball_link_member {
	nn::ts::quest::CQuest * quest;
	nn::ts::quest::CDamageDisp * dd;
} BALL_LINK_MEMBER;

typedef struct _ball_task_shot_log {
	nn::common::util::type::t2d::COLLISION_ITEM cis[16];
	int afterAngles[16];
	double afterLens[16];
	double collisionX1s[16];
	double collisionY1s[16];
	double collisionX2s[16];
	double collisionY2s[16];
	double ci2Balls[16];
	int angles[16];
} BALL_TASK_SHOT_LOG;

typedef struct _ball_task_shot_member {
	int cnt;
	double x1;
	double y1;
	double x2;
	double y2;
	bool collision;
	double collisionX;
	double collisionY;
	double afterXRate;
	double afterYRate;
	int afterAngle;
	double afterLen;

	nn::ts::quest::BALL_TASK_SHOT_LOG log;
} BALL_TASK_SHOT_MEMBER;
	
class CBall : public nn::common::CGameChar {
	
public:
	CBall();
	CBall( TASK_SAVE * oya);
	CBall( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CBall();
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

	void shot( int angle);
	void taskShot();
	void endShot();
	void hitEnemy( nn::ts::quest::BALL_TASK_SHOT_MEMBER * tsm,
		nn::common::util::type::t2d::COLLISION_ITEM * ci);
	void hitBall( nn::ts::quest::CBall * parther);
	void taskMove();
	void hitEnemyReflec(nn::ts::quest::BALL_TASK_SHOT_MEMBER * tsm,
		nn::common::util::type::t2d::COLLISION_ITEM * ci);
	void move( std::vector<nn::common::util::type::t2d::COLLISION_ITEM> * collisions);
	void addOtherBallCollision( std::vector<nn::common::util::type::t2d::COLLISION_ITEM> * collisions);
	void friendCombo( nn::ts::quest::CBall * parther);
	void damage( int damage);
	void deleteDamageDisp();
	static void finDeleteDamgeDispCB( void * argBall);
	void finDeleteDamgeDisp();
	void recovery( int percent);

	void dump();


	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];
	
	nn::ts::quest::BALL_GAME_MEMBER gm;
	static nn::ts::quest::BALL_DISP_MEMBER dm;

	void (* shotEndCBFunc)(void *);
	void * shotEndCBFuncArg;

	int loopCollisionStopCnt;
	nn::common::util::type::t2d::POINT2_F64 loopCollisionStopP[16];

	nn::ts::quest::BALL_TASK_SHOT_MEMBER taskShotMember;

	nn::dxlib::direct::CDispDirect * dispDirect;

	nn::ts::quest::BALL_LINK_MEMBER lm;

}; // CBall
	 
} // namespece quest

} // namespace game_ns
} // namespace nn
	

#endif // __BALL_H__
