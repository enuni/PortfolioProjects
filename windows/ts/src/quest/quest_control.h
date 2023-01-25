// QUEST_CONTROL を置換
// quest_control を置換
// QuestControl を置換
// ts を置換
// quest を置換

#ifndef __QUEST_CONTROL_H__
#define __QUEST_CONTROL_H__

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
namespace quest_control {

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

} // namespece quest_control
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct _control_te {
	int angle;
	bool spell;
} QUEST_CONTROL_TE;

typedef struct _control_quest_log {
	nn::ts::quest::QUEST_CONTROL_TE te[256];
	int lastTurn;
} QUEST_CONTROL_QUEST_LOG;

typedef struct _quest_ctrl_game_member {
	nn::ts::quest::QUEST_CONTROL_QUEST_LOG log;
} QUEST_CONTROL_GAME_MEMBER;

typedef struct _quest_ctrl_link_member {
	nn::ts::quest::CQuest * quest;
	nn::ts::common::CControl * ctrl;
} QUEST_CONTROL_LINK_MEMBER;
	
class CQuestControl : public TASK_SAVE {
	
public:
	CQuestControl();
	CQuestControl( TASK_SAVE * oya);
	CQuestControl( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CQuestControl();
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

	void addTe( int angle, bool spell);
	void dump();
	void createQuest();
	void resetQuest();

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;
	
	MASTER * master;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::quest::QUEST_CONTROL_GAME_MEMBER gm;
	nn::ts::quest::QUEST_CONTROL_LINK_MEMBER lm;
	
}; // CQuestControl
	 
} // namespece quest

} // namespace ts
} // namespace nn
	

#endif // __QUEST_CONTROL_H__
