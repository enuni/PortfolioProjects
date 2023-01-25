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

// �e�Q�[���̃w�b�_
#include "ctrl.h"
#include "effect/effect_ctrl.h"
#include "yb_common.h"

#include "save_data.h"
#include "enum.h"

#if 0
#include "animation\\LunaAnimation.h"
#endif

//---------------------------------------------------------------------------

//�������ꂽ�t�@�C���̑��� �\�[�X�Z�[�t����
//./data/font_data.NFTR
//./etc�ȉ�
//./src/image_ctrl�ȉ�
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
//���K�v

static MASTER * sMaster;

//---------------------------------------------------------------------------

void main_loopMain( void) {
	
	initSystem();
	
	heapInit();
	
	heapDispFreeSize();
	
	//�t�@�C���V�X�e��������
	//FS_Init( 3);
	fsInitSimple();
	
	//heapInit()��	initMain()�O
	sMaster = (MASTER *)heapAlloc( sizeof(MASTER));
	memset( sMaster, 0, sizeof( MASTER));
	//master->keyWork.mode = ::MASTER_MODE_PAD;
	sMaster->gameContinueFlag = TRUE;
	
	initMain();
	
	//initMain()��
#ifndef __CHILD__
	initCardSetThreadPriority( 20);
	
	//backupInit( initCardEject);
	//save_loadDispSaveSize();
#endif //__CHILD__
	
	sleepInit();
	
	touchInit();
	
	initRand();
	
	//�^�X�N������
	taskInit();

	::fontSetTypeSimple( nn::yb::e::global::FONT_TYPE_SIZE_10, "MS �S�V�b�N", 10);
	::fontSetTypeSimple( nn::yb::e::global::FONT_TYPE_SIZE_18, "MS �S�V�b�N", 18);
	
#if 0
	//--------------------------------------------
	// �A�j���[�V�����N���X�̏�����
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

	// �㉺���E ABXY LR START ��11�{�^��
	// 12�Ԗڂ�ESC
	//for( int i = 0; i < 14; i++)
	for( int i = 0; i < 11; i++)
	{

		sMaster->keyWork.configPad[i] = 1 + i;

	}
	sMaster->keyWork.mode = ::PAD_MODE_WIN;
	
#if 0
	// �e�Q�[���̏���
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
	
	int draw_w = nn::yb::e::global::WINDOW_W;//�E�B���h�E���[�h
	int draw_h = nn::yb::e::global::WINDOW_H;
	if (ctrl->saveData->configData.dispSize == 1)//�t���X�N���[��
	{
		draw_w = ctrl->drawGlobalW;
		draw_h = ctrl->drawGlobalH;
	}
	effect_ctrl->initialize(draw_w, draw_h);//�����I�ɏ�����(static�����o)
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
			// ESC�L�[����
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

		// �ȉ����ɁA�Q�[�����œ��I��disp3DEffects���R�[������^�C�~���O�𖈃t���[���ݒ�
		// ����悤�ɂ���΁A3D�G�t�F�N�g(�S��)��C�ӂ�z�l�ŕ`�悷�鎖���o���悤�ɂȂ�܂�
#if 0
		// ����܂ő���
		::disp_commonCallback( nn::yb::main::CEffectCtrl::disp3DEffectsCb, effect_ctrl, -300);
#else
		// �n�ʂ�BG�̊Ԃɕ`�悷��
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
		// �b�胉�C���\��
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
	// �A�j���[�V�����N���X���
	//--------------------------------------------
	LunaAnimation::Finalize();
#endif
	effect_ctrl->uninitialize();//�����I�ɏI������(static�����o)


	
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
