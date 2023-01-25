// STAGE_RESULT を置換
// stage_result を置換
// StageResult を置換
// yb を置換
// main を置換

#ifndef __STAGE_RESULT_H__
#define __STAGE_RESULT_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"

namespace nn {
namespace yb {
//---------------------------------------------------------------------------
// enum
namespace e {
namespace stage_result {

enum STATE {
	STATE_NONE,
	STATE_FADE_IN,
	STATE_IN_WAIT,
	STATE_IN,
	STATE_SCORE_LIST_IN,
	STATE_SCORE_LIST,
	STATE_TOTAL,
	STATE_RENEW,
	STATE_WAIT,
	STATE_OUT,
	
	STATE_MAX
};

} // namespece stage_result
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace main {
	
class CStageResult : public TASK_SAVE {
	
public:
	CStageResult( TASK_SAVE * oya);
	virtual ~CStageResult();
	void task();
	void disp();
	
	void start();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	static void nextStateCb( void * stageResultArg);
	void nextState();
	
	void (* endCb)( void *);
	void * endCbArg;

	nn::yb::main::CStage * stage;

	int scoreListDispCnt;
	bool totalScoreRenewFlag;
	
	nn::yb::main::CEffectCtrl* effect_ctrl;

	int textInFrameCnt;
	int slideInCnt;
	unsigned int waitFrameCnt;
	
}; // CStageResult
	 
} // namespece main

} // namespace yb
} // namespace nn
	

#endif // __STAGE_RESULT_H__
