// edit_menu を置換
// EditMenu を置換
// yb を置換
// edit を置換

#include <sys/stat.h>
#include <time.h>

#include "nn_common.h"

#include "disp_common.h"
#include "font.h"
#include "key_common.h"
#include "image_ctrl.h"

#include "yb_save_manager.h"
#include "enum.h"
#include "edit.h"
#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "ctrl.h"
#include "save_data.h"
#include "menu.h"
#include "data_hole.h"
#include "edit_hole_obj.h"

#include "edit_menu.h"
//---------------------------------------------------------------------------
static int sWindowSize[][2] = {
	{ 1820, 1000},
	{ 1820, 850},
	{ 1580, 850},
	{ 1180, 824},
	{  800, 600},
	{  640, 480},
};

const char * nn::yb::edit::gHoleDataPath = "data\\hole\\DX";
//---------------------------------------------------------------------------
nn::yb::edit::CEditMenu::CEditMenu(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CEditMenu Create\n");
	this->state = nn::yb::e::edit_menu::STATE_NONE;
	this->oldState = nn::yb::e::edit_menu::STATE_NONE;
	this->waitCnt = 0;

	this->cursorX = 0;
	// saveDataに置き換え
	//this->cursorYEditData = 0;
	//this->cursorYWindowSize = 0;
	memset( this->editDataFlag, 0, sizeof( this->editDataFlag));

	this->ctrl = NULL;

	this->returnFlag = false;

	this->saveData = NULL;
	
	memset( this->editDataDate, 0, sizeof( this->editDataDate));

	memset( this->prevHoleSpell, 0, sizeof( this->prevHoleSpell));
	
	this->dataHole = NULL;

}
//---------------------------------------------------------------------------
nn::yb::edit::CEditMenu::~CEditMenu() {

	::nn_commonLog( 1, "CEditMenu Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditMenu::start() {

	this->state = nn::yb::e::edit_menu::STATE_WINDOW_SIZE_SELECT;

	char data_file_name[32];
	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( this->editDataFlag); i++) {

		sprintf( data_file_name, "%s\\hole%02d.bin", nn::yb::edit::gHoleDataPath, i);
		struct _stat buf;
		int ret = _stat( data_file_name, &buf);
		if(ret == 0) {
			strcpy( this->editDataDate[i], ctime( &buf.st_atime));
		}

		if(ret == 0 && this->saveManager->isLoad( data_file_name)) {
			this->editDataFlag[i] = true;
		}

	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditMenu::returnEdit() {

	this->returnFlag = true;
	this->edit();
	this->returnFlag = false;

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditMenu::edit() {

	this->ctrl->saveMain();

	this->ctrl->changeWindowSize( sWindowSize[this->saveData->editWindowSizeIndex][0],  sWindowSize[this->saveData->editWindowSizeIndex][1]);

	nn::yb::edit::CEdit * edit = new nn::yb::edit::CEdit( this);
	edit->master = this->master;
	edit->saveManager = this->saveManager;
	edit->ctrl = this->ctrl;
	edit->editDataNo = nn::yb::e::global::EDIT_DATA_DISP_DATA_PER_PAGE * this->saveData->editEditDataPage + this->saveData->editEditDataIndex;
	edit->wWindow = sWindowSize[this->saveData->editWindowSizeIndex][0];
	edit->hWindow = sWindowSize[this->saveData->editWindowSizeIndex][1];
	edit->spell = this->saveData->editSpell;
	edit->start();
	this->state = nn::yb::e::edit_menu::STATE_NONE;

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditMenu::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CEditMenu::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;

	switch(this->state) {
		case nn::yb::e::edit_menu::STATE_NONE:
			break;
		case nn::yb::e::edit_menu::STATE_WINDOW_SIZE_SELECT:
			if(::key_commonCursorMoveUpDown( key, NN_COMMON_GET_ARRAY_SIZE( sWindowSize) - 1, &this->saveData->editWindowSizeIndex)) {

			}
			else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_A) {
				this->state = nn::yb::e::edit_menu::STATE_EDIT_DATA_SELECT;
			}
			else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_B) {
				this->ctrl->menu->state = nn::yb::e::menu::STATE_MAIN;
				this->ctrl->state = nn::yb::e::ctrl::STATE_NONE;
				this->taskDelete();
			}
			break;
		case nn::yb::e::edit_menu::STATE_EDIT_DATA_SELECT:
			if(::key_commonCursorMove( key, nn::yb::e::global::EDIT_DATA_DISP_PAGE - 1, nn::yb::e::global::EDIT_DATA_DISP_DATA_PER_PAGE - 1, &this->saveData->editEditDataPage, &this->saveData->editEditDataIndex)) {

			}
			else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_A) {
				this->selectData();
			}
			else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_B) {
				this->state = nn::yb::e::edit_menu::STATE_WINDOW_SIZE_SELECT;
			}
			break;
		case nn::yb::e::edit_menu::STATE_EDIT_SPELL_SELECT:
			if(::key_commonCursorMoveUpDown( key,
				nn::yb::e::global::SPELL_CARD_MAX - 1,
				&this->saveData->editSpell)
			) {

			}
			else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_A) {
				this->edit();
			}
			else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_B) {
				this->state = nn::yb::e::edit_menu::STATE_EDIT_DATA_SELECT;
			}
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditMenu::disp() {
	
	int x = 20;
	int y = 20;
	switch(this->state) {
		case nn::yb::e::edit_menu::STATE_NONE:
			break;
		case nn::yb::e::edit_menu::STATE_WINDOW_SIZE_SELECT:
			for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( sWindowSize); i++) {

				int color = RGB_WHITE;
				if(i == this->saveData->editWindowSizeIndex) {
					color = RGB_RED;
				}
				fontPrintfType( x, y + i * 20, nn::yb::e::global::FONT_TYPE_SIZE_18,
					color, "%4dx%4d", sWindowSize[i][0], sWindowSize[i][1]);

			}
			break;
		case nn::yb::e::edit_menu::STATE_EDIT_DATA_SELECT:
			for( int i = 0; i < nn::yb::e::global::EDIT_DATA_DISP_DATA_PER_PAGE; i++) {

				int color = RGB_WHITE;
				if(i == this->saveData->editEditDataIndex) {
					color = RGB_RED;
				}
				int index = nn::yb::e::global::EDIT_DATA_DISP_DATA_PER_PAGE * this->saveData->editEditDataPage + i;
				int mode = index / 48 + 1;
				int stage = index % 48 / 8 + 1;
				int hole =  index % 8 + 1;
				if(this->editDataFlag[index]) {
					fontPrintfType( x, y + i * 20, nn::yb::e::global::FONT_TYPE_SIZE_18,
						color, "%02d:%02d-%02d-%02d:%s", index, mode, stage, hole, this->editDataDate[index]);
				}
				else {
					fontPrintfType( x, y + i * 20, nn::yb::e::global::FONT_TYPE_SIZE_18,
						color, "%02d:%02d-%02d-%02d:NO DATA", index, mode, stage, hole);
				}

			}
			break;
		case nn::yb::e::edit_menu::STATE_EDIT_SPELL_SELECT:
			{
				char * spell_name[] = {
					"なし",
					"霊夢",
					"レミリア",
					"魔理沙",
					"文",
					"天子",
					"にとり",
					"フラン",
					"お燐",
				};
				for( int i = 0; i < nn::yb::e::global::SPELL_CARD_MAX; i++) {

					int color RGB_WHITE;
					if(i == this->saveData->editSpell) {
						color = RGB_RED;
					}

					fontPutsType( x, y + i * 20, nn::yb::e::global::FONT_TYPE_SIZE_18, color, spell_name[i]);

				}

				fontPutsType( x + 150, y, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_WHITE,
					"前ホール出現スペカ");
				for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( this->prevHoleSpell); i++) {

					if(this->prevHoleSpell[i] != 0) {
						fontPutsType( x + 150, y + 20 + i * 20, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_WHITE, spell_name[this->prevHoleSpell[i]]);
					}

				}
			}
			break;
	}
	

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEditMenu::selectData() {

	memset( this->prevHoleSpell, 0, sizeof( this->prevHoleSpell));

	int edit_data_no = nn::yb::e::global::EDIT_DATA_DISP_DATA_PER_PAGE * this->saveData->editEditDataPage + this->saveData->editEditDataIndex;

	if(edit_data_no % 8 >= 1) {
		char data_file_name[32];
		nn::yb::edit::CEdit::getHoleDataFileName( data_file_name, edit_data_no - 1);
		if(this->saveManager->isLoad( data_file_name)) {
			nn::yb::edit::CDataHole * load_hole =
				(nn::yb::edit::CDataHole *)this->saveManager->load( data_file_name, false);
			::TASK_SAVE::addTask( this, load_hole);

			int spellCnt = 0;
			for( int i = 0; i < load_hole->objCnt; i++) {

				int spell = nn::yb::edit::CEditHoleObj::getObjCategory2Spell( load_hole->obj[i]->category);
				if(spell != 0) {
					this->prevHoleSpell[spellCnt] = spell;
					spellCnt++;
				}

			}

			load_hole->taskDelete();
		}
	}

	this->state = nn::yb::e::edit_menu::STATE_EDIT_SPELL_SELECT;

}
//---------------------------------------------------------------------------

