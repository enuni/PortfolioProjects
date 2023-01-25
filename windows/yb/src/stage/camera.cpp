// camera を置換
// Camera を置換
// yb を置換
// stage を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "disp_common.h"

#include "camera.h"

#include "effect/effect_ctrl.h"

//---------------------------------------------------------------------------
nn::yb::stage::CCamera::CCamera(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CCamera Create\n");
	this->state = nn::yb::e::camera::STATE_NONE;
	this->oldState = nn::yb::e::camera::STATE_NONE;
	this->waitCnt = 0;
	
	this->x = 0;
	this->y = 0;
	this->xDst = 0;
	this->yDst = 0;
	
	this->vx = 0;
	this->vy = 0;
	this->dx = 0;
	this->dy = 0;
	this->diff = 0;

	this->moveEndCb = NULL;
	this->moveEndCbArg = NULL;

}
//---------------------------------------------------------------------------
nn::yb::stage::CCamera::~CCamera() {

	::nn_commonLog( 1, "CCamera Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::stage::CCamera::start() {

	this->state = nn::yb::e::camera::STATE_STANDBY;

}
//---------------------------------------------------------------------------
void nn::yb::stage::CCamera::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CCamera::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;

	switch(this->state) {
		case nn::yb::e::camera::STATE_STANDBY:
			break;
		case nn::yb::e::camera::STATE_MOVE:
			bool end_flag = nn::common::graphics2d::moveStart2GollDDA(
				this->x, this->y, this->xDst, this->yDst, this->vx, this->vy,
				this->dx, this->dy, this->diff, this->speed
			);
			disp_commonSetCameraPos( this->x, this->y);
			nn::yb::main::CEffectCtrl::setCameraPos((float)x, (float)y);//エフェクトのカメラオフセットを指定

			if(end_flag) {
				this->state = nn::yb::e::camera::STATE_STANDBY;
				if(this->moveEndCb != NULL) {
					void (* func)( void *) = this->moveEndCb;
					void * arg = this->moveEndCbArg;
					this->moveEndCb = NULL;
					this->moveEndCbArg = NULL;
					func( arg);
					/*
					// コールバックの中でカメラ移動を続けて設定したら、動けなくなる
					this->moveEndCb = NULL;
					this->moveEndCbArg = NULL;
					*/
				}
				/*
				// コールバックの中でカメラ移動を続けて設定したら、動けなくなる
				this->state = nn::yb::e::camera::STATE_STANDBY;
				*/
			}
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::stage::CCamera::disp() {
	
	
	

}
//---------------------------------------------------------------------------
void nn::yb::stage::CCamera::moveCameraPos(int x, int y) {

	this->moveCameraPos( x, y, 20, NULL, NULL);

}
//---------------------------------------------------------------------------
void nn::yb::stage::CCamera::setCameraPos(int x, int y) {

	disp_commonSetCameraPos( x, y);
	this->x = x;
	this->y = y;
	this->state = nn::yb::e::camera::STATE_STANDBY;

	nn::yb::main::CEffectCtrl::setCameraPos((float)x, (float)y);//エフェクトのカメラオフセットを指定
}
//---------------------------------------------------------------------------
void nn::yb::stage::CCamera::moveCameraPos( int x, int y, int speed, void (* cb)(void *), void * cbArg) {

	nn::common::graphics2d::initStart2GollDDA( x, y, this->x, this->y, this->vx, this->vy,
		this->dx, this->dy, this->diff
	);
	this->moveEndCb = cb;
	this->moveEndCbArg = cbArg;
	this->xDst = x;
	this->yDst = y;
	this->speed = speed;
	this->state = nn::yb::e::camera::STATE_MOVE;

}
//---------------------------------------------------------------------------
void nn::yb::stage::CCamera::moveCameraPos( int x, int y, void (* cb)(void *), void * cbArg) {

	this->moveCameraPos( x, y, 20, cb, cbArg);

}
//---------------------------------------------------------------------------

