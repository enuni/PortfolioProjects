// stage を置換
// Stage を置換
// yb を置換
// main を置換

#include "nn_common.h"

#include "disp_common.h"
#include "image_ctrl.h"
#include "direct_common.h"

#include "enum.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"

#include "font.h"
#include "animation.h"
#include "animation_data.h"

#include "sound_data.sadl"

#include "yb_common.h"

#include "hole_ctrl.h"
#include "hole.h"
#include "ball.h"

#include "ctrl.h"
#include "edit_menu.h"
#include "camera.h"
#include "save_data.h"
#include "menu.h"
#include "stage_result.h"
#include "ending.h"
#include "effect\\effect_ctrl.h"
#include "data_hole.h"

#include "stage.h"

int gHandle;
//---------------------------------------------------------------------------
nn::yb::main::CStage::CStage(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CStage Create\n");
	this->state = nn::yb::e::stage::STATE_NONE;
	this->oldState = nn::yb::e::stage::STATE_NONE;
	this->waitCnt = 0;

	this->holeCnt = 0;

	this->stageNo = 0;

	this->editFlag = false;
	this->ctrl = NULL;
	this->dataHole = NULL;

	this->saveData = NULL;

	memset( this->score, 0, sizeof( this->score));
	this->totalScore = 0;

	this->lp = 0;
	this->hp = 0;

	this->spell = nn::yb::e::global::SPELL_CARD_NONE;

	this->frameCnt = 0;

}
//---------------------------------------------------------------------------
nn::yb::main::CStage::~CStage() {

	::nn_commonLog( 1, "CStage Delete\n");

	if(this->dataHole != NULL) {
		this->dataHole->taskDelete();
		this->dataHole = NULL;
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::start() {
	
	// テスト用
#ifndef _DEBUG
#else _DEBUG
	// this->spell = nn::yb::e::global::SPELL_CARD_SPEA;
	// this->spell = nn::yb::e::global::SPELL_CARD_MSPA;
	// this->spell = nn::yb::e::global::SPELL_CARD_MOMIJI;
	// this->spell = nn::yb::e::global::SPELL_CARD_TENKO;
	// this->spell = nn::yb::e::global::SPELL_CARD_MUSO;
	// this->spell = nn::yb::e::global::SPELL_CARD_NITORI;
#endif

	/*
	// テスト用
	this->saveData->score.bestScore[this->stageNo - 1].total = 25;
	this->saveData->score.bestScore[this->stageNo - 1].score[0] = 3;
	this->saveData->score.bestScore[this->stageNo - 1].score[1] = 3;
	this->saveData->score.bestScore[this->stageNo - 1].score[2] = 2;
	this->saveData->score.bestScore[this->stageNo - 1].score[3] = 8;
	this->saveData->score.bestScore[this->stageNo - 1].score[4] = 4;
	this->saveData->score.bestScore[this->stageNo - 1].score[5] = 3;
	this->saveData->score.bestScore[this->stageNo - 1].score[6] = 3;
	this->saveData->score.bestScore[this->stageNo - 1].score[7] = 3;
	this->holeCnt = 3;
	this->score[0] = 4;
	this->score[1] = 27;
	this->score[2] = 2;
	this->totalScore = 33;
	*/

	this->effect_ctrl = new nn::yb::main::CEffectCtrl( this);
	if(this->editFlag) {
		// nn::yb::main::CCtrl::changeWindowSize()でロードする
		//this->ctrl->loadImage( 0);
		this->nextHole();
	}
	else {
		this->ctrl->loadImage( 1);

		this->state = nn::yb::e::stage::STATE_LOAD;
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CStage::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;

	switch(this->state) {
		case nn::yb::e::stage::STATE_LOAD:
			if(image_ctrlGetLoadAsyncState() == IMAGE_CTRL_ASYNC_STATE_NONE) {
				this->nextHole();
			}
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::disp() {

	switch(this->state) {
		case nn::yb::e::stage::STATE_LOAD:
			{
				this->frameCnt++; 
				nn::yb::e::ctrl::LOADING_SCREEN_PARAMS params;
				int bar_w = 8 * 15 + 1 * (15 - 1);
				params.text_x = 640 - bar_w - 20 - 1;
				params.text_y = 440 - 21;
				params.text_size = nn::yb::e::global::FONT_TYPE_SIZE_18;
				params.text_color = (GX_RGB( 31, 31, 31));

				const char* cText = "NOW LOADING...";
				int length = ::strlen(cText) ;
				params.text = new char[length + 1];
				::strcpy( params.text, cText ); 

				params.bar_x = 640 - bar_w - 20;
				params.bar_y = 440;
				params.bar_size = 8;
				params.bar_color = RGB_WHITE;

				params.window_x = 640 - bar_w - 20 - 1;
				params.window_y = 440 - 1;
				params.window_w = bar_w + 2;
				params.window_h = 8 + 2;
				params.window_color = RGB_BLACK;
				params.window_a = ALPHA_DEFAULT;

				this->ctrl->dispLoadingScreen( this->frameCnt,	params);
				if (params.text != NULL)
				{
					delete[] params.text;
					params.text = NULL;
				}
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::returnEdit() {

	this->ctrl->editMenu->dataHole = this->dataHole;
	this->dataHole = NULL;
	this->ctrl->editMenu->returnEdit();
	this->taskDelete();

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::nextHole() {

	if(this->holeCnt == 8) {
		// ステージクリア
		if(this->stageNo < nn::yb::e::global::STAGE_N) {
			switch(this->saveData->gameStageModeSelect) {
				case nn::yb::e::menu::STAGE_MODE_CHAOS:
					if(this->saveData->stageOpen <= this->stageNo) {
						this->saveData->stageOpen = this->stageNo + 1;
					}
					break;
				case nn::yb::e::menu::STAGE_MODE_STANDARD:
					if(this->saveData->stageOpenStandard <= this->stageNo) {
						this->saveData->stageOpenStandard = this->stageNo + 1;
					}
					break;
			}
			this->saveData->gameStageSelect[this->saveData->gameStageModeSelect] = this->stageNo;
		}
		nn::yb::main::CStageResult * stage_result = new nn::yb::main::CStageResult( this);
		stage_result->master = this->master;
		stage_result->stage = this;
		stage_result->endCb = nn::yb::main::CStage::stageResultEndCb;
		stage_result->endCbArg = this;
		stage_result->effect_ctrl = this->effect_ctrl;
		stage_result->start();
		this->state = nn::yb::e::stage::STATE_RESULT;
	}
	else {
		this->holeCtrl = new nn::yb::stage::CHoleCtrl( this);
		this->holeCtrl->master = this->master;
		this->holeCtrl->stage = this;
		this->holeCtrl->ctrl = this->ctrl;
		this->holeCtrl->effect_ctrl = this->effect_ctrl;
		this->holeCtrl->startSpell = this->spell;
		this->holeCtrl->start();
		
		this->holeCnt++;
		if(this->editFlag) {
			this->holeCtrl->hole->setHole( this->dataHole);
		}
		else {
			int hole_data_index;
			switch(this->saveData->gameStageModeSelect) {
				case nn::yb::e::menu::STAGE_MODE_CHAOS:
				default:
					hole_data_index = (this->stageNo - 1) * 8 + (this->holeCnt - 1);
					break;
				case nn::yb::e::menu::STAGE_MODE_STANDARD:
					hole_data_index = (this->stageNo - 1 + 6) * 8 + (this->holeCnt - 1);
					break;
			}
			if(hole_data_index < this->ctrl->holeDataN) {
				this->holeCtrl->hole->setHole( this->ctrl->holeData[hole_data_index],
					this->ctrl->holeDataSize[hole_data_index]
				);
			}
#if 0
			// ホールデータ結合前
			char data_file_name[32];
			sprintf( data_file_name, "hole%02d%02d.bin", this->stageNo, this->holeCnt);
			this->holeCtrl->hole->setHole( data_file_name);
#endif
		}

		/*
		// hole_ctrl.cpp openingCheck()に移行
		this->holeCtrl->xCamera = (int)this->holeCtrl->ball->x;
		this->holeCtrl->yCamera = (int)this->holeCtrl->ball->y;
		this->ctrl->camera->setCameraPos( (int)this->holeCtrl->ball->x, (int)this->holeCtrl->ball->y);
		*/
		this->holeCtrl->opening();

		this->state = nn::yb::e::stage::STATE_HOLE;
	}


}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::breakGame() {

	this->saveData->breakData.breakFlag = true;

	this->saveData->breakData.stageNo = this->stageNo;
	this->saveData->breakData.holeNo = this->holeCnt;
	memcpy( this->saveData->breakData.score, this->score, sizeof( this->saveData->breakData.score));
	this->saveData->breakData.totalScore = this->totalScore;
	this->saveData->breakData.lp = this->lp;
	this->saveData->breakData.hp = this->hp;
	this->saveData->breakData2.spell = this->spell;
	this->saveData->breakData3.stageMode = this->saveData->gameStageModeSelect;

	this->ctrl->saveMain();

	/*
	// 関数化
	this->ctrl->menu->state = nn::yb::e::menu::STATE_MAIN;
	this->ctrl->menu->gameState = nn::yb::e::menu::GAME_STATE_MAIN;
	*/
	this->ctrl->menu->returnTitle();
	this->taskDelete();

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::resumeGame() {

	this->saveData->breakData.breakFlag = false;

	this->stageNo = this->saveData->breakData.stageNo;
	this->holeCnt = this->saveData->breakData.holeNo;
	memcpy( this->score, this->saveData->breakData.score, sizeof( this->saveData->breakData.score));
	this->totalScore = this->saveData->breakData.totalScore;
	this->lp = this->saveData->breakData.lp;
	this->hp = this->saveData->breakData.hp;
	this->spell = this->saveData->breakData2.spell;
	this->saveData->gameStageModeSelect = this->saveData->breakData3.stageMode;

	this->ctrl->saveMain();

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::stageResultEndCb(void *stageArg) {

	nn::yb::main::CStage * stage = (nn::yb::main::CStage *)stageArg;

	stage->stageResultEnd();

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::stageResultEnd() {

	// ステージ開放とスコアセーブ
	this->ctrl->saveMain();

	if(this->stageNo < nn::yb::e::global::STAGE_N) {
		direct_commonCreateTaskBlackFadeInAllSimple( NULL, NULL, NULL);
		this->ctrl->menu->returnStageSelect();
		this->saveData->gameStageSelect[this->saveData->gameStageModeSelect] = this->stageNo;
		this->taskDelete();
	}
	else {
		// エンディング
		nn::yb::main::SCORE * score;
		switch(this->saveData->gameStageModeSelect) {
			case nn::yb::e::menu::STAGE_MODE_CHAOS:
			default:
				score = &this->saveData->score;
				break;
			case nn::yb::e::menu::STAGE_MODE_STANDARD:
				score = &this->saveData->scoreStandard;
				break;
		}

		int all_total_score = 0;
		for( int i = 0; i < nn::yb::e::global::STAGE_N; i++) {

			int stage_best = score->bestScore[i].total;
			if(stage_best <= 0) {
				stage_best = 99;
			}
			all_total_score += stage_best;

		}

		nn::yb::main::CEnding * ending = new nn::yb::main::CEnding( this);
		ending->endCb = endingEndCb;
		ending->endCbArg = this;
		ending->allTotalScore = all_total_score;
		ending->master = this->master;
		ending->start();
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::endingEndCb(void *stageArg) {

	nn::yb::main::CStage * stage = (nn::yb::main::CStage *)stageArg;

	stage->endingEnd();

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::endingEnd() {

	direct_commonCreateTaskBlackFadeInAllSimple( NULL, NULL, NULL);
	this->ctrl->menu->returnTitle();
	this->taskDelete();

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::saveHoleScoreRenew() {
	
	nn::yb::main::SCORE * score;
	switch(this->saveData->gameStageModeSelect) {
		case nn::yb::e::menu::STAGE_MODE_CHAOS:
		default:
			score = &this->saveData->score;
			break;
		case nn::yb::e::menu::STAGE_MODE_STANDARD:
			score = &this->saveData->scoreStandard;
			break;
	}
	
	score->bestScore[this->stageNo - 1].hole[this->holeCnt - 1] = this->holeCtrl->shotCnt;
	// 後で この後中断した場合、2回もセーブするので無駄
	this->ctrl->saveMain();

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::saveStageScoreRenew() {
	
	nn::yb::main::SCORE * score;
	switch(this->saveData->gameStageModeSelect) {
		case nn::yb::e::menu::STAGE_MODE_CHAOS:
		default:
			score = &this->saveData->score;
			break;
		case nn::yb::e::menu::STAGE_MODE_STANDARD:
			score = &this->saveData->scoreStandard;
			break;
	}

	score->bestScore[this->stageNo - 1].total = this->totalScore;
	memcpy( score->bestScore[this->stageNo - 1].score, this->score, sizeof( score->bestScore[this->stageNo - 1].score));
	// 後で stageResultEnd() で必ずセーブするので、セーブはしない
	//this->ctrl->saveMain();

}
//---------------------------------------------------------------------------
int nn::yb::main::CStage::getHp() {

	return this->hp;

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::setHp( int argHp) {

	this->hp = argHp;

}
//---------------------------------------------------------------------------
void nn::yb::main::CStage::damage( int damage) {

	this->hp += damage;
	if(this->hp < 0) {
		this->hp = 0;
	}

}
//---------------------------------------------------------------------------