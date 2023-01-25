// DEAD を置換
// dead を置換
// Dead を置換
// yb を置換
// stage を置換

#ifndef __DEAD_H__
#define __DEAD_H__

#include "task_save.h"
#include "master.h"
#include "task.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"

namespace nn {
namespace yb {
//---------------------------------------------------------------------------
// enum
namespace e {
namespace dead {

enum STATE {
	STATE_NONE,
	STATE_DEAD_START,
	STATE_DEAD_HP_UP,
	STATE_GAMEOVER_START,
	STATE_GAMEOVER,
	
	STATE_MAX
};

} // namespece dead
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace stage {
	
class CDead : public TASK_SAVE {
	
public:
	CDead( TASK_SAVE * oya);
	virtual ~CDead();
	void task();
	void disp();
	
	void start();

	static void nextCb( void *);
	void next();
	void end();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	nn::yb::stage::CHoleCtrl * holeCtrl;

	void (* endCb)(void *);
	void * endCbArg;

	int gameoverFrameCnt;

	TASK * gameoverAnime;
	
}; // CDead
	 
} // namespece stage

} // namespace yb
} // namespace nn
	

#endif // __DEAD_H__
