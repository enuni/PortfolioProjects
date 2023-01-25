// MENU を置換
// menu を置換
// Menu を置換
// yb を置換
// main を置換

#ifndef __MENU_H__
#define __MENU_H__

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
namespace menu {

enum STATE {
	STATE_NONE,
	STATE_LOAD,
	STATE_LOGO,
	STATE_TITLE,
	STATE_MAIN,
	STATE_GAME,
	STATE_CONFIG,
	
	STATE_MAX
};

enum CURSOR_Y {
	CURSOR_Y_GAME,
	CURSOR_Y_EDIT,

	CURSOR_Y_MAX
};

enum GAME_STATE {
	GAME_STATE_MAIN,
	GAME_STATE_STAGE_MODE_SELECT,
	GAME_STATE_STAGE_SELECT,
	GAME_STATE_SCORE,

	GAME_STATE_MAX
};

enum STAGE_MODE {
	STAGE_MODE_CHAOS,
	STAGE_MODE_STANDARD,

	STAGE_MODE_MAX
};

enum CURSOR_Y_GAME {
	CURSOR_Y_GAME_RESUME,
	CURSOR_Y_GAME_START,
	CURSOR_Y_GAME_SCORE,
	CURSOR_Y_GAME_CONFIG,
	CURSOR_Y_GAME_QUIT,

	CURSOR_Y_GAME_MAX
};

enum CURSOR_Y_SCORE {
	CURSOR_Y_SCORE_STAGE_MAX = nn::yb::e::global::STAGE_N - 1,
	CURSOR_Y_SCORE_QUIT,

	CURSOR_Y_SCORE_MAX
};

enum CURSOR_X_SCORE {
	CURSOR_X_SCORE_CHAOS,
	CURSOR_X_SCORE_STANDARD,

	CURSOR_X_MAX
};

enum STATE_LOGO {
	LOGO_STATE_WARNING,
	LOGO_STATE_MAIN,

	LOGO_STATE_MAX
};

enum TITLE_STATE {
	TITLE_STATE_FADE_IN,
	TITLE_STATE_TEXT_IN,
	TITLE_STATE_MAIN,

	TITLE_STATE_MAX
};

enum LOAD_STATE {
	LOAD_STATE_IMAGE,
	LOAD_STATE_SE,
	LOAD_STATE_BGM,
	
	LOAD_STATE_MAX
};

} // namespece menu
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace main {
	
class CMenu : public TASK_SAVE {
	
public:
	CMenu( TASK_SAVE * oya);
	virtual ~CMenu();
	void task();
	void disp();
	
	void start();

	void taskGame();

	void startStageFromScratch();

	void dispGame();
	void initFlower();

	// 分かりにくいので、変更
	//void returnMain();
	//static void returnMainCb( void * menuArg);
	void returnGameStateMain();
	static void returnGameStateMainCb( void * menuArg);

	void returnTitle();
	void returnStageSelect();

	void goGameMain();
	void taskMain();
	void goLogo();
	void goTitle();
	void taskLogo();
	void taskTitle();

	static void decWaitCntCb( void * menuArg);
	void decWaitCnt();
	static void nextCb( void * menuArg);
	void next();
	void cutoutSanae( void (* cb)( void *), void * cbArg);
	
	static void startStageFromScratchCb(void * menuArg);

	static void startStageFromBreakCb(void * menuArg);
	void startStageFromBreak();
	static void endSanaeCutinCb( void * menuArg);
	void endSanaeCutin();

	void taskLoad();
	void nextLoad();
	void nextLogo();
	void taskCreateFlower();

	void taskGameMain();
	void taskGameStageModeSelect();
	void taskGameStageSelect();
	void taskGameScore();

	void dispGameScore();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	nn::yb::main::CCtrl * ctrl;
	nn::yb::main::CSaveData * saveData;

	int gameState;
	int cursorYGame;
	//int cursorYGameStageMenu;
	int cursorYScore;

	unsigned int logoFrameCnt;
	int logoState;

	void * animeTitleTextTask;
	int titleState;
	unsigned int titleFrameCnt;

	void * animeSanaeTask;

	bool stageStartFlag;
	bool sanaeInCompFlag;
	
	int frameCnt;
	int loadState;
	int loadBgmCnt;
	nn::yb::main::CConfig * config;
	nn::yb::main::CFlower * rootFlower;

	int cursorXScore;

}; // CMenu
	 
} // namespece main

} // namespace yb
} // namespace nn
	

#endif // __MENU_H__
