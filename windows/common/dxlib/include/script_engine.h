
#ifndef __SCRIPT_ENGINE_H__
#define __SCRIPT_ENGINE_H__

#include "task_save.h"
#include "master.h"
#include "layout.h"

//------------------------------------------------------------------------------

namespace nn {
namespace common {
namespace e {
namespace script_engine {

#define SCRIPT_ENGINE_BACKUP_MAGIC_NO_SIZE (16)

enum {
	SCRIPT_ENGINE_LOAD_SCRIPT_VAR,
	
	SCRIPT_ENGINE_LOAD_MAX
};

enum {
	SCRIPT_ENGINE_LOAD_STATE_MAGIC_NO_LOAD_START,
	SCRIPT_ENGINE_LOAD_STATE_MAGIC_NO_LOAD,
	SCRIPT_ENGINE_LOAD_STATE_FIRST_START,
	SCRIPT_ENGINE_LOAD_STATE_FIRST,
	SCRIPT_ENGINE_LOAD_STATE_LOAD_START,
	SCRIPT_ENGINE_LOAD_STATE_LOAD,
	SCRIPT_ENGINE_LOAD_STATE_BREAK_DISP,
	SCRIPT_ENGINE_LOAD_STATE_BREAK,
	SCRIPT_ENGINE_LOAD_STATE_REPAIR,
	SCRIPT_ENGINE_LOAD_STATE_COMP,
	SCRIPT_ENGINE_LOAD_STATE_END,
	
	SCRIPT_ENGINE_LOAD_STATE_ERROR,
	
	SCRIPT_ENGINE_LOAD_STATE_MAX
};

enum {
	SCRIPT_ENGINE_LOAD_MODE_START,
	SCRIPT_ENGINE_LOAD_MODE_SCRIPT_VAR,
	
	SCRIPT_ENGINE_LOAD_MODE_MAX
};

enum {
	SCRIPT_ENGINE_SAVE_STATE_CHECK_START,
	SCRIPT_ENGINE_SAVE_STATE_CHECK,
	SCRIPT_ENGINE_SAVE_STATE_SAVE_START,
	SCRIPT_ENGINE_SAVE_STATE_SAVE,
	SCRIPT_ENGINE_SAVE_STATE_END,
	
	SCRIPT_ENGINE_SAVE_STATE_ERROR,
	
	SCRIPT_ENGINE_SAVE_STATE_MAX
};

enum {
	SCRIPT_ENGINE_SAVE_MODE_MAGIC_NO,
	SCRIPT_ENGINE_SAVE_MODE_SCRIPT_VAR,
	
	SCRIPT_ENGINE_SAVE_MODE_MAX,
	
	SCRIPT_ENGINE_SAVE_MODE_FIRST
};

enum {
	SCRIPT_ENGINE_STATE_NONE,
	SCRIPT_ENGINE_STATE_READ,
	SCRIPT_ENGINE_STATE_NEXT,
	SCRIPT_ENGINE_STATE_WAIT_CALL_BACK,
	SCRIPT_ENGINE_STATE_WAIT_TIMER,
	SCRIPT_ENGINE_STATE_WAIT_TOUCH,
	SCRIPT_ENGINE_STATE_WAIT_WAIT_INPUT,
	SCRIPT_ENGINE_STATE_WAIT_IMAGE_LOAD,
	SCRIPT_ENGINE_STATE_WAIT_ANIMA_ALL_END,
	SCRIPT_ENGINE_STATE_MSG,
	SCRIPT_ENGINE_STATE_ALL_END,
	
	SCRIPT_ENGINE_STATE_MAX
};

#define SCRIPT_VAR_SIZE (sizeof( int) * 512)
#define BACKUP_SCRIPT_VAR_N (1)

enum {
	SCRIPT_ENGINE_BACKUP_ADRESS_MAGIC_NO	=	0,
	SCRIPT_ENGINE_BACKUP_ADRESS_SCRIPT		=	SCRIPT_ENGINE_BACKUP_MAGIC_NO_SIZE,
	SCRIPT_ENGINE_BACKUP_ADRESS_SCRIPT_2	=	SCRIPT_ENGINE_BACKUP_ADRESS_SCRIPT + SCRIPT_VAR_SIZE + 2,
	SCRIPT_ENGINE_BACKUP_ADRESS_SCRIPT_3	=	SCRIPT_ENGINE_BACKUP_ADRESS_SCRIPT_2 + SCRIPT_VAR_SIZE + 2,
	
	SCRIPT_ENGINE_BACKUP_ADRESS_MAX			=	SCRIPT_ENGINE_BACKUP_ADRESS_SCRIPT_3 + SCRIPT_VAR_SIZE + 2
};

enum {
	SCRIPT_ENGINE_SELECT_STATE_MSG_OUT,
	SCRIPT_ENGINE_SELECT_STATE_SELECT,
	
	SCRIPT_ENGINE_SELECT_STATE_MAX
};

enum {
	SCRIPT_ENGINE_SELECT_LARGE_STATE_PLAY,
	SCRIPT_ENGINE_SELECT_LARGE_STATE_DIRECT,
	
	SCRIPT_ENGINE_SELECT_LARGE_STATE_MAX
};

enum {
	SCRIPT_ENGINE_MSG_STATE_NONE,
	SCRIPT_ENGINE_MSG_STATE_OUT,
	SCRIPT_ENGINE_MSG_STATE_WAIT,
	
	SCRIPT_ENGINE_MSG_STATE_MAX
};

enum {
	SCRIPT_ENGINE_FONT_DISP_MODE_MAIN,
	SCRIPT_ENGINE_FONT_DISP_MODE_SUB,
	
	SCRIPT_ENGINE_FONT_DISP_MODE_MAX
};

enum MODE {
	MODE_TOUCH,
	MODE_PAD,
	MODE_TOUCH_PAD,

	MODEMAX
};

enum TALK_CHAR_STATE {
	TALK_CHAR_STATE_HIDE,
	TALK_CHAR_STATE_ENABLE,
	TALK_CHAR_STATE_DISABLE,
	
	TALK_CHAR_STATE_MAX
};

enum TALK_CHAR_SIDE {
	TALK_CHAR_SIDE_LEFT,
	TALK_CHAR_SIDE_RIGHT,
	
	TALK_CHAR_SIDE_MAX
};

enum TALK_CHAR_MOVE {
	TALK_CHAR_MOVE_NONE,
	TALK_CHAR_MOVE_IN,
	TALK_CHAR_MOVE_OUT,
	
	TALK_CHAR_MOVE_MAX
};

enum TALK_OBJ_DISP_MODE {
	TALK_OBJ_DISP_MODE_OBJ,
	TALK_OBJ_DISP_MODE_MODEL,

	TALK_OBJ_DISP_MODE_MAX
};

enum MSG_MODE {
	MSG_MODE_NORMAL,
	MSG_MODE_ASYNC,
	MSG_MODE_REMAIN,

	MSG_MODE_MAX
};

enum MSG_BG_MODE {
	MSG_BG_MODE_BOARD,
	MSG_BG_MODE_OBJ,

	MSG_BG_MODE_MAX
};

enum SELECT_CURSOR_MODE {
	SELECT_CURSOR_MODE_LEFT_RIGHT,
	SELECT_CURSOR_MODE_UP_UP_DOWN,

	SELECT_CURSOR_MODE_MAX
};

} // namespace script_engine
} // namespace e

namespace dxlib {

typedef struct _script_engine_param {
	LAYOUT layoutMsg;
	int mode;
	int newLineH;
	int newLineHSub;

	int animeIdTalkOutLeftChar;
	int animeIdTalkOutRightChar;
	int msgBgMode;
	int layoutIdMsgBg;
	GXRgb layoutIdMsgBgColor;

	void * draw3d;
} SCRIPT_ENGINE_PARAM;

typedef struct _backup_data {
	
	u8 magicNo[SCRIPT_ENGINE_BACKUP_MAGIC_NO_SIZE];
	int * scriptVar;
	
} GAME_BACKUP_DATA;

class CScriptEngineLoad : public TASK_SAVE {
	
public:
	CScriptEngineLoad();
	CScriptEngineLoad( TASK_SAVE * oya);
	~CScriptEngineLoad();
	void task();
	static void load_next_cb( void * scriptEngineLoad);
	void load_next();

	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	u8 mode;
	u8 state;
	u8 cnt;
	MASTER * master;
	int no;
	
	BOOL loadBreakFlag;
	BOOL loadBreakFlagArray[nn::common::e::script_engine::SCRIPT_ENGINE_LOAD_MAX];
	BOOL loadScriptVarBreakFlagArray[BACKUP_SCRIPT_VAR_N];
	
	int var[3][512 + 1];
	
};

class CScriptEngineSave : public TASK_SAVE {
	
public:
	CScriptEngineSave();
	CScriptEngineSave( TASK_SAVE * oya);
	~CScriptEngineSave();
	void task();
	static void save_next_cb( void * scriptEngineSave);
	void save_next();

	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	u8 mode;
	u8 state;
	u8 cnt;
	MASTER * master;
	int no;
	
};

typedef struct _select_layout {
	int x;
	int y;
	int w;
	int h;
} SCRIPT_SELECT_LAYOUT;

typedef struct _selct_ctrl {
	SCRIPT_SELECT_LAYOUT selectLayout[16];
	int selectSeNo[16];
	int selectLayoutN;
	int cursorLayoutId[16];
} SCRIPT_SELECT_CTRL;

class CScriptEngineSelect : public TASK_SAVE {
	
public:
	CScriptEngineSelect();
	CScriptEngineSelect( TASK_SAVE * oya);
	~CScriptEngineSelect();
	void start();
	static void decide_select( void * selectArg);

	void task();

	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	char msg[256];
	BOOL msgFlag;
	int labelArray[16];
	MASTER * master;
	
	int cursor;
	
	SCRIPT_SELECT_CTRL * selectCtrl;
	
	int msgN;
	
	u16 state;
	u16 cnt;
	u16 largeState;
	
	int msgCnt;

	nn::common::dxlib::SCRIPT_ENGINE_PARAM * param;

	int cursorMode;
	
};

typedef struct _script_disp_obj {
	
	int objId;
	int x;
	int y;
	int w;
	int h;
	int z;
	int a;
	GXRgb color;
	int rotate;
	BOOL flipH;
	BOOL flipV;
	
} SCRIPT_DISP_OBJ;

typedef struct _script_disp_char {
	
	int charId;
	int x;
	int y;
	int z;
	BOOL flipH;
	BOOL flipV;
	
} SCRIPT_DISP_CHAR;

typedef struct _script_disp_anima {
	
	int animeId;
	int x;
	int y;
	int z;
	int waitMode;
	BOOL loopFlag;
	BOOL lastWaitFlag;
	int speed;
	
	int changeObjIdTypeArray[4];	//0なし　1置き換え　2シフト
	int changeObjIdLayerNoArray[4];	//操作レイヤー番号
	int changeObjIdNArray[4];	//操作値
	
	BOOL execFlag;
	TASK_SAVE * scriptEngine;
	TASK_SAVE * anime;
	
} SCRIPT_DISP_ANIME;

typedef struct _script_stack SCRIPT_STACK;
struct _script_stack {
	
	char name[64];
	int readCnt;
	
	SCRIPT_STACK * next;
	int label[256];
	int skipLabelIndex;
	
};

class CScriptEngineMsg : public TASK_SAVE {

public:
	CScriptEngineMsg();
	CScriptEngineMsg( TASK_SAVE * oya);
	~CScriptEngineMsg();
	void start();

	void task();

	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	char msg[256];
	
	int msgN;
	
	MASTER * master;
	
	u16 state;
	u16 cnt;
	
	int msgCnt;

	LAYOUT layoutMsg;
	int mode;
	int newLineH;
	int newLineHSub;
	int msgBgMode;
	int layoutIdMsgBg;
	GXRgb layoutIdMsgBgColor;
	
	int msgMode;
	
};

typedef struct _script_engine_talk_char {

	LAYOUT layoutBase;
	LAYOUT layoutOption[16];
	int layoutOptionState[16];
	int state;
	int side;
	int outFrameCnt;
	int moveState;
	int objDispMode;
	int scale;

} SCRIPT_ENGINE_TALK_CHAR;

class CScriptEngineTalk : public TASK_SAVE {

public:
	CScriptEngineTalk();
	CScriptEngineTalk( TASK_SAVE * oya);
	~CScriptEngineTalk();

	void start();

	void task();

	void setTalkCharState( int charIndex, int state);

	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	MASTER * master;
	
	u16 state;
	u16 cnt;

	SCRIPT_ENGINE_TALK_CHAR talkChar[8];

	int animeIdOutLeftChar;
	int animeIdOutRightChar;

	void * draw3d;
};

class CScriptEngine : public TASK_SAVE {

private:
	void comment();
	void disp_text_main();
	void delete_text_main();
	void disp_text_sub();
	void delete_text_sub();
	void skip();
	void volume_se();
	void volume_bgm();
	void stop_se();
	void stop_bgm();
	void play_se();
	void bgm_unload();
	void play_bgm();
	void bgm_load();
	void main_sub_disp();
	void set_select();
	void select();
	void script_inc( char ** script, int * cnt, int n);
	void get_script_var_const();
	int get_script_var_index( char ** script, int * cnt);
	int get_script_var_const( char ** script, int * cnt);
	void script_nl();
	void msg();
	void change_anima_obj_id();
	void delete_anime();
	void set_anime();
	void disp_anime();
	void exec_anime( SCRIPT_DISP_ANIME * dispAnime);
	void anima_arc_load();
	void anima_arc_unload();
	void bg_load();
	void bg_unload();
	void image_load();
	void image_unload();
	void texture_load();
	void texture_unload();
	void script_if();
	void jmp();
	void add();
	void sub();
	void mul();
	void div();
	void mod();
	void equal();
	void random();
	void print();
	void delete_bg();
	void disp_bg();
	void delete_char();
	void disp_char();
	void delete_obj();
	void disp_polygon();
	void disp_obj();
	void wait();
	void input();
	void end();
	void func();
	void call();
	void fade();
	void msg_async();
	void delete_msg();
	int get_script_n( char ** script, int * cnt);
	int pop_script();
	void push_script( char * scriptName);
	int script_cmp( char * s, int n);
	void set_scirpt_state_read();
	void font_disp( int x, int y, char * argS, int dispN, int argMode);
	int get_font_disp_n( char * s, int * n);
	void set_default();
	void script_load();
	TASK_SAVE * save_script_var( TASK_SAVE * oya, MASTER * master, void (* callBackFunc)(void *), void * callBackFuncArg, int no);
	TASK_SAVE * save_first( TASK_SAVE * oya, MASTER * master, void (* callBackFunc)(void *), void * callBackFuncArg);
	void script_save();
	void talk_char();
	int get_script_talk_char_side( char ** script, int * cnt);
	void talk_char_state();
	int get_script_talk_char_state( char ** script, int * cnt);
	void talk_option();
	void talk_option_state();
	int check_talk_index();

public:
	CScriptEngine();
	CScriptEngine( TASK_SAVE * oya);
	CScriptEngine( TASK_SAVE * oya, nn_common::SaveManager * m);
	~CScriptEngine();
	void constructorCommon();
	void start();
	void script( char * scriptPath);
	void task();
	void setScriptStateRead();
	void fontMainDisp( int x, int y, char * argS, int dispN);
	void fontSubDisp( int x, int y, char * argS, int dispN);
	
	void msg(char * text, void (* endCb)(void *), void * endCbArg);

	static void select_call_back( void * scriptEngine);
	static void set_scirpt_state_read( void * scriptEngine);
	static void anima_end_call_back( void * dispAnime);
	static void anima_destructor_func( void * dispAnime);

	static void set_scirpt_state_read_cb( void * scriptEngine);
	
	static bool isOperationNext( int mode, KEY_WORK * key, TOUCH_WORK * touch);
	void endFunc();

	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	MASTER * master;
	void (* scriptEndCallBackFunc)( void *);
	void * scriptEndCallBackFuncArg;
	
	SCRIPT_STACK * stack;
	
	char * open;
	int state;
	
	int waitFrame;
	int waitCnt;
	
	SCRIPT_DISP_OBJ dispObj[128];
	SCRIPT_DISP_CHAR dispChar[128];
	int dispBgId;
	
	int var[512 + 1];
	
	SCRIPT_DISP_ANIME dispAnime[32];
	
	SCRIPT_SELECT_CTRL selectCtrl[4];
	CScriptEngineSelect * scriptEngineSelect;
	
	void (* funcArray[256])( void *);
	void * funcArrayArg[256];
	
	char dispTextMain[256];
	char dispTextSub[256];
	
	int netBootType[2];
	int netChar[2];

	SCRIPT_ENGINE_PARAM param;

	CScriptEngineTalk * talk[1];

	CScriptEngineMsg * msgCtrl[1];

	static ::nn_common::DATARECODE dataRecode[];

	char scriptDirectory[256];

};

} // namespcae dxlib
} // namespace common
} // namespace nn

//------------------------------------------------------------------------------

#endif //__SCRIPT_ENGINE_H__

