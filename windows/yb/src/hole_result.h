// HOLE_RESULT を置換
// hole_result を置換
// HoleResult を置換
// yb を置換
// main を置換

#ifndef __HOLE_RESULT_H__
#define __HOLE_RESULT_H__

#include "task_save.h"
#include "master.h"

// 前方宣言
#include "class_list.h"

// enum
namespace nn {
namespace yb {

namespace e {
namespace hole_result {

enum SELECT {
	SELECT_NEXT,
	SELECT_BREAK,

	SELECT_MAX
};

enum STATE {
	STATE_NONE,
	STATE_SLIDE_IN,
	STATE_SHOT_N_CNT_UP,
	STATE_BEST_SCORE,
	STATE_SCORE_LIST,
	STATE_SCORE_TOTAL,
	STATE_SELECT,
	
	STATE_MAX
};

} // namespece hole_result
} // namespece e

namespace main {
	
class CHoleResult : public TASK_SAVE {
	
public:
	CHoleResult( TASK_SAVE * oya);
	virtual ~CHoleResult();
	void task();
	void disp();
	
	void start();

	static void nextState( void * holeResult, int state);

	void checkBestScoreRenew();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	int slideInCnt;
	int dispShotN;
	bool bestScoreRenewFlag;
	int dispShotNCntUpCnt;

	int cursorX;

	int dispShiftX;

	nn::yb::stage::CHoleCtrl * holeCtrl;
	
	void (* endCb)( void *);
	void * endCbArg;
	
}; // CHoleResult
	 
} // namespece main
} // namespace yb
} // namespace nn
	

#endif // __HOLE_RESULT_H__
