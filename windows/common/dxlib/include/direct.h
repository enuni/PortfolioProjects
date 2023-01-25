// DIRECT を置換
// direct を置換
// DirectFade を置換
// direct を置換
// dxlib を置換

#ifndef __DIRECT_H__
#define __DIRECT_H__

#include "task_save.h"
#include "master.h"
#include "anim.h"
//---------------------------------------------------------------------------
// 前方宣言
//---------------------------------------------------------------------------
namespace nn {
namespace dxlib {

// enum
namespace e {
namespace direct {

enum STATE {
	STATE_NONE,
	STATE_ACTIVE,
	
	STATE_MAX
};

enum FADE_MODE {
	FADE_MODE_BLACK_FADE_OUT,
	FADE_MODE_BLACK_FADE_IN,
	FADE_MODE_WHITE_FADE_OUT,
	FADE_MODE_WHITE_FADE_IN,

	FADE_MODE_MAX
};

enum WAIT_MODE {
	WAIT_MODE_NORMAL,
	WAIT_MODE_SKIP,
	WAIT_MODE_INPUT,

	WAIT_MODE_MAX
};

enum DISP_DIRECT_STATE {
	DISP_DIRECT_STATE_NONE,
	DISP_DIRECT_STATE_ANIME_SYNC,
	DISP_DIRECT_STATE_ANIME_ASYNC,

	DISP_DIRECT_STATE_MAX
};

} // namespece direct
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace direct {

class CWait : public TASK_SAVE {
	
public:
	CWait( TASK_SAVE * oya);
	CWait( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CWait();
	void constructorCommon();
	void task();
	void disp();
	
	void start();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	unsigned int frameCnt;
	unsigned int endFrame;
	int mode;
	
	void (* destructorFunc)(void *);
	void * destructorFuncArg;
}; // CWait
	
class CFade : public TASK_SAVE {
	
public:
	CFade( TASK_SAVE * oya);
	CFade( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CFade();
	void constructorCommon();
	void task();
	void disp();
	
	void start();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	unsigned int frameCnt;
	unsigned int endFrame;
	unsigned char acc;
	int mode;
	void (* destructorFunc)(void *);
	void * destructorFuncArg;

}; // CFade

TASK_SAVE * fade( TASK_SAVE * parent, int mode,
	void (* destructorFunc)(void *), void * destructorFuncArg,
	int frame = 16, unsigned char acc = 127
);
TASK_SAVE * waitSimple( TASK_SAVE * parent, unsigned int waitFrame,
	void (* destructorFunc)(void *), void * destructorFuncArg
);
TASK_SAVE * waitInput( TASK_SAVE * parent, MASTER * master,
	void (* destructorFunc)(void *), void * destructorFuncArg
);

typedef struct _disp_direct_user_set_member {
	int animeId;
	int layer;
	void (*directEndCBFunc)(void *);
	void * directEndCBFuncArg;
	int directEndCBFuncId;
	void * directEndCBFuncOwner;
} DISP_DIRECT_USER_SET_MEMBER;

typedef struct _disp_direct_ctrl_set_member {
	nn::dxlib::e::direct::DISP_DIRECT_STATE state;
	int frame;
	nn::dxlib::ANIM_DISP_DATA add;
} DISP_DIRECT_CTRL_SET_MEMBER;

typedef struct _disp_direct_member {
	DISP_DIRECT_USER_SET_MEMBER user;
	DISP_DIRECT_CTRL_SET_MEMBER ctrl;
} DISP_DIRECT_MEMBER;

class CDispDirect {

public:
	CDispDirect();
	virtual ~CDispDirect();
	void task();
	void start();

	DISP_DIRECT_MEMBER m;

};	// TaskのDisp自体を動かして演出する制御クラス

} // namespace direct	 
} // namespece dxlib

} // namespace nn
	

#endif // __DIRECT_H__
