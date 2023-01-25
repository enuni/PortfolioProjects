// STAGE を置換
// stage を置換
// Stage を置換
// yb を置換
// main を置換

#ifndef __STAGE_H__
#define __STAGE_H__

#include "task_save.h"
#include "master.h"
#include "nn_common2.h"

// 前方宣言
#include "class_list.h"

// enum
namespace nn {
namespace yb {

namespace e {
namespace stage {

enum STATE {
	STATE_NONE,
	STATE_LOAD,
	STATE_HOLE,
	STATE_RESULT,
	
	STATE_MAX
};

} // namespece stage
} // namespece e

namespace main {
	
class CStage : public TASK_SAVE {
	
public:
	CStage( TASK_SAVE * oya);
	virtual ~CStage();
	void task();
	void disp();
	
	void start();
	void nextHole();
	void returnEdit();

	void breakGame();
	void resumeGame();

	static void stageResultEndCb( void * stageArg);
	void stageResultEnd();
	void saveHoleScoreRenew();
	void saveStageScoreRenew();
	
	static void endingEndCb( void * stageArg);
	void endingEnd();

	void setHp( int argHp);
	int getHp();
	void damage( int damage);

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	nn::yb::stage::CHoleCtrl * holeCtrl;

	int holeCnt;

	// ゲームパラメータ
	int score[8];
	int totalScore;
	
	int lp;
	int spell;

	int stageNo;

	bool editFlag;
	nn::yb::main::CCtrl * ctrl;
	nn::yb::edit::CDataHole * dataHole;

	nn::yb::main::CSaveData * saveData;
	nn::yb::main::CEffectCtrl* effect_ctrl;
	int frameCnt;

private:
	int hp;
	
}; // CStage

} // namespece main
} // namespace yb
} // namespace nn
	

#endif // __STAGE_H__
