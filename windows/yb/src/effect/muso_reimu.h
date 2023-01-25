// MUSO_REIMU ��u��
// muso_reimu ��u��
// MusoReimu ��u��
// yb ��u��
// effect ��u��

#ifndef __MUSO_REIMU_H__
#define __MUSO_REIMU_H__

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
namespace effect {
namespace muso_reimu {

enum STATE {
	STATE_NONE,
	STATE_IN,
	STATE_SETUP,
	STATE_HANDS_UP,
	STATE_FIRE,
	STATE_FIRE_STOP,
	STATE_HANDS_DOWN,
	STATE_END,
	STATE_OUT,
	
	STATE_MAX
};

} // namespece muso_reimu
} // namespece effect
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace effect {
	
class CMusoReimu : public TASK_SAVE {
	
public:
	CMusoReimu( TASK_SAVE * oya);
	CMusoReimu( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CMusoReimu();
	void constructorCommon();
	void task();
	void disp();
	
	void start();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	int frameCnt;
	int x;
	int y;
	int dispZ;
	int objIdReimuTop;
	int objIdYukkuri;
	int fireScene;
	
	void (* endCb)( void *);
	void * endCbArg;
	
}; // CMusoReimu
	 
} // namespece effect

} // namespace yb
} // namespace nn
	

#endif // __MUSO_REIMU_H__
