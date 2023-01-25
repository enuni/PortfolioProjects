// EDIT_MENU を置換
// edit_menu を置換
// EditMenu を置換
// yb を置換
// edit を置換

#ifndef __EDIT_MENU_H__
#define __EDIT_MENU_H__

#include "task_save.h"
#include "master.h"
#include "enum.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
//---------------------------------------------------------------------------
// enum
namespace nn {
namespace yb {

namespace e {
namespace edit_menu {

enum STATE {
	STATE_NONE,
	STATE_WINDOW_SIZE_SELECT,
	STATE_EDIT_DATA_SELECT,
	STATE_EDIT_SPELL_SELECT,
	
	STATE_MAX
};

} // namespece edit_menu
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace edit {

extern const char * gHoleDataPath;

class CEditMenu : public TASK_SAVE {
	
public:
	CEditMenu( TASK_SAVE * oya);
	virtual ~CEditMenu();
	void task();
	void disp();
	
	void start();
	void edit();
	void returnEdit();
	void selectData();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	int cursorX;
	// saveDataに置き換え
	//int cursorYWindowSize;
	//int cursorYEditData;

	bool editDataFlag[nn::yb::e::global::EDIT_DATA_MAX];
	
	nn::yb::common::CYbSaveManager * saveManager;
	nn::yb::main::CCtrl * ctrl;

	char editDataDate[nn::yb::e::global::EDIT_DATA_MAX][32];

	bool returnFlag;

	nn::yb::main::CSaveData * saveData;

	int prevHoleSpell[16];
	nn::yb::edit::CDataHole * dataHole;
	
}; // CEditMenu
	 
} // namespece edit

} // namespace yb
} // namespace nn
	

#endif // __EDIT_MENU_H__
