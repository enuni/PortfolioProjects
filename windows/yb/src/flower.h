// FLOWER を置換
// flower を置換
// Flower を置換
// yb を置換
// main を置換

#ifndef __FLOWER_H__
#define __FLOWER_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
//---------------------------------------------------------------------------
namespace nn {
namespace yb {

// enum
namespace e {
namespace flower {

enum STATE {
	STATE_NONE,
	STATE_MOVE_HIDE,
	STATE_MOVE_NORMAL,
	
	STATE_MAX
};

} // namespece flower
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace main {
	
class CFlower : public TASK_SAVE {
	
public:
	CFlower( TASK_SAVE * oya);
	CFlower( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CFlower();
	void constructorCommon();
	void task();
	void disp();
	
	void start();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	int x;
	int y;
	int gx;
	int gy;

	int vx;
	int vy;
	int dx;
	int dy;
	int diff;

	int objId;
	
}; // CFlower
	 
} // namespece main

} // namespace yb
} // namespace nn
	

#endif // __FLOWER_H__
