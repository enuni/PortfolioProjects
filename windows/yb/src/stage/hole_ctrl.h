// HOLE_CTRL を置換
// hole_ctrl を置換
// HoleCtrl を置換
// yb を置換
// stage を置換

#ifndef __HOLE_CTRL_H__
#define __HOLE_CTRL_H__

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
namespace hole_ctrl {

enum STATE {
	STATE_NONE,
	STATE_OPENING,
	STATE_STANDBY,
	STATE_MAP,
	STATE_VERTICAL_SPIN,
	STATE_POWER_STANDBY,
	STATE_POWER,
	STATE_SHOT,
	STATE_FLYING,
	STATE_CUPIN,
	STATE_PAUSE,
	STATE_DEAD,
	
	STATE_MAX
};

enum STATUS_WINDOW_STATE {
	STATUS_WINDOW_STATE_IN,
	STATUS_WINDOW_STATE_OUT,

	STATUS_WINDOW_STATE_MAX
};

enum CUPIN_STATE {
	CUPIN_STATE_NONE,
	CUPIN_STATE_CUPIN_DISP,
	CUPIN_STATE_HOLE_IN_ONE_DISP,
	CUPIN_STATE_RESULT_DISP,
	
	CUPIN_STATE_MAX
};

enum HIO_STATE {
	HIO_STATE_NONE,
	HIO_STATE_WAIT,
	HIO_STATE_PANPANPAN,
	HIO_STATE_TEXT,
	HIO_STATE_STAR,
	HIO_STATE_OVATION,
	HIO_STATE_1UP,

	HIO_STATE_MAX
};

enum TIME_ATTACK_STATE {
	TIME_ATTACK_STATE_NONE,
	TIME_ATTACK_STATE_ACTIVE,
	TIME_ATTACK_STATE_PAUSE,
	TIME_ATTACK_STATE_END,

	TIME_ATTACK_STATE_MAX
};

} // namespece hole_ctrl
} // namespece e
//---------------------------------------------------------------------------

// classs
namespace stage {
	
class CHoleCtrl : public TASK_SAVE {
	
public:
	CHoleCtrl( TASK_SAVE * oya);
	virtual ~CHoleCtrl();
	void task();
	void disp();
	
	void start();
	
	void taskStandby();
	void taskMap();
	void taskPowerStandby();
	void taskPower();
	void taskCupin();

	void ballFlyingEnd();
	static void ballFlyingEndCb( void * holeCtrlArg);

	void nextCupIn();
	static void nextCupInCb( void * holeCtrlArg);
	

	static void pauseEndCb( void * holeCtrlArg);
	void pauseEnd();

	void taskVerticalSpin();

	static void openingCheckCb( void * holeCtrlArg);
	void openingCheck();
	void opening();
	static void openingEndCb( void * holeCtrlArg);
	void openingEnd();

	static void deadEndCb( void * holeCtrlArg);
	void deadEnd();

	void startStateStandby();	// 次のショットの開始

	void calcCuideCamera();

	static void shotCb( void * holeCtrlArg);
	void shot();
	void taskOpening();

	void setSpell( int spell);

	static void spellGetHitCardCb( void * holeCtrlArg);
	void spellGetHitCard();
	static void spellGetHitKeyCb( void * holeCtrlArg);
	void spellGetHitKey();

	bool taskDebug();
	
	static void restartHoleCb( void * holeCtrlArg);
	void restartHole();

	static void taskParticleCb( void * particleArg, void * holeCtrlArg);
	void taskParticle( void * particleArg);
	static void dispParticleCb( void * particleArg, void * holeCtrlArg);
	void dispParticle( void * particleArg);
	static void particleEndCb( void * holeCtrlArg);
	void particleEnd();
	void nextHio();
	void startHoleResult();
	void createHioStar( int n, double x, double y);
	void taskCupinHIO();
	void createAnimeHIOText();
	void createAnimeHIOSanae();
	void execHIO1up();
	void createHioStarSkip( int n, double x, double y);

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	nn::yb::main::CStage * stage;
	nn::yb::main::CBall * ball;
	nn::yb::stage::CHole * hole;
	
	double powerVec;

	int xCamera;
	int yCamera;

	bool leftPulseFlag;
	bool rightPuseFlag;

	int mapCtrlCnt;

	int shotCnt;

	int stateCupIn;

	nn::yb::main::CHoleResult * holeResult;
	// 後で使う
	//nn::yb::common::CYbSaveManager * saveManager;

	double ballVecGuideNextDispStart;
	int cursorRotHrizon;
	int cursorRotVertical;

	int stopSpinVertical;
	int stopSpinVerticalVec;
	int stopSpinVerticalWaitCnt;

	nn::yb::main::CCtrl * ctrl;

	int xVecGuideEnd;
	int yVecGuideEnd;
	int xVecGuideEndOld;
	int yVecGuideEndOld;
	
	//effect
	nn::yb::main::CEffectCtrl* effect_ctrl;
	static void effectCb( void * holeCtrlArg, void * dummy);
	unsigned int ctrl_cnt;//effect個数カウンタ

	nn::yb::main::CPause * pause;

	unsigned int frameCnt;

	int statusWindowState;
	int statusWindowY;
	unsigned int noInputCnt;

	int openingEndCnt;

	int nowBgm;
	int stageBgm;

	bool openingSkipFlag;

	int dispSpell;

	bool statusDispFlag;

	int spinWindowState;
	int spinWindowY;
	int noSpinCnt;

	bool debugPowerFixFlag;
	double debugPowerFixPower;
	int startSpell;

	int stateHio;
	int frameCntCupin;
	TASK_SAVE * animeHioRoot;
	TASK_SAVE * particleHioRoot;
	int paticleCntHio;
	TASK_SAVE * animeHIOSanae;
	TASK_SAVE * animeHIO1up;
	TASK_SAVE * animeCupin;

	int timeAttackState;
	int timeAttackOldTime;
	int timeAttackSumTime;

}; // CHoleCtrl
	 
} // namespece stage

} // namespace yb
} // namespace nn
	

#endif // __HOLE_CTRL_H__
