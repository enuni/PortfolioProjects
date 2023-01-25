// master_spark を置換
// MaterSpark を置換
// ts を置換
// effect を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "disp.h"

#include "ts_common.h"

#include "master_spark.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::effect::CMasterSpark::dataRecode[] = {
	DATA_END,
};

nn::ts::effect::MASTER_SPARK_DISP_MEMBER nn::ts::effect::CMasterSpark::dm;
//---------------------------------------------------------------------------
nn::ts::effect::CMasterSpark::CMasterSpark(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CMasterSpark Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::effect::CMasterSpark::CMasterSpark(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CMasterSpark Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::effect::CMasterSpark::CMasterSpark() : TASK_SAVE() {

	::nn_commonLog( 1, "CMasterSpark Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::effect::CMasterSpark::constructCommon() {
	
	this->state = nn::ts::e::master_spark::STATE_NONE;
	this->oldState = nn::ts::e::master_spark::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->gm, 0, sizeof( this->gm));
	
}
//---------------------------------------------------------------------------
nn::ts::effect::CMasterSpark::~CMasterSpark() {

	::nn_commonLog( 1, "CMasterSpark Delete\n");

	if(this->gm.param.endCBFunc != NULL) {
		this->gm.param.endCBFunc( this->gm.param.endCBFuncArg);
	}

}
//---------------------------------------------------------------------------
void nn::ts::effect::CMasterSpark::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::effect::CMasterSpark::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::effect::CMasterSpark::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::effect::CMasterSpark::start() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::effect::CMasterSpark::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::effect::CMasterSpark::startCommon() {

	int shift_len = this->gm.param.blank + this->gm.param.w / 2;
	this->gm.dispX = (int)(this->gm.param.x + nn::common::math::angleSpeed2MoveX( this->gm.param.angle, shift_len));
	this->gm.dispY = (int)(this->gm.param.y + nn::common::math::angleSpeed2MoveY( this->gm.param.angle, shift_len));
	
	this->state = nn::ts::e::master_spark::STATE_FIRE;
}
//---------------------------------------------------------------------------
void nn::ts::effect::CMasterSpark::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CMasterSpark::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::master_spark::STATE_NONE:
			break;
		case nn::ts::e::master_spark::STATE_FIRE:
			{
				if(gm.stateFrameCnt < nn::ts::e::master_spark::FRAME_N_FIRE) {
					gm.stateFrameCnt++;
				}
				else {
					this->gm.stateFrameCnt = 1;
					this->state = nn::ts::e::master_spark::STATE_WAIT;
				}
			}
			break;
		case nn::ts::e::master_spark::STATE_WAIT:
			{
				if(gm.stateFrameCnt < nn::ts::e::master_spark::FRAME_N_WAIT) {
					gm.stateFrameCnt++;
				}
				else {
					this->gm.stateFrameCnt = 0;
					this->state = nn::ts::e::master_spark::STATE_FADE_OUT;
				}
			}
			break;
		case nn::ts::e::master_spark::STATE_FADE_OUT:
			{
				if(gm.stateFrameCnt < nn::ts::e::master_spark::FRAME_N_FADE_OUT) {
					gm.stateFrameCnt++;
				}
				else {
					this->taskDelete();
					this->state = nn::ts::e::master_spark::STATE_NONE;
				}
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::effect::CMasterSpark::disp() {

	switch(this->state) {
		case nn::ts::e::master_spark::STATE_NONE:
			break;
		case nn::ts::e::master_spark::STATE_FIRE:
			{
				int h = this->gm.param.h * gm.stateFrameCnt / nn::ts::e::master_spark::FRAME_N_FIRE;
				nn::dxlib::disp::objCenterFull( this->dm.objIdMasterSpark,
					this->gm.dispX, this->gm.dispY, nn::ts::e::common::Z_BATTLE_EFFECT,
					this->gm.param.w, h, 359 - this->gm.param.angle,
					FALSE, FALSE, DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_ADD,
					this->gm.param.cameraFlag);
			}
			break;
		case nn::ts::e::master_spark::STATE_WAIT:
			{
				nn::dxlib::disp::objCenterFull( this->dm.objIdMasterSpark,
						this->gm.dispX, this->gm.dispY, nn::ts::e::common::Z_BATTLE_EFFECT,
						this->gm.param.w, this->gm.param.h, 359 - this->gm.param.angle,
						FALSE, FALSE, DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_ADD,
						this->gm.param.cameraFlag);
				int h = this->gm.param.h * gm.stateFrameCnt / nn::ts::e::master_spark::FRAME_N_WAIT / 2;
				nn::dxlib::disp::objCenterFull( this->dm.objIdMasterSparkBase,
					this->gm.dispX, this->gm.dispY, nn::ts::e::common::Z_BATTLE_EFFECT, this->gm.param.w, h, 359 - this->gm.param.angle,
					FALSE, FALSE, DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_ADD,
					this->gm.param.cameraFlag);
			}
			break;
		case nn::ts::e::master_spark::STATE_FADE_OUT:
			{
				int alpha = 255 * (nn::ts::e::master_spark::FRAME_N_FADE_OUT - this->gm.stateFrameCnt) / nn::ts::e::master_spark::FRAME_N_FADE_OUT;
				nn::dxlib::disp::objCenterFull( this->dm.objIdMasterSpark,
						this->gm.dispX, this->gm.dispY, nn::ts::e::common::Z_BATTLE_EFFECT, this->gm.param.w, this->gm.param.h, 359 - this->gm.param.angle,
						FALSE, FALSE, DISP_WHITE, alpha, DISP_BLEND_MODE_ADD,
						this->gm.param.cameraFlag);
				nn::dxlib::disp::objCenterFull( this->dm.objIdMasterSparkBase,
					this->gm.dispX, this->gm.dispY, nn::ts::e::common::Z_BATTLE_EFFECT,
					this->gm.param.w, this->gm.param.h / 2, 359 - this->gm.param.angle,
					FALSE, FALSE, DISP_WHITE, alpha, DISP_BLEND_MODE_ADD,
					this->gm.param.cameraFlag);
			}
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::effect::CMasterSpark::getClassId() {
	return -1;		// ToDo
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::effect::CMasterSpark::getDataRecode() {
	return nn::ts::effect::CMasterSpark::dataRecode;
}
//---------------------------------------------------------------------------

