// CONFIG を置換
// config を置換
// Config を置換
// yb を置換
// main を置換

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"

namespace nn {
namespace yb {
//---------------------------------------------------------------------------
// enum
namespace e {
namespace config {

enum MAIN_ITEM {
	MAIN_ITEM_BGM_VOL,
	MAIN_ITEM_SE_VOL,
	MAIN_ITEM_DISP_SIZE,
	MAIN_ITEM_DISP_MASK,
	MAIN_ITEM_PAD,
	MAIN_ITEM_SAVE_CANCEL,

	MAIN_ITEM_MAX
};

enum SET_BGM_VOL_ITEM {
	SET_BGM_VOL_ITEM_MAX = 128
};

enum SET_SE_VOL_ITEM {
	SET_SE_VOL_ITEM_MAX = 128
};

enum SET_DISP_SIZE_ITEM {
	SET_DISP_SIZE_ITEM_WINDOW,
	SET_DISP_SIZE_ITEM_FULL,

	SET_DISP_SIZE_ITEM_MAX
};

enum SET_DISP_MASK_ITEM {
	SET_DISP_MASK_ITEM_ENABLE,
	SET_DISP_MASK_ITEM_DISABLE,

	SET_DISP_MASK_ITEM_MAX
};

enum SET_PAD_ITEM {
	SET_PAD_ITEM_MAX = 0
};

enum STATE {
	STATE_NONE,
	STATE_MAIN,
	STATE_PAD,
	
	STATE_MAX
};

enum PAD_CONFIG {
	PAD_CONFIG_A,
	PAD_CONFIG_B,
	PAD_CONFIG_X,
	PAD_CONFIG_Y,
	PAD_CONFIG_L,
	PAD_CONFIG_R,
	PAD_CONFIG_START,
	//PAD_CONFIG_SELECT,

	PAD_CONFIG_MAX
};

} // namespece config
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace main {
	
class CConfig : public TASK_SAVE {
	
public:
	CConfig( TASK_SAVE * oya);
	virtual ~CConfig();
	void task();
	void disp();
	
	void start();

	int bsf(unsigned int mask);

	void taskMain();
	void taskPad();

	void renewButtonNoTbl();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	nn::yb::main::CSaveData * saveData;

	int cursorY;
	int cursorX;
	int oldOriginalPadState;
	int itemN;
	int configPad[MASTER_CONFIG_PAD_N];
	int buttonNoTbl[MASTER_CONFIG_PAD_N];

	int cursorYMain;
	int cursorXMain;

	nn::yb::main::CCtrl * ctrl;
	void (* endCb)( void *);
	void * endCbArg;

	nn::yb::main::CONFIG_DATA oldConfig;
	
}; // CConfig
	 
} // namespece main

} // namespace yb
} // namespace nn
	

#endif // __CONFIG_H__
