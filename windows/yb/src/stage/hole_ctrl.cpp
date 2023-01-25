// hole_ctrl を置換
// HoleCtrl を置換
// yb を置換
// stage を置換

#include <math.h>

#include "nn_common.h"
#include "nn_timer.h"

#include "disp_common.h"
#include "animation.h"
#include "sound_nns.h"
#include "direct_common.h"
#include "anime.h"
#include "particle.h"

#include "sin_cos.h"

#include "enum.h"
#include "ball.h"
#include "hole.h"
#include "hole_result.h"
#include "stage.h"
#include "camera.h"
#include "ctrl.h"
#include "menu.h"
#include "dead.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "animation_data.h"
#include "sound_data.sadl"

#include "cup.h"

#include "enemy.h"
#include "effect\\effect_ctrl.h"
#include "effect\\effect.h"
#include "effect\\musofuin.h"
#include "effect\\effect_star.h"
#include "font.h"

#include "pause.h"
#include "yb_common.h"

#include "spell_get.h"
#include "edit_hole_obj.h"

#include "hole_ctrl.h"
//---------------------------------------------------------------------------
nn::yb::stage::CHoleCtrl::CHoleCtrl(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CHoleCtrl Create\n");
	this->state = nn::yb::e::hole_ctrl::STATE_NONE;
	this->oldState = nn::yb::e::hole_ctrl::STATE_NONE;
	this->waitCnt = 0;

	this->master = NULL;

	this->stage = NULL;
	this->ball =NULL;
	this->hole = NULL;

	this->powerVec = 0.0;

	this->xCamera = 0;
	this->yCamera = 0;

	this->leftPulseFlag = false;
	this->rightPuseFlag = false;

	this->mapCtrlCnt = 0;

	this->shotCnt = 0;

	this->stateCupIn = 0;

	this->holeResult = NULL;

	this->ballVecGuideNextDispStart = 0.0;
	this->cursorRotHrizon = 0;
	this->cursorRotVertical = 0;

	this->ctrl = NULL;
	
	this->xVecGuideEnd = 0;
	this->yVecGuideEnd = 0;
	this->xVecGuideEndOld = 0;
	this->yVecGuideEndOld = 0;

	//effect
	this->effect_ctrl = NULL; 
	this->ctrl_cnt = 0;

	this->pause = NULL;

	this->frameCnt = 0;

	this->stopSpinVertical = 0;
	this->stopSpinVerticalVec = 1;
	this->stopSpinVerticalWaitCnt = 0;

	this->statusWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
	this->statusWindowY = 0;
	this->noInputCnt = 0;

	this->openingEndCnt = 0;

	this->nowBgm = 0;
	this->stageBgm = 0;

	this->openingSkipFlag = false;

	this->statusDispFlag = true;

	this->spinWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
	this->spinWindowY = 0;
	this->noSpinCnt = 0;

	this->debugPowerFixFlag = false;
	this->debugPowerFixPower = nn::yb::e::global::SHOT_BASE_MAX_POWER;

	this->startSpell = nn::yb::e::global::SPELL_CARD_NONE;

	this->stateHio = 0;
	this->frameCntCupin = 0;
	this->animeHioRoot = NULL;
	this->particleHioRoot = NULL;
	this->paticleCntHio = 0;
	this->animeHIOSanae = NULL;
	this->animeHIO1up = NULL;
	this->animeCupin = NULL;

	this->timeAttackState = nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_NONE;
	this->timeAttackOldTime = 0;
	this->timeAttackSumTime = 0;

}
//---------------------------------------------------------------------------
nn::yb::stage::CHoleCtrl::~CHoleCtrl() {

	::nn_commonLog( 1, "CHoleCtrl Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::start() {

	this->powerVec = 1.0;

	this->ballVecGuideNextDispStart = nn::yb::e::global::BALL_VEC_GUIDE_DISP_START;

	this->ball = new nn::yb::main::CBall( this);
	this->ball->master = this->master;
	this->ball->holeCtrl = this;
	this->ball->flyingEndCb = nn::yb::stage::CHoleCtrl::ballFlyingEndCb;
	this->ball->flyingEndCbArg = this;
	this->ball->start();

	this->hole = new nn::yb::stage::CHole( this);
	this->hole->master = this->master;
	this->hole->saveManager = this->ctrl->saveManagerEdit;
	this->hole->holeCtrl = this;
	this->hole->start();
	
	//effect
	// stage.cppでnewするよう変更
	//this->effect_ctrl = new nn::yb::main::CEffectCtrl( this); 
	this->ctrl_cnt = 0;
	
	this->state = nn::yb::e::hole_ctrl::STATE_OPENING;

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::opening() {
	
	switch(this->stage->stageNo) {
		case 1:
		case 2:
			this->nowBgm = BGM_STAGE01;
			break;
		case 3:
		case 4:
			this->nowBgm = BGM_STAGE03;
			break;
		case 5:
		case 6:
			this->nowBgm = BGM_STAGE05;
			break;
		case 7:
		case 8:
			break;
	}
	this->stageBgm = this->nowBgm;
	nn::yb::common::yb_commonPlaySeq( this->nowBgm);

	if(this->hole->openingCnt >= 1) {
		this->ctrl->camera->setCameraPos( this->hole->openingX[this->openingEndCnt], this->hole->openingY[this->openingEndCnt]);
		this->openingCheck();
		this->state = nn::yb::e::hole_ctrl::STATE_OPENING;
	}
	else {
		this->openingEnd();
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 1, "CHoleCtrl::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	this->frameCnt++;

	::KEY_WORK * key = &this->master->keyWork;
	
	if(key->press) {
		this->noInputCnt = 0;
	}
	else {
		this->noInputCnt++;
	}
	if(this->cursorRotHrizon != 0 ||
		(this->ball->flyState == nn::yb::e::ball::FLY_STATE_ON && this->cursorRotVertical != 0)
	) {
		this->noSpinCnt = 0;
	}
	else {
		this->noSpinCnt++;
	}

	// ステータスウィンドウ
	if(this->state == nn::yb::e::hole_ctrl::STATE_MAP) {
		if(this->noInputCnt >= 45) {
			this->statusWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_OUT;
		}
		else {
			this->statusWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
		}
	}
	else if(this->state == nn::yb::e::hole_ctrl::STATE_VERTICAL_SPIN ||
		this->state == nn::yb::e::hole_ctrl::STATE_POWER ||
		this->state == nn::yb::e::hole_ctrl::STATE_SHOT
	) {
		this->statusWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
	}
	else if(this->statusDispFlag) {
		this->statusWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
	}
	else {
		this->statusWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_OUT;
	}

	if(this->statusWindowState == nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_OUT) {
		this->statusWindowY += 4;
		if(this->statusWindowY > 128) {
			this->statusWindowY = 128;
		}
	}
	else if(this->statusWindowState == nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN) {
		this->statusWindowY -= 8;
		if(this->statusWindowY < 0) {
			this->statusWindowY = 0;
		}
	}

	// 回転のカーソル
	if(this->state == nn::yb::e::hole_ctrl::STATE_MAP) {
		if(this->noInputCnt >= 45) {
			this->spinWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_OUT;
		}
		else {
			this->spinWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
		}
	}
	else if(this->state == nn::yb::e::hole_ctrl::STATE_VERTICAL_SPIN ||
		this->state == nn::yb::e::hole_ctrl::STATE_POWER ||
		this->state == nn::yb::e::hole_ctrl::STATE_SHOT
	) {
		this->spinWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
	}
	else if(this->statusDispFlag) {
		this->spinWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
	}
	else {
		if(this->cursorRotHrizon != 0 ||
			(this->ball->flyState == nn::yb::e::ball::FLY_STATE_ON && this->cursorRotVertical != 0)
		) {
			this->spinWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
		}
		else {
			if(this->noSpinCnt >= 45) {
				this->spinWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_OUT;
			}
			else {
				this->spinWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
			}
		}
	}

	if(this->spinWindowState == nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_OUT) {
		this->spinWindowY += 4;
		if(this->spinWindowY > 128) {
			this->spinWindowY = 128;
		}
	}
	else if(this->spinWindowState == nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN) {
		this->spinWindowY -= 8;
		if(this->spinWindowY < 0) {
			this->spinWindowY = 0;
		}
	}

	if(key->trigger & nn::common::e::PAD_WIN_BUTTON_L1) {
		this->statusDispFlag = !this->statusDispFlag;
	}

	// 表情
	if(this->state == nn::yb::e::hole_ctrl::STATE_STANDBY ||
		this->state == nn::yb::e::hole_ctrl::STATE_MAP
	) {
		if(this->ball->dispState != nn::yb::e::ball::DISP_STATE_CRY) {
			if((this->noInputCnt % 180) == 179) {
				this->ball->setDispState( nn::yb::e::ball::DISP_STATE_JOY, 120);
			}
			else if(this->noInputCnt == 0) {
				this->ball->setDispState( nn::yb::e::ball::DISP_STATE_DEFAULT);
			}
		}
	}
	else if(this->state == nn::yb::e::hole_ctrl::STATE_VERTICAL_SPIN) {
		if(this->ball->dispState != nn::yb::e::ball::DISP_STATE_CRY) {
			if((this->noInputCnt % 180) == 179) {
				this->ball->setDispState( nn::yb::e::ball::DISP_STATE_CRY, 120);
			}
			else if(this->noInputCnt == 0) {
				this->ball->setDispState( nn::yb::e::ball::DISP_STATE_DEFAULT);
			}
		}
	}

	//	タイムアタック
	if(this->timeAttackState == nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_ACTIVE) {
		int now_time = nn_timerGetMilliTime();
		this->timeAttackSumTime += now_time - this->timeAttackOldTime;
		this->timeAttackOldTime = now_time;
	}

	switch(this->state) {
		case nn::yb::e::hole_ctrl::STATE_NONE:
			break;
		case nn::yb::e::hole_ctrl::STATE_OPENING:
			this->taskOpening();
			break;
		case nn::yb::e::hole_ctrl::STATE_STANDBY:
			this->taskStandby();
			break;
		case nn::yb::e::hole_ctrl::STATE_MAP:
			this->taskMap();
			break;
		case nn::yb::e::hole_ctrl::STATE_VERTICAL_SPIN:
			this->taskVerticalSpin();
			break;
		case nn::yb::e::hole_ctrl::STATE_POWER:
			this->taskPower();
			break;
		case nn::yb::e::hole_ctrl::STATE_FLYING:

			break;
		case nn::yb::e::hole_ctrl::STATE_CUPIN:
			this->taskCupin();
			break;
		case nn::yb::e::hole_ctrl::STATE_PAUSE:
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::createAnimeHIOText() {

	this->animeHioRoot = nn::common::dxlib::CAnime::exec( this, ANIMATION_ID_HOLE_IN_ONE_TEXT,
		320, 240, -30, ::ANIME_FLAGS_LAST_WAIT, NULL, NULL
	);

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::createAnimeHIOSanae() {

	this->animeHIOSanae = nn::common::dxlib::CAnime::exec( this->animeHioRoot, ANIMATION_ID_HOLE_IN_ONE_SANAE,
		320, 240, -48, ::ANIME_FLAGS_LAST_WAIT, nn::yb::stage::CHoleCtrl::nextCupInCb, this
	);

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::execHIO1up() {

	this->animeHIO1up = nn::common::dxlib::CAnime::exec( this->animeHioRoot, ANIMATION_ID_HOLE_IN_ONE_1UP,
		320, 240, -130, 0, nn::yb::stage::CHoleCtrl::nextCupInCb, this
	);
	if(this->stage->lp < 99) {
		this->stage->lp++;
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::taskCupinHIO() {
	
	::KEY_WORK * key = &this->master->keyWork;

	if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
		// スキップ
		if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_PANPANPAN) {		
			if(this->frameCntCupin <= 30) {
				this->createHioStarSkip( 30, 440.0, -700.0);
				this->createHioStarSkip( 40, 320.0, -1000.0);
			}
			else if(this->frameCntCupin <= 60) {
				this->createHioStarSkip( 40, 320.0, -700.0);
			}
			this->createAnimeHIOText();
			this->frameCntCupin = 0;
			this->stateHio = nn::yb::e::hole_ctrl::HIO_STATE_STAR;
		}
		if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_TEXT) {
			this->createAnimeHIOText();
			this->stateHio = nn::yb::e::hole_ctrl::HIO_STATE_STAR;
		}
		if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_STAR &&
			this->frameCntCupin < 140 - 20
		) {
			if(((nn::common::dxlib::CAnime *)(this->animeHioRoot))->frameCnt < 30) {
				sound_nnsPlaySe( SE_HIO);
			}
			this->frameCntCupin = 140 - 20;
			((nn::common::dxlib::CAnime *)(this->animeHioRoot))->end();
		}
		if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_OVATION) {
			((nn::common::dxlib::CAnime *)(this->animeHIOSanae))->endCallBackFunc = NULL;
			((nn::common::dxlib::CAnime *)(this->animeHIOSanae))->endCallBackFuncArg = NULL;
			((nn::common::dxlib::CAnime *)(this->animeHIOSanae))->end();
			this->execHIO1up();
			sound_nnsPlaySe( SE_THIRRORIRORIN);
			this->stateHio = nn::yb::e::hole_ctrl::HIO_STATE_1UP;
		}
		if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_1UP &&
			((nn::common::dxlib::CAnime *)(this->animeHIO1up))->frameCnt < 60
		) {
			((nn::common::dxlib::CAnime *)(this->animeHIO1up))->frameCnt = 60;
		}
	}

	if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_PANPANPAN) {
		if(this->frameCntCupin == 30) {
			this->createHioStar( 30, 440.0, 220.0);
		}
		else if(this->frameCntCupin == 60) {
			this->createHioStar( 40, 320.0, 260.0);
		}

		if(this->frameCntCupin >= 70) {
			this->nextHio();
		}
		else {
			this->frameCntCupin++;
		}
	}
	else if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_TEXT) {
		if(this->frameCntCupin >= 135) {
			this->nextHio();
		}
		else {
			this->frameCntCupin++;
		}
	}
	else if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_STAR) {
		if(this->frameCntCupin >= 140) {
			this->nextHio();
		}
		else {
			this->frameCntCupin++;
		}
	}
	else if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_OVATION) {
	}
	else if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_1UP) {
	}

#if 0
	if((this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_STAR && this->frameCntCupin >= 50) ||
		this->stateHio >= nn::yb::e::hole_ctrl::HIO_STATE_OVATION
	) {
		// 星の落ちていく音
		int se = ::n_commonRandN( 30);
		if(se == 0) {
			sound_nnsPlaySe( SE_RARA);
		}
	}
#endif

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::taskCupin() {

	::KEY_WORK * key = &this->master->keyWork;

	if(this->stateCupIn == nn::yb::e::hole_ctrl::CUPIN_STATE_HOLE_IN_ONE_DISP) {
		this->taskCupinHIO();
	}
	else if(this->stateCupIn == nn::yb::e::hole_ctrl::CUPIN_STATE_CUPIN_DISP) {
		if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
			if(((nn::common::dxlib::CAnime *)(this->animeCupin))->frameCnt < 193) {
				((nn::common::dxlib::CAnime *)(this->animeCupin))->frameCnt = 193;
			}
		}

#if 0
		if(this->frameCntCupin == 223) {
			//☆エフェクトの表示
			for (int i = 0; i < 2; i++)
			{
				/*
				nn::yb::main::effect_star(this, 60, this->ball->x, this->ball->y, this->ball->x, this->ball->y, 128, 0, 20, 3.0f,
					this->effect_ctrl);//
				*/
				nn::yb::main::effect_star(this, 60, 280 + 80 * i, 280, 320, 280, 128, 0, 20, 3.0f,
					this->effect_ctrl);//
			}
		}
		this->frameCntCupin++;
#endif
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::particleEndCb( void * holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->particleEnd();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::particleEnd() {

	this->paticleCntHio--;
	if(this->paticleCntHio == 0 && this->particleHioRoot != NULL) {
		// パーティクルが全て消えたタイミングで処理をしたい場合
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::createHioStarSkip( int n, double x, double y) {

	for( int i = 0; i < n; i++) {

		nn::common::graphics2d::CParticle * p = new nn::common::graphics2d::CParticle( this->particleHioRoot);
		p->objId = OBJ_ID_MAIN2_HOLE_IN_ONE_0 +
			::n_commonRandN(OBJ_ID_MAIN2_HOLE_IN_ONE_7 + 1 -
			OBJ_ID_MAIN2_HOLE_IN_ONE_0);
		p->x = x - 250.0 + ::n_commonRandN( 500);
		p->y = y - 600.0 + ::n_commonRandN( 1200);
		p->z = -120;
		p->angle = 270.0;
		p->speed = 2.0;
		p->angleSpeed = 0.0;
		p->addSpeed = 0.025;
		p->addAngleSpeed = 0.0;
		p->rotate = p->angle;
		p->rotateSpeed = -5.0 + ::n_commonRandN( 6) * 2.0;
		p->w = 0;
		p->h = 0;
		p->wSpeed = 0;
		p->hSpeed = 0;
		p->taskCb = nn::yb::stage::CHoleCtrl::taskParticleCb;
		p->taskCbArg = this;
		p->dispCb = nn::yb::stage::CHoleCtrl::dispParticleCb;
		p->dispCbArg = this;
		p->endCb = nn::yb::stage::CHoleCtrl::particleEndCb;
		p->endCbArg = this;

		p->start();

		this->paticleCntHio++;

	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::createHioStar( int n, double x, double y) {

	sound_nnsPlaySe( SE_PAN);
	for( int i = 0; i < n; i++) {

		nn::common::graphics2d::CParticle * p = new nn::common::graphics2d::CParticle( this->particleHioRoot);
		p->objId = OBJ_ID_MAIN2_HOLE_IN_ONE_0 +
			::n_commonRandN(OBJ_ID_MAIN2_HOLE_IN_ONE_7 + 1 -
			OBJ_ID_MAIN2_HOLE_IN_ONE_0);
		p->x = x;
		p->y = y;
		p->z = -120;
		p->angle = 60.0 + ::n_commonRandN(60);
		p->speed = 33.0 + ::n_commonRandN(25);
		p->angleSpeed = 0.0;
		p->addSpeed = -1.0;
		p->addAngleSpeed = 0.0;
		p->rotate = p->angle;
		p->rotateSpeed = 0.0;
		p->w = 32;
		p->h = 32;
		p->wSpeed = 0;
		p->hSpeed = 0;
		p->taskCb = nn::yb::stage::CHoleCtrl::taskParticleCb;
		p->taskCbArg = this;
		p->dispCb = nn::yb::stage::CHoleCtrl::dispParticleCb;
		p->dispCbArg = this;
		p->endCb = nn::yb::stage::CHoleCtrl::particleEndCb;
		p->endCbArg = this; 

		p->start();

		this->paticleCntHio++;

	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::nextHio() {

	if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_WAIT) {
		this->paticleCntHio = 0;
		this->particleHioRoot = new nn::common::graphics2d::CParticle( this);
		this->createHioStar( 30, 200.0, 240.0);
		this->stateHio = nn::yb::e::hole_ctrl::HIO_STATE_PANPANPAN;
	}
	else if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_PANPANPAN) {
		this->createAnimeHIOText();
		this->frameCntCupin = 0;
		this->stateHio = nn::yb::e::hole_ctrl::HIO_STATE_STAR;
	}
	else if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_TEXT) {
		this->frameCntCupin = 0;
		this->stateHio = nn::yb::e::hole_ctrl::HIO_STATE_STAR;
	}
	else if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_STAR) {
		this->createAnimeHIOSanae();
		this->frameCntCupin = 0;
		this->stateHio = nn::yb::e::hole_ctrl::HIO_STATE_OVATION;
	}
	else if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_OVATION) {
		this->execHIO1up();
		this->stateHio = nn::yb::e::hole_ctrl::HIO_STATE_1UP;
	}
	else if(this->stateHio == nn::yb::e::hole_ctrl::HIO_STATE_1UP) {
		this->particleHioRoot->taskDelete();
		this->particleHioRoot = NULL;
		this->startHoleResult();
		this->stateHio = nn::yb::e::hole_ctrl::HIO_STATE_NONE;
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::startHoleResult() {

	nn::yb::main::CHoleResult * hole_result = new nn::yb::main::CHoleResult( this);
	hole_result->holeCtrl = this;
	hole_result->master = this->master;
	hole_result->endCb = nn::yb::stage::CHoleCtrl::nextCupInCb;
	hole_result->endCbArg = this;
	hole_result->start();
	this->holeResult = hole_result;
	this->stateCupIn = nn::yb::e::hole_ctrl::CUPIN_STATE_RESULT_DISP;

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::nextCupIn() {

	if(this->stateCupIn == nn::yb::e::hole_ctrl::CUPIN_STATE_NONE) {
		sound_nnsPlaySe( SE_DODODODOOOON);
		nn::yb::common::yb_commonPlaySeq( BGM_CUPIN);
		if(this->shotCnt == 1) {
			direct_commonWaitSimple( NULL, 35, nn::yb::stage::CHoleCtrl::nextCupInCb, this);
			this->stateCupIn = nn::yb::e::hole_ctrl::CUPIN_STATE_HOLE_IN_ONE_DISP;
			this->stateHio = nn::yb::e::hole_ctrl::HIO_STATE_WAIT;
		}
		else {
			//☆エフェクトの表示
			for (int i = 0; i < 2; i++)
			{
#if 0
				nn::yb::main::effect_star(this, 60, this->ball->x, this->ball->y, this->ball->x, this->ball->y, 128, 0, 20, 3.0f,
					this->effect_ctrl);//
#endif
#if 0
				// 文字の回転位置に合わせる
				nn::yb::main::effect_star(this, 60, 240, 240, 320, 240, 128, 0, 20, 3.0f,
					this->effect_ctrl);//
#endif
				nn::yb::main::effect_star(this, 60, 320, 240, 320, 240, 128, 0, 20, 3.0f,
					this->effect_ctrl);//
			}

			this->animeCupin = nn::common::dxlib::CAnime::exec( this, ANIMATION_ID_CUPIN2,
				320, 240, -1, ::ANIME_FLAGS_LAST_WAIT, nn::yb::stage::CHoleCtrl::nextCupInCb, this
			);
			this->stateCupIn = nn::yb::e::hole_ctrl::CUPIN_STATE_CUPIN_DISP;
		}
	}
	else if(this->stateCupIn == nn::yb::e::hole_ctrl::CUPIN_STATE_CUPIN_DISP) {
		this->startHoleResult();
	}
	else if(this->stateCupIn == nn::yb::e::hole_ctrl::CUPIN_STATE_HOLE_IN_ONE_DISP) {
		this->nextHio();
	}
	else if(this->stateCupIn == nn::yb::e::hole_ctrl::CUPIN_STATE_RESULT_DISP) {
		this->holeResult->taskDelete();
		if(this->stage->getHp() < nn::yb::e::global::STAGE_HP_MAX) {
			this->stage->damage( +1);
		}
		if(this->stage->editFlag) {
			this->stage->returnEdit();
		}
		else {
			this->stage->score[this->stage->holeCnt - 1] = this->shotCnt;
			this->stage->totalScore += this->shotCnt;
			if(this->holeResult->bestScoreRenewFlag) {
				// セーブスコアの更新
				this->stage->saveHoleScoreRenew();
			}
			if(this->holeResult->cursorX == nn::yb::e::hole_result::SELECT_NEXT) {
				this->stage->nextHole();
				this->taskDelete();
			}
			else if(this->holeResult->cursorX == nn::yb::e::hole_result::SELECT_BREAK) {
				this->stage->breakGame();
			}
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::nextCupInCb( void * holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;
	
	hole_ctrl->nextCupIn();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::taskParticleCb( void * particleArg, void * holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->taskParticle( particleArg);

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::taskParticle( void * particleArg) {

	nn::common::graphics2d::CParticle * p = (nn::common::graphics2d::CParticle *) particleArg;

	double old_speed = p->speed;
	double move_x = nn::yb::common::yb_commonAngleSpeed2MoveX( p->angle, p->speed);
	double move_y = nn::yb::common::yb_commonAngleSpeed2MoveY( p->angle, p->speed);
	
	p->x += move_x;
	p->y += move_y;
	p->angle += p->angleSpeed;
	if(p->angle >= 360.0) {
		p->angle -= 360.0;
	}
	else if(p->angle < 0.0) {
		p->angle += 360.0;
	}
	p->rotate += p->rotateSpeed;
	if(p->rotate >= 360.0) {
		p->rotate -= 360.0;
	}
	else if(p->rotate < 0.0) {
		p->rotate += 360.0;
	}
	p->w += p->wSpeed;
	p->h += p->hSpeed;
	p->speed += p->addSpeed;
	p->angleSpeed += p->addAngleSpeed;

	if(old_speed > 0.0 && p->speed <= 0.0) {
		p->angle = 270.0;
		p->w = 0;
		p->h = 0;
		p->speed = 2.0;
		p->angleSpeed = 0.0;
		p->addSpeed = 0.025;
		p->addAngleSpeed = 0.0;
		p->rotateSpeed = -5.0 + ::n_commonRandN( 6) * 2.0;
	}

	if(p->y >= 1150.0) {
		p->taskDelete();
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::dispParticleCb( void * particleArg, void * holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->dispParticle( particleArg);

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::dispParticle( void * particleArg) {

	nn::common::graphics2d::CParticle * p = (nn::common::graphics2d::CParticle *) particleArg;

	::disp_commonObjDxCenterFull( p->objId, (int)p->x, (int)p->y, (int)p->z,
		(int)p->w, (int)p->h, (int)p->rotate, FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT,
		::DISP_COMMON_BLEND_MODE_NONE
	);

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::shotCb(void *holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->shot();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::shot() {

	this->ball->shot();

	if(this->timeAttackState == nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_PAUSE) {
		this->timeAttackOldTime = nn_timerGetMilliTime();
		this->timeAttackState = nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_ACTIVE;
	}

	this->state = nn::yb::e::hole_ctrl::STATE_FLYING;

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::taskPower() {

	::KEY_WORK * key = &this->master->keyWork;

	if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
		direct_commonWaitSimple( NULL, 20, shotCb, this);
		this->state = nn::yb::e::hole_ctrl::STATE_SHOT;
		/*
		this->ball->shot();
		this->state = nn::yb::e::hole_ctrl::STATE_FLYING;
		*/
	}
	else {
		if((int)this->ball->power % ((int)nn::yb::e::global::SHOT_BASE_MAX_POWER / 4) == 0) {
			if(this->powerVec > 0.0) {
				sound_nnsPlaySe( SE_VON);
			}
		}

		this->ball->power += this->powerVec;
		if(this->ball->power > nn::yb::e::global::SHOT_BASE_MAX_POWER) {
			this->ball->power = nn::yb::e::global::SHOT_BASE_MAX_POWER;
			this->powerVec = -this->powerVec;
		}
		else if(this->ball->power < 0.0) {
			this->ball->power = 0.0;
			this->powerVec = -this->powerVec;
		}
	}

	if(this->debugPowerFixFlag) {
		this->ball->power = this->debugPowerFixPower;
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::taskVerticalSpin() {

	::KEY_WORK * key = &this->master->keyWork;
	
	if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
		sound_nnsPlaySe( SE_CHURIN);
		this->ball->setDispState( nn::yb::e::ball::DISP_STATE_DEFAULT);
		this->state = nn::yb::e::hole_ctrl::STATE_POWER;
	}
	else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_B) {
		sound_nnsPlaySe( SE_DO);
		this->state = nn::yb::e::hole_ctrl::STATE_STANDBY;
		this->calcCuideCamera();
	}
	else {
		bool stop_move_flag = true;
		// 目標のスピンの所で少し停止
		// デフォルトのスピンの所で少し停止
		if(this->cursorRotVertical == 0 &&
			//this->cursorRotVertical * nn::yb::e::global::SHOT_SPIN_VERTICAL_MOVE_DIV
			0
			== this->stopSpinVertical
		) {
			if(this->stopSpinVerticalWaitCnt >= nn::yb::e::global::SHOT_SPIN_VERTICAL_WAIT_FRAME) {
				this->stopSpinVerticalWaitCnt = 0;
				stop_move_flag = true;
			}
			else {
				this->stopSpinVerticalWaitCnt ++;
				stop_move_flag = false;
			}
		}
		else {
			stop_move_flag = true;
		}

		if(stop_move_flag) {
			if(this->stopSpinVertical == nn::yb::e::global::SHOT_ROT_VERTICAL_MAX * nn::yb::e::global::SHOT_SPIN_VERTICAL_MOVE_DIV
				||
				this->stopSpinVertical == -nn::yb::e::global::SHOT_ROT_VERTICAL_MAX * nn::yb::e::global::SHOT_SPIN_VERTICAL_MOVE_DIV
			) {
				this->stopSpinVerticalVec = -this->stopSpinVerticalVec;
			}
			this->stopSpinVertical += this->stopSpinVerticalVec;
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::taskPowerStandby() {

	/*
	// 多分もう不要 taskVerticalSpin で代用可
	::KEY_WORK * key = &this->master->keyWork;

	if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
		sound_nnsPlaySe( SE_CHURIN);
		this->state = nn::yb::e::hole_ctrl::STATE_POWER;
	}
	else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_B) {
		sound_nnsPlaySe( SE_DO);
		this->state = nn::yb::e::hole_ctrl::STATE_STANDBY;
		this->ctrl->camera->moveCameraPos(
			this->xVecGuideEnd + ((int)this->ball->x - this->xVecGuideEnd) / 2,
			this->yVecGuideEnd + ((int)this->ball->y - this->yVecGuideEnd) / 2
		);
	}
	*/

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::taskMap() {

	::KEY_WORK * key = &this->master->keyWork;

	this->mapCtrlCnt = (this->mapCtrlCnt + 1) % 48;
	if( !(key->press & nn::common::e::PAD_WIN_BUTTON_Y) &&
		key->trigger & nn::common::e::PAD_WIN_BUTTON_A ) {
		sound_nnsPlaySe( SE_DEDO);
		this->state = nn::yb::e::hole_ctrl::STATE_STANDBY;
		// ガイド終点
		this->calcCuideCamera();
	}
	else if( !(key->press & nn::common::e::PAD_WIN_BUTTON_Y) &&
		key->trigger & nn::common::e::PAD_WIN_BUTTON_B ) {
		sound_nnsPlaySe( SE_DEDO);
		this->state = nn::yb::e::hole_ctrl::STATE_STANDBY;
		// ガイド終点
		this->calcCuideCamera();
	}
	else {
		int speed = 4;
		if(key->press & nn::common::e::PAD_WIN_BUTTON_Y) {
			speed = 8;
		}
		else {
			speed = 4;
		}
		if(key->press & nn::common::e::PAD_WIN_KEY_LEFT) {
			this->ctrl->camera->x -= speed;
			if(this->ctrl->camera->x < this->hole->startX + 320) {
				this->ctrl->camera->x = this->hole->startX + 320;
			}
			this->ctrl->camera->setCameraPos( this->ctrl->camera->x, this->ctrl->camera->y);
		}
		else if(key->press & nn::common::e::PAD_WIN_KEY_RIGHT) {
			this->ctrl->camera->x += speed;
			if(this->ctrl->camera->x > this->hole->endX - 320) {
				this->ctrl->camera->x = this->hole->endX - 320;
			}
			this->ctrl->camera->setCameraPos( this->ctrl->camera->x, this->ctrl->camera->y);
		}
		if(key->press & nn::common::e::PAD_WIN_KEY_UP) {
			this->ctrl->camera->y -= speed;
			if(this->ctrl->camera->y < this->hole->startY + 240) {
				this->ctrl->camera->y = this->hole->startY + 240;
			}
			this->ctrl->camera->setCameraPos( this->ctrl->camera->x, this->ctrl->camera->y);
		}
		else if(key->press & nn::common::e::PAD_WIN_KEY_DOWN) {
			this->ctrl->camera->y += speed;
			if(this->ctrl->camera->y > this->hole->endY - 240) {
				this->ctrl->camera->y = this->hole->endY - 240;
			}
			this->ctrl->camera->setCameraPos( this->ctrl->camera->x, this->ctrl->camera->y);
		}

	}

}
//---------------------------------------------------------------------------
bool nn::yb::stage::CHoleCtrl::taskDebug() {

	::KEY_WORK * key = &this->master->keyWork;
	::KEYBOARD_WORK * kb = &this->master->keyboardWork;

	bool proc_flag = false;

	if(kb->press[KEYBOARD_W]) {
		if((key->trigger & nn::common::e::PAD_UP) ||
			(key->pulse & nn::common::e::PAD_UP)
		) {
			this->debugPowerFixPower += 1.0;
			if(this->debugPowerFixPower > nn::yb::e::global::SHOT_BASE_MAX_POWER) {
				this->debugPowerFixPower -= nn::yb::e::global::SHOT_BASE_MAX_POWER;
			}
			proc_flag = true;
		}
		else if((key->trigger & nn::common::e::PAD_DOWN) ||
			(key->pulse & nn::common::e::PAD_DOWN)
		) {
			this->debugPowerFixPower -= 1.0;
			if(this->debugPowerFixPower < 0) {
				this->debugPowerFixPower += nn::yb::e::global::SHOT_BASE_MAX_POWER;
			}
			proc_flag = true;
		}
		else if((key->trigger & nn::common::e::PAD_LEFT) ||
			(key->pulse & nn::common::e::PAD_LEFT) ||
			(key->trigger & nn::common::e::PAD_RIGHT) ||
			(key->pulse & nn::common::e::PAD_RIGHT)
		) {
			this->debugPowerFixFlag = !this->debugPowerFixFlag;
			proc_flag = true;
		}
	}

	return proc_flag;

}
//---------------------------------------------------------------------------
#ifdef _DEBUG
double param = 0.0;
#endif
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::taskStandby() {

	::KEY_WORK * key = &this->master->keyWork;

#if defined(_DEBUG) || defined(__EDIT__)
	if(this->taskDebug()) {
		return;
	}
#endif

	if(this->xVecGuideEnd != this->xVecGuideEndOld || this->yVecGuideEnd != this->yVecGuideEndOld) {
		this->xVecGuideEndOld = this->xVecGuideEnd;
		this->yVecGuideEndOld = this->yVecGuideEnd;
		// ガイド終点
		this->calcCuideCamera();
	}

	if(key->press & nn::common::e::PAD_WIN_BUTTON_Y) {
		// 方向変更Yダッシュ
		if((key->trigger & nn::common::e::PAD_WIN_KEY_LEFT) || (key->pulse & nn::common::e::PAD_WIN_KEY_LEFT)) {
			sound_nnsPlaySe( SE_U);
			this->ball->angle += 1.0;
			this->ball->angle = double((((int)this->ball->angle + 44) / 45) * 45);
			if(this->ball->angle > 359.0) {
				this->ball->angle -= 360.0;
			}
		}
		else if((key->trigger & nn::common::e::PAD_WIN_KEY_RIGHT) || (key->pulse & nn::common::e::PAD_WIN_KEY_RIGHT)) {
			sound_nnsPlaySe( SE_U);
			this->ball->angle -= 1.0;
			if(this->ball->angle < 0.0) {
				this->ball->angle += 360.0;
			}
			this->ball->angle = double((((int)this->ball->angle) / 45 ) * 45);
		}
#ifdef _DEBUG
		else if((key->trigger & nn::common::e::PAD_WIN_KEY_UP) || (key->pulse & nn::common::e::PAD_WIN_KEY_UP)) {
			param += 0.010;
		}
		else if((key->trigger & nn::common::e::PAD_WIN_KEY_DOWN) || (key->pulse & nn::common::e::PAD_WIN_KEY_DOWN)) {
			param -= 0.010;
		}
#endif
	}
	else if(key->press & nn::common::e::PAD_WIN_BUTTON_B) {
		// カーブ スピン
		if(key->pulse & nn::common::e::PAD_WIN_KEY_LEFT) {
			if(this->cursorRotHrizon < nn::yb::e::global::SHOT_ROT_HORIZON_MAX) {			
				sound_nnsPlaySe( SE_U);
				this->cursorRotHrizon++;
				this->spinWindowY = 0;
			}
		}
		else if(key->pulse & nn::common::e::PAD_WIN_KEY_RIGHT) {
			if(this->cursorRotHrizon > -nn::yb::e::global::SHOT_ROT_HORIZON_MAX) {
				sound_nnsPlaySe( SE_U);
				this->cursorRotHrizon--;
				this->spinWindowY = 0;
			}
		}
		else if(this->ball->flyState == nn::yb::e::ball::FLY_STATE_ON) {
			if(key->pulse & nn::common::e::PAD_WIN_KEY_UP) {
				if(this->cursorRotVertical < nn::yb::e::global::SHOT_ROT_VERTICAL_MAX) {		
					sound_nnsPlaySe( SE_U);
					this->cursorRotVertical++;
					this->spinWindowY = 0;
				}
			}
			else if(key->pulse & nn::common::e::PAD_WIN_KEY_DOWN) {
				if(this->cursorRotVertical > -nn::yb::e::global::SHOT_ROT_VERTICAL_MAX) {		
					sound_nnsPlaySe( SE_U);
					this->cursorRotVertical--;
					this->spinWindowY = 0;
				}
			}
		}
	}
	else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_X) {
		// マップ表示に遷移
		sound_nnsPlaySe( SE_DEDO);
		this->mapCtrlCnt = 0;
		this->state = nn::yb::e::hole_ctrl::STATE_MAP;
	}
	else if(key->trigger & nn::common::e::PAD_WIN_KEY_UP) {
		// フライ、ゴロ変更
		if(this->ball->flyState != nn::yb::e::ball::FLY_STATE_ON) {
			sound_nnsPlaySe( SE_U);
			// 混乱するので変更
			//this->ball->flyState = nn::yb::e::ball::FLY_STATE_ON - this->ball->flyState;
			this->ball->flyState = nn::yb::e::ball::FLY_STATE_ON;
		}
	}
	else if(key->trigger & nn::common::e::PAD_WIN_KEY_DOWN) {
		// パワー変更
		sound_nnsPlaySe( SE_U);
		if(this->ball->flyState == nn::yb::e::ball::FLY_STATE_NONE) {
			this->ball->powerState = nn::yb::e::ball::POWER_STATE_FULL - this->ball->powerState;
		}
		else {
			this->ball->flyState = nn::yb::e::ball::FLY_STATE_NONE;
		}
	}
	else {
		if(!(key->press & nn::common::e::PAD_WIN_KEY_LEFT)) {
			this->leftPulseFlag = false;
		}
		if(!(key->press & nn::common::e::PAD_WIN_KEY_RIGHT)) {
			this->rightPuseFlag = false;
		}
		if(key->press & nn::common::e::PAD_WIN_KEY_LEFT) {
			// 方向変更
			if(key->trigger & nn::common::e::PAD_WIN_KEY_LEFT) {
				// 押されてすぐ
				sound_nnsPlaySe( SE_U);
				this->ball->angle += 1;
				//printf( "angle = %lf\n", this->ball->angle);
			}
			else if(key->pulse & nn::common::e::PAD_WIN_KEY_LEFT) {
				// しばらく押されている
				this->ball->angle += 1;
				this->leftPulseFlag = true;
			}
			else {
				// しばらく押された後は連続で動く
				if(this->leftPulseFlag) {
					this->ball->angle += 1;
				}
			}
			if(this->leftPulseFlag && this->frameCnt % 4 == 0) {
				sound_nnsPlaySe( SE_U);
			}
			if(this->ball->angle >= 360) {
				this->ball->angle -= 360;
			}
		}
		else if(key->press & nn::common::e::PAD_WIN_KEY_RIGHT) {
			// 方向変更
			if(key->trigger & nn::common::e::PAD_WIN_KEY_RIGHT) {
				// 押されてすぐ		
				sound_nnsPlaySe( SE_U);
				this->ball->angle -= 1;
				//printf( "angle = %lf\n", this->ball->angle);
			}
			else if(key->pulse & nn::common::e::PAD_WIN_KEY_RIGHT) {
				// しばらく押されている
				this->ball->angle -= 1;
				this->rightPuseFlag = true;
			}
			else {
				// しばらく押された後は連続で動く
				if(this->rightPuseFlag) {
					this->ball->angle -= 1;
				}
			}
			if(this->rightPuseFlag && this->frameCnt % 4 == 0) {
				sound_nnsPlaySe( SE_U);
			}
			if(this->ball->angle < 0) {
				this->ball->angle += 360;
			}
		}
		else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
			// ショットへ
			sound_nnsPlaySe( SE_CHURIN);
			this->ctrl->camera->moveCameraPos( (int)this->ball->x, (int)this->ball->y);
			if(this->ball->flyState == nn::yb::e::ball::FLY_STATE_NONE) {
				this->stopSpinVerticalVec = 0;
			}
			// 110408 フライ時は常に垂直スピンバーが動くように変更
			else if(this->cursorRotVertical > 0) {
				this->stopSpinVerticalVec = 1;
			}
			else {
				this->stopSpinVerticalVec = -1;
			}
			this->stopSpinVertical = this->cursorRotVertical * nn::yb::e::global::SHOT_SPIN_VERTICAL_MOVE_DIV;
			this->state = nn::yb::e::hole_ctrl::STATE_VERTICAL_SPIN;
		}
		else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_START) {
			nn::yb::main::CPause * pause = new nn::yb::main::CPause( this);
			pause->master = this->master;
			pause->holeCtrl = this;
			pause->endCb = nn::yb::stage::CHoleCtrl::pauseEndCb;
			pause->endCbArg = this;
			pause->start();
			this->pause = pause;
			this->state = nn::yb::e::hole_ctrl::STATE_PAUSE;
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::ballFlyingEnd() {

	if(this->ball->state == nn::yb::e::ball::STATE_STANDBY) {
		if(this->stage->getHp() <= 0) {
			// 死亡
			/*
			// 演出なし
			if(this->stage->lp <= 0) {
				// ゲームオーバー
				this->stage->ctrl->menu->state = nn::yb::e::menu::STATE_GAME;
				this->stage->taskDelete();
			}
			else {
				this->stage->lp--;
				this->stage->hp = nn::yb::e::global::STAGE_HP_MAX;
				this->state = nn::yb::e::hole_ctrl::STATE_STANDBY;
			}
			*/
			nn::yb::stage::CDead * dead = new nn::yb::stage::CDead( this);
			dead->master = this->master;
			dead->holeCtrl = this;
			dead->endCb = nn::yb::stage::CHoleCtrl::deadEndCb;
			dead->endCbArg = this;
			dead->start();
			this->nowBgm = this->stageBgm;
			sound_nnsStopSeq();
			this->state = nn::yb::e::hole_ctrl::STATE_DEAD;
		}
		else {
			this->startStateStandby();
		}
		this->statusWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
		if(this->statusDispFlag) {
			this->statusWindowY = 0;
		}
	}
	else {
		this->nextCupIn();
		if(this->timeAttackState != nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_NONE) {
			this->timeAttackState = nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_END;
		}
		this->state = nn::yb::e::hole_ctrl::STATE_CUPIN;
	}


}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::restartHoleCb( void * holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->restartHole();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::restartHole() {

	this->stage->spell = this->startSpell;
	this->stage->holeCnt--;
	this->stage->nextHole();
	this->taskDelete();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::deadEndCb( void * holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->deadEnd();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::deadEnd() {

	this->statusWindowY = 0;

	if(this->stage->getHp() <= 0) {
		// ゲームオーバー
		/*
		// 関数化
		this->stage->ctrl->menu->state = nn::yb::e::menu::STATE_GAME;
		*/
		this->stage->ctrl->menu->returnStageSelect();
		this->stage->taskDelete();
	}
	else {
		// 復活、蘇生
		nn::yb::common::yb_commonPlaySeq( this->nowBgm);
		this->nowBgm = BGM_STAGE01;
		this->startStateStandby();
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::ballFlyingEndCb( void * holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->ballFlyingEnd();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::disp() {

	//	デバッグ
#if defined(_DEBUG) || defined(__EDIT__)
	int debug_disp_cnt = 0;
	fontPrintfType( 0, debug_disp_cnt * 12, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE,
		"anble = %lf", this->ball->angle
	);
	debug_disp_cnt++;
	fontPrintfType( 0, debug_disp_cnt * 12, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE,
		"fix power = %d : %lf", this->debugPowerFixFlag, this->debugPowerFixPower
	);
	debug_disp_cnt++;
	fontPrintfType( 0, debug_disp_cnt * 12, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE,
		"ball x = %lf y = %lf", this->ball->x, this->ball->y
	);
	debug_disp_cnt++;
#endif //_DEBUG

	if(this->timeAttackState == nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_ACTIVE ||
		this->timeAttackState == nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_PAUSE ||
		this->timeAttackState == nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_END
	) {
		GXRgb color = RGB_WHITE;
		if(this->timeAttackState == nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_END) {
			color = GX_RGB( 8, 8, 31);
		}
		fontPrintfType( 0, 0, nn::yb::e::global::FONT_TYPE_SIZE_10, color,
			"%02d:%02d:%02d:%03d", this->timeAttackSumTime / 1000 / 60 / 60,
			this->timeAttackSumTime / 1000 / 60 % 60,
			this->timeAttackSumTime / 1000 % 60,
			this->timeAttackSumTime % 1000
		);
	}

	// 方向
	if(this->state == nn::yb::e::hole_ctrl::STATE_STANDBY || this->state == nn::yb::e::hole_ctrl::STATE_MAP) {

		double rot_h;
		double rot_v;
		double speed = 0.0;
		double z_speed = 0.0;

		nn::yb::common::CALC_SHOT_DATA data;
		memset( &data, 0, sizeof( nn::yb::common::CALC_SHOT_DATA));
		data.zSpeed = &z_speed;
		data.rotHorizon = &rot_h;
		data.rotVertical = &rot_v;
		data.speed = &speed;
		data.powerState = this->ball->powerState;
		data.flyState = this->ball->flyState;
		data.cursorSpinHrizon = this->cursorRotHrizon;
#if 1
		data.power = nn::yb::e::global::BALL_VEC_GUIDE_DISP_POWER;
#else
		// 水平スピン時に問題が出る
		data.power = nn::yb::e::global::BALL_VEC_GUIDE_DISP_POWER_NORMAL;
		if(this->ball->powerState != 0) {
			data.power = nn::yb::e::global::BALL_VEC_GUIDE_DISP_POWER_FULL;
		}
		if(this->ball->flyState != nn::yb::e::ball::FLY_STATE_NONE) {
			data.power = nn::yb::e::global::BALL_VEC_GUIDE_DISP_POWER_FLY;
		}
#endif
		data.stopSpinVertical = this->cursorRotVertical * nn::yb::e::global::SHOT_SPIN_VERTICAL_MOVE_DIV;
		nn::yb::common::yb_commonCalcShot( &data);

		double angle = this->ball->angle;
		// 多分いらなくなった
		double x = this->ball->x;
		double y = this->ball->y;
		double z = 0.0;
		bool ground_on_flag = false;
		bool fly_bound_flag = false;
		bool cupin_flag = false;
		double add_z = 0.0;
		double disp_interval = nn::yb::e::global::BALL_VEC_GUIDE_DISP_INTERVAL_NORMAL;
		int disp_dot_n = nn::yb::e::global::BALL_VEC_GUIDE_DISP_DOT_N;
		int disp_bound_n = nn::yb::e::global::BALL_VEC_GUIDE_DISP_BOUND_N;
		if(this->ball->powerState != 0) {
			disp_interval = nn::yb::e::global::BALL_VEC_GUIDE_DISP_INTERVAL_FULL;
		}
		if(this->ball->flyState != nn::yb::e::ball::FLY_STATE_NONE) {
			disp_dot_n = nn::yb::e::global::BALL_VEC_GUIDE_DISP_DOT_N_FLY;
			disp_interval = nn::yb::e::global::BALL_VEC_GUIDE_DISP_INTERVAL_FLY;
		}
		this->ballVecGuideNextDispStart += nn::yb::e::global::BALL_VEC_GUIDE_DISP_DOT_MOVE;
		if(this->ballVecGuideNextDispStart >= nn::yb::e::global::BALL_VEC_GUIDE_DISP_START + disp_interval) {
			this->ballVecGuideNextDispStart = nn::yb::e::global::BALL_VEC_GUIDE_DISP_START;
		}
		double next_disp = this->ballVecGuideNextDispStart;
		double len_now = 0.0;
		int dot_cnt = 0;
		int bound_cnt = 0;

		while((speed >= 0.100 || (!(z == 0.0)) || (!(z_speed == 0.0))) && dot_cnt < disp_dot_n && bound_cnt < disp_bound_n) {

			// 地面上かの判定
			nn::yb::common::CALC_BALL_DATA data;
			memset( &data, 0, sizeof( nn::yb::common::CALC_BALL_DATA));
			data.angle = &angle;
			data.groundSideFlag = false;
			data.groundFlag = true;
			data.rotHorizon = &rot_h;
			data.rotVertical = &rot_v;
			data.speed = &speed;
			data.z = &z;
			data.zSpeed = &z_speed;
			data.groundOnFlag = &ground_on_flag;
			data.flyBoundFlag = &fly_bound_flag;
			data.cupFlag = false;
			data.cupinFlag = &cupin_flag;
			data.addZ = &add_z;
			data.ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
			nn::yb::common::yb_commonCalcDown( &data);

			if(ground_on_flag) {
				// 回転処理
				nn::yb::common::CALC_SPIN_DATA spin_data;
				memset( &spin_data, 0, sizeof( nn::yb::common::CALC_SPIN_DATA));
				spin_data.angle = &angle;
				spin_data.spinHorizon = &rot_h;
				nn::yb::common::yb_commonCalcSpin( &spin_data);
			}
			

			// 事前計算
			nn::common::def::graphics2d::SEGMENT2_D fly_move;
			double add_x = nn::yb::common::yb_commonAngleSpeed2MoveX( angle, speed);
			double add_y = nn::yb::common::yb_commonAngleSpeed2MoveY( angle, speed);
			fly_move.s.x = x;
			fly_move.s.y = y;
			fly_move.v.x = add_x;
			fly_move.v.y = add_y;
			
			// ヤクモノ
			bool collision_flag = false;
			bool continue_flag = true;
			nn::common::def::graphics2d::VECTOR2_D collision;
			bool flags[1024];
			memset( flags, 0, sizeof( flags));
			while(continue_flag) {

				continue_flag = false;
				for( int i = 0; i < this->hole->data.holeObjN; i++) {

					nn::yb::def::HOLE_OBJ * obj = &this->hole->data.holeObj[i];
					switch(obj->category) {
						case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
							{
								if(flags[i] == false) {
									nn::common::def::graphics2d::LINE  line;
									line.s.x = obj->x1;
									line.s.y = obj->y1;
									line.e.x = obj->x2;
									line.e.y = obj->y2;

									bool result = false;

									nn::yb::common::CALC_BUMPER_DATA data;
									memset( &data, 0, sizeof( nn::yb::common::CALC_BUMPER_DATA));
									data.paraBumper = &line;
									data.paraMove = &fly_move;
									data.paraZ = z;
									data.paraBumperZ = nn::yb::e::global::HOLE_BUMPER_Z;

									data.returnResult = &result;
									data.returnAngle = &angle;
									data.returnSpeed = &speed;
									data.returnX = &x;
									data.returnY = &y;
									data.returnCollision = &collision;

									nn::yb::common::yb_commonCalcBumper( &data);
									if(result) {
										flags[i] = true;
										collision_flag = true;
										continue_flag = true;
									}
								}
							}
							break;
					}
					if(continue_flag) {
						// 更新
						double add_x = nn::yb::common::yb_commonAngleSpeed2MoveX( angle, speed);
						double add_y = nn::yb::common::yb_commonAngleSpeed2MoveY( angle, speed);
						fly_move.s.x = x;
						fly_move.s.y = y;
						fly_move.v.x = add_x;
						fly_move.v.y = add_y;
						break;
					}

				}

			}

			// 移動
			double x_move_now = 0.0;
			double y_move_now = 0.0;
			if(collision_flag) {
				x += fly_move.v.x;
				y += fly_move.v.y;
			}
			else {
				nn::yb::common::CALC_MOVE_DATA move_data;
				memset( &move_data, 0, sizeof( nn::yb::common::CALC_MOVE_DATA));
				move_data.x = &x;
				move_data.y = &y;
				move_data.z = z;
				move_data.addX = &x_move_now;
				move_data.addY = &y_move_now;
				move_data.speed = &speed;
				move_data.angle = angle;
				move_data.flyBoundFlag = fly_bound_flag;
				move_data.zSpeed = z_speed;
				data.ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
				nn::yb::common::yb_commonCalcMove( &move_data);
			}
			
			len_now += sqrt( NN_COMMON_POW2( x_move_now) + NN_COMMON_POW2( y_move_now) + NN_COMMON_POW2( add_z));

			if(speed <= 0.300 && z_speed == 0.0 && z == 0.0) {
				speed = 0.0;
			}

			if(collision_flag) {
				disp_commonMain3dBoard( (int)(collision.x), (int)(collision.y - z), nn::yb::e::global::DISP_Z_SHOT_GUIDE, 4, 4, RGB_RED, ALPHA_DEFAULT);
				disp_commonMain3dBoard( (int)(collision.x), (int)(collision.y), nn::yb::e::global::DISP_Z_SHOT_GUIDE + 1, 4, 4, RGB_BLUE, ALPHA_DEFAULT);
			}
			if(len_now >= next_disp) {
				next_disp += disp_interval;
				disp_commonMain3dBoard( (int)(x) - 3, (int)(y - z) - 3, nn::yb::e::global::DISP_Z_SHOT_GUIDE + 2, 5, 5, RGB_RED, ALPHA_DEFAULT);
				// z(空中)の移動なし
				disp_commonMain3dBoard( (int)(x) - 3, (int)(y) - 3, nn::yb::e::global::DISP_Z_SHOT_GUIDE + 3, 5, 5, RGB_BLUE, ALPHA_DEFAULT);
				dot_cnt++;
			}
			if(fly_bound_flag) {
				bound_cnt++;
				disp_commonMain3dBoard( (int)(x) - 6, (int)(y) - 6, nn::yb::e::global::DISP_Z_SHOT_GUIDE + 1, 11, 11, GX_RGB( 31, 24, 24), ALPHA_DEFAULT);
			}

		}
		::nn_commonLog( 120526, "len_now = %lf\n", len_now);
		this->xVecGuideEnd = (int)(x);
		this->yVecGuideEnd = (int)(y - z);
		disp_commonMain3dBoard( this->xVecGuideEnd - 5, this->yVecGuideEnd - 5, nn::yb::e::global::DISP_Z_SHOT_GUIDE, 9, 9, RGB_RED, ALPHA_DEFAULT);
	}
	
	// パワー
	if(this->state == nn::yb::e::hole_ctrl::STATE_POWER || this->state == nn::yb::e::hole_ctrl::STATE_VERTICAL_SPIN || this->state== nn::yb::e::hole_ctrl::STATE_SHOT) {
		disp_commonObjSimple( LAYOUT_ID_MAIN_POWER_BG);
		
		LAYOUT * layout_power = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_POWER);
		int max_power = (int)nn::yb::e::global::SHOT_BASE_MAX_POWER;
		int power = (int)this->ball->power;
		double power_h = (double)layout_power->h * power / max_power;
		GXRgb color = RGB_RED;
		if(this->state== nn::yb::e::hole_ctrl::STATE_SHOT && power == max_power) {
			color = GX_RGB( 31, 16,  0);
		}
		disp_commonMain3dBoard( layout_power->x, layout_power->y + (layout_power->h - (int)power_h), layout_power->z,
			layout_power->w, (int)power_h, color, 31);
	}

	// マップの矢印
	if(this->state == nn::yb::e::hole_ctrl::STATE_MAP) {
		if(this->mapCtrlCnt <= 24) {
			if(this->ctrl->camera->x > this->hole->startX + 320) {
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_WINDOW_CURSOR_LEFT);
			}
			if(this->ctrl->camera->x < this->hole->endX - 320) {
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_WINDOW_CURSOR_RIGHT);
			}
			if(this->ctrl->camera->y > this->hole->startY + 240) {
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_WINDOW_CURSOR_UP);
			}
			if(this->ctrl->camera->y < this->hole->endY - 240) {
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_WINDOW_CURSOR_DOWN);
			}
		}
	}
	
	// ステータスウィンドウ
	LAYOUT * layout_status_org = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_STATUS_WINDOW_ORG);
	LAYOUT * layout_status = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_STATUS_WINDOW);
	memcpy( layout_status, layout_status_org, sizeof( LAYOUT));
	int spin_shift_y = 0;
	if(this->state != nn::yb::e::hole_ctrl::STATE_DEAD &&
		this->state != nn::yb::e::hole_ctrl::STATE_PAUSE
	) {
		layout_status->y += this->statusWindowY;
	}
	if(this->state != nn::yb::e::hole_ctrl::STATE_VERTICAL_SPIN &&
		this->state != nn::yb::e::hole_ctrl::STATE_POWER &&
		this->state != nn::yb::e::hole_ctrl::STATE_SHOT &&
		this->state != nn::yb::e::hole_ctrl::STATE_DEAD &&
		this->state != nn::yb::e::hole_ctrl::STATE_PAUSE
	) {
		spin_shift_y = this->spinWindowY;
	}

	// 回転のカーソル
	// いきなり消えると微妙
	//if(this->cursorRotHrizon != 0 || this->cursorRotVertical != 0)
	if(this->state == nn::yb::e::hole_ctrl::STATE_STANDBY || this->state == nn::yb::e::hole_ctrl::STATE_VERTICAL_SPIN ||
		this->state == nn::yb::e::hole_ctrl::STATE_POWER_STANDBY || this->state == nn::yb::e::hole_ctrl::STATE_POWER ||
		this->state == nn::yb::e::hole_ctrl::STATE_SHOT || this->state == nn::yb::e::hole_ctrl::STATE_MAP ||
		this->state == nn::yb::e::hole_ctrl::STATE_PAUSE
	) {
		LAYOUT * layout_spinbar = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_SPINBAR_LEFTDOWN);
		if(this->state == nn::yb::e::hole_ctrl::STATE_POWER || this->state == nn::yb::e::hole_ctrl::STATE_VERTICAL_SPIN ||
			this->state == nn::yb::e::hole_ctrl::STATE_SHOT
		) {
			layout_spinbar = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_SPINBAR_CENTER);
		}

		disp_commonObjShift( LAYOUT_ID_SPINBAR_BG, layout_spinbar->x, layout_spinbar->y + spin_shift_y, layout_spinbar->z);
		disp_commonObjShift( LAYOUT_ID_SPINBAR_SPINBAR_YUKKURI, layout_spinbar->x, layout_spinbar->y + spin_shift_y, layout_spinbar->z);

		int cursor_spin_vertical = this->cursorRotVertical;
		if(this->ball->flyState == nn::yb::e::ball::FLY_STATE_NONE) {
			cursor_spin_vertical = 0;
		}
		int cursor_horizont_shift = -10;
		int cursor_vertical_shift = -6;
		disp_commonObjShift( LAYOUT_ID_SPINBAR_SPINBAR_CROSS_HORIZONT,
			layout_spinbar->x, layout_spinbar->y + cursor_vertical_shift * cursor_spin_vertical + spin_shift_y, layout_spinbar->z
		);
		disp_commonObjShift( LAYOUT_ID_SPINBAR_SPINBAR_CROSS_VERTICAL,
			layout_spinbar->x + cursor_horizont_shift * this->cursorRotHrizon, layout_spinbar->y + spin_shift_y, layout_spinbar->z
		);
		disp_commonObjShift( LAYOUT_ID_SPINBAR_SPINBAR_CURSOR_CENTER,
			layout_spinbar->x + cursor_horizont_shift * this->cursorRotHrizon,
			layout_spinbar->y + cursor_vertical_shift * cursor_spin_vertical + spin_shift_y,
			layout_spinbar->z
		);
		disp_commonObjShift( LAYOUT_ID_SPINBAR_SPINBAR_CURSOR_BG,
			layout_spinbar->x + cursor_horizont_shift * this->cursorRotHrizon,
			layout_spinbar->y + cursor_vertical_shift * cursor_spin_vertical + spin_shift_y,
			layout_spinbar->z
		);
		int stop_vertical_spin_y = layout_spinbar->y + cursor_vertical_shift * cursor_spin_vertical;
		if(nn::yb::e::hole_ctrl::STATE_VERTICAL_SPIN <= this->state && this->state <= nn::yb::e::hole_ctrl::STATE_FLYING && this->ball->flyState == nn::yb::e::ball::FLY_STATE_ON) {
			stop_vertical_spin_y = layout_spinbar->y + (int)((double)cursor_vertical_shift / (double)nn::yb::e::global::SHOT_SPIN_VERTICAL_MOVE_DIV * (double)this->stopSpinVertical);
		}
		disp_commonObjShift( LAYOUT_ID_SPINBAR_SPINBAR_CURSOR,
			layout_spinbar->x + cursor_horizont_shift * this->cursorRotHrizon,
			stop_vertical_spin_y + spin_shift_y,
			layout_spinbar->z
		);
	}

	// 打数
	disp_commonObjNumberSimpleShift( this->shotCnt, LAYOUT_ID_STATUS_WINDOW_PUT_NUM_NOW_1UNIT,
		LAYOUT_ID_STATUS_WINDOW_PUT_NUM_NOW_2UNIT, LAYOUT_ID_MAIN_STATUS_WINDOW
	);
	// 打数テキスト
	disp_commonObjSimpleShift( LAYOUT_ID_STATUS_WINDOW_PUT_NUM_NOW_TEXT, LAYOUT_ID_MAIN_STATUS_WINDOW);
	// 打数背景
	disp_commonObjSimpleShift( LAYOUT_ID_STATUS_WINDOW_PUT_NUM_NOW_BG, LAYOUT_ID_MAIN_STATUS_WINDOW);

	// ホール番号
	disp_commonObjNumberSimpleShift( this->stage->holeCnt, LAYOUT_ID_STATUS_WINDOW_HOLE_NUM_1UNIT,
		LAYOUT_ID_STATUS_WINDOW_HOLE_NUM_2UNIT, LAYOUT_ID_MAIN_STATUS_WINDOW
	);
	// ホールテキスト
	disp_commonObjSimpleShift( LAYOUT_ID_STATUS_WINDOW_HOLE_NUM_TEXT, LAYOUT_ID_MAIN_STATUS_WINDOW);
	// ホール背景
	disp_commonObjSimpleShift( LAYOUT_ID_STATUS_WINDOW_HOLE_NUM_BG, LAYOUT_ID_MAIN_STATUS_WINDOW);

	//disp_commonObjSimple( LAYOUT_ID_MAIN_STATUS_WINDOW);
	disp_commonObjSimpleShift( LAYOUT_ID_STATUS_WINDOW_HP_FRAME_1, LAYOUT_ID_MAIN_STATUS_WINDOW);
	disp_commonObjSimpleShift( LAYOUT_ID_STATUS_WINDOW_HP_FRAME_2, LAYOUT_ID_MAIN_STATUS_WINDOW);
	disp_commonObjSimpleShift( LAYOUT_ID_STATUS_WINDOW_HP_FRAME_3, LAYOUT_ID_MAIN_STATUS_WINDOW);
	disp_commonObjSimpleShift( LAYOUT_ID_STATUS_WINDOW_HP_FRAME_4, LAYOUT_ID_MAIN_STATUS_WINDOW);
	for( int i = 0; i < 4; i++) {

		if(i >= this->stage->getHp()) {
			break;
		}

		disp_commonObjSimpleShift( LAYOUT_ID_STATUS_WINDOW_HP_1 + i, LAYOUT_ID_MAIN_STATUS_WINDOW);

	}
	
	disp_commonObjNumberSimpleShift( this->stage->lp, LAYOUT_ID_STATUS_WINDOW_LP_N_1UNIT,
		LAYOUT_ID_STATUS_WINDOW_LP_N_2UNIT, LAYOUT_ID_MAIN_STATUS_WINDOW
	);
	disp_commonObjSimpleShift( LAYOUT_ID_STATUS_WINDOW_LP_ICON, LAYOUT_ID_MAIN_STATUS_WINDOW);

	// スペルカードアイコン
	int spell_layout_id_shift = LAYOUT_ID_MAIN_STATUS_WINDOW;
	if(this->state == nn::yb::e::hole_ctrl::STATE_FLYING || this->state == nn::yb::e::hole_ctrl::STATE_SHOT) {
		spell_layout_id_shift = LAYOUT_ID_MAIN_STATUS_WINDOW_ORG;
	}
	disp_commonObjSimpleShift( LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_FRAME, spell_layout_id_shift);
	int layout_id_spell_icon_tbl[nn::yb::e::global::SPELL_CARD_MAX] = {
		0,												// nn::yb::e::global::SPELL_CARD_NONE,
		LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_ICON_MUSO,	// nn::yb::e::global::SPELL_CARD_MUSO,
		LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_ICON_SPEA,	// nn::yb::e::global::SPELL_CARD_SPEA,
		LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_ICON_MSPA,	// nn::yb::e::global::SPELL_CARD_MSPA,
		LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_ICON_MOMI,	// nn::yb::e::global::SPELL_CARD_MOMIJI,
		LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_ICON_PRES,	// nn::yb::e::global::SPELL_CARD_TENKO,
		LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_ICON_MECH,	// nn::yb::e::global::SPELL_CARD_NITORI,
		LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_ICON_BLOD,	// nn::yb::e::global::SPELL_CARD_FLAN,
		LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_ICON_CAR,	// nn::yb::e::global::SPELL_CARD_ORIN,
	};
#if 1
	// 即時描画反映
	if(this->stage->spell != nn::yb::e::global::SPELL_CARD_NONE) {
		if(this->ball->spellCardEnableFlag || this->state != nn::yb::e::hole_ctrl::STATE_FLYING) {
			disp_commonObjSimpleShift( layout_id_spell_icon_tbl[this->stage->spell], spell_layout_id_shift);
		}
		else {
			disp_commonObjSimpleColorShift( layout_id_spell_icon_tbl[this->stage->spell], spell_layout_id_shift, RGB_WHITE, 8);
		}
	}
#else
	// 演出後描画反映
	if(this->dispSpell != nn::yb::e::global::SPELL_CARD_NONE) {
		if(this->ball->dispSpellCardEnableFlag || this->state != nn::yb::e::hole_ctrl::STATE_FLYING) {
			disp_commonObjSimpleShift( layout_id_spell_icon_tbl[this->dispSpell], spell_layout_id_shift);
		}
		else {
			disp_commonObjSimpleColorShift( layout_id_spell_icon_tbl[this->dispSpell], spell_layout_id_shift, RGB_WHITE, 8);
		}
	}
#endif
	
	// オープニング
#if 1
	if(this->state == nn::yb::e::hole_ctrl::STATE_OPENING) {
		disp_commonObjSimple( LAYOUT_ID_HOLE_OPENING_STAGE_TEXT);
		disp_commonObjNumberSimple( this->stage->stageNo,  LAYOUT_ID_HOLE_OPENING_STAGE_NUM_1UNIT, LAYOUT_ID_HOLE_OPENING_STAGE_NUM_2UNIT);
		disp_commonObjSimple( LAYOUT_ID_HOLE_OPENING_HOLE_TEXT);
		disp_commonObjNumberSimple( this->stage->holeCnt,  LAYOUT_ID_HOLE_OPENING_HOLE_NUM_1UNIT, LAYOUT_ID_HOLE_OPENING_HOLE_NUM_2UNIT);
	}
#endif

	::KEY_WORK * key = &this->master->keyWork;

	switch(this->state) {
		case nn::yb::e::hole_ctrl::STATE_OPENING:
			if(this->openingSkipFlag != true) {
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_X);	//IMAGE	
			}
			else {
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_X,  RGB_BLACK_HLAF, 16);	//IMAGE	
			}

			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_LEFT,  RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_RIGHT, RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_DOWN,  RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_UP,    RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_A,     RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_S,     RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_Z,     RGB_BLACK_HLAF, 16);	//IMAGE	
			break;
		case nn::yb::e::hole_ctrl::STATE_STANDBY:
			if(key->press & nn::common::e::PAD_WIN_BUTTON_Y) {
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_RIGHT);	//IMAGE	
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_LEFT);	//IMAGE	

				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_DOWN,  RGB_BLACK_HLAF, 16);	//IMAGE	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_UP,    RGB_BLACK_HLAF, 16);	//IMAGE	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_X,     RGB_BLACK_HLAF, 16);	//IMAGE	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_A,     RGB_BLACK_HLAF, 16);	//IMAGE	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_S,     RGB_BLACK_HLAF, 16);	//IMAGE	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_Z,     RGB_BLACK_HLAF, 16);	//IMAGE	
			}
			else if(key->press & nn::common::e::PAD_WIN_BUTTON_B) {
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_X,     RGB_BLACK_HLAF, 16);	//IMAGE	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_A,     RGB_BLACK_HLAF, 16);	//IMAGE	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_S,     RGB_BLACK_HLAF, 16);	//IMAGE	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_Z,     RGB_BLACK_HLAF, 16);	//IMAGE	
				
				if(this->cursorRotHrizon > -nn::yb::e::global::SHOT_ROT_HORIZON_MAX) {
					disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_RIGHT);	//IMAGE	
				}
				else {
					disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_RIGHT, RGB_BLACK_HLAF, 16);	//IMAGE	
				}
				if(this->cursorRotHrizon < nn::yb::e::global::SHOT_ROT_HORIZON_MAX) {
					disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_LEFT);	//IMAGE	
				}
				else {
					disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_LEFT,  RGB_BLACK_HLAF, 16);	//IMAGE	
				}

				if(this->ball->flyState == nn::yb::e::ball::FLY_STATE_ON) {
					if(this->cursorRotVertical > -nn::yb::e::global::SHOT_ROT_VERTICAL_MAX) {		
						disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_DOWN);	//IMAGE	
					}
					else {
						disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_DOWN,  RGB_BLACK_HLAF, 16);	//IMAGE	
					}
					if(this->cursorRotVertical < nn::yb::e::global::SHOT_ROT_VERTICAL_MAX) {
						disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_UP);	//IMAGE	
					}
					else {
						disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_UP,    RGB_BLACK_HLAF, 16);	//IMAGE	
					}
				}
				else {
					disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_DOWN,  RGB_BLACK_HLAF, 16);	//IMAGE	
					disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_UP,    RGB_BLACK_HLAF, 16);	//IMAGE	
				}
				
			}
			else {
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_DOWN);	//IMAGE	
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_RIGHT);	//IMAGE	
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_LEFT);	//IMAGE	
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_X);	//IMAGE	
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_A);	//IMAGE	
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_S);	//IMAGE	
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_Z);	//IMAGE	
				
				if(this->ball->flyState == nn::yb::e::ball::FLY_STATE_ON) {
					disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_UP,    RGB_BLACK_HLAF, 16);	//IMAGE	
				}
				else {
					disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_UP);	//IMAGE	
				}
			}
			break;
		case nn::yb::e::hole_ctrl::STATE_MAP:
			if(key->press & nn::common::e::PAD_WIN_BUTTON_Y) {	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_X,     RGB_BLACK_HLAF, 16);	//IMAGE	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_A,     RGB_BLACK_HLAF, 16);	//IMAGE	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_S,     RGB_BLACK_HLAF, 16);	//IMAGE	
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_Z,     RGB_BLACK_HLAF, 16);	//IMAGE	
			}
			else {
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_A);	//IMAGE	
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_Z);	//IMAGE	
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_X);	//IMAGE	
				
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_S,     RGB_BLACK_HLAF, 16);	//IMAGE	
			}
			disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_DOWN);	//IMAGE	
			disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_RIGHT);	//IMAGE	
			disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_LEFT);	//IMAGE	
			disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_UP);	//IMAGE	
			break;
		case nn::yb::e::hole_ctrl::STATE_VERTICAL_SPIN:
			disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_Z);	//IMAGE	
			disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_X);	//IMAGE	

			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_A,     RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_S,     RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_LEFT,  RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_RIGHT, RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_DOWN,  RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_UP,    RGB_BLACK_HLAF, 16);	//IMAGE	
			break;
		case nn::yb::e::hole_ctrl::STATE_POWER:
			disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_X);	//IMAGE	

			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_Z,     RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_A,     RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_S,     RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_LEFT,  RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_RIGHT, RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_DOWN,  RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_UP,    RGB_BLACK_HLAF, 16);	//IMAGE	
			break;
		case nn::yb::e::hole_ctrl::STATE_FLYING:
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_A,     RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_S,     RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_LEFT,  RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_RIGHT, RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_DOWN,  RGB_BLACK_HLAF, 16);	//IMAGE	
			disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_UP,    RGB_BLACK_HLAF, 16);	//IMAGE	

			if(this->ball->procWaitCnt == 0 && this->ball->state == nn::yb::e::ball::STATE_FLYING) {
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_X);	//IMAGE	
			}
			else {
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_X,     RGB_BLACK_HLAF, 16);	//IMAGE	
			}

			if(this->stage->spell != nn::yb::e::global::SPELL_CARD_NONE &&
				this->ball->state == nn::yb::e::ball::STATE_FLYING &&
				this->ball->spellCardEnableFlag) {
				disp_commonMain3dObjSimple( LAYOUT_ID_MAIN_KEY_Z);	//IMAGE	
			}
			else {
				disp_commonMain3dObjSimpleColor( LAYOUT_ID_MAIN_KEY_Z,     RGB_BLACK_HLAF, 16);	//IMAGE	
			}

			break;
		case nn::yb::e::hole_ctrl::STATE_CUPIN:
			break;
		case nn::yb::e::hole_ctrl::STATE_PAUSE:
			break;
	}

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// effect
//---------------------------------------------------------------------------

void nn::yb::stage::CHoleCtrl::effectCb( void * holeCtrlArg, void * dummy) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->ctrl_cnt--;
	if(hole_ctrl->ctrl_cnt == 0) {
		hole_ctrl->effect_ctrl->pop_event();
		hole_ctrl->taskDelete();
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::pauseEndCb(void *holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * holeCtrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	holeCtrl->pauseEnd();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::pauseEnd() {
	
	this->statusWindowState = nn::yb::e::hole_ctrl::STATUS_WINDOW_STATE_IN;
	this->statusWindowY = 0;
	this->spinWindowY = 0;

	switch(this->pause->cursorY) {
		case nn::yb::e::pause::SELECT_CANCEL:
			this->state = nn::yb::e::hole_ctrl::STATE_STANDBY;
			break;
		case nn::yb::e::pause::SELECT_RESTART_HOLE:
			{
				this->stage->setHp( 0);
				nn::yb::stage::CDead * dead = new nn::yb::stage::CDead( this);
				dead->master = this->master;
				dead->holeCtrl = this;
				dead->endCb = nn::yb::stage::CHoleCtrl::restartHoleCb;
				dead->endCbArg = this;
				dead->start();
				sound_nnsStopSeq();
				this->state = nn::yb::e::hole_ctrl::STATE_DEAD;
			}
			break;
		case nn::yb::e::pause::SELECT_RESTART_STAGE:
			this->stage->ctrl->menu->startStageFromScratch();
			this->stage->taskDelete();
			break;
		case nn::yb::e::pause::SELECT_TITLE:
			this->stage->ctrl->menu->returnTitle();
			this->stage->taskDelete();
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::openingCheckCb( void *holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->openingCheck();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::openingCheck() {

	this->openingEndCnt++;

	if(this->openingEndCnt == this->hole->openingCnt) {
		this->ctrl->camera->moveCameraPos( (int)this->ball->x, (int)this->ball->y, 15,
			nn::yb::stage::CHoleCtrl::openingEndCb, this
		);
	}
	else {
		this->ctrl->camera->moveCameraPos( this->hole->openingX[this->openingEndCnt], this->hole->openingY[this->openingEndCnt], 5,
			nn::yb::stage::CHoleCtrl::openingCheckCb, this
		);
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::openingEndCb(  void *holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->openingEnd();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::openingEnd() {

	this->xCamera = (int)this->ball->x;
	this->yCamera = (int)this->ball->y;
	this->ctrl->camera->setCameraPos( (int)this->ball->x, (int)this->ball->y);
	this->startStateStandby();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::startStateStandby() {

	if(this->stage->getHp() == 1) {
		this->ball->setDispState( nn::yb::e::ball::DISP_STATE_CRY);
	}
	else {
		this->ball->setDispState( nn::yb::e::ball::DISP_STATE_DEFAULT);
	}

	// ゆっくりの向きを近くの敵 or カップに合わせる
 	if(this->hole->enemyN >= 1) {
		int min_len = INT_MAX;
		int min_len_index = -1;
		for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( this->hole->enemy); i++) {

			nn::yb::main::CEnemy * enemy = this->hole->enemy[i];
			if(enemy != NULL) {
				int len = (int)NN_COMMON_POS_2_LEN( this->ball->x, this->ball->y, enemy->x, enemy->y);
				if(len < min_len) {
					min_len = len;
					min_len_index = i;
				}
			}

		}

		if(min_len_index >= 0) {
			nn::yb::main::CEnemy * enemy = this->hole->enemy[min_len_index];
			double angle = nn::common::graphics2d::pos2angle( this->ball->x, this->ball->y, enemy->x, enemy->y);
			//printf( "angle = %lf\n", angle);
			this->ball->angle = (double)((360 + (int)angle) % 360);
		}
	}
	else {
		double angle = nn::common::graphics2d::pos2angle( this->ball->x, this->ball->y, this->hole->cup->x, this->hole->cup->y);
		this->ball->angle = (double)((360 + (int)angle) % 360);
	}

	if(this->timeAttackState == nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_ACTIVE) {
		this->timeAttackState = nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_PAUSE;
	}

	this->state = nn::yb::e::hole_ctrl::STATE_STANDBY;
#ifdef _DEBUG
	// テスト用 いきなりカップイン
#if 0
	this->nextCupIn();
	this->state = nn::yb::e::hole_ctrl::STATE_CUPIN;
#endif
#endif //_DEBUG

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::calcCuideCamera() {

	static int cnt = 0;
#if 0
	this->ctrl->camera->moveCameraPos( this->xVecGuideEnd, this->yVecGuideEnd);;
#else
	// 跳ね返りを反映すると酔う
	this->ctrl->camera->moveCameraPos(
		this->xVecGuideEnd + ((int)this->ball->x - this->xVecGuideEnd) / 3,
		this->yVecGuideEnd + ((int)this->ball->y - this->yVecGuideEnd) / 3
	);
#endif

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::taskOpening() {

	::KEY_WORK * key = &this->master->keyWork;

	if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A || key->trigger & nn::common::e::PAD_WIN_BUTTON_START) {
		this->ctrl->camera->moveCameraPos( (int)this->ball->x, (int)this->ball->y, 20,
			nn::yb::stage::CHoleCtrl::openingEndCb, this
		);
		this->openingSkipFlag = true;
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::setSpell( int spell) {

	this->stage->spell = spell;

	nn::yb::effect::CSpellGet * spell_get = new nn::yb::effect::CSpellGet( this);
	spell_get->x = 320.0;
	spell_get->y = 240.0;
	spell_get->vx = 5.0;
	spell_get->vy = -10.0;
	LAYOUT * layout_window = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_STATUS_WINDOW_ORG);
	LAYOUT * layout_card = disp_commonGetLayoutMain3d( LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_ICON_MUSO);
	spell_get->dstX = (double)(layout_window->x + layout_card->x + layout_card->w / 2);
	spell_get->dstY = (double)(layout_window->y + layout_card->y + layout_card->h / 2);
	spell_get->speed = 2.0;
	spell_get->limitAngle = 8.0;
	spell_get->hitCardCbCb = nn::yb::stage::CHoleCtrl::spellGetHitCardCb;
	spell_get->hitCardCbCbArg = this;
	spell_get->hitKeyCbCb = nn::yb::stage::CHoleCtrl::spellGetHitKeyCb;
	spell_get->hitKeyCbCbArg = this;
	int obj_id_spell_icon_tbl[nn::yb::e::global::SPELL_CARD_MAX] = {
		0,								// nn::yb::e::global::SPELL_CARD_NONE,
		OBJ_ID_MAIN_SPELL_CARD_ICON_1,	// nn::yb::e::global::SPELL_CARD_MUSO,
		OBJ_ID_MAIN_SPELL_CARD_ICON_2,	// nn::yb::e::global::SPELL_CARD_SPEA,
		OBJ_ID_MAIN_SPELL_CARD_ICON_3,	// nn::yb::e::global::SPELL_CARD_MSPA,
		OBJ_ID_MAIN_SPELL_CARD_ICON_5,	// nn::yb::e::global::SPELL_CARD_MOMIJI,
		OBJ_ID_MAIN_SPELL_CARD_ICON_4,	// nn::yb::e::global::SPELL_CARD_TENKO,
		OBJ_ID_MAIN_SPELL_CARD_ICON_6,	// nn::yb::e::global::SPELL_CARD_NITORI,
		OBJ_ID_MAIN_SPELL_CARD_ICON_7,	// nn::yb::e::global::SPELL_CARD_FLAN,
		OBJ_ID_MAIN_SPELL_CARD_ICON_8,	// nn::yb::e::global::SPELL_CARD_ORIN,
	};
	spell_get->objIdSpellCardIcon = obj_id_spell_icon_tbl[spell];
	spell_get->start();
	

	this->ball->spellCardEnableFlag = true;

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::spellGetHitCardCb( void * holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->spellGetHitCard();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::spellGetHitCard() {

	this->ball->dispSpellCardEnableFlag = true;
	this->dispSpell = this->stage->spell;

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::spellGetHitKeyCb( void * holeCtrlArg) {

	nn::yb::stage::CHoleCtrl * hole_ctrl = (nn::yb::stage::CHoleCtrl *)holeCtrlArg;

	hole_ctrl->spellGetHitKey();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHoleCtrl::spellGetHitKey() {

}
//---------------------------------------------------------------------------