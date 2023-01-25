#include <stdio.h>

#include "nn_common.h"

#include "init.h"
#include "master.h"
#include "heap.h"
#include "fs.h"
#include "backup.h"
#include "sleep.h"
#include "touch.h"
#include "task.h"
#include "draw.h"
#include "disp.h"
#include "vram_ctrl.h"
#include "image_ctrl.h"
#include "font.h"
#include "sound_nns.h"
#include "task_class.h"
#include "task_save.h"

#include "anim.h"
#include "font2.h"
#include "script.h"

// ToDo 各ゲームのヘッダ
#if 1
#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "anime_data.h"
#include "anime_data.inc"
#include "control.h"
#include "ts_common.h"
#include "font_FOT_Skip_Std_20.h"
#include "control.h"
#endif

//---------------------------------------------------------------------------
MASTER * master;
nn::dxlib::CScript * script;
nn::dxlib::CFont * font;
nn::dxlib::script::GlobalCommonVar * common_var;
//---------------------------------------------------------------------------

void main_loopMain( void) {

	nn::common::setLogKind( NN_COMMON_E_LOG_KIND_COMMON_SAVE |
		NN_TS_COMMON_E_LOG_KIND_CLASS_DUMP |
		NN_COMMON_E_LOG_KIND_COMMON_TASK |
		NN_TS_COMMON_E_LOG_KIND_XML);
	nn::common::logInit( nn::common::e::LOG_DST_INDEX_COMMON_SAVE_MANAGER, "save_system.log");
	
	initSystem();
	
	heapInit();
	
	heapDispFreeSize();
	
	//ファイルシステム初期化
	//FS_Init( 3);
	fsInitSimple();
	
	//heapInit()後	initMain()前
	master = (MASTER *)heapAlloc( sizeof(MASTER));
	memset( master, 0, sizeof( MASTER));
	master->gameContinueFlag = TRUE;
	
	initMain();
	
	//initMain()後
#ifndef __CHILD__
	initCardSetThreadPriority( 20);
	
	backupInit( initCardEject);
	//save_loadDispSaveSize();
#endif //__CHILD__
	
	sleepInit();
	
	touchInit();
	
	initRand();

	nn::dxlib::disp::setCameraArea( 854, 480);
	
	// ToDo 使用するタスクシステムのコメントアウトを解除
	//タスクシステム初期化
#if 0
	taskInit();
#endif
#if 1
	::TASK_SAVE::init();
#endif
#if 0
	TASK_CLASS::sRoot = new TASK_CLASS();
#endif
	
	// fps
	int frame_cnt = 0;
	int old_mill_sec = 0;
	int fps = 0;
	
	// キー・ボタン設定
	master->keyWork.mode = ::PAD_MODE_WIN;
	// キーボード設定
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_DOWN] = nn::common::e::PAD_INDEX_DOWN;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_LEFT] = nn::common::e::PAD_INDEX_LEFT;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_RIGHT] = nn::common::e::PAD_INDEX_RIGHT;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_UP] = nn::common::e::PAD_INDEX_UP;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_Z] = nn::common::e::PAD_INDEX_B;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_X] = nn::common::e::PAD_INDEX_A;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_C] = nn::common::e::PAD_INDEX_NONE;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_A] = nn::common::e::PAD_INDEX_Y;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_S] = nn::common::e::PAD_INDEX_X;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_D] = nn::common::e::PAD_INDEX_NONE;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_Q] = nn::common::e::PAD_INDEX_L1;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_W] = nn::common::e::PAD_INDEX_R1;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_ESC] = nn::common::e::PAD_INDEX_SELECT;
	master->keyWork.configKey[nn::common::e::KEYBOARD_ORG_INDEX_SPACE] = nn::common::e::PAD_INDEX_START;
	// ゲームパッド設定
	master->keyWork.configPad[nn::common::e::PAD_ORG_INDEX_DOWN] = nn::common::e::PAD_INDEX_DOWN;
	master->keyWork.configPad[nn::common::e::PAD_ORG_INDEX_LEFT] = nn::common::e::PAD_INDEX_LEFT;
	master->keyWork.configPad[nn::common::e::PAD_ORG_INDEX_RIGHT] = nn::common::e::PAD_INDEX_RIGHT;
	master->keyWork.configPad[nn::common::e::PAD_ORG_INDEX_UP] = nn::common::e::PAD_INDEX_UP;
	master->keyWork.configPad[nn::common::e::PAD_ORG_INDEX_1] = nn::common::e::PAD_INDEX_B;
	master->keyWork.configPad[nn::common::e::PAD_ORG_INDEX_2] = nn::common::e::PAD_INDEX_A;
	master->keyWork.configPad[nn::common::e::PAD_ORG_INDEX_3] = nn::common::e::PAD_INDEX_Y;
	master->keyWork.configPad[nn::common::e::PAD_ORG_INDEX_4] = nn::common::e::PAD_INDEX_X;
	master->keyWork.configPad[nn::common::e::PAD_ORG_INDEX_5] = nn::common::e::PAD_INDEX_L1;
	master->keyWork.configPad[nn::common::e::PAD_ORG_INDEX_6] = nn::common::e::PAD_INDEX_R1;
	master->keyWork.configPad[nn::common::e::PAD_ORG_INDEX_7] = nn::common::e::PAD_INDEX_START;
	// キーボード・ゲームパッド共通で、nn::common::e::PAD_～ PAD_DOWN等で取る
	
	nn::ts::common::setCallBackFunc();

	//int texture_id = image_ctrlLoadMain3dImagePaletteHeapVram( 0, "data\\graphics\\image\\stand.png");
	image_ctrlSetTextureData( 0, ::texture_main_3d_data, ::palette_3d_main_3d_data, TEXTURE_ID_MAIN_3D_OFFSET, TEXTURE_MAIN_3D_ID_MAX);
	image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_TEST);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_EFFECT_MASTER_SPARK);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_EFFECT_MASTER_SPARK_BASE);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_EFFECT_COMMON);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_BALL_BG);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_BATTLE);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_BALL_UP);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_STAGE_START);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_TITLE);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_BALL);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_BOSS);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_ENEMY);
	image_ctrlLoadMain3dImagePaletteHeapVram( ::TEXTURE_ID_ITEM);
	nn::dxlib::disp::setObjData( 0, ::obj_main_3d_data, OBJ_ID_MAIN_3D_OFFSET, OBJ_MAIN_3D_ID_MAX);
	nn::dxlib::disp::setLayoutData( 0, ::layout_main_3d_data, LAYOUT_ID_MAIN_3D_OFFSET, LAYOUT_MAIN_3D_ID_MAX);

	nn::dxlib::CAnim::arcInit( gAnimationArcInfo, ANIMATION_ARC_ID_MAX);
	nn::dxlib::CAnim::loadArc( ANIMATION_ARC_ID_COMMON);
	
	font = new nn::dxlib::CFont();
	strcpy( font->imageFileDirectory, "data\\graphics\\image\\font\\");
	font->init( &gFOT_Skip_Std_20_Common,
		gFOT_Skip_Std_20_ImageFileNames,
		gFOT_Skip_Std_20_Hankaku,
		gFOT_Skip_Std_20_Zenkaku,
		gFOT_Skip_Std_20_Kanji1,
		gFOT_Skip_Std_20_Kanji2_1,
		gFOT_Skip_Std_20_Kanji2_2
	);

	LAYOUT * layout_msg_text = nn::dxlib::disp::getLayout( LAYOUT_ID_TALK_MSG_TEXT);
	
	script = new nn::dxlib::CScript();
	script->font = font;
	script->master = master;
	script->msgParam.layoutText = *layout_msg_text;
	script->msgParam.layoutIdMsgBg = LAYOUT_ID_TALK_MSG_BG;
	script->msgParam.hSpace = -1;
	script->msgParam.vSpace = -14;
	script->msgParam.skip = true;
	script->msgParam.skipButtonBit = nn::common::e::PAD_B;
	script->msgParam.skipOnPressCnt = 30;
	script->msgParam.opeMode = nn::dxlib::e::script::OPE_MODE_TOUCH_PAD;
	script->selectParam.buttonLayoutId[0] = 0;
	script->selectParam.buttonLayoutId[1] = 0;
	script->selectParam.cursorLayoutId[0] = 0;
	script->selectParam.cursorLayoutId[1] = 0;
	script->selectParam.buttonBgLayoutId[0] = 0;
	script->selectParam.buttonBgLayoutId[1] = 0;
	script->selectParam.buttonActiveAnimeId[0] = -1;
	script->selectParam.buttonActiveAnimeId[1] = -1;
	script->selectParam.buttonIdleAnimeId[0] = -1;
	script->selectParam.buttonIdleAnimeId[1] = -1;
	script->selectParam.selectN = 2;
	script->selectParam.cursorAnimeIdDecide = 0;
	script->selectParam.cursorMode = nn::dxlib::e::script::SELECT_CURSOR_MODE_LEFT_RIGHT;
	script->selectParam.touchLayoutId[0] = 0;
	script->selectParam.touchLayoutId[1] = 0;
	strcpy( script->scriptDirectory, "data\\script\\");

	common_var = (nn::dxlib::script::GlobalCommonVar *)malloc( sizeof(nn::dxlib::script::GlobalCommonVar));
	memset( common_var, 0, sizeof( nn::dxlib::script::GlobalCommonVar));
	
	nn::ts::common::setDispMember();
#if 1
	// ToDo 各ゲームの初期化・各ゲームのルートタスクの生成
	nn::ts::common::CControl * ctrl = new nn::ts::common::CControl( NULL);
	ctrl->font = font;
	ctrl->start();
	ctrl->startTitle();
#endif

	int win_x;
	int win_y;
	SetFogEnable( FALSE);
	SetFogColor( 255, 255, 255);
	SetUseLighting(FALSE);
	SetTextureAddressModeUV(DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP);
	SetCameraPositionAndTarget_UpVecY( VGet( 0.0f, 100.0f, -100.0f), VGet(0.0f, 0.0f, 0.0f));

	while(masterIsMainLoop( master)) {
		GetWindowPosition( &win_x, &win_y);
		
		touchTask( &master->touchWork);
		masterKeyTask( &master->keyWork);
#ifdef _DEBUG
		if(master->keyWork.trigger & nn::common::e::PAD_SELECT) {
			// ESCキー押下
			master->gameContinueFlag = false;
		}
#endif //_DEBUG
		fontTask();
		fsTask();
		vram_ctrlTask();
		image_ctrlTask();
		sound_nnsTask();
		backupTask();
		sleepTask();
		
		//nn::dxlib::disp::objUv( texture_id, 0, 0, 400, 400, 0, 0, 0, false);
		//nn::dxlib::disp::obj( OBJ_ID_TEST_BALL_0, 0, 0, 0, false); 
		script->run();
		
		// ToDo 使用するタスクシステムのコメントアウトを解除
#if 0
		taskExec();
#endif
#if 0
		::TASK_CLASS::taskExec();
#endif
#if 1
		::TASK_SAVE::taskDispExec();
#endif
		if(ctrl->gm.dispSkipFlag) {
			nn::dxlib::disp::showEnd();
			continue;
		}

		nn::dxlib::disp::show();
		fontShow();
		
		fontSubBg1Reflect();
		fontMainBg1Reflect();
		
		drawMain2dAllDCFlushRange();
		drawSub2dAllDCFlushRange();
		
		drawTask();
		nn::dxlib::disp::task();

		drawSwapBuffers();
		drawWaitVBlankIntr();
		
#if 1
		//fps
		frame_cnt++;
		int mill_sec = GetNowCount();
		if(mill_sec - old_mill_sec >= 1000) {
			fps = frame_cnt;
			frame_cnt = 0;
			old_mill_sec = mill_sec;
		}
#endif
		
		drawMain3dReset();
		drawMain2dAllTransReset();
		drawSub2dAllTransReset();
		::ClearDrawScreen();
		clsDx();
		printfDx( "%02dfps\n", fps);
		
	}
	FILE * fp = fopen( "init.txt", "w");
	if(fp) {
		fprintf( fp, "%d\n%d\n", win_x, win_y);
		fclose( fp);
	}
	
	nn::common::logEnd( nn::common::e::LOG_DST_INDEX_COMMON_SAVE_MANAGER);

}

//---------------------------------------------------------------------------
