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
#include "disp_common.h"
#include "vram_ctrl.h"
#include "image_ctrl.h"
#include "font.h"
#include "sound_nns.h"
#include "task_save.h"

// 各ゲームのヘッダ
#include "ctrl.h"
#include "effect/effect_ctrl.h"
#include "yb_common.h"

#include "save_data.h"
#include "enum.h"

#if 0
#include "animation\\LunaAnimation.h"
#endif

//---------------------------------------------------------------------------

//生成されたファイルの他に ソースセーフから
//./data/font_data.NFTR
//./etc以下
//./src/image_ctrl以下
//./src/backup.cpp
//./src/backup.h
//./src/direct_common.cpp
//./src/direct_common.h
//./src/disp_common.cpp
//./src/disp_common.h
//./src/draw.cpp
//./src/draw.h
//./src/font.cpp
//./src/font.h
//./src/fs.cpp
//./src/fs.h
//./src/heap.cpp
//./src/heap.h
//./src/init.cpp
//./src/init.h
//./src/key_common.cpp
//./src/key_common.h
//./src/master.cpp
//./src/master.h
//./src/sleep.cpp
//./src/sleep.h
//./src/sound_nns.cpp
//./src/sound_nns.h
//./src/touch.cpp
//./src/touch.h
//が必要

static MASTER * sMaster;

//---------------------------------------------------------------------------

void main_loopMain( void) {
	
	initSystem();
	
	heapInit();
	
	heapDispFreeSize();
	
	//ファイルシステム初期化
	//FS_Init( 3);
	fsInitSimple();
	
	//heapInit()後	initMain()前
	sMaster = (MASTER *)heapAlloc( sizeof(MASTER));
	memset( sMaster, 0, sizeof( MASTER));
	//master->keyWork.mode = ::MASTER_MODE_PAD;
	sMaster->gameContinueFlag = TRUE;
	
	initMain();
	
	//initMain()後
#ifndef __CHILD__
	initCardSetThreadPriority( 20);
	
	//backupInit( initCardEject);
	//save_loadDispSaveSize();
#endif //__CHILD__
	
	sleepInit();
	
	touchInit();
	
	initRand();
	
	//タスク初期化
	taskInit();

	::fontSetTypeSimple( nn::yb::e::global::FONT_TYPE_SIZE_10, "MS ゴシック", 10);
	::fontSetTypeSimple( nn::yb::e::global::FONT_TYPE_SIZE_18, "MS ゴシック", 18);
	
#if 0
	//--------------------------------------------
	// アニメーションクラスの初期化
	//--------------------------------------------
	if ( !LunaAnimation::Initialize() )
	{
		return ;
	}
#endif

	int frame_cnt = 0;
	/*
	int old_mill_sec = 0;
	int fps = 0;
	int frame_cnt_array[100];
	memset( frame_cnt_array, 0, sizeof( frame_cnt_array));
	int frame_cnt_n = 0;
	int frame_cnt_i = 0;
	int frame_cnt_sum = 0;
	LONGLONG old_time = GetNowHiPerformanceCount();
	int fps_sample_n = 1;
	*/
	LONGLONG proc_time[60];
	memset( proc_time, 0, sizeof( proc_time));
	int proc_time_n = 0;
	int proc_time_i = 0;
	LONGLONG proc_time_sum = 0;
	LONGLONG old_time = GetNowHiPerformanceCount();
	int proc_time_sample_n = 60;

#if 0
	::CreateMaskScreen();
	int mask_handle = ::LoadMask( "mask.png");
	::DrawMask( 0, 0, mask_handle, DX_MASKTRANS_NONE);
#endif
	
	int config_key_tbl[MASTER_CONFIG_KEY_N] = {
		1, 2, 3, 4, 6, 5, 0, 8, 7, 0, 9, 10, 12, 11
	};

	memcpy( sMaster->keyWork.configKey, config_key_tbl, sizeof( config_key_tbl));

	// 上下左右 ABXY LR START の11ボタン
	// 12番目はESC
	//for( int i = 0; i < 14; i++)
	for( int i = 0; i < 11; i++)
	{

		sMaster->keyWork.configPad[i] = 1 + i;

	}
	sMaster->keyWork.mode = ::PAD_MODE_WIN;
	
#if 0
	// 各ゲームの処理
	nn::ti::base::CBase * base = new nn::ti::base::CBase( NULL);
	base->master = sMaster;
	base->start();
#endif

	TASK_SAVE::sRoot = new TASK_SAVE();

	nn::yb::main::CCtrl * ctrl = new nn::yb::main::CCtrl( NULL);
	ctrl->master = sMaster;
	ctrl->start();

	//effect ctrl
	////nn::yb::main::CEffectCtrl ctrl_arg;
	////ctrl_arg.master = sMaster;
	nn::yb::main::CEffectCtrl * effect_ctrl = new nn::yb::main::CEffectCtrl( NULL/*, &ctrl_arg*/);
	
	int draw_w = nn::yb::e::global::WINDOW_W;//ウィンドウモード
	int draw_h = nn::yb::e::global::WINDOW_H;
	if (ctrl->saveData->configData.dispSize == 1)//フルスクリーン
	{
		draw_w = ctrl->drawGlobalW;
		draw_h = ctrl->drawGlobalH;
	}
	effect_ctrl->initialize(draw_w, draw_h);//明示的に初期化(staticメンバ)
	effect_ctrl->start();


#ifdef _DEBUG
	int win_x;
	int win_y;
#endif //_DEBUG
	while(masterIsMainLoop( sMaster)) {
#ifdef _DEBUG
		GetWindowPosition( &win_x, &win_y);
#endif //_DEBUG
		
		touchTask( &sMaster->touchWork);
		masterKeyTask( &sMaster->keyWork);
		masterKeyboardTask( &sMaster->keyboardWork);
		if(sMaster->keyWork.trigger & nn::common::e::PAD_WIN_BUTTON_SLECT) {
			// ESCキー押下
#ifdef _DEBUG
			sMaster->gameContinueFlag = false;
#endif //_DEBUG
		}
		fontTask();
		fsTask();
		vram_ctrlTask();
		image_ctrlTask();
		sound_nnsTask();
		backupTask();
		sleepTask();
		
		taskExec();
		::TASK_SAVE::taskExec();

		// 以下を例に、ゲーム内で動的にdisp3DEffectsをコールするタイミングを毎フレーム設定
		// するようにすれば、3Dエフェクト(全体)を任意のz値で描画する事が出来ようになります
#if 0
		// これまで相当
		::disp_commonCallback( nn::yb::main::CEffectCtrl::disp3DEffectsCb, effect_ctrl, -300);
#else
		// 地面とBGの間に描画する
		::disp_commonCallback( nn::yb::main::CEffectCtrl::disp3DEffectsCb, effect_ctrl, effect_ctrl->m_effect3D_z);
#endif

		ClearDrawScreen();
		FillMaskScreen(1);
		
		disp_commonShowSub2dObj();
		disp_commonShowMain3dObj();
		fontShow();
		
		fontSubBg1Reflect();
		fontMainBg1Reflect();
		
		drawMain2dAllDCFlushRange();
		drawSub2dAllDCFlushRange();
		
		drawTask();

		/*
		// 暫定ライン表示
		for ( int i = 0; i < stage->lineN; i++) {
			
			DrawLine( stage->line[i].s.x - ball->x  +320, stage->line[i].s.y - ball->y + 240,
				stage->line[i].e.x - ball->x  +320, stage->line[i].e.y - ball->y  +240, 0x00FF00, 5);

		}
		*/
		
		frame_cnt++;

		//effect_ctrl->disp3DEffects();// draw 3D effects (tentative)
		ctrl->fullscreenTask();

		drawSwapBuffers();
		drawWaitVBlankIntr();

#if 1
		//fps
		/*
		int mill_sec = GetNowCount();
		if(mill_sec - old_mill_sec >= 1000) {
			fps = frame_cnt;
			frame_cnt = 0;
			old_mill_sec = mill_sec;
			clsDx();
			printfDx( "%02dfps", fps);
		}
		//LAYOUT * Layout = ::disp_commonGetLayoutMain3d( LAYOUT_ID_INTERFACE_DUN2_HP_NUM);
		//::disp_commonMain3dObjNumber( fps, Layout->obj_id, 596, 0, 0, -22);
		*/
		/*
		LONGLONG new_time = GetNowHiPerformanceCount();
		if(new_time - old_time >= 1000000) {
			frame_cnt_sum -= frame_cnt_array[frame_cnt_i];
			frame_cnt_array[frame_cnt_i] = frame_cnt;
			frame_cnt_sum += frame_cnt;
			frame_cnt = 0;
			frame_cnt_i = (frame_cnt_i + 1) % fps_sample_n;
			if(frame_cnt_n < fps_sample_n) {
				frame_cnt_n++;
			}
			old_time = new_time;
			double fps = 0.00;
			if(frame_cnt_n >= 1) {
				fps = (double)frame_cnt_sum / (double)frame_cnt_n;
			}
			clsDx();
			printfDx( "%02.02lf fps", fps);
		}
		*/
		LONGLONG new_time = GetNowHiPerformanceCount();
		proc_time_sum -= proc_time[proc_time_i];
		proc_time[proc_time_i] = new_time - old_time;
		proc_time_sum += proc_time[proc_time_i];
		proc_time_i = (proc_time_i + 1) % proc_time_sample_n;
		if(proc_time_n < proc_time_sample_n) {
			proc_time_n++;
		}
		int proc_time_ave = (int)(proc_time_sum / proc_time_sample_n);
		old_time = new_time;
		if(frame_cnt >= 60) {
			frame_cnt = 0;
			double disp_fps = 1000000.0 / (double)proc_time_ave;
			if(disp_fps > 60.00) {
				disp_fps = 60.00;
			}
			ctrl->dispFps = disp_fps;
			/*
			clsDx();
			printfDx( "%04d fps", disp_fps);
			*/

		}

#endif
		
		drawMain3dReset();
		drawMain2dAllTransReset();
		drawSub2dAllTransReset();
		
	}
	
#if 0
	//--------------------------------------------
	// アニメーションクラス解放
	//--------------------------------------------
	LunaAnimation::Finalize();
#endif
	effect_ctrl->uninitialize();//明示的に終了処理(staticメンバ)


	
#ifdef _DEBUG
	FILE * fp = fopen( "init.txt", "w");
	if(fp) {
		fprintf( fp, "%d\n%d\n", win_x, win_y);
		fclose( fp);
	}
#endif //_DEBUG

}

//---------------------------------------------------------------------------

MASTER * nn::yb::common::getMaster() {

	return sMaster;

}

//---------------------------------------------------------------------------
