// hole を置換
// Hole を置換
// yb を置換
// stage を置換

#include "nn_common.h"

#include "disp_common.h"
#include "image_ctrl.h"
#include "direct_common.h"
#include "sound_nns.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "sound_data.sadl"

#include "data_hole.h"
#include "edit_hole_obj.h"

#include "ball.h"
#include "enemy.h"
#include "cup.h"
#include "hole_ctrl.h"
#include "enum.h"
#include "ctrl.h"
#include "camera.h"
#include "yb_save_manager.h"
#include "yb_common.h"
#include "stage.h"

#include "effect/effect_ctrl.h"

#include "hole.h"
//---------------------------------------------------------------------------
nn::yb::stage::CHole::CHole(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CHole Create\n");
	this->state = nn::yb::e::hole::STATE_NONE;
	this->oldState = nn::yb::e::hole::STATE_NONE;
	this->waitCnt = 0;

	this->master = NULL;

	this->par = 0;

	this->cup = NULL;
	
	memset( this->enemy, 0, sizeof( this->enemy));
	
	memset( this->line, 0, sizeof( this->line));
	this->lineN;
	
	this->enemyN = 0;

	this->holeCtrl = NULL;

	memset( &this->data, 0, sizeof(this->data));

	this->stateLastEnemy2Cup = 0;

	this->openingCnt = 0;
	memset( this->openingX, 0, sizeof( this->openingX));
	memset( this->openingY, 0, sizeof( this->openingY));
	this->warpDispCnt = 0;

	this->startX = 0;
	this->startY = 0;
	this->endX = 0;
	this->endY = 0;

	this->frameCnt = 0;

}
//---------------------------------------------------------------------------
nn::yb::stage::CHole::~CHole() {

	::nn_commonLog( 1, "CHole Delete\n");

}
//---------------------------------------------------------------------------

void nn::yb::stage::CHole::setHole( char * buf, int size) {

	nn::yb::edit::CDataHole * load_hole = (nn::yb::edit::CDataHole *)this->saveManager->load( buf, size, false);

	this->setHole( load_hole);
	this->saveManager->ClearObjVector();
}

//---------------------------------------------------------------------------
void nn::yb::stage::CHole::setHole( char * dataFileName) {

	if(this->saveManager->isLoad( dataFileName)) {
		nn::yb::edit::CDataHole * load_hole = (nn::yb::edit::CDataHole *)this->saveManager->load( dataFileName, false);

		this->setHole( load_hole);
	}
	this->saveManager->ClearObjVector();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::setHole(nn::yb::edit::CDataHole *data) {
	
	nn::yb::main::CBall * ball = this->holeCtrl->ball;

	this->startX = INT_MAX;
	this->startY = INT_MAX;
	this->endX = INT_MIN;
	this->endY = INT_MIN;

	for( int i = 0; i < data->objCnt; i++) {
		
		bool x1y1_flag = false;
		bool x2y2_flag = false;

		nn::yb::edit::CEditHoleObj * obj = data->obj[i];
		switch(obj->category) {
			case nn::yb::e::edit_hole_obj::CATEGORY_BALL:
				{
					ball->x = (double)obj->x1;
					ball->y = (double)obj->y1;
					x1y1_flag = true;
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_PILLAR:
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
				{
					nn::yb::def::HOLE_OBJ * hole_obj = &this->data.holeObj[this->data.holeObjN];
					hole_obj->category = obj->category;
					this->data.holeObjN++;
					hole_obj->x1 = obj->x1;
					hole_obj->y1 = obj->y1;
					hole_obj->x2 = obj->x2;
					hole_obj->y2 = obj->y2;
					x1y1_flag = true;
					x2y2_flag = true;
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN:
			case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_REMIFLA:
			case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
				{
					nn::yb::def::HOLE_OBJ * hole_obj = &this->data.holeObj[this->data.holeObjN];
					hole_obj->category = obj->category;
					this->data.holeObjN++;
					hole_obj->x1 = obj->x1;
					hole_obj->y1 = obj->y1;
					hole_obj->z1 = obj->z1;
					hole_obj->x2 = obj->x2;
					hole_obj->y2 = obj->y2;
					hole_obj->z2 = obj->z2;
					x1y1_flag = true;
					x2y2_flag = true;
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE:
			case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA:
				{
					nn::yb::def::HOLE_OBJ * hole_obj = &this->data.holeObj[this->data.holeObjN];
					hole_obj->category = obj->category;
					this->data.holeObjN++;
					hole_obj->x1 = obj->x1;
					hole_obj->y1 = obj->y1;
					hole_obj->dda.x = obj->x1;
					hole_obj->dda.y = obj->y1;
					hole_obj->z1 = obj->z1;
					hole_obj->x2 = obj->x2;
					hole_obj->y2 = obj->y2;
					hole_obj->z2 = obj->z2;
					x1y1_flag = true;
					x2y2_flag = true;
				}
				break;
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
			case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA:
			case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MOMIJI:
			case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_TENKO:
			case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_GREEN_FAIRY:
			case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_BLUE_FAIRY:
			case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_NITORI:
			case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_FLAN:
			case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_ORIN:
				{
					nn::yb::main::CEnemy * enemy;
					enemy = new nn::yb::main::CEnemy( this);
					enemy->x = obj->x1;
					enemy->y = obj->y1;
					enemy->z = obj->z1;
					enemy->type = nn::yb::edit::CEditHoleObj::getObjCategory2EnemyType( obj->category);

					enemy->priority = TASK_DEFAULT_PRIORITY + 1;
					enemy->holeCtrl = this->holeCtrl;
#if 0
	//nullpo 20101113 エフェクトが終了してからCEnemyクラスを消去
#else
					enemy->effectCtrl = this->holeCtrl->effect_ctrl;
#endif
					enemy->start();
					this->enemy[this->enemyN] = enemy;
					this->enemyN++;
					x1y1_flag = true;
				}
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
			case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE:
			case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_UP:
			case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_UP:
			case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT:
			case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_DOWN:
			case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_DOWN:
			case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_DOWN:
			case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT:
			case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP:
				{
					nn::yb::def::HOLE_OBJ * hole_obj = &this->data.holeObj[this->data.holeObjN];
					hole_obj->category = obj->category;
					hole_obj->vec = obj->vec;
					this->data.holeObjN++;
					hole_obj->x1 = obj->x1;
					hole_obj->y1 = obj->y1;
					hole_obj->x2 = obj->x2;
					hole_obj->y2 = obj->y2;
					x1y1_flag = true;
					x2y2_flag = true;
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
				{
					nn::yb::def::HOLE_OBJ * hole_obj = &this->data.holeObj[this->data.holeObjN];
					hole_obj->category = obj->category;
					if(obj->x1 <= obj->x2 ) {
						hole_obj->x1 = obj->x1;
						hole_obj->x2 = obj->x2;
					}
					else {
						hole_obj->x1 = obj->x2;
						hole_obj->x2 = obj->x1;
					}
					if(obj->y1 <= obj->y2) {
						hole_obj->y1 = obj->y1;
						hole_obj->y2 = obj->y2;
					}
					else {
						hole_obj->y1 = obj->y2;
						hole_obj->y2 = obj->y1;
					}
					this->data.holeObjN++;
					x1y1_flag = true;
					x2y2_flag = true;
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_CAMERA:
				{
					this->openingX[this->openingCnt] = obj->x1;
					this->openingY[this->openingCnt] = obj->y1;
					this->openingCnt++;
					x1y1_flag = true;
				}
				break;

		}
		
		if(x1y1_flag) {
			if(obj->x1 - 480 < this->startX) {
				this->startX = obj->x1 - 480;
			}
			if(obj->y1 - 360 < this->startY) {
				this->startY = obj->y1 - 480;
			}
			if(obj->x1 + 480 > this->endX) {
				this->endX = obj->x1 + 480;
			}
			if(obj->y1 + 360 > this->endY) {
				this->endY = obj->y1 + 480;
			}
		}
		if(x2y2_flag) {
			if(obj->x2 - 480 < this->startX) {
				this->startX = obj->x2 - 480;
			}
			if(obj->y2 - 360 < this->startY) {
				this->startY = obj->y2 - 480;
			}
			if(obj->x2 + 480 > this->endX) {
				this->endX = obj->x2 + 480;
			}
			if(obj->y2 + 360 > this->endY) {
				this->endY = obj->y2 + 480;
			}
		}
		
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::start() {
	
#if 0
	this->par = 3;
	
	memset( this->enemy, 0, sizeof( this->enemy));

	nn::yb::main::CEnemy * enemy;
	enemy = new nn::yb::main::CEnemy( this);
	enemy->x = (int)this->holeCtrl->ball->x;
	enemy->y = (int)this->holeCtrl->ball->y - 200;
	enemy->priority = TASK_DEFAULT_PRIORITY + 1;
	enemy->holeCtrl = this->holeCtrl;
	this->enemy[0] = enemy;
	this->enemyN = 1;
	
	enemy = new nn::yb::main::CEnemy( this);
	enemy->x = (int)this->holeCtrl->ball->x - 200;
	enemy->y = (int)this->holeCtrl->ball->y;
	enemy->priority = TASK_DEFAULT_PRIORITY + 1;
	enemy->holeCtrl = this->holeCtrl;
	this->enemy[1] = enemy;
	this->enemyN = 2;
	
	/*
	// テスト用に敵1減
	enemy = new nn::yb::main::CEnemy( this);
	enemy->x = (int)this->holeCtrl->ball->x + 200;
	enemy->y = (int)this->holeCtrl->ball->y;
	enemy->priority = TASK_DEFAULT_PRIORITY + 1;
	enemy->holeCtrl = this->holeCtrl;
	this->enemy[2] = enemy;
	this->enemyN = 3;
	*/

	/*
	// 負荷テスト
	for( int i = 0; i < 300; i++) {

		enemy = new nn::yb::main::CEnemy( this);
		enemy->x = (int)this->holeCtrl->ball->x - 300 + 32 * (i % 20);
		enemy->y = (int)this->holeCtrl->ball->y - 220 + 32 * (i / 20);
		enemy->priority = TASK_DEFAULT_PRIORITY + 1;
		enemy->ball = this->holeCtrl->ball;
		this->enemyN++;

	}
	*/
	
	/*
	// テスト用 旧実装用
	this->line[0].s.x = (int)this->holeCtrl->ball->x - 360;
	this->line[0].s.y = (int)this->holeCtrl->ball->y - 300;
	this->line[0].e.x = (int)this->holeCtrl->ball->x - 150;
	this->line[0].e.y = (int)this->holeCtrl->ball->y + 300;
	
	this->line[1].s.x = (int)this->holeCtrl->ball->x - 320;
	this->line[1].s.y = (int)this->holeCtrl->ball->y - 240;
	this->line[1].e.x = (int)this->holeCtrl->ball->x + 320;
	this->line[1].e.y = (int)this->holeCtrl->ball->y - 200;

	this->lineN = 0;
	*/
	this->data.holeObjN = 0;
	
	// テスト用
	nn::yb::def::HOLE_OBJ * obj;
	obj = &this->data.holeObj[0];
	obj->category = nn::yb::e::hole::OBJ_CATEGORY_GROUND; obj->x1 = 0; obj->y1 = 0; obj->x2 = 250; obj->y2 = 1000;
	obj = &this->data.holeObj[1];
	obj->category = nn::yb::e::hole::OBJ_CATEGORY_GROUND; obj->x1 = 250; obj->y1 = 0; obj->x2 = 500; obj->y2 = 1000;
	this->data.holeObjN = 2;

	obj = &this->data.holeObj[2];
	obj->category = nn::yb::e::hole::OBJ_CATEGORY_BUMPER;
	obj->x1 = (int)this->holeCtrl->ball->x - 360;
	obj->y1 = (int)this->holeCtrl->ball->y - 300;
	obj->x2 = (int)this->holeCtrl->ball->x - 150;
	obj->y2 = (int)this->holeCtrl->ball->y + 300;
	
	obj = &this->data.holeObj[3];
	obj->category = nn::yb::e::hole::OBJ_CATEGORY_BUMPER;
	obj->x1 = (int)this->holeCtrl->ball->x - 320;
	obj->y1 = (int)this->holeCtrl->ball->y - 240;
	obj->x2 = (int)this->holeCtrl->ball->x + 320;
	obj->y2 = (int)this->holeCtrl->ball->y - 200;
	this->data.holeObjN = 4;

	/*
	// テスト用
	for( int i = 0; i < 128; i++) {
		
		nn::yb::def::HOLE_OBJ * obj;
		obj = &this->data.holeObj[i];
		obj->category = nn::yb::e::hole::OBJ_CATEGORY_GROUND; obj->x1 = i * 16; obj->y1 = 0; obj->x2 = (i + 1) * 16; obj->y2 = 16;
		this->data.holeObjN++;

	}
	*/
#endif

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::taskUnzanMove( nn::yb::def::HOLE_OBJ * obj) {

	if(this->holeCtrl->ball->state != nn::yb::e::ball::STATE_SPELL &&
		this->holeCtrl->state != nn::yb::e::hole_ctrl::STATE_PAUSE
	) {
		switch(obj->state) {
			case nn::yb::e::hole::UNZAN_STATE_WENT_BEFORE:
				{
					if(obj->frameCnt >= nn::yb::e::global::UNZAN_MOVE_BEFORE_WAIT_CNT) {
						obj->dda.x = obj->x1;
						obj->dda.y = obj->y1;
						obj->dda.xDst = obj->x2;
						obj->dda.yDst = obj->y2;
						obj->dda.speed = nn::yb::e::global::UNZAN_MOVE_SPEED;
						nn::common::graphics2d::initStart2GollDDA(
							obj->dda.xDst, obj->dda.yDst,
							obj->dda.x, obj->dda.y,
							obj->dda.vx, obj->dda.vy,
							obj->dda.dx, obj->dda.dy,
							obj->dda.diff
						);
						obj->frameCnt = 0;
						obj->state = nn::yb::e::hole::UNZAN_STATE_WENT;
					}
					else {
						obj->frameCnt++;
					}
				}
				break;
			case nn::yb::e::hole::UNZAN_STATE_WENT:
				{
					bool result = nn::common::graphics2d::moveStart2GollDDA(
						obj->dda.x, obj->dda.y,
						obj->dda.xDst, obj->dda.yDst,
						obj->dda.vx, obj->dda.vy,
						obj->dda.dx, obj->dda.dy,
						obj->dda.diff,
						obj->dda.speed
					);
					if(result) {
						obj->state = nn::yb::e::hole::UNZAN_STATE_COMEBACK_BEFORE;
					}
				}
				break;
			case nn::yb::e::hole::UNZAN_STATE_COMEBACK_BEFORE:
				{
					if(obj->frameCnt >= nn::yb::e::global::UNZAN_MOVE_BEFORE_WAIT_CNT) {
						obj->dda.x = obj->x2;
						obj->dda.y = obj->y2;
						obj->dda.xDst = obj->x1;
						obj->dda.yDst = obj->y1;
						obj->dda.speed = nn::yb::e::global::UNZAN_MOVE_SPEED;
						nn::common::graphics2d::initStart2GollDDA(
							obj->dda.xDst, obj->dda.yDst,
							obj->dda.x, obj->dda.y,
							obj->dda.vx, obj->dda.vy,
							obj->dda.dx, obj->dda.dy,
							obj->dda.diff
						);
						obj->frameCnt = 0;
						obj->state = nn::yb::e::hole::UNZAN_STATE_COMEBACK;
					}
					else {
						obj->frameCnt++;
					}
				}
				break;
			case nn::yb::e::hole::UNZAN_STATE_COMEBACK:
				{
					bool result = nn::common::graphics2d::moveStart2GollDDA(
						obj->dda.x, obj->dda.y,
						obj->dda.xDst, obj->dda.yDst,
						obj->dda.vx, obj->dda.vy,
						obj->dda.dx, obj->dda.dy,
						obj->dda.diff,
						obj->dda.speed
					);
					if(result) {
						obj->state = nn::yb::e::hole::UNZAN_STATE_WENT_BEFORE;
					}
				}
				break;
		}
	}

}
//---------------------------------------------------------------------------

void nn::yb::stage::CHole::taskOku( nn::yb::def::HOLE_OBJ * obj) {

	if(this->holeCtrl->ball->state != nn::yb::e::ball::STATE_SPELL &&
		this->holeCtrl->ball->state != nn::yb::e::ball::STATE_CUPIN &&
		this->holeCtrl->ball->state != nn::yb::e::ball::STATE_CUPIN_DISP &&
		this->holeCtrl->state != nn::yb::e::hole_ctrl::STATE_PAUSE
	) {
		if(obj->state == nn::yb::e::hole::OKU_STATE_NORMAL) {
			if(obj->frameCnt >= nn::yb::e::global::OKU_NORMAL_2_ATTACK_FRAME_CNT) {
				obj->frameCnt = 0;
				obj->state = nn::yb::e::hole::OKU_STATE_ATTACK_BEFORE;
			}
			else {
				obj->frameCnt++;
			}
		}
		else if(obj->state == nn::yb::e::hole::OKU_STATE_ATTACK_BEFORE) {
			if(obj->frameCnt >= 45) {
				obj->frameCnt = 0;
				obj->state = nn::yb::e::hole::OKU_STATE_ATTACK;
			}
			else {
				obj->frameCnt++;
			}
		}
		else {
			// OKU_STATE_ATTACK
			if(obj->frameCnt >= nn::yb::e::global::OKU_ATTACK_2_NORMAL_FRAME_CNT) {
				obj->frameCnt = 0;
				obj->state = nn::yb::e::hole::OKU_STATE_NORMAL;
			}
			else {
				obj->frameCnt++;
				if(obj->frameCnt % 5 == 0) {
					::sound_nnsPlaySe( SE_BA);

				}

				if (obj->frameCnt % 2 == 0) {
					//弾エフェクト
					const int ciAnimeFrame = 15;
					int iDispZ = nn::yb::e::global::DISP_Z_BALL_UP_OBJ - 1;
					if(obj->y1 >= this->holeCtrl->ball->y) {
						iDispZ = nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ - 1;
					}

					OBJ * bullet = disp_commonGetObj( OBJ_ID_COMMON_EFFECT_BULLET_0);
					int x = obj->x1 + n_commonRandN( 48) - 24;
					this->holeCtrl->effect_ctrl->push_hole_event( NULL, this, NULL);
					nn::yb::main::effect_start_end_total_hole( this->holeCtrl, OBJ_ID_COMMON_EFFECT_BULLET_0,
						nn::yb::enum_effect::SLIDE_MODE_FRAME,
						ciAnimeFrame,
						x,
						x, ciAnimeFrame,
						obj->y1 + 24 - obj->z1,
						obj->y1 + 8, ciAnimeFrame,
						iDispZ,
						bullet->w, bullet->w, ciAnimeFrame,
						bullet->h, bullet->h, ciAnimeFrame,
						0 , 0 , ciAnimeFrame,
						31, 31, ciAnimeFrame,
						DISP_COMMON_BLEND_MODE_ADD, nn::yb::enum_effect::EFFECT_ANIME_FLAT, GX_RGB( 31, 20, 20),
						this->holeCtrl->effect_ctrl,
						nn::yb::main::CEffectCtrl::cb_pop_hole_event
						);//拡大
				}
				if (obj->frameCnt % 5 == 0) {
					//弾エフェクト
					const int ciAnimeFrame = 15;
					int iDispZ = nn::yb::e::global::DISP_Z_BALL_UP_OBJ - 1;
					if(obj->y1 >= this->holeCtrl->ball->y) {
						iDispZ = nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ - 1;
					}

					OBJ * bullet = disp_commonGetObj( OBJ_ID_COMMON_EFFECT_BULLET_1);
					int x = obj->x1 + n_commonRandN( 48) - 24;
					this->holeCtrl->effect_ctrl->push_hole_event( NULL, this, NULL);
					nn::yb::main::effect_start_end_total_hole( this->holeCtrl, OBJ_ID_COMMON_EFFECT_BULLET_1,
						nn::yb::enum_effect::SLIDE_MODE_FRAME,
						ciAnimeFrame,
						x,
						x, ciAnimeFrame,
						obj->y1 + 24 - obj->z1,
						obj->y1 + 8, ciAnimeFrame,
						iDispZ,
						bullet->w, bullet->w, ciAnimeFrame,
						bullet->h, bullet->h, ciAnimeFrame,
						0 , 0 , ciAnimeFrame,
						31, 31, ciAnimeFrame,
						DISP_COMMON_BLEND_MODE_ADD, nn::yb::enum_effect::EFFECT_ANIME_FLAT, GX_RGB( 31, 4, 4),
						this->holeCtrl->effect_ctrl,
						nn::yb::main::CEffectCtrl::cb_pop_hole_event);//拡大
				}
			}
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CHole::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	this->frameCnt++;

	::KEY_WORK * key = &this->master->keyWork;
	
	for( int i = 0; i < this->data.holeObjN; i++) {

		nn::yb::def::HOLE_OBJ * obj = &this->data.holeObj[i];
		
		switch(obj->category) {
			case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
				this->taskOku( obj);
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE:
			case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA:
				this->taskUnzanMove( obj);
				break;
		}

	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::disp() {

	nn::yb::common::DISP_HOLE_OBJ_DATA d;

	int obj_id_bg = OBJ_ID_BG_ITEM0_0;
	switch(this->holeCtrl->stage->stageNo) {
		case 1:
		case 2:
			obj_id_bg = OBJ_ID_BG_ITEM0_0;
			break;
		case 3:
		case 4:
			obj_id_bg = OBJ_ID_BG_ITEM0_1;
			break;
		case 5:
			obj_id_bg = OBJ_ID_BG_ITEM0_2;
			break;
		case 6:
			obj_id_bg = OBJ_ID_BG_ITEM0_3;
			break;
		case 7:
		case 8:
			break;
	}
	for( int i = 0; i < (int)((this->endY - this->startY) / 512) + 3; i++) {

		for( int j = 0; j < (int)((this->endX - this->startX) / 512) + 3; j++) {

			disp_commonObj( obj_id_bg, this->startX + (j - 1) * 512, this->startY + (i - 1) * 512, nn::yb::e::global::DISP_Z_BG);

		}

	}

	this->warpDispCnt = 0;

	for( int i = 0; i < this->data.holeObjN; i++) {

		nn::yb::def::HOLE_OBJ * obj = &this->data.holeObj[i];
		switch(obj->category) {
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
				this->dispGround( obj);
				break;

			case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
				this->setDispDataCommon( &d, obj);
				d.color = GX_RGB( 12, 31, 12);
				d.dispZ = nn::yb::e::global::DISP_Z_BUMPER;
				nn::yb::common::yb_commonDispHoleObj( &d);
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_PILLAR:
			case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN:
			case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_REMIFLA:
				{
					this->setDispDataCommon( &d, obj);
					d.z1 = obj->z1;
					d.z2 = obj->z2;
					d.frameCnt = this->frameCnt;
					d.dispZ = nn::yb::e::global::DISP_Z_BALL_UP_OBJ;
					if(obj->y1 >= this->holeCtrl->ball->y) {
						d.dispZ = nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ;
					}
					nn::yb::common::yb_commonDispHoleObj( &d);
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE:
			case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA:
				{
					this->setDispDataCommon( &d, obj);
					d.x1 = obj->dda.x;
					d.y1 = obj->dda.y;
					d.z1 = obj->z1;
					d.z2 = obj->z2;
					d.frameCnt = this->frameCnt;
					d.dispZ = nn::yb::e::global::DISP_Z_BALL_UP_OBJ;
					if(obj->y1 >= this->holeCtrl->ball->y) {
						d.dispZ = nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ;
					}
					nn::yb::common::yb_commonDispHoleObj( &d);
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
				{
					this->setDispDataCommon( &d, obj);
					d.z1 = obj->z1;
					d.z2 = obj->z2;
					d.dispZ = nn::yb::e::global::DISP_Z_BALL_UP_OBJ;
					if(obj->y1 >= this->holeCtrl->ball->y) {
						d.dispZ = nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ;
					}

					if(obj->state == nn::yb::e::hole::OKU_STATE_NORMAL) {
						d.subCategory = nn::yb::e::edit_hole_obj::OKU_CATEGORY_NORMAL;
						d.frameCnt = obj->frameCnt;
					}
					else if(obj->state == nn::yb::e::hole::OKU_STATE_ATTACK_BEFORE) {
						d.subCategory = nn::yb::e::edit_hole_obj::OKU_CATEGORY_NORMAL;
						d.frameCnt = 0;
					}
					else {
						// OKU_STATE_ATTACK
						d.subCategory = nn::yb::e::edit_hole_obj::OKU_CATEGORY_ATTACK;
					}
					nn::yb::common::yb_commonDispHoleObj( &d);
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
				this->dispWarp( obj);
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP:
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_RIGHT:
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN:
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_LEFT:
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_RIGHT:
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_LEFT:
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_RIGHT:
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_LEFT:
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
				this->setDispDataCommon( &d, obj);
				d.dispZ = nn::yb::e::global::DISP_Z_WARP;
				nn::yb::common::yb_commonDispHoleObj( &d);
				break;
				//テンプレ
				//disp_commonMain3dBoardCenter( obj->x1, obj->y1, nn::yb::e::global::DISP_Z_WARP, 96, 96, 0, RGB_WHITE, ALPHA_DEFAULT);
		}

	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::dispWarp( nn::yb::def::HOLE_OBJ * obj) {
	
	nn::yb::common::DISP_HOLE_OBJ_DATA d;

	GXRgb color_tbl[16] = {
		RGB_WHITE,
		RGB_RED,				RGB_BLUE,				RGB_GREEN,
		GX_RGB( 31, 31,  0),	GX_RGB( 31,  0, 31),	GX_RGB(  0, 31, 31),
		GX_RGB( 31,  8,  8),	GX_RGB(  8, 31,  8),	GX_RGB(  8,  8, 31),
		GX_RGB( 31, 16,  0),	GX_RGB( 31,  0, 16),
		GX_RGB( 16, 31,  0),	GX_RGB(  0, 31, 16),
		GX_RGB(  0, 16, 31),	GX_RGB( 16,  0, 31),
	};

	int color_index = this->warpDispCnt % 16;
	GXRgb color = color_tbl[color_index];
	this->warpDispCnt++;

	this->setDispDataCommon( &d, obj);
	d.color = color;
	d.dispZ = nn::yb::e::global::DISP_Z_WARP;
	d.vec = obj->vec;
	nn::yb::common::yb_commonDispHoleObj( &d);

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::dispGround( nn::yb::def::HOLE_OBJ * obj) {

	int x1 = obj->x1;
	int x2 = obj->x2;;
	if(obj->x1 > obj->x2) {
		x1 = obj->x2;
		x2 = obj->x1;
	}
	int y1 = obj->y1;
	int y2 = obj->y2;
	if(obj->y1 > obj->y2) {
		y1 = obj->y2;
		y2 = obj->y1;
	}
	int z = nn::yb::e::global::DISP_Z_GROUND;
	
	nn::yb::common::DISP_HOLE_OBJ_DATA disp_data;
	nn::yb::common::DISP_HOLE_OBJ_DATA * d = (nn::yb::common::DISP_HOLE_OBJ_DATA *)&disp_data;

	memset( d, 0, sizeof( nn::yb::common::DISP_HOLE_OBJ_DATA));

	d->category = obj->category;
	if(this->holeCtrl->stage->stageNo > nn::yb::e::global::STAGE_N / 2) {
		d->subCategory = nn::yb::e::edit_hole_obj::GROUND_CATEGORY_RED;
	}
	else {
		d->subCategory = nn::yb::e::edit_hole_obj::GROUND_CATEGORY_GREEN;
	}
	d->dispZ = z;
	d->x1 = x1;
	d->x2 = x2;
	d->y1 = y1;
	d->y2 = y2;
	d->hScale = 1.0;
	d->vScale = 1.0;
	d->frameCnt = this->frameCnt;
	nn::yb::common::yb_commonDispHoleObj( d);

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::defeatedEnemy( void * enemy) {

	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE(this->enemy); i++) {

		if(this->enemy[i] == enemy) {
			this->enemy[i] = NULL;
			break;
		}

	}
	this->enemyN--;

	if(this->enemyN == 1) {
		this->lastEnemy2Cup();
		this->enemyN = 0;
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::lastEnemy2Cup() {

	this->stateLastEnemy2Cup = 0;
	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE(this->enemy); i++) {

		if(this->enemy[i] != NULL) {
			nn::yb::main::CEnemy * last = this->enemy[i];
			this->enemyLast = last;
			if((int)this->holeCtrl->ball->x - nn::yb::e::global::WINDOW_W_HALF <= last->x &&
				last->x <= (int)this->holeCtrl->ball->x + nn::yb::e::global::WINDOW_W_HALF &&
				(int)this->holeCtrl->ball->y - nn::yb::e::global::WINDOW_H_HALF <= last->y &&
				last->y <= (int)this->holeCtrl->ball->y + nn::yb::e::global::WINDOW_H_HALF
			) {
				// カメラ移動なし
				//this->lastEnemy2CupCbExec();
				this->holeCtrl->ctrl->camera->moveCameraPos( last->x, last->y,
					nn::yb::stage::CHole::lastEnemy2CupExecCb, this
				);
				this->holeCtrl->ball->procWaitCnt++;
			}
			else {
				this->holeCtrl->ctrl->camera->moveCameraPos( last->x, last->y,
					nn::yb::stage::CHole::lastEnemy2CupExecCb, this
				);
				this->holeCtrl->ball->procWaitCnt++;
			}

			/*// 演出なし
			nn::yb::main::CEnemy * last = this->enemy[i];
			this->cup = new nn::yb::stage::CCup( this);
			cup->x = last->x;
			cup->y = last->y;
			last->taskDelete();*/
		}

	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::lastEnemy2CupExecCb(void *argHole) {

	nn::yb::stage::CHole * hole = (nn::yb::stage::CHole *)argHole;
	
	hole->lastEnemy2CupCbExec();

}

//---------------------------------------------------------------------------
void nn::yb::stage::CHole::lastEnemy2CupCbExec() {

	nn::yb::main::CEnemy * last = this->enemyLast;

	if(this->stateLastEnemy2Cup == 0) {
		this->cup = new nn::yb::stage::CCup( this);
		cup->x = last->x;
		cup->y = last->y;
		last->taskDelete();

		::direct_commonWaitSimple( NULL, 30, lastEnemy2CupExecCb, this);
		this->stateLastEnemy2Cup = 1;
	}
	else {
		if((int)this->holeCtrl->ball->x - nn::yb::e::global::WINDOW_W_HALF <= last->x &&
			last->x <= (int)this->holeCtrl->ball->x + nn::yb::e::global::WINDOW_W_HALF &&
			(int)this->holeCtrl->ball->y - nn::yb::e::global::WINDOW_H_HALF <= last->y &&
			last->y <= (int)this->holeCtrl->ball->y + nn::yb::e::global::WINDOW_H_HALF
		) {
			// カメラ移動なし
			//this->lastEnemy2CupAfter();
			this->holeCtrl->ctrl->camera->moveCameraPos(
				(int)this->holeCtrl->ball->x, (int)this->holeCtrl->ball->y,
				nn::yb::stage::CHole::lastEnemy2CupAfterCb, this
			);
		}
		else {
			this->holeCtrl->ctrl->camera->moveCameraPos(
				(int)this->holeCtrl->ball->x, (int)this->holeCtrl->ball->y,
				nn::yb::stage::CHole::lastEnemy2CupAfterCb, this
			);
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::lastEnemy2CupAfterCb(void *argHole) {

	nn::yb::stage::CHole * hole = (nn::yb::stage::CHole *)argHole;
	
	hole->lastEnemy2CupAfter();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::lastEnemy2CupAfter() {

	this->holeCtrl->ball->procWaitCnt--;

}
//---------------------------------------------------------------------------
void nn::yb::stage::CHole::setDispDataCommon(void * dispData, void * obj) {

	nn::yb::common::DISP_HOLE_OBJ_DATA * d = (nn::yb::common::DISP_HOLE_OBJ_DATA *)dispData;
	nn::yb::def::HOLE_OBJ * o = (nn::yb::def::HOLE_OBJ *)obj;

	memset( d, 0, sizeof( nn::yb::common::DISP_HOLE_OBJ_DATA));

	d->category = o->category;
	d->vec = 0;
	d->x1 = o->x1;
	d->x2 = o->x2;
	d->y1 = o->y1;
	d->y2 = o->y2;
	d->z1 = 0;
	d->z2 = 0;

	d->hScale = 1.0;
	d->vScale = 1.0;

	d->color = RGB_WHITE;

}
//---------------------------------------------------------------------------
