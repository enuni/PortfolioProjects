// pull を置換
// Pull を置換
// ts を置換
// quest を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "touch.h"

#include "disp.h"

#include "ts_common.h"
#include "ts_save_manager.h"

#include "pull.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::quest::CPull::dataRecode[] = {
	DATA_LOCAL( nn::ts::quest::CPull, state),
	DATA_LOCAL( nn::ts::quest::CPull, oldState),
	DATA_LOCAL( nn::ts::quest::CPull, waitCnt),
	DATA_LOCAL( nn::ts::quest::CPull, gm),
	DATA_LOCAL( nn::ts::quest::CPull, lm.decideCBFuncId),
	DATA_PTR( nn::ts::quest::CPull, lm.decideCBFuncOwner),
	DATA_BASE(::TASK_SAVE::dataRecode),
	DATA_END,
};

nn::ts::quest::PULL_DISP_MEMBER nn::ts::quest::CPull::dm;
//---------------------------------------------------------------------------
nn::ts::quest::CPull::CPull(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CPull Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CPull::CPull(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CPull Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CPull::CPull() : TASK_SAVE() {

	::nn_commonLog( 1, "CPull Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::constructCommon() {
	
	this->state = nn::ts::e::pull::STATE_NONE;
	this->oldState = nn::ts::e::pull::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->lm, 0, sizeof( this->lm));

	strcpy_s( this->name, sizeof( this->name), "CPull");
	
}
//---------------------------------------------------------------------------
nn::ts::quest::CPull::~CPull() {

	::nn_commonLog( 1, "CPull Delete\n");

}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::start() {
	
	this->startCommon();

	this->state = nn::ts::e::pull::STATE_IDLE;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::startCommon() {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CPull::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::pull::STATE_NONE:
			break;
		case nn::ts::e::pull::STATE_IDLE:
			this->taskIdle();
			break;
		case nn::ts::e::pull::STATE_PULL:
			this->taskPull();
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::disp() {

	switch(this->state) {
		case nn::ts::e::pull::STATE_NONE:
		case nn::ts::e::pull::STATE_IDLE:
			break;
		case nn::ts::e::pull::STATE_PULL:
			if(this->gm.nowPullLength >= 10.0) {
				int angle = 360 - (int)this->gm.nowAngle;
				nn::dxlib::disp::objCenterFull( this->dm.objIdArrorw,
					this->gm.dispX, this->gm.dispY, nn::ts::e::common::Z_BATTLE_ARROW,
					(int)this->gm.nowPullLength * 2, 0,
					angle,
					FALSE, FALSE, DISP_WHITE, 192, DISP_BLEND_MODE_NONE);
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::taskIdle() {
	
	::TOUCH_WORK * touch = &master->touchWork;

	if(touch->touchPoint & TOUCH_TOUCH) {
		this->state = nn::ts::e::pull::STATE_PULL;
	}
}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::taskPull() {
	
	::TOUCH_WORK * touch = &master->touchWork;
	
	if((touch->touchPoint & TOUCH_RELEASE) == 0) {
		this->gm.nowAngle = nn::common::math::pos2Angle_d(
			touch->first_x, touch->first_y,
			touch->x, touch->y);

		this->gm.nowPullLength = nn::common::math::getLength(
			touch->first_x, touch->first_y,
			touch->x, touch->y);
	}
	
	if(touch->touchPoint & TOUCH_RELEASE) {
		if(this->gm.nowPullLength >= 10.0) {
			int angle_int = (int)(this->gm.nowAngle * 1000.0);
			this->decide( angle_int);
		}
		this->state = nn::ts::e::pull::STATE_IDLE;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::decide( int angle) {

	this->gm.decideAngle = angle;

	if(this->lm.decideCBFuncId != 0) {
		void (* func)(void * endCBFuncArg, void * thisArg) =
			(void (*)(void * endCBFuncArg, void * thisArg))
				nn::common::util::CCallBackManager::get( this->lm.decideCBFuncId);
		if(func != NULL) {
			func( this->lm.decideCBFuncOwner, this);
		}
	}

	this->taskDelete();

}
//---------------------------------------------------------------------------
int nn::ts::quest::CPull::getClassId() {
	return nn::ts::e::ts_save_manager::CLASS_ID_PULL;		// ToDo
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::quest::CPull::getDataRecode() {
	return nn::ts::quest::CPull::dataRecode;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CPull::dump() {
		
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CPull::dump() start ----------\n");
	LOG_CLASS_DUMP( "this = 0x%08x\n", this);
	LOG_CLASS_DUMP( "this->state = %d\n", this->state);
	LOG_CLASS_DUMP( "this->oldState = %d\n", this->oldState);
	LOG_CLASS_DUMP( "this->waitCnt = %d\n", this->waitCnt);
	LOG_CLASS_DUMP( "this->frameCnt = %u\n", this->frameCnt);
	LOG_CLASS_DUMP( "this->gm.decideAngle = %d\n", this->gm.decideAngle);
	LOG_CLASS_DUMP( "this->gm.nowAngle = %lf\n", this->gm.nowAngle);
	LOG_CLASS_DUMP( "this->gm.nowPullLength = %lf\n", this->gm.nowPullLength);
	LOG_CLASS_DUMP( "this->gm.dispX = %d\n", this->gm.dispX);
	LOG_CLASS_DUMP( "this->gm.dispY = %d\n", this->gm.dispY);
	LOG_CLASS_DUMP( "this->gm.decideCBFuncId = %d\n", this->lm.decideCBFuncId);
	LOG_CLASS_DUMP( "this->gm.decideCBFuncOwner = 0x%08x\n", this->lm.decideCBFuncOwner);
	LOG_CLASS_DUMP( "this->parent = 0x%08x\n", this->parent);
	LOG_CLASS_DUMP( "this->childTop = 0x%08x\n", this->childTop);
	LOG_CLASS_DUMP( "this->childBottom = 0x%08x\n", this->childBottom);
	LOG_CLASS_DUMP( "this->brotherUp = 0x%08x\n", this->brotherUp);
	LOG_CLASS_DUMP( "this->brotherDown = 0x%08x\n", this->brotherDown);
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CBall::CPull() end   ----------\n");

}
//---------------------------------------------------------------------------

