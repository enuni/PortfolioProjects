// CUP ��u��
// cup ��u��
// Cup ��u��
// yb ��u��
// stage ��u��

#ifndef __CUP_H__
#define __CUP_H__

#include "task_save.h"
#include "master.h"

// �O���錾
#include "class_list.h"

// enum
namespace nn {
namespace yb {

namespace e {
namespace cup {

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

} // namespece cup
} // namespece e

namespace stage {
	
class CCup : public TASK_SAVE {
	
public:
	CCup( TASK_SAVE * oya);
	virtual ~CCup();
	void task();
	void disp();
	
	void start();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;
	
	int x;
	int y;
	int r;
	
}; // CCup
	 
} // namespece stage
} // namespace yb
} // namespace nn
	

#endif // __CUP_H__
