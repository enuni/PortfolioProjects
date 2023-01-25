// SCRIPT_TASK を置換
// script_task を置換
// ScriptTask を置換
// dxlib を置換
//  を置換

#ifndef __SCRIPT_TASK_H__
#define __SCRIPT_TASK_H__

#include "task_save.h"
#include "master.h"
#include "obj.h"
#include "layout.h"
#include "script.h"
//---------------------------------------------------------------------------
namespace nn {
namespace dxlib {

// enum
namespace e {
namespace script_task {

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

enum MSG_STATE {
	MSG_STATE_NONE,
	MSG_STATE_OUT,
	MSG_STATE_OUT_END,
	MSG_STATE_WAIT,
	
	MSG_STATE_MAX
};

enum SELECT_STATE {
	SELECT_STATE_NONE,
	SELECT_STATE_WAIT,
	SELECT_STATE_SELECT,
	SELECT_STATE_DECIDE,

	SELECT_STATE_MAX
};

} // namespece script_task
} // namespece e
//---------------------------------------------------------------------------
// classs

class CScript;
class CFont;
	
class CScriptTask : public TASK_SAVE {
	typedef struct _msg_member {
		char msg[256];
		
		int cnt;
		int msgN;
		int msgCnt;
		int msgMode;

		nn::dxlib::SCRIPT_MSG_PARAM param;

		void (* userDispFunc)( void *);
		void * userDispFuncArg;

		/*
		LAYOUT layoutMsg;
		int mode;
		int newLineH;
		int newLineHSub;
		int msgBgMode;
		int layoutIdMsgBg;
		GXRgb layoutIdMsgBgColor;
		*/
	} MSG_MEMBER;

	typedef struct _select_member {
		int cursor;
		int buttonAnimeActiveCnt;

		nn::dxlib::SCRIPT_SELECT_PARAM param;
	} SELECT_MEMBER;

	typedef struct _select_text_member {
		int cursor;
		int decideFrameCnt;

		nn::dxlib::SCRIPT_SELECT_TEXT_PARAM param;
	} SELECT_TEXT_MEMBER;
	
public:

	CScriptTask( TASK_SAVE * oya);
	CScriptTask( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CScriptTask();
	void constructorCommon();
	void task();
	void disp();
	
	void start();

	void taskMsg();

	static void dispText( CFont * font, int x, int y, int hSpace, int vSpace, char * argS, int dispN);
	static int getFontN( char * argS);

	static bool isOpeNext( int mode, KEY_WORK *key, TOUCH_WORK *touch);

	static TASK_SAVE * msgStart(TASK_SAVE *parent, char *msg,
		nn::dxlib::e::script_task::MODE mode,
		void (* endFunc)( void *), void *endFuncArg,
		MASTER * master, nn::dxlib::SCRIPT_MSG_PARAM * msgParam,
		nn::dxlib::CFont * font
	);

	void taskSelect();
	void dispSelect();
	void taskSelectText();
	void dispSelectText();

	static void nextStateCB ( void * scriptTask);
	void nextState();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	int mode;
	nn::dxlib::CScriptTask::MSG_MEMBER msg;
	nn::dxlib::CScriptTask::SELECT_MEMBER select;
	nn::dxlib::CScriptTask::SELECT_TEXT_MEMBER selectText;

	void (* destructorFunc)( void *);
	void * destructorFuncArg;

	nn::dxlib::CScript * script;


	void (* endFunc)( void *);
	void * endFuncArg;

	CFont * font;
	
}; // CScriptTask

} // namespace dxlib
} // namespace nn
	

#endif // __SCRIPT_TASK_H__
