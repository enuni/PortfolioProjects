// title を置換
// Title を置換
// ts を置換
// title を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "font.h"
#include "disp.h"
#include "direct.h"
#include "anim.h"
#include "script.h"
#include "font2.h"

#include "ts_common.h"
#include "control.h"

#include "title.h"

extern nn::dxlib::CFont * font;
extern MASTER * master;
//---------------------------------------------------------------------------
char * gTextWarning[] = {
	"本ソフトは東方Projectの二次創作です。",
	"",
	"バグ修正・追加要素のアップデートパッチは、",
	"agrippaのサイトで公開いたします。",
	"",
	"その他問い合わせは、agrippaのサイトか",
	"n2n.lj.infoseek@gmail.com",
	"えぬに までお願いします。",
};

char * gTextStart = "Click or Touch !";

nn::ts::title::TITLE_DISP_MEMBER nn::ts::title::CTitle::dm;
//---------------------------------------------------------------------------
nn::ts::title::CTitle::CTitle(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CTitle Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructorCommon();

}
//---------------------------------------------------------------------------
nn::ts::title::CTitle::CTitle(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CTitle Create(TASK_SAVE * oya)\n");
	
	this->constructorCommon();

}
//---------------------------------------------------------------------------
void nn::ts::title::CTitle::constructorCommon() {
	
	this->state = nn::ts::e::title::STATE_NONE;
	this->oldState = nn::ts::e::title::STATE_NONE;
	this->waitCnt = 0;

	this->stateFrameCnt = 0;
	
	strcpy_s( this->name, sizeof( this->name), "CTitle");
	
}
//---------------------------------------------------------------------------
nn::ts::title::CTitle::~CTitle() {

	::nn_commonLog( 1, "CTitle Delete\n");
	
	this->lm.ctrl->startQuest();

}
//---------------------------------------------------------------------------
void nn::ts::title::CTitle::start() {


#ifdef _DEBUG
	this->state = nn::ts::e::title::STATE_NEXT;
	this->next();
#else //_DEBUG
	this->state = nn::ts::e::title::STATE_WARNING;
	nn::dxlib::disp::setBrightness( -255);
	nn::dxlib::direct::fade( this,
		nn::dxlib::e::direct::FADE_MODE_BLACK_FADE_IN,
		nn::ts::title::CTitle::decWaitCntCb, this
	);

	this->waitCnt++;
#endif //_DEBUG

}
//---------------------------------------------------------------------------
void nn::ts::title::CTitle::taskWarningLogo() {

	::KEY_WORK * key = &master->keyWork;
	::TOUCH_WORK * touch = &master->touchWork;
	
	unsigned int next_frame_cnt = 90;
	switch(this->state ) {
		case nn::ts::e::title::STATE_WARNING:
			next_frame_cnt = 180;
			break;
		case nn::ts::e::title::STATE_LOGO:
			next_frame_cnt = 90;
			break;
	}


	if(key->trigger || touch->touchPoint) {
		this->waitCnt++;
		this->next();
	}
	else if(this->stateFrameCnt >= next_frame_cnt) {
		nn::dxlib::direct::fade( this,
			nn::dxlib::e::direct::FADE_MODE_BLACK_FADE_OUT,
			nn::ts::title::CTitle::nextCb, this
		);
		this->waitCnt++;
	}
	else {
		this->stateFrameCnt++;
	}
}
//---------------------------------------------------------------------------
void nn::ts::title::CTitle::taskTitle() {

	::KEY_WORK * key = &master->keyWork;
	::TOUCH_WORK * touch = &master->touchWork;

	this->stateFrameCnt++;
	
	if(key->trigger || touch->touchPoint) {
		this->next();
	}

}
//---------------------------------------------------------------------------
void nn::ts::title::CTitle::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CTitle::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	if(this->waitCnt <= 0) {
		switch(this->state) {
			case nn::ts::e::title::STATE_NONE:
				break;
			case nn::ts::e::title::STATE_WARNING:
			case nn::ts::e::title::STATE_LOGO:
				this->taskWarningLogo();
				break;
			case nn::ts::e::title::STATE_TITLE_WAIT:
				this->taskTitle();
				break;
		}
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::ts::title::CTitle::disp() {

	switch(this->state) {
		case nn::ts::e::title::STATE_NONE:
			break;
		case nn::ts::e::title::STATE_WARNING:
			{
				for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( gTextWarning); i++) {

					font->putsColor( gTextWarning[i], 160, 160 + 22 * i, nn::ts::e::common::Z_TITLE_TEXT, DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_NONE, false);

				}
			}
			break;
		case nn::ts::e::title::STATE_LOGO:
			{
				nn::dxlib::disp::objSimple( this->dm.layoutIdAgrippa, false);
				nn::dxlib::disp::objSimple( this->dm.layoutIdN2nsoft, false);
			}
			break;
		case nn::ts::e::title::STATE_TITLE_IN:
		case nn::ts::e::title::STATE_TITLE_DIRECT:
			{
				nn::dxlib::disp::boardFull( 0, 0, nn::ts::e::common::Z_TITLE_BG,
					854, 480, 0, DISP_WHITE, DISP_ALPHA_DEFAULT,
					DISP_BLEND_MODE_NONE, false);
			}
			break;
		case nn::ts::e::title::STATE_TITLE_WAIT:
			{
				if(this->stateFrameCnt % 60 < 30) {
					int w = font->getStringDispW( gTextStart);
					font->putsColor( "Click or Touch !",
						427 - w / 2, 360, nn::ts::e::common::Z_TITLE_TEXT,
						DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_NONE, false);
				}
				nn::dxlib::disp::boardFull( 0, 0, nn::ts::e::common::Z_TITLE_BG,
					854, 480, 0, DISP_GET_COLOR( 255, 250, 250), DISP_ALPHA_DEFAULT,
					DISP_BLEND_MODE_NONE, false);
			}
			break;
		case nn::ts::e::title::STATE_NEXT:
			{
					nn::dxlib::disp::boardFull( 0, 0, nn::ts::e::common::Z_TITLE_BG,
						854, 480, 0, DISP_GET_COLOR( 255, 250, 250), DISP_ALPHA_DEFAULT,
						DISP_BLEND_MODE_NONE, false);
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::title::CTitle::decWaitCntCb(void * titleArg) {

	nn::ts::title::CTitle * title = (nn::ts::title::CTitle *)titleArg;

	title->decWaitCnt();

}
//---------------------------------------------------------------------------
void nn::ts::title::CTitle::decWaitCnt() {

	this->waitCnt--;

}
//---------------------------------------------------------------------------
void nn::ts::title::CTitle::nextCb( void * titleArg) {

	nn::ts::title::CTitle * title = (nn::ts::title::CTitle *)titleArg;

	title->next();

}
//---------------------------------------------------------------------------
void nn::ts::title::CTitle::next() {

	if(this->state == nn::ts::e::title::STATE_WARNING) {
		this->waitCnt--;
		this->stateFrameCnt = 0;

		nn::dxlib::disp::setBrightness( -255);
		nn::dxlib::direct::fade( this,
			nn::dxlib::e::direct::FADE_MODE_BLACK_FADE_IN,
			nn::ts::title::CTitle::decWaitCntCb, this
		);

		this->waitCnt++;
		this->state = nn::ts::e::title::STATE_LOGO;
	}
	else if(this->state == nn::ts::e::title::STATE_LOGO) {
		this->waitCnt--;
		this->stateFrameCnt = 0;

		nn::dxlib::disp::setBrightness( -255);
		nn::dxlib::direct::fade( this,
			nn::dxlib::e::direct::FADE_MODE_BLACK_FADE_IN,
			nn::ts::title::CTitle::nextCb, this
		);

		this->state = nn::ts::e::title::STATE_TITLE_IN;
	}
	else if(this->state == nn::ts::e::title::STATE_TITLE_IN) {
		nn::dxlib::CAnim::exec( this, this->dm.animeIdTitleTextIn,
			427, 240, 0, ANIM_FLAGS_LAST_WAIT,
			nn::ts::title::CTitle::nextCb, this
		);

		this->state = nn::ts::e::title::STATE_TITLE_DIRECT;
	}
	else if(this->state == nn::ts::e::title::STATE_TITLE_DIRECT) {
		this->state = nn::ts::e::title::STATE_TITLE_WAIT;
	}
	else if(this->state == nn::ts::e::title::STATE_TITLE_WAIT) {
		this->stateFrameCnt = 0;

		nn::dxlib::direct::fade( this,
			nn::dxlib::e::direct::FADE_MODE_BLACK_FADE_OUT,
			nn::ts::title::CTitle::nextCb, this
		);

		this->state = nn::ts::e::title::STATE_NEXT;
	}
	else if(this->state == nn::ts::e::title::STATE_NEXT) {
		nn::dxlib::disp::setBrightness( 0);		// ToDo 次でフェードアウト出来るようになったら削除

		this->taskDelete();
	}

}
//---------------------------------------------------------------------------

