// PAUSE ��u��
// pause ��u��
// Pause ��u��
// yb ��u��
// main ��u��

#ifndef __PAUSE_H__
#define __PAUSE_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// �O���錾
#include "class_list.h"
//---------------------------------------------------------------------------
namespace nn {
namespace yb {

// enum
namespace e {
namespace pause {

enum STATE {
	STATE_NONE,
	STATE_MENU,
	STATE_SELECT,
	STATE_HOLE_RESTART_FAIL,
	
	STATE_MAX
};

enum SELECT {
	SELECT_CANCEL,
	SELECT_RESTART_HOLE,
	SELECT_RESTART_STAGE,
	SELECT_TITLE,

	SELECT_MAX
};

enum SELECT_CHECK {
	SELECT_CHECK_YES,
	SELECT_CHECK_NO,

	SELECT_CHECK_MAX
};

} // namespece pause
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace main {
	
class CPause : public TASK_SAVE {
	
public:
	CPause( TASK_SAVE * oya);
	virtual ~CPause();
	void task();
	void disp();
	
	void start();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	nn::yb::stage::CHoleCtrl * holeCtrl;

	void (* endCb)(void *);
	void * endCbArg;

	int cursorY;
	int cursorXSelect;

	int holeRestartFailCnt;
	
}; // CPause
	 
} // namespece main

} // namespace yb
} // namespace nn
	

#endif // __PAUSE_H__
