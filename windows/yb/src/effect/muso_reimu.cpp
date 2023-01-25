// muso_reimu を置換
// MusoReimu を置換
// yb を置換
// effect を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "disp_common.h"

#include "muso_reimu.h"

//---------------------------------------------------------------------------
static const int sMaxFrameArray[] = {
	0,			// STATE_NONE
	30,			// STATE_IN
	120,		// STATE_SETUP
	62,			// STATE_HANDS_UP
	0,			// STATE_FIRE
	18,			// STATE_FIRE_STOP
	4,			// STATE_HANDS_DOWN
	10,			// STATE_END
	12,			// STATE_OUT
};

static const nn::yb::e::effect::muso_reimu::STATE sNextStateArray[] = {
	nn::yb::e::effect::muso_reimu::STATE_NONE,			// STATE_NONE
	nn::yb::e::effect::muso_reimu::STATE_SETUP,			// STATE_IN
	nn::yb::e::effect::muso_reimu::STATE_HANDS_UP,		// STATE_SETUP
	nn::yb::e::effect::muso_reimu::STATE_FIRE,			// STATE_HANDS_UP
	nn::yb::e::effect::muso_reimu::STATE_FIRE_STOP,		// STATE_FIRE
	nn::yb::e::effect::muso_reimu::STATE_HANDS_DOWN,	// STATE_FIRE_STOP
	nn::yb::e::effect::muso_reimu::STATE_END,			// STATE_HANDS_DOWN
	nn::yb::e::effect::muso_reimu::STATE_OUT,			// STATE_END
	nn::yb::e::effect::muso_reimu::STATE_NONE,			// STATE_OUT
};
//---------------------------------------------------------------------------
nn::yb::effect::CMusoReimu::CMusoReimu(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CMusoReimu Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructorCommon();

}
//---------------------------------------------------------------------------
nn::yb::effect::CMusoReimu::CMusoReimu(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CMusoReimu Create(TASK_SAVE * oya)\n");
	
	this->constructorCommon();

}
//---------------------------------------------------------------------------
void nn::yb::effect::CMusoReimu::constructorCommon() {
	
	this->state = nn::yb::e::effect::muso_reimu::STATE_NONE;
	this->oldState = nn::yb::e::effect::muso_reimu::STATE_NONE;
	this->waitCnt = 0;

	this->frameCnt = 0;
	this->x = 0;
	this->y = 0;
	this->dispZ = 0;
	this->objIdReimuTop = 0;
	this->objIdYukkuri = 0;
	this->fireScene = 0;
	this->endCb = NULL;
	this->endCbArg = NULL;
	
}
//---------------------------------------------------------------------------
nn::yb::effect::CMusoReimu::~CMusoReimu() {

	::nn_commonLog( 1, "CMusoReimu Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::effect::CMusoReimu::start() {

	this->state = nn::yb::e::effect::muso_reimu::STATE_IN;

}
//---------------------------------------------------------------------------
void nn::yb::effect::CMusoReimu::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CMusoReimu::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	
	switch(this->state) {
		case nn::yb::e::effect::muso_reimu::STATE_NONE:
			break;
		case nn::yb::e::effect::muso_reimu::STATE_IN:
		case nn::yb::e::effect::muso_reimu::STATE_SETUP:
		case nn::yb::e::effect::muso_reimu::STATE_HANDS_UP:
		case nn::yb::e::effect::muso_reimu::STATE_FIRE_STOP:
		case nn::yb::e::effect::muso_reimu::STATE_HANDS_DOWN:
		case nn::yb::e::effect::muso_reimu::STATE_END:
			this->frameCnt++;
			if(this->frameCnt > ::sMaxFrameArray[this->state]) {
				this->state = ::sNextStateArray[this->state];
				this->frameCnt = 0;
			}
			break;
		case nn::yb::e::effect::muso_reimu::STATE_FIRE:
			{
				this->frameCnt++;
				if(this->frameCnt > 6) {
					this->fireScene++;
					if(this->fireScene > 8) {
						this->state = ::sNextStateArray[this->state];
						this->fireScene = 0;
					}
					this->frameCnt = 0;
				}
			}
			break;
		case nn::yb::e::effect::muso_reimu::STATE_OUT:
			this->frameCnt++;
			if(this->frameCnt >= ::sMaxFrameArray[this->state]) {
				if( this->endCb != NULL) {
					this->endCb( this->endCbArg);
				}
				this->endCb = NULL;
			}
			if(this->frameCnt > ::sMaxFrameArray[this->state]) {
				this->state = ::sNextStateArray[this->state];
				this->frameCnt = 0;
				this->taskDelete();
			}
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::effect::CMusoReimu::disp() {

	int obj_id_array[] = {
		0,							// STATE_NONE,
		this->objIdReimuTop,		// STATE_IN,
		this->objIdReimuTop,		// STATE_SETUP,
		this->objIdReimuTop + 1,	// STATE_HANDS_UP,
		this->objIdReimuTop + 2,	// STATE_FIRE,
		this->objIdReimuTop + 1,	// STATE_FIRE_STOP,
		this->objIdReimuTop + 4,	// STATE_HANDS_DOWN,
		this->objIdReimuTop,		// STATE_NED,
		this->objIdReimuTop,		// STATE_OUT,
	};
	int reimu_y_shift = - 16;

	switch(this->state) {
		case nn::yb::e::effect::muso_reimu::STATE_NONE:
			break;
		case nn::yb::e::effect::muso_reimu::STATE_IN:
			{
				int alpha_in = (int)( 31.0 * ( (double)this->frameCnt / (double)sMaxFrameArray[this->state] ) );
				disp_commonObjCenter( obj_id_array[this->state], this->x, this->y + reimu_y_shift, 0, 0, this->dispZ,
					0, FALSE, FALSE, RGB_WHITE, alpha_in
				);

				int alpha_out = (int)( 31.0 * ( (double)(sMaxFrameArray[this->state] - this->frameCnt) / (double)sMaxFrameArray[this->state] ) );
				disp_commonObjCenter( this->objIdYukkuri, this->x, this->y, 0, 0, this->dispZ,
					0, FALSE, FALSE, RGB_WHITE, alpha_out
				);
			}
			break;
		case nn::yb::e::effect::muso_reimu::STATE_SETUP:
		case nn::yb::e::effect::muso_reimu::STATE_HANDS_UP:
		case nn::yb::e::effect::muso_reimu::STATE_HANDS_DOWN:
		case nn::yb::e::effect::muso_reimu::STATE_FIRE_STOP:
		case nn::yb::e::effect::muso_reimu::STATE_END:
			disp_commonObjCenter( obj_id_array[this->state], this->x, this->y + reimu_y_shift, 0, 0, this->dispZ,
				0, FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT
			);
			break;
		case nn::yb::e::effect::muso_reimu::STATE_FIRE:
			{
				int obj_id = obj_id_array[this->state] + (this->fireScene % 2);
				disp_commonObjCenter( obj_id, this->x, this->y + reimu_y_shift, 0, 0, this->dispZ,
					0, FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT
				);
			}
			break;
		case nn::yb::e::effect::muso_reimu::STATE_OUT:
			{
				int alpha_in = (int)( 31.0 * ( (double)this->frameCnt / (double)sMaxFrameArray[this->state] ) );
				disp_commonObjCenter( this->objIdYukkuri, this->x, this->y, 0, 0, this->dispZ,
					0, FALSE, FALSE, RGB_WHITE, alpha_in
				);

				int alpha_out = (int)( 31.0 * ( (double)(sMaxFrameArray[this->state] - this->frameCnt) / (double)sMaxFrameArray[this->state] ) );
				disp_commonObjCenter( obj_id_array[this->state], this->x, this->y + reimu_y_shift, 0, 0, this->dispZ,
					0, FALSE, FALSE, RGB_WHITE, alpha_out
				);
			}
			break;
	}

}
//---------------------------------------------------------------------------

