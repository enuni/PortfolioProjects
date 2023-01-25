// stage_result を置換
// StageResult を置換
// yb を置換
// main を置換

#include "nn_common.h"

#include "disp_common.h"
#include "direct_common.h"
#include "sound_nns.h"
#include "animation.h"
#include "font.h"

#include "yb_common.h"
#include "save_data.h"
#include "enum.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "sound_data.sadl"
#include "animation_data.h"

#include "stage.h"
#include "hole_ctrl.h"
#include "menu.h"

#include "effect\\effect_star.h"

#include "stage_result.h"
//---------------------------------------------------------------------------
nn::yb::main::CStageResult::CStageResult(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CStageResult Create\n");
	this->state = nn::yb::e::stage_result::STATE_NONE;
	this->oldState = nn::yb::e::stage_result::STATE_NONE;
	this->waitCnt = 0;

	this->scoreListDispCnt = 0;
	this->totalScoreRenewFlag = false;

	this->textInFrameCnt = 0;
	this->slideInCnt = 0;
	this->waitFrameCnt = 0;

}
//---------------------------------------------------------------------------
nn::yb::main::CStageResult::~CStageResult() {

	::nn_commonLog( 1, "CStageResult Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::main::CStageResult::start() {

	direct_commonCreateTaskBlackFadeInAllSimple( NULL, nn::yb::main::CStageResult::nextStateCb, this);
	this->state = nn::yb::e::stage_result::STATE_FADE_IN;

}
//---------------------------------------------------------------------------
void nn::yb::main::CStageResult::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CStageResult::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	
	switch(this->state) {
		case nn::yb::e::stage_result::STATE_NONE:
			break;
		case nn::yb::e::stage_result::STATE_IN:
			{
				if(this->textInFrameCnt >= 31) {
					sound_nnsPlaySe( SE_YATTA);
					this->nextState();
				}
				else {
					if(this->textInFrameCnt % 8 == 0) {
						sound_nnsPlaySe( SE_DEDHIN);
					}
					this->textInFrameCnt++;
				}
			}
			break;
		case nn::yb::e::stage_result::STATE_SCORE_LIST_IN:
			{
				if(this->slideInCnt >= ::animationGetFrame( ANIMATION_ID_SLIDE_IN_HOLE_RESULT)) {
					this->nextState();
				}
				else {
					this->slideInCnt++;
				}
			}
			break;
		case nn::yb::e::stage_result::STATE_SCORE_LIST:
		case nn::yb::e::stage_result::STATE_TOTAL:
		case nn::yb::e::stage_result::STATE_RENEW:
			break;
		case nn::yb::e::stage_result::STATE_WAIT:
			this->waitFrameCnt++;
			if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
				direct_commonCreateTaskBlackFadeOutAllSimple( NULL, nn::yb::main::CStageResult::nextStateCb, this);
				this->state = nn::yb::e::stage_result::STATE_OUT;
			}
			break;
		case nn::yb::e::stage_result::STATE_OUT:
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::main::CStageResult::disp() {

	int layout_id_stage_mode[nn::yb::e::menu::STAGE_MODE_MAX] = {
		LAYOUT_ID_STAGE_RESULT_CHAOS,			// nn::yb::e::menu::STAGE_MODE_CHAOS
		LAYOUT_ID_STAGE_RESULT_STANDARD,		// nn::yb::e::menu::STAGE_MODE_STANDARD
	};

	nn::yb::main::SCORE * score;
	switch(this->stage->saveData->gameStageModeSelect) {
		case nn::yb::e::menu::STAGE_MODE_CHAOS:
		default:
			score = &this->stage->saveData->score;
			break;
		case nn::yb::e::menu::STAGE_MODE_STANDARD:
			score = &this->stage->saveData->scoreStandard;
			break;
	}

	switch(this->state) {
		case nn::yb::e::stage_result::STATE_NONE:
			break;
		case nn::yb::e::stage_result::STATE_FADE_IN:
		case nn::yb::e::stage_result::STATE_IN_WAIT:
			disp_commonObjSimple( LAYOUT_ID_TITLE_BG);
			disp_commonObjSimple( layout_id_stage_mode[this->stage->saveData->gameStageModeSelect]);
			disp_commonObjSimple( LAYOUT_ID_TITLE_STAGE_RESULT_YUKKURI);
			break;
		case nn::yb::e::stage_result::STATE_IN:
			{
				disp_commonObjSimple( LAYOUT_ID_TITLE_BG);
				disp_commonObjSimple( layout_id_stage_mode[this->stage->saveData->gameStageModeSelect]);
				disp_commonObjSimple( LAYOUT_ID_TITLE_STAGE_RESULT_YUKKURI);
				LAYOUT * layout_stage = disp_commonGetLayoutMain3d( LAYOUT_ID_STAGE_RESULT_STAGE);
				::disp_commonObjDxCenterFull( layout_stage->obj_id,
					layout_stage->x + layout_stage->w / 2,
					layout_stage->y + layout_stage->h / 2,
					layout_stage->z,
					layout_stage->w + (31 * 2) - (this->textInFrameCnt * 2),
					layout_stage->h + (31 * 2) - (this->textInFrameCnt * 2), 0, FALSE, FALSE, GX_RGB( 31, 16, 0), 31, DISP_COMMON_BLEND_MODE_NONE);
				
				LAYOUT * layout_stage_n = disp_commonGetLayoutMain3d( LAYOUT_ID_STAGE_RESULT_STAGE_N);
				::disp_commonObjDxCenterFull( layout_stage_n->obj_id + this->stage->stageNo,
					layout_stage_n->x + layout_stage_n->w / 2,
					layout_stage_n->y + layout_stage_n->h / 2,
					layout_stage_n->z,
					layout_stage_n->w + (31 * 2) - (this->textInFrameCnt * 2),
					layout_stage_n->h + (31 * 2) - (this->textInFrameCnt * 2), 0, FALSE, FALSE, GX_RGB( 31, 24, 0), 31, DISP_COMMON_BLEND_MODE_NONE);
				
				LAYOUT * layout_clear = disp_commonGetLayoutMain3d( LAYOUT_ID_STAGE_RESULT_CLEAR);
				::disp_commonObjDxCenterFull( layout_clear->obj_id ,
					layout_clear->x + layout_clear->w / 2,
					layout_clear->y + layout_clear->h / 2,
					layout_clear->z,
					layout_clear->w + (31 * 2) - (this->textInFrameCnt * 2),
					layout_clear->h + (31 * 2) - (this->textInFrameCnt * 2), 0, FALSE, FALSE, GX_RGB( 31, 16, 0), 31, DISP_COMMON_BLEND_MODE_NONE);
			}
			break;
		case nn::yb::e::stage_result::STATE_SCORE_LIST_IN:
		case nn::yb::e::stage_result::STATE_SCORE_LIST:
		case nn::yb::e::stage_result::STATE_TOTAL:
		case nn::yb::e::stage_result::STATE_RENEW:
		case nn::yb::e::stage_result::STATE_WAIT:
		case nn::yb::e::stage_result::STATE_OUT:
			{
				disp_commonObjSimple( LAYOUT_ID_TITLE_BG);
				disp_commonObjSimple( layout_id_stage_mode[this->stage->saveData->gameStageModeSelect]);
				disp_commonObjSimple( LAYOUT_ID_TITLE_STAGE_RESULT_YUKKURI);
				// ステージ番号
				disp_commonObjSimpleColor( LAYOUT_ID_STAGE_RESULT_STAGE, GX_RGB( 31, 16, 0), 31);	//IMAGE	
				disp_commonObjSimpleColor( LAYOUT_ID_STAGE_RESULT_CLEAR, GX_RGB( 31, 16, 0), 31);	//IMAGE	
				disp_commonObjNumberSimpleColor( this->stage->stageNo, LAYOUT_ID_STAGE_RESULT_STAGE_N, LAYOUT_ID_STAGE_RESULT_STAGE_N, GX_RGB( 31, 24, 0), 31);

				int disp_total_score = 0;
				for( int i = 0; i < this->scoreListDispCnt - 1; i++) {
					disp_total_score += this->stage->score[i];
				}
				
				int slide_in_shift_y = 0;
				if(this->state == nn::yb::e::stage_result::STATE_SCORE_LIST_IN) {
					ANIMATION_DISP_DATA anime_disp_data;
					animationGetDispData( &anime_disp_data, ANIMATION_ID_SLIDE_IN_HOLE_RESULT, NULL, 0, this->slideInCnt);
					slide_in_shift_y = anime_disp_data.x;
				}

				// スコアリスト
				nn::yb::common::DISP_SCORE_LIST_DATA data;
				LAYOUT * layout_score_list = disp_commonGetLayoutMain3d( LAYOUT_ID_STAGE_RESULT_SCORE_LIST);
				data.x = layout_score_list->x;
				data.y = layout_score_list->y + slide_in_shift_y;
				data.z = layout_score_list->z;
				data.holeCnt = this->scoreListDispCnt;
				data.nowHoleDispFlag = false;
				memcpy( data.score, this->stage->score, sizeof data.score);
				data.shotCnt = 0;
				data.totalScore = disp_total_score;
				data.totalDispFlag = true;
				if(this->state == nn::yb::e::stage_result::STATE_TOTAL) {
					data.totalDispFlag = true;
				}
				nn::yb::common::yb_commonDispScoreList( &data);

				// ステージベスト
				if(this->state >= nn::yb::e::stage_result::STATE_SCORE_LIST) {
					disp_commonMain3dObjSimple( LAYOUT_ID_STAGE_RESULT_STAGE_BEST);	//IMAGE	
					int before_best_total = score->bestScore[this->stage->stageNo - 1].total;
					if(before_best_total == 0) {
						before_best_total = 99;
					}
					disp_commonObjNumberSimple( before_best_total, LAYOUT_ID_STAGE_RESULT_STAGE_BEST_N_1, LAYOUT_ID_STAGE_RESULT_STAGE_BEST_N_2);
				}

				// ステージベスト更新
				if(this->totalScoreRenewFlag) {
					disp_commonMain3dObjSimple( LAYOUT_ID_STAGE_RESULT_STAGE_BEST_ARROW);	//IMAGE	
					disp_commonMain3dObjSimple( LAYOUT_ID_STAGE_RESULT_STAGE_BEST_RENEW);	//IMAGE
					::disp_commonObjNumberSimple( this->stage->totalScore, LAYOUT_ID_STAGE_RESULT_STAGE_BEST_RENEW_N_1,
						LAYOUT_ID_STAGE_RESULT_STAGE_BEST_RENEW_N_2
					);
					disp_commonMain3dObjSimple( LAYOUT_ID_STAGE_RESULT_SANAE);
				}
				
				// 次へ
				if(this->state == nn::yb::e::stage_result::STATE_WAIT) {
					if(this->waitFrameCnt % 60 <= 30) {
						int x = 220; int y = 450; 
						char * msg = "決定ボタンを押してね";
						fontPutsType( x - 1, y - 1, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, msg);
						fontPutsType( x - 1, y + 1, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, msg);
						fontPutsType( x + 1, y - 1, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, msg);
						fontPutsType( x + 1, y + 1, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, msg);
						fontPutsType( x, y, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_WHITE, msg);
					}
				}
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CStageResult::nextStateCb(void *stageResultArg) {

	nn::yb::main::CStageResult * stage_result = (nn::yb::main::CStageResult *)stageResultArg;
	stage_result->nextState();

}
//---------------------------------------------------------------------------
void nn::yb::main::CStageResult::nextState() {
	
	nn::yb::main::SCORE * score;
	switch(this->stage->saveData->gameStageModeSelect) {
		case nn::yb::e::menu::STAGE_MODE_CHAOS:
		default:
			score = &this->stage->saveData->score;
			break;
		case nn::yb::e::menu::STAGE_MODE_STANDARD:
			score = &this->stage->saveData->scoreStandard;
			break;
	}

	switch(this->state) {
		case nn::yb::e::stage_result::STATE_FADE_IN:
			direct_commonWaitSimple( NULL, 30, nn::yb::main::CStageResult::nextStateCb, this);
			this->state = nn::yb::e::stage_result::STATE_IN_WAIT;
			break;
		case nn::yb::e::stage_result::STATE_IN_WAIT:
			{
				//☆エフェクトの表示
				for (int i = 0; i < 4; i++)
				{
					int x = 80 + n_commonRandNDirect( 480);
					nn_commonLog( 110503, "x = %d\n", x);
					nn::yb::main::effect_star( this, 60, x, 80, x, 80, 128, 0, 20, 3.0f, this->effect_ctrl);//
				}
				this->state = nn::yb::e::stage_result::STATE_IN;
			}
			break;
		case nn::yb::e::stage_result::STATE_IN:
			this->state = nn::yb::e::stage_result::STATE_SCORE_LIST_IN;
			break;
		case nn::yb::e::stage_result::STATE_SCORE_LIST_IN:
			direct_commonWaitSimple( NULL, 45, nn::yb::main::CStageResult::nextStateCb, this);
			this->state = nn::yb::e::stage_result::STATE_SCORE_LIST;
			break;
		case nn::yb::e::stage_result::STATE_SCORE_LIST:
			if(this->scoreListDispCnt >= 9) {
				direct_commonWaitSimple( NULL, 45, nn::yb::main::CStageResult::nextStateCb, this);
				this->state = nn::yb::e::stage_result::STATE_TOTAL;
			}
			else {
				if(this->scoreListDispCnt >= 1) {
					sound_nnsPlaySe( SE_PIRYUN);
				}
				this->scoreListDispCnt++;
				direct_commonWaitSimple( NULL, 45, nn::yb::main::CStageResult::nextStateCb, this);
			}
			break;
		case nn::yb::e::stage_result::STATE_TOTAL:
			{
				int before_best_total = score->bestScore[this->stage->stageNo - 1].total;
				if(before_best_total == 0) {
					before_best_total = 99;
				}
				if(before_best_total > this->stage->totalScore) {
					sound_nnsPlaySe( SE_DODODODODOU);
					this->totalScoreRenewFlag = true;
					direct_commonWaitSimple( NULL, 45, nn::yb::main::CStageResult::nextStateCb, this);
					this->state = nn::yb::e::stage_result::STATE_RENEW;
				}
				else {
					this->state = nn::yb::e::stage_result::STATE_WAIT;
				}
			}
			break;
		case nn::yb::e::stage_result::STATE_RENEW:
			this->state = nn::yb::e::stage_result::STATE_WAIT;
			break;
		case nn::yb::e::stage_result::STATE_OUT:
			// セーブスコアの更新
			if(this->totalScoreRenewFlag) {
				this->stage->saveStageScoreRenew();
			}
			if(this->endCb != NULL) {
				this->endCb( this->endCbArg);
			}
			this->taskDelete();
			break;
	}

}
//---------------------------------------------------------------------------

