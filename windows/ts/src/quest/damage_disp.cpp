// damage_disp を置換
// DamageDisp を置換
// ts を置換
// quest を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "disp.h"
#include "anim.h"

#include "damage_disp.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::quest::CDamageDisp::dataRecode[] = {
	DATA_END,
};

nn::ts::quest::DAMGE_DISP_DISP_MEMBER nn::ts::quest::CDamageDisp::dm;
//---------------------------------------------------------------------------
nn::ts::quest::CDamageDisp::CDamageDisp(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CDamageDisp Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CDamageDisp::CDamageDisp(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CDamageDisp Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CDamageDisp::CDamageDisp() : TASK_SAVE() {

	::nn_commonLog( 1, "CDamageDisp Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CDamageDisp::constructCommon() {
	
	this->state = nn::ts::e::damage_disp::STATE_NONE;
	this->oldState = nn::ts::e::damage_disp::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->gm, 0, sizeof( this->gm));
	
}
//---------------------------------------------------------------------------
nn::ts::quest::CDamageDisp::~CDamageDisp() {

	::nn_commonLog( 1, "CDamageDisp Delete\n");

	if(this->gm.param.deleteCBFunc != NULL) {
		this->gm.param.deleteCBFunc( this->gm.param.deleteCBFuncArg);
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CDamageDisp::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CDamageDisp::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CDamageDisp::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CDamageDisp::start() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CDamageDisp::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CDamageDisp::startCommon() {

	this->state = nn::ts::e::damage_disp::STATE_ACTIVE;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CDamageDisp::damage( int damage) {
	
	this->gm.damage += damage;
	this->gm.addDamage = (this->gm.damage - this->gm.dispDamge) / this->gm.param.dmageFrameN;
	if((this->gm.damage - this->gm.dispDamge) % this->gm.param.dmageFrameN >= 1) {
		this->gm.addDamage += 1;
	}

	this->gm.exFrameCnt = 0;
	this->gm.dispFrameCnt = 0;
	this->state = nn::ts::e::damage_disp::STATE_ACTIVE;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CDamageDisp::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CDamageDisp::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::damage_disp::STATE_NONE:
			break;
		case nn::ts::e::damage_disp::STATE_ACTIVE:
			if(this->gm.moveFrameCnt < this->gm.param.moveFrameN) {
				this->gm.moveFrameCnt++;
			}
			if(this->gm.exFrameCnt < this->gm.param.exFrameN) {
				this->gm.exFrameCnt++;
			}
			if(this->gm.dispDamge < this->gm.damage) {
				this->gm.dispDamge += this->gm.addDamage;
				if(this->gm.dispDamge > this->gm.damage) {
					this->gm.dispDamge = this->gm.damage;
				}
			}

			if(this->gm.moveFrameCnt == this->gm.param.moveFrameN &&
				this->gm.exFrameCnt == this->gm.param.exFrameN &&
				this->gm.dispDamge == this->gm.damage) {
					this->state = nn::ts::e::damage_disp::STATE_DISP;
			}
		case nn::ts::e::damage_disp::STATE_DISP:
			this->gm.dispFrameCnt++;
			if(this->gm.dispFrameCnt >= this->gm.param.dispFrameN) {
				if(this->gm.deleteFlag) {
					this->taskDelete();
				}
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CDamageDisp::disp() {

	switch(this->state) {
		case nn::ts::e::damage_disp::STATE_NONE:
			break;
		case nn::ts::e::damage_disp::STATE_ACTIVE:
		case nn::ts::e::damage_disp::STATE_DISP:
			{
				int move_acc = 33;
				int x = nn::dxlib::CAnim::utilCalcMove( this->gm.param.startX, this->gm.param.endX,
					move_acc, this->gm.param.moveFrameN, this->gm.moveFrameCnt);
				int y = nn::dxlib::CAnim::utilCalcMove( this->gm.param.startY, this->gm.param.endY,
					move_acc, this->gm.param.moveFrameN, this->gm.moveFrameCnt);

				int ex_now = this->gm.exFrameCnt - this->gm.param.exFrameN / 2;
				if(ex_now < 0) {
					ex_now = -ex_now;
				}
				ex_now = this->gm.param.exFrameN / 2 - ex_now;
				int ex_acc = 66;
				int ex_rate = nn::dxlib::CAnim::utilCalcMove(
					(int)(this->gm.param.startExRate * 1000.0),
					(int)(this->gm.param.endExRate * 1000.0),
					ex_acc, this->gm.param.exFrameN, ex_now);

				nn::dxlib::disp::objNumberCenter( this->gm.dispDamge, 1,
					this->dm.objIdNumberTop,
					x, y, this->gm.param.z,
					this->gm.param.blank, ex_rate / 1000.0,
					this->gm.param.rgb, this->gm.param.alpha, this->gm.param.blend,
					DISP_NUMBER_OPTION_COMMA);
			}
			break;
	}

}//---------------------------------------------------------------------------
int nn::ts::quest::CDamageDisp::getClassId() {
	return -1;		// ToDo
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::quest::CDamageDisp::getDataRecode() {
	return nn::ts::quest::CDamageDisp::dataRecode;
}
//---------------------------------------------------------------------------

