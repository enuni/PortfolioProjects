// DEBUG_MENU ��u��
// debug_menu ��u��
// DebugMenu ��u��
// ts ��u��
// debug ��u��

#ifndef __DEBUG_MENU_H__
#define __DEBUG_MENU_H__

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
namespace debug_menu {

enum STATE {
	STATE_NONE,
	STATE_MAIN,
	
	STATE_MAX
};

enum DEBUG_MENU {
	DEBUG_MENU_X = 630,
	DEBUG_MENU_Y = 50,
	DEBUG_MENU_TEXT_HEIGHT = 22,
	DEBUG_MENU_BG_MERGIN = 10,
	DEBUG_MENU_TEXT_WIDTH = 20 * 10,
};

} // namespece debug_menu
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace debug {

typedef struct _debug_menu_link_member {
	nn::dxlib::CFont * font;
	void (* commandCBFunc)( void *, int);
	void * commnadCBFuncArg;
} DEUBG_MENU_LINK_MEMBER;

typedef struct _debug_menu_game_member {
	int cursorY;
} DEUBG_MENU_GAME_MEMBER;

typedef struct _debug_menu_member {
	nn::ts::debug::DEUBG_MENU_LINK_MEMBER link;
	nn::ts::debug::DEUBG_MENU_GAME_MEMBER game;
} DEUBG_MENU_MEMBER;
	
class CDebugMenu : public TASK_SAVE {
	
public:
	CDebugMenu();
	CDebugMenu( TASK_SAVE * oya);
	CDebugMenu( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CDebugMenu();
	void constructCommon();					// �����o�[0�N���A�Ȃ�new������constructor�ŕK����鎖
	void task();
	void disp();
	
	void setMember( void * owner);			// �����������TASK����K�v�ȃp�����[�^�����炤 void *�^�͐錾����ς��Ă��ǂ����A�֐����Ō^�ϊ����Ă��ǂ�
	void setMemberLoad( void * owner);		// setMember()�̃��[�h���ŁA���[�h���ĕs�v�Ȃ��̂̓Z�b�g���Ȃ�
	void setMemberCommon( void * owner);	// setMember()��setMemberLoad()�̋��ʏ���
	void start();							// setMember()��ɂ��Task�����グ����
	void startLoad();						// start()�̃��[�h����
	void startCommon();						// start()��startLoad()�̋��ʏ���
	void dump();								// �����o�̒l���f�o�b�O�o��
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::debug::DEUBG_MENU_MEMBER m;
	
}; // CDebugMenu
	 
} // namespece debug

} // namespace ts
} // namespace nn
	

#endif // __DEBUG_MENU_H__
