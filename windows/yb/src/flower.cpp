// flower を置換
// Flower を置換
// yb を置換
// main を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "disp_common.h"

#include "obj_main_3d_data.h"

#include "flower.h"
//---------------------------------------------------------------------------
nn::yb::main::CFlower::CFlower(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CFlower Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructorCommon();

}
//---------------------------------------------------------------------------
nn::yb::main::CFlower::CFlower(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CFlower Create(TASK_SAVE * oya)\n");
	
	this->constructorCommon();

}
//---------------------------------------------------------------------------
void nn::yb::main::CFlower::constructorCommon() {
	
	this->state = nn::yb::e::flower::STATE_NONE;
	this->oldState = nn::yb::e::flower::STATE_NONE;
	this->waitCnt = 0;

	this->x = 0;
	this->y = 0;
	this->gx = 0;
	this->gy = 0;

	this->vx = 0;
	this->vy = 0;
	this->dx = 0;
	this->dy = 0;
	this->diff = 0;

	this->objId = 0;
	
}
//---------------------------------------------------------------------------
nn::yb::main::CFlower::~CFlower() {

	::nn_commonLog( 1, "CFlower Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::main::CFlower::start() {

	this->gx = this->x + 100;
	this->gy = this->y + 600;

	nn::common::graphics2d::initStart2GollDDA( this->gx, this->gy,
		this->x, this->y, this->vx, this->vy, this->dx, this->dy,
		this->diff
	);

	this->state = nn::yb::e::flower::STATE_MOVE_NORMAL;

}
//---------------------------------------------------------------------------
void nn::yb::main::CFlower::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CFlower::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;

	
	switch(this->state) {
		case nn::yb::e::flower::STATE_NONE:
			break;
		case nn::yb::e::flower::STATE_MOVE_HIDE:
		case nn::yb::e::flower::STATE_MOVE_NORMAL:
			{
				bool result = nn::common::graphics2d::moveStart2GollDDA( this->x, this->y,
					this->gx, this->gy, this->vx, this->vy, this->dx, this->dy, this->diff,
					1
				);

				if(result) {
					this->taskDelete();
				}
			}
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::main::CFlower::disp() {

	switch(this->state) {
		case nn::yb::e::flower::STATE_NONE:
		case nn::yb::e::flower::STATE_MOVE_HIDE:
			break;
		case nn::yb::e::flower::STATE_MOVE_NORMAL:
			disp_commonObjCenter( this->objId, this->x, this->y, 30, 30, -50, 0, FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT);
			break;
	}

}
//---------------------------------------------------------------------------

