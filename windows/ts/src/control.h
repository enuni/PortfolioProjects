// CONTROL ��u��
// control ��u��
// Control ��u��
// ts ��u��
// common ��u��

#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <string>
#include <map>

#include "task_save.h"
#include "master.h"
#include "font2.h"
//---------------------------------------------------------------------------
// �O���錾
#include "class_list.h"
extern MASTER * master;
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace control {

enum STATE {
	STATE_NONE,
	STATE_TITLE,
	STATE_QUEST,
	STATE_DEBUG_MENU,
	STATE_WAIT,
	
	STATE_MAX
};

enum TUTORIAL {
	TUTORIAL_PULL,
	TUTORIAL_PENETRATION,
	TUTORIAL_ITEM,
	TUTORIAL_WEAKPOINT,
	
	TUTORIAL_MAX
};

} // namespece control
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace common {

typedef struct _save_data {
	bool tutorials[nn::ts::e::control::TUTORIAL_MAX];
} SAVE_DATA;

typedef struct _control_game_member {
	bool dispSkipFlag;
	int autoFlag;
	int autoEndTurn;
} CONTROL_GAME_MEMBER;
	
class CControl : public TASK_SAVE {
	
public:
	CControl();
	CControl( TASK_SAVE * oya);
	CControl( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CControl();
	void constructCommon();					// �����o�[0�N���A�Ȃ�new������constructor�ŕK����鎖
	void task();
	void disp();
	
	void setMember( void * owner);			// �����������TASK����K�v�ȃp�����[�^�����炤 void *�^�͐錾����ς��Ă��ǂ����A�֐����Ō^�ϊ����Ă��ǂ�
	void setMemberLoad( void * owner);		// setMember()�̃��[�h���ŁA���[�h���ĕs�v�Ȃ��̂̓Z�b�g���Ȃ�
	void setMemberCommon( void * owner);	// setMember()��setMemberLoad()�̋��ʏ���
	void start();							// setMember()��ɂ��Task�����グ����
	void startLoad();						// start()�̃��[�h����
	void startCommon();						// start()��startLoad()�̋��ʏ���
	void dump();
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	void startTitle();
	void startQuest();
	void startQuestBegining();
	void startQuestResume();
	
	static void debugMenuCommandCB( void * argControl, int command);
	void debugMenuCommand( int command);
	void endDebugMenu();

	static void resumeSelectEndCB( void * argControl);
	void resumeSelectEnd();

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::quest::CQuestControl * questCtrl;

	SAVE_DATA saveData;

	std::map<std::string, int> enemyName2Id;
	std::map<std::string, int> enemyAttackName2Id;

	nn::ts::title::CTitle * title;

	nn::ts::common::CONTROL_GAME_MEMBER gm;

	nn::dxlib::CFont * font;
	nn::ts::debug::CDebugMenu * debugMenu;
	
}; // CControl
	 
} // namespece common

} // namespace ts
} // namespace nn
	

#endif // __CONTROL_H__
