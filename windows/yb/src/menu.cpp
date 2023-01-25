// menu を置換
// Menu を置換
// yb を置換
// main を置換

#include "nn_common.h"

#include "font.h"
#include "disp_common.h"
#include "key_common.h"
#include "sound_nns.h"
#include "draw.h"
#include "direct_common.h"
#include "animation.h"
#include "task.h"
#include "image_ctrl.h"

#include "enum.h"
#include "ctrl.h"
#include "edit_menu.h"
#include "yb_save_manager.h"
#include "save_data.h"
#include "stage.h"
#include "config.h"
#include "yb_common.h"
#include "ending.h"
#include "flower.h"

#include "sound_data.sadl"
#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "animation_data.h"

#include "menu.h"
//---------------------------------------------------------------------------
char * gTextTblMainMenu[] = {
	"ゲーム",
	"エディット",
};
char * gTextTblGameMenu[nn::yb::e::menu::CURSOR_Y_GAME_MAX] = {
	"中断した所から",
	"スタート",
	"スコア",
	"コンフィグ",
	"終了",
};
char * gTextGuideGameMain[] = {
	"キーボードでの操作は、",
	"　カーソルキー     ： 移動",
	"　XキーorEnterキー ： 決定",
	"　Zキー            ： キャンセル",
	"ですよ。",
};
char * gTextWarning[] = {
	"",
	"",
	"",
	"本ソフトは東方Projectの二次創作です。",
	"当サークルは、上海アリス幻樂団様とは一切関係ありません。",
	"",
	"バグ修正・追加要素のアップデートパッチは、",
	"agrippaのサイトで公開いたします。",
	"",
	"その他問い合わせは、agrippaのサイトか",
	"n2n.lj.infoseek@gmail.com えぬに までお願いします。",
};

char * gTextGuideGameModeSelectChaos[] = {
	"ステージモードを選んでくださいね。",
	"「カオスステージモード」",
	"開発者の未熟と有限の時が生んだ",
	"クリアさえも危ういステージ",
	"Ver.1.03までのほぼ勢いだけで作",
	"られたステージがこちら",
};

char * gTextGuideGameModeSelectStandard[] = {
	"ステージモードを選んでくださいね。",
	"「スタンダードステージモード」",
	"浄化された本来の姿のステージ",
	"安心してゆっくりやり込んでいってね！",
	"Ver.1.10で登場した作りこまれたステ",
	"ージがこちら",
};

char * gTextScoreStageMode[] = {
	"カオス",
	"スタンダード",
};

//---------------------------------------------------------------------------
nn::yb::main::CMenu::CMenu(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CMenu Create\n");
	this->state = nn::yb::e::menu::STATE_NONE;
	this->oldState = nn::yb::e::menu::STATE_NONE;
	this->waitCnt = 0;

	this->ctrl = NULL;
	this->saveData = NULL;

	this->gameState = nn::yb::e::menu::GAME_STATE_MAIN;
	this->cursorYGame = 0;
	this->cursorYScore = 0;

	this->logoFrameCnt = 0;
	this->logoState = nn::yb::e::menu::LOGO_STATE_WARNING;

	this->animeTitleTextTask = NULL;
	this->titleState = nn::yb::e::menu::TITLE_STATE_FADE_IN;

	this->animeSanaeTask = NULL;

	this->stageStartFlag = false;
	this->sanaeInCompFlag = false;

	this->frameCnt = 0;
	this->loadState = nn::yb::e::menu::LOAD_STATE_IMAGE;
	this->loadBgmCnt = 0;

	this->config = NULL;

	this->rootFlower = NULL;

	this->cursorXScore = nn::yb::e::menu::CURSOR_X_SCORE_STANDARD;

}
//---------------------------------------------------------------------------
nn::yb::main::CMenu::~CMenu() {

	::nn_commonLog( 1, "CMenu Delete\n");

	
	this->ctrl->releaseLoadingImg();//ロード画面解放
}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::start() {

	this->ctrl->loadLoadingImg();//ロード画面読み込み
	
	image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_TITLE);
	image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_MENU);
	this->loadState = nn::yb::e::menu::LOAD_STATE_IMAGE;
	this->state = nn::yb::e::menu::STATE_LOAD;

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::taskMain() {
	
	::KEY_WORK * key = &this->master->keyWork;

	if(::key_commonCursorMoveUpDown( key, NN_COMMON_GET_ARRAY_SIZE(gTextTblMainMenu) - 1, &this->saveData->menuStartIndex)) {
		sound_nnsPlaySe( SE_U);
	}
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_A) {
		sound_nnsPlaySe( SE_DEDON);
		switch(this->saveData->menuStartIndex) {
			case nn::yb::e::menu::CURSOR_Y_GAME:
				{
					this->goGameMain();
				}
				break;
			case nn::yb::e::menu::CURSOR_Y_EDIT:
				{
					nn::yb::edit::CEditMenu * edit_menu = new nn::yb::edit::CEditMenu( this);
					this->ctrl->editMenu = edit_menu;
					edit_menu->master = this->master;
					edit_menu->ctrl = this->ctrl;
					edit_menu->saveManager = this->ctrl->saveManagerEdit;
					edit_menu->saveData = this->saveData;
					edit_menu->start();
					if(this->animeSanaeTask != NULL) {
						::taskDelete( (::TASK *)this->animeSanaeTask);
						this->animeSanaeTask = NULL;
					}
					this->ctrl->state = nn::yb::e::ctrl::STATE_EDIT;
					this->state = nn::yb::e::menu::STATE_NONE;
				}
				break;
		}
	}
#if defined(_DEBUG)
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_B) {
		nn::yb::main::CEnding * ending = new nn::yb::main::CEnding( this);
		ending->allTotalScore = 100;
		ending->master = this->master;
		ending->start();
	}
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_X) {
	}
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_Y) {
	}
#endif //_DEBUG

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::initFlower() {

	this->rootFlower = new nn::yb::main::CFlower( this);

	for( int i = 0; i < 100; i++) {
		nn::yb::main::CFlower * flower = new nn::yb::main::CFlower( this->rootFlower);
		flower->x = -50 + n_commonRandN( 160) * 4;
		flower->y = -50;
		flower->objId = OBJ_ID_MENU_BG_ICON_0 + n_commonRandN(OBJ_ID_MENU_BG_ICON_4 - OBJ_ID_MENU_BG_ICON_0 + 1);
		flower->start();

		flower->state = nn::yb::e::flower::STATE_MOVE_HIDE;
		for( int j = 0; j < i * 20; j++) {
			flower->task();
		}
		flower->state = nn::yb::e::flower::STATE_MOVE_NORMAL;

	}
		

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::taskCreateFlower() {

	if(this->frameCnt % 20 == 0) {
		nn::yb::main::CFlower * flower = new nn::yb::main::CFlower( this->rootFlower);
		flower->x = -50 + n_commonRandN( 160) * 4;
		flower->y = -50;
		flower->objId = OBJ_ID_MENU_BG_ICON_0 + n_commonRandN(OBJ_ID_MENU_BG_ICON_4 - OBJ_ID_MENU_BG_ICON_0 + 1);
		flower->start();
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::taskGameMain() {

	::KEY_WORK * key = &this->master->keyWork;

	if(::key_commonCursorMoveUpDown( key, nn::yb::e::menu::CURSOR_Y_GAME_MAX - 1, &this->cursorYGame)) {
		if( (!this->saveData->breakData.breakFlag) && this->cursorYGame == 0) {
			::key_commonCursorMoveUpDown( key, nn::yb::e::menu::CURSOR_Y_GAME_MAX - 1, &this->cursorYGame);
		}
		sound_nnsPlaySe( SE_U);
	}
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_A) {
		sound_nnsPlaySe( SE_DEDON);
		switch(this->cursorYGame) {
			case nn::yb::e::menu::CURSOR_Y_GAME_RESUME:
				{
					this->cutoutSanae( nn::yb::main::CMenu::startStageFromBreakCb, this);
				}
				break;
			case nn::yb::e::menu::CURSOR_Y_GAME_START:
				{
					this->gameState = nn::yb::e::menu::GAME_STATE_STAGE_MODE_SELECT;
				}
				break;
			case nn::yb::e::menu::CURSOR_Y_GAME_SCORE:
				{
					this->gameState = nn::yb::e::menu::GAME_STATE_SCORE;
				}
				break;
			case nn::yb::e::menu::CURSOR_Y_GAME_CONFIG:
				{
					nn::yb::main::CConfig * config = new nn::yb::main::CConfig( this);
					config->master = this->master;
					config->saveData = this->saveData;
					config->ctrl = this->ctrl;
					config->endCb = nn::yb::main::CMenu::returnGameStateMainCb;
					config->endCbArg = this;
					config->start();
					this->config = config;
					this->state = nn::yb::e::menu::STATE_CONFIG;
				}
				break;
			case nn::yb::e::menu::CURSOR_Y_GAME_QUIT:
				{
					this->master->gameContinueFlag = FALSE;
				}
				break;
		}
	}
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_B) {
#if defined(_DEBUG) || defined(__EDIT__)
		sound_nnsPlaySe( SE_BU);
		this->state = nn::yb::e::menu::STATE_MAIN;
#endif	// _DEBUG
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::taskGameStageModeSelect() {

	::KEY_WORK * key = &this->master->keyWork;

	if(key_commonCursorMoveUpDown( key, nn::yb::e::menu::STAGE_MODE_MAX - 1, 
			&this->saveData->gameStageModeSelect
		)
	) {
		sound_nnsPlaySe( SE_U);
	}
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_A) {
		sound_nnsPlaySe( SE_DEDON);
		this->gameState = nn::yb::e::menu::GAME_STATE_STAGE_SELECT;
	}
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_B) {
		sound_nnsPlaySe( SE_BU);
		this->gameState = nn::yb::e::menu::GAME_STATE_MAIN;
		if(this->cursorYGame == 0 && (!this->saveData->breakData.breakFlag)) {
			this->cursorYGame = 1;
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::taskGameStageSelect() {

	::KEY_WORK * key = &this->master->keyWork;

	int stage_open[nn::yb::e::menu::STAGE_MODE_MAX] = {
		this->saveData->stageOpen,				// nn::yb::e::menu::STAGE_MODE_CHAOS
		this->saveData->stageOpenStandard,		// nn::yb::e::menu::STAGE_MODE_STANDARD
	};

	if(::key_commonCursorMoveLR( key, stage_open[this->saveData->gameStageModeSelect] - 1,
			&this->saveData->gameStageSelect[this->saveData->gameStageModeSelect]
		)
	) {
		sound_nnsPlaySe( SE_U);
	}
	else if(::key_commonCursorMoveUpDown( key, nn::yb::e::menu::STAGE_MODE_MAX - 1,
			&this->saveData->gameStageModeSelect
		)
	) {
		sound_nnsPlaySe( SE_U);
	}
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_A) {
		sound_nnsPlaySe( SE_DEDON);
		this->saveData->breakData.breakFlag = false;
		this->ctrl->saveMain();

		this->cutoutSanae( nn::yb::main::CMenu::startStageFromScratchCb, this);

		/*
		// コールバックでの呼び出しに変更
		this->startStageFromScratch();
		*/
	}
#if defined(_DEBUG) == FALSE
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_B) {
		sound_nnsPlaySe( SE_BU);
		this->gameState = nn::yb::e::menu::GAME_STATE_STAGE_MODE_SELECT;
	}
#else	// _DEBUG
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_B) {
		if(this->saveData->gameStageModeSelect == nn::yb::e::menu::STAGE_MODE_CHAOS) {
			this->saveData->stageOpen = nn::yb::e::global::STAGE_N;
		}
		else {
			this->saveData->stageOpenStandard = nn::yb::e::global::STAGE_N;
		}
	}
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_Y) {
		this->saveData->stageOpen = 1;
	}
#endif	// _DEBUG

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::taskGameScore() {

	::KEY_WORK * key = &this->master->keyWork;

	if(::key_commonCursorMoveUpDown( key, nn::yb::e::menu::CURSOR_Y_SCORE_MAX -1, &this->cursorYScore)) {
		sound_nnsPlaySe( SE_U);
	}
	else if(::key_commonCursorMoveLR( key, nn::yb::e::menu::CURSOR_X_MAX - 1, &this->cursorXScore)) {
		sound_nnsPlaySe( SE_U);
	}
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_A) {
		if(this->cursorYScore == nn::yb::e::menu::CURSOR_Y_SCORE_QUIT) {
			sound_nnsPlaySe( SE_DEDON);
			this->gameState = nn::yb::e::menu::GAME_STATE_MAIN;
		}
	}
	else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_B) {
		sound_nnsPlaySe( SE_BU);
		this->gameState = nn::yb::e::menu::GAME_STATE_MAIN;
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::taskGame() {

	::KEY_WORK * key = &this->master->keyWork;

	this->taskCreateFlower();

	switch(this->gameState) {
		case nn::yb::e::menu::GAME_STATE_MAIN:
			this->taskGameMain();
			break;
		case nn::yb::e::menu::GAME_STATE_STAGE_MODE_SELECT:
			this->taskGameStageModeSelect();
			break;
		case nn::yb::e::menu::GAME_STATE_STAGE_SELECT:
			this->taskGameStageSelect();
			break;
		case nn::yb::e::menu::GAME_STATE_SCORE:
			this->taskGameScore();
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::taskLogo() {

	bool flag = false;

	::KEY_WORK * key = &this->master->keyWork;
	int change_frame_cnt = 90;
	switch(this->logoState ) {
		case nn::yb::e::menu::LOGO_STATE_WARNING:
			change_frame_cnt = 270;
			break;
		case nn::yb::e::menu::LOGO_STATE_MAIN:
			change_frame_cnt = 90;
			break;
	}

	if(key->trigger) {
		//flag = true;
		this->logoFrameCnt = 0;
		drawMainAllBrightness( -16);
		this->next();
		this->waitCnt++;
	}
	else if(this->logoFrameCnt >= 90 || flag) {
		this->logoFrameCnt = 0;
		direct_commonCreateTaskBlackFadeOutAllSimple( NULL, nn::yb::main::CMenu::nextCb, this);
		this->waitCnt++;
	}
	else {
		this->logoFrameCnt++;
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::taskTitle() {

	::KEY_WORK * key = &this->master->keyWork;

	switch(this->titleState) {
		case nn::yb::e::menu::TITLE_STATE_TEXT_IN:
			if(key->trigger) {
				animationEnd( ((ANIMATION *)((TASK *)this->animeTitleTextTask)->work));
			}
			break;
		case nn::yb::e::menu::TITLE_STATE_MAIN:
			this->titleFrameCnt++;
			if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_A) {
				this->goGameMain();
			}
#if defined(_DEBUG)
			else if(key->trigger & ::nn::common::e::PAD_WIN_BUTTON_B) {
				if(this->animeTitleTextTask != NULL) {
					TASK * anime_task = (TASK *)this->animeTitleTextTask;
					::taskDelete( anime_task);
					this->animeTitleTextTask = NULL;
				}
				this->goTitle();
			}
#endif //_DEBUG
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::taskLoad() {

	if(this->loadState == nn::yb::e::menu::LOAD_STATE_IMAGE) {
		if(image_ctrlGetLoadAsyncState() == IMAGE_CTRL_ASYNC_STATE_NONE) {
			this->nextLoad();
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CMenu::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}
	
	this->frameCnt++; 

	if(this->waitCnt <= 0) {
		switch(this->state) {
			case nn::yb::e::menu::STATE_LOAD:
				this->taskLoad();
				break;
			case nn::yb::e::menu::STATE_LOGO:
				this->taskLogo();
				break;
			case nn::yb::e::menu::STATE_TITLE:
				this->taskTitle();
				break;
			case nn::yb::e::menu::STATE_MAIN:
				this->taskMain();
				break;
			case nn::yb::e::menu::STATE_GAME:
				this->taskGame();
				break;
			case nn::yb::e::menu::STATE_CONFIG:
				this->taskCreateFlower();
				break;
		}
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::dispGame() {

	switch(this->gameState) {
		case nn::yb::e::menu::GAME_STATE_MAIN:
			{
				int layout_id_select_tbl[] = {
					LAYOUT_ID_MENU_RESUME_ENABLE,	//IMAGE	
					LAYOUT_ID_MENU_START_ENABLE,	//IMAGE	
					LAYOUT_ID_MENU_SCORE_ENABLE,	//IMAGE	
					LAYOUT_ID_MENU_CONFIG_ENABLE,	//IMAGE	
					LAYOUT_ID_MENU_EXIT_ENABLE,	//IMAGE	
				};
				int layout_id_not_select_tbl[] = {
					LAYOUT_ID_MENU_RESUME_DISABLE,	//IMAGE	
					LAYOUT_ID_MENU_START_DISABLE,	//IMAGE	
					LAYOUT_ID_MENU_SCORE_DISABLE,	//IMAGE	
					LAYOUT_ID_MENU_CONFIG_DISABLE,	//IMAGE	
					LAYOUT_ID_MENU_EXIT_DISABLE,	//IMAGE	
				};
				int index_start = 1;
				if(this->saveData->breakData.breakFlag) {
					index_start = 0;
				}
				for( int i = index_start; i < nn::yb::e::menu::CURSOR_Y_GAME_MAX; i++) {

					if(i == this->cursorYGame) {
						disp_commonObjSimple( layout_id_select_tbl[i]);
						disp_commonObjSimpleShift( LAYOUT_ID_MENU_MAIN_ICON_CURSOR_LEAF, layout_id_select_tbl[i]);
					}
					else {
						disp_commonObjSimple( layout_id_not_select_tbl[i]);
					}

				}
			}
			break;
		case nn::yb::e::menu::GAME_STATE_STAGE_MODE_SELECT:
			{
				int layout_id_select_tbl[nn::yb::e::menu::STAGE_MODE_MAX] = {
					LAYOUT_ID_MENU_CHAOS_ENABLE,		// nn::yb::e::menu::STAGE_MODE_CHAOS
					LAYOUT_ID_MENU_STANDARD_ENABLE,		// nn::yb::e::menu::STAGE_MODE_STANDARD
				};
				int layout_id_not_select_tbl[nn::yb::e::menu::STAGE_MODE_MAX] = {
					LAYOUT_ID_MENU_CHAOS_DISABLE,		// nn::yb::e::menu::STAGE_MODE_CHAOS
					LAYOUT_ID_MENU_STANDARD_DISABLE,	// nn::yb::e::menu::STAGE_MODE_STANDARD
				};
				for( int i = 0; i < nn::yb::e::menu::STAGE_MODE_MAX; i++) {

					if(i == this->saveData->gameStageModeSelect) {
						disp_commonObjSimple( layout_id_select_tbl[i]);
						disp_commonObjSimpleShift( LAYOUT_ID_MENU_MAIN_ICON_CURSOR_LEAF, layout_id_select_tbl[i]);
					}
					else {
						disp_commonObjSimple( layout_id_not_select_tbl[i]);
					}

				}
			}
			break;
		case nn::yb::e::menu::GAME_STATE_STAGE_SELECT:
			{
				// オフセット
				int layout_id_offset[nn::yb::e::menu::STAGE_MODE_MAX] = {
					LAYOUT_ID_MENU_CHAOS_OFFSET,		// nn::yb::e::menu::STAGE_MODE_CHAOS
					LAYOUT_ID_MENU_STANDARD_OFFSET,		// nn::yb::e::menu::STAGE_MODE_STANDARD
				};
				// ステージモード
				int layout_id_stage_mode[nn::yb::e::menu::STAGE_MODE_MAX] = {
					LAYOUT_ID_MENU_STAGE_SELECT_CHAOS,		// nn::yb::e::menu::STAGE_MODE_CHAOS
					LAYOUT_ID_MENU_STAGE_SELECT_STANDARD,	// nn::yb::e::menu::STAGE_MODE_STANDARD
				};
				disp_commonObjSimpleShift( layout_id_stage_mode[this->saveData->gameStageModeSelect],
					layout_id_offset[this->saveData->gameStageModeSelect]
				);

				// ステージセレクト
				int stage_open[nn::yb::e::menu::STAGE_MODE_MAX] = {
					this->saveData->stageOpen,				// nn::yb::e::menu::STAGE_MODE_CHAOS
					this->saveData->stageOpenStandard,		// nn::yb::e::menu::STAGE_MODE_STANDARD
				};
				LAYOUT * layout_offset = disp_commonGetLayoutMain3d(
					layout_id_offset[this->saveData->gameStageModeSelect]
				);
				for( int i = 0; i < stage_open[this->saveData->gameStageModeSelect]; i++) {

					int layout_id_stage = LAYOUT_ID_MENU_STAGE_1 + i;
					LAYOUT * layout_stage = disp_commonGetLayoutMain3d(
						layout_id_stage
					);
					int obj_id[nn::yb::e::menu::STAGE_MODE_MAX] = {
						OBJ_ID_MENU_STAGE_ICON_CHAOS_0 + i,	// nn::yb::e::menu::STAGE_MODE_CHAOS
						OBJ_ID_MENU_STAGE_ICON_0 + i,		// nn::yb::e::menu::STAGE_MODE_STANDARD
					};
					int x = layout_stage->x + layout_stage->w / 2 + layout_offset->x;
					int y = layout_stage->y + layout_stage->h / 2 + layout_offset->y;
					if(i == this->saveData->gameStageSelect[this->saveData->gameStageModeSelect]) {
						GXRgb color = RGB_WHITE;
						if(this->stageStartFlag) {
							color = GX_RGB( 31, 24, 0);
						}
						disp_commonObjCenter( obj_id[this->saveData->gameStageModeSelect],
							x, y, 0, 0, layout_stage->z - 1, 0,
							FALSE, FALSE, color, ALPHA_DEFAULT
						);
					}
					else {
						disp_commonObjCenter( obj_id[this->saveData->gameStageModeSelect],
							x, y, layout_stage->w, layout_stage->h, layout_stage->z, layout_stage->rotate,
							FALSE, FALSE, GX_RGB( 16, 16, 16), ALPHA_DEFAULT
						);
					}

				}
				// ステージとステージ間の線
				for( int i = 1; i < stage_open[this->saveData->gameStageModeSelect]; i++) {

					LAYOUT * layout_stage_line_bg = disp_commonGetLayoutMain3d( LAYOUT_ID_MENU_STAGE_LINE_BG);
					LAYOUT * layout_stage_line = disp_commonGetLayoutMain3d( LAYOUT_ID_MENU_STAGE_LINE);
					::disp_commonMain3dBoard(
						layout_stage_line_bg->x + layout_stage_line_bg->w * (i - 1) + layout_offset->x,
						layout_stage_line_bg->y + layout_offset->y,
						layout_stage_line_bg->z, layout_stage_line_bg->w, layout_stage_line_bg->h,
						RGB_BLACK, ALPHA_DEFAULT
					);
					disp_commonMain3dBoard(
						layout_stage_line->x + layout_stage_line->w * (i - 1) + layout_offset->x,
						layout_stage_line->y + layout_offset->y,
						layout_stage_line->z, layout_stage_line->w, layout_stage_line->h,
						RGB_WHITE, ALPHA_DEFAULT
					);

				}

			}
			break;
		case nn::yb::e::menu::GAME_STATE_SCORE:
			{
				this->dispGameScore();
			}
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::dispGameScore() {

	disp_commonObjSimple( LAYOUT_ID_MENU_SCORE_WINDOW_STAGE);
	disp_commonObjSimple( LAYOUT_ID_MENU_SCORE_WINDOW_TOTALBEST);
	disp_commonObjSimple( LAYOUT_ID_MENU_SCORE_WINDOW_HOLEBEST);
	LAYOUT * layout_text_stage = ::disp_commonGetLayoutMain3d( LAYOUT_ID_MENU_SCORE_TEXT_STAGE);
	LAYOUT * layout_text_totalbest = ::disp_commonGetLayoutMain3d( LAYOUT_ID_MENU_SCORE_TEXT_TOTALBEST);
	LAYOUT * layout_text_holebest = ::disp_commonGetLayoutMain3d( LAYOUT_ID_MENU_SCORE_TEXT_HOLEBEST);

	int line_cnt = 0;
	GXRgb color = RGB_BLACK;

	// ステージモード
	fontPutsType( layout_text_stage->x, layout_text_stage->y + line_cnt * layout_text_stage->h,
		nn::yb::e::global::FONT_TYPE_SIZE_18, color,
		gTextScoreStageMode[this->cursorXScore]
	);
	line_cnt += 2;

	// ステージ選択メニュー
	for( int i = 0; i < nn::yb::e::global::STAGE_N; i++) {

		color = RGB_BLACK;
		if(i == this->cursorYScore) {
			color = RGB_RED;
		}
		fontPrintfType( layout_text_stage->x, layout_text_stage->y + line_cnt * layout_text_stage->h,
			nn::yb::e::global::FONT_TYPE_SIZE_18, color, "ステージ%02d", i + 1
		);
		line_cnt++;

	}

	// 戻る
	color = RGB_BLACK;
	if(this->cursorYScore == nn::yb::e::menu::CURSOR_Y_SCORE_QUIT) {
		color = RGB_RED;
	}
	fontPrintfType( layout_text_stage->x, layout_text_stage->y + line_cnt * layout_text_stage->h,
		nn::yb::e::global::FONT_TYPE_SIZE_18, color, "戻る"
	);
	line_cnt++;

	if(this->cursorYScore != nn::yb::e::menu::CURSOR_Y_SCORE_QUIT) {
		// そのステージのトータルベスト
		nn::yb::main::SCORE * score[nn::yb::e::menu::STAGE_MODE_MAX] = {
			&this->saveData->score,				// nn::yb::e::menu::STAGE_MODE_CHAOS
			&this->saveData->scoreStandard,		// nn::yb::e::menu::STAGE_MODE_STANDARD
		};

		nn::yb::main::SCORE_STAGE * best = &score[this->cursorXScore]->bestScore[this->cursorYScore];
		int disp_total_best = best->total;
		if(disp_total_best == 0) {
			disp_total_best = 99;
		}
		color = RGB_BLACK;
		fontPrintfType( layout_text_totalbest->x, layout_text_totalbest->y + 0 * layout_text_totalbest->h,
			nn::yb::e::global::FONT_TYPE_SIZE_18, color, "トータルベスト：%2d", disp_total_best
		);
		// トータルベストの各ホールスコア
		for( int i = 0; i < 8; i++) {

			int disp_score = best->score[i];
			if(disp_score == 0) {
				disp_score = 99;
			}
			color = RGB_BLACK;
			fontPrintfType( layout_text_totalbest->x, layout_text_totalbest->y + (1 + i) * layout_text_totalbest->h,
				nn::yb::e::global::FONT_TYPE_SIZE_18, color, "ホール%2d：%2d", i + 1, disp_score
			);

		}
		
		// 各ホールのベストスコア
		color = RGB_BLACK;
		fontPrintfType( layout_text_holebest->x, layout_text_holebest->y + 0 * layout_text_holebest->h,
			nn::yb::e::global::FONT_TYPE_SIZE_18, color, "ホール別ベスト"
		);
		for( int i = 0; i < 8; i++) {

			int disp_hole = best->hole[i];
			if(disp_hole == 0) {
				disp_hole = 99;
			}
			color = RGB_BLACK;
			fontPrintfType( layout_text_holebest->x, layout_text_holebest->y + (1 + i) * layout_text_holebest->h,
				nn::yb::e::global::FONT_TYPE_SIZE_18, color, "ホール%2d：%2d", i + 1, disp_hole
			);

		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::disp() {
	
	switch(this->state) {
		case nn::yb::e::menu::STATE_LOAD:
			{
				nn::yb::e::ctrl::LOADING_SCREEN_PARAMS params;
				int bar_w = 8 * 15 + 1 * (15 - 1);
				params.text_x = 640 - bar_w - 20 - 1;
				params.text_y = 440 - 21;
				params.text_size = nn::yb::e::global::FONT_TYPE_SIZE_18;
				params.text_color = (GX_RGB( 31, 31, 31));

				const char* cText = "NOW LOADING...";
				int length = ::strlen(cText) ;
				params.text = new char[length + 1];
				::strcpy( params.text, cText ); 

				params.bar_x = 640 - bar_w - 20;
				params.bar_y = 440;
				params.bar_size = 8;
				params.bar_color = RGB_WHITE;

				params.window_x = 640 - bar_w - 20 - 1;
				params.window_y = 440 - 1;
				params.window_w = bar_w + 2;
				params.window_h = 8 + 2;
				params.window_color = RGB_BLACK;
				params.window_a = 31;

				this->ctrl->dispLoadingScreen( this->frameCnt,	params);
				if (params.text != NULL)
				{
					delete[] params.text;
					params.text = NULL;
				}
			}
			break;
		case nn::yb::e::menu::STATE_LOGO:
			{
				switch(this->logoState) {
					case nn::yb::e::menu::LOGO_STATE_WARNING:
						{
							for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( gTextWarning); i++) {

								fontPutsType( 20,  20 + 20 * i, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_WHITE, gTextWarning[i]);

							}
						}
						break;
					case nn::yb::e::menu::LOGO_STATE_MAIN:
						{
							disp_commonObjSimple( LAYOUT_ID_TITLE_AGRIPPA);
							disp_commonObjSimple( LAYOUT_ID_TITLE_N2NSOFT);
						}
						break;
				}
			}
			break;
		case nn::yb::e::menu::STATE_TITLE:
			{
				disp_commonObjSimple( LAYOUT_ID_TITLE_BG);
				if(this->titleState == nn::yb::e::menu::TITLE_STATE_MAIN) {
					if(this->titleFrameCnt % 60 <= 30) {
						int x = 250; int y = 444;
						fontPutsType( x - 1, y - 1, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, "Press Enter Key");
						fontPutsType( x - 1, y + 1, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, "Press Enter Key");
						fontPutsType( x + 1, y - 1, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, "Press Enter Key");
						fontPutsType( x + 1, y + 1, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, "Press Enter Key");
						fontPutsType( x, y, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_WHITE, "Press Enter Key");
					}
				}
			}
			break;
		case nn::yb::e::menu::STATE_MAIN:
			{
				for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE(gTextTblMainMenu); i++) {

					GXRgb color = RGB_WHITE;
					if(i == this->saveData->menuStartIndex) {
						color = RGB_RED;
					}
					fontPutsType( 20, 20 + i * 20, nn::yb::e::global::FONT_TYPE_SIZE_18, color, gTextTblMainMenu[i]);

				}
			}
			break;
		case nn::yb::e::menu::STATE_GAME:
			this->dispGame();
			break;
	}

	if(this->state == nn::yb::e::menu::STATE_MAIN || this->state == nn::yb::e::menu::STATE_GAME ||
		this->state == nn::yb::e::menu::STATE_CONFIG
	) {
		disp_commonObjSimple( LAYOUT_ID_MENU_BG);
	}
	
	if(this->state == nn::yb::e::menu::STATE_MAIN || 
		(this->state == nn::yb::e::menu::STATE_GAME && 
			(
				this->gameState == nn::yb::e::menu::GAME_STATE_MAIN ||
				this->gameState == nn::yb::e::menu::GAME_STATE_STAGE_MODE_SELECT ||
				this->gameState == nn::yb::e::menu::GAME_STATE_STAGE_SELECT
			)
		)
	) {
		if(this->sanaeInCompFlag) {
			// メッセージボックス
			LAYOUT * layout_msg_box = disp_commonGetLayoutMain3d( LAYOUT_ID_TITLE_MSG_BOX);
			nn::yb::common::yb_commonDispMsgBox( layout_msg_box->x, layout_msg_box->y,
				layout_msg_box->z, layout_msg_box->w, layout_msg_box->h,
				OBJ_ID_TITLE_MSG_BOX_0, RGB_WHITE, 24
			);
			disp_commonObjSimpleColor( LAYOUT_ID_TITLE_MSG_BOX_BALLOON, RGB_WHITE, 24);	// 吹き出し

			LAYOUT * layout_msg_box_text = disp_commonGetLayoutMain3d( LAYOUT_ID_TITLE_MSG_BOX_TEXT);

			char ** disp_text;
			int disp_text_n;
			switch(this->gameState) {
				case nn::yb::e::menu::GAME_STATE_MAIN:
				default:
					disp_text = gTextGuideGameMain;
					disp_text_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideGameMain);
					break;
				case nn::yb::e::menu::GAME_STATE_STAGE_MODE_SELECT:
					{
						switch(this->saveData->gameStageModeSelect) {
							case nn::yb::e::menu::STAGE_MODE_CHAOS:
							default:
								disp_text = gTextGuideGameModeSelectChaos;
								disp_text_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideGameModeSelectChaos);
								break;
							case nn::yb::e::menu::STAGE_MODE_STANDARD:
								disp_text = gTextGuideGameModeSelectStandard;
								disp_text_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideGameModeSelectStandard);
								break;
						}
					}
					break;
				case nn::yb::e::menu::GAME_STATE_STAGE_SELECT:
					disp_text = gTextGuideGameMain;
					disp_text_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideGameMain);
					break;
			}
			for( int i = 0; i < disp_text_n; i++) {

				fontPutsType( layout_msg_box_text->x, layout_msg_box_text->y + layout_msg_box_text->h * i,
					nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, disp_text[i]
				);

			}
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::startStageFromScratch() {

	if(this->rootFlower != NULL) {
		this->rootFlower->taskDelete();
		this->rootFlower = NULL;
	}
	nn::yb::main::CStage * stage = new nn::yb::main::CStage( this->ctrl);
	stage->master = this->master;
	stage->ctrl = this->ctrl;
	stage->saveData = this->saveData;
	stage->editFlag = false;
	stage->dataHole = NULL;
	stage->stageNo = this->saveData->gameStageSelect[this->saveData->gameStageModeSelect] + 1;
	stage->lp = 5;
	stage->setHp( 4);
	stage->start();
	this->stageStartFlag = false;
	this->state = nn::yb::e::menu::STATE_NONE;
	// 最初からリスタート→タイトルに戻る→連打でホールタスクが2個以上作られる不具合対応
	/*
	this->waitCnt--;
	*/

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::returnGameStateMainCb( void * menuArg) {

	nn::yb::main::CMenu * menu = (nn::yb::main::CMenu *)menuArg;

	menu->returnGameStateMain();

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::returnGameStateMain() {

	// コンフィグから戻る時なので今の所不要
	//nn::yb::common::yb_commonPlaySeq( BGM_MENU);
	this->state = nn::yb::e::menu::STATE_GAME;
	this->gameState =  nn::yb::e::menu::GAME_STATE_MAIN;

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::returnTitle() {

	nn::yb::common::yb_commonPlaySeq( BGM_OP);
#if defined(_DEBUG) == FALSE && defined(__EDIT__) == FALSE
	if(this->animeTitleTextTask == NULL) {
		this->animeTitleTextTask = animationExec( NULL, ANIMATION_ID_TITLE_DX, 320, 240, -128, ANIMATION_FLAGS_LAST_WAIT, nn::yb::main::CMenu::nextCb, this);
	}
	this->titleFrameCnt = 0;
	this->titleState = nn::yb::e::menu::TITLE_STATE_TEXT_IN;
	this->state = nn::yb::e::menu::STATE_TITLE;
#else // _DEBUG
	this->state = nn::yb::e::menu::STATE_MAIN;
#endif // _DEBUG
	this->gameState = nn::yb::e::menu::GAME_STATE_MAIN;

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::returnStageSelect() {
	
	initFlower();
	nn::yb::common::yb_commonPlaySeq( BGM_MENU);
	this->state = nn::yb::e::menu::STATE_GAME;
	this->gameState = nn::yb::e::menu::GAME_STATE_STAGE_SELECT;
	if(this->animeSanaeTask == NULL) {
		this->animeSanaeTask = animationExec( NULL, ANIMATION_ID_SANAE_CUTIN, 320, 240, -128,
			ANIMATION_FLAGS_LAST_WAIT, nn::yb::main::CMenu::endSanaeCutinCb, this
		);
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::goGameMain() {

	this->initFlower();
	nn::yb::common::yb_commonPlaySeq( BGM_MENU);

	if(this->animeTitleTextTask != NULL) {
		TASK * anime_task = (TASK *)this->animeTitleTextTask;
		::taskDelete( anime_task);
		this->animeTitleTextTask = NULL;
	}
	
	if(this->animeSanaeTask == NULL) {
		this->animeSanaeTask = animationExec( NULL, ANIMATION_ID_SANAE_CUTIN, 320, 240, -128,
			ANIMATION_FLAGS_LAST_WAIT, nn::yb::main::CMenu::endSanaeCutinCb, this
		);
	}

	this->state = nn::yb::e::menu::STATE_GAME;
	this->gameState = nn::yb::e::menu::GAME_STATE_MAIN;
	
	this->cursorYGame = 0;
	if(!this->saveData->breakData.breakFlag) {
		this->cursorYGame = 1;
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::goLogo() {

#if defined(_DEBUG) == FALSE && defined(__EDIT__) == FALSE
	// BGM
	nn::yb::common::yb_commonPlaySeq( BGM_LOGO);

	this->logoFrameCnt = 0;
	
	drawMainAllBrightness( -16);
	direct_commonCreateTaskBlackFadeInAllSimple( NULL, nn::yb::main::CMenu::decWaitCntCb, this);
	this->waitCnt++;
	this->state = nn::yb::e::menu::STATE_LOGO;
#else // _DEBUG
	this->state = nn::yb::e::menu::STATE_MAIN;
#endif // _DEBUG

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::nextLogo() {

	this->logoFrameCnt = 0;
	
	drawMainAllBrightness( -16);
	direct_commonCreateTaskBlackFadeInAllSimple( NULL, nn::yb::main::CMenu::decWaitCntCb, this);
	this->waitCnt++;

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::decWaitCntCb(void * menuArg) {

	nn::yb::main::CMenu * menu = (nn::yb::main::CMenu *)menuArg;

	menu->decWaitCnt();

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::decWaitCnt() {

	this->waitCnt--;

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::nextCb(void * menuArg) {

	nn::yb::main::CMenu * menu = (nn::yb::main::CMenu *)menuArg;

	menu->next();

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::nextLoad() {

	if(this->loadState == nn::yb::e::menu::LOAD_STATE_IMAGE) {
		sound_nnsLoadGroupUseThread( GROUP_ALL, nn::yb::main::CMenu::nextCb, this);
		this->loadState = nn::yb::e::menu::LOAD_STATE_SE;
	}
	else if(this->loadState == nn::yb::e::menu::LOAD_STATE_SE) {
		this->loadBgmCnt = GROUP_ALL_BGM_START;

		if(this->loadBgmCnt < GROUP_ALL_BGM_N) {
			sound_nnsLoadSeqUseThread( this->loadBgmCnt, nn::yb::main::CMenu::nextCb, this);
		}
		else {
			sound_nnsHeapSaveState();
			this->goLogo();
		}
		this->loadBgmCnt++;

		this->loadState = nn::yb::e::menu::LOAD_STATE_BGM;
	}
	else if(this->loadState == nn::yb::e::menu::LOAD_STATE_BGM) {
		if(this->loadBgmCnt < GROUP_ALL_BGM_N) {
			sound_nnsLoadSeqUseThread( this->loadBgmCnt, nn::yb::main::CMenu::nextCb, this);
		}
		else {
			sound_nnsHeapSaveState();
			this->goLogo();
		}
		this->loadBgmCnt++;
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::next() {

	if(this->state == nn::yb::e::menu::STATE_LOAD) {
		this->nextLoad();
	}
	else if(this->state == nn::yb::e::menu::STATE_LOGO) {
		this->decWaitCnt();
		if(this->logoState == nn::yb::e::menu::LOGO_STATE_MAIN) {
			this->goTitle();
		}
		else {
			this->logoState = nn::yb::e::menu::LOGO_STATE_MAIN;
			this->nextLogo();
		}
	}
	else if(this->state == nn::yb::e::menu::STATE_TITLE) {
		if(this->titleState == nn::yb::e::menu::TITLE_STATE_FADE_IN) {
			if(this->animeTitleTextTask == NULL) {
				this->animeTitleTextTask = animationExec( NULL, ANIMATION_ID_TITLE_DX, 320, 240, -128, ANIMATION_FLAGS_LAST_WAIT, nn::yb::main::CMenu::nextCb, this);
			}
			this->titleState = nn::yb::e::menu::TITLE_STATE_TEXT_IN;
		}
		else if(this->titleState == nn::yb::e::menu::TITLE_STATE_TEXT_IN) {
			this->titleState = nn::yb::e::menu::TITLE_STATE_MAIN;
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::goTitle() {
	
	nn::yb::common::yb_commonPlaySeq( BGM_OP);
	direct_commonCreateTaskBlackFadeInAllSimple( NULL, nn::yb::main::CMenu::nextCb, this);
	this->titleState = nn::yb::e::menu::TITLE_STATE_FADE_IN;
	this->titleFrameCnt = 0;
	this->state = nn::yb::e::menu::STATE_TITLE;

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::cutoutSanae( void (* cb)( void *), void * cbArg) {

	if(this->animeSanaeTask != NULL) {
		TASK * anime_task = (TASK *)this->animeSanaeTask;
		ANIMATION * anime = (ANIMATION *)anime_task->work;
		anime->lastWaitFlag = false;
		anime->waitFlag = false;
		anime->lastWaitExecFlag = false;
		anime->vec = -anime->vec;
		anime->endCallBackFunc = cb;
		anime->endCallBackFuncArg = cbArg;
		this->animeSanaeTask = NULL;

		this->stageStartFlag = true;
		this->sanaeInCompFlag = false;
		this->waitCnt++;
	}
	else {
		if(cb != NULL) {
			(*cb)( cbArg);
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::startStageFromScratchCb( void * menuArg) {

	nn::yb::main::CMenu * menu = (nn::yb::main::CMenu *)menuArg;

	menu->startStageFromScratch();

	// 最初からリスタート→タイトルに戻る→連打でホールタスクが2個以上作られる不具合対応
	menu->waitCnt--;

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::startStageFromBreakCb(void * menuArg) {

	nn::yb::main::CMenu * menu = (nn::yb::main::CMenu *)menuArg;

	menu->startStageFromBreak();

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::startStageFromBreak() {

	if(this->rootFlower != NULL) {
		this->rootFlower->taskDelete();
		this->rootFlower = NULL;
	}
	this->saveData->breakData.breakFlag = true;
	nn::yb::main::CStage * stage = new nn::yb::main::CStage( this->ctrl);
	stage->master = this->master;
	stage->ctrl = this->ctrl;
	stage->saveData = this->saveData;
	stage->editFlag = false;
	stage->dataHole = NULL;
	stage->resumeGame();
	stage->start();
	this->stageStartFlag = false;
	this->state = nn::yb::e::menu::STATE_NONE;
	this->waitCnt--;

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::endSanaeCutinCb(void *menuArg) {

	nn::yb::main::CMenu * menu = (nn::yb::main::CMenu *)menuArg;

	menu->endSanaeCutin();

}
//---------------------------------------------------------------------------
void nn::yb::main::CMenu::endSanaeCutin() {

	this->sanaeInCompFlag = true;

}
//---------------------------------------------------------------------------