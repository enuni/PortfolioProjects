// QUEST_CONTROL ��u��
// quest_control ��u��
// QuestControl ��u��
// ts ��u��
// quest ��u��

#ifndef __QUEST_CONTROL_H__
#define __QUEST_CONTROL_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// �O���錾
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
