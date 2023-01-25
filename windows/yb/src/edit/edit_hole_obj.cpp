// edit_hole_obj を置換
// EditHoleObj を置換
// yb を置換
// edit を置換

#include "nn_common.h"
#include "nn_common2.h"
#include "touch.h"

#include "disp_common.h"

#include "yb_common.h"
#include "yb_save_manager.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"

#include "animation.h"
#include "animation_data.h"

#include "edit.h"
#include "data_hole.h"
#include "enemy.h"

#include "edit_hole_obj.h"

char * nn::yb::edit::gEditObjNameTbl[] = {	// edit_hole_obj.h enum CATEGORY と合わせる
	"ボール",
	"地面",
	"バンパー",
	"敵",
	"敵(夢)",
	"敵(槍)",
	"坂(上)",
	"坂(右上)",
	"坂(右)",
	"坂(右下)",
	"坂(下)",
	"坂(左下)",
	"坂(左)",
	"坂(左上)",
	"西行妖",
	"ワープ",
	"ワープ(上-上)",
	"ワープ(上-右)",
	"ワープ(上-下)",
	"ワープ(上-左)",
	"ワープ(右-上)",
	"ワープ(右-右)",
	"ワープ(右-下)",
	"ワープ(右-左)",
	"ワープ(下-上)",
	"ワープ(下-右)",
	"ワープ(下-下)",
	"ワープ(下-左)",
	"ワープ(左-上)",
	"ワープ(左-右)",
	"ワープ(左-下)",
	"ワープ(左-左)",
	"キック(上)",
	"キック(右)",
	"キック(下)",
	"キック(左)",
	"敵(空中)",
	"敵(夢、空中)",
	"敵(槍、空中)",
	"敵(毛玉)",
	"敵(毛玉、空中)",
	"敵(羽目玉)",
	"敵(羽目玉、空中)",
	"ダッシュ",
	"ダッシュ(上)",
	"ダッシュ(右)",
	"ダッシュ(下)",
	"ダッシュ(左)",
	"ジャンプ",
	"カメラ",
	"敵(恋)",
	"敵(恋、空中)",
	"ダメージ床",
	"バンカー",
	"重力エリア",
	"弾幕エリア",
	"敵(文)",
	"敵(天子)",
	"敵(妖精 緑)",
	"敵(妖精 青)",
	"雲山",
	"お空",
	"吹き上げエリア",
	"コンベア(上)",
	"コンベア(右)",
	"コンベア(下)",
	"コンベア(左)",
	"小ジャンプ",
	"大ジャンプ",
	"SE床",
	"キック(右上)",
	"キック(左上)",
	"キック(右下)",
	"キック(左下)",
	"ダッシュ(右上)",
	"ダッシュ(左上)",
	"ダッシュ(右下)",
	"ダッシュ(左下)",
	"ワープ2(なし)",
	"ワープ2(上)",
	"ワープ2(右上)",
	"ワープ2(右)",
	"ワープ2(右下)",
	"ワープ2(下)",
	"ワープ2(左下)",
	"ワープ2(左)",
	"ワープ2(左上)",
	"敵(にとり)",
	"敵(フラン)",
	"敵(お燐)",
	"雲山(レミフラ)",
	"雲山(移動)",
	"雲山(移動レミ)",
	"Timeスタート",
	"Timeゴール",
};

#if 1
// クラスセーブは、タスク未対応
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::yb::edit::CEditHoleObj::dataRecode[] = {
	DATA_LOCAL(	nn::yb::edit::CEditHoleObj,	nn::yb::edit::CEditHoleObj::category),
	DATA_LOCAL(	nn::yb::edit::CEditHoleObj,	nn::yb::edit::CEditHoleObj::x1),
	DATA_LOCAL(	nn::yb::edit::CEditHoleObj,	nn::yb::edit::CEditHoleObj::x2),
	DATA_LOCAL(	nn::yb::edit::CEditHoleObj,	nn::yb::edit::CEditHoleObj::y1),
	DATA_LOCAL(	nn::yb::edit::CEditHoleObj,	nn::yb::edit::CEditHoleObj::y2),
	DATA_PTR( nn::yb::edit::CEditHoleObj, nn::yb::edit::CEditHoleObj::hole),
	DATA_LOCAL(	nn::yb::edit::CEditHoleObj,	nn::yb::edit::CEditHoleObj::vec),
	DATA_LOCAL(	nn::yb::edit::CEditHoleObj,	nn::yb::edit::CEditHoleObj::z1),
	DATA_LOCAL(	nn::yb::edit::CEditHoleObj,	nn::yb::edit::CEditHoleObj::z2),
	DATA_BASE( ::TASK_SAVE::dataRecode ),
	DATA_END,
};
#endif
//---------------------------------------------------------------------------
#if 1
// クラスセーブは、タスク未対応
nn::yb::edit::CEditHoleObj::CEditHoleObj() : TASK_SAVE() {

	::nn_commonLog( 1, "CEditHoleObj Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
nn::yb::edit::CEditHoleObj::CEditHoleObj(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CEditHoleObj Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
nn::yb::edit::CEditHoleObj::CEditHoleObj(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CEditHoleObj Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
#else
nn::yb::edit::CEditHoleObj::CEditHoleObj(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CEditHoleObj Create\n");
	this->constructorCommon();

}
#endif
//---------------------------------------------------------------------------
void nn::yb::edit::CEditHoleObj::constructorCommon() {

	this->state = nn::yb::e::edit_hole_obj::STATE_NONE;
	this->oldState = nn::yb::e::edit_hole_obj::STATE_NONE;
	this->waitCnt = 0;

	this->category = nn::yb::e::edit_hole_obj::CATEGORY_BALL;
	this->x1 = 0;
	this->y1 = 0;
	this->x2 = 0;
	this->y2 = 0;
	this->hole = NULL;
	this->activeFlag = false;
	
	this->xSlideOld = 0;
	this->ySlideOld = 0;

	this->vec = 0;
	this->z1 = 0;
	this->z2 = 0;

}
//---------------------------------------------------------------------------
nn::yb::edit::CEditHoleObj::~CEditHoleObj() {

	::nn_commonLog( 1, "CEditHoleObj Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditHoleObj::start() {

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditHoleObj::active() {

	this->hole->edit->activeClearObjAll();
	this->activeFlag = true;

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditHoleObj::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CEditHoleObj::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &nn::yb::common::getMaster()->keyWork;
	::TOUCH_WORK * touch = &nn::yb::common::getMaster()->touchWork;
	::KEYBOARD_WORK * kb = &nn::yb::common::getMaster()->keyboardWork;

	if(this->hole->edit->mode == nn::yb::e::edit::MODE_EDIT) {
		if(key->press) {
			if(this->activeFlag) {
				if((kb->press[KEYBOARD_LSHIFT] || kb->press[KEYBOARD_RSHIFT]) &&
					kb->press[KEYBOARD_T]
				) {
					if((key->trigger & PAD_INPUT_UP) || (key->hold & PAD_INPUT_UP)) {
						this->y1--;
					}
					else if((key->trigger & PAD_INPUT_DOWN) || (key->hold & PAD_INPUT_DOWN)) {
						this->y1++;
					}
					else if((key->trigger & PAD_INPUT_LEFT) || (key->hold & PAD_INPUT_LEFT)) {
						this->x1--;
					}
					else if((key->trigger & PAD_INPUT_RIGHT) || (key->hold & PAD_INPUT_RIGHT)) {
						this->x1++;
					}
				}
				else if(kb->press[KEYBOARD_T]) {
					if((key->trigger & PAD_INPUT_UP) || (key->hold & PAD_INPUT_UP)) {
						this->y2--;
					}
					else if((key->trigger & PAD_INPUT_DOWN) || (key->hold & PAD_INPUT_DOWN)) {
						this->y2++;
					}
					else if((key->trigger & PAD_INPUT_LEFT) || (key->hold & PAD_INPUT_LEFT)) {
						this->x2--;
					}
					else if((key->trigger & PAD_INPUT_RIGHT) || (key->hold & PAD_INPUT_RIGHT)) {
						this->x2++;
					}
				}
				else if(kb->press[KEYBOARD_A]) {
					if(key->pulse & nn::common::e::PAD_WIN_KEY_LEFT) {
						if(this->category != nn::yb::e::edit_hole_obj::CATEGORY_BALL) {
							this->category--;
							if(this->category <= nn::yb::e::edit_hole_obj::CATEGORY_BALL) {
								this->category = nn::yb::e::edit_hole_obj::CATEGORY_MAX - 1;
							}
						}
					}
					else if(key->pulse & nn::common::e::PAD_WIN_KEY_RIGHT) {
						if(this->category != nn::yb::e::edit_hole_obj::CATEGORY_BALL) {
							this->category++;
							if(this->category >= nn::yb::e::edit_hole_obj::CATEGORY_MAX) {
								this->category = nn::yb::e::edit_hole_obj::CATEGORY_BALL + 1;
							}
						}
					}
				}
				else if(kb->press[KEYBOARD_Z]) {
					if((key->trigger & nn::common::e::PAD_WIN_KEY_UP) || (key->hold & nn::common::e::PAD_WIN_KEY_UP)) {
						this->z1++;
					}
					else if((key->trigger & nn::common::e::PAD_WIN_KEY_DOWN) || (key->hold & nn::common::e::PAD_WIN_KEY_DOWN)) {
						this->z1--;
					}
				}
				else if(kb->press[KEYBOARD_V]) {
					if((key->trigger & nn::common::e::PAD_WIN_KEY_LEFT) || (key->hold & nn::common::e::PAD_WIN_KEY_LEFT)) {
						if(nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE <= this->category &&
							this->category <= nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP
						) {
							this->vec--;
							if(this->vec < nn::yb::e::edit_hole_obj::WARP2_VEC_NONE) {
								this->vec += nn::yb::e::edit_hole_obj::WARP2_VEC_MAX;
							}
						}
					}
					else if((key->trigger & nn::common::e::PAD_WIN_KEY_RIGHT) || (key->hold & nn::common::e::PAD_WIN_KEY_RIGHT)) {
						if(nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE <= this->category &&
							this->category <= nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP
						) {
							this->vec++;
							if(this->vec > nn::yb::e::edit_hole_obj::WARP2_VEC_LEFT_UP) {
								this->vec -= nn::yb::e::edit_hole_obj::WARP2_VEC_MAX;
							}
						}
					}
				}
				else if(kb->press[KEYBOARD_M]) {
					if((key->trigger & nn::common::e::PAD_WIN_KEY_UP) || (key->hold & nn::common::e::PAD_WIN_KEY_UP)) {
						this->y1--;
						this->y2--;
					}
					else if((key->trigger & nn::common::e::PAD_WIN_KEY_DOWN) || (key->hold & nn::common::e::PAD_WIN_KEY_DOWN)) {
						this->y1++;
						this->y2++;
					}
					else if((key->trigger & nn::common::e::PAD_WIN_KEY_LEFT) || (key->hold & nn::common::e::PAD_WIN_KEY_LEFT)) {
						this->x1--;
						this->x2--;
					}
					else if((key->trigger & nn::common::e::PAD_WIN_KEY_RIGHT) || (key->hold & nn::common::e::PAD_WIN_KEY_RIGHT)) {
						this->x1++;
						this->x2++;
					}
				}
			}
		}
		else {
			switch(this->category) {
				case nn::yb::e::edit_hole_obj::CATEGORY_BALL:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA:
				case nn::yb::e::edit_hole_obj::CATEGORY_PILLAR:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SKY:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO_SKY:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA_SKY:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA_SKY:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA_SKY:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_JUMP:
				case nn::yb::e::edit_hole_obj::CATEGORY_CAMERA:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA_SKY:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MOMIJI:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_TENKO:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_GREEN_FAIRY:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_BLUE_FAIRY:
				case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN:
				case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_REMIFLA:
				case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
				case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_WEAK: 
				case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_STRONG: 
				case nn::yb::e::edit_hole_obj::CATEGORY_SE_FLOOR:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_NITORI:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_FLAN:
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_ORIN:
					this->taskTouchBall();
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
					this->taskTouchLine();
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_GROUND:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP:

				case nn::yb::e::edit_hole_obj::CATEGORY_WARP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_LEFT:

				case nn::yb::e::edit_hole_obj::CATEGORY_DAMAGE:
				case nn::yb::e::edit_hole_obj::CATEGORY_BUNKER:
				case nn::yb::e::edit_hole_obj::CATEGORY_GRAVITY:
				case nn::yb::e::edit_hole_obj::CATEGORY_FUKIAGE: 
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE:
				case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA:
					
				case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_START:
				case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_GOAL:
					this->taskTouchRect();
					break;
			}
		}
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditHoleObj::touchActive() {
	
	::TOUCH_WORK * touch = &nn::yb::common::getMaster()->touchWork;
	int touch_x_correct = touch->x + this->hole->edit->xWindow;
	int touch_y_correct = touch->y + this->hole->edit->yWindow;
	
	int x1_temp = this->x1;
	int x2_temp = this->x2;
	if(x1_temp > x2_temp) {
		x1_temp = this->x2;
		x2_temp = this->x1;
	}
	int y1_temp = this->y1;
	int y2_temp = this->y2;
	if(y1_temp > y2_temp) {
		y1_temp = this->y2;
		y2_temp = this->y1;
	}

	int check = this->checkTouch();

	switch(this->category) {
		case nn::yb::e::edit_hole_obj::CATEGORY_BALL:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_CAMERA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MOMIJI:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_TENKO:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_GREEN_FAIRY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_BLUE_FAIRY:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_REMIFLA:
		case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_NITORI:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_FLAN:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_ORIN:
			if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_BALL) {
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_MOVE;
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_PILLAR:
			if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_PILLAR) {
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_MOVE;
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
			if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_LINE_X1_Y1) {
				int x_temp = this->x1;
				int y_temp = this->y1;
				this->x1 = this->x2;
				this->y1 = this->y2;
				this->x2 = x_temp;
				this->y2 = y_temp;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_TRANS;
			}
			else if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_LINE_X2_Y2) {
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_TRANS;
			}
			else if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_LINE) {
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_MOVE;
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_GROUND:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP:

		case nn::yb::e::edit_hole_obj::CATEGORY_DAMAGE:
		case nn::yb::e::edit_hole_obj::CATEGORY_BUNKER:
		case nn::yb::e::edit_hole_obj::CATEGORY_GRAVITY:

		case nn::yb::e::edit_hole_obj::CATEGORY_FUKIAGE: 
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT:
			
		case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_START:
		case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_GOAL:
			if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X1_Y1) {
				this->x1 = x2_temp;
				this->y1 = y2_temp;
				this->x2 = x1_temp;
				this->y2 = y1_temp;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_TRANS;
			}
			else if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X2_Y1) {
				this->x1 = x1_temp;
				this->y1 = y2_temp;
				this->x2 = x2_temp;
				this->y2 = y1_temp;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_TRANS;
			}
			else if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X1_Y2) {
				this->x1 = x2_temp;
				this->y1 = y1_temp;
				this->x2 = x1_temp;
				this->y2 = y2_temp;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_TRANS;
			}
			else if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X2_Y2) {
				this->x1 = x1_temp;
				this->y1 = y1_temp;
				this->x2 = x2_temp;
				this->y2 = y2_temp;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_TRANS;
			}
			else if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_RECT) {
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_MOVE;
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA:
			if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_WARP_X1_Y1) {
				int temp_x1 = this->x1;
				int temp_y1 = this->y1;
				this->x1 = this->x2;
				this->y1 = this->y2;
				this->x2 = temp_x1;
				this->y2 = temp_y1;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_TRANS;
				switch(this->category) {
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP;
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP;
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP;
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT;
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT;
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT;
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN;
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN;
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN;
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT;
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT;
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN:
						this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT;
						break;
				}
			}
			else if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_WARP_X2_Y2) {
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_TRANS;
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP:
			if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_WARP_X1_Y1) {
				int temp_x1 = this->x1;
				int temp_y1 = this->y1;
				this->x1 = this->x2;
				this->y1 = this->y2;
				this->x2 = temp_x1;
				this->y2 = temp_y1;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				int temp_ctegory = this->category;
				this->category = nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE + this->vec;
				this->vec = nn::yb::e::edit_hole_obj::WARP2_VEC_NONE + (temp_ctegory - nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE);
				this->state = nn::yb::e::edit_hole_obj::STATE_TRANS;
			}
			else if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_WARP_X2_Y2) {
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_TRANS;
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP:
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_WEAK: 
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_STRONG: 
		case nn::yb::e::edit_hole_obj::CATEGORY_SE_FLOOR:
			if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_WARP_X1_Y1) {
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
				this->state = nn::yb::e::edit_hole_obj::STATE_MOVE;
			}
			break;
	}

}
//---------------------------------------------------------------------------
bool nn::yb::edit::CEditHoleObj::isTouch() {

	int check = this->checkTouch();
	
	switch(this->category) {
		case nn::yb::e::edit_hole_obj::CATEGORY_BALL:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_CAMERA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MOMIJI:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_TENKO:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_GREEN_FAIRY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_BLUE_FAIRY:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_REMIFLA:
		case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_NITORI:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_FLAN:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_ORIN:
			if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_BALL) {
				return true;
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_PILLAR:
			if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_PILLAR) {
				return true;
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
			if( (check & nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X1_Y1) ||
				(check & nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X2_Y2) ||
				(check & nn::yb::e::edit_hole_obj::IS_TOUCH_LINE)
			) {
				return true;
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_GROUND:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_DAMAGE:
		case nn::yb::e::edit_hole_obj::CATEGORY_BUNKER:
		case nn::yb::e::edit_hole_obj::CATEGORY_GRAVITY:

		case nn::yb::e::edit_hole_obj::CATEGORY_FUKIAGE: 
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT:
			
		case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_START:
		case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_GOAL:
			if((check & nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X1_Y1) ||
				(check & nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X2_Y1) ||
				(check & nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X1_Y2) ||
				(check & nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X2_Y2) ||
				(check & nn::yb::e::edit_hole_obj::IS_TOUCH_RECT)
			) {
				return true;
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA:
			if( (check & nn::yb::e::edit_hole_obj::IS_TOUCH_WARP_X1_Y1) ||
				(check & nn::yb::e::edit_hole_obj::IS_TOUCH_WARP_X2_Y2)
			) {
				return true;
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP:
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_WEAK: 
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_STRONG: 
		case nn::yb::e::edit_hole_obj::CATEGORY_SE_FLOOR:
			if(check & nn::yb::e::edit_hole_obj::IS_TOUCH_WARP_X1_Y1) {
				return true;
			}
			break;
	}
	return false;

}
//---------------------------------------------------------------------------
int nn::yb::edit::CEditHoleObj::checkTouch() {

	int result = 0;

	::TOUCH_WORK * touch = &nn::yb::common::getMaster()->touchWork;
	int touch_x_correct = touch->x + this->hole->edit->xWindow;
	int touch_y_correct = touch->y + this->hole->edit->yWindow;
	
	int x1_temp = this->x1;
	int x2_temp = this->x2;
	if(x1_temp > x2_temp) {
		x1_temp = this->x2;
		x2_temp = this->x1;
	}
	int y1_temp = this->y1;
	int y2_temp = this->y2;
	if(y1_temp > y2_temp) {
		y1_temp = this->y2;
		y2_temp = this->y1;
	}

	// ボールサイズ
	if(this->x1 - 24 <= touch_x_correct && touch_x_correct <= this->x1 + 24 &&
		this->y1 - 32 <= touch_y_correct && touch_y_correct <= this->y1 + 32
	) {
		result |= nn::yb::e::edit_hole_obj::IS_TOUCH_BALL;
	}
	// 柱サイズ
	if(this->x1 - 24 <= touch_x_correct && touch_x_correct <= this->x1 + 24 &&
		this->y1 - 112 <= touch_y_correct && touch_y_correct <= this->y1 + 24
	) {
		result |= nn::yb::e::edit_hole_obj::IS_TOUCH_PILLAR;
	}
	// 板サイズ x1 y1
	if(this->x1 - 48 <= touch_x_correct && touch_x_correct <= this->x1 + 48 &&
		this->y1 - 48 <= touch_y_correct && touch_y_correct <= this->y1 + 48
	) {
		result |= nn::yb::e::edit_hole_obj::IS_TOUCH_WARP_X1_Y1;
	}
	// 板サイズ x2 y2
	if(this->x2 - 48 <= touch_x_correct && touch_x_correct <= this->x2 + 48 &&
		this->y2 - 48 <= touch_y_correct && touch_y_correct <= this->y2 + 48
	) {
		result |= nn::yb::e::edit_hole_obj::IS_TOUCH_WARP_X2_Y2;
	}
	// 線分
	int x_line_tbl[9] = {
		-1,  0, +1, -1,  0, +1, -1,  0, +1,
	};
	int y_line_tbl[9] = {
		-1, -1, -1,  0,  0,  0, +1, +1, +1,
	};
	for( int i = 0; i < 9; i++) {

		int x1_line = this->x1 + x_line_tbl[i];
		int y1_line = this->y1 + y_line_tbl[i];
		int x2_line = this->x2 + x_line_tbl[i];
		int y2_line = this->y2 + y_line_tbl[i];
		if(x1_temp <= touch_x_correct && touch_x_correct <= x2_temp && y1_temp <= touch_y_correct && touch_y_correct <= y2_temp &&
			(
				( (x2_line - x1_line == 0) && (touch_x_correct - x1_line == 0) ) ||
				( (x2_line - x1_line != 0) && (touch_x_correct - x1_line != 0) &&
				( (y2_line - y1_line)*10)/(x2_line - x1_line) == ((touch_y_correct - y1_line)*10)/(touch_x_correct - x1_line) ) ||
				( (y2_line - y1_line != 0) && (touch_y_correct - y1_line != 0) &&
				( (x2_line - x1_line)*10)/(y2_line - y1_line) == ((touch_x_correct - x1_line)*10)/(touch_y_correct - y1_line) )
			)
		) {
			result |= nn::yb::e::edit_hole_obj::IS_TOUCH_LINE;
		}

	}
	if(NN_COMMON_IS_POS_AROUND( touch_x_correct, touch_y_correct, this->x1, this->y1, 6)) {
		result |= nn::yb::e::edit_hole_obj::IS_TOUCH_LINE_X1_Y1;
	}
	if(NN_COMMON_IS_POS_AROUND( touch_x_correct, touch_y_correct, this->x2, this->y2, 6)) {
		result |= nn::yb::e::edit_hole_obj::IS_TOUCH_LINE_X2_Y2;
	}
	if(NN_COMMON_IS_POS_AROUND( touch_x_correct, touch_y_correct, x1_temp, y1_temp, 6)) {
		result |= nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X1_Y1;
	}
	if(NN_COMMON_IS_POS_AROUND( touch_x_correct, touch_y_correct, x2_temp, y1_temp, 6)) {
		result |= nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X2_Y1;
	}
	if(NN_COMMON_IS_POS_AROUND( touch_x_correct, touch_y_correct, x1_temp, y2_temp, 6)) {
		result |= nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X1_Y2;
	}
	if(NN_COMMON_IS_POS_AROUND( touch_x_correct, touch_y_correct, x2_temp, y2_temp, 6)) {
		result |= nn::yb::e::edit_hole_obj::IS_TOUCH_CORNER_X2_Y2;
	}
	if(x1_temp <= touch_x_correct && touch_x_correct <= x2_temp && y1_temp <= touch_y_correct && touch_y_correct <= y2_temp) {
		result |= nn::yb::e::edit_hole_obj::IS_TOUCH_RECT;
	}

	return result;
}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditHoleObj::taskTouchRect() {

	::TOUCH_WORK * touch = &nn::yb::common::getMaster()->touchWork;
	int touch_x_correct = touch->x + this->hole->edit->xWindow;
	int touch_y_correct = touch->y + this->hole->edit->yWindow;
	int release_x_correct = touch->release_x + this->hole->edit->xWindow;
	int release_y_correct = touch->release_y + this->hole->edit->yWindow;

	switch(this->state) {
		case nn::yb::e::edit_hole_obj::STATE_MOVE:
			if(touch->touchPoint & TOUCH_PRESS) {
				this->x1 += touch_x_correct - this->xSlideOld;
				this->y1 += touch_y_correct - this->ySlideOld;
				this->x2 += touch_x_correct - this->xSlideOld;
				this->y2 += touch_y_correct - this->ySlideOld;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
			}
			else if(touch->touchPoint & TOUCH_RELEASE) {
				this->x1 += release_x_correct - this->xSlideOld;
				this->y1 += release_y_correct - this->ySlideOld;
				this->x2 += release_x_correct - this->xSlideOld;
				this->y2 += release_y_correct - this->ySlideOld;
				this->state = nn::yb::e::edit_hole_obj::STATE_NONE;
			}
			break;
		case nn::yb::e::edit_hole_obj::STATE_TRANS:
			if(touch->touchPoint & TOUCH_PRESS) {
				this->x2 += touch_x_correct - this->xSlideOld;
				this->y2 += touch_y_correct - this->ySlideOld;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
			}
			else if(touch->touchPoint & TOUCH_RELEASE) {
				this->x2 += release_x_correct - this->xSlideOld;
				this->y2 += release_y_correct - this->ySlideOld;
				this->state = nn::yb::e::edit_hole_obj::STATE_NONE;
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditHoleObj::taskTouchLine() {

	::TOUCH_WORK * touch = &nn::yb::common::getMaster()->touchWork;
	int touch_x_correct = touch->x + this->hole->edit->xWindow;
	int touch_y_correct = touch->y + this->hole->edit->yWindow;
	int release_x_correct = touch->release_x + this->hole->edit->xWindow;
	int release_y_correct = touch->release_y + this->hole->edit->yWindow;

	switch(this->state) {
		case nn::yb::e::edit_hole_obj::STATE_MOVE:
			if(touch->touchPoint & TOUCH_PRESS) {
				this->x1 += touch_x_correct - this->xSlideOld;
				this->y1 += touch_y_correct - this->ySlideOld;
				this->x2 += touch_x_correct - this->xSlideOld;
				this->y2 += touch_y_correct - this->ySlideOld;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
			}
			else if(touch->touchPoint & TOUCH_RELEASE) {
				this->x1 += release_x_correct - this->xSlideOld;
				this->y1 += release_y_correct - this->ySlideOld;
				this->x2 += release_x_correct - this->xSlideOld;
				this->y2 += release_y_correct - this->ySlideOld;
				this->state = nn::yb::e::edit_hole_obj::STATE_NONE;
			}
			break;
		case nn::yb::e::edit_hole_obj::STATE_TRANS:
			if(touch->touchPoint & TOUCH_PRESS) {
				this->x2 += touch_x_correct - this->xSlideOld;
				this->y2 += touch_y_correct - this->ySlideOld;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
			}
			else if(touch->touchPoint & TOUCH_RELEASE) {
				this->x2 += release_x_correct - this->xSlideOld;
				this->y2 += release_y_correct - this->ySlideOld;
				this->state = nn::yb::e::edit_hole_obj::STATE_NONE;
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditHoleObj::taskTouchBall() {

	::TOUCH_WORK * touch = &nn::yb::common::getMaster()->touchWork;
	int touch_x_correct = touch->x + this->hole->edit->xWindow;
	int touch_y_correct = touch->y + this->hole->edit->yWindow;
	int release_x_correct = touch->release_x + this->hole->edit->xWindow;
	int release_y_correct = touch->release_y + this->hole->edit->yWindow;

	switch(this->state) {
		case nn::yb::e::edit_hole_obj::STATE_MOVE:
			if(touch->touchPoint & TOUCH_PRESS) {
				this->x1 += touch_x_correct - this->xSlideOld;
				this->y1 += touch_y_correct - this->ySlideOld;
				this->xSlideOld = touch_x_correct;
				this->ySlideOld = touch_y_correct;
			}
			else if(touch->touchPoint & TOUCH_RELEASE) {
				this->x1 += release_x_correct - this->xSlideOld;
				this->y1 += release_y_correct - this->ySlideOld;
				this->state = nn::yb::e::edit_hole_obj::STATE_NONE;
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditHoleObj::disp() {

	nn::yb::common::DISP_HOLE_OBJ_DATA d;

	GXRgb color = GX_RGB( 31, 31, 31);
	switch(this->state) {
		case nn::yb::e::edit_hole_obj::STATE_NONE:
			if(this->activeFlag) {
				color = GX_RGB( 31, 0 , 0);
			}
			else {
				switch(this->category) {
					case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
						color = GX_RGB( 12, 31, 12);
						break;
					default:
						break;
				}
			}
			break;
		case nn::yb::e::edit_hole_obj::STATE_MOVE:
			color = GX_RGB( 31, 16, 0);
			break;
		case nn::yb::e::edit_hole_obj::STATE_TRANS:
			color = GX_RGB( 31, 31, 0);
			break;
	}

	switch(this->category) {
		case nn::yb::e::edit_hole_obj::CATEGORY_BALL:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA:
		case nn::yb::e::edit_hole_obj::CATEGORY_PILLAR:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA:
			this->setDispDataCommon( &d);
			d.color = color;
			d.dispZ = 0;
			nn::yb::common::yb_commonDispHoleObj( &d);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MOMIJI:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_TENKO:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_GREEN_FAIRY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_BLUE_FAIRY:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_REMIFLA:
		case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_NITORI:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_FLAN:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_ORIN:
			this->setDispDataCommon( &d);
			switch(this->category) {
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SKY:
					d.category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO_SKY:
					d.category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA_SKY:
					d.category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA_SKY:
					d.category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA_SKY:
					d.category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA_SKY:
					d.category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA;
					break;
			}
			d.color = color;
			d.dispZ = 0;
			nn::yb::common::yb_commonDispHoleObj( &d);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA:
			this->setDispDataCommon( &d);
			d.color = color;
			d.dispZ = 0;
			nn::yb::common::yb_commonDispHoleObj( &d);
			
			this->setDispDataCommon( &d);
			d.x1 = this->x2;
			d.y1 = this->y2;
			d.color = color;
			d.dispZ = 0;
			nn::yb::common::yb_commonDispHoleObj( &d);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP:
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_WEAK: 
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_STRONG: 
		case nn::yb::e::edit_hole_obj::CATEGORY_SE_FLOOR:
			this->setDispDataCommon( &d);
			d.color = color;
			d.dispZ = 1;
			nn::yb::common::yb_commonDispHoleObj( &d);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP:
			this->setDispDataCommon( &d);
			d.color = color;
			d.dispZ = 1;
			d.vec = this->vec;
			nn::yb::common::yb_commonDispHoleObj( &d);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_GROUND:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_DAMAGE:
		case nn::yb::e::edit_hole_obj::CATEGORY_BUNKER:
		case nn::yb::e::edit_hole_obj::CATEGORY_GRAVITY:
		case nn::yb::e::edit_hole_obj::CATEGORY_FUKIAGE: 
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_START:
		case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_GOAL:
			{
				int x1 = this->x1;
				int x2 = this->x2;;
				if(this->x1 > this->x2) {
					x1 = this->x2;
					x2 = this->x1;
				}
				int y1 = this->y1;
				int y2 = this->y2;
				if(this->y1 > this->y2) {
					y1 = this->y2;
					y2 = this->y1;
				}
				int z = 507;
				if(this->category == nn::yb::e::edit_hole_obj::CATEGORY_GROUND ||
					this->category == nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_START ||
					this->category == nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_GOAL
				) {
					if(this->activeFlag) {
						z = 509;
					}
					else {
						z = 511;
					}
				}
				else {
					if(this->activeFlag) {
						z = 505;
					}
					else {
						z = 507;
					}
				}
				if(this->activeFlag) {
					disp_commonMain3dBoard( (int)(x1 * this->hole->edit->scale) - 2, (int)(y1 * this->hole->edit->scale) - 2, z + 1, (int)((x2 - x1) * this->hole->edit->scale) + 4, (int)((y2 - y1) * this->hole->edit->scale) + 4, color, 31);
				}
				this->setDispDataCommon( &d);
				d.dispZ = z;
				d.x1 = x1;
				d.x2 = x2;
				d.y1 = y1;
				d.y2 = y2;
				d.subCategory = nn::yb::e::edit_hole_obj::GROUND_CATEGORY_GREEN;
				nn::yb::common::yb_commonDispHoleObj( &d);
			}
			break;
			
		case nn::yb::e::edit_hole_obj::CATEGORY_CAMERA:
			if(this->hole->edit->scale == 1.0) {
				disp_commonMain3dBoardCenter( this->x1, this->y1, 2, 48, 48, 0, color, ALPHA_DEFAULT);
			}
			break;
			//テンプレ
			//disp_commonMain3dBoardCenter( this->x1, this->y1, 1, 96, 96, 0, color, ALPHA_DEFAULT);
	}

}
//---------------------------------------------------------------------------
#if 1
// クラスセーブは、タスク未対応
::nn_common::DATARECODE * ::nn::yb::edit::CEditHoleObj::getDataRecode() {

	return ::nn::yb::edit::CEditHoleObj::dataRecode;

}
//---------------------------------------------------------------------------
int ::nn::yb::edit::CEditHoleObj::getClassId() {

	return nn::yb::e::yb_save_manager::CLASS_ID_EDIT_HOLE_OBJ;

}
#endif
//---------------------------------------------------------------------------
void nn::yb::edit::CEditHoleObj::setDispDataCommon( void * dispData) {

	nn::yb::common::DISP_HOLE_OBJ_DATA * d = (nn::yb::common::DISP_HOLE_OBJ_DATA *)dispData;

	memset( d, 0, sizeof( nn::yb::common::DISP_HOLE_OBJ_DATA));

	d->category = this->category;
	d->vec = this->vec;
	d->x1 = this->x1;
	d->x2 = this->x2;
	d->y1 = this->y1;
	d->y2 = this->y2;
	d->z1 = this->z1;
	d->z2 = this->z2;

	d->hScale = this->hole->edit->scale;
	d->vScale = this->hole->edit->scale;

	d->centerX = this->hole->edit->xWindow + this->hole->edit->wWindow / 2;
	d->centerY = this->hole->edit->yWindow + this->hole->edit->hWindow / 2;

}
//---------------------------------------------------------------------------
int nn::yb::edit::CEditHoleObj::getObjCategory2EnemyType(int category) {

	int type = 0;

	if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY)						{ type = nn::yb::e::enemy::TYPE_BAKEBAKE;		}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO)				{ type = nn::yb::e::enemy::TYPE_BULE_FAIRY;		}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA)				{ type = nn::yb::e::enemy::TYPE_RED_FAIRY;		}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SKY)				{ type = nn::yb::e::enemy::TYPE_BAKEBAKE;		}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO_SKY)			{ type = nn::yb::e::enemy::TYPE_BULE_FAIRY;		}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA_SKY)			{ type = nn::yb::e::enemy::TYPE_RED_FAIRY;		}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA)			{ type = nn::yb::e::enemy::TYPE_KEDAMA;			}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA_SKY)		{ type = nn::yb::e::enemy::TYPE_KEDAMA;			}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA)		{ type = nn::yb::e::enemy::TYPE_HANEMEDAMA;		}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA_SKY)	{ type = nn::yb::e::enemy::TYPE_HANEMEDAMA;		}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA)				{ type = nn::yb::e::enemy::TYPE_MSPA;			}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MOMIJI)			{ type = nn::yb::e::enemy::TYPE_MOMIJI;			}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_TENKO)				{ type = nn::yb::e::enemy::TYPE_TENKO;			}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_GREEN_FAIRY)		{ type = nn::yb::e::enemy::TYPE_GREEN_FAIRY;	}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_BLUE_FAIRY)		{ type = nn::yb::e::enemy::TYPE_BLUE_FAIRY;		}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_NITORI)			{ type = nn::yb::e::enemy::TYPE_NITORI;			}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_FLAN)				{ type = nn::yb::e::enemy::TYPE_FLAN;			}
	else if(category == nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_ORIN)				{ type = nn::yb::e::enemy::TYPE_ORIN;			}

	return type;

}
//---------------------------------------------------------------------------
int nn::yb::edit::CEditHoleObj::getObjCategory2Spell(int category) {

	int type = nn::yb::edit::CEditHoleObj::getObjCategory2EnemyType( category);
	return nn::yb::main::CEnemy::getEnemyType2Spell( type);

}
//---------------------------------------------------------------------------

