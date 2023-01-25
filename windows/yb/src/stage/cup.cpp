// cup を置換
// Cup を置換
// yb を置換
// stage を置換

#include "nn_common.h"

#include "disp_common.h"

#include "obj_main_3d_data.h"

#include "enum.h"

#include "cup.h"

//---------------------------------------------------------------------------
nn::yb::stage::CCup::CCup(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CCup Create\n");
	this->state = nn::yb::e::cup::STATE_NONE;
	this->oldState = nn::yb::e::cup::STATE_NONE;
	this->waitCnt = 0;

	this->x = 0;
	this->y = 0;
	this->r = 24;

}

//---------------------------------------------------------------------------
nn::yb::stage::CCup::~CCup() {

	::nn_commonLog( 1, "CCup Delete\n");

}

//---------------------------------------------------------------------------
void nn::yb::stage::CCup::start() {

}
//---------------------------------------------------------------------------
void nn::yb::stage::CCup::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CCup::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CCup::disp() {
	
	disp_commonObjCenterBasic( OBJ_ID_MAIN_CUP_0, this->x, this->y - 3, nn::yb::e::global::DISP_Z_CUP);

}
//---------------------------------------------------------------------------
