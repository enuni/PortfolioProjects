// CAMERA を置換
// camera を置換
// Camera を置換
// yb を置換
// stage を置換

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
//---------------------------------------------------------------------------
// enum
namespace nn {
namespace yb {
namespace e {
namespace camera {

enum STATE {
	STATE_NONE,
	STATE_STANDBY,
	STATE_MOVE,
	
	STATE_MAX
};

} // namespece camera
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace stage {
	
class CCamera : public TASK_SAVE {
	
public:
	CCamera( TASK_SAVE * oya);
	virtual ~CCamera();
	void task();
	void disp();
	
	void start();

	void setCameraPos( int x, int y);
	void moveCameraPos( int x, int y);
	void moveCameraPos( int x, int y, void (* cb)(void *), void * cbArg);
	void moveCameraPos( int x, int y, int speed, void (* cb)(void *), void * cbArg);

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;
	
	int x;
	int y;
	int xDst;
	int yDst;
	
	int vx;
	int vy;
	int dx;
	int dy;
	int diff;

	int speed;

	void (* moveEndCb)( void *);
	void * moveEndCbArg;
	
	
}; // CCamera
	 
} // namespece stage

} // namespace yb
} // namespace nn
	

#endif // __CAMERA_H__
