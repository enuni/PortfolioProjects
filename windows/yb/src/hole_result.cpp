// hole_result を置換
// HoleResult を置換
// yb を置換
// main を置換

#include "nn_common.h"

#include "disp_common.h"
#include "draw.h"
#include "animation.h"
#include "direct_common.h"
#include "key_common.h"
#include "sound_nns.h"

#include "LunaAnimation.h"
#include "yb_common.h"
#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "animation_data.h"
#include "sound_data.sadl"
#include "enum.h"

#include "ball.h"
#include "hole_ctrl.h"
#include "stage.h"
#include "save_data.h"
#include "menu.h"

#include "hole_result.h"

//---------------------------------------------------------------------------
nn::yb::main::CHoleResult::CHoleResult(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CHoleResult Create\n");
	this->state = nn::yb::e::hole_result::STATE_NONE;
	this->oldState = nn::yb::e::hole_result::STATE_NONE;
	this->waitCnt = 0;

	this->slideInCnt = 0;
	this->dispShotN = 0;
	this->bestScoreRenewFlag = false;
	this->dispShotNCntUpCnt = 0;
	this->holeCtrl = NULL;
	this->cursorX = 0;
	this->dispShiftX = 0;

}
//---------------------------------------------------------------------------
nn::yb::main::CHoleResult::~CHoleResult() {

	::nn_commonLog( 1, "CHoleResult Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::main::CHoleResult::start() {

	this->state = nn::yb::e::hole_result::STATE_SLIDE_IN;

}
//---------------------------------------------------------------------------
void nn::yb::main::CHoleResult::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CHoleResult::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	nn::yb::main::SCORE * score;
	switch(this->holeCtrl->stage->saveData->gameStageModeSelect) {
		case nn::yb::e::menu::STAGE_MODE_CHAOS:
		default:
			score = &this->holeCtrl->stage->saveData->score;
			break;
		case nn::yb::e::menu::STAGE_MODE_STANDARD:
			score = &this->holeCtrl->stage->saveData->scoreStandard;
			break;
	}

	::KEY_WORK * key = &this->master->keyWork;

	if(this->state == nn::yb::e::hole_result::STATE_NONE) {
	}
	else if(this->state == nn::yb::e::hole_result::STATE_SLIDE_IN) {
		if(key->press & nn::common::e::PAD_WIN_KEY_LEFT) {
			this->dispShiftX -= 2;
		}
		else if(key->press & nn::common::e::PAD_WIN_KEY_RIGHT) {
			this->dispShiftX += 2;
		}
		else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
			this->slideInCnt = ::animationGetFrame( ANIMATION_ID_SLIDE_IN_HOLE_RESULT);
		}

		if(this->slideInCnt >= ::animationGetFrame( ANIMATION_ID_SLIDE_IN_HOLE_RESULT)) {
			this->state = nn::yb::e::hole_result::STATE_SHOT_N_CNT_UP;
		}
		else {
			this->slideInCnt++;
		}
	}
	else if(this->state == nn::yb::e::hole_result::STATE_SHOT_N_CNT_UP) {
		if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
			this->dispShotN = this->holeCtrl->shotCnt;
			this->checkBestScoreRenew();
			this->state = nn::yb::e::hole_result::STATE_SELECT;
		}
		else {
			this->dispShotNCntUpCnt++;
			if(this->dispShotNCntUpCnt >= 25) {
				this->dispShotNCntUpCnt = 0;

				if(this->dispShotN >= this->holeCtrl->shotCnt) {
					this->checkBestScoreRenew();
					if(this->bestScoreRenewFlag) {
						sound_nnsPlaySe( SE_DODODEN);
					}
					direct_commonWaitSkip( NULL, 70, nn::yb::main::CHoleResult::nextState, this, nn::yb::common::yb_commonIsSkip, this->master);
					this->state = nn::yb::e::hole_result::STATE_BEST_SCORE;
				}
				else {
					if(this->dispShotN < 4) {
						sound_nnsPlaySe( SE_DEDO);
					}
					else {
						sound_nnsPlaySe( SE_DO);
					}
					this->dispShotN++;
				}

			}
		}
	}
	else if(this->state == nn::yb::e::hole_result::STATE_BEST_SCORE) {
		
	}
	else if(this->state == nn::yb::e::hole_result::STATE_SCORE_LIST) {

	}
	else if(this->state == nn::yb::e::hole_result::STATE_SELECT) {
		if(this->holeCtrl->stage->holeCnt < 8 && key_commonCursorMoveLR( key, nn::yb::e::hole_result::SELECT_MAX - 1, &this->cursorX)) {
			sound_nnsPlaySe( SE_U);
		}
		else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
			sound_nnsPlaySe( SE_DEDON);
			if(this->endCb != NULL) {
				this->endCb( this->endCbArg);
			}
		}
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::main::CHoleResult::nextState( void * holeResult, int state) {

	nn::yb::main::CHoleResult * hole_result = (nn::yb::main::CHoleResult *)holeResult;

	if(state == ::WAIT_DESTRUCT_STATE_SKIP) {
		masterKeyReset( &hole_result->master->keyWork);
		hole_result->state = nn::yb::e::hole_result::STATE_SELECT;
	}
	else {
		if(hole_result->state == nn::yb::e::hole_result::STATE_BEST_SCORE) {
			sound_nnsPlaySe( SE_DEDON);
			direct_commonWaitSkip( NULL, 70, nn::yb::main::CHoleResult::nextState, hole_result, nn::yb::common::yb_commonIsSkip, hole_result->master);
			hole_result->state = nn::yb::e::hole_result::STATE_SCORE_LIST;
		}
		else if(hole_result->state == nn::yb::e::hole_result::STATE_SCORE_LIST) {
			sound_nnsPlaySe( SE_DEDON);
			direct_commonWaitSkip( NULL, 70, nn::yb::main::CHoleResult::nextState, hole_result, nn::yb::common::yb_commonIsSkip, hole_result->master);
			hole_result->state = nn::yb::e::hole_result::STATE_SCORE_TOTAL;
		}
		else if(hole_result->state == nn::yb::e::hole_result::STATE_SCORE_TOTAL) {
			hole_result->state = nn::yb::e::hole_result::STATE_SELECT;
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CHoleResult::disp() {

	nn::yb::main::CStage * stage = this->holeCtrl->stage;
	nn::yb::main::SCORE * score;
	switch(stage->saveData->gameStageModeSelect) {
		case nn::yb::e::menu::STAGE_MODE_CHAOS:
		default:
			score = &stage->saveData->score;
			break;
		case nn::yb::e::menu::STAGE_MODE_STANDARD:
			score = &stage->saveData->scoreStandard;
			break;
	}
	
	int slide_in_shift_y = 0;
	int dark_board = -45;
	if(this->state == nn::yb::e::hole_result::STATE_SLIDE_IN) {
		ANIMATION_DISP_DATA anime_disp_data;
		animationGetDispData( &anime_disp_data, ANIMATION_ID_SLIDE_IN_HOLE_RESULT, NULL, 0, this->slideInCnt);
		slide_in_shift_y = anime_disp_data.x;
		dark_board = -100;
	}

	// 暗くする
	disp_commonMain3dBoard( 0, 0, dark_board, 640, 480, ::GX_RGB( 0, 0, 0), 16);

	// ステージモード
	int layout_id_stage_mode[nn::yb::e::menu::STAGE_MODE_MAX] = {
		LAYOUT_ID_HOLE_RESULT_CHAOS,				// nn::yb::e::menu::STAGE_MODE_CHAOS
		LAYOUT_ID_HOLE_RESULT_STANDARD,		// nn::yb::e::menu::STAGE_MODE_STANDARD
	};
	disp_commonObjSimple( layout_id_stage_mode[stage->saveData->gameStageModeSelect]);

	// テキストステージ
	LAYOUT * layout_text_stage = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_TEXT_STAGE);
	disp_commonMain3dObj( layout_text_stage->obj_id, layout_text_stage->x, layout_text_stage->y, layout_text_stage->z);

	int stage_no = stage->stageNo;
	// ステージ番号
	LAYOUT * layout_stage_n_1 = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_STAGE_N_1UNIT);
	LAYOUT * layout_stage_n_2 = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_STAGE_N_2UNIT);
	disp_commonMain3dObjNumber( stage_no, layout_stage_n_1->obj_id, layout_stage_n_1->x, layout_stage_n_1->y, layout_stage_n_1->z, layout_stage_n_2->x - layout_stage_n_1->x);

	// テキストステージベスト
	LAYOUT * layout_text_stage_best = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_TEXT_STAGE_BEST);
	disp_commonMain3dObj( layout_text_stage_best->obj_id, layout_text_stage_best->x, layout_text_stage_best->y, layout_text_stage_best->z);

	int stage_best = score->bestScore[stage->stageNo - 1].total;
	if(stage_best == 0) {
		stage_best = 99;
	}
	// ステージベスト数値
	LAYOUT * layout_stage_best_n_1 = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_STAGE_BEST_N_1UNIT);
	LAYOUT * layout_stage_best_n_2 = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_STAGE_BEST_N_2UNIT);
	disp_commonMain3dObjNumber( stage_best, layout_stage_best_n_1->obj_id, layout_stage_best_n_1->x, layout_stage_best_n_1->y, layout_stage_best_n_1->z, layout_stage_best_n_2->x - layout_stage_best_n_1->x);

	int hole_no = stage->holeCnt;
	// ホール番号
	LAYOUT * layout_hole_n_1 = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_HOLE_N_1UNIT);
	LAYOUT * layout_hole_n_2 = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_HOLE_N_2UNIT);
	disp_commonMain3dObjNumber( hole_no, layout_hole_n_1->obj_id, layout_hole_n_1->x + this->dispShiftX, layout_hole_n_1->y + slide_in_shift_y, layout_hole_n_1->z, layout_hole_n_2->x - layout_hole_n_1->x);

	// テキストホール
	LAYOUT * layout_text_hole = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_TEXT_HOLE);
	disp_commonMain3dObj( layout_text_hole->obj_id, layout_text_hole->x + this->dispShiftX, layout_text_hole->y + slide_in_shift_y, layout_text_hole->z);

	// 打数
	LAYOUT * layout_shot_n = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_SHOT_N);
	disp_commonMain3dObjNumber( this->dispShotN, layout_shot_n->obj_id, layout_shot_n->x + this->dispShiftX, layout_shot_n->y + slide_in_shift_y, layout_shot_n->z, -layout_shot_n->w);

	// テキスト打
	LAYOUT * layout_text_shot = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_TEXT_SHOT);
	disp_commonMain3dObj( layout_text_shot->obj_id, layout_text_shot->x + this->dispShiftX, layout_text_shot->y + slide_in_shift_y, layout_text_shot->z);

	// テキスト ベストスコア
	LAYOUT * layout_text_best_score = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_TEXT_BEST_SCORE);
	disp_commonMain3dObj( layout_text_best_score->obj_id, layout_text_best_score->x + this->dispShiftX, layout_text_best_score->y + slide_in_shift_y, layout_text_best_score->z);

	int best_score_before = score->bestScore[stage->stageNo - 1].hole[stage->holeCnt - 1];
	if(best_score_before == 0) {
		best_score_before = 99;
	}
	// ベストスコア　更新前
	LAYOUT * layout_best_score_n_before_1 = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_BEST_SCORE_N_BEFORE_1UNIT);
	LAYOUT * layout_best_score_n_before_2 = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_BEST_SCORE_N_BEFORE_2UNIT);
	disp_commonMain3dObjNumber( best_score_before, layout_best_score_n_before_1->obj_id,
		layout_best_score_n_before_1->x + this->dispShiftX, layout_best_score_n_before_1->y + slide_in_shift_y, layout_best_score_n_before_1->z,
		layout_best_score_n_before_2->x - layout_best_score_n_before_1->x
	);

	// ベストスコア更新後
	if(this->bestScoreRenewFlag) {
		LAYOUT * layout_best_score_n_after_1 = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_BEST_SCORE_N_AFTER_1_UNIT);
		LAYOUT * layout_best_score_n_after_2 = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_BEST_SCORE_N_AFTER_2UNIT);
		disp_commonMain3dObjNumber( this->holeCtrl->shotCnt, layout_best_score_n_after_1->obj_id,
			layout_best_score_n_after_1->x + this->dispShiftX, layout_best_score_n_after_1->y + slide_in_shift_y, layout_best_score_n_after_1->z,
			layout_best_score_n_after_2->x - layout_best_score_n_after_1->x
		);

		disp_commonMain3dObjShift( LAYOUT_ID_HOLE_RESULT_BEST_SCORE_RIGHT_ARROW, this->dispShiftX, 0, 0);
	}

	// スコアリスト	
	LAYOUT * layout_score_list = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_SCORE_LIST);
	nn::yb::common::DISP_SCORE_LIST_DATA data;
	data.x = layout_score_list->x + this->dispShiftX;
	data.y = layout_score_list->y + slide_in_shift_y;
	data.z = layout_score_list->z;
	data.holeCnt = this->holeCtrl->stage->holeCnt;
	data.nowHoleDispFlag = false;
	if(this->state >= nn::yb::e::hole_result::STATE_SCORE_LIST) {
		data.nowHoleDispFlag = true;
	}
	memcpy( data.score, this->holeCtrl->stage->score, sizeof data.score);
	data.shotCnt = this->holeCtrl->shotCnt;
	data.totalDispFlag = true;
	data.totalScore = this->holeCtrl->stage->totalScore;
	if(this->state >= nn::yb::e::hole_result::STATE_SCORE_TOTAL) {
		data.totalScore += this->holeCtrl->shotCnt;
	}
	nn::yb::common::yb_commonDispScoreList( &data);

	// 次 中断 選択
	if(this->state == nn::yb::e::hole_result::STATE_SELECT) {
		LAYOUT * layout_button_next = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_BUTTON_NEXT_HOLE);
		disp_commonMain3dObj( layout_button_next->obj_id, layout_button_next->x, layout_button_next->y + slide_in_shift_y, layout_button_next->z);
		if(this->holeCtrl->stage->holeCnt < 8) {
			// 中断
			LAYOUT * layout_button_break = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_BUTTON_BREAK);
			disp_commonMain3dObj( layout_button_break->obj_id, layout_button_break->x, layout_button_break->y + slide_in_shift_y, layout_button_break->z);
		}
		int layout_id_cursor[2] = {
			LAYOUT_ID_HOLE_RESULT_CURSOR_NEXT_HOLE,
			LAYOUT_ID_HOLE_RESULT_CURSOR_BREAK,
		};
		LAYOUT * layout_cursor = disp_commonGetLayoutMain3d( layout_id_cursor[this->cursorX]);
		disp_commonMain3dObj( layout_cursor->obj_id, layout_cursor->x, layout_cursor->y + slide_in_shift_y, layout_cursor->z);

		// 早苗さん
		if(this->holeCtrl->shotCnt >= 2) {
			disp_commonObjSimple( LAYOUT_ID_HOLE_RESULT_SANAE);
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CHoleResult::checkBestScoreRenew() {
	
	nn::yb::main::SCORE * score;
	switch(this->holeCtrl->stage->saveData->gameStageModeSelect) {
		case nn::yb::e::menu::STAGE_MODE_CHAOS:
		default:
			score = &this->holeCtrl->stage->saveData->score;
			break;
		case nn::yb::e::menu::STAGE_MODE_STANDARD:
			score = &this->holeCtrl->stage->saveData->scoreStandard;
			break;
	}

	int best_score_before = score->bestScore[this->holeCtrl->stage->stageNo - 1].hole[this->holeCtrl->stage->holeCnt - 1];
	if(best_score_before == 0) {
		best_score_before = 99;
	}
	if(this->holeCtrl->shotCnt < best_score_before) {
		this->bestScoreRenewFlag = true;
	}

}
//---------------------------------------------------------------------------

