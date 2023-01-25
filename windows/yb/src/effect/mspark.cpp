#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nn_common.h"

#include "disp_common.h"
#include "sin_cos.h"

#include "obj_main_3d_data.h"

#include "effect\\effect_ctrl.h"
#include "effect\\effect_spell.h"

#include "common\\LunaAnimation.h"

#include "mspark.h"
#include "bit.h"
#include "ball.h"
#include "animation.h"
#include "animation_data.h"
#include "effect\\effect_star.h"
#include "sound_nns.h"
#include "sound_data.sadl"

#include "yb_common.h"

const int nn::yb::main::EFFECT_MSPARK::FPS_MSPARK = 60;
const float	nn::yb::main::EFFECT_MSPARK::SCALE_FPS_MSPARK = (float)FPS_MSPARK / 30.0f;	//FPS換算

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MSPARK::EFFECT_MSPARK()
: nn::yb::main::EFFECT_SPELL() {

	::nn_commonLog(1,  "EFFECT_MSPARK CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::EFFECT_MSPARK));

	this->iStarCt = 0;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MSPARK::EFFECT_MSPARK( TASK_SAVE * parent, nn::yb::main::EFFECT_MSPARK * effectMSPARKArg) 
	: nn::yb::main::EFFECT_SPELL( parent, effectMSPARKArg, (int)(210 * SCALE_FPS_MSPARK), (int)(210 * SCALE_FPS_MSPARK))
{
	::nn_commonLog(1,  "EFFECT_MSPARK START\n");

	this->shadowX = effectMSPARKArg->shadowX;
	this->shadowY = effectMSPARKArg->shadowY;
	this->srcX = effectMSPARKArg->srcX;
	this->srcY = effectMSPARKArg->srcY;
	this->shadowSpeed = effectMSPARKArg->shadowSpeed;
	this->z = effectMSPARKArg->z;
	this->angle = effectMSPARKArg->angle;
	this->targetX = effectMSPARKArg->targetX;
	this->targetY = effectMSPARKArg->targetY;
	this->m_pBall = effectMSPARKArg->m_pBall;
	this->vec = effectMSPARKArg->vec;

	this->ctrl->push_event( nn::yb::main::EFFECT_MSPARK::call_back, this, NULL);
	nn::yb::main::effect_fadein_out(this, OBJ_ID_SPELLNAME_SPELLNAME_5, 
		nn::yb::enum_effect::SLIDE_MODE_FRAME,
		this->totalFrame, 160, DISP_COMMON_BLEND_MODE_ALPHA , nn::yb::enum_effect::EFFECT_ANIME_ADD , 
		(int)(20 * SCALE_FPS_MSPARK),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y + 140,
		1024, 256,
		128, 32,
		0, 0,
		0, 31,
		(int)(15 * SCALE_FPS_MSPARK),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 31,
		(int)((this->totalFrame - (20 + 15)) * SCALE_FPS_MSPARK),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y - 210, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 0,
		this->ctrl);//スペル名称文字(指定フレーム終了後に消去: スペル中ずっと表示)
	this->cnt++;

	this->iStarCt = 0;

}
//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MSPARK::~EFFECT_MSPARK() {

	::nn_commonLog(1,  "EFFECT_MSPARK END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MSPARK::task() {
	nn::yb::main::EFFECT_SPELL::task();

	//スキップ処理
    ::KEY_WORK * key = &this->m_pBall-> master->keyWork;
	if (key->trigger & nn::common::e::PAD_WIN_BUTTON_A) 
	{
		//決定ボタンが押されていたら
		if (this->m_eState >= 0 
			&& this->m_eState <= 4)
		{
			::nn_commonLog(1, "spell effect skip: this->m_eState = %d\n", this->m_eState);
			this->m_eState = 5;
			this->m_uiAnimeCounter = 0;

			this->shadowX = this->dx;
			this->shadowY = this->dy;

			
			this->m_pBall->setDispState( nn::yb::e::ball::DISP_STATE_BURN, 150);
			this->m_pBall->ballForm = nn::yb::e::ball::BALL_FORM_BURN;


			if (this->m_taskCutIn != NULL)//カットインが表示されている場合、停止
			{ 
				::animationEnd( (ANIMATION *)this->m_taskCutIn->work );
				::animationHide( (ANIMATION *)this->m_taskCutIn->work );
				this->m_taskCutIn = NULL;
			}
		}
	}

	static int old_state = 0;
	if(old_state != this->m_eState) {
		::nn_commonLog(1, "this->m_eState = %d\n", this->m_eState);
		old_state = this->m_eState;
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
				animationChangeObjId( (ANIMATION *)this->m_taskCutIn->work, ANIMATION_CHANGE_OBJ_ID_TYPE_OBJ_ID, OBJ_ID_SPELL_CHAR_REIMU_0, OBJ_ID_SPELL_CHAR_MARISA_0);

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;

	case 1://カットイン終了待ち
		{
			if (
					this->m_uiAnimeCounter >= 60//指定フレーム経過した場合、次の状態へ
				)
			{
				//影移動準備
				nn::common::graphics2d::initStart2GollDDA( this->srcX, this->srcY,
					this->shadowX, this->shadowY, this->vx, this->vy,
					this->dx, this->dy, this->diff
				);
				sound_nnsPlaySe( SE_LEVELUP);//影移動 SE

				
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}

		}
		break;
	case 2://影移動
		{	
			end_flag = nn::common::graphics2d::moveStart2GollDDA(
				this->shadowX, this->shadowY, this->srcX, this->srcY, this->vx, this->vy,
				this->dx, this->dy, this->diff, this->shadowSpeed
			);
			//////☆エフェクトの表示
			////static int iStarFrame = 0;

			////if (
			////	this->iStarCt % 60 == 15 //一定フレーム間隔
			////	 ////this->shadowX //一定距離間隔
			////	)
			////{

			////	::nn_commonLog(1,  "EFFECT_MSPARK : shadowX=%d, shadowY=%d\n", shadowX, shadowY);

			////	this->ctrl->push_event( nn::yb::main::EFFECT_MSPARK::call_back, this, NULL);
			////	nn::yb::main::effect_star(this,30, 
			////		this->shadowX, this->shadowY,						
			////		this->shadowX, this->shadowY,
			////		128, 0,
			////		1,
			////		0.1f,
			////		this->ctrl);//
			////	this->cnt++;
			////	this->cnt++;

			////	////this->iStarCt = 0;
			////}
			////this->iStarCt++;

			if(end_flag) {
				this->iStarCt = 0;//カウンタをリセット
				sound_nnsPlaySe( SE_MSPARK);//ビット発射SE


				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;
	case 3://発射
		{
			const unsigned int ciTotalFrame = static_cast<unsigned int>(20 * SCALE_FPS_MSPARK);
			if (
				this->m_uiAnimeCounter >= ciTotalFrame ///指定フレーム経過した場合、次の状態へ
				)
			{
				this->ctrl->push_event( nn::yb::main::EFFECT_MSPARK::call_back, this, NULL);
				nn::yb::main::effect_quake(this, nn::yb::enum_effect::EFFECT_ANIME_SUB, ciTotalFrame * 2, 15, 20, this->ctrl);//画面振動(待機状態までずっと振動が続く)
				this->cnt++;
				sound_nnsPlaySe( SE_GROUND);//ビット着弾SE

				// enuni 110506
				this->m_pBall->setDispState( nn::yb::e::ball::DISP_STATE_BURN, 150);;
				this->m_pBall->ballForm = nn::yb::e::ball::BALL_FORM_BURN;

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;
	case 4://待機
		{
			if (
				this->m_uiAnimeCounter >= 20 * SCALE_FPS_MSPARK ///指定フレーム経過した場合、次の状態へ
				)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;
	case 5://フェードアウト
		{
			if (
				this->m_uiAnimeCounter >= 40 * SCALE_FPS_MSPARK ///指定フレーム経過した場合、次の状態へ
				)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;
	case 6://終了処理
		{
			if (
					this->m_uiAnimeCounter >= 15 * SCALE_FPS_MSPARK//指定フレーム経過した場合、次の状態へ
				)
			{
				nn::yb::main::EFFECT_MSPARK::call_back( this, 0);
				
				this->m_pBall->spellEffectEnd();
				this->initialize();
				this->ctrl->clear_cb();// clear callback
				this->taskDelete();// 
				this->m_pBall->m_pCurrSpellEffect = NULL;

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

void nn::yb::main::EFFECT_MSPARK::disp() {

	nn::yb::main::EFFECT_SPELL::disp();

	const int ciWidthScale = 20;
	switch ( this->m_eState )
	{
	case 2://影移動
		{
			int obj_id_tbl[] = {
				OBJ_ID_GAME_CHAR_MARISA_0,	// nn::yb::e::mspark::VEC_UP
				OBJ_ID_GAME_CHAR_MARISA_1,	// nn::yb::e::mspark::VEC_RIGHT
				OBJ_ID_GAME_CHAR_MARISA_2,	// nn::yb::e::mspark::VEC_DOWN
				OBJ_ID_GAME_CHAR_MARISA_3,	// nn::yb::e::mspark::VEC_LEFT
			};
			disp_commonObjCenterColor( obj_id_tbl[this->vec], this->shadowX, this->shadowY - 64, this->z, RGB_WHITE, 32);
			disp_commonObjCenterColor( ::OBJ_ID_MAIN_SHADOW_0, this->shadowX, this->shadowY, this->z, RGB_WHITE, 32);
		}
		break;
	case 3://発射
		{
			int w = this->m_uiAnimeCounter * ciWidthScale;
			disp_commonObjDxCenterFull( OBJ_ID_MASTERSPARK_MASTERSPARK_0, this->targetX, this->targetY, this->z, 1000, w, ((360 - this->angle) + 180) % 360, FALSE, FALSE, RGB_WHITE, 32, DISP_COMMON_BLEND_MODE_ADD);

		}
		break;
	case 4://待機
		{
			int w = static_cast<int>((20 * SCALE_FPS_MSPARK) * ciWidthScale);
			disp_commonObjDxCenterFull( OBJ_ID_MASTERSPARK_MASTERSPARK_0, this->targetX, this->targetY, this->z, 1000, w, ((360 - this->angle) + 180) % 360, FALSE, FALSE, RGB_WHITE, 32, DISP_COMMON_BLEND_MODE_ADD);

			int wBase =  static_cast<int>(this->m_uiAnimeCounter * ciWidthScale * 0.5);
			disp_commonObjDxCenterFull( OBJ_ID_MASTERSPARK_BASE_MASTERSPARK_BASE_0, this->targetX, this->targetY, this->z, 1000, wBase, ((360 - this->angle) + 180) % 360, FALSE, FALSE, RGB_WHITE, 32, DISP_COMMON_BLEND_MODE_ADD);
		}
		break;
	case 5://フェードアウト
		{
			int w = static_cast<int>((20 * SCALE_FPS_MSPARK) * ciWidthScale);
			int alpha = static_cast<int>(31 * ((40 * SCALE_FPS_MSPARK) - this->m_uiAnimeCounter) / (40 * SCALE_FPS_MSPARK) );
			disp_commonObjDxCenterFull( OBJ_ID_MASTERSPARK_MASTERSPARK_0, this->targetX, this->targetY, this->z, 1000, w, ((360 - this->angle) + 180) % 360, FALSE, FALSE, RGB_WHITE, alpha, DISP_COMMON_BLEND_MODE_ADD);

			int wBase =  static_cast<int>((40 * SCALE_FPS_MSPARK - this->m_uiAnimeCounter) / (float)(40 * SCALE_FPS_MSPARK) * w * 0.5);
			int alphaBase = alpha ;
			disp_commonObjDxCenterFull( OBJ_ID_MASTERSPARK_BASE_MASTERSPARK_BASE_0, this->targetX, this->targetY, this->z, 1000, wBase, ((360 - this->angle) + 180) % 360, FALSE, FALSE, RGB_WHITE, alphaBase, DISP_COMMON_BLEND_MODE_ADD);

		}
		break;
	}
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MSPARK::call_back( void * effectMSPARKArg, void * dummy) {

	nn::yb::main::EFFECT_MSPARK * effect = (nn::yb::main::EFFECT_MSPARK *)effectMSPARKArg;

	effect->cnt--;
	if(effect->cnt == 0) {
		effect->ctrl->pop_event();
		effect->taskDelete();
	}

}

//---------------------------------------------------------------------------

