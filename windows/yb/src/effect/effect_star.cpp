#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nn_common.h"

#include "disp_common.h"
#include "sin_cos.h"

#include "effect\\effect_ctrl.h"
#include "control.h"

#include "obj_main_3d_data.h"

#include "effect_star.h"

#include "common\\LunaAnimation.h"
//---------------------------------------------------------------------------

int nn::yb::main::CEffectStar::POS_STARBIT_FROM_X	= 0;
int nn::yb::main::CEffectStar::POS_STARBIT_FROM_Y	= 0;
int nn::yb::main::CEffectStar::POS_STARBIT_WIDTH	= 64;
int nn::yb::main::CEffectStar::POS_STARBIT_HEIGHT	= 64;

float nn::yb::main::CEffectStar::STARBIT_SPEED_X_INIT	= 3.0f;
float nn::yb::main::CEffectStar::STARBIT_SPEED_Y_INIT	= -4.0f;
float nn::yb::main::CEffectStar::STARBIT_ACC_X_INIT	= -0.03f;
float nn::yb::main::CEffectStar::STARBIT_ACC_Y_INIT	= 0.38f;
//---------------------------------------------------------------------------

nn::yb::main::CEffectStar::CEffectStar() {

	::nn_commonLog(1,  "CEffectStar CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::CEffectStar));

}

//---------------------------------------------------------------------------

nn::yb::main::CEffectStar::CEffectStar( TASK_SAVE * parent, nn::yb::main::CEffectStar * effectArg, int frame, int  x, int y) 
: ::TASK_SAVE( parent) 
, totalFrame(frame)//デフォルト値
{
	::nn_commonLog(1,  "CEffectStar START\n");

	this->ctrl = effectArg->ctrl;
	this->cnt = 1;//子タスクを持つ場合、全て子タスクが終了した瞬間に親タスクも終了してしまうため、自分自身をカウントしておく
	this->frameCnt = 0;
	
	this->objId = effectArg->objId;
	this->a = 31;
	this->r = 0;
	this->w = effectArg->w;
	this->h = effectArg->h;
	this->blend = DISP_COMMON_BLEND_MODE_ADD;
	////this->frame = 360;//表示する最大フレーム
	this->frame = frame;//表示する最大フレーム

	this->m_iStarNum= effectArg->m_iStarNum;//★オブジェクト個数
	this->m_fScale = effectArg->m_fScale;


	this->m_vec3DPos = D3DXVECTOR3((float)x, (float)y, -100.0f);//エフェクトオブジェクトのz値を指定(for debug)
	this->m_vec3DTarget = effectArg->m_vec3DTarget;
	this->m_vec3DSpeed = D3DXVECTOR3(15.0f, 15.0f, 0.0f);
	this->m_vec3DAccel= D3DXVECTOR3(-0.25f, -0.25f, 0.0f);
	this->m_eState = 0;
	

	int angle = (::n_commonRandN(36) - 18) * 2;
	if (angle == 0)
		angle = 1;

	this->m_uiScaleCounter = 0;//拡大縮小用カウンター
	this->m_fScaleLimit = 2.0f;
	////////this->m_vec3DScale= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	////this->m_vec3DScale.x = this->m_vec3DScale.y = 3.0f;
	this->m_vec3DScale.x = this->m_vec3DScale.y = m_fScale;

	this->m_uiAnimeCounter = 0;
	this->m_uiAnimeCounterLimit = 240;
 


	this->m_uiAnimeCounterLimit = frame;//消えるまでのフレーム数
	objIdOffset = n_commonRandN( 4 );
			
	float fSpeedX = STARBIT_SPEED_X_INIT * static_cast<int>(::n_commonRandN( 5 ) - 2);
	float fAccelX = STARBIT_ACC_X_INIT * static_cast<int>(n_commonRandN( 5 ) - 2);
	if (objIdOffset < 3)
	{
		fSpeedX *= -1.0f + n_commonRandN( 8 ) * 0.1f;
		fAccelX *= -1.0f+ n_commonRandN( 2 ) * 0.1f;
	}
	float fSpeedY = STARBIT_SPEED_Y_INIT * static_cast<int>(GetRand( 3 ));
	float fAccelY = STARBIT_ACC_Y_INIT ;
	this->m_vec3DSpeed = D3DXVECTOR3(fSpeedX, fSpeedY, 0.0f);
	this->m_vec3DAccel= D3DXVECTOR3(fAccelX, fAccelY, 0.0f);
	this->m_eState = 0;
	

}

//---------------------------------------------------------------------------

nn::yb::main::CEffectStar::~CEffectStar() {

	::nn_commonLog(1,  "CEffectStar END\n");
}

//---------------------------------------------------------------------------

void nn::yb::main::CEffectStar::task() {

	this->disp();
	this->frameCnt++;
	bool end_flag = false;

	this->m_uiAnimeCounter++;
	switch ( this->m_eState )
	{
	case 0://初期移動(当たり判定無し)
		{
			if (
					this->m_uiAnimeCounter >= this->m_uiAnimeCounterLimit//指定フレーム経過した場合
				)
			{
				end_flag = true;
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
			}

			this->m_vec3DScale.x = this->m_vec3DScale.y = LunaAnimation::AddSpeed(this->m_fScaleLimit, 0.5f, (float)this->m_uiAnimeCounterLimit, (float)this->m_uiAnimeCounter);

			
			this->m_vec3DSpeed += this->m_vec3DAccel;
			this->m_vec3DPos += this->m_vec3DSpeed;


			this->r = LunaAnimation::AddSpeed(0.0f, (float)PI * 1.0f * 180.0f, (float)this->m_uiAnimeCounterLimit, (float)this->m_uiAnimeCounter);

			float Alpha = LunaAnimation::AddSpeed(255.0f,16.0f, (float)this->m_uiAnimeCounterLimit, (float)this->m_uiAnimeCounter);
			this->a = Alpha;

		}
		break;
	}

		if(this->frameCnt >= this->frame) {
			end_flag = true;
		}
		if(
			////(int)(this->m_vec3DPos.x + this->w / 2) < 0 || 640 < (int)(this->m_vec3DPos.x - this->w / 2)
			////|| (int)(this->m_vec3DPos.y + this->h / 2) < 0 || 480 < (int)(this->m_vec3DPos.y - this->h / 2)|| //画面内からはみ出した場合

			(int)(this->h ) <= 0 || 0 >= (int)(this->h)//縮小して大きさが0になった場合
			) {
				end_flag = true;//タスク消去フラグを立てる
		}

		//もし最初に規定した最大フレームまで進んだら、カウントしない
		////if (this->frame < cnt)
		////{
		////	cnt = this->frame;
		////}

	if(	
		end_flag 
		////&& this->cnt == 0 
		) {
		this->ctrl->pop_event();
		this->taskDelete();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::CEffectStar::disp() {

	int x = (int)this->m_vec3DPos.x;
	int y = (int)this->m_vec3DPos.y;
	int z = (int)this->m_vec3DPos.z;
	int w = (int)(this->m_vec3DScale.x * this->w );
	int h = (int)(this->m_vec3DScale.y * this->h );
	u16 r = (int)this->r;

	int b = (int)this->blend;
	::disp_commonObjDxCenterFull( this->objId + objIdOffset //画像IDにオフセットを加える
		, x, y, z, w, h, r, FALSE, FALSE, RGB_WHITE, static_cast<u16>(a), b);

}

//---------------------------------------------------------------------------

void nn::yb::main::CEffectStar::call_back( void * effectArg, void * dummy) {

	nn::yb::main::CEffectStar * effect = (nn::yb::main::CEffectStar *)effectArg;

	effect->cnt--;
	if(effect->cnt == 0) {
		effect->ctrl->pop_event();
		effect->taskDelete();
	}

}

//---------------------------------------------------------------------------

nn::yb::main::CEffectStar * nn::yb::main::effect_star_arg( TASK_SAVE * parent, nn::yb::main::CEffectStar * effectArg, int frame, int  x, int y) {

	nn::yb::main::CEffectStar * effect_star = new nn::yb::main::CEffectStar( parent, effectArg, frame, x, y);

	const float fWidth = effectArg->w;
	const float fHeight = effectArg->h;
	////for (int i = 0; i < 15; i++)
	for (int i = 0; i < 5; i++)
	{
		
		float fScale = n_commonRandN( 5 ) * 2.0f / 5.0f ;
		int iFrame = n_commonRandN( frame ) + 30;
		effectArg->w = fWidth * fScale;
		effectArg->h = fHeight * fScale;
		nn::yb::main::CEffectStar * effect_star1 = new nn::yb::main::CEffectStar( parent, effectArg, iFrame, x, y);
		////this->cnt++;
	}

	return effect_star;

}

//---------------------------------------------------------------------------
//
nn::yb::main::CEffectStar * nn::yb::main::effect_star( 
		 TASK_SAVE * parent, int frame, int x, int y, int tx, int ty, int size, int color, int numEffect, float fScale, nn::yb::main::CEffectCtrl * ctrlArg
	){
		
	nn::yb::main::CEffectStar effect_anim_arg;
	
	effect_anim_arg.ctrl = ctrlArg;

	////angle %= 360;
	effect_anim_arg.m_vec3DPos.x = (float)x;
	effect_anim_arg.m_vec3DPos.y = (float)y;
	effect_anim_arg.m_vec3DTarget.x = (float)tx;
	effect_anim_arg.m_vec3DTarget.y = (float)ty;
	effect_anim_arg.w = (float)size;
	effect_anim_arg.h = (float)size;
	effect_anim_arg.objId = OBJ_ID_STAR_BIT_STAR_0 + color;

	effect_anim_arg.m_iStarNum = numEffect;
	effect_anim_arg.m_fScale = fScale;

	return nn::yb::main::effect_star_arg( parent, &effect_anim_arg, frame, x, y);
}

//---------------------------------------------------------------------------

