// laser を置換
// Laser を置換
// ts を置換
// effect を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "disp.h"

#include "ts_common.h"

#include "effect_laser.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::effect::CLaser::dataRecode[] = {
	DATA_END,
};

nn::ts::effect::LASER_DISP_MEMBER nn::ts::effect::CLaser::dm;
//---------------------------------------------------------------------------
nn::ts::effect::CLaser::CLaser(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CLaser Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::effect::CLaser::CLaser(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CLaser Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::effect::CLaser::CLaser() : TASK_SAVE() {

	::nn_commonLog( 1, "CLaser Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::effect::CLaser::constructCommon() {
	
	this->state = nn::ts::e::effect_laser::STATE_NONE;
	this->oldState = nn::ts::e::effect_laser::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->gm, 0, sizeof( this->gm));
	
}
//---------------------------------------------------------------------------
nn::ts::effect::CLaser::~CLaser() {

	::nn_commonLog( 1, "CLaser Delete\n");

	if(this->gm.param.endCBFunc != NULL) {
		this->gm.param.endCBFunc( this->gm.param.endCBFuncArg);
	}

}
//---------------------------------------------------------------------------
void nn::ts::effect::CLaser::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::effect::CLaser::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::effect::CLaser::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::effect::CLaser::start() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::effect::CLaser::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::effect::CLaser::startCommon() {

	this->state = nn::ts::e::effect_laser::STATE_FIRE;

}
//---------------------------------------------------------------------------
void nn::ts::effect::CLaser::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CLaser::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
	case nn::ts::e::effect_laser::STATE_NONE:
			break;
		case nn::ts::e::effect_laser::STATE_FIRE:
			this->gm.dispW += this->gm.param.speed;
			if(this->gm.dispW >= this->gm.param.w) {
				this->gm.dispW = this->gm.param.w;
				if(this->gm.param.landingCBFunc != NULL) {
					this->gm.param.landingCBFunc( this->gm.param.landingCBFuncArg);
				}
				this->state = nn::ts::e::effect_laser::STATE_WAIT;
			}
			break;
		case nn::ts::e::effect_laser::STATE_WAIT:
			{
				if(this->gm.stateFrameCnt < nn::ts::e::effect_laser::FRAME_N_WAIT) {
					this->gm.stateFrameCnt++;
				}
				else {
					this->gm.stateFrameCnt = 0;
					this->state = nn::ts::e::effect_laser::STATE_FADE_OUT;
				}
			}
			break;
		case nn::ts::e::effect_laser::STATE_FADE_OUT:
			{
				if(this->gm.stateFrameCnt < nn::ts::e::effect_laser::FRAME_N_FADE_OUT) {
					this->gm.stateFrameCnt++;
				}
				else {
					this->gm.stateFrameCnt = 0;
					this->taskDelete();
				}
			}
			break;

	}

}
//---------------------------------------------------------------------------
void nn::ts::effect::CLaser::disp() {

	switch(this->state) {
		case nn::ts::e::effect_laser::STATE_NONE:
			break;
		case nn::ts::e::effect_laser::STATE_FIRE:
			{
				int shift_len = this->gm.param.blank + this->gm.dispW / 2;
				int disp_x = (int)(this->gm.param.x + nn::common::math::angleSpeed2MoveX( this->gm.param.angle, shift_len));
				int disp_y = (int)(this->gm.param.y + nn::common::math::angleSpeed2MoveY( this->gm.param.angle, shift_len));
				nn::dxlib::disp::objCenterFull( this->dm.objIdLaser[this->gm.param.atribute],
					disp_x, disp_y, nn::ts::e::common::Z_BATTLE_EFFECT,
					this->gm.dispW, this->gm.param.h, 359 - this->gm.param.angle, FALSE, FALSE,
					DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_ADD, this->gm.param.cameraFlag);

			}
			break;
		case nn::ts::e::effect_laser::STATE_WAIT:
			{
				int shift_len = this->gm.param.blank + this->gm.param.w / 2;
				int disp_x = (int)(this->gm.param.x + nn::common::math::angleSpeed2MoveX( this->gm.param.angle, shift_len));
				int disp_y = (int)(this->gm.param.y + nn::common::math::angleSpeed2MoveY( this->gm.param.angle, shift_len));
				nn::dxlib::disp::objCenterFull( this->dm.objIdLaser[this->gm.param.atribute],
					disp_x, disp_y, nn::ts::e::common::Z_BATTLE_EFFECT,
					this->gm.param.w, this->gm.param.h, 359 - this->gm.param.angle, FALSE, FALSE,
					DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_ADD, this->gm.param.cameraFlag);

			}
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::effect::CLaser::getClassId() {
	return -1;		// ToDo
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::effect::CLaser::getDataRecode() {
	return nn::ts::effect::CLaser::dataRecode;
}
//---------------------------------------------------------------------------

