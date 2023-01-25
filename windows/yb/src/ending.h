// ENDING ��u��
// ending ��u��
// Ending ��u��
// yb ��u��
// main ��u��

#ifndef __ENDING_H__
#define __ENDING_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// �O���錾
namespace nn {
namespace yb {

namespace main {

} // namespece main

} // namespace yb
} // namespace nn

namespace nn {
namespace yb {
//---------------------------------------------------------------------------
// enum
namespace e {
namespace ending {

enum STATE {
	STATE_NONE,
	STATE_START,
	STATE_STAFF_ROLL,
	STATE_FIN,
	STATE_END,
	
	STATE_MAX
};

} // namespece ending

namespace staff_roll {

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

} // namespece staff_roll
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace main {
	
class CEnding : public TASK_SAVE {
	
public:
	CEnding( TASK_SAVE * oya);
	virtual ~CEnding();
	void task();
	void disp();
	
	void start();

	static void nextCb( void * endingArg);
	void next();

	void (* endCb)( void *);
	void * endCbArg;

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	int frameCnt;
	int lineCnt;

	int allTotalScore;
	int yukkuriAlpha;
	
}; // CEnding

class CStaffRoll : public TASK_SAVE {
	
public:
	CStaffRoll( TASK_SAVE * oya);
	virtual ~CStaffRoll();
	void task();
	void disp();
	
	void start();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	char * text;
	int y;
	void (* endCb)( void *);
	void * endCbArg;
	int cnt;
	
}; // CStaffRoll
	 
} // namespece main

} // namespace yb
} // namespace nn
	

#endif // __ENDING_H__
