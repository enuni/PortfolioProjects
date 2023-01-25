// pause を置換
// Pause を置換
// yb を置換
// common を置換

#include "nn_common.h"

#include "disp_common.h"
#include "yb_common.h"
#include "key_common.h"
#include "font.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "sound_data.sadl"

#include "enum.h"
#include "hole_ctrl.h"
#include "stage.h"
#include "ctrl.h"
#include "menu.h"

#include "pause.h"
//---------------------------------------------------------------------------
nn::yb::main::CPause::CPause(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CPause Create\n");
	this->state = nn::yb::e::pause::STATE_NONE;
	this->oldState = nn::yb::e::pause::STATE_NONE;
	this->waitCnt = 0;

	this->holeCtrl = NULL;
	this->endCb = NULL;
	this->endCbArg = NULL;

	this->cursorY = nn::yb::e::pause::SELECT_CANCEL;
	this->cursorXSelect = nn::yb::e::pause::SELECT_CHECK_NO;

	this->holeRestartFailCnt = 0;

}
//---------------------------------------------------------------------------
nn::yb::main::CPause::~CPause() {

	::nn_commonLog( 1, "CPause Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::main::CPause::start() {

	this->state = nn::yb::e::pause::STATE_MENU;

}
//---------------------------------------------------------------------------
void nn::yb::main::CPause::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CPause::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	
	switch(this->state) {
		case nn::yb::e::pause::STATE_NONE:
			break;
		case nn::yb::e::pause::STATE_MENU:
			{
				if(key_commonCursorMoveUpDown( key, nn::yb::e::pause::SELECT_MAX - 1, &this->cursorY)) {
					sound_nnsPlaySe( SE_U);
				}
				else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
					sound_nnsPlaySe( SE_DEDON);

					switch(this->cursorY) {
						case nn::yb::e::pause::SELECT_RESTART_HOLE:
							if(this->holeCtrl->stage->lp >= 1) {
								this->state = nn::yb::e::pause::STATE_SELECT;
							}
							else {
								this->state = nn::yb::e::pause::STATE_HOLE_RESTART_FAIL;
							}
							break;
						default:
							if(this->endCb != NULL) {
								this->endCb( this->endCbArg);
							}
							this->taskDelete();
					}
				}
				else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_START) {
					sound_nnsPlaySe( SE_BU);

					this->cursorY = nn::yb::e::pause::SELECT_CANCEL;
					if(this->endCb != NULL) {
						this->endCb( this->endCbArg);
					}
					this->taskDelete();
				}
			}
			break;
		case nn::yb::e::pause::STATE_SELECT:
			{
				if(key_commonCursorMoveLR( key, nn::yb::e::pause::SELECT_CHECK_MAX - 1, &this->cursorXSelect)) {
					sound_nnsPlaySe( SE_U);
				}
				else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
					switch(this->cursorXSelect) {
						case nn::yb::e::pause::SELECT_CHECK_YES:
							sound_nnsPlaySe( SE_DEDON);

							if(this->endCb != NULL) {
								this->endCb( this->endCbArg);
							}
							this->taskDelete();
							break;
						case nn::yb::e::pause::SELECT_CHECK_NO:
							sound_nnsPlaySe( SE_BU);

							this->state = nn::yb::e::pause::STATE_MENU;
							break;
					}
				}
				else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_B) {
					sound_nnsPlaySe( SE_BU);

					this->cursorXSelect = nn::yb::e::pause::SELECT_CHECK_NO;
					this->state = nn::yb::e::pause::STATE_MENU;
				}
			}
			break;
		case nn::yb::e::pause::STATE_HOLE_RESTART_FAIL:
			this->holeRestartFailCnt++;
			if(key->trigger || this->holeRestartFailCnt >= 60) {
				sound_nnsPlaySe( SE_DEDON);

				this->holeRestartFailCnt = 0;
				this->state = nn::yb::e::pause::STATE_MENU;
			}
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
char * gTextTblPause[] = {
	"キャンセル",
	"このホールをやりなおす",
	"このステージの最初に戻る",
	"タイトルに戻る",
};

char * gTextTblCheck[] = {
	"",
	"ゆっくりを1つ失います。このホールを1打目からやりなおしますか？",
	"",
	"",
};

char * gTextTblCheckSelect[] = {
	"はい",
	"いいえ",
};
void nn::yb::main::CPause::disp() {

	// 暗くする
	disp_commonMain3dBoard( 0, 0, -49, 640, 480, ::GX_RGB( 0, 0, 0), 16);

	switch(this->state) {
		case nn::yb::e::pause::STATE_NONE:
			break;
		case nn::yb::e::pause::STATE_MENU:
			{
				for( int i = 0; i < nn::yb::e::pause::SELECT_MAX; i++) {

					GXRgb color = RGB_WHITE;
					if(i == this->cursorY) {
						color = RGB_RED;
					}
					fontPutsType( 210, 100 + i * 20, nn::yb::e::global::FONT_TYPE_SIZE_18, color, gTextTblPause[i]);

				}

				nn::yb::common::DISP_SCORE_LIST_DATA data;
				LAYOUT * layout_score_list = disp_commonGetLayoutMain3d( LAYOUT_ID_HOLE_RESULT_SCORE_LIST);
				data.x = layout_score_list->x;
				data.y = layout_score_list->y - 85;
				data.z = layout_score_list->z;
				data.holeCnt = this->holeCtrl->stage->holeCnt;
				data.nowHoleDispFlag = false;
				memcpy( data.score, this->holeCtrl->stage->score, sizeof data.score);
				data.shotCnt = 0;
				data.totalDispFlag = true;
				data.totalScore = this->holeCtrl->stage->totalScore;

				nn::yb::common::yb_commonDispScoreList( &data);
			}
			break;
		case nn::yb::e::pause::STATE_SELECT:
			fontPutsType( 25, 100, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_WHITE, gTextTblCheck[this->cursorY]);
			for( int i = 0; i < nn::yb::e::pause::SELECT_CHECK_MAX; i++) {

				GXRgb color = RGB_WHITE;
				if(i == this->cursorXSelect) {
					color = RGB_RED;
				}
				fontPutsType( 250 + i * 100, 140, nn::yb::e::global::FONT_TYPE_SIZE_18, color, gTextTblCheckSelect[i]);

			}
			break;
		case nn::yb::e::pause::STATE_HOLE_RESTART_FAIL:
			fontPutsType( 210, 120, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_WHITE,
				"くっ！ゆっくりが足りない"
			);
			break;
	}

}
//---------------------------------------------------------------------------

