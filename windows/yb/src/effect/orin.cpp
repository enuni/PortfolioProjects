#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nn_common.h"

#include "disp_common.h"
#include "sin_cos.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"

#include "effect\\effect_ctrl.h"
#include "effect\\effect_spell.h"

#include "common\\LunaAnimation.h"

#include "orin.h"
#include "bit.h"
#include "ball.h"
#include "animation.h"
#include "animation_data.h"
#include "effect\\effect_star.h"
#include "sound_nns.h"
#include "sound_data.sadl"

#include "yb_common.h"

#include "camera.h"
#include "ctrl.h"
#include "hole_ctrl.h"

#include "enum.h"

const int nn::yb::main::EFFECT_ORIN::FPS_ORIN = 60;
const float	nn::yb::main::EFFECT_ORIN::SCALE_FPS_ORIN = (float)FPS_ORIN / 30.0f;	//FPS換算

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ORIN::EFFECT_ORIN()
: nn::yb::main::EFFECT_SPELL() {

	::nn_commonLog(1,  "EFFECT_ORIN CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::EFFECT_ORIN));

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ORIN::EFFECT_ORIN( TASK_SAVE * parent, nn::yb::main::EFFECT_ORIN * effectORINArg) 
: nn::yb::main::EFFECT_SPELL( parent, effectORINArg, (int)(400 * SCALE_FPS_ORIN), (int)(400 * SCALE_FPS_ORIN))
{
	::nn_commonLog(1,  "EFFECT_ORIN START\n");

	this->master = effectORINArg->master;

	this->dstX = effectORINArg->dstX;
	this->dstY = effectORINArg->dstY;
	this->srcX = effectORINArg->srcX;
	this->srcY = effectORINArg->srcY;
	this->z = effectORINArg->z;
	this->speed = effectORINArg->speed;
	this->dstX2 = effectORINArg->dstX2;
	this->dstY2 = effectORINArg->dstY2;

	this->vec = effectORINArg->vec;
	this->sendEventCb = effectORINArg->sendEventCb;
	this->sendEventCbArg = effectORINArg->sendEventCbArg;
	this->vec2 = effectORINArg->vec2;

	// スペル名
	this->ctrl->push_event( nn::yb::main::EFFECT_SPELL::call_back, this, NULL);
	nn::yb::main::effect_fadein_out(this, OBJ_ID_SPELLNAME_SPELLNAME_9, 
		nn::yb::enum_effect::SLIDE_MODE_FRAME,
		this->totalFrame, 160, DISP_COMMON_BLEND_MODE_ALPHA , nn::yb::enum_effect::EFFECT_ANIME_ADD , 
		(int)(20 * SCALE_FPS_ORIN),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y + 140,
		1024, 256,
		128, 32,
		0, 0,
		0, 31,
		(int)(15 * SCALE_FPS_ORIN),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 31,
		(int)((this->totalFrame - (20 + 15)) * SCALE_FPS_ORIN),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y - 210, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 0,
		this->ctrl);//スペル名称文字(指定フレーム終了後に消去: スペル中ずっと表示)
	this->cnt++;

	this->ctrl->stopEffect();

}
//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ORIN::~EFFECT_ORIN() {

	::nn_commonLog(1,  "EFFECT_ORIN END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_ORIN::task() {
	
	nn::yb::main::EFFECT_SPELL::task();
	this->disp();

	//スキップ処理
    ::KEY_WORK * key = &this->master->keyWork;
	if (key->trigger & nn::common::e::PAD_WIN_BUTTON_A) 
	{
		if (nn::yb::e::orin::STATE_CUTIN <= this->m_eState &&
			this->m_eState <= nn::yb::e::orin::STATE_SHOCK
			)
		{
			::nn_commonLog(999, "spell effect skip: m_eState = %d\n", this->m_eState);
			this->m_uiAnimeCounter = 0;
			this->m_eState = nn::yb::e::orin::STATE_SHOCK;
			this->srcX = this->dstX2;
			this->srcY = this->dstY2;

			sound_nnsResumeSeqDX( BGM_ORIN);

			if(this->sendEventCb != NULL) {
				this->sendEventCb( this->sendEventCbArg, (void *)nn::yb::e::orin::CB_STATE_SKIP, NULL, 0);
			}

			if (this->m_taskCutIn != NULL)//カットインが表示されている場合、停止
			{
				::animationEnd( (ANIMATION *)m_taskCutIn->work );
				::animationHide( (ANIMATION *)m_taskCutIn->work );
				m_taskCutIn = NULL;
			}
		}
	}

	this->frameCnt++;
	bool end_flag = false;

	this->m_uiAnimeCounter++;
	switch ( this->m_eState )
	{
	case nn::yb::e::orin::STATE_DARK_OUT:
		{
			if (m_uiAnimeCounter >= 30 + 15)
			{
				this->m_taskCutIn = animationExec( NULL, ANIMATION_ID_SPELL_CHAR_3, 320, 240, 100 + 513, FALSE, nn::yb::main::EFFECT_SPELL::cutin_cb, this);
				animationChangeObjId( (ANIMATION *)m_taskCutIn->work, ANIMATION_CHANGE_OBJ_ID_TYPE_OBJ_ID, OBJ_ID_SPELL_CHAR_REIMU_0, OBJ_ID_SPELL_CHAR_ORIN_0);

				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_CUTIN;
				break;
			}
		}
		break;
	case nn::yb::e::orin::STATE_CUTIN:
		{
			if (this->m_uiAnimeCounter >= 140)
			{
				nn::common::graphics2d::initStart2GollDDA( this->dstX, this->dstY,
					this->srcX, this->srcY, this->vx, this->vy,
					this->dx, this->dy, this->diff
				);
				sound_nnsPlaySe( SE_BITSTART);
				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_DASH;
			}
		}
		break;
	case nn::yb::e::orin::STATE_DASH:
		{
			end_flag = nn::common::graphics2d::moveStart2GollDDA(
				this->srcX, this->srcY, this->dstX, this->dstY, this->vx, this->vy,
				this->dx, this->dy, this->diff, this->speed
			);
			if(end_flag)
			{
				nn::common::graphics2d::initStart2GollDDA( this->dstX2, this->dstY2,
					this->srcX, this->srcY, this->vx, this->vy,
					this->dx, this->dy, this->diff
				);

				sound_nnsPauseSeqDX( BGM_ORIN);
				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_SLOW;
			}
		}
		break;
	case nn::yb::e::orin::STATE_SLOW:
		{
			if( this->frameCnt % 16 == 0 )
			{
				end_flag = nn::common::graphics2d::moveStart2GollDDA(
					this->srcX, this->srcY, this->dstX2, this->dstY2, this->vx, this->vy,
					this->dx, this->dy, this->diff, 1
				);
			}

			if (this->m_uiAnimeCounter >= 120)
			{
				sound_nnsResumeSeqDX( BGM_ORIN);
				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_BRAKE;
			}

		}
		break;
	case nn::yb::e::orin::STATE_BRAKE:
		{
			end_flag = nn::common::graphics2d::moveStart2GollDDA(
				this->srcX, this->srcY, this->dstX2, this->dstY2, this->vx, this->vy,
				this->dx, this->dy, this->diff, this->speed
			);
			if(end_flag)
			{
				if(this->sendEventCb != NULL) {
					this->sendEventCb( this->sendEventCbArg, (void *)nn::yb::e::orin::CB_STATE_CLASH,
						nn::yb::main::EFFECT_ORIN::recvEventCb, this
					);
				}
				sound_nnsPlaySe( SE_MSPARK);
				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_CLASH;
			}
		}
		break;
	case nn::yb::e::orin::STATE_CLASH:
		{
			if (this->m_uiAnimeCounter >= 20)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_STOP1;
			}
		}
		break;
	case nn::yb::e::orin::STATE_STOP1:
		{
			if (this->m_uiAnimeCounter >= 10)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_STOP2;
			}
		}
		break;
	case nn::yb::e::orin::STATE_STOP2:
		{
		}
		break;
	case nn::yb::e::orin::STATE_SHOCK_BEFORE:
		{
			if (this->m_uiAnimeCounter >= 5)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_SHOCK;
			}
		}
		break;
	case nn::yb::e::orin::STATE_SHOCK:
		{
			if (this->m_uiAnimeCounter >= 30)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_SHOCK_KICK1;
			}
		}
		break;
	case nn::yb::e::orin::STATE_SHOCK_KICK1:
		{
			if (this->m_uiAnimeCounter >= 10)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_SHOCK_KICK2;
			}
		}
		break;
	case nn::yb::e::orin::STATE_SHOCK_KICK2:
		{
			if (this->m_uiAnimeCounter >= 3)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_SHOCK_KICK3;
			}
		}
		break;
	case nn::yb::e::orin::STATE_SHOCK_KICK3:
		{
			if (this->m_uiAnimeCounter >= 3)
			{
				this->sendEventCb( this->sendEventCbArg, (void *)nn::yb::e::orin::CB_STATE_KICK,
					nn::yb::main::EFFECT_ORIN::recvEventCb, this
				);
				this->m_uiAnimeCounter = 0;
				this->m_eState = nn::yb::e::orin::STATE_SHOCK_KICK4;
			}
		}
		break;
	case nn::yb::e::orin::STATE_SHOCK_KICK4:
		{
			if (this->m_uiAnimeCounter >= 15)
			{
				this->m_uiAnimeCounter = 0;
				this->sendEventCb( this->sendEventCbArg, (void *)nn::yb::e::orin::CB_STATE_END,
					NULL, NULL
				);
				this->ctrl->clear_cb();
				this->taskDelete();
				if (this->sendEventCbArg != NULL)
				{
					nn::yb::main::CBall *ball = (nn::yb::main::CBall *)this->sendEventCbArg;
					ball->m_pCurrSpellEffect = NULL;
				}
			}
		}
		break;
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_ORIN::disp() {

	nn::yb::main::EFFECT_SPELL::disp();
	
	int layhout_id_orin_tbl[] = {
		LAYOUT_ID_ORIN_UP_ORIN,		// nn::yb::e::orin::VEC_UP
		LAYOUT_ID_ORIN_RIGHT_ORIN,	// nn::yb::e::orin::VEC_RIGHT
		LAYOUT_ID_ORIN_DOWN_ORIN,	// nn::yb::e::orin::VEC_DOWN
		LAYOUT_ID_ORIN_LEFT_ORIN,	// nn::yb::e::orin::VEC_LEFT
	};
	LAYOUT * layout_orin = disp_commonGetLayoutMain3d( layhout_id_orin_tbl[this->vec2]);

	int layhout_id_kasha_tbl[] = {
		LAYOUT_ID_ORIN_UP_KASHA,		// nn::yb::e::orin::VEC_UP
		LAYOUT_ID_ORIN_RIGHT_KASHA,	// nn::yb::e::orin::VEC_RIGHT
		LAYOUT_ID_ORIN_DOWN_KASHA,	// nn::yb::e::orin::VEC_DOWN
		LAYOUT_ID_ORIN_LEFT_KASHA,	// nn::yb::e::orin::VEC_LEFT
	};
	LAYOUT * layout_kasha =  disp_commonGetLayoutMain3d( layhout_id_kasha_tbl[this->vec2]);

	int layhout_id_yukkuri_tbl[] = {
		LAYOUT_ID_ORIN_UP_YUKKURI,		// nn::yb::e::orin::VEC_UP
		LAYOUT_ID_ORIN_RIGHT_YUKKURI,	// nn::yb::e::orin::VEC_RIGHT
		LAYOUT_ID_ORIN_DOWN_YUKKURI,	// nn::yb::e::orin::VEC_DOWN
		LAYOUT_ID_ORIN_LEFT_YUKKURI,	// nn::yb::e::orin::VEC_LEFT
	};
	LAYOUT * layout_kasha_yukkuri =  disp_commonGetLayoutMain3d( layhout_id_yukkuri_tbl[this->vec2]);

	switch ( this->m_eState )
	{
	case nn::yb::e::orin::STATE_DASH:
	case nn::yb::e::orin::STATE_SLOW:
	case nn::yb::e::orin::STATE_BRAKE:
	case nn::yb::e::orin::STATE_CLASH:
	case nn::yb::e::orin::STATE_STOP1:
	case nn::yb::e::orin::STATE_STOP2:
	case nn::yb::e::orin::STATE_SHOCK_BEFORE:
	case nn::yb::e::orin::STATE_SHOCK:
	case nn::yb::e::orin::STATE_SHOCK_KICK1:
	case nn::yb::e::orin::STATE_SHOCK_KICK2:
	case nn::yb::e::orin::STATE_SHOCK_KICK3:
		{
			int obj_id_shift = this->frameCnt / 16 % 2;
			switch ( this->m_eState )
			{
			case nn::yb::e::orin::STATE_SLOW:
				obj_id_shift = this->frameCnt / 48 % 2;
				break;
			case nn::yb::e::orin::STATE_BRAKE:
			case nn::yb::e::orin::STATE_CLASH:
				obj_id_shift = 2;
				break;
			case nn::yb::e::orin::STATE_STOP1:
				obj_id_shift = 3;
				break;
			case nn::yb::e::orin::STATE_STOP2:
			case nn::yb::e::orin::STATE_SHOCK_BEFORE:
				obj_id_shift = 4;
				break;
			case nn::yb::e::orin::STATE_SHOCK:
				obj_id_shift = 5;
				break;
			case nn::yb::e::orin::STATE_SHOCK_KICK1:
			case nn::yb::e::orin::STATE_SHOCK_KICK2:
			case nn::yb::e::orin::STATE_SHOCK_KICK3:
			case nn::yb::e::orin::STATE_SHOCK_KICK4:
				obj_id_shift = 6 +  this->m_eState - nn::yb::e::orin::STATE_SHOCK_KICK1;
				break;
			}
			disp_commonObjCenterColor( layout_orin->obj_id + obj_id_shift,
				this->srcX + layout_orin->x + layout_orin->w / 2,
				this->srcY + layout_orin->y + layout_orin->h / 2, this->z + layout_orin->z,
				RGB_WHITE, 31
			);
			
			disp_commonObjCenterColor( layout_kasha->obj_id,
				this->srcX + layout_kasha->x + layout_kasha->w / 2,
				this->srcY + layout_kasha->y + layout_kasha->h / 2, this->z + layout_orin->z,
				RGB_WHITE, 31
			);

			switch( this->m_eState )
			{
			case nn::yb::e::orin::STATE_SHOCK_BEFORE:
			case nn::yb::e::orin::STATE_SHOCK:
			case nn::yb::e::orin::STATE_SHOCK_KICK1:
			case nn::yb::e::orin::STATE_SHOCK_KICK2:
			case nn::yb::e::orin::STATE_SHOCK_KICK3:
				disp_commonObjCenterColor( layout_kasha_yukkuri->obj_id,
					this->srcX + layout_kasha_yukkuri->x + layout_kasha_yukkuri->w / 2,
					this->srcY + layout_kasha_yukkuri->y + layout_kasha_yukkuri->h / 2, this->z + layout_kasha_yukkuri->z,
					RGB_WHITE, 31
				);
				break;
			}
		}
		break;
	case nn::yb::e::orin::STATE_SHOCK_KICK4:
		{
			disp_commonObjCenterColor( layout_orin->obj_id + 9,
				this->dstX2 + layout_orin->x + layout_orin->w / 2,
				this->dstY2 + layout_orin->y + layout_orin->h / 2, this->z + layout_orin->z,
				RGB_WHITE, 31
			);
		}
		break;
	}
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_ORIN::recvEventCb( void * effectORINArg, void * stateArg) {

	nn::yb::main::EFFECT_ORIN * orin = (nn::yb::main::EFFECT_ORIN *)effectORINArg;
	nn::yb::e::orin::CB_STATE state = (nn::yb::e::orin::CB_STATE)(int)stateArg;

	orin->recvEvent( state);

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_ORIN::recvEvent( nn::yb::e::orin::CB_STATE state) {
	
	switch(state) {
		case nn::yb::e::orin::CB_STATE_CLASH:
			this->m_uiAnimeCounter = 0;
			this->m_eState = nn::yb::e::orin::STATE_SHOCK_BEFORE;
			break;
	}

}

//---------------------------------------------------------------------------
# if 0
// 必要になったら
void nn::yb::main::EFFECT_ORIN::cutinEndCb( void * orinArg) {

	nn::yb::main::EFFECT_ORIN * orin = (nn::yb::main::EFFECT_ORIN *)orinArg;

	orin->cutinEnd();

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_ORIN::cutinEnd() {

	this->m_eState = nn::yb::e::orin::STATE_DASH;
	this->m_taskCutIn = NULL;

}
#endif
//---------------------------------------------------------------------------

