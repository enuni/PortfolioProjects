// debug_menu を置換
// DebugMenu を置換
// ts を置換
// debug を置換

#include "nn_common.h"
#include "nn_common2.h"
#include "key_common.h"
#include "disp.h"

#include "ts_common.h"


#include "debug_menu.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::debug::CDebugMenu::dataRecode[] = {
	DATA_LOCAL( nn::ts::debug::CDebugMenu, state),
	DATA_LOCAL( nn::ts::debug::CDebugMenu, oldState),
	DATA_LOCAL( nn::ts::debug::CDebugMenu, waitCnt),
	DATA_LOCAL( nn::ts::debug::CDebugMenu, frameCnt),
	DATA_END,
};
//---------------------------------------------------------------------------
nn::ts::debug::CDebugMenu::CDebugMenu(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CDebugMenu Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::debug::CDebugMenu::CDebugMenu(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CDebugMenu Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::debug::CDebugMenu::CDebugMenu() : TASK_SAVE() {

	::nn_commonLog( 1, "CDebugMenu Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::debug::CDebugMenu::constructCommon() {
	
	this->state = nn::ts::e::debug_menu::STATE_NONE;
	this->oldState = nn::ts::e::debug_menu::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->m, 0, sizeof( this->m));
	
}
//---------------------------------------------------------------------------
nn::ts::debug::CDebugMenu::~CDebugMenu() {

	::nn_commonLog( 1, "CDebugMenu Delete\n");

}
//---------------------------------------------------------------------------
void nn::ts::debug::CDebugMenu::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::debug::CDebugMenu::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::debug::CDebugMenu::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::debug::CDebugMenu::start() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::debug::CDebugMenu::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::debug::CDebugMenu::startCommon() {

	this->state = nn::ts::e::debug_menu::STATE_MAIN;

}
//---------------------------------------------------------------------------
void nn::ts::debug::CDebugMenu::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CDebugMenu::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::debug_menu::STATE_NONE:
			break;
		case nn::ts::e::debug_menu::STATE_MAIN:
			if(key_commonCursorMoveUpDown( key, nn::ts::e::common::DEBUG_COMMAND_MAX - 1, &this->m.game.cursorY)) {

			}
			else if(key->trigger & nn::common::e::PAD_LEFT) {
				this->m.link.commandCBFunc( this->m.link.commnadCBFuncArg, nn::ts::e::common::DEBUG_COMMAND_UNDO);
			}
			else if(key->trigger & nn::common::e::PAD_RIGHT) {
				this->m.link.commandCBFunc( this->m.link.commnadCBFuncArg, nn::ts::e::common::DEBUG_COMMAND_REDO);
			}
			else if(key->trigger & nn::common::e::PAD_A) {
				this->m.link.commandCBFunc( this->m.link.commnadCBFuncArg, this->m.game.cursorY);
			}
			else if(key->trigger & nn::common::e::PAD_B) {
				this->m.link.commandCBFunc( this->m.link.commnadCBFuncArg, nn::ts::e::common::DEBUG_COMMAND_END);
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::debug::CDebugMenu::disp() {

	static char * sItemMainTexts[nn::ts::e::common::DEBUG_COMMAND_MAX] = {
		"セーブ",
		"ロード",
		"一手戻る",
		"一手進む",
		"バトル突破",
		"クエストリスタート",
		"戻る",
	};

	switch(this->state) {
		case nn::ts::e::debug_menu::STATE_NONE:
			break;
		case nn::ts::e::debug_menu::STATE_MAIN:
			int x = nn::ts::e::debug_menu::DEBUG_MENU_X;
			int y = nn::ts::e::debug_menu::DEBUG_MENU_Y;
			int w = nn::ts::e::debug_menu::DEBUG_MENU_TEXT_WIDTH;
			int h = nn::ts::e::debug_menu::DEBUG_MENU_TEXT_HEIGHT;
			int mergin = nn::ts::e::debug_menu::DEBUG_MENU_BG_MERGIN;
			int item_n = nn::ts::e::common::DEBUG_COMMAND_MAX;

			for( int i = 0; i < item_n; i++) {

				int color = DISP_WHITE;
				if(i == this->m.game.cursorY) {
					color = DISP_RED;
				}

				this->m.link.font->putsColor( sItemMainTexts[i],
					x, y + i * h,
					nn::ts::e::common::Z_COMMON_DEBUG_MENU_TEXT,
					color, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_NONE, false);

			}
			nn::dxlib::disp::boardFull(
				x - mergin, y,
				nn::ts::e::common::Z_COMMON_DEBUG_MENU_BG,
				w + mergin * 2, h * item_n + mergin * 2, 0,
				DISP_BLACK, 192, DISP_BLEND_MODE_ALPHA, false);
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::debug::CDebugMenu::getClassId() {
	return -1;		// ToDo
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::debug::CDebugMenu::getDataRecode() {
	return nn::ts::debug::CDebugMenu::dataRecode;
}
//---------------------------------------------------------------------------
void nn::ts::debug::CDebugMenu::dump() {
	LOG_CLASS_DUMP( "---------- nn::ts::debug::CDebugMenu::dump() start ----------\n");
	LOG_CLASS_DUMP( "---------- nn::ts::debug::CDebugMenu::dump() end   ----------\n");
}
//---------------------------------------------------------------------------

