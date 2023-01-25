
#include <math.h>

#include "nn_common.h"
#include "nn_common2.h"
#include "nn_timer.h"

#include "draw.h"
#include "disp_common.h"
#include "animation.h"
#include "direct_common.h"
#include "sound_nns.h"

#include "sin_cos.h"

#include "enum.h"
#include "stage.h"
#include "cup.h"
#include "hole_result.h"
#include "hole.h"
#include "hole_ctrl.h"
#include "ctrl.h"
#include "camera.h"
#include "yb_common.h"
#include "edit_hole_obj.h"
#include "save_data.h"
#include "enemy.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "animation_data.h"
#include "config.h"

#include "sound_data.sadl"

#include "effect\\effect.h"
#include "effect\\effect_ctrl.h"
#include "effect\\musofuin.h"
#include "effect\\gungnir.h"
#include "effect\\mspark.h"
#include "effect\\momiji.h"
#include "effect\\tenko.h"
#include "effect\\nitori.h"
#include "effect\\flan.h"
#include "effect\\orin.h"

#include "effect\\muso_reimu.h"


#include "ball.h"

//---------------------------------------------------------------------------

nn::yb::main::CBall::CBall(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CBall Create\n");

	this->x = 0.0;
	this->y = 0.0;
	this->r	= 0;
	this->angle = 0;
	this->speed = 0.0;
	this->power = 0.0;
	this->cupHitState = 0;
	this->flySkipCnt = 0;
	this->cupInFlag = false;
	this->cupinDispCnt = 0;
	this->powerState = nn::yb::e::ball::POWER_STATE_NORMAL;

	this->procWaitCnt = 0;
	
	this->state = nn::yb::e::ball::STATE_NONE;
	this->oldState = nn::yb::e::ball::STATE_NONE;
	this->waitCnt = 0;

	this->holeCtrl = NULL;

	this->flyingEndCb = NULL;
	this->flyingEndCbArg = NULL;

	this->z = 0.0;
	this->zSpeed = 0.0;

	this->rotHorizon = 0.0;
	this->rotVertical = 0.0;

	this->goodLuckUpCnt = 0;

	this->goodLuckAnimeTask = NULL;

	this->slopeCnt = 0;
	this->flyState = nn::yb::e::ball::FLY_STATE_NONE;

	this->priority = 120;

	this->m_bIsGround = true;

	this->warpFlag = false;

	this->flyMove = new nn::common::def::graphics2d::SEGMENT2_D;

	this->goodLuckUpCntBound = 0;
	this->hideFlag = false;
	this->flyWarpExecFlag = false;

	this->dispState = nn::yb::e::ball::DISP_STATE_DEFAULT;
	this->dispStateReturnCnt = 0;
	this->musoFlag = false;


	this->m_pCurrSpellEffect = NULL;
	this->m_pMusoReimu = NULL;

	this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;

	this->flyBallFormPressFrameCnt = 0;

	this->bGenkotsuEffect = false;
	this->bFlareEffect = false;
	this->bBulletHitEffect = false;

	this->spellOrinCb = NULL;
	this->spellOrinCbArg = NULL;
	this->spellOrinState = 0;
	this->spellOrinAnimeTask = NULL;

	this->flyConveyorFlag = false;

	this->flyFukiageCheckFlag = false;
	this->flyFukiageCnt = 0;

	this->flyJumpCheckFlag = false;
	this->jumpFlag = false;
	this->flyJumpStrongCheckFlag = false;
	this->jumpStrongFlag = false;

	this->flyCheckBunkerOnFlag = false;

	this->flyGravityCheckFlag = false;
	this->flyGravityCnt = 0;

	this->flyMspaFrameCnt = 0;

	this->flyVampireRemainingFrame = 0;

	this->flyCheckConveyorFlag = false;
	this->shotTimeOverFlag = false;

}

//---------------------------------------------------------------------------

nn::yb::main::CBall::~CBall() {

	::nn_commonLog( 1, "CBall Delete\n");

	delete this->flyMove;

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::start() {

	this->x = 100.0;//1210.0;
	this->y = 100.0;//1170.0;
	this->r = 16;
	this->z = 0.0;
	this->angle = 270;
	this->speed = 0.0;
	this->power = 0.0;
	this->cupHitState = 0;
	this->flySkipCnt = 0;
	disp_commonSetCameraFlag(TRUE);
	disp_commonSetCameraSize(640, 480);
	// camera.cpp変更
	//disp_commonSetCameraPos( (int)this->x, (int)this->y);
	this->holeCtrl->ctrl->camera->setCameraPos( (int)this->x, (int)this->y);
	this->cupInFlag = false;
	this->cupinDispCnt = 0;
	this->powerState = 0;
	this->procWaitCnt = 0;
	this->state = nn::yb::e::ball::STATE_STANDBY;
	
#if 0
	this->stage->line[0].s.x = int(this->x - 100);
	this->stage->line[0].s.y = int(this->y - 100);
	this->stage->line[0].e.x = int(this->x + 100);
	this->stage->line[0].e.y = int(this->y - 100);

	this->stage->line[1].s.x = int(this->x + 100);
	this->stage->line[1].s.y = int(this->y - 100);
	this->stage->line[1].e.x = int(this->x + 100);
	this->stage->line[1].e.y = int(this->y + 100);

	this->stage->line[2].s.x = int(this->x + 100);
	this->stage->line[2].s.y = int(this->y + 100);
	this->stage->line[2].e.x = int(this->x - 100);
	this->stage->line[2].e.y = int(this->y + 100);

	this->stage->line[3].s.x = int(this->x - 100);
	this->stage->line[3].s.y = int(this->y + 100);
	this->stage->line[3].e.x = int(this->x - 100);
	this->stage->line[3].e.y = int(this->y - 100);

	this->stage->lineN = 4;
#endif

#if 0
	this->stage->line[0].s.x = int(this->x - 0);
	this->stage->line[0].s.y = int(this->y - 100);
	this->stage->line[0].e.x = int(this->x + 100);
	this->stage->line[0].e.y = int(this->y - 0);

	this->stage->line[1].s.x = int(this->x + 100);
	this->stage->line[1].s.y = int(this->y - 0);
	this->stage->line[1].e.x = int(this->x + 0);
	this->stage->line[1].e.y = int(this->y + 100);

	this->stage->line[2].s.x = int(this->x + 0);
	this->stage->line[2].s.y = int(this->y + 100);
	this->stage->line[2].e.x = int(this->x - 100);
	this->stage->line[2].e.y = int(this->y + 0);

	this->stage->line[3].s.x = int(this->x - 100);
	this->stage->line[3].s.y = int(this->y + 0);
	this->stage->line[3].e.x = int(this->x - 0);
	this->stage->line[3].e.y = int(this->y - 100);

	this->stage->lineN = 4;
#endif

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CBall::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	if(this->procWaitCnt == 0) {
		switch(this->state) {
			case nn::yb::e::ball::STATE_NONE:
				break;
			case nn::yb::e::ball::STATE_STANDBY:
				break;
			case nn::yb::e::ball::STATE_FLYING:
				this->taskFlying();
				break;
			case nn::yb::e::ball::STATE_SPELL:
				break;
			case nn::yb::e::ball::STATE_CUPIN:
				this->taskCupin();
				break;
			case nn::yb::e::ball::STATE_CUPIN_DISP:
				break;
		}
	}

	if(!this->hideFlag) {
		this->disp();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskCupin() {

	int cupin_disp_frame = 36;
	if(this->holeCtrl->stage->saveData->configData.dispMask == nn::yb::e::config::SET_DISP_MASK_ITEM_DISABLE) {
		cupin_disp_frame = 18;
	}
	if(this->cupinDispCnt < cupin_disp_frame) {
		this->cupinDispCnt += 3;
	}
	else {
		if(this->flyingEndCb != NULL) {
			this->flyingEndCb( this->flyingEndCbArg);
		}
		this->state = nn::yb::e::ball::STATE_CUPIN_DISP;
		if(this->ballForm == ::nn::yb::e::ball::BALL_FORM_SPEAR) {
			this->ballForm = ::nn::yb::e::ball::BALL_FORM_DEFAULT;
		}
	}
}

//---------------------------------------------------------------------------
#define ANGLE_2_RIGHT_LEFT_ANGLE (360 - (int)this->angle) + 90)
#define ANGLE_2_RIGHT_LEFT_VEC (nn::yb::e::orin::VEC)((ANGLE_2_RIGHT_LEFT_ANGLE % 360 / 180 * 2 + 1)
//added by nullpo: スペルカード処理
void nn::yb::main::CBall::spellCard(nn::yb::e::global::SPELL_CARD kind) {

	/*
	// EFFECT_MUSOFUIN() EFFECT_GUNGNIR()内で使用
	int animation_id_spell_char_tbl[nn::yb::e::global::SPELL_CARD_MAX] = {
		0, ANIMATION_ID_SPELL_CHAR_2_REIMU, ANIMATION_ID_SPELL_CHAR_2_REMIRIA,
	};
	*/
	int bgm_id_tbl[nn::yb::e::global::SPELL_CARD_MAX] = {
		0,	// SPELL_CARD_NONE
		BGM_MUSO,	// SPELL_CARD_MUSO
		BGM_SPEA,	// SPELL_CARD_SPEA
		BGM_MSPA,	// SPELL_CARD_MSPA
		BGM_AYA,	// SPELL_CARD_MOMIJI
		BGM_TENKO,	// SPELL_CARD_TENKO
		BGM_NITORI,	// SPELL_CARD_NITORI
		BGM_FLAN,	// SPELL_CARD_FLAN
		BGM_ORIN,	// SPELL_CARD_ORIN,
	};

	//既に起動されているエフェクトがある場合タスクを消去
	if (this->m_pCurrSpellEffect != NULL )
	{
		if (this->isLive(this->m_pCurrSpellEffect))
		{
			this->m_pCurrSpellEffect->ctrl->stopAllEffects();//stop 3D effect
			//古いエフェクトは終了するため、変数初期化
			this->m_pCurrSpellEffect->initialize();
			this->m_pCurrSpellEffect->taskDelete();

			if (this->m_pCurrSpellEffect->ctrl != NULL)
			{
				this->m_pCurrSpellEffect->ctrl->clear_cb();// clear callback
			}
		}
		this->m_pCurrSpellEffect = NULL;
	}

	switch (kind)
	{
	case nn::yb::e::global::SPELL_CARD_MUSO:
		{
			if (this->m_pMusoReimu != NULL )
			{
				if (this->isLive(this->m_pMusoReimu))
				{
					//古いエフェクトは終了するため、変数初期化
					this->m_pMusoReimu->taskDelete();
				}
				this->m_pMusoReimu = NULL;
			}

			this->holeCtrl->effect_ctrl->push_event( nn::yb::main::CBall::spellEffectEndDummyCb, this, NULL);
			
			nn::yb::main::EFFECT_MUSOFUIN arg;
			arg.ctrl = this->holeCtrl->effect_ctrl;

			arg.m_iStartX = (int)(this->x + 2);
			arg.m_iStartY = (int)(this->y - 24);

			//現在のカメラ位置を保存
			arg.camera_offset_x = (int)(this->holeCtrl->ctrl->camera->x);
			arg.camera_offset_y = (int)(this->holeCtrl->ctrl->camera->y);

			arg.dstLayX[0] = (int)(this->x + 2 + (n_commonRandNDirect( 9) - 4) * 80);
			arg.dstLayY[0] = (int)(this->y - this->z + 26 + (n_commonRandNDirect( 9) - 4) * 60);
			arg.dstLayX[1] = (int)(this->x + 2 + (n_commonRandNDirect( 9) - 4) * 80);
			arg.dstLayY[1] = (int)(this->y - this->z + 26 + (n_commonRandNDirect( 9) - 4) * 60);
			arg.dstLayX[2] = (int)(this->x + 2 + (n_commonRandNDirect( 9) - 4) * 80);
			arg.dstLayY[2] = (int)(this->y - this->z + 26 + (n_commonRandNDirect( 9) - 4) * 60);
			arg.dstLayX[3] = (int)(this->x + 2 + (n_commonRandNDirect( 9) - 4) * 80);
			arg.dstLayY[3] = (int)(this->y - this->z + 26 + (n_commonRandNDirect( 9) - 4) * 60);
			arg.dstLayX[4] = (int)(this->x + 2);
			arg.dstLayY[4] = (int)(this->y - this->z + 26);
			arg.dstN = 5;

			arg.m_bTargetHitEffect = true;

			arg.spellEffectMusoCb = nn::yb::main::CBall::spellEffectMusoCb;
			arg.ball = this;

			this->m_pCurrSpellEffect = new nn::yb::main::EFFECT_MUSOFUIN( NULL, &arg);

			// ドットキャラ
			////nn::yb::effect::CMusoReimu * mr = new nn::yb::effect::CMusoReimu( this);
			this->m_pMusoReimu = new nn::yb::effect::CMusoReimu( this);
			m_pMusoReimu->x = (int)(this->x);
			m_pMusoReimu->y = (int)(this->y - 16 - this->z);
			m_pMusoReimu->dispZ = nn::yb::e::global::DISP_Z_SPELL_BALL;
			m_pMusoReimu->endCb = nn::yb::main::CBall::spellEffectMusoReimuCb;
			m_pMusoReimu->endCbArg = this;
			m_pMusoReimu->objIdReimuTop = ::OBJ_ID_GAME_CHAR_REIMU_0;
			int index_add = (359 - ((int)this->angle + 359 - 112) % 360) / 45;
			m_pMusoReimu->objIdYukkuri = ::OBJ_ID_MAIN_YUKKURI_REIMU_0 + index_add;
			m_pMusoReimu->start();
			m_pMusoReimu->disp();

			this->zSpeed = nn::yb::e::global::FLYING_MUSOFUIN_SET_Z_SPEED;
			this->speed = nn::yb::e::global::FLYING_MUSOFUIN_SET_SPEED;
			this->musoFlag = true;
			this->state = nn::yb::e::ball::STATE_SPELL;

			this->setDispState( nn::yb::e::ball::DISP_STATE_MUSO);

			// EFFECT_GUNGNIR()内で実行
			//animationExec( NULL, animation_id_spell_char_tbl[this->holeCtrl->stage->spell], 320, 240, 100 + 513, FALSE, NULL, NULL);
		}
		break;
	case nn::yb::e::global::SPELL_CARD_SPEA:
		{
			this->holeCtrl->effect_ctrl->push_event( nn::yb::main::CBall::spellEffectEndGungnirCb, this, NULL);
			
			nn::yb::main::EFFECT_GUNGNIR arg;
			arg.ctrl = this->holeCtrl->effect_ctrl;
			//現在のカメラ位置を保存
			arg.camera_offset_x = this->holeCtrl->ctrl->camera->x;
			arg.camera_offset_y = this->holeCtrl->ctrl->camera->y;

			//着弾座標を渡す
			arg.m_vec3DTargetPos.x = (float)(this->x );
			arg.m_vec3DTargetPos.y = (float)(this->y - 48 + this->cupinDispCnt - (int)this->z );//ジャンプしている場合
			arg.m_vec3DTargetPos.z = (float)(this->z );//現在のballのz座標を渡す
			
			//衝撃波座標を渡す
			arg.m_vec3DWavePos.x = (float)(this->x );
			arg.m_vec3DWavePos.y = (float)(this->y );//着地座標
			arg.m_vec3DWavePos.z = (float)(0.0f );//

			arg.m_bIsGround = this->m_bIsGround;//現在位置が地面上にあるかどうか

			arg.m_pBall = this;//座標操作用に参照を渡す

			this->m_pCurrSpellEffect = new nn::yb::main::EFFECT_GUNGNIR( NULL, &arg);

			this->zSpeed = -20.0;
			this->speed = 0.0;

			// EFFECT_GUNGNIR()内で実行
			//animationExec( NULL, animation_id_spell_char_tbl[this->holeCtrl->stage->spell], 320, 240, 100 + 513, FALSE, NULL, NULL);
		}
		break;
	case nn::yb::e::global::SPELL_CARD_MSPA:
		{
			this->holeCtrl->effect_ctrl->push_event( nn::yb::main::CBall::spellEffectEndCb, this, NULL);
			
			nn::yb::main::EFFECT_MSPARK arg; 
			arg.ctrl = this->holeCtrl->effect_ctrl;

			//現在のカメラ位置を保存
			arg.camera_offset_x = this->holeCtrl->ctrl->camera->x;
			arg.camera_offset_y = this->holeCtrl->ctrl->camera->y;

			int front_x = (int)nn::yb::common::yb_commonAngleSpeed2MoveX( this->angle, 500.0);
			int front_y = (int)nn::yb::common::yb_commonAngleSpeed2MoveY( this->angle, 500.0);
			arg.shadowX = 320 + front_x;
			arg.shadowY = 240 + front_y; 
			arg.srcX = 320 - front_x;
			arg.srcY = 240 - front_y;
			arg.shadowSpeed = 15;
			arg.z = -15;
			arg.angle = (int)this->angle;
			arg.targetX = 320;
			arg.targetY = 240;

			arg.m_pBall = this;//表情操作用に参照を渡す

			arg.vec = (nn::yb::e::mspark::VEC)(((360 - (int)this->angle) + 315) % 360 / 90);

			this->m_pCurrSpellEffect = new nn::yb::main::EFFECT_MSPARK( NULL, &arg);

			this->zSpeed = 0.0;
			this->speed = nn::yb::e::global::FLYING_MSPARK_SET_SPEED;
			this->ballForm = nn::yb::e::ball::BALL_FORM_BURN;
			this->flyMspaFrameCnt = 0;
		}
		break;
	case nn::yb::e::global::SPELL_CARD_MOMIJI:
		{
			////this->holeCtrl->effect_ctrl->push_event( nn::yb::main::CBall::spellEffectEndCb, this, NULL);
			////this->holeCtrl->effect_ctrl->push_event( NULL, this, NULL);//EFFECT_MOMIJIクラス内で処理を途中で抜けるため、callbackは不要
			
			nn::yb::main::EFFECT_MOMIJI arg;
			arg.ctrl = this->holeCtrl->effect_ctrl;

			//現在のカメラ位置を保存
			arg.camera_offset_x = this->holeCtrl->ctrl->camera->x;
			arg.camera_offset_y = this->holeCtrl->ctrl->camera->y;

			int front_x = (int)nn::yb::common::yb_commonAngleSpeed2MoveX( this->angle, 500.0);
			int front_y = (int)nn::yb::common::yb_commonAngleSpeed2MoveY( this->angle, 500.0);
			arg.shadowX = (int)(this->x + front_x);
			arg.shadowY = (int)(this->y + front_y - this->z);
			arg.srcX = (int)(this->x - front_x);
			arg.srcY = (int)(this->y - front_y - this->z);
			arg.shadowSpeed = 30;
			arg.z = 177;
			arg.angle = (int)this->angle;
			arg.targetX = (int)(this->x);
			arg.targetY = (int)(this->y);

			arg.m_vec3DStartPos.x = (float)(this->x);
			arg.m_vec3DStartPos.y = (float)(this->y);
			arg.m_vec3DStartPos.z = (float)(this->z);

			arg.m_pBall = this;//表情操作用に参照を渡す

			this->m_pCurrSpellEffect = new nn::yb::main::EFFECT_MOMIJI( NULL, &arg);
		}
		break;
	case nn::yb::e::global::SPELL_CARD_TENKO:
		{
			this->holeCtrl->effect_ctrl->push_event( nn::yb::main::CBall::spellEffectEndTenkoCb, this, NULL);
			
			nn::yb::main::EFFECT_TENKO arg;
			arg.ctrl = this->holeCtrl->effect_ctrl;

			//現在のカメラ位置を保存
			arg.camera_offset_x = this->holeCtrl->ctrl->camera->x;
			arg.camera_offset_y = this->holeCtrl->ctrl->camera->y;

			int front_x = (int)nn::yb::common::yb_commonAngleSpeed2MoveX( this->angle, 500.0);
			int front_y = (int)nn::yb::common::yb_commonAngleSpeed2MoveY( this->angle, 500.0);
			arg.shadowX = this->holeCtrl->ctrl->displayW/2 + front_x;
			arg.shadowY = this->holeCtrl->ctrl->displayH/2 + front_y;
			arg.srcX = this->holeCtrl->ctrl->displayW/2 - front_x;
			arg.srcY = this->holeCtrl->ctrl->displayH/2 - front_y;
			arg.shadowSpeed = 20;
			arg.z = -15;
			arg.angle = (int)this->angle;
			arg.targetX = 320;
			arg.targetY = 240;

			arg.m_vec3DStartPos.x = (float)(this->x);
			arg.m_vec3DStartPos.y = (float)(this->y);
			arg.m_vec3DStartPos.z = (float)(this->z);

			arg.m_pBall = this;//表情操作用に参照を渡す

			this->m_pCurrSpellEffect = new nn::yb::main::EFFECT_TENKO( NULL, &arg);

		}
		break;
	case nn::yb::e::global::SPELL_CARD_NITORI:
		{
			this->holeCtrl->effect_ctrl->push_event( nn::yb::main::CBall::spellEffectEndNitoriCb, this, NULL);
			
			nn::yb::main::EFFECT_NITORI arg;
			arg.ctrl = this->holeCtrl->effect_ctrl;

			//現在のカメラ位置を保存
			arg.camera_offset_x = this->holeCtrl->ctrl->camera->x;
			arg.camera_offset_y = this->holeCtrl->ctrl->camera->y;

			int front_x = (int)nn::yb::common::yb_commonAngleSpeed2MoveX( this->angle, 500.0);
			int front_y = (int)nn::yb::common::yb_commonAngleSpeed2MoveY( this->angle, 500.0);
			arg.shadowX = 320 + front_x;
			arg.shadowY = 240 + front_y;
			arg.srcX = 320 - front_x;
			arg.srcY = 240 - front_y;
			arg.shadowSpeed = 20;
			arg.z = -15;
			arg.angle = (int)this->angle;

			arg.m_vec3DStartPos.x = (float)(this->x);
			arg.m_vec3DStartPos.y = (float)(this->y);
			arg.m_vec3DStartPos.z = (float)(this->z);

			arg.m_pBall = this;//表情操作用に参照を渡す

			if(this->goodLuckUpCnt >= nn::yb::e::global::FLYING_GOOD_LUCK_UP_AFTER_FRAME - 1) {
				if(this->speed < nn::yb::e::global::FLY_NITORI_RELEASE_SPEED_BUMPER_MIN) {
					this->speed = nn::yb::e::global::FLY_NITORI_RELEASE_SPEED_BUMPER_MIN;
				}
				else {
					this->speed *= nn::yb::e::global::FLY_NITORI_RELEASE_SPEED_BUMPER_RATE;
				}
			}
			else {
				if(this->zSpeed == 0.0) {
					if(this->speed < nn::yb::e::global::FLY_NITORI_RELEASE_SPEED_MIN) {
						this->speed = nn::yb::e::global::FLY_NITORI_RELEASE_SPEED_MIN;
					}
				}
			}

			this->m_pCurrSpellEffect = new nn::yb::main::EFFECT_NITORI( NULL, &arg);

		}
		break;
	case nn::yb::e::global::SPELL_CARD_FLAN:
		{
			this->holeCtrl->effect_ctrl->push_event( nn::yb::main::CBall::spellEffectEndFlanCb, this, NULL);
			
			nn::yb::main::EFFECT_FLAN arg;
			arg.ctrl = this->holeCtrl->effect_ctrl;

			//現在のカメラ位置を保存
			arg.camera_offset_x = this->holeCtrl->ctrl->camera->x;
			arg.camera_offset_y = this->holeCtrl->ctrl->camera->y;

			int front_x = (int)nn::yb::common::yb_commonAngleSpeed2MoveX( this->angle, 500.0);
			int front_y = (int)nn::yb::common::yb_commonAngleSpeed2MoveY( this->angle, 500.0);
			arg.shadowX = 320 + front_x;
			arg.shadowY = 240 + front_y;
			arg.srcX = 320 - front_x;
			arg.srcY = 240 - front_y;
			arg.shadowSpeed = 20;
			arg.z = -15;
			arg.angle = (int)this->angle;

			arg.m_vec3DStartPos.x = (float)(this->x);
			arg.m_vec3DStartPos.y = (float)(this->y);
			arg.m_vec3DStartPos.z = (float)(this->z);

			arg.m_pBall = this;//表情操作用に参照を渡す

			this->m_pCurrSpellEffect = new nn::yb::main::EFFECT_FLAN( NULL, &arg);
			
			this->zSpeed = 0.0;
			this->speed = 8.0;
			this->rotHorizon = 0.0;
			this->rotVertical = 0.0;

		}
		break;
	case nn::yb::e::global::SPELL_CARD_ORIN:
		{
			this->holeCtrl->effect_ctrl->push_event( nn::yb::main::CBall::spellEffectEndOrinCb, this, NULL);
			
			nn::yb::main::EFFECT_ORIN arg;
			arg.ctrl = this->holeCtrl->effect_ctrl;
			arg.master = this->master;

			//現在のカメラ位置を保存
			arg.camera_offset_x = this->holeCtrl->ctrl->camera->x;
			arg.camera_offset_y = this->holeCtrl->ctrl->camera->y;

			double temp_angle = 180.0 * ((ANGLE_2_RIGHT_LEFT_ANGLE % 360 / 180);
			arg.srcX  = 320 - 30 - (int)nn::yb::common::yb_commonAngleSpeed2MoveX( temp_angle, 500.0);
			arg.srcY  = 216 - 32 - (int)nn::yb::common::yb_commonAngleSpeed2MoveY( temp_angle, 500.0);
			arg.dstX  = 320 - 30 - (int)nn::yb::common::yb_commonAngleSpeed2MoveX( temp_angle, 64.0);
			arg.dstY  = 216 - 32 - (int)nn::yb::common::yb_commonAngleSpeed2MoveY( temp_angle, 64.0);
			arg.dstX2 = 320 - 30;
			arg.dstY2 = 216 - 32;
			arg.speed = 10;
			arg.z = -30;
			arg.vec = (nn::yb::e::orin::VEC)(((360 - (int)this->angle) + 135) % 360 / 90);
			arg.vec2 = ANGLE_2_RIGHT_LEFT_VEC;
			printf( "angle = %lf temp_angle = %lf vec2 = %d\n", this->angle, temp_angle, arg.vec2);

			arg.sendEventCb = nn::yb::main::CBall::spellEffectOrinCb;
			arg.sendEventCbArg = this;

			this->m_pCurrSpellEffect = new nn::yb::main::EFFECT_ORIN( NULL, &arg);
			
			if(this->zSpeed < 0.0) {
				this->zSpeed = 0.0;
			}
			else {
				this->zSpeed *= 1.5;
				if(this->zSpeed > 7.0) {
					this->zSpeed = 7.0;
				}
			}
			if(this->speed < 1.0) {
				this->speed = 1.0;
			}

		}
		break;
	}

	this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
	
	this->state = nn::yb::e::ball::STATE_SPELL;
	
	if(this->holeCtrl->nowBgm != bgm_id_tbl[this->holeCtrl->stage->spell]) {
		nn::yb::common::yb_commonPlaySeq( bgm_id_tbl[this->holeCtrl->stage->spell]);
		this->holeCtrl->nowBgm = bgm_id_tbl[this->holeCtrl->stage->spell];
	}
	
	this->flyFrameCnt = 0;

}
//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellGungnirHitEffect() {

	sound_nnsPlaySe( SE_GROUND);//ビット着弾SE
	//衝撃波の生成
	nn::yb::main::effect_start_end_total( this, OBJ_ID_DUN_SPELL_FLASH_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		15,
		(int)this->x, (int)this->x, 10,
		(int)(this->y - this->z), (int)(this->y - this->z), 10,
		127,
		32, 1024, 15,
		32, 1024, 15,
		0, 0, 10,
		31, 0, 15,
		DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, NULL);//拡大

	//ヒットエフェクトの生成
	nn::yb::main::effect_start_end_total(this, OBJ_ID_DUN_SPELL_HIT_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		30,
		(int) this->x,  (int)this->x, 30,
		(int)(this->y - this->z),  (int)(this->y - this->z), 30,
		127,
		32, 512, 30,
		32, 512, 30,
		0, 0, 30,
		24, 0, 30,
		DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, NULL);//拡大

	//エフェクトの生成
	nn::yb::main::effect_fadein_out(this, OBJ_ID_DUN_FUMAZIN_FUMAZIN_CENTER_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		120, 1, DISP_COMMON_BLEND_MODE_ADD ,enum_effect::EFFECT_ANIME_ADD , 
		20,
		(int)this->x,  (int)this->x,
		(int)(this->y- this->z) - 208,  (int)(this->y- this->z) - 208,
		128, 256,
		512, 512,
		0, 0,
		0, 31,
		80,
		(int)this->x,  (int)this->x,
		(int)(this->y - this->z) - 208,  (int)(this->y - this->z) - 208,
		256, 256,
		512, 512,
		0, 0,
		31, 12,
		20,
		(int)this->x,  (int)this->x,
		(int)(this->y - this->z) - 208,  (int)(this->y - this->z) - 208,
		256, 128,
		512, 512,
		0, 0,
		12, 0,
		NULL);//エフェクト(指定フレーム終了後に消去)

	nn::yb::main::effect_loop_fadein_out(this, OBJ_ID_DUN_FUMAZIN_FUMAZIN_LOOP_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		100, 1,DISP_COMMON_BLEND_MODE_ADD ,enum_effect::EFFECT_ANIME_FLAT, 
		0, 20,
		20,
		(int)this->x,  (int)this->x,
		(int)(this->y - this->z) - 40,  (int)(this->y - this->z) - 40,
		0, 90,
		900, 900,
		0, 0,
		0, 20,
		60,
		(int)this->x,  (int)this->x,
		(int)(this->y - this->z),  (int)(this->y - this->z),
		90, 90,
		900, 900,
		0, 0,
		20, 20,
		20,
		(int)this->x,  (int)this->x,
		(int)(this->y - this->z),  (int)(this->y - this->z),
		90, 0,
		900, 900,
		0, 0,
		20, 0,
		NULL);//縦方向ループエフェクト(指定フレーム終了後に消去)

}

//---------------------------------------------------------------------------

bool nn::yb::main::CBall::taskInputGoodLuck() {

	::KEY_WORK * key = &this->master->keyWork;

	bool result = false;

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_ORIN ||
		this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE ||
		this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS ||
		this->ballForm == nn::yb::e::ball::BALL_FORM_TORNADO
	) {
		return false;
	}

#if 1
	if((key->trigger & nn::common::e::PAD_WIN_BUTTON_A) && this->goodLuckAnimeTask == NULL)
#else
	// テスト用 常にタイミングよし
	if(this->goodLuckUpCnt > 0 && this->goodLuckAnimeTask == NULL)
#endif
	{
		// がんばりボタン
		bool large_flag = false;
		if(this->goodLuckUpCnt > 0) {
			if(this->speed < nn::yb::e::global::FLY_GOOD_LUCK_TIMING_SET_SPEED_BUMPER) {
				this->speed = nn::yb::e::global::FLY_GOOD_LUCK_TIMING_SET_SPEED_BUMPER;
			}
			else {
				this->speed *= nn::yb::e::global::FLY_GOOD_LUCK_TIMING_UP_SPEED_MUL_BUMPER;
			}
			if(this->ballForm == nn::yb::e::ball::BALL_FORM_BURN &&
				this->speed > 20.0
			) {
				this->speed = 20.0;
			}
			large_flag = true;
		}
		if(!large_flag) {
			if(this->goodLuckUpCntBound > 0 && this->flyBoundCnt == 1) {
				nn_commonLog( 1, "bound speed up speed = %lf\n", this->speed);
				this->speed *= nn::yb::e::global::FLY_GOOD_LUCK_TIMING_UP_SPEED_MUL_BOUND;
				this->zSpeed *= nn::yb::e::global::FLY_GOOD_LUCK_TIMING_UP_SPEED_MUL_BOUND;
				large_flag = true;
			}
		}
		if(large_flag) {
			this->goodLuckAnimeTask = animationExec( NULL, ANIMATION_ID_GOOD_LUCK_LARGE, (int)(this->x), (int)(this->y - this->z - 10), 40, FALSE, nn::yb::main::CBall::goodLuckEffectEndCb, this); 
		}
		else {
			this->speed *= nn::yb::e::global::FLY_GOOD_LUCK_UP_SPEED_MUL;
			this->goodLuckAnimeTask = animationExec( NULL, ANIMATION_ID_GOOD_LUCK_SMALL, (int)(this->x), (int)(this->y - this->z - 10), 40, FALSE, nn::yb::main::CBall::goodLuckEffectEndCb, this); 
		}
		// ボタン押下後すぐに跳ね返ってlargeに昇格した場合面倒なので、SEの差異はなし
		sound_nnsPlaySe( SE_POWAN2);
		result = true;
	}
	return result;

}

//---------------------------------------------------------------------------

bool nn::yb::main::CBall::taskInputSpell() {

	::KEY_WORK * key = &this->master->keyWork;

	if(this->holeCtrl->stage->spell != nn::yb::e::global::SPELL_CARD_NONE && this->spellCardEnableFlag) {
		if(key->trigger & nn::common::e::PAD_WIN_BUTTON_B) {
			// スペルカード
			////this->spellCard(nn::yb::e::global::SPELL_CARD_MUSO);
			this->spellCard(static_cast<nn::yb::e::global::SPELL_CARD>(this->holeCtrl->stage->spell));
			return true;
		}
	}
	return false;

}

//---------------------------------------------------------------------------

bool nn::yb::main::CBall::taskInputBallForm() {

	::KEY_WORK * key = &this->master->keyWork;
	bool result = false;

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
		if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
			this->releaseFlan();
			result = true;
		}
		else if(key->trigger & nn::common::e::PAD_WIN_KEY_UP) {
			this->angle = 90.0;
			result = true;
		}
		else if(key->trigger & nn::common::e::PAD_WIN_KEY_RIGHT) {
			this->angle = 0.0;
			result = true;
		}
		else if(key->trigger & nn::common::e::PAD_WIN_KEY_DOWN) {
			this->angle = 270.0;
			result = true;
		}
		else if(key->trigger & nn::common::e::PAD_WIN_KEY_LEFT) {
			this->angle = 180.0;
			result = true;
		}
	}

	return result;

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::releaseOrin() {

	this->speed = 0.0;
	this->zSpeed = nn::yb::e::global::ORIN_CLASH_AFTER_Z_SPEED;
	this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::releaseFlan() {
	
	sound_nnsPlaySe( SE_TELYUROROON);

	this->speed = 0.0;
	this->zSpeed = -1.0;
	this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::releaseGungnir() {
	
	this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;

	/*
	// 多分以下120922以降不要
	if (this->m_pCurrSpellEffect != NULL )
	{
		if (this->isLive(this->m_pCurrSpellEffect))
		{
			//古いエフェクトは終了するため、変数初期化
			this->m_pCurrSpellEffect->initialize();
			this->m_pCurrSpellEffect->taskDelete();

			this->m_pCurrSpellEffect->ctrl->clear_cb();// clear callback
		}
		this->m_pCurrSpellEffect = NULL;
	}
	*/

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyCup() {

	nn::yb::stage::CHole * hole =  this->holeCtrl->hole;

	if(hole->cup == NULL ||
		this->flyGroundOnFlag == false ||
		this->speed >= nn::yb::e::global::FLYING_HIGH_SPEED ||
		this->flyBoundFlag ||
		this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE
	) {
		return;
	}
	
	// ボールの中心とカップの中心の距離
	double vx_ball2cup = hole->cup->x - this->x;
	double vy_ball2cup = hole->cup->y - this->y;
	double len_ball2cup_pow2 = NN_COMMON_POW2( vx_ball2cup) + NN_COMMON_POW2( vy_ball2cup);
	double len_ball2cup = sqrt(NN_COMMON_POW2( vx_ball2cup) + NN_COMMON_POW2( vy_ball2cup));
	
	// 移動量
	double move_x = nn::yb::common::yb_commonAngleSpeed2MoveX( this->angle, this->speed);
	double move_y = nn::yb::common::yb_commonAngleSpeed2MoveY( this->angle, this->speed);

	// 移動後
	double vx_ball2cup_after = hole->cup->x - (this->x + move_x);
	double vy_ball2cup_after = hole->cup->y - (this->y + move_y);
	double len_ball2cup_pow2_after = NN_COMMON_POW2( vx_ball2cup_after) + NN_COMMON_POW2( vy_ball2cup_after);

	// ボールそのものベクトル
	double vx_ball = nn::yb::common::yb_commonAngleSpeed2MoveX( this->angle, 1.0);
	double vy_ball = nn::yb::common::yb_commonAngleSpeed2MoveY( this->angle, 1.0);
	double len_ball = sqrt(NN_COMMON_POW2( vx_ball) + NN_COMMON_POW2( vy_ball));

	// ボールとカップのベクトル比較
	double dot_ball2cup_ball = vx_ball2cup * vx_ball + vy_ball2cup * vy_ball;
	int angle_cpu2ball_ball = NN_COMMON_VEC2ANGLE( dot_ball2cup_ball, len_ball2cup, len_ball);

	// ボールそのものベクトルよりちょっと左とちょっと右
	int angle_left = (int)this->angle + nn::yb::e::global::DP_VEC_CHECK_SLIDE_ANGLE;
	if(angle_left >= 360) {
		angle_left -= 360;
	}
	int angle_right = (int)this->angle - nn::yb::e::global::DP_VEC_CHECK_SLIDE_ANGLE;
	if(angle_right < 0) {
		angle_right += 360;
	}
	double vx_ball_left		=  gCos[angle_left] * 1;
	double vy_ball_left		= -gSin[angle_left] * 1;
	double dp_ball_left		= vx_ball2cup * vx_ball_left + vy_ball2cup * vy_ball_left;
	double vx_ball_right	=  gCos[angle_right] * 1;
	double vy_ball_right	= -gSin[angle_right] * 1;
	double dp_ball_right	= vx_ball2cup * vx_ball_right + vy_ball2cup * vy_ball_right;
	int cup2ball_vec;
	if(dp_ball_left > dp_ball_right) {
		// カップの中心が進行方向の左
		cup2ball_vec = +1;
	}
	else if(dp_ball_left < dp_ball_right) {
		// カップの中心が進行方向の右
		cup2ball_vec = -1;
	}
	else {
		cup2ball_vec = 0;
	}

	if(len_ball2cup_pow2 <= NN_COMMON_POW2( (hole->cup->r - 4))) {
		this->flyCupOnFlag = true;
		// ボールの中心がカップの中
		if(len_ball2cup_pow2 <= NN_COMMON_POW2( (hole->cup->r - this->r))) {
			// ボール全体がカップの中
			if(len_ball2cup_pow2_after <= NN_COMMON_POW2( (hole->cup->r - this->r))) {
				// 移動後もボール全体がカップの中
				if(this->speed >= nn::yb::e::global::FLYING_MIDDLE_SPEED) {
					nn_commonLog( 999, "カップイン 中速\n");
					// スルー
					this->flyCupOnDispDownFlag = true;
				}
				else if(this->speed >= nn::yb::e::global::FLYING_LOW_SPEED) {
					nn_commonLog( 999, "カップイン 低速\n");
					// スルー
					this->flyCupOnDispDownFlag = true;
				}
				else if(this->speed >= nn::yb::e::global::FLYING_LOW_SPEED * 2.0 / 3.0) {
					nn_commonLog( 999, "カップイン 超低速\n");
					// スルー
					this->flyCupOnDispDownFlag = true;
				}
				else {
					nn_commonLog( 999, "カップイン 停止\n");
					this->speed = 0.0;
					this->flyCupOnDispDownFlag = true;
				}
			}
			else {
				// 移動後はボール全体がカップの中ではない
				// カップの壁にヒット
				if(this->speed >= nn::yb::e::global::FLYING_MIDDLE_SPEED) {
					// 中速
					nn_commonLog( 999, "中速でカップの壁にヒット\n");
					// スルー
					this->speed *= 0.8;
				}
				else if(this->speed >= nn::yb::e::global::FLYING_LOW_SPEED) {
					// 低速
					nn_commonLog( 999, "低速でカップの壁にヒット\n");
					// 停止
					this->speed *= 0.5;
				}
				else {
					// 停止
					nn_commonLog( 999, "停止でカップの壁にヒット\n");
					// 停止
					this->speed = 0.0;
				}
			}
		}
		else {
			// 縁
			if(this->speed >= nn::yb::e::global::FLYING_MIDDLE_SPEED) {
				// 中速
				if(nn::yb::e::global::BALL_2_CUP_REAR_ANGLE <= angle_cpu2ball_ball) {
					// 減速
					/*
					if(this->cupHitState == 0) {
						this->hit_cup();
					}
					*/
				}
				else if(nn::yb::e::global::BALL_2_CUP_SIDE_ANGLE <= angle_cpu2ball_ball && angle_cpu2ball_ball < nn::yb::e::global::BALL_2_CUP_REAR_ANGLE) {
					// 曲がる
					nn_commonLog( 999, "中速で縁を滑る\n");
					this->add_angle( (int)(angle_cpu2ball_ball *
						nn::yb::e::global::FLYING_SKATE_CUP_EDGE_ANGLE_ALTER_RATE_MIDDLE_SPEED *
						cup2ball_vec));
				}
				else {
					// スルー
				}
			}
			else if(this->speed >= nn::yb::e::global::FLYING_LOW_SPEED * 2.0 / 3.0) {
				// 低速
				if(nn::yb::e::global::BALL_2_CUP_REAR_ANGLE <= angle_cpu2ball_ball) {
					// ヒット
					nn_commonLog( 999, "低速で縁にヒット\n");
					/*
					// 中心に向かう
					this->add_angle( angle_cpu2ball_ball * cup2ball_vec);
					this->speed = nn::yb::e::global::FLYING_FALL_CONST_SPEED;
					*/
				}
				else if(nn::yb::e::global::BALL_2_CUP_SIDE_ANGLE <= angle_cpu2ball_ball && angle_cpu2ball_ball < nn::yb::e::global::BALL_2_CUP_REAR_ANGLE) {
					// 曲がる+加速
					nn_commonLog( 999, "低速で縁を滑る\n");
					this->add_angle( (int)(angle_cpu2ball_ball *
						nn::yb::e::global::FLYING_SKATE_CUP_EDGE_ANGLE_ALTER_RATE_LOW_SPEED *
						cup2ball_vec));
					this->speed += nn::yb::e::global::FLYING_SKATE_CUP_EDGE_ACC_SPEED;
				}
				else {
					// 中心に向かう
					this->add_angle( angle_cpu2ball_ball * cup2ball_vec);
					//this->speed = nn::yb::e::global::FLYING_FALL_CONST_SPEED;
				}
			}
			else {
				// 停止
				nn_commonLog( 999, "縁で停止\n");
				// 中心に向かう
				this->add_angle( angle_cpu2ball_ball * cup2ball_vec);
				this->speed = nn::yb::e::global::FLYING_FALL_CONST_SPEED;
			}
		}
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyBumper() {

	double old_angle = this->angle;

	nn::yb::stage::CHole * hole =  this->holeCtrl->hole;
	
	bool collision_flag = false;
	bool continue_flag = true;
	bool flags[1024];
	memset( flags, 0, sizeof( flags));
	while(continue_flag) {

		continue_flag = false;
		for( int i = 0; i < hole->data.holeObjN; i++) {

			nn::yb::def::HOLE_OBJ * obj = &hole->data.holeObj[i];
			switch(obj->category) {
				case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
					{
						if(flags[i] == false) {
							nn::common::def::graphics2d::LINE  line;
							line.s.x = obj->x1;
							line.s.y = obj->y1;
							line.e.x = obj->x2;
							line.e.y = obj->y2;

							bool result = false;
							nn::common::def::graphics2d::VECTOR2_D collision;

							nn::yb::common::CALC_BUMPER_DATA data;
							memset( &data, 0, sizeof( nn::yb::common::CALC_BUMPER_DATA));
							data.paraBumper = &line;
							data.paraMove = this->flyMove;
							data.paraZ = this->z;
							data.paraBumperZ = nn::yb::e::global::HOLE_BUMPER_Z;

							data.returnResult = &result;
							data.returnAngle = &this->angle;
							data.returnSpeed = &this->speed;
							data.returnX = &this->x;
							data.returnY = &this->y;
							data.returnCollision = &collision;

							nn::yb::common::yb_commonCalcBumper( &data);
							if(result) {
								flags[i] = true;
								collision_flag = true;
								continue_flag = true;
							}
						}
					}
					break;
			}
			if(continue_flag) {
				// 更新
				double add_x = nn::yb::common::yb_commonAngleSpeed2MoveX( angle, speed);
				double add_y = nn::yb::common::yb_commonAngleSpeed2MoveY( angle, speed);
				this->flyMove->s.x = x;
				this->flyMove->s.y = y;
				this->flyMove->v.x = add_x;
				this->flyMove->v.y = add_y;
				break;
			}

		}
	}

	if(collision_flag) {
		this->x += this->flyMove->v.x;
		this->y += this->flyMove->v.y;

		// SE
		if(this->ballForm == nn::yb::e::ball::BALL_FORM_MECHA) {
			sound_nnsPlaySe( SE_GASHAN);
		}
		else if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {

		}
		else {
			sound_nnsPlaySe( SE_BOYON);
		}

		if(this->ballForm == nn::yb::e::ball::BALL_FORM_ORIN) {
			this->angle = old_angle;
			this->releaseOrin();
		}
		else if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
			this->releaseFlan();
		}
		else {
			// がんばりボタン処理
			if(this->goodLuckAnimeTask != NULL) {
				ANIMATION * good_luck_anime_work = (ANIMATION *)this->goodLuckAnimeTask->work;
				if(good_luck_anime_work->frameCnt <= nn::yb::e::global::FLYING_GOOD_LUCK_UP_BEFORE_FRAME) {
					nn_commonLog( 999, "speed up\n");
					if(this->speed < nn::yb::e::global::FLY_GOOD_LUCK_TIMING_JUDGE_SPEED_BUMPER) {
						this->speed = nn::yb::e::global::FLY_GOOD_LUCK_TIMING_SET_SPEED_BUMPER;
					}
					else {
						this->speed *= nn::yb::e::global::FLY_GOOD_LUCK_TIMING_UP_SPEED_MUL_BUMPER;
					}
					if(this->ballForm == nn::yb::e::ball::BALL_FORM_BURN &&
						this->speed > 20.0
					) {
						this->speed = 20.0;
					}
					good_luck_anime_work->endCallBackFunc = NULL;
					::taskDelete( this->goodLuckAnimeTask);
					this->goodLuckAnimeTask = animationExec( NULL, ANIMATION_ID_GOOD_LUCK_LARGE, (int)(this->x), (int)(this->y - this->z - 10), 40, FALSE, nn::yb::main::CBall::goodLuckEffectEndCb, this); 
				}
			}
			else {
				this->goodLuckUpCnt = nn::yb::e::global::FLYING_GOOD_LUCK_UP_AFTER_FRAME;
			}

			this->flyProcFlag = true;
		}
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlySpin() {

	if(this->flyGroundOnFlag) {
		nn::yb::common::CALC_SPIN_DATA data;
		memset( &data, 0, sizeof( nn::yb::common::CALC_SPIN_DATA));
		data.angle = &this->angle;
		data.spinHorizon = &this->rotHorizon;
		nn::yb::common::yb_commonCalcSpin( &data);
	}

}

//---------------------------------------------------------------------------

bool nn::yb::main::CBall::taskFlyFall() {

	if(this->z <= nn::yb::e::global::FLY_FALL_Z) {
		sound_nnsPlaySe( SE_AAAAA);
		this->holeCtrl->stage->damage( -1);
		this->x = this->oldX;
		this->y = this->oldY;
		this->z = 0.0;
		this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;	// 落ちたら元の形状に戻る
		this->state = ::nn::yb::e::ball::STATE_STANDBY;
		if(this->flyingEndCb != NULL) {
			this->flyingEndCb( this->flyingEndCbArg);
		}
		return true;
	}
	else {
		return false;
	}

}
//---------------------------------------------------------------------------

bool nn::yb::main::CBall::taskFlyMove() {
	
	double add_x = 0.0;
	double add_y = 0.0;

	if(this->flySlopeStopCnt >= nn::yb::e::global::FLY_SLOPE_EFFECT_INTERVAL_FRAME + 1) {
		this->speed = 0.0;
	}

	nn::yb::common::CALC_MOVE_DATA data;
	memset( &data, 0, sizeof( nn::yb::common::CALC_MOVE_DATA));
	data.x = &this->x;
	data.y = &this->y;
	data.z = this->z;
	data.addX = &add_x;
	data.addY = &add_y;
	data.speed = &this->speed;
	data.angle = this->angle;
	data.flyBoundFlag = this->flyBoundFlag;
	data.zSpeed = this->zSpeed;
	data.ballForm = this->ballForm;
	nn::yb::common::yb_commonCalcMove( &data);

	nn_commonLog( 110319, "speed = %lf zSpeed = %lf z = %lf\n", this->speed, this->zSpeed, this->z);
	if(this->speed <= 0.300 && this->zSpeed == 0.0 && this->z == 0) {
		nn_commonLog( 110319, "stop!\n");
		bool stop_flag = true;
		if(this->flySlopeOnFlag) {
			// 坂の途中で
			if(this->flySlopeStopCnt >= nn::yb::e::global::FLY_SLOPE_EFFECT_INTERVAL_FRAME + 1) {
				stop_flag = true;
				nn_commonLog( 110319, "stop2!\n");
			}
			else {
				this->flySlopeStopCnt++;
				stop_flag = false;
			}
		}
		if(stop_flag) {
			// 停止処理
			this->cupHitState = 0;
			this->speed = 0.0;
			nn_commonLog( 110319, "stop3!\n");
			return true;
		}
	}
	return false;

}
//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyCupin() {

	nn::yb::stage::CHole * hole =  this->holeCtrl->hole;

	if(this->flyStopFlag ||
		(this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR && this->z == 0.0)) {
		if(hole->cup != NULL) {
			// カップイン判定
			double vx_ball2cup_cupin = hole->cup->x - this->x;
			double vy_ball2cup_cupin = hole->cup->y - this->y;
			double len_ball2cup_pow2_cupin = NN_COMMON_POW2( vx_ball2cup_cupin) + NN_COMMON_POW2( vy_ball2cup_cupin);
			double judge_len = NN_COMMON_POW2( (hole->cup->r - this->r));
			if(this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR) {
				judge_len = NN_COMMON_POW2( (hole->cup->r - 2));
			}
			if(len_ball2cup_pow2_cupin <= judge_len) {
				this->state = ::nn::yb::e::ball::STATE_CUPIN;
				// 停止したら元の形状に戻る
				if(this->ballForm != ::nn::yb::e::ball::BALL_FORM_SPEAR) {
					this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
				}
				if(len_ball2cup_pow2_cupin > NN_COMMON_POW2( (hole->cup->r - (this->r + 4)))) {
					this->x = (double)hole->cup->x;
					this->y = (double)hole->cup->y;
				}
			}
		}
		if(this->state != ::nn::yb::e::ball::STATE_CUPIN &&
			this->ballForm != nn::yb::e::ball::BALL_FORM_SPEAR
		) {
			if(this->moveStopCnt == 60) {
			// 停止したら元の形状に戻る
			this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
				this->state = ::nn::yb::e::ball::STATE_STANDBY;
				if(this->flyingEndCb != NULL) {
					this->flyingEndCb( this->flyingEndCbArg);
				}
			}
			else {
				this->moveStopCnt++;
			}
		}
	}
	else {
		this->moveStopCnt = 0;
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlySky() {

	nn::yb::stage::CHole * hole =  this->holeCtrl->hole;

	bool ground_flag = false;
	bool ground_side_flag = false;
	for( int i = 0; i < hole->data.holeObjN; i++) {

		nn::yb::def::HOLE_OBJ * obj = &hole->data.holeObj[i];
		switch(obj->category) {
			case nn::yb::e::edit_hole_obj::CATEGORY_GROUND:
			case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP:
			case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP:
			case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT:
			case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN:
			case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN:
			case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN:
			case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT:
			case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP:
				{
					if(obj->x1 <= (int)this->x && (int)this->x <= obj->x2 &&
						obj->y1 <= (int)this->y && (int)this->y <= obj->y2
					){
						ground_flag = true;
					}
					else if(obj->x1 - nn::yb::e::global::BALL_RADIUS <= (int)this->x && (int)this->x <= obj->x2 + nn::yb::e::global::BALL_RADIUS &&
						obj->y1 - nn::yb::e::global::BALL_RADIUS <= (int)this->y && (int)this->y <= obj->y2 + nn::yb::e::global::BALL_RADIUS
					){
						ground_side_flag = true;
					}
				}
				break;
		}

	}
	
	// 110412
	// ボールの中心とカップの中心の距離
	bool cup_flag = false;
	if(this->holeCtrl->hole->cup != NULL) {
		double vx_ball2cup = this->holeCtrl->hole->cup->x - this->x;
		double vy_ball2cup = this->holeCtrl->hole->cup->y - this->y;
		double len_ball2cup = sqrt(NN_COMMON_POW2( vx_ball2cup) + NN_COMMON_POW2( vy_ball2cup));
		if(len_ball2cup <= (double)this->holeCtrl->hole->cup->r * 0.550) {
			cup_flag = true;
		}
	}

	double add_z = 0.0;
	
	// 空中判定
	nn::yb::common::CALC_BALL_DATA data;
	memset( &data, 0, sizeof( nn::yb::common::CALC_BALL_DATA));
	data.angle = &this->angle;
	data.groundSideFlag = ground_side_flag;
	data.groundFlag = ground_flag;
	data.rotHorizon = &this->rotHorizon;
	data.rotVertical = &this->rotVertical;
	data.speed = &this->speed;
	data.z = &this->z;
	data.zSpeed = &this->zSpeed;
	data.groundOnFlag = &this->flyGroundOnFlag;
	data.flyBoundFlag = &this->flyBoundFlag;
	// 110412
	data.cupFlag = cup_flag;
	data.cupinFlag = &this->flySkyCupinFlag;
	data.addZ = &add_z;
	// 120521
	data.ballForm = this->ballForm;

	nn::yb::common::yb_commonCalcDown( &data);
	if(this->flySkyCupinFlag) {
		// 110412 チップイン(フライ時に直接カップイン)
		this->speed = 0.0;
		this->zSpeed = 0.0;
		this->x = (double)this->holeCtrl->hole->cup->x;
		this->y = (double)this->holeCtrl->hole->cup->y;
		this->z = 0.0;
		this->flyProcFlag = true;
		this->state = nn::yb::e::ball::STATE_CUPIN;
		if(this->ballForm != ::nn::yb::e::ball::BALL_FORM_SPEAR) {
			this->ballForm = ::nn::yb::e::ball::BALL_FORM_DEFAULT;
		}
		return;
	}

	if(this->flyBoundFlag) {
		if(this->ballForm == nn::yb::e::ball::BALL_FORM_MECHA) {
			sound_nnsPlaySe( SE_GASHAN);
		}
		else if(this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR) {

		}
		else if(this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS) {

		}
		else if(this->flyCheckBunkerOnFlag) {

		}
		else {
			sound_nnsPlaySe( SE_CHON);
		}
	}
	
	// がんばりボタン処理
	if(this->flyBoundFlag) {
		this->flyBoundCnt++;
		if(this->goodLuckAnimeTask != NULL) {
			ANIMATION * good_luck_anime_work = (ANIMATION *)this->goodLuckAnimeTask->work;
			if(good_luck_anime_work->frameCnt <= nn::yb::e::global::FLYING_GOOD_LUCK_UP_BEFORE_FRAME &&
				this->flyBoundCnt == 1
			) {
				nn_commonLog( 1, "bound speed up speed = %lf\n", this->speed);
				this->speed *= nn::yb::e::global::FLY_GOOD_LUCK_TIMING_UP_SPEED_MUL_BOUND;
				this->zSpeed *= nn::yb::e::global::FLY_GOOD_LUCK_TIMING_UP_SPEED_MUL_BOUND;
				good_luck_anime_work->endCallBackFunc = NULL;
				::taskDelete( this->goodLuckAnimeTask);
				this->goodLuckAnimeTask = animationExec( NULL, ANIMATION_ID_GOOD_LUCK_LARGE, (int)(this->x), (int)(this->y - this->z - 10), 40, FALSE, nn::yb::main::CBall::goodLuckEffectEndCb, this); 
			}
		}
		else {
			this->goodLuckUpCntBound = nn::yb::e::global::FLYING_GOOD_LUCK_UP_AFTER_FRAME;
		}
	}

	//落下フラグをグンニグルスペカエフェクト用に保存
	this->m_bIsGround = ground_flag;

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlySlope( nn::yb::def::HOLE_OBJ * obj) {

	if(this->flyCheckConveyorFlag) {
		return;
	}
	else if(this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS ||
		this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE
	) {
		return;
	}
	else if(this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR) {
		if(this->flySlopeOnFlag == false && this->flyCupOnFlag == false &&
			this->flyGroundOnFlag &&
			obj->x1 <= (int)this->x && (int)this->x <= obj->x2 &&
			obj->y1 <= (int)this->y && (int)this->y <= obj->y2
		) {
			double set_speed = 15.0;
			if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP)				{ this->speed = set_speed; this->angle =  90.0; }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP)		{ this->speed = set_speed; this->angle =  45.0; }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT)		{ this->speed = set_speed; this->angle =   0.0; }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN)	{ this->speed = set_speed; this->angle = 315.0; }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN)			{ this->speed = set_speed; this->angle = 270.0; }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN)	{ this->speed = set_speed; this->angle = 225.0; }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT)			{ this->speed = set_speed; this->angle = 180.0; }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP)		{ this->speed = set_speed; this->angle = 135.0; }
			this->flySlopeOnFlag = true;
		}
		return;
	}

	if((!this->flySlopeOnFlag) && (!this->flyCupOnFlag) && this->flyGroundOnFlag &&
		obj->x1 <= (int)this->x && (int)this->x <= obj->x2 &&
		obj->y1 <= (int)this->y && (int)this->y <= obj->y2
	){
		if(this->slopeCnt >= nn::yb::e::global::FLY_SLOPE_EFFECT_INTERVAL_FRAME) {
			// 前回適用してから一定フレーム以上経っていたら坂計算 カウントはtaskFlying()で行う
			nn::common::def::graphics2d::VECTOR2_D move;
			move.x = nn::yb::common::yb_commonAngleSpeed2MoveX( this->angle, this->speed);
			move.y = nn::yb::common::yb_commonAngleSpeed2MoveY( this->angle, this->speed);
			nn::common::def::graphics2d::VECTOR2_D add;
			double base = nn::yb::e::global::FLY_SLOPE_CALC_BASE_SPEED;
			if(this->flyBoundFlag) {
				base *= (this->zSpeed / nn::yb::e::global::FLY_SLOPE_BOUND_Z_SPEED_DIV);
			}
			/*
			int slope_angle = 0;
			// 使う場合要調整
			if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP)				slope_angle = 90;
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP)		slope_angle = 45;
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT)		slope_angle =  0;
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN)	slope_angle = 315;
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN)			slope_angle = 270;
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN)	slope_angle = 225;
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT)			slope_angle = 180;
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP)		slope_angle = 135;
			if((360 + slope_angle - (int)this->angle) % 360 <= 90 || (360 + (int)this->angle - slope_angle) % 360 <= 90) {
			}
			else {
				// ボールの方向と坂の方向が異なる
			}
			*/
			if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP)				{ add.x =   0.0                      ; add.y = -(base/(this->speed + 1.0));}
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP)		{ add.x =   base/(this->speed + 1.0) ; add.y = -(base/(this->speed + 1.0));}
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT)		{ add.x =   base/(this->speed + 1.0) ; add.y =   0.0                      ;}
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN)	{ add.x =   base/(this->speed + 1.0) ; add.y =   base/(this->speed + 1.0) ;}
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN)			{ add.x =   0.0                      ; add.y =   base/(this->speed + 1.0) ;}
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN)	{ add.x = -(base/(this->speed + 1.0)); add.y =   base/(this->speed + 1.0) ;}
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT)			{ add.x = -(base/(this->speed + 1.0)); add.y =   0.0                      ;}
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP)		{ add.x = -(base/(this->speed + 1.0)); add.y = -(base/(this->speed + 1.0));}
			nn::common::def::graphics2d::VECTOR2_D after;
			after.x = move.x + add.x;
			after.y = move.y + add.y;
			nn::common::def::graphics2d::VECTOR2_D angle0;
			angle0.x = 1000.0;
			angle0.y = 0.0;
			double dot = nn::common::graphics2d::vec2dot( &angle0, &after);
			double cross = nn::common::graphics2d::vec2cross( &angle0, &after);
			double after_speed = sqrt( NN_COMMON_POW2( after.x) + NN_COMMON_POW2( after.y));
			int after_angle = NN_COMMON_VEC2ANGLE2( dot, cross);
			if(after_angle < 0) {
				after_angle += 360;
			}
			after_angle = 360 - after_angle;
			::nn_commonLog( 101225, "move.x = %lf move.y = %lf\n", move.x, move.y);
			::nn_commonLog( 101225, "after.x = %lf after.y = %lf\n", after.x, after.y);
			::nn_commonLog( 101225, "this->angle = %d\n", (int)this->angle);
			::nn_commonLog( 101225, "after_angle = %d\n", after_angle);
			::nn_commonLog( 101225, "after_speed = %lf", after_speed);
			if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP)				{ if( 89 <= after_angle && after_angle <=  91) { after_angle =  90;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP)		{ if( 44 <= after_angle && after_angle <=  46) { after_angle =  45;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT)		{ if(359 <= after_angle && after_angle <=   1) { after_angle =   0;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN)	{ if(314 <= after_angle && after_angle <= 316) { after_angle = 315;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN)			{ if(269 <= after_angle && after_angle <= 271) { after_angle = 270;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN)	{ if(224 <= after_angle && after_angle <= 226) { after_angle = 225;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT)			{ if(179 <= after_angle && after_angle <= 181) { after_angle = 180;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP)		{ if(134 <= after_angle && after_angle <= 136) { after_angle = 135;} }
			
			if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN)				{ if( 89 <= after_angle && after_angle <=  91) { after_angle =  90;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN)	{ if( 44 <= after_angle && after_angle <=  46) { after_angle =  45;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT)			{ if(359 <= after_angle && after_angle <=   1) { after_angle =   0;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP)		{ if(314 <= after_angle && after_angle <= 316) { after_angle = 315;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP)			{ if(269 <= after_angle && after_angle <= 271) { after_angle = 270;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP)		{ if(224 <= after_angle && after_angle <= 226) { after_angle = 225;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT)		{ if(179 <= after_angle && after_angle <= 181) { after_angle = 180;} }
			else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN)	{ if(134 <= after_angle && after_angle <= 136) { after_angle = 135;} }

			this->speed = after_speed;
			this->angle = after_angle;
			this->slopeCnt = 0;
		}
		this->flySlopeOnFlag = true;

		obj->onFlag = 1;
	}
	/*
	// (this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR)の中に来ない 不要
	else if (
		obj->x1 > (int)this->x || (int)this->x > obj->x2 ||
		obj->y1 > (int)this->y || (int)this->y > obj->y2
		)
	{
		if (obj->onFlag == 1)
		{
			//前フレームで範囲内にあり、今回のフレームで範囲外に出た場合
			if (this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR)
			{
				if (this->m_pCurrSpellEffect != NULL)
				{
					//終了エフェクトを出して停止
					nn::yb::main::EFFECT_GUNGNIR* gungnir = (nn::yb::main::EFFECT_GUNGNIR*)this->m_pCurrSpellEffect;
					gungnir->anime_cb(this->m_pCurrSpellEffect);
				}
				this->speed = 0.0;
				this->zSpeed = 0.0;
			}
		}
		obj->onFlag = -1;
	}
	*/

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyPillar(  nn::yb::def::HOLE_OBJ * obj) {

	// とりあえず壁状の柱へのめり込みは解消しない
	//if(0 <= (int)this->z && (int)this->z <= nn::yb::e::global::HOLE_PILLAR_Z && (!this->flyPillarHitFlag))
	if(0 <= (int)this->z && (int)this->z <= nn::yb::e::global::HOLE_PILLAR_Z)
	{
		nn::common::def::graphics2d::SEGMENT2_D after;
		double afterLen;
		nn::common::def::graphics2d::VECTOR2_D circle;
		circle.x = obj->x1;
		circle.y = obj->y1;
		bool result = nn::common::graphics2d::isCollisionCircle(
			&after, &afterLen, this->flyMove, nn::yb::e::global::BALL_RADIUS, &circle, nn::yb::e::global::HOLE_PILLAR_RADIUS
		);
		if(result) {
			nn::common::def::graphics2d::SEGMENT2_D no_move;
			no_move = *this->flyMove;
			no_move.v.x = 0.0;
			no_move.v.y = 0.0;
			result = nn::common::graphics2d::isCollisionCircle(
				&after, &afterLen, &no_move, nn::yb::e::global::BALL_RADIUS, &circle, nn::yb::e::global::HOLE_PILLAR_RADIUS
			);
			if(this->ballForm == nn::yb::e::ball::BALL_FORM_MECHA) {
				this->defeatedOjama( obj);//お邪魔キャラを破壊
			}
			else {
				if(result) {
					// 動かなくても衝突なら上から衝突
					sound_nnsPlaySe( SE_KASHAKASHA);
					this->z = (double)(nn::yb::e::global::HOLE_PILLAR_Z + 1);
					if(this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR) {
						this->zSpeed = 6.0;
						this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
					}
					else {
						this->zSpeed = -(this->zSpeed * nn::yb::e::global::FLY_BOUND_Z_SPEED_RATE);
					}

					//	大ジャンプ・グングニルで木の上で進行不可になる問題対応
					if(this->speed < 2.0) {
						this->speed = 2.0;
					}
				}
				else {
					sound_nnsPlaySe( SE_POKUN);
					if(this->ballForm == nn::yb::e::ball::BALL_FORM_ORIN) {
						this->releaseOrin();
					}
					else if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
						this->releaseFlan();
					}
					else {
						int after_angle = (int)nn::common::graphics2d::pos2angle( circle.x, circle.y, after.s.x, after.s.y);
						if(after_angle < 0) {
							after_angle += 360;
						}
						this->angle = after_angle;
						this->speed *= nn::yb::e::global::FLY_PILLAR_SPEED_RATE;
						// とりあえず壁状の柱へのめり込みは解消しない
						//this->flyPillarHitFlag = true;
					}
				}
			}
		}
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyWarp(  nn::yb::def::HOLE_OBJ * obj) {

	bool warp_exec_flag = false;
	if(this->isHitWarp1( obj)){
		if( (!this->warpFlag) && this->flyGroundOnFlag) {
			sound_nnsPlaySe( SE_DYUJUJU);
			this->x = obj->x2;
			this->y = obj->y2;
			switch(obj->category) {
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP:
					this->angle = 90.0;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT:
					this->angle = 0.0;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN:
					this->angle = 270.0;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_LEFT:
					this->angle = 180.0;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP:
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP:
					{
						switch(obj->vec) {
							case nn::yb::e::edit_hole_obj::WARP2_VEC_UP:
								this->angle = 90.0f;
								break;
							case nn::yb::e::edit_hole_obj::WARP2_VEC_RIGHT_UP:
								this->angle = 45.0f;
								break;
							case nn::yb::e::edit_hole_obj::WARP2_VEC_RIGHT:
								this->angle = 0.0f;
								break;
							case nn::yb::e::edit_hole_obj::WARP2_VEC_RIGHT_DOWN:
								this->angle = 315.0f;
								break;
							case nn::yb::e::edit_hole_obj::WARP2_VEC_DOWN:
								this->angle = 270.0f;
								break;
							case nn::yb::e::edit_hole_obj::WARP2_VEC_LEFT_DOWN:
								this->angle = 225.0f;
								break;
							case nn::yb::e::edit_hole_obj::WARP2_VEC_LEFT:
								this->angle = 180.0f;
								break;
							case nn::yb::e::edit_hole_obj::WARP2_VEC_LEFT_UP:
								this->angle = 135.0f;
								break;
							case nn::yb::e::edit_hole_obj::WARP2_VEC_NONE:
								break;
						}
					}
					break;
			}
			warp_exec_flag = true;
			this->warpFlag = true;
		}
	}
	else if(this->isHitWarp2( obj)){
		if( (!this->warpFlag) && this->flyGroundOnFlag) {
			sound_nnsPlaySe( SE_DYUJUJU);
			this->x = obj->x1;
			this->y = obj->y1;
			switch(obj->category) {
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_UP:
					this->angle = 90.0;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_UP:
					this->angle = 45.0f;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT:
					this->angle = 0.0;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_DOWN:
					this->angle = 315.0f;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_DOWN:
					this->angle = 270.0;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_DOWN:
					this->angle = 225.0f;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT:
					this->angle = 180.0;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP:
					this->angle = 135.0f;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP:
				case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE:
					break;
			}
			warp_exec_flag = true;
			this->warpFlag = true;
		}
	}

	if(warp_exec_flag) {
		this->holeCtrl->ctrl->camera->moveCameraPos( (int)this->x, (int)this->y,
			nn::yb::main::CBall::warpEndCb, this
		);
		this->procWaitCnt++;
		this->hideFlag = true;
		this->flyWarpExecFlag = true;
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::warpEndCb( void * ballArg) {

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;

	ball->warEnd();

}
//---------------------------------------------------------------------------

void nn::yb::main::CBall::warEnd() {

	this->hideFlag = false;
	this->procWaitCnt--;

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyJump( nn::yb::def::HOLE_OBJ * obj) {

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
		return;
	}
	
	if(obj->x1 - 32 <= (int)this->x && (int)this->x <= obj->x1 + 32 &&
		obj->y1 - 32 <= (int)this->y && (int)this->y <= obj->y1 + 32
	){
		this->flyJumpCheckFlag = true;
		if(this->jumpFlag == false && this->flyGroundOnFlag &&
			this->flyBoundFlag == false && this->speed > 0.0
		){
			sound_nnsPlaySe( SE_PYON);
			this->zSpeed = nn::yb::e::global::FLY_JUMP_Z_SPEED_BASE * this->speed;
			this->zSpeed = 0.800 * this->speed;
			this->speed *= 0.600;
			/*
			// 制限は面白くない
			if(this->zSpeed > nn::yb::e::global::FLY_JUMP_Z_SPEED_MAX) {
				this->zSpeed = nn::yb::e::global::FLY_JUMP_Z_SPEED_MAX;
			}
			this->speed = nn::yb::e::global::FLY_JUMP_SPEED_BASE * this->speed;
			if(this->speed > nn::yb::e::global::FLY_JUMP_SPEED_MAX) {
				this->speed = nn::yb::e::global::FLY_JUMP_SPEED_MAX;
			}
			*/
			::nn_commonLog( 0, "this->speed = %lf this->zSpeed = %lf\n", this->speed, this->zSpeed);
			this->jumpFlag = true;
		}
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyDash( nn::yb::def::HOLE_OBJ * obj) {

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
		return;
	}

	if(this->flyGroundOnFlag && (!this->flyBoundFlag) &&
		(!this->shotTimeOverFlag) &&
		obj->x1 - 48 <= (int)this->x && (int)this->x <= obj->x1 + 48 &&
		obj->y1 - 48 <= (int)this->y && (int)this->y <= obj->y1 + 48
	){
		if(this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS) {
			this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
		}

		switch(obj->category) {
			case nn::yb::e::edit_hole_obj::CATEGORY_DASH:
				if(this->speed < nn::yb::e::global::FLY_DASH_SPEED) {
					if(this->flyDashSeWaitCnt <= 0) {
						sound_nnsPlaySe( SE_SHUN);
						this->flyDashSeWaitCnt = 10;
					}
					this->speed = nn::yb::e::global::FLY_DASH_SPEED;
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP:
			case nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT:
			case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN:
			case nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT:
			case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_LEFT:
			case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_RIGHT:
			case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_LEFT:
			case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_RIGHT:
#if 0
				// ダッシュ後の角度に誤差がある場合がある(左=179度等)
				nn::common::def::graphics2d::VECTOR2_D move;
				move.x = nn::yb::common::yb_commonAngleSpeed2MoveX( this->angle, this->speed);
				move.y = nn::yb::common::yb_commonAngleSpeed2MoveY( this->angle, this->speed);
				nn::common::def::graphics2d::VECTOR2_D add;
				if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP)				{ add.x =   0.0 ; add.y = -20.0;}
				else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT)		{ add.x = +20.0 ; add.y =   0.0;}
				else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN)		{ add.x =   0.0 ; add.y = +20.0;}
				else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT)		{ add.x = -20.0 ; add.y =   0.0;}
				else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_LEFT)	{ add.x = -20.0 ; add.y = -20.0;}
				else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_RIGHT)	{ add.x =  20.0 ; add.y = -20.0;}
				else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_LEFT)	{ add.x = -20.0 ; add.y =  20.0;}
				else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_RIGHT){ add.x =  20.0 ; add.y =  20.0;}
				nn::common::def::graphics2d::VECTOR2_D after;
				after.x = move.x + add.x;
				after.y = move.y + add.y;
				nn::common::def::graphics2d::VECTOR2_D angle0;
				angle0.x = 1000.0;
				angle0.y = 0.0;
				// 後で ここまで複雑な計算いる？
				double dot = nn::common::graphics2d::vec2dot( &angle0, &after);
				double cross = nn::common::graphics2d::vec2cross( &angle0, &after);
				double after_speed = sqrt( NN_COMMON_POW2( after.x) + NN_COMMON_POW2( after.y));
				int after_angle = NN_COMMON_VEC2ANGLE2( dot, cross);
				if(after_angle < 0) {
					after_angle += 360;
				}
				after_angle = 360 - after_angle;

				if(this->flyDashStandbyCnt < 3) {
					this->flyDashStandbyCnt++;
				}
				else if(this->flyDashStandbyCnt < 4) {
					this->flyDashStandbyCnt++;
					this->angle = after_angle;
				}
				else {
					if(this->speed < nn::yb::e::global::FLY_DASH_SPEED) {
						if(this->flyDashSeWaitCnt <= 0) {
							sound_nnsPlaySe( SE_SHUN);
							this->flyDashSeWaitCnt = 10;
						}
						this->speed = nn::yb::e::global::FLY_DASH_SPEED;
					}
					this->angle = after_angle;
				}
#else
				if(this->flyDashStandbyCnt < 5) {
					this->flyDashStandbyCnt++;
				}
				else {
					if(this->speed < nn::yb::e::global::FLY_DASH_SPEED) {
						if(this->flyDashSeWaitCnt <= 0) {
							sound_nnsPlaySe( SE_SHUN);
							this->flyDashSeWaitCnt = 10;
						}
						this->speed = nn::yb::e::global::FLY_DASH_SPEED;
					}
					if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP)				{ this->angle =  90.0;}
					else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT)		{ this->angle =   0.0;}
					else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN)		{ this->angle = 270.0;}
					else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT)		{ this->angle = 180.0;}
					else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_LEFT)	{ this->angle = 135.0;}
					else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_RIGHT)	{ this->angle =  45.0;}
					else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_LEFT)	{ this->angle = 225.0;}
					else if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_RIGHT){ this->angle = 315.0;}
					this->speed = nn::yb::e::global::FLY_DASH_SPEED;
				}
#endif
				break;
		}
	}


}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyKick( nn::yb::def::HOLE_OBJ * obj) {

	if(this->flyGroundOnFlag &&
		obj->x1 - nn::yb::e::global::HOLE_KICK_HALF_W <= (int)this->x && (int)this->x <= obj->x1 + nn::yb::e::global::HOLE_KICK_HALF_W &&
		obj->y1 - nn::yb::e::global::HOLE_KICK_HALF_H <= (int)this->y && (int)this->y <= obj->y1 + nn::yb::e::global::HOLE_KICK_HALF_H
	){
		int old_angle = (int)this->angle;
		switch(obj->category) {
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP:
				this->angle = 90;
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_RIGHT:
				this->angle = 0;
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN:
				this->angle = 270;
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_LEFT:
				this->angle = 180;
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_RIGHT:
				this->angle = 45;
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_LEFT:
				this->angle = 135;
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_RIGHT:
				this->angle = 315;
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_LEFT:
				this->angle = 225;
				break;
		}
		if(this->angle != old_angle) {
			sound_nnsPlaySe( SE_GAKON);
			nn_commonLog( 1, "ガコン");
		}
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyDamage( nn::yb::def::HOLE_OBJ * obj) {

	if(this->flyGroundOnFlag &&
		obj->x1 + 16 <= (int)this->x && (int)this->x <= obj->x2 - 16 &&
		obj->y1 + 16 <= (int)this->y && (int)this->y <= obj->y2 - 16
	){
		// 効果音
		sound_nnsPlaySe( SE_GYA);

		//進行方向を維持したまま任意の速度(xy軸・z軸)で跳ね上がります。
		this->zSpeed = nn::yb::e::global::FLY_DAMAGE_Z_SPEED;
		this->speed = nn::yb::e::global::FLY_DAMAGE_SPEED;

		
		if (this->ballForm == nn::yb::e::ball::BALL_FORM_TORNADO)
		{
			//竜巻エフェクト削除
			if (this->m_pCurrSpellEffect != NULL)
			{
				if (this->isLive(this->m_pCurrSpellEffect))
				{
					this->m_pCurrSpellEffect->ctrl->stopEffect();
				}
			}
		}
		this->holeCtrl->stage->damage( -1);

		//スペルカード状態等解除
		this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;

		this->setDispState( nn::yb::e::ball::DISP_STATE_SHOCK, 45);
	}
}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyBunker( nn::yb::def::HOLE_OBJ * obj) {

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_TORNADO ||
		this->ballForm == nn::yb::e::ball::BALL_FORM_ORIN ||
		this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE
	) {
		return;
	}

	//接地している間、超減速する, 跳ねない
	if(this->flyGroundOnFlag && 
		obj->x1 <= (int)this->x && (int)this->x <= obj->x2 &&
		obj->y1 <= (int)this->y && (int)this->y <= obj->y2
	){
		this->zSpeed = 0.0;//跳ねない

		if(this->flyBoundFlag) {
			sound_nnsPlaySe( SE_ZA);
			this->speed = 0.0;
		}
		else {
			this->speed *= nn::yb::e::global::FLY_BUNKER_SPEED_RATE;
		}
		
	}

}
//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyGravity( nn::yb::def::HOLE_OBJ * obj) {

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS) {
		return;
	}
	
	if(obj->x1 <= (int)this->x && (int)this->x <= obj->x2 &&
		obj->y1 <= (int)this->y && (int)this->y <= obj->y2
	){
		if(!this->flyGroundOnFlag)
		{
			if(this->flyGravityCnt % 60 == 0) {
				sound_nnsPlaySe( SE_NUUN);
			}

			this->flyBoundFlag = false;

			if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
				this->z -= 4.0;
				if(this->z < 0.0) {
					this->z = 0.0;
				}
			}
			else {
				this->zSpeed -= nn::yb::e::global::FLY_GRAVITY_Z_SPEED_BASE; //速度はマイナスに
				if(this->zSpeed < nn::yb::e::global::FLY_GRAVITY_Z_SPEED_MIN) {
					this->zSpeed = nn::yb::e::global::FLY_GRAVITY_Z_SPEED_MIN;
				}
			}
		}
		else
		{
			//接地している場合
			this->zSpeed = 0;//跳ねない
		}
		this->flyGravityCheckFlag = true;
		this->flyGravityCnt++;
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyConveyor( nn::yb::def::HOLE_OBJ * obj) {
	
	if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
		return;
	}
 
	if(
		this->flyGroundOnFlag &&
		obj->x1 <= (int)this->x && (int)this->x <= obj->x2 &&
		obj->y1 <= (int)this->y && (int)this->y <= obj->y2 
	){
		if (
			!this->flyConveyorFlag &&
			::abs(this->speed) < nn::yb::e::global::FLY_CONVEYOR_MAX_SPEED //速度が十分小さい場合、コンベアで移動
			)
		{
			switch(obj->category) {
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP:
					this->angle = 90;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT:
					this->angle = 0;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN:
					this->angle = 270;
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT:
					this->angle = 180;
					break;
			}
			this->speed =  nn::yb::e::global::FLY_CONVEYOR_SPEED;
			this->rotHorizon = 0.0;
			this->rotVertical = 0.0;
			this->flyConveyorFlag = true;
			this->flyProcFlag = true;
		}
		else if (
			this->flyConveyorFlag
			)
		{
			// se
			if(this->flyFrameCnt % 40 == 0) {
				sound_nnsPlaySe( SE_KORORIN);
			}

			//次のフレームの座標を計算
			double next_x = this->x + this->flyMove->v.x;
			double next_y = this->y + this->flyMove->v.y;
			//次のフレームでコンベア範囲外に出る場合
			if(
				obj->x1 > (int)next_x || (int)next_x > obj->x2 ||
				obj->y1 > (int)next_y || (int)next_y > obj->y2 
			)
			{
				//射出速度を設定
				this->speed =  nn::yb::e::global::FLY_CONVEYOR_ESCAPE_SPEED;
			}
			else
			{
				//移動中の場合
				this->speed =  nn::yb::e::global::FLY_CONVEYOR_SPEED;
			}
		}
	}
	else
	{
		//他のすべてのコンベアーの範囲にも含まれていない場合
		bool bIsOutOfDC = false;
		nn::yb::stage::CHole * hole =  this->holeCtrl->hole;
		for( int i = 0; i < hole->data.holeObjN; i++) 
		{
			nn::yb::def::HOLE_OBJ * objConv = &hole->data.holeObj[i];
			if (
				(
					objConv->category == nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP ||
					objConv->category == nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT ||
					objConv->category == nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT ||
					objConv->category == nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN 
				)
				&& objConv != obj
				)
			{	
				if(
					this->flyGroundOnFlag &&
					objConv->x1 <= (int)this->x && (int)this->x <= objConv->x2 &&
					objConv->y1 <= (int)this->y && (int)this->y <= objConv->y2 
				)
				{
					bIsOutOfDC = true;//ゆっくりが一つでもコンベアーの当たり判定に入っている場合
					break;
				}
			}
		}
		if (!bIsOutOfDC)
			this->flyConveyorFlag = false;//全てのコンベアーの当たり判定からゆっくりが範囲外に出た場合は、フラグをリセット
	}
}
//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyFukiage( nn::yb::def::HOLE_OBJ * obj) {

	if (
		this->ballForm == nn::yb::e::ball::BALL_FORM_MECHA ||
		this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR 
		)
	{
		//スピア、メカ時は効果を受けない
		return;
	}

	if (obj->x1 <= (int)this->x && (int)this->x <= obj->x2 &&
		obj->y1 <= (int)this->y && (int)this->y <= obj->y2
	){
		if (this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS)
		{
			//プレス時はより多くの効果を受ける
			if (0 <= (int)this->z && (int)this->z <= 96)
			{
				this->zSpeed += nn::yb::e::global::FLY_FUKIAGE_Z_SPEED_PRESS;
			}
			else if (128 <= (int)this->z)
			{
				this->zSpeed = -0.1;
			}
			if(this->speed < 1.5) {
				this->speed = 1.5;
			}
		}
		else if (this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE)
		{
			// フラン化時は上昇するだけ
			if(this->z < 150.0)
			{
				this->z += 1.0;
			}
		}
		else
		{
			if (0 <= (int)this->z && (int)this->z <= 96)
			{
				if(abs(this->speed) > 0.1) {
					this->zSpeed += nn::yb::e::global::FLY_FUKIAGE_Z_SPEED;
				}
			}
		}
		// se
		if (this->flyFukiageCnt % 90 == 0)
		{
			sound_nnsPlaySe( SE_FUON);
		}
		this->flyFukiageCheckFlag = true;
		this->flyFukiageCnt++;
	}
}
//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyJumpWeak( nn::yb::def::HOLE_OBJ * obj) {

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
		return;
	}

	if(this->flyGroundOnFlag && (!this->flyBoundFlag) &&
		obj->x1 - 32 <= (int)this->x && (int)this->x <= obj->x1 + 32 &&
		obj->y1 - 32 <= (int)this->y && (int)this->y <= obj->y1 + 32
	){
		sound_nnsPlaySe( SE_PYON);
		this->zSpeed = nn::yb::e::global::FLY_JUMP_WEAK_Z_SPEED_BASE * this->speed;
		if(this->zSpeed > nn::yb::e::global::FLY_JUMP_WEAK_Z_SPEED_MAX) {
			this->zSpeed = nn::yb::e::global::FLY_JUMP_WEAK_Z_SPEED_MAX;
		}
	}
}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyJumpStrong( nn::yb::def::HOLE_OBJ * obj) {

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
		return;
	}

	if(obj->x1 - 32 <= (int)this->x && (int)this->x <= obj->x1 + 32 &&
		obj->y1 - 32 <= (int)this->y && (int)this->y <= obj->y1 + 32
	){
		this->flyJumpStrongCheckFlag = true;
		if((!this->jumpStrongFlag) && this->flyGroundOnFlag && (!this->flyBoundFlag)){
			sound_nnsPlaySe( SE_PAPYOON);
			this->zSpeed = nn::yb::e::global::FLY_JUMP_STRONG_Z_SPEED;
			/*
			// 制限は面白くない
			if(this->speed > nn::yb::e::global::FLY_JUMP_STRONG_SPEED_MAX) {
				this->speed = nn::yb::e::global::FLY_JUMP_STRONG_SPEED_MAX;
			}
			*/
			this->jumpStrongFlag = true;
		}
	}
}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlySeFloor( nn::yb::def::HOLE_OBJ * obj) {

	if(this->flyGroundOnFlag && (!this->flyBoundFlag) &&
		obj->x1 - 32 <= (int)this->x && (int)this->x <= obj->x1 + 32 &&
		obj->y1 - 32 <= (int)this->y && (int)this->y <= obj->y1 + 32
	){
		////sound_nnsPlaySe( SE_PYON);
	}

}
//---------------------------------------------------------------------------

void nn::yb::main::CBall::calcBoundUnzan( nn::yb::def::HOLE_OBJ * obj) {

	nn::common::def::graphics2d::SEGMENT2_D after;
	int x = obj->x1;
	int y = obj->y1;
	if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE ||
		obj->category == nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA
	) {
		x = obj->dda.x;
		y = obj->dda.y;
	}
	after.s.x = x;
	after.s.y = y;
	after.v.x = this->x - x;
	after.v.y = this->y - y;
	int after_angle = (int)NN_COMMON_VEC2ANGLE2( after.v.x, after.v.y);
	if(after_angle < 0) {
		after_angle += 360;
	}
	after_angle = 360 - after_angle;
	after_angle = (after_angle + 2) / 5 * 5;
	if(after_angle >= 360) {
		after_angle -= 360;
	}
	this->angle = after_angle;
	
	this->angle = (double)after_angle;//ゆっくりの方向を設定

	this->speed = 4.0;
	if(this->z >= obj->z1 + nn::yb::e::global::UNZAN_Z_SHIFT + 2) {
		this->speed = 3.8;
		this->zSpeed = 6.0;
	}
	else if(this->z < obj->z1) {
		this->zSpeed = -4.0;
	}

}
//---------------------------------------------------------------------------

void nn::yb::main::CBall::calcBoundOku( nn::yb::def::HOLE_OBJ * obj) {

	nn::common::def::graphics2d::SEGMENT2_D after;
	after.s.x = obj->x1;
	after.s.y = obj->y1;
	after.v.x = this->x - obj->x1;
	after.v.y = this->y - obj->y1;
	int after_angle = (int)NN_COMMON_VEC2ANGLE2( after.v.x, after.v.y);
	if(after_angle < 0) {
		after_angle += 360;
	}
	after_angle = 360 - after_angle;
	after_angle = (after_angle + 2) / 5 * 5;
	if(after_angle >= 360) {
		after_angle -= 360;
	}
	this->angle = after_angle;
	
	this->angle = (double)after_angle;//ゆっくりの方向を設定

	this->speed = 4.0;
	if(this->z >= obj->z1 + nn::yb::e::global::OKU_Z_SHIFT + 2) {
		this->zSpeed = 6.5;
	}
	else if(this->z < obj->z1) {
		this->zSpeed = -4.0;
	}

}
//---------------------------------------------------------------------------

void nn::yb::main::CBall::defeatedOjama( nn::yb::def::HOLE_OBJ * obj) {
	//ヒットエフェクトを表示してお邪魔キャラを破壊
	sound_nnsPlaySe( SE_BOMB );// SE
	
	int x = obj->x1;
	int y = obj->y1;
	if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE ||
		obj->category == nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA
	) {
		x = obj->dda.x;
		y = obj->dda.y;
	}

	nn::yb::main::effect_frame_anime( this->holeCtrl, OBJ_ID_EXPLOSION_ITEM0_0, OBJ_ID_EXPLOSION_ITEM0_0 + 15, 
		x,
		y - 12 - obj->z1,
		nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ - 1, 
		0,
		0,
		160,0.0f,
		160,0.0f,
		0, 0.0f,
		28, 0.0f,
		DISP_COMMON_BLEND_MODE_ALPHA, this->holeCtrl->effect_ctrl, 60);//拡大

	//マップオブジェクトを消去
	for( int i = 0; i < this->holeCtrl->hole->data.holeObjN; i++) {
		nn::yb::def::HOLE_OBJ * objHole = &this->holeCtrl->hole->data.holeObj[i];
		if(objHole == obj) {
			this->holeCtrl->hole->data.holeObj[i].category = -1;
			this->holeCtrl->hole->data.holeObj[i].frameCnt = 0;
			this->holeCtrl->hole->data.holeObj[i].state = 0;
			this->holeCtrl->hole->data.holeObj[i].x1 = 0;
			this->holeCtrl->hole->data.holeObj[i].y1 = 0;
			this->holeCtrl->hole->data.holeObj[i].z1 = 0;
			this->holeCtrl->hole->data.holeObj[i].x2 = 0;
			this->holeCtrl->hole->data.holeObj[i].y2 = 0;
			this->holeCtrl->hole->data.holeObj[i].z2 = 0;
			break;
		}
	}
}
//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyUnzan( nn::yb::def::HOLE_OBJ * obj) {

	//ゆっくりの跳ね返り移動処理
	if(this->isHitUnzan(obj)) 
	{
		//メカゆっくり状態の場合
		if (this->ballForm == nn::yb::e::ball::BALL_FORM_MECHA)
		{
			this->defeatedOjama(obj);//お邪魔キャラを破壊
		}
		else
		{
			if (!this->bGenkotsuEffect)//2重ヒット防止
			{
				//グンニグルによるつぶれ状態の場合、グンニグルエフェクトを中断
				if (this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR)
				{
					this->releaseGungnir();
				}
				else if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE ||
					this->ballForm == nn::yb::e::ball::BALL_FORM_ORIN ||
					this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS
				) {
					this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
				}
				if(this->ballForm == nn::yb::e::ball::BALL_FORM_TORNADO) {
					sound_nnsPlaySe( SE_TECHUNUU);
				}
				else {
					//ダメージを受ける
					this->holeCtrl->stage->damage( -1);
					// ゆっくり表情
					this->setDispState( nn::yb::e::ball::DISP_STATE_SHOCK, 45);
				}

				this->calcBoundUnzan(obj);//跳ね返り処理

				////::nn_commonLog(1,  "angle = %f \n", angle);
				////::nn_commonLog(1,  "angle_deg = %f, \n", angle_deg);
				////::nn_commonLog(1,  "dx = %d, \n", dx);
				////::nn_commonLog(1,  "dy = %d, \n", dy);

				// SE
				sound_nnsPlaySe( SE_BAKOON);
				
				int x = obj->x1;
				int y = obj->y1;
				if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE ||
					obj->category == nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA
				) {
					x = obj->dda.x;
					y = obj->dda.y;
				}

				//反射エフェクト
				nn::common::def::graphics2d::SEGMENT2_D after;
				after.s.x = x;
				after.s.y = y;
				after.v.x = this->x - x;
				after.v.y = (this->y - this->z) - (y - obj->z1);
				int after_angle = (int)NN_COMMON_VEC2ANGLE2( after.v.x, after.v.y);
				if(after_angle < 0) {
					after_angle += 360;
				}
				after_angle = 360 - after_angle;

				const int ciLength = 60;
				int dx = static_cast<int>(nn::yb::common::yb_commonAngleSpeed2MoveX( after_angle, ciLength));
				int dy = static_cast<int>(nn::yb::common::yb_commonAngleSpeed2MoveY( after_angle, ciLength));

				const int ciAnimeFrame = 20;
				int iDispZ = nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ;//げんこつの表示Z値は、ゆっくりのY位置が雲山より上(負)の場合、雲山より優先度低。下(正)の場合、優先度高
				
#if 0
				// 隠れると見えない
				if (y < this->y || obj->z1 + nn::yb::e::global::UNZAN_Z_SHIFT < this->z)
				{
					iDispZ = nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ - 1;
				}
				else if (y >= this->y)
				{
					iDispZ = nn::yb::e::global::DISP_Z_BALL_UP_OBJ - 1;
				}
#else
				iDispZ = nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ - 1;
#endif

				this->holeCtrl->effect_ctrl->push_event( NULL, this, NULL);
				nn::yb::main::effect_start_end_total( this->holeCtrl, OBJ_ID_MAIN_OJAMA_1, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					ciAnimeFrame,
					x + nn::yb::e::global::UNZAN_X_SHIFT + (int)(dx * 0.5),
					x + nn::yb::e::global::UNZAN_X_SHIFT + (int)(dx * 1.2), ciAnimeFrame,
					y + nn::yb::e::global::UNZAN_Y_SHIFT - obj->z1 + (int)(dy * 0.5),
					y + nn::yb::e::global::UNZAN_Y_SHIFT - obj->z1 + (int)(dy * 1.2), ciAnimeFrame,
					iDispZ,
					47, 47, ciAnimeFrame,
					39, 39, ciAnimeFrame,
					-(int)(after_angle - 90.0) , -(int)(after_angle - 90.0) , ciAnimeFrame,
					42, 0, ciAnimeFrame,
					DISP_COMMON_BLEND_MODE_ALPHA, nn::yb::enum_effect::EFFECT_ANIME_FLAT, RGB_WHITE, this->holeCtrl->effect_ctrl);//拡大
				////this->ctrl_cnt++;

				this->bGenkotsuEffect = true;//げんこつエフェクトフラグを生成したのでフラグを立てる
			}
		}
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyOku( nn::yb::def::HOLE_OBJ * obj) {

	//ゆっくりの跳ね返り移動処理
	if(this->isHitOku(obj))
	{
		//メカゆっくり状態の場合
		if (this->ballForm == nn::yb::e::ball::BALL_FORM_MECHA)
		{
			this->defeatedOjama(obj);//お邪魔キャラを破壊
		}
		else
		{
			if (!this->bFlareEffect)//2重ヒット防止
			{
				//グンニグルによるつぶれ状態の場合、グンニグルエフェクトを中断
				if (this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR)
				{
					this->releaseGungnir();
				}
				else if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE ||
					this->ballForm == nn::yb::e::ball::BALL_FORM_ORIN ||
					this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS
				) {
					this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
				}

				if(this->ballForm == nn::yb::e::ball::BALL_FORM_TORNADO) {
					sound_nnsPlaySe( SE_TECHUNUU);
				}
				else {
					//ダメージを受ける
					this->holeCtrl->stage->damage( -1);
					// ゆっくり表情
					this->setDispState( nn::yb::e::ball::DISP_STATE_SHOCK, 45);
				}

				this->calcBoundOku(obj);//本体との跳ね返り処理
				////::nn_commonLog(1,  "angle = %f \n", angle);
				////::nn_commonLog(1,  "angle_deg = %f, \n", angle_deg);
				////::nn_commonLog(1,  "dx = %d, \n", dx);
				////::nn_commonLog(1,  "dy = %d, \n", dy);

				// SE
				sound_nnsPlaySe( SE_BAGON);
				sound_nnsPlaySe( SE_KOOOOOO);

				//反射フレアエフェクト
				const int ciLength = 80;
				const int ciAnimeFrame = 25;
				int iDispZ = nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ - 1;
				
				this->holeCtrl->effect_ctrl->push_event( NULL, this, NULL);
				nn::yb::main::effect_start_end_total( this->holeCtrl, OBJ_ID_COMMON_EFFECT_FLARE_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
						ciAnimeFrame,
						obj->x1 , obj->x1, ciAnimeFrame,
						obj->y1  - obj->z1, obj->y1  - obj->z1, ciAnimeFrame,
						iDispZ,
						180, 380, ciAnimeFrame,
						180, 380, ciAnimeFrame,
						0, 0, ciAnimeFrame,
						42, 0, ciAnimeFrame,
						DISP_COMMON_BLEND_MODE_ADD, nn::yb::enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, this->holeCtrl->effect_ctrl);//拡大

				this->bFlareEffect = true;//フレアエフェクトフラグを生成したのでフラグを立てる
			}
		}
	}

	//弾エフェクトとの跳ね返り処理
	if(this->isHitOkuBullet(obj)) 
	{
		if (obj->state == nn::yb::e::hole::OKU_STATE_ATTACK)
		{
			if (!this->bBulletHitEffect)//2重ヒット防止
			{
				if (this->ballForm == nn::yb::e::ball::BALL_FORM_TORNADO)
				{
					//竜巻エフェクト削除
					if (this->m_pCurrSpellEffect != NULL)
					{
						if (this->isLive(this->m_pCurrSpellEffect))
						{
							this->m_pCurrSpellEffect->ctrl->stopEffect();
						}
					}
				}
				
				if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE ||
					this->ballForm == nn::yb::e::ball::BALL_FORM_ORIN ||
					this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS ||
					this->ballForm == nn::yb::e::ball::BALL_FORM_TORNADO
				) {
					this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
				}

				nn::common::def::graphics2d::SEGMENT2_D after;
				after.s.x = obj->x1;
				after.s.y = obj->y1;
				after.v.x = this->x - obj->x1;
				after.v.y = (this->y - this->z) - obj->y1;
				int after_angle = (int)NN_COMMON_VEC2ANGLE2( after.v.x, after.v.y);
				if(after_angle < 0) {
					after_angle += 360;
				}
				after_angle = 360 - after_angle;
				this->angle = after_angle;
				
				this->angle = (double)after_angle;//ゆっくりの方向を設定

				this->speed = 4.5;
				this->zSpeed = -8.0;
				
				//ダメージ2を受ける
				this->holeCtrl->stage->damage( -2);

				//ヒットエフェクト生成
				sound_nnsPlaySe( SE_GYA);
				// effect
				const int ciAnimeFrame = 30;
				int iDispZ = nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ - 1;//表示Z値は、ゆっくりのY位置が雲山より上(負)の場合、雲山より優先度低。下(正)の場合、優先度高
				
				this->holeCtrl->effect_ctrl->push_event( NULL, this, NULL);
				nn::yb::main::effect_start_end_total( this->holeCtrl, OBJ_ID_DUN_SPELL_HIT_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
						ciAnimeFrame,
						(int)this->x , (int)this->x, ciAnimeFrame,
						(int)(this->y  - this->z), (int)(this->y  - this->z), ciAnimeFrame,
						iDispZ,
						32, 360, ciAnimeFrame,
						32, 360, ciAnimeFrame,
						0, 0, ciAnimeFrame,
						42, 0, ciAnimeFrame,
						DISP_COMMON_BLEND_MODE_ADD, nn::yb::enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, this->holeCtrl->effect_ctrl);//拡大

				this->bBulletHitEffect = true;//エフェクト生成したのでフラグを立てる
			}
		}
	}

}

//---------------------------------------------------------------------------

bool nn::yb::main::CBall::isHitUnzan(nn::yb::def::HOLE_OBJ * obj) {
	
	int x = obj->x1;
	int y = obj->y1;
	if(obj->category == nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE ||
		obj->category == nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA
	) {
		x = obj->dda.x;
		y = obj->dda.y;
	}

	double vx_enemy2ball = (x + nn::yb::e::global::UNZAN_X_SHIFT) - this->x;
	double vy_enemy2ball = (y + nn::yb::e::global::UNZAN_Y_SHIFT) - this->y;
	double vz_enemy2ball = (obj->z1 + nn::yb::e::global::UNZAN_Z_SHIFT) - this->z;

	double len_enemy2ball_pow2 = NN_COMMON_POW2( vx_enemy2ball) +
		NN_COMMON_POW2( vy_enemy2ball) + NN_COMMON_POW2( vz_enemy2ball);

	if(len_enemy2ball_pow2 <=
		NN_COMMON_POW2(nn::yb::e::global::UNZAN_RADIUS))
	{
		return true;
	}
	return false;

}

//---------------------------------------------------------------------------

bool nn::yb::main::CBall::isHitWarp1(nn::yb::def::HOLE_OBJ * obj) {
	
	if(obj->x1 - nn::yb::e::global::HOLE_WARP_HALF_W <= (int)this->x &&
		(int)this->x <= obj->x1 + nn::yb::e::global::HOLE_WARP_HALF_W &&
		obj->y1 - nn::yb::e::global::HOLE_WARP_HALF_H <= (int)this->y &&
		(int)this->y <= obj->y1 + nn::yb::e::global::HOLE_WARP_HALF_H
	){
		return true;
	}
	return false;

}

//---------------------------------------------------------------------------

bool nn::yb::main::CBall::isHitWarp2(nn::yb::def::HOLE_OBJ * obj) {
	
	if(obj->x2 - nn::yb::e::global::HOLE_WARP_HALF_W <= (int)this->x &&
		(int)this->x <= obj->x2 + nn::yb::e::global::HOLE_WARP_HALF_W &&
		obj->y2 - nn::yb::e::global::HOLE_WARP_HALF_H <= (int)this->y &&
		(int)this->y <= obj->y2 + nn::yb::e::global::HOLE_WARP_HALF_H
	){
		return true;
	}
	return false;

}

//---------------------------------------------------------------------------

bool nn::yb::main::CBall::isHitOku(nn::yb::def::HOLE_OBJ * obj) {

	double vx_enemy2ball = obj->x1 - this->x;
	double vy_enemy2ball = obj->y1 - this->y;
	double vz_enemy2ball = (obj->z1 + nn::yb::e::global::UNZAN_Z_SHIFT) - this->z;

	double len_enemy2ball_pow2 = NN_COMMON_POW2( vx_enemy2ball) +
		NN_COMMON_POW2( vy_enemy2ball) + NN_COMMON_POW2( vz_enemy2ball);

	if(len_enemy2ball_pow2 <=
		NN_COMMON_POW2(nn::yb::e::global::OKU_RADIUS)) 
	{
		return true;
	}
	return false;

}

//---------------------------------------------------------------------------

bool nn::yb::main::CBall::isHitOkuBullet(nn::yb::def::HOLE_OBJ *obj) {

	double vx_enemy2ball = obj->x1 - this->x;
	double vy_enemy2ball = obj->y1 - this->y;

	double len_enemy2ball_pow2 = NN_COMMON_POW2( vx_enemy2ball) +
		NN_COMMON_POW2( vy_enemy2ball);

	if(len_enemy2ball_pow2 <=
		NN_COMMON_POW2(nn::yb::e::global::OKU_BULLET_RADIUS) &&
		0.0 <= this->z && this->z < obj->z1) 
	{
		return true;
	}
	return false;

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlyCheckYakumono() {

	nn::yb::stage::CHole * hole =  this->holeCtrl->hole;
	bool bIsOutOfDCUnzan = false;
	bool bIsOutOfDCOku = false;
	bool bIsOutOfDCOkuBullet = false;
	
	for( int i = 0; i < hole->data.holeObjN; i++) {

		nn::yb::def::HOLE_OBJ * obj = &hole->data.holeObj[i];
		switch(obj->category) {
			case nn::yb::e::edit_hole_obj::CATEGORY_BUNKER:
				{
					if(obj->x1 <= (int)this->x && (int)this->x <= obj->x2 &&
						obj->y1 <= (int)this->y && (int)this->y <= obj->y2
					){
						this->flyCheckBunkerOnFlag = true;
					}
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP:
			case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT:
			case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN:
			case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT:
				{
					if(obj->x1 <= (int)this->x && (int)this->x <= obj->x2 &&
						obj->y1 <= (int)this->y && (int)this->y <= obj->y2
					){
						this->flyCheckConveyorFlag = true;
					}
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN:
				{
					if(this->isHitUnzan(obj)) 
					{
						bIsOutOfDCUnzan = true;//ゆっくりが一つでも雲山の当たり判定に入っている場合
					}
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
				{
					if(this->isHitOku(obj))
					{
						bIsOutOfDCOku = true;
					}
					if(this->isHitOkuBullet(obj))
					{
						bIsOutOfDCOkuBullet = true;
					}
				}
				break;
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
				{
					if(this->flyWarpOnFlag == false &&
						this->isHitWarp1( obj) || this->isHitWarp2( obj)
					) {
						this->flyWarpOnFlag = true;
					}
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_START:
				{
					if(obj->x1 <= (int)this->x && (int)this->x <= obj->x2 &&
						obj->y1 <= (int)this->y && (int)this->y <= obj->y2
					){
						if(this->holeCtrl->timeAttackState == nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_NONE) {
							this->holeCtrl->timeAttackSumTime = 0;
							this->holeCtrl->timeAttackOldTime = nn_timerGetMilliTime();
							this->holeCtrl->timeAttackState = nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_ACTIVE;
						}
					}
				}
				break;
			case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_GOAL:
				{
					if(obj->x1 <= (int)this->x && (int)this->x <= obj->x2 &&
						obj->y1 <= (int)this->y && (int)this->y <= obj->y2
					){
						if(this->holeCtrl->hole->enemyN == 0) {
							if(this->holeCtrl->timeAttackState != nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_NONE) {
								this->holeCtrl->timeAttackState =  nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_END;
							}
						}
					}
				}
				break;
		}

	}

	if (!bIsOutOfDCUnzan)
		this->bGenkotsuEffect = false;//全ての雲山の当たり判定からゆっくりが範囲外に出た場合は、エフェクトフラグをリセット
	if (!bIsOutOfDCOku)
		this->bFlareEffect = false;//全てのお空の当たり判定からゆっくりが範囲外に出た場合は、エフェクトフラグをリセット
	if (!bIsOutOfDCOkuBullet)
		this->bBulletHitEffect = false;//全てのお空の弾の当たり判定からゆっくりが範囲外に出た場合は、エフェクトフラグをリセット

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::taskFlying() {

	nn::yb::stage::CHole * hole =  this->holeCtrl->hole;

	::KEY_WORK * key = &this->master->keyWork;

	this->flyProcFlag = false;
	this->flyInputProcFlag = false;
	this->flyGroundOnFlag = true;
	this->flyStopFlag = false;
	this->flySlopeOnFlag = false;
	this->flyWarpOnFlag = false;
	this->flyCupOnFlag = false;
	this->flyWarpExecFlag = false;
	this->flySkyCupinFlag = false;
	this->flyCupOnDispDownFlag = false;
	this->flyFukiageCheckFlag = false;
	this->flyJumpCheckFlag = false;
	this->flyJumpStrongCheckFlag = false;
	this->flyCheckBunkerOnFlag = false;
	this->flyGravityCheckFlag = false;
	this->flyCheckConveyorFlag = false;

	// 停止しなかった時の保険

	int limit_time1 = 60 * 45;
	int limit_time2 = 60 * 60;
	if(this->holeCtrl->timeAttackState != nn::yb::e::hole_ctrl::TIME_ATTACK_STATE_NONE) {
		limit_time1 *= limit_time1;
		limit_time2 *= limit_time2;
	}
	if(this->flyFrameCnt == limit_time1) {
		this->spellCard( nn::yb::e::global::SPELL_CARD_SPEA);
		this->shotTimeOverFlag = true;
		this->flyProcFlag = true;
		this->flyInputProcFlag = true;
	}
	if(this->flyFrameCnt >= limit_time2) {
		this->speed *= 0.5;
		this->zSpeed *= 0.5;
	}
	this->flyFrameCnt++;

	// 速度超過の保険
	if(this->speed >= 40.0) {
		this->speed = 0.0;
		this->zSpeed = 0.0;
		sound_nnsPlaySe( SE_AAAAA);
		if(this->holeCtrl->hole->cup != NULL) {
			this->x = this->holeCtrl->hole->cup->x;
			this->y = this->holeCtrl->hole->cup->y;
			this->z = 300.0;
		}
		else {
			nn::yb::main::CEnemy * enemy = NULL;
			for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE(this->holeCtrl->hole->enemy); i++) {

				if(this->holeCtrl->hole->enemy[i] != NULL) {
					enemy = this->holeCtrl->hole->enemy[i];
					break;
				}

			}
			if(enemy != NULL) {
				this->x = enemy->x;
				this->y = enemy->y;
				this->z = 300.0;
			}
		}
	}

	// 全ヤクモノを調べてflagを立てたりしておく
	this->taskFlyCheckYakumono();

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS) {
		this->flyBallFormPressFrameCnt++;
	}

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
		if(this->x < this->holeCtrl->hole->startX ||  this->holeCtrl->hole->endX < this->x ||
			this->y< this->holeCtrl->hole->startY ||  this->holeCtrl->hole->endY  < this->y
		){
			this->releaseFlan();
		}
		else if(this->flyVampireRemainingFrame == 0) {
			this->releaseFlan();
		}
		else {
			this->flyVampireRemainingFrame--;
		}
	}

	if(this->flyDashSeWaitCnt > 0) {
		this->flyDashSeWaitCnt--;
	}

	// 早い早い
	if(this->speed >= 14.0) {
		nn_commonLog( 999, "早い早い\n");
		if( !this->flyFastSeFlag ) {
			if(this->flyFastSeCnt >= 25) {
				nn_commonLog( 999, "早い早い2\n");
				sound_nnsPlaySe( SE_HAYAITTE);
				this->setDispState( nn::yb::e::ball::DISP_SATTE_DIZZY, 120);
				this->flyFastSeCnt = 0;
				this->flyFastSeFlag = true;
			}
			else {
				this->flyFastSeCnt++;
			}
		}
	}
	else {
		this->flyFastSeFlag = false;
	}

	::nn_commonLog( 120829, "ball->z = %lf\n", this->z);
	if(this->z < -32.0) {
		nn_commonLog( 999, "落ちる\n");
		this->setDispState( nn::yb::e::ball::DISP_SATTE_DIZZY);
	}
	if(this->z > 210.0) {
		nn_commonLog( 999, "高い\n");
		this->setDispState( nn::yb::e::ball::DISP_SATTE_DIZZY);
	}

	// マスタースパークで吹き飛ばされた後、しばらくは空中扱い
	if(this->ballForm == nn::yb::e::ball::BALL_FORM_BURN) {
#if 1
		if(this->speed <= nn::yb::e::global::FLYING_MSPARK_RELEASE_SPEED) {
#else
		// フレーム制限はイマイチ面白くない
		if(this->speed <= nn::yb::e::global::FLYING_MSPARK_RELEASE_SPEED ||
			this->flyMspaFrameCnt >= 60 * 15
		) {
			this->flyMspaFrameCnt = 0;
#endif
			this->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
			// 急減速
			if(this->speed > nn::yb::e::global::FLYING_MSPARK_RELEASE_SPEED * nn::yb::e::global::FLYING_MSPARK_RELEASE_SPEED_DEC_RATE) {
				this->speed *= nn::yb::e::global::FLYING_MSPARK_RELEASE_SPEED_DEC_RATE;
			}
		}
		else {
			this->flyMspaFrameCnt++;
			this->speed -= nn::yb::e::global::FIELD_DEC_SPEED_BASE_FLY;
		}
	}
	else if(this->ballForm == nn::yb::e::ball::BALL_FORM_ORIN) {
		if(this->speed < 15.0) {
			this->speed += 0.5;
		}
		else {
			this->speed = 16.0;
		}
	}
	
	// 坂
	if(this->slopeCnt < 100) {
		this->slopeCnt++;
	}

	// がんばりボタン
	if(this->goodLuckUpCnt > 0) {
		this->goodLuckUpCnt--;
	}
	if(this->goodLuckUpCntBound > 0) {
		this->goodLuckUpCntBound--;
	}

	// がんばりボタン
	if( !this->flyProcFlag && !this->flyInputProcFlag ) {
		if( this->taskInputGoodLuck() ) {
			this->flyInputProcFlag = true;
		}
	}
	// スペル
	if( !this->flyProcFlag && !this->flyInputProcFlag ) {
		if( this->taskInputSpell() ) {
			this->flyProcFlag = true;
			this->flyInputProcFlag = true;
		}
	}

	// ゆっくり変形時の特殊処理
	if( !this->flyProcFlag && !this->flyInputProcFlag ) {
		if( this->taskInputBallForm() ) {
			this->flyProcFlag = true;
			this->flyInputProcFlag = true;
		}
	}

	// 地面上かの判定
	if( !this->flyProcFlag && this->flyGroundOnFlag) {
		this->taskFlySky();
	}

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_MECHA && this->flyGroundOnFlag) {
		if(this->flyFrameCnt % 32 == 0) {
			sound_nnsPlaySe( SE_GASHAN);
		}
	}

	// スペカ向き操作
	if( !this->flyProcFlag && !this->flyInputProcFlag ) {
		switch(this->ballForm) {
			case nn::yb::e::ball::BALL_FORM_PRESS:
				{
					if(key->press & nn::common::e::PAD_LEFT) {
						this->add_angle( 2.0);
						if(this->z > 0.0) {
							if(this->speed < 0.600) {
								this->speed = 0.600;
							}
							this->speed += 0.014;
						}
					}
					else if(key->press & nn::common::e::PAD_RIGHT) {
						this->add_angle( -2.0);
						if(this->z > 0.0) {
							if(this->speed < 0.600) {
								this->speed = 0.600;
							}
							this->speed += 0.014;
						}
					}
				}
				break;
			case nn::yb::e::ball::BALL_FORM_TORNADO:
				{
					if(this->flyGroundOnFlag) {
						if(key->press & nn::common::e::PAD_LEFT) {
							this->add_angle( nn::yb::e::global::SPELL_MOMIJI_CURVE);
						}
						else if(key->press & nn::common::e::PAD_RIGHT) {
							this->add_angle( -nn::yb::e::global::SPELL_MOMIJI_CURVE);
						}
					}
				}
				break;
		}
	}
	
	// ボールとカップの当たり判定関係
	if( !this->flyProcFlag ) {
		this->taskFlyCup();
	}

	// 回転処理
	if( !this->flyProcFlag ) {
		this->taskFlySpin();
	}

	// 事前計算
	double add_x = nn::yb::common::yb_commonAngleSpeed2MoveX( this->angle, this->speed);
	double add_y = nn::yb::common::yb_commonAngleSpeed2MoveY( this->angle, this->speed);

	this->flyMove->s.x = this->x;
	this->flyMove->s.y = this->y;
	this->flyMove->v.x = add_x;
	this->flyMove->v.y = add_y;

	this->taskFlyBumper();
	
	if( !this->flyProcFlag ) {
		for( int i = 0; i < hole->data.holeObjN; i++) {

			nn::yb::def::HOLE_OBJ * obj = &hole->data.holeObj[i];
			switch(obj->category) {
				case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
					// taskFlyBumper()で処理
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_GROUND:
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP:
					this->taskFlySlope( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_PILLAR:
					this->taskFlyPillar( obj);
					break;
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
					this->taskFlyWarp( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_LEFT:
					this->taskFlyKick( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_LEFT:
				case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_RIGHT:
					this->taskFlyDash( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_JUMP:
					this->taskFlyJump( obj);
					break;
					
				case nn::yb::e::edit_hole_obj::CATEGORY_DAMAGE:
					this->taskFlyDamage( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_BUNKER:
					this->taskFlyBunker( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_GRAVITY:
					this->taskFlyGravity( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN:
				case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_REMIFLA:
				case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE:
				case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA:
					this->taskFlyUnzan( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
					this->taskFlyOku( obj);
					break;
					
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP:
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT:
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN:
				case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT:
					this->taskFlyConveyor( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_FUKIAGE:
					this->taskFlyFukiage( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_WEAK:
					this->taskFlyJumpWeak( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_STRONG:
					this->taskFlyJumpStrong( obj);
					break;
				case nn::yb::e::edit_hole_obj::CATEGORY_SE_FLOOR:
					this->taskFlySeFloor( obj);
					break;
			}
		}
	}

	if(this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR) {
		if(this->flyBoundFlag) {
			if(this->flySlopeOnFlag) {
				// グングニルで着地した場所が坂
				if(this->flyWarpOnFlag == false) {
					// ワープ後の場所にエフェクトが出る
					this->spellGungnirHitEffect();
				}
			}
			else {
				// グングニルで着地した場所が地面
				this->releaseGungnir();

				if(this->flyWarpOnFlag == false) {
					// ワープ後の場所にエフェクトが出る
					this->spellGungnirHitEffect();
					::direct_commonWaitSimple( NULL,
						110, nn::yb::main::CBall::spellEffectEndGungnirHitCb, this
					);
					this->state = nn::yb::e::ball::STATE_SPELL;
				}
			}
		}
		else if(this->flyGroundOnFlag && this->flySlopeOnFlag == false) {
			// グングニルで坂から地面に出た瞬間
			this->speed = 0.0;
			this->releaseGungnir();
			if(this->flyWarpOnFlag == false) {
				// ワープ後の場所にエフェクトが出る
				this->spellGungnirHitEffect();
				::direct_commonWaitSimple( NULL,
					110, nn::yb::main::CBall::spellEffectEndGungnirHitCb, this
				);
				this->state = nn::yb::e::ball::STATE_SPELL;
			}
		}
		else {
			if(this->flySlopeOnFlag) {
			}
			else {
				this->speed = 0.0;
				this->zSpeed = -20.0;
			}
		}
	}

	// ワープ板の上から出れば、もう一度ワープ出来る
	if( !this->flyWarpOnFlag ) {
		this->warpFlag = false;
	}

	// SE制御のため
	if( !this->flyFukiageCheckFlag) {
		this->flyFukiageCnt = 0;
	}
	if( !this->flyGravityCheckFlag) {
		this->flyGravityCnt = 0;
	}

	// ジャンプ版の上から出ればもう一度ジャンプ出来る
	if( !this->flyJumpCheckFlag) {
		this->jumpFlag = false;
	}
	if( !this->flyJumpStrongCheckFlag) {
		this->jumpStrongFlag = false;
	}

	// 落下	
	if( !this->flyProcFlag ) {
		if( this->taskFlyFall() ) {
			this->flyProcFlag = true;
		}
	}

	// 移動、停止判定
	if( !this->flyProcFlag ) {
		if( this->taskFlyMove() ) {
			this->flyStopFlag = true;
		}
	}

	// カップイン判定
	if( !this->flyProcFlag ) {
		this->taskFlyCupin();
	}

	// がんばりボタンエフェクトの位置更新
	if(this->goodLuckAnimeTask != NULL) {
		ANIMATION * good_luck_anime_work = (ANIMATION *)this->goodLuckAnimeTask->work;
		good_luck_anime_work->x = (int)(this->x - 320);
		good_luck_anime_work->y = (int)(this->y - this->z - 250);
	}

	// カメラ更新
	if(!this->flyWarpExecFlag) {
		if(this->z >= -512.0) {
			int camera_x = (int)this->x;
			int camera_y = (int)(this->y - this->z);
			if(camera_x < this->holeCtrl->hole->startX) {
				camera_x = this->holeCtrl->hole->startX;
			}
			if(camera_y < this->holeCtrl->hole->startY) {
				camera_y = this->holeCtrl->hole->startY;
			}
			if(camera_x > this->holeCtrl->hole->endX) {
				camera_x = this->holeCtrl->hole->endX;
			}
			if(camera_y > this->holeCtrl->hole->endY) {
				camera_y = this->holeCtrl->hole->endY;
			}
			this->holeCtrl->ctrl->camera->setCameraPos( camera_x, camera_y);
		}
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::proc_wait_cnt_dec( void * ballArg) {

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;
	ball->procWaitCnt--;

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::hit_cup() {

	this->speed *= nn::yb::e::global::FLYING_HIT_CUP_DEC_RATE;
	this->flySkipCnt = nn::yb::e::global::FLYING_HIT_CUP_SKIP_CNT;
	this->cupHitState = 1;

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::shot() {

	sound_nnsPlaySe( SE_DON);

	this->oldX = this->x;
	this->oldY = this->y;
	this->flyBoundCnt = 0;
	this->flySlopeStopCnt = 0;
	this->moveStopCnt = 0;
	this->flyDashStandbyCnt = 0;
	this->flyFastSeFlag = false;
	this->flyFastSeCnt = 0;
	this->spellCardEnableFlag = true;
	this->dispSpellCardEnableFlag = true;
	this->flyFrameCnt = 0;
	// とりあえず壁状の柱へのめり込みは解消しない
	//this->flyPillarHitFlag = false;
	this->flyFukiageCheckFlag = false;
	this->flyFukiageCnt = 0;
	this->flyDashSeWaitCnt = 0;
	this->flyGravityCheckFlag = false;
	this->flyGravityCnt = 0;
	this->flyMspaFrameCnt = 0;
	this->shotTimeOverFlag = false;

	nn::yb::common::CALC_SHOT_DATA data;
	memset( &data, 0, sizeof( nn::yb::common::CALC_SHOT_DATA));
	data.zSpeed = &this->zSpeed;
	data.rotHorizon = &this->rotHorizon;
	data.rotVertical = &this->rotVertical;
	data.speed = &this->speed;
	data.powerState = this->powerState;
	data.flyState = this->flyState;
	data.cursorSpinHrizon = this->holeCtrl->cursorRotHrizon;
	data.power = this->power;
	data.stopSpinVertical = this->holeCtrl->stopSpinVertical;
	nn::yb::common::yb_commonCalcShot( &data);

	this->power = 0.0;
	this->holeCtrl->cursorRotHrizon = 0;
	this->holeCtrl->cursorRotVertical = 0;
	this->holeCtrl->stopSpinVertical = 0;

	this->holeCtrl->shotCnt++;
	/*
	// 難易度調整
	if(this->holeCtrl->shotCnt >= 2) {
		this->holeCtrl->stage->damage( -1);
	}
	*/
	this->holeCtrl->stage->damage( -1);

	this->state = nn::yb::e::ball::STATE_FLYING;

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::add_angle( double addAngle) {

	this->angle += addAngle;
	if(this->angle >= 360.0) {
		this->angle -= 360.0;
	}
	if(this->angle < 0.0) {
		this->angle += 360.0;
	}
	if(this->angle < 0.0 || 360.0 <= this->angle) {
		printf( "add_angle error\n");
		this->angle = 0;
		this->speed = 0.0;
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::disp() {

	nn::yb::stage::CHole * hole =  this->holeCtrl->hole;
	
	// ボール
	int index_add = (359 - ((int)this->angle + 359 - 112) % 360) / 45;
	int z = nn::yb::e::global::DISP_Z_BALL;
	if((int)this->z < -this->r) {
		z = nn::yb::e::global::DISP_Z_FALL_BALL;
	}
	if(this->state == nn::yb::e::ball::STATE_SPELL || this->musoFlag) {
		z = nn::yb::e::global::DISP_Z_SPELL_BALL;
	}
	if((this->state == nn::yb::e::ball::STATE_CUPIN || this->state == nn::yb::e::ball::STATE_CUPIN_DISP)
		&& this->y + this->cupinDispCnt >= hole->cup->y + 12
	) {
		disp_commonMain3dMask( TEXTURE_ID_MASK, hole->cup->x - 32, hole->cup->y - 32, nn::yb::e::global::DISP_Z_BALL_MASK);
		disp_commonMain3dMaskRelease( nn::yb::e::global::DISP_Z_BALL_MASK_RELEASE);
	}

	// カップマスク描画なし
	if(this->holeCtrl->hole->cup != NULL) {
		if(this->holeCtrl->stage->saveData->configData.dispMask == nn::yb::e::config::SET_DISP_MASK_ITEM_DISABLE) {
			OBJ * obj_cup_frame = disp_commonGetObj( OBJ_ID_MAIN_CUP_FRAME_0);
			if((this->state == nn::yb::e::ball::STATE_CUPIN || this->state == nn::yb::e::ball::STATE_CUPIN_DISP)
				&& this->y + this->cupinDispCnt >= hole->cup->y + 12
			) {
				disp_commonObjCenterBasic( OBJ_ID_MAIN_CUP_FRAME_0, this->holeCtrl->hole->cup->x + 2, this->holeCtrl->hole->cup->y - 3 - 7, nn::yb::e::global::DISP_Z_CUP);
				disp_commonObjCenterBasic( OBJ_ID_MAIN_CUP_FRAME_1, this->holeCtrl->hole->cup->x + 2, this->holeCtrl->hole->cup->y - 3 - 7 + obj_cup_frame->h, nn::yb::e::global::DISP_Z_BALL_MASK_RELEASE);
			}
			else {
				disp_commonObjCenterBasic( OBJ_ID_MAIN_CUP_FRAME_0, this->holeCtrl->hole->cup->x + 2, this->holeCtrl->hole->cup->y - 3 - 7, nn::yb::e::global::DISP_Z_CUP);
				disp_commonObjCenterBasic( OBJ_ID_MAIN_CUP_FRAME_1, this->holeCtrl->hole->cup->x + 2, this->holeCtrl->hole->cup->y - 3 - 7 + obj_cup_frame->h, nn::yb::e::global::DISP_Z_CUP);
			}
		}
	}	

	int add_y = 0;
	if(this->flyCupOnDispDownFlag) {
		add_y = 2;
	}

	bool burn_flag = false;
	bool press_flag = false;
	int obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_0;
	if(this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS) {
		obj_id_reimu = 0;
	}
	else if(this->ballForm == nn::yb::e::ball::BALL_FORM_ORIN) {
		obj_id_reimu = 0;
	}
	else if(this->ballForm == nn::yb::e::ball::BALL_FORM_MECHA) {
		int index_add = (359 - ((int)this->angle + 359 - 112) % 360) / 90;//メカゆっくりは90度単位で4方向
		obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_MECHA_0 + index_add;
	}
	else if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
		int index_add = (359 - ((int)this->angle + 359 - 112) % 360) / 90;//フランゆっくりは90度単位で4方向
		obj_id_reimu = OBJ_ID_MAIN_YUKKURI_FLAN_0 + index_add;
	}
	else {
		switch(this->dispState) {
			case nn::yb::e::ball::DISP_STATE_DEFAULT:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_0 + index_add;
				break;
			case nn::yb::e::ball::DISP_STATE_DEAD:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_GAAN_4;
				break;
			case nn::yb::e::ball::DISP_STATE_NORMAL_FRONT:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_4;
				break;
			case nn::yb::e::ball::DISP_STATE_CRY:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_NAKI_0 + index_add;
				break;
			case nn::yb::e::ball::DISP_STATE_DOYA:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_TERE_4;
				break;
			case nn::yb::e::ball::DISP_STATE_JOY:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_TERE_0 + index_add;
				break;
			case nn::yb::e::ball::DISP_SATTE_DIZZY:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_GURUGURU_0 + index_add;
				break;
			case nn::yb::e::ball::DISP_STATE_SHOCK:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_GAAN_4;
				break;
			case nn::yb::e::ball::DISP_SATTE_DAMAGE:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_GAAN_0 + index_add;
				break;
			case nn::yb::e::ball::DISP_STATE_PRESS:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_GAAN_4;
				press_flag = true;
				break;
			case nn::yb::e::ball::DISP_STATE_BURN:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_GAAN_4;
				burn_flag = true;
				break;
			case nn::yb::e::ball::DISP_SATTE_PERA:
				obj_id_reimu = OBJ_ID_MAIN_YUKKURI_REIMU_PRESS_0;
				break;
			case nn::yb::e::ball::DISP_STATE_MUSO:
			case nn::yb::e::ball::DISP_STATE_CLASH:
				obj_id_reimu = 0;
				return;
		}
	}
	
	if( obj_id_reimu != 0) {
		if(press_flag) {
			disp_commonObjCenter( obj_id_reimu, (int)this->x, (int)this->y - 16 - (int)this->z + this->cupinDispCnt + add_y + 24, 96, 8, z, 0, FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT);
		}
		else if(burn_flag) {
			disp_commonObjCenter( obj_id_reimu, (int)this->x, (int)this->y - 16 - (int)this->z + this->cupinDispCnt + add_y, 0, 0, z, 0, FALSE, FALSE, GX_RGB( 6, 4, 4), ALPHA_DEFAULT);
		}
		else {
			disp_commonObjCenterBasic( obj_id_reimu, (int)this->x, (int)this->y - 16 - (int)this->z + this->cupinDispCnt + add_y, z);
		}
	}
	if(this->ballForm == nn::yb::e::ball::BALL_FORM_PRESS) {
		int frame_cnt = this->flyBallFormPressFrameCnt % animationGetFrame( ANIMATION_ID_YUKKURI_PRESS);
		ANIMATION_DISP_DATA add;
		memset( &add, 0, sizeof( ANIMATION_DISP_DATA));
		add.x = (int)this->x;
		add.y = (int)this->y - 16 - (int)this->z + this->cupinDispCnt + add_y;
		add.wr = 10000;
		add.hr = 10000;
		add.z = z;
		add.angle = 0;
		add.flipH = FALSE;
		add.flipV = FALSE;
		add.r = 31;
		add.g = 31;
		add.b = 31;
		add.a = 31;
		animationDisp( ANIMATION_ID_YUKKURI_PRESS, &add, frame_cnt, FALSE);
	}
	else if(this->ballForm == nn::yb::e::ball::BALL_FORM_ORIN) {
		int x = (int)this->x;
		int y = (int)this->y - 28 - (int)this->z + this->cupinDispCnt + add_y;
		int vec = (nn::yb::e::orin::VEC)(((360 - (int)this->angle) + 135) % 360 / 90);
		int obj_id_tbl[] = {
			OBJ_ID_MAIN_YUKKURI_REIMU_0,	// nn::yb::e::orin::VEC_UP
			OBJ_ID_MAIN_YUKKURI_REIMU_2,	// nn::yb::e::orin::VEC_RIGHT
			OBJ_ID_MAIN_YUKKURI_REIMU_4,	// nn::yb::e::orin::VEC_DOWN
			OBJ_ID_MAIN_YUKKURI_REIMU_6,	// nn::yb::e::orin::VEC_LEFT
		};
		disp_commonObjCenterBasic( obj_id_tbl[vec], x + 2, y + 4, z);
		
		int layhout_id_kasha_tbl[] = {
			LAYOUT_ID_ORIN_UP_KASHA,	// nn::yb::e::orin::VEC_UP
			LAYOUT_ID_ORIN_RIGHT_KASHA,	// nn::yb::e::orin::VEC_RIGHT
			LAYOUT_ID_ORIN_DOWN_KASHA,	// nn::yb::e::orin::VEC_DOWN
			LAYOUT_ID_ORIN_LEFT_KASHA,	// nn::yb::e::orin::VEC_LEFT
		};
		LAYOUT * layout_kasha =  disp_commonGetLayoutMain3d( layhout_id_kasha_tbl[vec]);
		disp_commonObjCenterBasic( layout_kasha->obj_id,
			x + layout_kasha->x, y + layout_kasha->y, z + layout_kasha->z
		);
	}
	else if(this->ballForm == nn::yb::e::ball::BALL_FORM_VAMPIRE) {
		int remining_time = (this->flyVampireRemainingFrame + (59 - 15)) / 60;
		GXRgb color = RGB_WHITE;
		if(remining_time <= 3) {
			color = GX_RGB( 31, 18, 18);
		}
		disp_commonObjNumberSimpleZeroColor( remining_time, 2,
			LAYOUT_ID_MAIN_VAMPIRE_REMANING_TIME_NUM_LOW_DIGIT,
			LAYOUT_ID_MAIN_VAMPIRE_REMANING_TIME_NUM_HIGH_DIGIT,
			color,
			ALPHA_DEFAULT
		);
	}
	else if(this->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR) {
		disp_commonObjDxCenterFull( OBJ_ID_GUNGNIR_GUNGNIR_0,
			(int)this->x, (int)this->y - 16 - (int)this->z + this->cupinDispCnt + add_y - 110, z - 10, 256, 128, 270, FALSE, FALSE, RGB_WHITE, 31, DISP_COMMON_BLEND_MODE_ADD
		);
	}

	if(this->dispStateReturnCnt >= 1) {
		this->dispStateReturnCnt--;
		if(this->dispStateReturnCnt == 0) {
			if(this->dispState == nn::yb::e::ball::DISP_STATE_DOYA) {
				this->dispState = nn::yb::e::ball::DISP_STATE_JOY;
				this->dispStateReturnCnt = 90;
			}
			else if(this->dispState == nn::yb::e::ball::DISP_STATE_SHOCK) {
				this->dispState = nn::yb::e::ball::DISP_SATTE_DAMAGE;
				this->dispStateReturnCnt = 90;
			}
			else if(this->dispState == nn::yb::e::ball::DISP_STATE_PRESS ||
				this->dispState == nn::yb::e::ball::DISP_STATE_BURN
			) {
				this->dispState = nn::yb::e::ball::DISP_STATE_SHOCK;
				this->dispStateReturnCnt = 90;
			}
			else {
				this->dispState = nn::yb::e::ball::DISP_STATE_DEFAULT;
			}
		}
	}

	bool ground_flag = true;
	if((this->state == nn::yb::e::ball::STATE_FLYING && (!this->m_bIsGround)) ||
		this->state == nn::yb::e::ball::STATE_CUPIN ||
		this->holeCtrl->state == nn::yb::e::hole_ctrl::STATE_CUPIN
	) {
		ground_flag = false;
	}
	if((int)this->z >= -this->r && ground_flag) {
		disp_commonObjCenterColor( OBJ_ID_MAIN_SHADOW_0, (int)this->x, (int)this->y,
			nn::yb::e::global::DISP_Z_OBJ_SHADOW, RGB_WHITE, 16
		);
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndCb(void *ballArg) {

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;

	ball->spellEffectEnd();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndDummyCb(void *ballArg, void *dummy) {
	
	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;
	(void)dummy;

	/*
	// エフェクト重複不具合対応
	ball->musoFlag = false;
	*/
	ball->musoFlag = false;
	ball->spellEffectEnd();
	
}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectMusoCb( void * ballArg, int bitId) {

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;
	if(bitId == 4) {
		/*
		// エフェクト重複不具合対応
		ball->spellEffectEnd();
		*/
		ball->setDispState( nn::yb::e::ball::DISP_STATE_SHOCK, 120);
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectMusoReimuCb( void * ballArg) {
	
	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;
	if(ball->dispState != nn::yb::e::ball::DISP_STATE_SHOCK) {
		ball->dispState = nn::yb::e::ball::DISP_STATE_DEFAULT;
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndGungnirCb( void * ballArg, void * dummy) {

	(void)dummy;

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;

	ball->spellEffectEndGungnir();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndGungnir() {

	this->ballForm = nn::yb::e::ball::BALL_FORM_SPEAR;

	this->spellEffectEnd();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndGungnirHitCb( void * ballArg) {
	
	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;

	ball->spellEffectEndGungnirHit();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndGungnirHit() {

	this->state = nn::yb::e::ball::STATE_FLYING;

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndMomijiCb( void * ballArg, void * dummy) {

	(void)dummy;

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;

	ball->spellEffectEndMomiji();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndMomiji() {

	this->ballForm = nn::yb::e::ball::BALL_FORM_TORNADO;
	this->speed = nn::yb::e::global::FLY_MOMIJI_RELEASE_SPEED;
	this->zSpeed = nn::yb::e::global::FLY_MOMIJI_RELEASE_Z_SPEED;

	this->spellEffectEnd();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndTenkoCb( void * ballArg, void * dummy) {

	(void)dummy;

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;

	ball->spellEffectEndTenko();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndTenko() {

	this->ballForm = nn::yb::e::ball::BALL_FORM_PRESS;
	this->speed = nn::yb::e::global::FLY_TENKO_RELEASE_SPEED;
	this->zSpeed = nn::yb::e::global::FLY_TENKO_RELEASE_Z_SPEED;
	this->rotHorizon = 0.0;
	this->rotVertical = 0.0;

	this->spellEffectEnd();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndNitoriCb( void * ballArg, void * dummy) {

	(void)dummy;

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;

	ball->spellEffectEndNitori();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndNitori() {

	this->ballForm = nn::yb::e::ball::BALL_FORM_MECHA;

	this->spellEffectEnd();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndFlanCb( void * ballArg, void * dummy) {

	(void)dummy;

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;

	ball->spellEffectEndFlan();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndFlan() {

	// エフェクトの中で処理済み
	//this->ballForm = nn::yb::e::ball::BALL_FORM_VAMPIRE;
	//this->flyVampireRemainingFrame = 60 * 10 + 15;

	this->spellEffectEnd();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndOrinCb( void * ballArg, void * dummy) {

	(void)dummy;

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;

	ball->spellEffectEndOrin();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndOrin() {

	this->spellEffectEnd();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectOrinCb( void * ballArg, void * stateArg, void * cbArg, void * cbArgArg) {

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;

	ball->spellEffectOrin( (int)stateArg, (void (*)(void *, void *))cbArg, cbArgArg);

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectOrin( int state, void (* cb)(void *, void *), void * cbArg) {

	switch(state) {
		case nn::yb::e::orin::CB_STATE_CLASH:
			{
				this->spellOrinCb  = cb;
				this->spellOrinCbArg = cbArg;
				this->spellOrinState = state;

				this->setDispState( nn::yb::e::ball::DISP_STATE_CLASH);
				this->spellOrinAnimeTask = animationExec( NULL, ANIMATION_ID_SPELL_ORIN_CLASH_MOVE,
					320, 240 - 24, - 400, 0,
					nn::yb::main::CBall::spellOrinClashCb, this
				);
				//nn::yb::e::orin::VEC vec = (nn::yb::e::orin::VEC)(((360 - (int)this->angle) + 135) % 360 / 90);
				nn::yb::e::orin::VEC vec = ANGLE_2_RIGHT_LEFT_VEC;
				int anime_id_tbl[] = {
					ANIMATION_ID_SPELL_ORIN_CLASH_ANIME_DOWN,	// nn::yb::e::orin::VEC_UP
					ANIMATION_ID_SPELL_ORIN_CLASH_ANIME_RIGHT,	// nn::yb::e::orin::VEC_RIGHT
					ANIMATION_ID_SPELL_ORIN_CLASH_ANIME_DOWN,	// nn::yb::e::orin::VEC_DOWN
					ANIMATION_ID_SPELL_ORIN_CLASH_ANIME_LEFT,	// nn::yb::e::orin::VEC_LEFT
				};
				animationChangeObjId( (ANIMATION *)this->spellOrinAnimeTask->work,
					ANIMATION_CHANGE_OBJ_ID_TYPE_OBJ_ID,
					ANIMATION_ID_SPELL_ORIN_CLASH_ANIME_DOWN, anime_id_tbl[vec]
				);
			}
			break;
		case nn::yb::e::orin::CB_STATE_KICK:
			{
				// お燐の4方向バージョンならnn::yb::main::CBall::spellOrinClash()でやる?
				this->setDispState( nn::yb::e::ball::DISP_STATE_DEFAULT);
				this->ballForm = nn::yb::e::ball::BALL_FORM_ORIN;
			}
			break;
		case nn::yb::e::orin::CB_STATE_SKIP:
			{
				if(this->spellOrinAnimeTask != NULL) {
					animationEnd( (ANIMATION *)this->spellOrinAnimeTask->work);
				}
				else {
					this->spellOrinClash();
				}
			}
			break;
		case nn::yb::e::orin::CB_STATE_END:
			{
				this->spellEffectEndOrin();
			}
			break;
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellOrinClashCb( void * argBall) {

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)argBall;

	ball->spellOrinClash();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellOrinClash() {

	this->spellOrinAnimeTask = NULL;

	if(this->spellOrinCb != NULL) {
		this->spellOrinCb( this->spellOrinCbArg, (void *)this->spellOrinState);
		this->spellOrinCb = NULL;
	}

	/*
	// お燐の4方向バージョンならここ
	this->setDispState( nn::yb::e::ball::DISP_STATE_DEFAULT);
	this->ballForm = nn::yb::e::ball::BALL_FORM_ORIN;
	*/

	// お燐の4方向バージョンなら不要?
	this->setDispState( nn::yb::e::ball::DISP_STATE_CLASH);

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEndCb(void *ballArg, void *dummy) {
	
	(void)dummy;

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;

	ball->spellEffectEnd();

}

//---------------------------------------------------------------------------

void nn::yb::main::CBall::spellEffectEnd() {

	//スペルカードのBGMを引き継ぐ
	//sound_nnsStopSeq();
	//sound_nnsResumeSeqDX( BGM_STAGE01);

	// 各エフェクト内で処理
	//this->setDispState( nn::yb::e::ball::DISP_STATE_SHOCK, 30);

#ifndef _DEBUG
	this->spellCardEnableFlag = false;
	this->dispSpellCardEnableFlag = false;
#endif	// _DEBUG

	this->state = nn::yb::e::ball::STATE_FLYING;

}
//---------------------------------------------------------------------------
void nn::yb::main::CBall::goodLuckEffectEndCb( void * ballArg) {

	nn::yb::main::CBall * ball = (nn::yb::main::CBall *)ballArg;
	ball->goodLuckEffectEnd();

}
//---------------------------------------------------------------------------
void nn::yb::main::CBall::goodLuckEffectEnd() {

	this->goodLuckAnimeTask = NULL;

}
//---------------------------------------------------------------------------
void nn::yb::main::CBall::setDispState( int state) {

	this->setDispState( state, 0);

}
//---------------------------------------------------------------------------
void nn::yb::main::CBall::setDispState(int state, int cnt) {

	this->dispState = state;
	this->dispStateReturnCnt = cnt;

}
//---------------------------------------------------------------------------
void nn::yb::main::CBall::setSpeed( double argSpeed) {

	this->speed = argSpeed;

}
//---------------------------------------------------------------------------
void nn::yb::main::CBall::setZSpeed( double argZSpeed) {

	this->zSpeed = argZSpeed;

}
//---------------------------------------------------------------------------
double nn::yb::main::CBall::getSpeed() {
	
	return this->speed;

}
//---------------------------------------------------------------------------
double nn::yb::main::CBall::getZSpeed() {
	
	return this->zSpeed;

}
//---------------------------------------------------------------------------

