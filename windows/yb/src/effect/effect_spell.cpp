#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nn_common.h"

#include "disp_common.h"
#include "sin_cos.h"

////#include "control.h"
////#include "my.h"
////#include "dungeon.h"

#include "obj_main_3d_data.h"

#include "effect\\effect_ctrl.h"
#include "effect\\effect_spell.h"

#include "common\\LunaAnimation.h"

#include "sound_nns.h"
#include "sound_data.sadl"

#include "musofuin.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SPELL::EFFECT_SPELL() 
: m_eState(0)
, m_uiAnimeCounter(0)
, m_uiAnimeCounterLimit(30)
{

	::nn_commonLog(1,  "EFFECT_SPELL CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::EFFECT_SPELL));

	this->camera_offset_x = 0;
	this->camera_offset_y = 0;

	this->m_taskCutIn = NULL;
	this->m_pBlackEffect = NULL;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SPELL::EFFECT_SPELL( TASK_SAVE * parent, nn::yb::main::EFFECT_SPELL * effectEffectArg, int frame, int darkenFrame) 
: ::TASK_SAVE( parent) 
, totalFrame(frame)//デフォルト値
, m_eState(0)
, m_uiAnimeCounter(0)
, m_uiAnimeCounterLimit(30)
{
	::nn_commonLog(1,  "EFFECT_SPELL START\n");

	
	const float cfFpsSpeedScale = nn::yb::main::EFFECT_MUSOFUIN::SCALE_FPS_MUSOFUIN;//FPS変更による速度調整

	this->ctrl = effectEffectArg->ctrl;
	this->cnt = 0;
	this->frameCnt = 0;

	this->ballCnt = 0;
	this->cardCnt = 0;

	this->m_taskCutIn = NULL;
	this->m_pBlackEffect = NULL;

	
	this->camera_offset_x = effectEffectArg->camera_offset_x;
	this->camera_offset_y = effectEffectArg->camera_offset_y;

	////this->cnt++;
	this->ctrl->push_event( nn::yb::main::EFFECT_SPELL::call_back, this, NULL);
	this->m_pBlackEffect = nn::yb::main::effect_fadein_out(this, OBJ_ID_DUN_SPELL_BG_BLACK_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		darkenFrame, 180 + 513   ,DISP_COMMON_BLEND_MODE_ALPHA ,enum_effect::EFFECT_ANIME_ADD , 
		30,
		320, 320,
		240, 240,
		1024, 1024,
		512, 512,
		0, 0,
		0, 24,
		darkenFrame - (30 + 15),
		320, 320,
		240, 240,
		1024, 1024,
		512, 512,
		0, 0,
		24, 24,
		15,
		320, 320,
		240, 240,
		1024, 1024,
		512, 512,
		0, 0,
		24, 0,
		this->ctrl);//画面暗転
	this->cnt++;

	//衝撃波の生成
	////nn::yb::main::effect_out_ex(parent, OBJ_ID_DUN_SPELL_FLASH_0, camera_offset_x, camera_offset_y, 1, 0, 0, 32, 150.0f, 32, 150.0f, 0, 0.0f, 31, -0.5f, DISP_COMMON_BLEND_MODE_ADD, ctrlArg, 120);//拡大(第一波)
	//this->ctrl->push_event( nn::yb::main::EFFECT_SPELL::call_back, this, NULL);
	nn::yb::main::effect_out_ex(parent, OBJ_ID_DUN_SPELL_FLASH_0, camera_offset_x, camera_offset_y, 1, 0, 0, 32, 10.0f, 32, 10.0f, 0, 0.0f, 31, -0.3f, DISP_COMMON_BLEND_MODE_ADD, NULL, (int)(120 * cfFpsSpeedScale));//拡大(第二波)
	//this->cnt++;

	this->ctrl->push_event( nn::yb::main::EFFECT_SPELL::call_back, this, NULL);
	nn::yb::main::effect_out_ex(parent, OBJ_ID_DUN_SPELL_SONIC_0, camera_offset_x, camera_offset_y, 1, 0, 0, 512, -5.0f, 512, -5.0f, 0, 0.0f, 0, 1.0f, DISP_COMMON_BLEND_MODE_ADD, this->ctrl, (int)(45 * cfFpsSpeedScale));//収縮(第一波)
	this->cnt++;

	this->ctrl->push_event( nn::yb::main::EFFECT_SPELL::call_back, this, NULL);
	nn::yb::main::effect_start_end_total(parent, OBJ_ID_DUN_SPELL_FLASH_ELLIPSE_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		(int)(30 * cfFpsSpeedScale),
		camera_offset_x, camera_offset_x, 10,
		camera_offset_y, camera_offset_y, 10,
		256,
		32, 1024, (int)(30 * cfFpsSpeedScale),
		32, 1024, (int)(30 * cfFpsSpeedScale),
		0, 0, (int)(10 * cfFpsSpeedScale),
		31, 0, (int)(30 * cfFpsSpeedScale),
		DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_ADD, RGB_WHITE, this->ctrl);//拡大(第二波)
	this->cnt++;


	//this->ctrl->push_event( nn::yb::main::EFFECT_SPELL::call_back, this, NULL);
	nn::yb::main::effect_first_flash(this, camera_offset_x, camera_offset_y, 0, NULL);
	//this->cnt++;

	this->ctrl->push_event( nn::yb::main::EFFECT_SPELL::call_back, this, NULL);
	nn::yb::main::effect_fadein_out(this, OBJ_ID_DUN_SPELL_BGCIRCLE_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		this->totalFrame, 230,DISP_COMMON_BLEND_MODE_ADD , nn::yb::enum_effect::EFFECT_ANIME_FLAT , 
		20,
		camera_offset_x, camera_offset_x,
		camera_offset_y, camera_offset_y,
		0, 512,
		0, 512,
		0, 360,
		0, 31,
		darkenFrame - 20,
		camera_offset_x, camera_offset_x,
		camera_offset_y, camera_offset_y,
		512, 64,
		512, 64,
		0, -360,
		31, 0,
		this->totalFrame - (darkenFrame),
		camera_offset_x, camera_offset_x,
		camera_offset_y, camera_offset_y,
		64, 0,
		64, 0,
		0, -360,
		0, 0,
		this->ctrl);//背景魔方陣(指定フレーム終了後に消去: スペル中ずっと表示)
	this->cnt++;

	this->ctrl->push_event( nn::yb::main::EFFECT_SPELL::call_back, this, NULL);
	nn::yb::main::effect_loop_fadein_out(this, OBJ_ID_DUN_SPELL_BELT_TEXT_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		(int)(70 * cfFpsSpeedScale), 160, DISP_COMMON_BLEND_MODE_ALPHA , nn::yb::enum_effect::EFFECT_ANIME_FLAT, 
		20, 0,
		15,
		camera_offset_x, camera_offset_x,
		camera_offset_y + 200, camera_offset_y + 180,
		1024, 1024,
		64, 64,
		0, 0,
		0, 24,
		30,
		camera_offset_x, camera_offset_x,
		camera_offset_y + 180, camera_offset_y + 140,
		1024, 1024,
		64, 64,
		0, 0,
		24, 24,
		25,
		camera_offset_x, camera_offset_x,
		camera_offset_y + 140, camera_offset_y + 120 ,
		1024, 1024,
		64, 64,
		0, 0,
		24, 0,
		this->ctrl);//横方向スペル開始ループ文字エフェクト(指定フレーム終了後に消去)
	this->cnt++;

	this->ctrl->push_event( nn::yb::main::EFFECT_SPELL::call_back, this, NULL);
	nn::yb::main::effect_loop_fadein_out(this, OBJ_ID_DUN_SPELL_BELD_TEXT_V_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		(int)(70 * cfFpsSpeedScale), 160,DISP_COMMON_BLEND_MODE_ALPHA , nn::yb::enum_effect::EFFECT_ANIME_FLAT, 
		0, 20,
		15,
		camera_offset_x + 300, camera_offset_x + 280,
		camera_offset_y, camera_offset_y,
		64, 64,
		1024, 1024,
		0, 0,
		0, 24,
		30,
		 camera_offset_x + 280,  camera_offset_x + 240,
		camera_offset_y, camera_offset_y,
		64, 64,
		1024, 1024,
		0, 0,
		24, 24,
		25,
		camera_offset_x + 240, camera_offset_x + 220,
		camera_offset_y, camera_offset_y,
		64, 64,
		1024, 1024,
		0, 0,
		24, 0,
		this->ctrl);//縦方向スペル開始ループ文字エフェクト(指定フレーム終了後に消去)
	this->cnt++;

	//スペカ開始SE
	sound_nnsPlaySe( SE_SPELLSTART);
	
	//別のスペカを中断した場合に備え、スペカ起動時に3Dエフェクトを中断する
	this->ctrl->stopEffect();
	this->ctrl->unloadEffect();//unload 3D effect
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SPELL::~EFFECT_SPELL() {

	::nn_commonLog(1,  "EFFECT_SPELL END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_SPELL::task() {

	this->disp();
	this->frameCnt++;

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_SPELL::disp() {

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_SPELL::call_back( void * effectEffectArg, void * dummy) {

	nn::yb::main::EFFECT_SPELL * effect = (nn::yb::main::EFFECT_SPELL *)effectEffectArg;

	effect->cnt--;
	if(effect->cnt == 0) {
		effect->ctrl->pop_event();
		effect->taskDelete();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_SPELL::initialize( void ) {
	
	this->m_eState = (99);
	this->m_uiAnimeCounter = (0);
	this->m_uiAnimeCounterLimit = (30);

	this->cnt = 999;
	this->frameCnt = 0;

	this->ballCnt = 0;
	this->cardCnt = 0;
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_SPELL::cutin_cb( void * effectArg) {

	nn::yb::main::EFFECT_SPELL * effect = (nn::yb::main::EFFECT_SPELL *)effectArg;

	effect->m_taskCutIn = NULL;//
}
//---------------------------------------------------------------------------