// DEBUG_MENU を置換
// debug_menu を置換
// DebugMenu を置換
// ts を置換
// debug を置換

#ifndef __DEBUG_MENU_H__
#define __DEBUG_MENU_H__

#include "task_save.h"
#include "master.h"
#include "font2.h"
//---------------------------------------------------------------------------
// 前方宣言
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
	void constructCommon();					// メンバー0クリアなどnewしたらconstructorで必ずやる事
	void task();
	void disp();
	
	void setMember( void * owner);			// 自分を作ったTASKから必要なパラメータをもらう void *型は宣言から変えても良いし、関数内で型変換しても良い
	void setMemberLoad( void * owner);		// setMember()のロード時版、ロードして不要なものはセットしない
	void setMemberCommon( void * owner);	// setMember()とsetMemberLoad()の共通処理
	void start();							// setMember()後にやるTask立ち上げ処理
	void startLoad();						// start()のロード時版
	void startCommon();						// start()とstartLoad()の共通処理
	void dump();								// メンバの値をデバッグ出力
	
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
