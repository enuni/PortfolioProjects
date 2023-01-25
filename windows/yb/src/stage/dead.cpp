// dead を置換
// Dead を置換
// yb を置換
// stage を置換

#include "nn_common.h"

#include "direct_common.h"
#include "font.h"
#include "disp_common.h"
#include "animation.h"
#include "sound_nns.h"

#include "effect/effect_ctrl.h"
#include "effect/effect.h"

#include "enum.h"
#include "hole_ctrl.h"
#include "stage.h"
#include "ball.h"

#include "obj_main_3d_data.h"
#include "animation_data.h"
#include "sound_data.sadl"

#include "dead.h"

// 死亡演出とゲームオーバー演出
//---------------------------------------------------------------------------
nn::yb::stage::CDead::CDead(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CDead Create\n");
	this->state = nn::yb::e::dead::STATE_NONE;
	this->oldState = nn::yb::e::dead::STATE_NONE;
	this->waitCnt = 0;

	this->holeCtrl = NULL;
	this->endCb = NULL;
	this->endCbArg = NULL;
	this->gameoverFrameCnt = 0;
	this->gameoverAnime = NULL;

}
//---------------------------------------------------------------------------
nn::yb::stage::CDead::~CDead() {

	::nn_commonLog( 1, "CDead Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::stage::CDead::start() {

	this->state = nn::yb::e::dead::STATE_DEAD_START;
	direct_commonWaitSimple( NULL, 50, nn::yb::stage::CDead::nextCb, this);

	sound_nnsPlaySe( SE_PISHUN);
	
	this->holeCtrl->ball->setDispState( nn::yb::e::ball::DISP_STATE_DEAD);

	// 無くて大丈夫なら不要
	//this->holeCtrl->effect_ctrl->push_event( nn::yb::stage::CDead::effectCb, this, NULL);
	nn::yb::main::effect_start_end_total( this->holeCtrl, OBJ_ID_DUN_SPELL_HIT_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		20,
		(int)this->holeCtrl->ball->x, (int)this->holeCtrl->ball->x, 20,
		(int)this->holeCtrl->ball->y, (int)this->holeCtrl->ball->y, 20,
		1,//敵より手前に描画
		32, 384, 20,
		32, 384, 20,
		0, 90, 20,
		42, 0, 20,
		DISP_COMMON_BLEND_MODE_ADD, nn::yb::enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, this->holeCtrl->effect_ctrl);//拡大

}
//---------------------------------------------------------------------------
void nn::yb::stage::CDead::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CDead::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	
	switch(this->state) {
		case nn::yb::e::dead::STATE_NONE:
			break;
		case nn::yb::e::dead::STATE_DEAD_START:
			break;
		case nn::yb::e::dead::STATE_DEAD_HP_UP:
			break;
		case nn::yb::e::dead::STATE_GAMEOVER_START:
			break;
		case nn::yb::e::dead::STATE_GAMEOVER:
			{
				if(this->waitCnt == 0) {
					this->gameoverFrameCnt++;
					if(this->gameoverFrameCnt >= 180) {
						this->end();
					}
				}
			}
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CDead::disp() {

	switch(this->state) {
		case nn::yb::e::dead::STATE_NONE:
			break;
		case nn::yb::e::dead::STATE_DEAD_START:
			break;
		case nn::yb::e::dead::STATE_DEAD_HP_UP:
			break;
		case nn::yb::e::dead::STATE_GAMEOVER_START:
			break;
		case nn::yb::e::dead::STATE_GAMEOVER:
			{
				// 仮だったので消去
				//fontPutsType( 20, 20, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_WHITE, "ゲームオーバー");
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CDead::nextCb(void * arg) {

	nn::yb::stage::CDead * dead = (nn::yb::stage::CDead *)arg;
	dead->next();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CDead::next() {

	switch(this->state) {
		case nn::yb::e::dead::STATE_DEAD_START:
			if(this->holeCtrl->stage->lp <= 0) {
				sound_nnsPlaySeq( BGM_GAMEOVER);
				direct_commonCreateTaskBlackFadeOutAllSimple( NULL, nn::yb::stage::CDead::nextCb, this);
				this->state = nn::yb::e::dead::STATE_GAMEOVER_START;
			}
			else {
				sound_nnsPlaySe( SE_PIRYUN);
				this->holeCtrl->stage->lp--;
				direct_commonWaitSimple( NULL, 20, nn::yb::stage::CDead::nextCb, this);
				this->state = nn::yb::e::dead::STATE_DEAD_HP_UP;
			}
			break;
		case nn::yb::e::dead::STATE_DEAD_HP_UP:
			if(this->holeCtrl->stage->getHp() >= nn::yb::e::global::STAGE_HP_MAX) {
				sound_nnsPlaySe( SE_DODODEN);
				this->end();
			}
			else {
				sound_nnsPlaySe( SE_DEDHIN);
				this->holeCtrl->stage->damage( +1);
				direct_commonWaitSimple( NULL, 20, nn::yb::stage::CDead::nextCb, this);
			}
			break;
		case nn::yb::e::dead::STATE_GAMEOVER_START:
			direct_commonCreateTaskBlackFadeInAllSimple( NULL, nn::yb::stage::CDead::nextCb, this);
			this->state = nn::yb::e::dead::STATE_GAMEOVER;
			this->gameoverAnime = animationExec( NULL, ANIMATION_ID_GAMEOVER, 320, 240, -25, ANIMATION_FLAGS_LAST_WAIT, NULL, NULL);
			this->waitCnt = 1;
			break;
		case nn::yb::e::dead::STATE_GAMEOVER:
			this->waitCnt = 0;
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CDead::end() {

	this->holeCtrl->ball->setDispState( nn::yb::e::ball::DISP_STATE_DEFAULT);
	this->holeCtrl->ball->angle = 270.0;

	if(this->gameoverAnime != NULL) {
		ANIMATION * anime = (ANIMATION *)this->gameoverAnime->work;
		::taskDelete( anime->taskThis);
		this->gameoverAnime = NULL;
	}

	if(this->endCb != NULL) {
		this->endCb( this->endCbArg);
	}

	this->taskDelete();

}
//---------------------------------------------------------------------------

