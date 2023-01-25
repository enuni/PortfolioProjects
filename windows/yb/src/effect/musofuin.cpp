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

#include "musofuin.h"
#include "bit.h"
#include "animation.h"
#include "animation_data.h"

#include "sound_nns.h"
#include "sound_data.sadl"

const int nn::yb::main::EFFECT_MUSOFUIN::FPS_MUSOFUIN = 60;
const float	nn::yb::main::EFFECT_MUSOFUIN::SCALE_FPS_MUSOFUIN = (float)FPS_MUSOFUIN / 30.0f;	//FPS換算

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MUSOFUIN::EFFECT_MUSOFUIN()
: nn::yb::main::EFFECT_SPELL() 
, m_bTargetHitEffect(false)
, spellEffectMusoCb(NULL)
, ball(NULL)
{

	::nn_commonLog(1,  "EFFECT_MUSOFUIN CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::EFFECT_MUSOFUIN));

	
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MUSOFUIN::EFFECT_MUSOFUIN( TASK_SAVE * parent, nn::yb::main::EFFECT_MUSOFUIN * effectMUSOFUINArg) 
: nn::yb::main::EFFECT_SPELL( parent, effectMUSOFUINArg, (int)(170 * SCALE_FPS_MUSOFUIN), (int)(170 * SCALE_FPS_MUSOFUIN))
{
	::nn_commonLog(1,  "EFFECT_MUSOFUIN START\n");

	//090208えぬに
	this->dstN = effectMUSOFUINArg->dstN;
	memcpy( this->dstLayX, effectMUSOFUINArg->dstLayX, sizeof( this->dstLayX));
	memcpy( this->dstLayY, effectMUSOFUINArg->dstLayY, sizeof( this->dstLayY));
	this->bitCnt = effectMUSOFUINArg->bitCnt;

	this->ctrl->push_event( nn::yb::main::EFFECT_MUSOFUIN::call_back, this, NULL);
	nn::yb::main::effect_fadein_out(this, OBJ_ID_DUN_SPELL_NAME_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		this->totalFrame, 160, DISP_COMMON_BLEND_MODE_ALPHA , nn::yb::enum_effect::EFFECT_ANIME_ADD , 
		(int)(20 * SCALE_FPS_MUSOFUIN),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y + 140,
		1024, 256,
		128, 32,
		0, 0,
		0, 31,
		(int)(15 * SCALE_FPS_MUSOFUIN),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 31,
		(int)((this->totalFrame - (20 + 15)) * SCALE_FPS_MUSOFUIN),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y - 210, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 0,
		this->ctrl);//スペル名称文字(指定フレーム終了後に消去: スペル中ずっと表示)
	this->cnt++;

	//発射するx, y座標
	this->m_iStartX = effectMUSOFUINArg->m_iStartX;
	this->m_iStartY = effectMUSOFUINArg->m_iStartY;

	this->m_bTargetHitEffect = effectMUSOFUINArg->m_bTargetHitEffect;

	this->spellEffectMusoCb = effectMUSOFUINArg->spellEffectMusoCb;
	this->ball = effectMUSOFUINArg->ball;

	this->bitRoot = new TASK_SAVE( this);
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MUSOFUIN::~EFFECT_MUSOFUIN() {

	::nn_commonLog(1,  "EFFECT_MUSOFUIN END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MUSOFUIN::task() {
	nn::yb::main::EFFECT_SPELL::task();

	//スキップ処理
    ::KEY_WORK * key = &this->ball->master->keyWork;
	if (key->trigger & nn::common::e::PAD_WIN_BUTTON_A) 
	{
		//決定ボタンが押されていたら
		if (this->m_eState >= 0 
			&& this->m_eState <= 2)
		{
			::nn_commonLog(1, "spell effect skip: this->m_eState = %d\n", this->m_eState);
			this->m_eState = 99;
			this->m_uiAnimeCounter = 0;

			//ゆっくりを表示させる
			this->ball->setDispState( nn::yb::e::ball::DISP_STATE_DEFAULT);

			if (this->m_taskCutIn != NULL)//カットインが表示されている場合、停止
			{ 
				::animationHide( (ANIMATION *)this->m_taskCutIn->work );
				::animationEnd( (ANIMATION *)this->m_taskCutIn->work );
				this->m_taskCutIn = NULL;
			}

			//ビットを削除
			this->bitRoot->taskDelete();
			
			//暗転エフェクトの削除
			if (this->m_pBlackEffect != NULL )
			{
				//既に起動されているエフェクトがある場合タスクを消去
				if (this->isLive(this->m_pBlackEffect))
				{
					this->m_pBlackEffect->taskDelete();
				}
				this->m_pBlackEffect = NULL;
			}
			if (this->ball->m_pMusoReimu != NULL)
			{
				this->ball->m_pMusoReimu->taskDelete();
			}

		}
	}

	this->disp();

	this->frameCnt++;
	bool end_flag = false;

	this->m_uiAnimeCounter++;
	switch ( this->m_eState )
	{
	case 0://カットイン
		{
			if (
					this->m_uiAnimeCounter >= 30 + 15 ///指定フレーム経過した場合、次の状態へ
				)
			{
				this->m_taskCutIn = animationExec( NULL, ANIMATION_ID_SPELL_CHAR_3, 320, 240, 100 + 513, FALSE, nn::yb::main::EFFECT_SPELL::cutin_cb, this);
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}

		}
		break;

	case 1://カットイン終了待ち
		{
			if (
					this->m_uiAnimeCounter >= 120//指定フレーム経過した場合、次の状態へ
				)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

		}
		break;
	case 2://弾生成
		{
			if (
				this->m_uiAnimeCounter >= 25 * SCALE_FPS_MUSOFUIN + this->dstN * 10 + 60 ///指定フレーム経過した場合、次の状態へ
				)
			{


				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
				//090208えぬに
				const int ciStartFrame = static_cast<int>(25 * SCALE_FPS_MUSOFUIN);
				if(this->bitCnt < this->dstN) {

					////if(this->frameCnt == ciStartFrame + this->bitCnt * 10) {
					if(this->m_uiAnimeCounter == ciStartFrame + this->bitCnt * 10) {
					
						sound_nnsPlaySe( SE_BITSTART);//ビット発射SE

						int iColor = (this->bitCnt) % 5;//光球の色
						nn::yb::main::effect_bit2(this->bitRoot, 120, m_iStartX, m_iStartY, this->dstLayX[this->bitCnt], this->dstLayY[this->bitCnt], 128, iColor,
							this->ball, this->bitCnt, this->spellEffectMusoCb, this->ctrl, this->m_bTargetHitEffect);//
						this->bitCnt++;
					}
				}
			/*
				const int ciStartFrame = 15;
				if (this->frameCnt == ciStartFrame + 10)
				{
					nn::yb::main::effect_bit(this, 120, 320, 240, 100, 100, 128, 0,
						this->ctrl);//
				}
				else if (this->frameCnt == ciStartFrame + 20)
				{
					nn::yb::main::effect_bit(this, 120, 320, 240, 500, 100, 128, 1,
						this->ctrl);//
				}
				else if (this->frameCnt == ciStartFrame + 30)
				{
					nn::yb::main::effect_bit(this, 120, 320, 240, 500, 400, 128, 2,
						this->ctrl);//
				}
				else if (this->frameCnt == ciStartFrame + 40)
				{
					nn::yb::main::effect_bit(this, 120, 320, 240, 100, 400, 128, 3,
						this->ctrl);//
				}
				else if (this->frameCnt == ciStartFrame + 50)
				{
					nn::yb::main::effect_bit(this, 120, 320, 240, 100, 300, 128, 4,
						this->ctrl);//
				}
			*/

		}
		break;
	case 3://終了処理
		{
			if (
					this->m_uiAnimeCounter >= 10//指定フレーム経過した場合、次の状態へ
				)
			{
				this->ball->m_pCurrSpellEffect = NULL;

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

		}
		break;
	case 99://キャンセル用終了処理
		{
			if (
					this->m_uiAnimeCounter >= 20//
				)
			{
				nn::yb::main::EFFECT_MUSOFUIN::call_back( this, 0);
				
				this->ball->spellEffectMusoCb( this->ball, 4 );
				this->ball->spellEffectMusoReimuCb( this->ball );
				this->ball->spellEffectEndDummyCb( this->ball, NULL );
				////this->initialize();

				this->ctrl->clear_cb();// clear callback
				this->taskDelete();// 
				this->ball->m_pCurrSpellEffect = NULL;

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

		}
		break;
	}



}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MUSOFUIN::disp() {

	nn::yb::main::EFFECT_SPELL::disp();

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MUSOFUIN::call_back( void * effectMUSOFUINArg, void * dummy) {

	nn::yb::main::EFFECT_MUSOFUIN * effect = (nn::yb::main::EFFECT_MUSOFUIN *)effectMUSOFUINArg;

	effect->cnt--;
	if(effect->cnt == 0) {
		effect->ctrl->pop_event();
		effect->taskDelete();
	}
}

//---------------------------------------------------------------------------

