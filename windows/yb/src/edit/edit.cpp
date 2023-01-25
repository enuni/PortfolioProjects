// edit を置換
// Edit を置換
// yb を置換
// edit を置換

#include "nn_common.h"
#include "touch.h"
#include "font.h"
#include "disp_common.h"
#include "image_ctrl.h"
#include "key_common.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"

#include "enum.h"
#include "yb_common.h"
#include "yb_save_manager.h"

#include "edit_hole_obj.h"
#include "data_hole.h"
#include "edit_menu.h"
#include "ctrl.h"

#include "hole_ctrl.h"
#include "hole.h"
#include "stage.h"

#include "edit.h"
//---------------------------------------------------------------------------
nn::yb::edit::CEdit::CEdit(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CEdit Create\n");
	this->state = nn::yb::e::edit::STATE_NONE;
	this->oldState = nn::yb::e::edit::STATE_NONE;
	this->waitCnt = 0;

	this->master = NULL;
	
	this->hole = NULL;

	//this->w;
	//this->h;
	//memset( this->obj, 0, sizeof( this->obj));
	//this->objCnt = 0;
	this->createMode = nn::yb::e::edit_hole_obj::CATEGORY_BUMPER;
	this->mode = nn::yb::e::edit::MODE_CREATE;

	memset( this->modeButton, 0, sizeof( this->modeButton));
	memset( this->createModeButton, 0, sizeof( this->createModeButton));
	memset( this->editModeButton, 0, sizeof( this->editModeButton));
	memset( this->commonButton, 0, sizeof( this->commonButton));

	this->activeObjIndex = 0;

	this->createState = nn::yb::e::edit::CREATE_STATE_NONE;

	this->objDataDispX = 0;
	
	this->windowScrollFlag = false;
	this->xSlideOld = 0;
	this->ySlideOld = 0;
	this->xWindow = 2048;
	this->yWindow = 2048;

	this->scale = 1.0;

	this->spell = 0;

	this->createButtonN = 0;

}
//---------------------------------------------------------------------------
nn::yb::edit::CEdit::~CEdit() {

	::nn_commonLog( 1, "CEdit Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::start() {

	// nn::yb::main::CCtrl::changeWindowSize()でロードする
	//this->ctrl->loadImage( 0);
	
	this->createMode = nn::yb::e::edit_hole_obj::CATEGORY_GROUND;
	this->mode = nn::yb::e::edit::MODE_CREATE;

	char data_file_name[32];
	if(this->ctrl->editMenu->returnFlag) {
		this->hole = this->ctrl->editMenu->dataHole;
		this->hole->edit = this;
	}
	else {
		nn::yb::edit::CEdit::getHoleDataFileName( data_file_name, this->editDataNo);
#if 0
		// 一括update
		// int n = 48;	// 0
		int n = 96;	// DX
		for( int i = 0; i < n; i++) {

			nn::yb::edit::CEdit::getHoleDataFileName( data_file_name, i);
			
			if(this->saveManager->isLoad( data_file_name)) {
				this->load( data_file_name);
			}

			this->saveManager->save( data_file_name, false, 0);

			this->hole->taskDeleteInstant();

		}
		return;
#endif

		if(this->saveManager->isLoad( data_file_name)) {
			this->load( data_file_name);
		}
		else {
			this->hole = new nn::yb::edit::CDataHole( NULL, this->saveManager);
			this->hole->edit = this;
#if 1
			// クラスセーブは、タスク未対応
			nn::yb::edit::CEditHoleObj * obj = new nn::yb::edit::CEditHoleObj( this->hole, this->saveManager);
#else
			nn::yb::edit::CEditHoleObj * obj = new nn::yb::edit::CEditHoleObj( this);
#endif
			obj->x1 = 2048 + 320;
			obj->y1 = 2048 + 240;
			obj->hole = this->hole;
			obj->start();
			obj->activeFlag = true;
			this->hole->obj[this->hole->objCnt] = obj;
			this->hole->objCnt++;
#if 0
			obj->x1 = 320;
			obj->y1 = 240;
			this->loadFromImage( "flan2.png", 0x000000FF, nn::yb::e::edit_hole_obj::CATEGORY_GROUND);
			this->loadFromImage( "flan1.png", 0xFFFF00FF, nn::yb::e::edit_hole_obj::CATEGORY_BUNKER);
			this->loadFromImage( "flan1.png", 0x800000FF, nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN);
			this->loadFromImage( "flan1.png", 0xFFFFFFFF, nn::yb::e::edit_hole_obj::CATEGORY_DAMAGE);
			this->loadFromImage( "flan1.png", 0xFF00FFFF, nn::yb::e::edit_hole_obj::CATEGORY_GRAVITY);
			this->loadFromImage( "flan1.png", 0x00FFFFFF, nn::yb::e::edit_hole_obj::CATEGORY_FUKIAGE);
			this->loadFromImage( "flan1.png", 0xFF0000FF, nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP);
#endif
		}
	}

	this->createButtonAll();

	disp_commonSetCameraFlag( true);
	disp_commonSetCameraPos( this->xWindow + this->wWindow / 2, this->yWindow + this->hWindow / 2);

}
typedef struct _area_data {
	int area;
	int pos;
	int w;
	int h;
	unsigned int color;
} AREA_DATA;
//---------------------------------------------------------------------------
static unsigned int get_color( int image, int x, int y) {

	int r, g, b, a;
	GetPixelSoftImage( image, x, y, &r, &g, &b, &a);
	return (r << 24) + (g << 16) + (b << 8) + a;

}
//---------------------------------------------------------------------------
static void set_area( AREA_DATA * maxArea, int image, int pos, int * map) {

	int w;
	int h;
	::GetSoftImageSize( image,&w, &h);
	
	int x = pos % w;
	int y = pos / w;

	unsigned int color_org, color;

	AREA_DATA area_data;
	AREA_DATA * area = &area_data;


	color = get_color( image, x, y);
	color_org = color;
	if(map[pos]) {
		memset( maxArea, 0, sizeof( maxArea));
		return;
	}
	
	int i, j, k;
	for( i = x; i < w; i++) {

		color = get_color( image, i, y);
		if(color != color_org || map[y * w + i]) {
			break;
		}

		for( j = y + 1; j < h; j++) {

			bool flag = false;
			for( k = x; k <= i; k++) {

				color = get_color( image, k, j);
				if(color != color_org || map[j * w + k]) {
					flag = true;
				}

			}
			if(flag) {
				break;
			}

		}
		area->w = (i + 1) - x;
		area->h = j - y;
		area->area = area->w * area->h;
		area->pos = pos;
		area->color = color_org;
		if(maxArea->area < area->area) {
			*maxArea = *area;
		}

	}
	printf( "max = %d\n", maxArea->area);

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::loadFromImage( char * imageFileName, unsigned int checkColor, int setCategory) {

	int image = ::LoadSoftImage( imageFileName);
	int w;
	int h;
	::GetSoftImageSize( image,&w, &h);
	int dot_size = 28;

	printf( "w = %d h = %d", w, h);

	int * map;
	map = (int *)malloc(sizeof(int) * w * h);
	memset( map, 0, sizeof(int) * w * h);

	while(1) {

		AREA_DATA max_area;
		AREA_DATA * max = &max_area;
		memset( max, 0, sizeof(max));

		for( int x = 0; x < w; x++) {

			for( int y = 0; y < h; y++) {

				int pos = y * w + x;
				AREA_DATA area;
				AREA_DATA * a = &area;
				memset( a, 0, sizeof( a));
				unsigned int color = get_color( image, x, y);
				if(color == checkColor) {
					set_area( a, image, pos, map);
					if(a->area > max->area) {
						*max = *a;
					}
				}

			}

		}

		if(max->area <= 1) {
			break;
		}
		
		int max_area_x = (max->pos % w);
		int max_area_y = (max->pos / w);
		nn::yb::edit::CEditHoleObj * obj = new nn::yb::edit::CEditHoleObj( this->hole, this->saveManager);
		obj->x1 = max_area_x * dot_size;
		obj->y1 = max_area_y * dot_size;
		obj->x2 = obj->x1 + dot_size * max->w;
		obj->y2 = obj->y1 + dot_size * max->h;
		obj->category = setCategory;
		obj->hole = this->hole;
		obj->start();
		obj->activeFlag = true;
		this->hole->obj[this->hole->objCnt] = obj;
		this->hole->objCnt++;
		printf( "---create objCnt = %d\n", this->hole->objCnt);

		for( int i = max_area_y; i < max_area_y + max->h; i++) {
			
			for( int j = max_area_x; j < max_area_x + max->w; j++) {

				map[i * w + j] = 1;
	
			}

		}

	}

	for( int i = 0; i < h; i++) {

		for( int j = 0; j < w; j++) {

			int pos = i * w + j;
			unsigned int color = get_color( image, j, i);
			if(map[pos] == 0 && color == checkColor) {
				nn::yb::edit::CEditHoleObj * obj = new nn::yb::edit::CEditHoleObj( this->hole, this->saveManager);
				obj->x1 = j * dot_size;
				obj->y1 = i * dot_size;
				obj->x2 = obj->x1 + dot_size;
				obj->y2 = obj->y1 + dot_size;
				obj->category = setCategory;
				obj->hole = this->hole;
				obj->start();
				obj->activeFlag = true;
				this->hole->obj[this->hole->objCnt] = obj;
				this->hole->objCnt++;
				printf( "---create objCnt = %d\n", this->hole->objCnt);
			}

		}

	}

	free( map);

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::getHoleDataFileName( char * dataFileName, int dataNo) {

	sprintf( dataFileName, "%s\\hole%02d.bin", nn::yb::edit::gHoleDataPath, dataNo);

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::createButtonAll() {

	int xStart = 60;
	int yStart = 0;
	int w = 60 + 1;
	int h = 14 + 1;
	int x;
	int y;

	char * modeCaptionTbl[] = {
		"生成",
		"編集",
	};

	int modeIdTbl[] = {
		nn::yb::e::edit::BUTTON_ID_CREATE,
		nn::yb::e::edit::BUTTON_ID_EDIT,
	};

	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( modeCaptionTbl); i++) {

		x = xStart + (i / 2) * w;
		y = yStart + (i % 2) * h;
		this->createButton( modeIdTbl[i], nn::yb::edit::CEdit::onButtonModeCb, modeCaptionTbl[i],
			x, y, GX_RGB( 0, 31, 0),  GX_RGB( 0, 16, 0), &this->modeButton[i]
		);

	}

	xStart = x + w;

	char * createModeCaptionTbl[] = {
		"地面",
		"敵",
		"バンパー",
		"坂",
		"西行妖",
		"キック",
		"敵2",
		"ダッシュ",
		"ジャンプ",
		"カメラ",
		"敵3",
		"ダメージ",
		"バンカー",
		"重力",
		"敵4",
		"雲山",
		"お空",
		"吹き上げ",
		"コンベヤ",
		"大ジャンプ",
		"SE床",
		"ワープ",
		"敵5",
		"雲山2",
	};

	int createModeIdTbl[] = {
		nn::yb::e::edit::BUTTON_ID_CREATE_GROUND,
		nn::yb::e::edit::BUTTON_ID_CREATE_ENEMY,
		nn::yb::e::edit::BUTTON_ID_CREATE_BUMPER,
		nn::yb::e::edit::BUTTON_ID_CREATE_SLOPE_UP,
		nn::yb::e::edit::BUTTON_ID_CREATE_PILLAR,
		nn::yb::e::edit::BUTTON_ID_CREATE_KICK_UP,
		nn::yb::e::edit::BUTTON_ID_CREATE_ENEMY_KEDAMA,
		nn::yb::e::edit::BUTTON_ID_CREATE_DASH,
		nn::yb::e::edit::BUTTON_ID_CREATE_JUMP,
		nn::yb::e::edit::BUTTON_ID_CREATE_CAMERA,
		nn::yb::e::edit::BUTTON_ID_CREATE_ENEMY_MSPA,
		nn::yb::e::edit::BUTTON_ID_CREATE_DAMAGE,
		nn::yb::e::edit::BUTTON_ID_CREATE_BUNKER,
		nn::yb::e::edit::BUTTON_ID_CREATE_GRAVITY,
		nn::yb::e::edit::BUTTON_ID_CREATE_ENEMY_MOMIJI,
		nn::yb::e::edit::BUTTON_ID_CREATE_UNZAN,
		nn::yb::e::edit::BUTTON_ID_CREATE_OKU,
		nn::yb::e::edit::BUTTON_ID_CREATE_FUKIAGE, //吹き上げエリア
		nn::yb::e::edit::BUTTON_ID_CREATE_CONVEYOR_LEFT_UP,//コンベアー
		nn::yb::e::edit::BUTTON_ID_CREATE_JUMP_STRONG, //小ジャンプ
		nn::yb::e::edit::BUTTON_ID_CREATE_SE_FLOOR, //SE床
		nn::yb::e::edit::BUTTON_ID_CREATE_WARP2_NONE,
		nn::yb::e::edit::BUTTON_ID_CREATE_ENEMY_NITORI,
		nn::yb::e::edit::BUTTON_ID_CREATE_UNZAN_MOVE,
	};

	this->createButtonN = NN_COMMON_GET_ARRAY_SIZE( createModeCaptionTbl);
	for( int i = 0; i < this->createButtonN; i++) {

		x = xStart + (i % 4) * w;
		y = yStart + (i / 4) * h;
		this->createButton( createModeIdTbl[i], nn::yb::edit::CEdit::onButtonCreateModeCb, createModeCaptionTbl[i],
			x, y, GX_RGB( 0, 0, 31),  GX_RGB( 0, 0, 16), &this->createModeButton[i]
		);

	}

	this->objDataDispX = x + w + 10;

	char * editModeCaptionTbl[] = {
		"削除",
	};
	
	int editModeIdTbl[] = {
		nn::yb::e::edit::BUTTON_ID_EDIT_DELETE,
	};

	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( editModeCaptionTbl); i++) {

		x = xStart + (i / 2) * w;
		y = yStart + (i % 2) * h;
		this->createButton( editModeIdTbl[i], nn::yb::edit::CEdit::onButtonEditModeCb, editModeCaptionTbl[i],
			x, y, GX_RGB( 0, 0, 31),  GX_RGB( 0, 0, 16), &this->editModeButton[i]
		);

	}

	if(x + w > this->objDataDispX) {
		this->objDataDispX = x + w + 10;
	}
	
	xStart = this->objDataDispX + 130;
	
	char * commonCaptionTbl[] = {
		"プレイ",
		"セーブ",
		"終了",
	};
	
	int commonIdTbl[] = {
		nn::yb::e::edit::BUTTON_ID_COMMON_PLAY,
		nn::yb::e::edit::BUTTON_ID_COMMON_SAVE,
		nn::yb::e::edit::BUTTON_ID_COMMON_END,
	};
	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( commonCaptionTbl); i++) {

		x = xStart + (i / 2) * w;
		y = yStart + (i % 2) * h;
		this->createButton( commonIdTbl[i], nn::yb::edit::CEdit::onButtonCommonCb, commonCaptionTbl[i],
			x, y, GX_RGB( 0, 24, 31),  GX_RGB( 0, 12, 16), &this->commonButton[i]
		);

	}

	this->setButtonStateStart();
	this->setButtonStateChangeMode();

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::setButtonStateStart() {

	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( this->commonButton); i++) {

		nn::yb::common::CButton * button = this->commonButton[i];
		if(button != NULL) {
			button->state = nn::yb::e::button::STATE_ON;
		}

	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::load( char * dataFileName) {
	
	nn::yb::edit::CDataHole * load_hole =
		(nn::yb::edit::CDataHole *)this->saveManager->load( dataFileName, false);

	::TASK_SAVE::addTask( TASK_SAVE::sRoot, load_hole);
	load_hole->edit = this;
	this->hole = load_hole;
	this->hole->obj[0]->activeFlag = true;

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::play() {
	
	char data_file_name[32];
	// edit復帰する用に用意、121015～不要になったが、バックアップとして残す
	strcpy( data_file_name, "hole_temp.bin");

	this->saveManager->save( data_file_name, false, 0);

	this->ctrl->changeWindowSize( nn::yb::e::global::WINDOW_W, nn::yb::e::global::WINDOW_H);

	nn::yb::main::CStage * stage = new nn::yb::main::CStage( this->ctrl);
	stage->master = this->master;
	stage->ctrl = this->ctrl;
	stage->saveData = this->ctrl->saveData;
	stage->stageNo = 1;
	stage->editFlag = true;
	stage->dataHole = this->hole;
	stage->lp = 99;
	stage->setHp( 4);
	stage->spell = this->spell;
	stage->start();

	this->taskDelete();

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::save() {

	char data_file_name[32];
	sprintf( data_file_name, "%s\\hole%02d.bin", nn::yb::edit::gHoleDataPath, this->editDataNo);

	this->saveManager->save( data_file_name, false, 0);

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::createButton(int id, void (*onCb)(void *, void *), char *caption,
	int x, int y, GXRgb color, GXRgb darkColor, nn::yb::common::CButton **buttonContainer
) {
	nn::yb::common::CButton * button;
	button = new nn::yb::common::CButton( this);
	button->master = this->master;
	button->onCb = onCb;
	button->onCbArg = this;
	button->id = id;
	strcpy( button->caption, caption);
	button->x = x;
	button->y = y;
	button->w = 60;
	button->h = 14;
	button->color = color;
	button->darkColor = darkColor;
	button->state = nn::yb::e::button::STATE_OFF;
	button->start();
	*buttonContainer = button;
}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onButtonCommonCb(void *argEdit, void *argButton) {

	nn::yb::edit::CEdit * edit = (nn::yb::edit::CEdit *)argEdit;

	edit->onButtonCommon( argButton);

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onSave() {

	this->save();
	nn::yb::common::CDialog * dialog = new nn::yb::common::CDialog( NULL);
	dialog->master = this->master;
	dialog->id = nn::yb::e::edit::DIALOG_ID_SAVE;
	strcpy( dialog->msg, "セーブ完了");
	dialog->buttonN = 1;
	strcpy( dialog->buttonCaption[nn::yb::e::edit::DIALOG_BUTTON_NO_OK], "OK");
	dialog->onCb = nn::yb::edit::CEdit::onDialogCb;
	dialog->onArg = this;
	dialog->start();

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onEnd() {
	
	nn::yb::common::CDialog * dialog = new nn::yb::common::CDialog( NULL);
	dialog->master = this->master;
	dialog->id = nn::yb::e::edit::DIALOG_ID_END;
	strcpy( dialog->msg, "本当に終了しますか？");
	dialog->buttonN = 2;
	strcpy( dialog->buttonCaption[nn::yb::e::edit::DIALOG_BUTTON_NO_OK], "OK");
	strcpy( dialog->buttonCaption[nn::yb::e::edit::DIALOG_BUTTON_NO_CANCEL], "キャンセル");
	dialog->onCb = nn::yb::edit::CEdit::onDialogCb;
	dialog->onArg = this;
	dialog->start();

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onButtonCommon(void *argButton) {

	nn::yb::common::CButton * button = (nn::yb::common::CButton *)argButton;

	switch(button->id) {
		case nn::yb::e::edit::BUTTON_ID_COMMON_PLAY:
			this->play();
			break;
		case nn::yb::e::edit::BUTTON_ID_COMMON_SAVE:
			this->onSave();
			break;
		case nn::yb::e::edit::BUTTON_ID_COMMON_END:
			this->onEnd();
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onDialogCb( void *argEdit, void *argDialog) {

	nn::yb::edit::CEdit * edit = (nn::yb::edit::CEdit *)argEdit;

	edit->onDialog( argDialog);

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onDialog( void *argDialog) {

	nn::yb::common::CDialog * dialog = (nn::yb::common::CDialog *)argDialog;

	switch(dialog->id) {
		case nn::yb::e::edit::DIALOG_ID_END:
			switch(dialog->buttonNo) {
				case nn::yb::e::edit::DIALOG_BUTTON_NO_OK:
					this->ctrl->editMenu->start();
					this->ctrl->changeWindowSize( nn::yb::e::global::WINDOW_W, nn::yb::e::global::WINDOW_H);
					this->hole->taskDelete();
					this->taskDelete();
					break;
				case nn::yb::e::edit::DIALOG_BUTTON_NO_CANCEL:
					break;
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onButtonEditModeCb(void *argEdit, void *argButton) {

	nn::yb::edit::CEdit * edit = (nn::yb::edit::CEdit *)argEdit;

	edit->onButtonEditMode( argButton);

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onDelete() {

	nn::yb::edit::CEditHoleObj * obj = this->hole->obj[this->activeObjIndex];
	if(obj && obj->category != nn::yb::e::edit_hole_obj::CATEGORY_BALL) {
		obj->taskDelete();
		for( int i = this->activeObjIndex; i < this->hole->objCnt - 1; i++) {

			this->hole->obj[i] = this->hole->obj[i + 1];

		}
		this->hole->obj[this->hole->objCnt - 1] = NULL;
		this->hole->objCnt--;
		if(this->activeObjIndex >= this->hole->objCnt) {
			this->activeObjIndex = this->hole->objCnt - 1;
		}
		this->hole->obj[this->activeObjIndex]->active();
	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onButtonEditMode(void *argButton) {

	nn::yb::common::CButton * button = (nn::yb::common::CButton *)argButton;

	switch(button->id) {
		case nn::yb::e::edit::BUTTON_ID_EDIT_DELETE:
			this->onDelete();
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onButtonCreateModeCb(void *argEdit, void *argButton) {

	nn::yb::edit::CEdit * edit = (nn::yb::edit::CEdit *)argEdit;

	edit->onButtonCreateMode( argButton);

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onButtonCreateMode( void * argButton) {

	nn::yb::common::CButton * button = (nn::yb::common::CButton *)argButton;

	this->createMode = button->id - nn::yb::e::edit::BUTTON_ID_CREATE_GROUND + nn::yb::e::edit_hole_obj::CATEGORY_GROUND;

	this->setButtonStateChangeCreateMode();

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::setButtonStateChangeCreateMode() {

	for( int i = 0; i < this->createButtonN; i++) {

		nn::yb::common::CButton * button = this->createModeButton[i];

		if(button != NULL) {
			int button_create_mode = button->id - nn::yb::e::edit::BUTTON_ID_CREATE_GROUND + nn::yb::e::edit_hole_obj::CATEGORY_GROUND;
			if(button_create_mode == this->createMode) {
				button->state = nn::yb::e::button::STATE_ON;
				this->createButtonCursorX = i;
			}
			else {
				button->state = nn::yb::e::button::STATE_OFF;
			}
		}

	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onButtonModeCb( void * argEdit, void * argButton) {

	nn::yb::edit::CEdit * edit = (nn::yb::edit::CEdit *)argEdit;

	edit->onButtonMode( argButton);

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::onButtonMode(void *argButton) {

	nn::yb::common::CButton * button = (nn::yb::common::CButton *)argButton;
	
	int mode = nn::yb::e::edit::MODE_CREATE;
	switch(button->id) {
		case nn::yb::e::edit::BUTTON_ID_CREATE:
			mode = nn::yb::e::edit::MODE_CREATE;
			break;
		case  nn::yb::e::edit::BUTTON_ID_EDIT:
			mode = nn::yb::e::edit::MODE_EDIT;
			break;
	}
	this->changeMode( mode);

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::changeMode(int mode) {

	this->mode = mode;
	this->setButtonStateChangeMode();

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::setButtonStateAllHide() {
	
	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( this->modeButton); i++) {

		nn::yb::common::CButton * button = this->modeButton[i];

		if(button != NULL) {
			button->state = nn::yb::e::button::STATE_HIDE;
		}

	}

	this->setButtonStateCreateModeHide();
	this->setButtonStateEditModeHide();

	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( this->commonButton); i++) {

		nn::yb::common::CButton * button = this->commonButton[i];
		if(button != NULL) {
			button->state = nn::yb::e::button::STATE_HIDE;
		}

	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::setButtonStateCreateModeHide() {

	for( int i = 0; i < this->createButtonN; i++) {

		nn::yb::common::CButton * button = this->createModeButton[i];
		if(button) {
			button->state = nn::yb::e::button::STATE_HIDE;
		}

	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::setButtonStateEditModeHide() {
	
	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( this->editModeButton); i++) {

		nn::yb::common::CButton * button = this->editModeButton[i];
		if(button) {
			button->state = nn::yb::e::button::STATE_HIDE;
		}

	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::setButtonStateChangeMode() {

	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( this->modeButton); i++) {

		nn::yb::common::CButton * button = this->modeButton[i];

		if(button != NULL) {
			if(button->id == this->mode) {
				button->state = nn::yb::e::button::STATE_ON;
			}
			else {
				button->state = nn::yb::e::button::STATE_OFF;
			}
		}

	}

	switch(this->mode) {
		case nn::yb::e::edit::MODE_CREATE:
			{
				this->setButtonStateChangeCreateMode();
				this->setButtonStateEditModeHide();
			}
			break;
		case nn::yb::e::edit::MODE_EDIT:
			{
				for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( this->editModeButton); i++) {

					nn::yb::common::CButton * button = this->editModeButton[i];
					if(button) {
						button->state = nn::yb::e::button::STATE_ON;
					}

				}
				this->setButtonStateCreateModeHide();
			}
			break;
	}


}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::activeClearObjAll() {

	for( int i = 0; i < this->hole->objCnt; i++) {

		nn::yb::edit::CEditHoleObj * obj = this->hole->obj[i];

		if(obj) {
			obj->activeFlag = false;
		}

	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CEdit::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	::TOUCH_WORK * touch = &this->master->touchWork;
	::KEYBOARD_WORK * kb = &this->master->keyboardWork;
	
	if((kb->press[KEYBOARD_LCONTROL] || kb->press[KEYBOARD_RCONTROL]) &&
		(kb->press[KEYBOARD_LSHIFT] || kb->press[KEYBOARD_RSHIFT])
	) {
		// 全てのカメラを削除
		if(kb->trigger[KEYBOARD_DELETE]) {
			for( int i = 0; i < this->hole->objCnt; i++) {

				if(this->hole->obj[i]->category == nn::yb::e::edit_hole_obj::CATEGORY_CAMERA) {
					this->activeObjIndex = i;
					this->onDelete();
					i--;
				}

			}
		}
	}
	else if(kb->press[KEYBOARD_LCONTROL] || kb->press[KEYBOARD_RCONTROL]) {
		if(kb->trigger[KEYBOARD_S]) {
			this->onSave();
		}
		else if(kb->trigger[KEYBOARD_P]) {
			this->play();
		}
	}
	else if(kb->press[KEYBOARD_LSHIFT] || kb->press[KEYBOARD_RSHIFT]) {
		if(kb->press[KEYBOARD_T] == 0) {
			if(key_commonCursorMoveLR( key, this->hole->objCnt - 1, &this->activeObjIndex)) {
				this->hole->obj[this->activeObjIndex]->active();
				this->xWindow = this->hole->obj[this->activeObjIndex]->x1 - this->wWindow / 2;
				this->yWindow = this->hole->obj[this->activeObjIndex]->y1 - this->hWindow / 2;
				disp_commonSetCameraPos( this->xWindow + this->wWindow / 2, this->yWindow + this->hWindow / 2);
			}
		}
	}
	else if(kb->press[KEYBOARD_LALT] || kb->press[KEYBOARD_RALT]) {
		if(key->press & nn::common::e::PAD_UP) {
			this->scale += 0.01;
		}
		else if(key->press & nn::common::e::PAD_DOWN) {
			this->scale -= 0.01;
		}
	}
	else if(kb->trigger[KEYBOARD_F1]) {
		// オブジェクト全体の位置をずらす
		for( int i = 0; i < this->hole->objCnt; i++) {

			nn::yb::edit::CEditHoleObj * obj = this->hole->obj[i];
			obj->x1 += 2048;
			obj->y1 += 2048;
			obj->x2 += 2048;
			obj->y2 += 2048;

		}
	}
	else if(kb->trigger[KEYBOARD_C]) {
		this->changeMode( nn::yb::e::edit::MODE_CREATE);
	}
	else if(kb->trigger[KEYBOARD_E]) {
		this->changeMode( nn::yb::e::edit::MODE_EDIT);
	}
	else if(kb->trigger[KEYBOARD_DELETE]) {
		this->onDelete();
	}
	else if(kb->trigger[KEYBOARD_F4]) {
		this->onEnd();
	}

	bool window_scroll_flag = false;
	if(this->createState == nn::yb::e::edit::CREATE_STATE_NONE &&
		this->hole->obj[this->activeObjIndex]->state == nn::yb::e::edit_hole_obj::STATE_NONE
	) {
		window_scroll_flag = this->taskWindowScrollHand();
	}
	if(!window_scroll_flag) {
		switch(this->mode) {
			case nn::yb::e::edit::MODE_CREATE:
				this->taskCreate();
				break;
			case nn::yb::e::edit::MODE_EDIT:
				this->taskEdit();
				break;

		}
	}

	this->disp();

}
//---------------------------------------------------------------------------
bool nn::yb::edit::CEdit::taskWindowScrollHand() {
	
	::KEY_WORK * key = &this->master->keyWork;
	::TOUCH_WORK * touch = &this->master->touchWork;
	::KEYBOARD_WORK * kb = &this->master->keyboardWork;

	if(this->windowScrollFlag) {
		if(kb->press[KEYBOARD_SPACE] && (touch->touchPoint & TOUCH_PRESS)) {
			this->xWindow -= touch->x - this->xSlideOld;
			this->yWindow -= touch->y - this->ySlideOld;
			if(this->xWindow < 0) {
				this->xWindow = 0;
			}
			if(this->yWindow < 0) {
				this->yWindow = 0;
			}
			this->xSlideOld = touch->x;
			this->ySlideOld = touch->y;
			disp_commonSetCameraPos( this->xWindow + this->wWindow / 2, this->yWindow + this->hWindow / 2);
		}
		else {
			this->windowScrollFlag = false;
		}
		return true;
	}
	else {
		if(kb->press[KEYBOARD_SPACE] && (touch->touchPoint & TOUCH_PRESS)) {
			this->windowScrollFlag = true;
			this->xSlideOld = touch->x;
			this->ySlideOld = touch->y;
			this->disp();
			return true;
		}
	}
	return false;

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::setWindowPos( int x, int y) {

	this->xWindow = x;
	if(this->xWindow < 0) {
		this->xWindow = 0;
	}
	this->yWindow = y;
	if(this->yWindow < 0) {
		this->yWindow = 0;
	}
	disp_commonSetCameraPos( this->xWindow + this->wWindow / 2, this->yWindow + this->hWindow / 2);

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::taskWindowScrollOut() {

	::TOUCH_WORK * touch = &this->master->touchWork;
	
	if(touch->touchPoint & TOUCH_PRESS) {
		bool window_move_flag = false;
		int x_var = 0;
		int move_step_1 = 30;
		int move_step_2 = 120;
		int speed_1 = 2;
		int speed_2 = 6;
		int speed_3 = 16;
		if(touch->x < 0) {
			if(touch->x >= -move_step_1) {
				x_var = -speed_1;
			}
			else if(touch->x >= -move_step_2) {
				x_var = -speed_2;
			}
			else {
				x_var = -speed_3;
			}
			window_move_flag = true;
		}
		else if(touch->x > this->wWindow) {
			if(touch->x <= this->wWindow + move_step_1) {
				x_var = speed_1;
			}
			else if(touch->x <= this->wWindow + move_step_2) {
				x_var = speed_2;
			}
			else {
				x_var = speed_3;
			}
			window_move_flag = true;
		}
		int y_var = 0;
		if(touch->y < 0) {
			if(touch->y >= -move_step_1) {
				y_var = -speed_1;
			}
			else if(touch->y >= -move_step_2) {
				y_var = -speed_2;
			}
			else {
				y_var = -speed_3;
			}
			window_move_flag = true;
		}
		else if(touch->y > this->hWindow) {
			if(touch->y <= this->hWindow + move_step_1) {
				y_var = speed_1;
			}
			else if(touch->y <= this->yWindow + move_step_2) {
				y_var = speed_2;
			}
			else {
				y_var = speed_3;
			}
			window_move_flag = true;
		}
		if(window_move_flag) {
			this->setWindowPos( this->xWindow + x_var, this->yWindow + y_var);
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::taskCreate() {

	::KEY_WORK * key = &nn::yb::common::getMaster()->keyWork;

	::TOUCH_WORK * touch = &this->master->touchWork;
	int touch_x_correct = touch->x + this->xWindow;
	int touch_y_correct = touch->y + this->yWindow;
	int release_x_correct = touch->release_x + this->xWindow;
	int release_y_correct = touch->release_y + this->yWindow;

	KEYBOARD_WORK * kb = &this->master->keyboardWork;

	if(kb->press[KEYBOARD_LCONTROL] || kb->press[KEYBOARD_RCONTROL]) {
		if((key->trigger & nn::common::e::PAD_UP) || (key->pulse & nn::common::e::PAD_UP)) {
			this->createButtonCursorX -= 4;
			if(this->createButtonCursorX < 0) {
				this->createButtonCursorX += this->createButtonN;
			}
			this->createMode = this->createModeButton[this->createButtonCursorX]->id - nn::yb::e::edit::BUTTON_ID_CREATE_GROUND + nn::yb::e::edit_hole_obj::CATEGORY_GROUND;
			this->setButtonStateChangeCreateMode();
		}
		else if((key->trigger & nn::common::e::PAD_DOWN) || (key->pulse & nn::common::e::PAD_DOWN)) {
			this->createButtonCursorX += 4;
			if(this->createButtonCursorX >= this->createButtonN) {
				this->createButtonCursorX -= this->createButtonN;
			}
			this->createMode = this->createModeButton[this->createButtonCursorX]->id - nn::yb::e::edit::BUTTON_ID_CREATE_GROUND + nn::yb::e::edit_hole_obj::CATEGORY_GROUND;
			this->setButtonStateChangeCreateMode();
		}
		else if(key_commonCursorMoveLR( key, this->createButtonN - 1, &this->createButtonCursorX)) {
			this->createMode = this->createModeButton[this->createButtonCursorX]->id - nn::yb::e::edit::BUTTON_ID_CREATE_GROUND + nn::yb::e::edit_hole_obj::CATEGORY_GROUND;
			this->setButtonStateChangeCreateMode();
		}
	}
	
	switch(this->createState) {
		case nn::yb::e::edit::CREATE_STATE_NONE:
			if(touch->touchPoint & TOUCH_TOUCH) {
				switch(this->createMode) {
					// サイズ変更のある広げるタイプ
					case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
					case nn::yb::e::edit_hole_obj::CATEGORY_GROUND:
					case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_LEFT:						
					case nn::yb::e::edit_hole_obj::CATEGORY_DAMAGE:
					case nn::yb::e::edit_hole_obj::CATEGORY_BUNKER:
					case nn::yb::e::edit_hole_obj::CATEGORY_GRAVITY:
					case nn::yb::e::edit_hole_obj::CATEGORY_FUKIAGE: 
					case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE:
					case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA:
					case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_START:
					case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_GOAL:
						{
#if 1
							// クラスセーブは、タスク未対応
							nn::yb::edit::CEditHoleObj * obj = new nn::yb::edit::CEditHoleObj( this->hole, this->saveManager);
#else
							nn::yb::edit::CEditHoleObj * obj = new nn::yb::edit::CEditHoleObj( this);
#endif
							//obj->category = nn::yb::e::edit_hole_obj::CATEGORY_GROUND;	// createMode と category は共通
							obj->category = this->createMode;
							obj->x1 = touch_x_correct;
							obj->y1 = touch_y_correct;
							obj->x2 = touch_x_correct;
							obj->y2 = touch_y_correct;
							obj->hole = this->hole;
							obj->start();
							this->hole->obj[this->hole->objCnt] = obj;
							this->hole->objCnt++;

							this->createState = nn::yb::e::edit::CREATE_STATE_CREATE;
						}
						break;
					// サイズ変更のない置くタイプ
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA:
					case nn::yb::e::edit_hole_obj::CATEGORY_PILLAR:
					case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_KICK_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_KICK_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SKY:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO_SKY:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA_SKY:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA_SKY:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA_SKY:
					case nn::yb::e::edit_hole_obj::CATEGORY_DASH:
					case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_JUMP:
					case nn::yb::e::edit_hole_obj::CATEGORY_CAMERA:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA_SKY:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MOMIJI:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_TENKO:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_GREEN_FAIRY:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_BLUE_FAIRY:
					case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN:
					case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_REMIFLA:
					case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
					case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_WEAK: 
					case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_STRONG: 
					case nn::yb::e::edit_hole_obj::CATEGORY_SE_FLOOR:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_NITORI:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_FLAN:
					case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_ORIN:
						{
#if 1
							// クラスセーブは、タスク未対応
							nn::yb::edit::CEditHoleObj * obj = new nn::yb::edit::CEditHoleObj( this->hole, this->saveManager);
#else
							nn::yb::edit::CEditHoleObj * obj = new nn::yb::edit::CEditHoleObj( this);
#endif
							obj->category = this->createMode;
							obj->x1 = touch_x_correct;
							obj->y1 = touch_y_correct;
							obj->hole = this->hole;
							obj->start();
							this->hole->obj[this->hole->objCnt] = obj;
							this->hole->objCnt++;
							
							this->createState = nn::yb::e::edit::CREATE_STATE_CREATE;
						}
						break;
				}
			}
			break;
		case nn::yb::e::edit::CREATE_STATE_CREATE:
			{
				this->taskWindowScrollOut();

				nn::yb::edit::CEditHoleObj * obj = this->hole->obj[this->hole->objCnt - 1];
				if(touch->touchPoint & TOUCH_PRESS) {
					if(touch_x_correct >= 0) {
						obj->x2 = touch_x_correct;
					}
					else {
						obj->x2 = 0;
					}
					if(touch_y_correct >= 0) {
						obj->y2 = touch_y_correct;
					}
					else {
						obj->y2 = 0;
					}
				}
				else if(touch->touchPoint & TOUCH_RELEASE) {
					if(release_x_correct >= 0) {
						obj->x2 = release_x_correct;
					}
					else {
						obj->x2 = 0;
					}
					if(release_y_correct >= 0) {
						obj->y2 = release_y_correct;
					}
					else {
						obj->y2 = 0;
					}
					int x1_temp = obj->x1;
					int x2_temp = obj->x2;
					if(x1_temp > x2_temp) {
						x1_temp = obj->x2;
						x2_temp = obj->x1;
					}
					int y1_temp = obj->y1;
					int y2_temp = obj->y2;
					if(y1_temp > y2_temp) {
						y1_temp = obj->y2;
						y2_temp = obj->y1;
					}
					if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_BUMPER ||
						obj->category == nn::yb::e::edit_hole_obj::CATEGORY_GROUND
					) {
						if(x2_temp - x1_temp < 8 && y2_temp - y1_temp < 8) {
							obj->taskDelete();
							this->hole->obj[this->hole->objCnt - 1] = NULL;
							this->hole->objCnt--;
						}
					}
					this->createState = nn::yb::e::edit::CREATE_STATE_NONE;
				}
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::taskEdit() {
	
	::KEY_WORK * key = &this->master->keyWork;
	::TOUCH_WORK * touch = &this->master->touchWork;
	::KEYBOARD_WORK * kb = &this->master->keyboardWork;

	if(this->hole->obj[this->activeObjIndex]->state == nn::yb::e::edit_hole_obj::STATE_MOVE ||
		this->hole->obj[this->activeObjIndex]->state == nn::yb::e::edit_hole_obj::STATE_TRANS
	) {
		this->taskWindowScrollOut();
	}
	else {
		/*
		if(touch->touchPoint & TOUCH_TOUCH) {

			int active_obj_index = this->activeObjIndex;
			nn::yb::edit::CEditHoleObj * active_obj = this->hole->obj[active_obj_index];
			bool obj_touch_flag = false;

			if(key->press & nn::common::e::PAD_WIN_BUTTON_Y) {
				int index = active_obj_index + 1;
				for( int i = 0; i < this->hole->objCnt; i++ ) {

					if(index >= this->hole->objCnt) {
						index = 0;
					}

					nn::yb::edit::CEditHoleObj * obj = this->hole->obj[index];

					if(obj) {
						if(obj->isTouch()) {
							obj_touch_flag = true;
							active_obj_index = index;
							break;
						}
					}

					index++;

				}
			}
			else if(active_obj->isTouch()) {
				// 通常はアクティブオブジェを優先
				this->hole->obj[this->activeObjIndex]->touchActive();
			}
			else {
				// 小さいオブジェを優先してアクティブにする
				int min_area = INT_MAX;
				for( int i = 0; i < this->hole->objCnt; i++ ){

					nn::yb::edit::CEditHoleObj * obj = this->hole->obj[i];

					if(obj) {
						if(obj->isTouch()) {
							obj_touch_flag = true;
							// 小さいオブジェを優先してアクティブにする
							switch(obj->category) {
								case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
									active_obj_index = i;
									min_area = 0;
									break;
								case nn::yb::e::edit_hole_obj::CATEGORY_BALL:
								case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY:
									{
										int area = 64 * 64;
										if(area < min_area) {
											active_obj_index = i;
											min_area = area;
										}
									}
									break;
								default:
									{
										int w = obj->x2 - obj->x1;
										if(obj->x1 > obj->x2) {
											w = obj->x1 - obj->x2;
										}
										int h = obj->y2 - obj->y1;
										if(obj->y1 > obj->y2) {
											h = obj->y1 - obj->y2;
										}
										int area = w * h;
										if(area < min_area) {
											active_obj_index = i;
											min_area = area;
										}
									}
									break;
							}
						}
					}

				}
			}
			if(obj_touch_flag) {
				if(active_obj_index != this->activeObjIndex) {
					this->activeObjIndex = active_obj_index;
					this->hole->obj[this->activeObjIndex]->active();
				}
				else {
					// アクティブオブジェをタッチ
					// 挙動がイマイチ アクティブオブジェを切り替えるつもりの時に移動、変形状態になっても困る
					//this->hole->obj[this->activeObjIndex]->touchActive();
				}
			}

		}
		*/
		if(touch->touchPoint & TOUCH_TOUCH) {
			int active_obj_index = this->activeObjIndex;
			nn::yb::edit::CEditHoleObj * active_obj = this->hole->obj[active_obj_index];
			bool obj_touch_flag = false;

			bool active_obj_ground_flag = false;
			switch(active_obj->category) {
				case nn::yb::e::edit_hole_obj::CATEGORY_GROUND:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_DAMAGE:
				case nn::yb::e::edit_hole_obj::CATEGORY_BUNKER:
				case nn::yb::e::edit_hole_obj::CATEGORY_GRAVITY:
					active_obj_ground_flag = true;
					break;
				default:
					break;
			}

			if(kb->press[KEYBOARD_LSHIFT] || kb->press[KEYBOARD_RSHIFT]) {
				// 他のブオブジェをアクティブにする
				int index = active_obj_index + 1;
				for( int i = 0; i < this->hole->objCnt; i++ ) {

					if(index >= this->hole->objCnt) {
						index = 0;
					}

					nn::yb::edit::CEditHoleObj * obj = this->hole->obj[index];

					if(obj) {
						if(obj->isTouch()) {
							obj_touch_flag = true;
							active_obj_index = index;
							break;
						}
					}

					index++;

				}
				if(obj_touch_flag) {
					if(active_obj_index != this->activeObjIndex) {
						this->activeObjIndex = active_obj_index;
						this->hole->obj[this->activeObjIndex]->active();
					}
				}
			}
			else {
				// 小さいオブジェを優先してアクティブにする
				// アクティブオブジェがタッチ箇所にある場合、最優先で移動、変形
				int min_area = INT_MAX;
				for( int i = 0; i < this->hole->objCnt; i++ ){

					nn::yb::edit::CEditHoleObj * obj = this->hole->obj[i];

					if(obj) {
						if(obj->isTouch()) {
							obj_touch_flag = true;
							// 小さいオブジェを優先してアクティブにする
							int area;
							switch(obj->category) {
								case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
									area = 1;
									break;
								case nn::yb::e::edit_hole_obj::CATEGORY_BALL:
								case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY:
								case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO:
								case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA:
								case nn::yb::e::edit_hole_obj::CATEGORY_PILLAR:
								case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SKY:
								case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO_SKY:
								case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA_SKY:
								case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA:
								case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA_SKY:
								case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA:
								case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA_SKY:
									area = 64 * 48;
									break;
								case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP:
								case nn::yb::e::edit_hole_obj::CATEGORY_KICK_RIGHT:
								case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN:
								case nn::yb::e::edit_hole_obj::CATEGORY_KICK_LEFT:
								case nn::yb::e::edit_hole_obj::CATEGORY_DASH:
								case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP:
								case nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT:
								case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN:
								case nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT:
								case nn::yb::e::edit_hole_obj::CATEGORY_JUMP:
									area = 96 * 96;
									break;
								default:
									{
										int w = obj->x2 - obj->x1;
										if(obj->x1 > obj->x2) {
											w = obj->x1 - obj->x2;
										}
										int h = obj->y2 - obj->y1;
										if(obj->y1 > obj->y2) {
											h = obj->y1 - obj->y2;
										}
										area = w * h;
									}
									break;
							}

							// アクティブオブジェ最優先
							if(this->hole->obj[i]->activeFlag) {
								active_obj_index = i;
								break;
							}
							else if(area < min_area) {
								active_obj_index = i;
								min_area = area;
							}
						}
					}

				}
				if(obj_touch_flag) {
					if(active_obj_index != this->activeObjIndex) {
						this->activeObjIndex = active_obj_index;
						this->hole->obj[this->activeObjIndex]->active();
					}
					else {
						this->hole->obj[this->activeObjIndex]->touchActive();
					}
				}
			}

		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::edit::CEdit::disp() {
	
	nn::yb::edit::CEditHoleObj * obj = this->hole->obj[this->activeObjIndex];
	
	fontPrintfType( this->objDataDispX, 15 * 0, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "%s", nn::yb::edit::gEditObjNameTbl[obj->category]);

	// CATEGORY_CAMERAはscaleに対応していない
	if(this->scale == 1.0) {
		int camera_cnt = 0;
		int last_camera_index = 0;
		for( int i = 0; i < this->hole->objCnt; i++) {

			nn::yb::edit::CEditHoleObj * o = this->hole->obj[i];
			if(o->category == nn::yb::e::edit_hole_obj::CATEGORY_CAMERA) {
				if(camera_cnt == 0) {
					disp_commonMain3dBoardCenter( o->x1, o->y1, 1, 32, 32, 1, RGB_BLUE, ALPHA_DEFAULT);
				}
				else {
					disp_commonMain3dLine( this->hole->obj[last_camera_index]->x1, this->hole->obj[last_camera_index]->y1, o->x1, o->y1, 1, 5, RGB_WHITE, ALPHA_DEFAULT);
					disp_commonMain3dLine( this->hole->obj[last_camera_index]->x1, this->hole->obj[last_camera_index]->y1, o->x1, o->y1, 0, 3, RGB_BLACK, ALPHA_DEFAULT);
					disp_commonMain3dBoardCenter( o->x1, o->y1, 1, 32, 32, 0, RGB_GREEN, ALPHA_DEFAULT);
				}
				camera_cnt++;
				last_camera_index = i;
			}

		}

		if(camera_cnt >= 2) {
			disp_commonMain3dBoardCenter( this->hole->obj[last_camera_index]->x1, this->hole->obj[last_camera_index]->y1, 0, 32, 32, 0, RGB_RED, ALPHA_DEFAULT);
		}
	}

	int x1 = obj->x1 - 2048;
	int y1 = obj->y1 - 2048;
	int x2 = obj->x2 - 2048;
	int y2 = obj->y2 - 2048;

	switch(obj->category) {
		case nn::yb::e::edit_hole_obj::CATEGORY_BALL:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP:
		case nn::yb::e::edit_hole_obj::CATEGORY_CAMERA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA_SKY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MOMIJI:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_TENKO:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_GREEN_FAIRY:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_BLUE_FAIRY:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_REMIFLA:
		case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_WEAK: 
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_STRONG: 
		case nn::yb::e::edit_hole_obj::CATEGORY_SE_FLOOR:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_NITORI:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_FLAN:
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_ORIN:
		case nn::yb::e::edit_hole_obj::CATEGORY_PILLAR:
			fontPrintfType( this->objDataDispX, 15 * 1, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "x = %4d z = %4d", x1, obj->z1);
			fontPrintfType( this->objDataDispX, 15 * 2, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "y = %4d", y1);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
			{
				fontPrintfType( this->objDataDispX, 15 * 1, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "x1 = %4d x2 = %4d", x1, x2);
				fontPrintfType( this->objDataDispX, 15 * 2, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "y1 = %4d y2 = %4d", y1, y2);
				fontPrintfType( this->objDataDispX, 15 * 3,
					nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE,
					"angle = %lf", nn::yb::common::yb_commonVec2angle( (double)(x2 - x1), (double)(y2 - y1))
				);
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_GROUND:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_START:
		case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_GOAL:
			fontPrintfType( this->objDataDispX, 15 * 1, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "x1 = %4d x2 = %4d", x1, x2);
			fontPrintfType( this->objDataDispX, 15 * 2, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "y1 = %4d y2 = %4d", y1, y2);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA:
			fontPrintfType( this->objDataDispX, 15 * 1, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "x1 = %4d x2 = %4d", x1, x2);
			fontPrintfType( this->objDataDispX, 15 * 2, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "y1 = %4d y2 = %4d", y1, y2);
			fontPrintfType( this->objDataDispX, 15 * 3, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "z1 = %4d", obj->z1);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_DAMAGE:
		case nn::yb::e::edit_hole_obj::CATEGORY_BUNKER:
		case nn::yb::e::edit_hole_obj::CATEGORY_GRAVITY:
		case nn::yb::e::edit_hole_obj::CATEGORY_FUKIAGE: 
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT:
			fontPrintfType( this->objDataDispX, 15 * 1, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "x1 = %4d w = %4d", x1, x2 - x1);
			fontPrintfType( this->objDataDispX, 15 * 2, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "y1 = %4d h = %4d", y1, y2 - y1);
			break;
	}
	
	fontPrintfType( 0, 480 - 10, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE,
		"scale = %lf", this->scale
	);

}
//---------------------------------------------------------------------------

