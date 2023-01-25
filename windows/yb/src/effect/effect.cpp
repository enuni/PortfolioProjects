#include <stdio.h>
#include <string.h>
#include <stdlib.h>

////#include "nn_common.h"
////
////#include "disp_common.h"
#include "sin_cos.h"

#include "effect/effect_ctrl.h" 


#include "nn_common.h"
#include "nn_common2.h"

#include "disp_common.h"

#include "obj_main_3d_data.h"




#include "effect.h"


#include "LunaAnimation.h"
//---------------------------------------------------------------------------

nn::yb::main::EFFECT_THROW::EFFECT_THROW() {

	::nn_commonLog(999,  "EFFECT_THROW CREATE\n");
	
	memset( this, 0, sizeof( nn::yb::main::EFFECT_THROW));

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_THROW::EFFECT_THROW( TASK_SAVE * parent, yb::main::EFFECT_THROW * effectThrowArg) : ::TASK_SAVE( parent) {

	::nn_commonLog(999,  "EFFECT_THROW START\n");

	this->cnt = effectThrowArg->cnt;
	this->dispX = effectThrowArg->dispX;
	this->dispY = effectThrowArg->dispY;
	this->varX = effectThrowArg->varX;
	this->varY = effectThrowArg->varY;
	this->frame = effectThrowArg->frame;
	this->ctrl = effectThrowArg->ctrl;
	this->dispObjId = effectThrowArg->dispObjId;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_THROW::~EFFECT_THROW() {

	::nn_commonLog(999,  "EFFECT_THROW END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_THROW::task() {

	this->disp();

	this->dispX += this->varX;
	this->dispY += this->varY;

	this->cnt++;
	if(this->cnt > this->frame) {
		if(this->ctrl != NULL) {
			this->ctrl->pop_event();
		}
		this->taskDelete();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_THROW::disp() {

	int z = 1;
	::disp_commonObj( this->dispObjId, this->dispX, this->dispY, z);

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_THROW * nn::yb::main::effect_throw( TASK_SAVE * parent, int srcPosXy, int dstPosXy, int objDipsId, nn::yb::main::CEffectCtrl * ctrlArg) {

	nn::yb::main::CEffectCtrl * ctrl = ctrlArg;

	////int src_pos_x = ctrl->dun->get_pos_x( srcPosXy);
	////int src_pos_y = ctrl->dun->get_pos_y( srcPosXy);

	////int dst_pos_x = ctrl->dun->get_pos_x( dstPosXy);
	////int dst_pos_y = ctrl->dun->get_pos_y( dstPosXy);

	int src_pos_x = srcPosXy;
	int src_pos_y = srcPosXy;

	int dst_pos_x = dstPosXy;
	int dst_pos_y = dstPosXy;

	
	int move = 0;
	if(src_pos_x < dst_pos_x) {
		move = dst_pos_x - src_pos_x;
	}
	else if(dst_pos_x < src_pos_x) {
		move = src_pos_x - dst_pos_x;
	}
	if(src_pos_y < dst_pos_y) {
		move = dst_pos_y - src_pos_y;
	}
	else if(dst_pos_y < src_pos_y) {
		move = src_pos_y - dst_pos_y;
	}

	int frame = 2 * move;
	if(src_pos_x != dst_pos_x && src_pos_y != dst_pos_y) {
		frame = (int)((float)frame * 1.6);
	}

	int src_disp_x = 0;
	int src_disp_y = 0;
	////ctrl->dun->get_lay_xy_disp_map( &src_disp_x, &src_disp_y, srcPosXy);

	int dst_disp_x = 0;
	int dst_disp_y = 0;
	////ctrl->dun->get_lay_xy_disp_map( &dst_disp_x, &dst_disp_y, dstPosXy);

	nn::yb::main::EFFECT_THROW effect_throw_arg;
	effect_throw_arg.dispX = src_disp_x;
	effect_throw_arg.dispY = src_disp_y;
	effect_throw_arg.varX = (dst_disp_x - src_disp_x) / frame;
	effect_throw_arg.varY = (dst_disp_y - src_disp_y) / frame;
	effect_throw_arg.frame = frame;
	effect_throw_arg.ctrl = ctrl;
	effect_throw_arg.dispObjId = objDipsId;
	nn::yb::main::EFFECT_THROW * effect_throw = new nn::yb::main::EFFECT_THROW( parent, &effect_throw_arg);

	return effect_throw;

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SLIDE::EFFECT_SLIDE() {

	::nn_commonLog(999,  "EFFECT_SLIDE CREATE\n");
	
	memset( this, 0, sizeof( nn::yb::main::EFFECT_SLIDE));

	this->camera_offset_x = 0;
	this->camera_offset_y = 0;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SLIDE::EFFECT_SLIDE( ::TASK_SAVE * parent, nn::yb::main::EFFECT_SLIDE * effectSlideArg) : ::TASK_SAVE( parent) {

	::nn_commonLog(999,  "EFFECT_THROW START\n");
	
	this->ctrl = effectSlideArg->ctrl;
	this->mode = effectSlideArg->mode;
	this->objId = effectSlideArg->objId;
	this->flipV = effectSlideArg->flipV;
	this->flipH = effectSlideArg->flipH;
	this->frame = effectSlideArg->frame;
	this->x = effectSlideArg->x;
	this->y = effectSlideArg->y;
	this->vX = effectSlideArg->vX;
	this->vY = effectSlideArg->vY;
	this->z = effectSlideArg->z;
	this->w = effectSlideArg->w;
	this->h = effectSlideArg->h;
	this->vW = effectSlideArg->vW;
	this->vH = effectSlideArg->vH;
	this->a = effectSlideArg->a;
	this->vA = effectSlideArg->vA;
	this->r = effectSlideArg->r;
	this->vR = effectSlideArg->vR;

	this->blend = effectSlideArg->blend;
	
	this->aX = effectSlideArg->aX;
	this->aY = effectSlideArg->aY;
	this->aW = effectSlideArg->aW;
	this->aH = effectSlideArg->aH;
	this->aR = effectSlideArg->aR;
	this->aA = effectSlideArg->aA;

	this->cnt = effectSlideArg->cnt;

	this->camera_offset_x = effectSlideArg->camera_offset_x;
	this->camera_offset_y = effectSlideArg->camera_offset_y;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SLIDE::~EFFECT_SLIDE() {

	::nn_commonLog(999,  "EFFECT_THROW END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_SLIDE::task() {

	//::nn_commonLog(999,  "EFFECT_SLIDE::task()\n");

	this->disp();

	this->x += this->vX;
	this->y += this->vY;
	this->w += this->vW;
	this->h += this->vH;
	this->a += this->vA;
	this->r += this->vR;

	//added by nullpo 20090112
	if (this->w < 0.0f)
		this->w = 0.0f;
	if (this->h < 0.0f)
		this->h = 0.0f;
	if (this->a < 0.0f)
		this->a = 0.0f;
	this->vX += this->aX;
	this->vY += this->aY;
	this->vW += this->aW;
	this->vH += this->aH;
	this->vA += this->aA;
	this->vR += this->aR;


	////if (this->isLimit)
	////{
	////	if (this->isLimitMax)
	////	{
	////		if (this->x > lX)
	////			this->x = lX;
	////		if (this->y > lY)
	////			this->y = lY;
	////		if (this->w > lW)
	////			this->w = lW;
	////		if (this->h > lH)
	////			this->h = lH;
	////		if (this->a > lA)
	////			this->a = lA;
	////		if (this->r > lR)
	////			this->r = lR;
	////	}
	////	else
	////	{
	////		if (this->x < lX)
	////			this->x = lX;
	////		if (this->y < lY)
	////			this->y = lY;
	////		if (this->w < lW)
	////			this->w = lW;
	////		if (this->h < lH)
	////			this->h = lH;
	////		if (this->a < lA)
	////			this->a = lA;
	////		if (this->r < lR)
	////			this->r = lR;
	////	}
	////}
	

	this->cnt++;

	bool end_flag = false;
	if(this->mode == nn::yb::enum_effect::SLIDE_MODE_FRAME) {
		if(this->cnt >= this->frame) {
			end_flag = true;
		}
	}
	else if(this->mode == nn::yb::enum_effect::SLIDE_MODE_DISP) {
		if(
			(int)(this->x + this->w / 2) < 0 + camera_offset_x || 640 + camera_offset_x < (int)(this->x - this->w / 2)
			|| (int)(this->y + this->h / 2) < 0 + camera_offset_y || 480 + camera_offset_y < (int)(this->y - this->h / 2)//画面内からはみ出した場合

			|| (int)(this->h ) <= 0 || 0 >= (int)(this->h)//縮小して大きさが0になった場合
			) {
				end_flag = true;//タスク消去フラグを立てる
		}
	}

	if(end_flag) {
		if(this->ctrl != NULL) {
			this->ctrl->pop_event();
		}
		this->taskDelete();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_SLIDE::disp() {

	int x = (int)this->x;
	int y = (int)this->y;
	int w = (int)this->w;
	int h = (int)this->h;
	u16 a = (int)this->a;
	u16 r = (int)this->r;

	int b = (int)this->blend;

	bool flip_h = this->flipH;
	bool flip_v = this->flipV;
	//::nn_commonLog(999,  "obj_id = %d x = %d, y = %d z = %d, w = %d, h = %d r= %d a = %d\n", this->objId, x, y, z, w, h, r, a);

	////::disp_commonObjCenterFull( 0, this->objId, x, y, z, w, h, r, FALSE, FALSE, RGB_WHITE, a);
	::disp_commonObjDxCenterFull( this->objId, x, y, z, w, h, r, flip_h, flip_v, RGB_WHITE, a, b);////added by nullpo 20090112 for debug

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SLIDE * nn::yb::main::effect_out_simple( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, nn::yb::main::CEffectCtrl * ctrlArg) {

	nn::yb::main::EFFECT_SLIDE effect_slide_arg;
	
	effect_slide_arg.ctrl = ctrlArg;

	effect_slide_arg.mode = nn::yb::enum_effect::SLIDE_MODE_DISP;
	effect_slide_arg.objId = objId;
	effect_slide_arg.x = (float)x;
	effect_slide_arg.y = (float)y;
	angle %= 360;
	effect_slide_arg.vX = (float)(::gSin[angle] * speed);
	effect_slide_arg.vY = (float)(-::gCos[angle] * speed);
	effect_slide_arg.z = z;

	::OBJ * obj = ::disp_commonGetObj( objId);

	effect_slide_arg.w = (float)obj->w;
	effect_slide_arg.h = (float)obj->h;
	effect_slide_arg.vW = (float)0;
	effect_slide_arg.vH = (float)0;
	effect_slide_arg.a = 31;
	effect_slide_arg.vA = (float)0;
	effect_slide_arg.r = 0;
	effect_slide_arg.vR = (float)0;
	return nn::yb::main::effect_out_arg( parent, &effect_slide_arg);

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SLIDE * nn::yb::main::effect_out( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, int h, int r, int a, nn::yb::main::CEffectCtrl * ctrlArg) {

	nn::yb::main::EFFECT_SLIDE effect_slide_arg;
	
	effect_slide_arg.ctrl = ctrlArg;

	effect_slide_arg.mode = nn::yb::enum_effect::SLIDE_MODE_DISP;
	effect_slide_arg.objId = objId;
	effect_slide_arg.x = (float)x;
	effect_slide_arg.y = (float)y;
	angle %= 360;
	effect_slide_arg.vX = (float)(::gSin[angle] * speed);
	effect_slide_arg.vY = (float)(-::gCos[angle] * speed);
	effect_slide_arg.z = z;

	effect_slide_arg.w = (float)w;
	effect_slide_arg.h = (float)h;
	effect_slide_arg.vW = (float)0;
	effect_slide_arg.vH = (float)0;
	effect_slide_arg.a = (float)a;
	effect_slide_arg.vA = (float)0;
	effect_slide_arg.r = (float)r;
	effect_slide_arg.vR = (float)0;

	return nn::yb::main::effect_out_arg( parent, &effect_slide_arg);

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SLIDE * nn::yb::main::effect_out_ex( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg, int frame) {

	nn::yb::main::EFFECT_SLIDE effect_slide_arg;
	
	effect_slide_arg.ctrl = ctrlArg;

	effect_slide_arg.mode = nn::yb::enum_effect::SLIDE_MODE_FRAME;
	effect_slide_arg.objId = objId;
	effect_slide_arg.frame = frame;
	effect_slide_arg.x = (float)x;
	effect_slide_arg.y = (float)y;
	angle %= 360;
	effect_slide_arg.vX = (float)(::gSin[angle] * speed);
	effect_slide_arg.vY = (float)(-::gCos[angle] * speed);
	effect_slide_arg.z = z;

	effect_slide_arg.w = (float)w;
	effect_slide_arg.h = (float)h;
	effect_slide_arg.vW = (float)vW;
	effect_slide_arg.vH = (float)vH;
	effect_slide_arg.a = (float)a;
	effect_slide_arg.vA = (float)vA;
	effect_slide_arg.r = (float)r;
	effect_slide_arg.vR = (float)vR;

	effect_slide_arg.blend = blend;

	return nn::yb::main::effect_out_arg( parent, &effect_slide_arg);

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SLIDE * nn::yb::main::effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_SLIDE * effectSlideArg) {

	nn::yb::main::EFFECT_SLIDE * effect_slide = new nn::yb::main::EFFECT_SLIDE( parent, effectSlideArg);

	return effect_slide;

}

//---------------------------------------------------------------------------
//加速度設定有り
nn::yb::main::EFFECT_SLIDE * nn::yb::main::effect_out_ex_a(
	TASK_SAVE * parent, int objId,
	int x, int y, int z,
	int angle, int speed,
	int w, float vW, int h, float vH, 
	int r, float vR, int a, float vA, int blend, 
	nn::yb::main::CEffectCtrl * ctrlArg, 
	float aX, float aY, float aW, float aH, float aR, float aA
	 ) {

	nn::yb::main::EFFECT_SLIDE effect_slide_arg;
	
	effect_slide_arg.ctrl = ctrlArg;

	effect_slide_arg.mode = nn::yb::enum_effect::SLIDE_MODE_DISP;
	effect_slide_arg.objId = objId;
	effect_slide_arg.x = (float)x;
	effect_slide_arg.y = (float)y;
	angle %= 360;
	effect_slide_arg.vX = (float)(::gSin[angle] * speed);
	effect_slide_arg.vY = (float)(-::gCos[angle] * speed);
	effect_slide_arg.z = z;

	effect_slide_arg.w = (float)w;
	effect_slide_arg.h = (float)h;
	effect_slide_arg.vW = (float)vW;
	effect_slide_arg.vH = (float)vH;
	effect_slide_arg.a = (float)a;
	effect_slide_arg.vA = (float)vA;
	effect_slide_arg.r = (float)r;
	effect_slide_arg.vR = (float)vR;

	effect_slide_arg.blend = blend;

	effect_slide_arg.aX = aX;
	effect_slide_arg.aY = aY;
	effect_slide_arg.aW = aW;
	effect_slide_arg.aH = aH;
	effect_slide_arg.aA = aA;
	effect_slide_arg.aR = aR;

	return nn::yb::main::effect_out_arg( parent, &effect_slide_arg);
}
//---------------------------------------------------------------------------
//加速度設定有り(speedの加速度を設定)
nn::yb::main::EFFECT_SLIDE * nn::yb::main::effect_out_ex_a_spd(
	TASK_SAVE * parent, int objId,
	int x, int y, int z,
	int angle, int speed,
	int w, float vW, int h, float vH, 
	int r, float vR, int a, float vA, int blend, 
	nn::yb::main::CEffectCtrl * ctrlArg, 
	float aSpeed, float aW, float aH, float aR, float aA, bool flipH, bool flipV,
	int cameraX, int cameraY
	 
	) {

	nn::yb::main::EFFECT_SLIDE effect_slide_arg;
	
	effect_slide_arg.ctrl = ctrlArg;

	effect_slide_arg.mode = nn::yb::enum_effect::SLIDE_MODE_DISP;
	effect_slide_arg.objId = objId;

	effect_slide_arg.flipH = flipH;
	effect_slide_arg.flipV = flipV;

	effect_slide_arg.x = (float)x;
	effect_slide_arg.y = (float)y;
	angle %= 360;
	effect_slide_arg.vX = (float)(::gSin[angle] * speed);
	effect_slide_arg.vY = (float)(-::gCos[angle] * speed);
	effect_slide_arg.z = z;

	effect_slide_arg.w = (float)w;
	effect_slide_arg.h = (float)h;
	effect_slide_arg.vW = (float)vW;
	effect_slide_arg.vH = (float)vH;
	effect_slide_arg.a = (float)a;
	effect_slide_arg.vA = (float)vA;
	effect_slide_arg.r = (float)r;
	effect_slide_arg.vR = (float)vR;

	effect_slide_arg.blend = blend;

	effect_slide_arg.aX = (float)(::gSin[angle] * aSpeed);
	effect_slide_arg.aY = (float)(-::gCos[angle] * aSpeed);
	effect_slide_arg.aW = aW;
	effect_slide_arg.aH = aH;
	effect_slide_arg.aA = aA;
	effect_slide_arg.aR = aR;

	effect_slide_arg.camera_offset_x = cameraX;
	effect_slide_arg.camera_offset_y = cameraY;

	return nn::yb::main::effect_out_arg( parent, &effect_slide_arg);
}
//---------------------------------------------------------------------------
//加速度+限界設定有り
nn::yb::main::EFFECT_SLIDE * nn::yb::main::effect_out_ex_limit(
	TASK_SAVE * parent, int objId,
	int x, int y, int z,
	int angle, int speed,
	int w, float vW, int h, float vH, 
	int r, float vR, int a, float vA, int blend, 
	nn::yb::main::CEffectCtrl * ctrlArg, 
	float aX, float aY, float aW, float aH, float aR, float aA,
	float lX, float lY, float lW, float lH, float lR, float lA
	 ) {

	nn::yb::main::EFFECT_SLIDE effect_slide_arg;
	
	effect_slide_arg.ctrl = ctrlArg;

	effect_slide_arg.mode = nn::yb::enum_effect::SLIDE_MODE_DISP;
	effect_slide_arg.objId = objId;
	effect_slide_arg.x = (float)x;
	effect_slide_arg.y = (float)y;
	angle %= 360;
	effect_slide_arg.vX = (float)(::gSin[angle] * speed);
	effect_slide_arg.vY = (float)(-::gCos[angle] * speed);
	effect_slide_arg.z = z;

	effect_slide_arg.w = (float)w;
	effect_slide_arg.h = (float)h;
	effect_slide_arg.vW = (float)vW;
	effect_slide_arg.vH = (float)vH;
	effect_slide_arg.a = (float)a;
	effect_slide_arg.vA = (float)vA;
	effect_slide_arg.r = (float)r;
	effect_slide_arg.vR = (float)vR;

	effect_slide_arg.blend = blend;

	effect_slide_arg.aX = aX;
	effect_slide_arg.aY = aY;
	effect_slide_arg.aW = aW;
	effect_slide_arg.aH = aH;
	effect_slide_arg.aA = aA;
	effect_slide_arg.aR = aR;

	effect_slide_arg.lX = lX;
	effect_slide_arg.lY = lY;
	effect_slide_arg.lW = lW;
	effect_slide_arg.lH = lH;
	effect_slide_arg.lA = lA;
	effect_slide_arg.lR = lR;

	return nn::yb::main::effect_out_arg( parent, &effect_slide_arg);
}



//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM::EFFECT_ANIM() {

	::nn_commonLog(999,  "EFFECT_ANIM CREATE\n");
	
	memset( this, 0, sizeof( nn::yb::main::EFFECT_ANIM));

	this->camera_offset_x = 0;
	this->camera_offset_y = 0;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM::EFFECT_ANIM( ::TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM * effectAnimArg) : ::TASK_SAVE( parent) {

	::nn_commonLog(999,  "EFFECT_ANIM START\n");
	
	this->ctrl = effectAnimArg->ctrl;
	this->mode = effectAnimArg->mode;
	this->objId = effectAnimArg->objId;
	this->x = effectAnimArg->x;
	this->y = effectAnimArg->y;
	this->vX = effectAnimArg->vX;
	this->vY = effectAnimArg->vY;
	this->z = effectAnimArg->z;
	this->w = effectAnimArg->w;
	this->h = effectAnimArg->h;
	this->vW = effectAnimArg->vW;
	this->vH = effectAnimArg->vH;
	this->a = effectAnimArg->a;
	this->vA = effectAnimArg->vA;
	this->r = effectAnimArg->r;
	this->vR = effectAnimArg->vR;

	this->blend = effectAnimArg->blend;
	this->kind = effectAnimArg->kind;
	
	this->aX = effectAnimArg->aX;
	this->aY = effectAnimArg->aY;
	this->aW = effectAnimArg->aW;
	this->aH = effectAnimArg->aH;
	this->aR = effectAnimArg->aR;
	this->aA = effectAnimArg->aA;

	this->frame = effectAnimArg->frame;
	this->cnt = effectAnimArg->cnt;

	//始点・終点・始点から終点までにかける時間
	this->sx = effectAnimArg->sx; this->ex = effectAnimArg->ex; this->tx = effectAnimArg->tx;
	this->sy = effectAnimArg->sy; this->ey = effectAnimArg->ey; this->ty = effectAnimArg->ty;
	this->sw = effectAnimArg->sw; this->ew = effectAnimArg->ew; this->tw = effectAnimArg->tw;
	this->sh = effectAnimArg->sh; this->eh = effectAnimArg->eh; this->th = effectAnimArg->th;
	this->sr = effectAnimArg->sr; this->er = effectAnimArg->er; this->tr = effectAnimArg->tr;
	this->sa = effectAnimArg->sa; this->ea = effectAnimArg->ea; this->ta = effectAnimArg->ta;
	
	this->camera_offset_x = effectAnimArg->camera_offset_x;
	this->camera_offset_y = effectAnimArg->camera_offset_y;

	this->color = effectAnimArg->color;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM::~EFFECT_ANIM() {

	::nn_commonLog(999,  "EFFECT_ANIM END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_ANIM::task() {

	//::nn_commonLog(999,  "EFFECT_ANIM::task()\n");

	this->disp();

	////this->x += this->vX;
	////this->y += this->vY;
	////this->w += this->vW;
	////this->h += this->vH;
	////this->a += this->vA;
	////this->r += this->vR;

	//////added by nullpo 20090112
	////if (this->w < 0.0f)
	////	this->w = 0.0f;
	////if (this->h < 0.0f)
	////	this->h = 0.0f;
	////if (this->a < 0.0f)
	////	this->a = 0.0f;
	////this->vX += this->aX;
	////this->vY += this->aY;
	////this->vW += this->aW;
	////this->vH += this->aH;
	////this->vA += this->aA;
	////this->vR += this->aR;

	switch (this->kind)
	{
		case enum_effect::EFFECT_ANIME_FLAT:
		{
			this->x = LunaAnimation::FlatSpeed((float)this->sx, (float)this->ex, (float)this->tx, (float)this->cnt);
			this->y = LunaAnimation::FlatSpeed((float)this->sy, (float)this->ey, (float)this->ty, (float)this->cnt);
			this->w = LunaAnimation::FlatSpeed((float)this->sw, (float)this->ew, (float)this->tw, (float)this->cnt);
			this->h = LunaAnimation::FlatSpeed((float)this->sh, (float)this->eh, (float)this->th, (float)this->cnt);
			this->a = LunaAnimation::FlatSpeed((float)this->sa, (float)this->ea, (float)this->ta, (float)this->cnt);
			this->r = LunaAnimation::FlatSpeed((float)this->sr, (float)this->er, (float)this->tr, (float)this->cnt);
			break;
		}
		case enum_effect::EFFECT_ANIME_ADD:
		{
			this->x = LunaAnimation::AddSpeed((float)this->sx, (float)this->ex, (float)this->tx, (float)this->cnt);
			this->y = LunaAnimation::AddSpeed((float)this->sy, (float)this->ey, (float)this->ty, (float)this->cnt);
			this->w = LunaAnimation::AddSpeed((float)this->sw, (float)this->ew, (float)this->tw, (float)this->cnt);
			this->h = LunaAnimation::AddSpeed((float)this->sh, (float)this->eh, (float)this->th, (float)this->cnt);
			this->a = LunaAnimation::AddSpeed((float)this->sa, (float)this->ea, (float)this->ta, (float)this->cnt);
			this->r = LunaAnimation::AddSpeed((float)this->sr, (float)this->er, (float)this->tr, (float)this->cnt);
			break;
		}
		case enum_effect::EFFECT_ANIME_SUB:
		{
			this->x = LunaAnimation::SubSpeed((float)this->sx, (float)this->ex, (float)this->tx, (float)this->cnt);
			this->y = LunaAnimation::SubSpeed((float)this->sy, (float)this->ey, (float)this->ty, (float)this->cnt);
			this->w = LunaAnimation::SubSpeed((float)this->sw, (float)this->ew, (float)this->tw, (float)this->cnt);
			this->h = LunaAnimation::SubSpeed((float)this->sh, (float)this->eh, (float)this->th, (float)this->cnt);
			this->a = LunaAnimation::SubSpeed((float)this->sa, (float)this->ea, (float)this->ta, (float)this->cnt);
			this->r = LunaAnimation::SubSpeed((float)this->sr, (float)this->er, (float)this->tr, (float)this->cnt);
			break;
		}
	}


	this->cnt++;

	bool end_flag = false;
	if(this->mode == nn::yb::enum_effect::SLIDE_MODE_FRAME) {
		if(this->cnt >= this->frame) {
			end_flag = true;
		}
	}
	else if(this->mode == nn::yb::enum_effect::SLIDE_MODE_DISP) {
		if(
			(int)(this->x + this->w / 2) < 0 + camera_offset_x || 640 + camera_offset_x < (int)(this->x - this->w / 2)
			|| (int)(this->y + this->h / 2) < 0 + camera_offset_y || 480 + camera_offset_y < (int)(this->y - this->h / 2)//画面内からはみ出した場合

			|| (int)(this->h ) <= 0 || 0 >= (int)(this->h)//縮小して大きさが0になった場合
			) {
				end_flag = true;//タスク消去フラグを立てる
		}

		//もし最初に規定した最大フレームまで進んだら、カウントしない
		if (this->frame < cnt)
		{
			cnt = this->frame;
		}
	}

	if(end_flag) {
		if(this->ctrl != NULL) {
			this->ctrl->pop_event();
		}
		this->taskDelete();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_ANIM::disp() {

	int x = (int)this->x;
	int y = (int)this->y;
	int w = (int)this->w;
	int h = (int)this->h;
	u16 a = (int)this->a;
	u16 r = (int)this->r;

	int b = (int)this->blend;


	::disp_commonObjDxCenterFull( this->objId, x, y, z, w, h, r, FALSE, FALSE, this->color, a, b);

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM * nn::yb::main::effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM * effectAnimArg) {

	nn::yb::main::EFFECT_ANIM * effect_anim = new nn::yb::main::EFFECT_ANIM( parent, effectAnimArg);

	return effect_anim;

}

//---------------------------------------------------------------------------
//初期値・終了値・全体値設定
nn::yb::main::EFFECT_ANIM * nn::yb::main::effect_start_end_total(
	 TASK_SAVE * parent, int objId, int mode,
	 int frame,
	 int sx, int ex, int tx,
	 int sy, int ey, int ty,
	 int z, 
	 int sw, int ew, int tw,
	 int sh, int eh, int th,
	 int sr, int er, int tr,
	 int sa, int ea, int ta,
	 int blend, 
	 int kind, 
	 GXRgb color,
	 nn::yb::main::CEffectCtrl * ctrlArg
													 
	 ) {

	nn::yb::main::EFFECT_ANIM effect_anim_arg;
	

	effect_anim_arg.ctrl = ctrlArg;

	effect_anim_arg.mode = mode;
	effect_anim_arg.objId = objId;
	effect_anim_arg.frame = frame;
	effect_anim_arg.kind = kind;
	////effect_anim_arg.x = (float)sx;
	////effect_anim_arg.y = (float)sy;
	////angle %= 360;
	////effect_anim_arg.vX = (float)(::gSin[angle] * speed);
	////effect_anim_arg.vY = (float)(-::gCos[angle] * speed);
	////effect_anim_arg.z = z;
	effect_anim_arg.sx = sx; effect_anim_arg.ex = ex; effect_anim_arg.tx = tx;
	effect_anim_arg.sy = sy; effect_anim_arg.ey = ey; effect_anim_arg.ty = ty;
	effect_anim_arg.z = z;
	effect_anim_arg.sw = sw; effect_anim_arg.ew = ew; effect_anim_arg.tw = tw;
	effect_anim_arg.sh = sh; effect_anim_arg.eh = eh; effect_anim_arg.th = th;
	effect_anim_arg.sr = sr; effect_anim_arg.er = er; effect_anim_arg.tr = tr;
	effect_anim_arg.sa = sa; effect_anim_arg.ea = ea; effect_anim_arg.ta = ta;
	
	effect_anim_arg.blend = blend;

	effect_anim_arg.color = color;
	
	return nn::yb::main::effect_out_arg( parent, &effect_anim_arg);
}


//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_FADE::EFFECT_ANIM_FADE() {

	::nn_commonLog(999,  "EFFECT_ANIM_FADE CREATE\n");
	
	memset( this, 0, sizeof( nn::yb::main::EFFECT_ANIM_FADE));

	this->camera_offset_x = 0;
	this->camera_offset_y = 0;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_FADE::EFFECT_ANIM_FADE( ::TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_FADE * effectAnimArg) : ::TASK_SAVE( parent) {

	::nn_commonLog(999,  "EFFECT_THROW START\n");
	
	this->ctrl = effectAnimArg->ctrl;
	this->mode = effectAnimArg->mode;
	this->objId = effectAnimArg->objId;
	this->x = effectAnimArg->x;
	this->y = effectAnimArg->y;
	this->z = effectAnimArg->z;
	this->w = effectAnimArg->w;
	this->h = effectAnimArg->h;
	this->a = effectAnimArg->a;
	this->r = effectAnimArg->r;

	this->blend = effectAnimArg->blend;
	this->kind = effectAnimArg->kind;
	
	this->frame = effectAnimArg->frame;
	this->cnt = effectAnimArg->cnt;

	//fade in
	this->intime = effectAnimArg->intime;
	this->isx = effectAnimArg->isx; this->iex = effectAnimArg->iex; 
	this->isy = effectAnimArg->isy; this->iey = effectAnimArg->iey;
	this->isw = effectAnimArg->isw; this->iew = effectAnimArg->iew; 
	this->ish = effectAnimArg->ish; this->ieh = effectAnimArg->ieh; 
	this->isr = effectAnimArg->isr; this->ier = effectAnimArg->ier; 
	this->isa = effectAnimArg->isa; this->iea = effectAnimArg->iea; 
	//animation
	this->animtime = effectAnimArg->animtime;
	this->asx = effectAnimArg->asx; this->aex = effectAnimArg->aex; 
	this->asy = effectAnimArg->asy; this->aey = effectAnimArg->aey;
	this->asw = effectAnimArg->asw; this->aew = effectAnimArg->aew; 
	this->ash = effectAnimArg->ash; this->aeh = effectAnimArg->aeh; 
	this->asr = effectAnimArg->asr; this->aer = effectAnimArg->aer; 
	this->asa = effectAnimArg->asa; this->aea = effectAnimArg->aea; 
	//fade out
	this->outtime = effectAnimArg->outtime;
	this->osx = effectAnimArg->osx; this->oex = effectAnimArg->oex; 
	this->osy = effectAnimArg->osy; this->oey = effectAnimArg->oey;
	this->osw = effectAnimArg->osw; this->oew = effectAnimArg->oew; 
	this->osh = effectAnimArg->osh; this->oeh = effectAnimArg->oeh; 
	this->osr = effectAnimArg->osr; this->oer = effectAnimArg->oer; 
	this->osa = effectAnimArg->osa; this->oea = effectAnimArg->oea; 
	
	this->camera_offset_x = effectAnimArg->camera_offset_x;
	this->camera_offset_y = effectAnimArg->camera_offset_y;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_FADE::~EFFECT_ANIM_FADE() {

	::nn_commonLog(999,  "EFFECT_THROW END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_ANIM_FADE::task() {

	//::nn_commonLog(999,  "EFFECT_ANIM_FADE::task()\n");

	this->disp();

	switch (this->kind)
	{
		case enum_effect::EFFECT_ANIME_FLAT:
		{
			if (this->cnt <= this->intime)
			{
				this->x = LunaAnimation::FlatSpeed((float)this->isx, (float)this->iex, (float)this->intime, (float)this->cnt);
				this->y = LunaAnimation::FlatSpeed((float)this->isy, (float)this->iey, (float)this->intime, (float)this->cnt);
				this->w = LunaAnimation::FlatSpeed((float)this->isw, (float)this->iew, (float)this->intime, (float)this->cnt);
				this->h = LunaAnimation::FlatSpeed((float)this->ish, (float)this->ieh, (float)this->intime, (float)this->cnt);
				this->a = LunaAnimation::FlatSpeed((float)this->isa, (float)this->iea, (float)this->intime, (float)this->cnt);
				this->r = LunaAnimation::FlatSpeed((float)this->isr, (float)this->ier, (float)this->intime, (float)this->cnt);
			}
			else if ((float)this->cnt <= this->intime + this->animtime)
			{
				int cur_cnt = this->cnt - this->intime;
				this->x = LunaAnimation::FlatSpeed((float)this->asx, (float)this->aex, (float)this->animtime, (float)cur_cnt);
				this->y = LunaAnimation::FlatSpeed((float)this->asy, (float)this->aey, (float)this->animtime, (float)cur_cnt);
				this->w = LunaAnimation::FlatSpeed((float)this->asw, (float)this->aew, (float)this->animtime, (float)cur_cnt);
				this->h = LunaAnimation::FlatSpeed((float)this->ash, (float)this->aeh, (float)this->animtime, (float)cur_cnt);
				this->a = LunaAnimation::FlatSpeed((float)this->asa, (float)this->aea, (float)this->animtime, (float)cur_cnt);
				this->r = LunaAnimation::FlatSpeed((float)this->asr, (float)this->aer, (float)this->animtime, (float)cur_cnt);
			}
			else if ((float)this->cnt <= this->intime + this->animtime + this->outtime)
			{
				int cur_cnt = this->cnt - (this->intime + this->animtime);
				this->x = LunaAnimation::FlatSpeed((float)this->osx, (float)this->oex, (float)this->outtime, (float)cur_cnt);
				this->y = LunaAnimation::FlatSpeed((float)this->osy, (float)this->oey, (float)this->outtime, (float)cur_cnt);
				this->w = LunaAnimation::FlatSpeed((float)this->osw, (float)this->oew, (float)this->outtime, (float)cur_cnt);
				this->h = LunaAnimation::FlatSpeed((float)this->osh, (float)this->oeh, (float)this->outtime, (float)cur_cnt);
				this->a = LunaAnimation::FlatSpeed((float)this->osa, (float)this->oea, (float)this->outtime, (float)cur_cnt);
				this->r = LunaAnimation::FlatSpeed((float)this->osr, (float)this->oer, (float)this->outtime, (float)cur_cnt);
			}
			
			break;
		}
		case enum_effect::EFFECT_ANIME_ADD:
		{
			if (this->cnt <= this->intime)
			{
				this->x = LunaAnimation::AddSpeed((float)this->isx, (float)this->iex, (float)this->intime, (float)this->cnt);
				this->y = LunaAnimation::AddSpeed((float)this->isy, (float)this->iey, (float)this->intime, (float)this->cnt);
				this->w = LunaAnimation::AddSpeed((float)this->isw, (float)this->iew, (float)this->intime, (float)this->cnt);
				this->h = LunaAnimation::AddSpeed((float)this->ish, (float)this->ieh, (float)this->intime, (float)this->cnt);
				this->a = LunaAnimation::AddSpeed((float)this->isa, (float)this->iea, (float)this->intime, (float)this->cnt);
				this->r = LunaAnimation::AddSpeed((float)this->isr, (float)this->ier, (float)this->intime, (float)this->cnt);
			}
			else if (this->cnt <= this->intime + this->animtime)
			{
				int cur_cnt = this->cnt - this->intime;
				this->x = LunaAnimation::AddSpeed((float)this->asx, (float)this->aex, (float)this->animtime, (float)cur_cnt);
				this->y = LunaAnimation::AddSpeed((float)this->asy, (float)this->aey, (float)this->animtime, (float)cur_cnt);
				this->w = LunaAnimation::AddSpeed((float)this->asw, (float)this->aew, (float)this->animtime, (float)cur_cnt);
				this->h = LunaAnimation::AddSpeed((float)this->ash, (float)this->aeh, (float)this->animtime, (float)cur_cnt);
				this->a = LunaAnimation::AddSpeed((float)this->asa, (float)this->aea, (float)this->animtime, (float)cur_cnt);
				this->r = LunaAnimation::AddSpeed((float)this->asr, (float)this->aer, (float)this->animtime, (float)cur_cnt);
			}
			else if (this->cnt <= this->intime + this->animtime + this->outtime)
			{
				int cur_cnt = this->cnt - (this->intime + this->animtime);
				this->x = LunaAnimation::AddSpeed((float)this->osx, (float)this->oex, (float)this->outtime, (float)cur_cnt);
				this->y = LunaAnimation::AddSpeed((float)this->osy, (float)this->oey, (float)this->outtime, (float)cur_cnt);
				this->w = LunaAnimation::AddSpeed((float)this->osw, (float)this->oew, (float)this->outtime, (float)cur_cnt);
				this->h = LunaAnimation::AddSpeed((float)this->osh, (float)this->oeh, (float)this->outtime, (float)cur_cnt);
				this->a = LunaAnimation::AddSpeed((float)this->osa, (float)this->oea, (float)this->outtime, (float)cur_cnt);
				this->r = LunaAnimation::AddSpeed((float)this->osr, (float)this->oer, (float)this->outtime, (float)cur_cnt);
			}
			break;
		}
		case enum_effect::EFFECT_ANIME_SUB:
		{
			if (this->cnt <= this->intime)
			{
				this->x = LunaAnimation::SubSpeed((float)this->isx, (float)this->iex, (float)this->intime, (float)this->cnt);
				this->y = LunaAnimation::SubSpeed((float)this->isy, (float)this->iey, (float)this->intime, (float)this->cnt);
				this->w = LunaAnimation::SubSpeed((float)this->isw, (float)this->iew, (float)this->intime, (float)this->cnt);
				this->h = LunaAnimation::SubSpeed((float)this->ish, (float)this->ieh, (float)this->intime, (float)this->cnt);
				this->a = LunaAnimation::SubSpeed((float)this->isa, (float)this->iea, (float)this->intime, (float)this->cnt);
				this->r = LunaAnimation::SubSpeed((float)this->isr, (float)this->ier, (float)this->intime, (float)this->cnt);
			}
			else if (this->cnt <= this->intime + this->animtime)
			{
				int cur_cnt = this->cnt - this->intime;
				this->x = LunaAnimation::SubSpeed((float)this->asx, (float)this->aex, (float)this->animtime, (float)cur_cnt);
				this->y = LunaAnimation::SubSpeed((float)this->asy, (float)this->aey, (float)this->animtime, (float)cur_cnt);
				this->w = LunaAnimation::SubSpeed((float)this->asw, (float)this->aew, (float)this->animtime, (float)cur_cnt);
				this->h = LunaAnimation::SubSpeed((float)this->ash, (float)this->aeh, (float)this->animtime, (float)cur_cnt);
				this->a = LunaAnimation::SubSpeed((float)this->asa, (float)this->aea, (float)this->animtime, (float)cur_cnt);
				this->r = LunaAnimation::SubSpeed((float)this->asr, (float)this->aer, (float)this->animtime, (float)cur_cnt);
			}
			else if (this->cnt <= this->intime + this->animtime + this->outtime)
			{
				int cur_cnt = this->cnt - (this->intime + this->animtime);
				this->x = LunaAnimation::SubSpeed((float)this->osx, (float)this->oex, (float)this->outtime, (float)cur_cnt);
				this->y = LunaAnimation::SubSpeed((float)this->osy, (float)this->oey, (float)this->outtime, (float)cur_cnt);
				this->w = LunaAnimation::SubSpeed((float)this->osw, (float)this->oew, (float)this->outtime, (float)cur_cnt);
				this->h = LunaAnimation::SubSpeed((float)this->osh, (float)this->oeh, (float)this->outtime, (float)cur_cnt);
				this->a = LunaAnimation::SubSpeed((float)this->osa, (float)this->oea, (float)this->outtime, (float)cur_cnt);
				this->r = LunaAnimation::SubSpeed((float)this->osr, (float)this->oer, (float)this->outtime, (float)cur_cnt);
			}
			break;
		}
	}


	this->cnt++;

	bool end_flag = false;
	if(this->mode == nn::yb::enum_effect::SLIDE_MODE_FRAME) {
		if(this->cnt >= this->frame) {
			end_flag = true;
		}
	}
	else if(this->mode == nn::yb::enum_effect::SLIDE_MODE_DISP) {
		if(
			(int)(this->x + this->w / 2) < 0 + camera_offset_x || 640 + camera_offset_x < (int)(this->x - this->w / 2)
			|| (int)(this->y + this->h / 2) < 0 + camera_offset_y || 480 + camera_offset_y < (int)(this->y - this->h / 2)//画面内からはみ出した場合

			|| (int)(this->h ) <= 0 || 0 >= (int)(this->h)//縮小して大きさが0になった場合
			) {
				end_flag = true;//タスク消去フラグを立てる
		}

		//もし最初に規定した最大フレームまで進んだら、カウントしない
		if (this->frame < cnt)
		{
			cnt = this->frame;
		}
	}

	if(end_flag) {
		if(this->ctrl != NULL) {
			this->ctrl->pop_event();
		}
		this->taskDelete();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_ANIM_FADE::disp() {

	int x = (int)this->x;
	int y = (int)this->y;
	int w = (int)this->w;
	int h = (int)this->h;
	u16 a = (int)this->a;
	u16 r = (int)this->r;

	int b = (int)this->blend;


	::disp_commonObjDxCenterFull( this->objId, x, y, z, w, h, r, FALSE, FALSE, RGB_WHITE, a, b);

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_FADE * nn::yb::main::effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_FADE * effectAnimArg) {

	nn::yb::main::EFFECT_ANIM_FADE * effect_anim = new nn::yb::main::EFFECT_ANIM_FADE( parent, effectAnimArg);

	return effect_anim;

}

//---------------------------------------------------------------------------
//初期値・終了値・全体値設定
nn::yb::main::EFFECT_ANIM_FADE * nn::yb::main::effect_fadein_out( 
		TASK_SAVE * parent, int objId, int mode,
		int frame,
		int z, 
		int blend, 
		int kind, 
		//fade in
		int intime,
		int isx, int iex, 
		int isy, int iey,
		int isw, int iew, 
		int ish, int ieh, 
		int isr, int ier, 
		int isa, int iea, 
		//animation
		int animtime,
		int asx, int aex, 
		int asy, int aey,
		int asw, int aew, 
		int ash, int aeh, 
		int asr, int aer, 
		int asa, int aea, 
		//fade out
		int outtime,
		int osx, int oex, 
		int osy, int oey,
		int osw, int oew, 
		int osh, int oeh, 
		int osr, int oer, 
		int osa, int oea, 
		nn::yb::main::CEffectCtrl * ctrlArg
	){

	nn::yb::main::EFFECT_ANIM_FADE effect_anim_arg;
	
	effect_anim_arg.ctrl = ctrlArg;

	effect_anim_arg.mode = mode;
	effect_anim_arg.objId = objId;
	effect_anim_arg.frame = frame;
	effect_anim_arg.blend = blend;
	effect_anim_arg.kind = kind;
	////effect_anim_arg.x = (float)sx;
	////effect_anim_arg.y = (float)sy;
	////angle %= 360;
	////effect_anim_arg.vX = (float)(::gSin[angle] * speed);
	////effect_anim_arg.vY = (float)(-::gCos[angle] * speed);
	effect_anim_arg.z = z;

	//fade in
	effect_anim_arg.intime = intime;
	effect_anim_arg.isx = isx; effect_anim_arg.iex = iex; 
	effect_anim_arg.isy = isy; effect_anim_arg.iey = iey;
	effect_anim_arg.isw = isw; effect_anim_arg.iew = iew; 
	effect_anim_arg.ish = ish; effect_anim_arg.ieh = ieh; 
	effect_anim_arg.isr = isr; effect_anim_arg.ier = ier; 
	effect_anim_arg.isa = isa; effect_anim_arg.iea = iea; 
	//animation
	effect_anim_arg.animtime = animtime;
	effect_anim_arg.asx = asx; effect_anim_arg.aex = aex; 
	effect_anim_arg.asy = asy; effect_anim_arg.aey = aey;
	effect_anim_arg.asw = asw; effect_anim_arg.aew = aew; 
	effect_anim_arg.ash = ash; effect_anim_arg.aeh = aeh; 
	effect_anim_arg.asr = asr; effect_anim_arg.aer = aer; 
	effect_anim_arg.asa = asa; effect_anim_arg.aea = aea; 
	//fade out
	effect_anim_arg.outtime = outtime;
	effect_anim_arg.osx = osx; effect_anim_arg.oex = oex; 
	effect_anim_arg.osy = osy; effect_anim_arg.oey = oey;
	effect_anim_arg.osw = osw; effect_anim_arg.oew = oew; 
	effect_anim_arg.osh = osh; effect_anim_arg.oeh = oeh; 
	effect_anim_arg.osr = osr; effect_anim_arg.oer = oer; 
	effect_anim_arg.osa = osa; effect_anim_arg.oea = oea; 

	return nn::yb::main::effect_out_arg( parent, &effect_anim_arg);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_LOOP::EFFECT_LOOP() {

	::nn_commonLog(999,  "EFFECT_LOOP CREATE\n");
	
	memset( this, 0, sizeof( nn::yb::main::EFFECT_LOOP));

	this->camera_offset_x = 0;
	this->camera_offset_y = 0;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_LOOP::EFFECT_LOOP( ::TASK_SAVE * parent, nn::yb::main::EFFECT_LOOP * effectAnimArg) 
: nn::yb::main::EFFECT_ANIM_FADE( parent, effectAnimArg ) {

	::nn_commonLog(999,  "EFFECT_THROW START\n");
	
	this->ctrl = effectAnimArg->ctrl;
	this->mode = effectAnimArg->mode;
	this->objId = effectAnimArg->objId;
	this->x = effectAnimArg->x;
	this->y = effectAnimArg->y;
	this->z = effectAnimArg->z;
	this->w = effectAnimArg->w;
	this->h = effectAnimArg->h;
	this->a = effectAnimArg->a;
	this->r = effectAnimArg->r;

	this->blend = effectAnimArg->blend;
	this->kind = effectAnimArg->kind;
	
	this->frame = effectAnimArg->frame;
	this->cnt = effectAnimArg->cnt;

	//fade in
	this->intime = effectAnimArg->intime;
	this->isx = effectAnimArg->isx; this->iex = effectAnimArg->iex; 
	this->isy = effectAnimArg->isy; this->iey = effectAnimArg->iey;
	this->isw = effectAnimArg->isw; this->iew = effectAnimArg->iew; 
	this->ish = effectAnimArg->ish; this->ieh = effectAnimArg->ieh; 
	this->isr = effectAnimArg->isr; this->ier = effectAnimArg->ier; 
	this->isa = effectAnimArg->isa; this->iea = effectAnimArg->iea; 
	//animation
	this->animtime = effectAnimArg->animtime;
	this->asx = effectAnimArg->asx; this->aex = effectAnimArg->aex; 
	this->asy = effectAnimArg->asy; this->aey = effectAnimArg->aey;
	this->asw = effectAnimArg->asw; this->aew = effectAnimArg->aew; 
	this->ash = effectAnimArg->ash; this->aeh = effectAnimArg->aeh; 
	this->asr = effectAnimArg->asr; this->aer = effectAnimArg->aer; 
	this->asa = effectAnimArg->asa; this->aea = effectAnimArg->aea; 
	//fade out
	this->outtime = effectAnimArg->outtime;
	this->osx = effectAnimArg->osx; this->oex = effectAnimArg->oex; 
	this->osy = effectAnimArg->osy; this->oey = effectAnimArg->oey;
	this->osw = effectAnimArg->osw; this->oew = effectAnimArg->oew; 
	this->osh = effectAnimArg->osh; this->oeh = effectAnimArg->oeh; 
	this->osr = effectAnimArg->osr; this->oer = effectAnimArg->oer; 
	this->osa = effectAnimArg->osa; this->oea = effectAnimArg->oea; 
	
	
	this->m_fLoopSpeedX = effectAnimArg->m_fLoopSpeedX;//
	this->m_fLoopSpeedY = effectAnimArg->m_fLoopSpeedY;//
	////m_fMoveSpeedX = 0.0f;//
	////m_fMoveSpeedY = 0.0f;//

	m_fLoopStartPosX = 0.0f;//m_cLoobObjの内、画像を描画する開始座標
	m_fLoopStartPosY = 0.0f;//m_cLoobObjの内、画像を描画する開始座標
	m_iLoopNumX = 1;
	m_iLoopNumY = 1;//x,ｙ方向に描画する回数(最低1回はループ)


	////int iCurrentPosX = this->w - m_fLoopStartPosX;
	////while (iCurrentPosX < this->h )
	////{
	////	iCurrentPosX += this->h;
	////	m_iLoopNumX++;
	////}

	////int iCurrentPosY = this->h - m_fLoopStartPosY;
	////while ( iCurrentPosY < this->w )
	////{
	////	iCurrentPosY += this->w;
	////	m_iLoopNumY++;
	////}
	
	int iCurrentPosX = static_cast<int>(this->w - m_fLoopStartPosX);
	if (this->m_fLoopSpeedX > 0.0f)
	{
		while (iCurrentPosX <= this->w )
		{
			iCurrentPosX += static_cast<int>(this->w);
			m_iLoopNumX++;
		}
	}

	int iCurrentPosY = static_cast<int>(this->h - m_fLoopStartPosY);
	if (this->m_fLoopSpeedY > 0.0f)
	{
		while ( iCurrentPosY <= this->h )
		{
			iCurrentPosY += static_cast<int>(this->h);
			m_iLoopNumY++;
		}
	}
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_LOOP::~EFFECT_LOOP() {

	::nn_commonLog(999,  "EFFECT_THROW END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_LOOP::task() {

	this->disp();

	switch (this->kind)
	{
		case enum_effect::EFFECT_ANIME_FLAT:
		{
			if (this->cnt <= this->intime)
			{
				this->x = LunaAnimation::FlatSpeed((float)this->isx, (float)this->iex, (float)this->intime, (float)this->cnt);
				this->y = LunaAnimation::FlatSpeed((float)this->isy, (float)this->iey, (float)this->intime, (float)this->cnt);
				this->w = LunaAnimation::FlatSpeed((float)this->isw, (float)this->iew, (float)this->intime, (float)this->cnt);
				this->h = LunaAnimation::FlatSpeed((float)this->ish, (float)this->ieh, (float)this->intime, (float)this->cnt);
				this->a = LunaAnimation::FlatSpeed((float)this->isa, (float)this->iea, (float)this->intime, (float)this->cnt);
				this->r = LunaAnimation::FlatSpeed((float)this->isr, (float)this->ier, (float)this->intime, (float)this->cnt);
			}
			else if (this->cnt <= this->intime + this->animtime)
			{
				int cur_cnt = this->cnt - this->intime;
				this->x = LunaAnimation::FlatSpeed((float)this->asx, (float)this->aex, (float)this->animtime, (float)cur_cnt);
				this->y = LunaAnimation::FlatSpeed((float)this->asy, (float)this->aey, (float)this->animtime, (float)cur_cnt);
				this->w = LunaAnimation::FlatSpeed((float)this->asw, (float)this->aew, (float)this->animtime, (float)cur_cnt);
				this->h = LunaAnimation::FlatSpeed((float)this->ash, (float)this->aeh, (float)this->animtime, (float)cur_cnt);
				this->a = LunaAnimation::FlatSpeed((float)this->asa, (float)this->aea, (float)this->animtime, (float)cur_cnt);
				this->r = LunaAnimation::FlatSpeed((float)this->asr, (float)this->aer, (float)this->animtime, (float)cur_cnt);
			}
			else if (this->cnt <= this->intime + this->animtime + this->outtime)
			{
				int cur_cnt = this->cnt - (this->intime + this->animtime);
				this->x = LunaAnimation::FlatSpeed((float)this->osx, (float)this->oex, (float)this->outtime, (float)cur_cnt);
				this->y = LunaAnimation::FlatSpeed((float)this->osy, (float)this->oey, (float)this->outtime, (float)cur_cnt);
				this->w = LunaAnimation::FlatSpeed((float)this->osw, (float)this->oew, (float)this->outtime, (float)cur_cnt);
				this->h = LunaAnimation::FlatSpeed((float)this->osh, (float)this->oeh, (float)this->outtime, (float)cur_cnt);
				this->a = LunaAnimation::FlatSpeed((float)this->osa, (float)this->oea, (float)this->outtime, (float)cur_cnt);
				this->r = LunaAnimation::FlatSpeed((float)this->osr, (float)this->oer, (float)this->outtime, (float)cur_cnt);
			}
			
			break;
		}
		case enum_effect::EFFECT_ANIME_ADD:
		{
			if (this->cnt <= this->intime)
			{
				this->x = LunaAnimation::AddSpeed((float)this->isx, (float)this->iex, (float)this->intime, (float)this->cnt);
				this->y = LunaAnimation::AddSpeed((float)this->isy, (float)this->iey, (float)this->intime, (float)this->cnt);
				this->w = LunaAnimation::AddSpeed((float)this->isw, (float)this->iew, (float)this->intime, (float)this->cnt);
				this->h = LunaAnimation::AddSpeed((float)this->ish, (float)this->ieh, (float)this->intime, (float)this->cnt);
				this->a = LunaAnimation::AddSpeed((float)this->isa, (float)this->iea, (float)this->intime, (float)this->cnt);
				this->r = LunaAnimation::AddSpeed((float)this->isr, (float)this->ier, (float)this->intime, (float)this->cnt);
			}
			else if (this->cnt <= this->intime + this->animtime)
			{
				int cur_cnt = this->cnt - this->intime;
				this->x = LunaAnimation::AddSpeed((float)this->asx, (float)this->aex, (float)this->animtime, (float)cur_cnt);
				this->y = LunaAnimation::AddSpeed((float)this->asy, (float)this->aey, (float)this->animtime, (float)cur_cnt);
				this->w = LunaAnimation::AddSpeed((float)this->asw, (float)this->aew, (float)this->animtime, (float)cur_cnt);
				this->h = LunaAnimation::AddSpeed((float)this->ash, (float)this->aeh, (float)this->animtime, (float)cur_cnt);
				this->a = LunaAnimation::AddSpeed((float)this->asa, (float)this->aea, (float)this->animtime, (float)cur_cnt);
				this->r = LunaAnimation::AddSpeed((float)this->asr, (float)this->aer, (float)this->animtime, (float)cur_cnt);
			}
			else if (this->cnt <= this->intime + this->animtime + this->outtime)
			{
				int cur_cnt = this->cnt - (this->intime + this->animtime);
				this->x = LunaAnimation::AddSpeed((float)this->osx, (float)this->oex, (float)this->outtime, (float)cur_cnt);
				this->y = LunaAnimation::AddSpeed((float)this->osy, (float)this->oey, (float)this->outtime, (float)cur_cnt);
				this->w = LunaAnimation::AddSpeed((float)this->osw, (float)this->oew, (float)this->outtime, (float)cur_cnt);
				this->h = LunaAnimation::AddSpeed((float)this->osh, (float)this->oeh, (float)this->outtime, (float)cur_cnt);
				this->a = LunaAnimation::AddSpeed((float)this->osa, (float)this->oea, (float)this->outtime, (float)cur_cnt);
				this->r = LunaAnimation::AddSpeed((float)this->osr, (float)this->oer, (float)this->outtime, (float)cur_cnt);
			}
			break;
		}
		case enum_effect::EFFECT_ANIME_SUB:
		{
			if (this->cnt <= this->intime)
			{
				this->x = LunaAnimation::SubSpeed((float)this->isx, (float)this->iex, (float)this->intime, (float)this->cnt);
				this->y = LunaAnimation::SubSpeed((float)this->isy, (float)this->iey, (float)this->intime, (float)this->cnt);
				this->w = LunaAnimation::SubSpeed((float)this->isw, (float)this->iew, (float)this->intime, (float)this->cnt);
				this->h = LunaAnimation::SubSpeed((float)this->ish, (float)this->ieh, (float)this->intime, (float)this->cnt);
				this->a = LunaAnimation::SubSpeed((float)this->isa, (float)this->iea, (float)this->intime, (float)this->cnt);
				this->r = LunaAnimation::SubSpeed((float)this->isr, (float)this->ier, (float)this->intime, (float)this->cnt);
			}
			else if (this->cnt <= this->intime + this->animtime)
			{
				int cur_cnt = this->cnt - this->intime;
				this->x = LunaAnimation::SubSpeed((float)this->asx, (float)this->aex, (float)this->animtime, (float)cur_cnt);
				this->y = LunaAnimation::SubSpeed((float)this->asy, (float)this->aey, (float)this->animtime, (float)cur_cnt);
				this->w = LunaAnimation::SubSpeed((float)this->asw, (float)this->aew, (float)this->animtime, (float)cur_cnt);
				this->h = LunaAnimation::SubSpeed((float)this->ash, (float)this->aeh, (float)this->animtime, (float)cur_cnt);
				this->a = LunaAnimation::SubSpeed((float)this->asa, (float)this->aea, (float)this->animtime, (float)cur_cnt);
				this->r = LunaAnimation::SubSpeed((float)this->asr, (float)this->aer, (float)this->animtime, (float)cur_cnt);
			}
			else if (this->cnt <= this->intime + this->animtime + this->outtime)
			{
				int cur_cnt = this->cnt - (this->intime + this->animtime);
				this->x = LunaAnimation::SubSpeed((float)this->osx, (float)this->oex, (float)this->outtime, (float)cur_cnt);
				this->y = LunaAnimation::SubSpeed((float)this->osy, (float)this->oey, (float)this->outtime, (float)cur_cnt);
				this->w = LunaAnimation::SubSpeed((float)this->osw, (float)this->oew, (float)this->outtime, (float)cur_cnt);
				this->h = LunaAnimation::SubSpeed((float)this->osh, (float)this->oeh, (float)this->outtime, (float)cur_cnt);
				this->a = LunaAnimation::SubSpeed((float)this->osa, (float)this->oea, (float)this->outtime, (float)cur_cnt);
				this->r = LunaAnimation::SubSpeed((float)this->osr, (float)this->oer, (float)this->outtime, (float)cur_cnt);
			}
			break;
		}
	}


	m_fLoopStartPosX -= m_fLoopSpeedX ;
	m_fLoopStartPosY -= m_fLoopSpeedY ;
	if (m_fLoopSpeedX > 0.0f)
	{
		if (m_fLoopStartPosX <= -this->w)
			m_fLoopStartPosX = 0.0f;
	}
	else if (m_fLoopSpeedX < 0.0f)
	{
		if (m_fLoopStartPosX >= 0.0f)
			m_fLoopStartPosX = -this->w;
	}

	if (m_fLoopSpeedY > 0.0f)
	{
		////if (m_fLoopStartPosY >= 0.0f)
		////	m_fLoopStartPosY = -this->h;
		if (m_fLoopStartPosY < -this->h)
			m_fLoopStartPosY = 0.0f;
	}
	////else if (m_fLoopSpeedY < 0.0f)
	////{
	////	////if (m_fLoopStartPosY <= -this->h)
	////	////	m_fLoopStartPosY = 0.0f;
	////	if (m_fLoopStartPosY >= this->h)
	////		m_fLoopStartPosY = 0.0f;
	////}
	////m_cBG.SetXY(m_cBG.GetX() + m_vec2DMoveSpeed.x, m_cBG.GetY() + m_vec2DMoveSpeed.y);



	this->cnt++;

	bool end_flag = false;
	if(this->mode == nn::yb::enum_effect::SLIDE_MODE_FRAME) {
		if(this->cnt >= this->frame) {
			end_flag = true;
		}
	}
	else if(this->mode == nn::yb::enum_effect::SLIDE_MODE_DISP) {
		if(
			(int)(this->x + this->w / 2) < 0 || 640 < (int)(this->x - this->w / 2)
			|| (int)(this->y + this->h / 2) < 0 || 480 < (int)(this->y - this->h / 2)//画面内からはみ出した場合

			|| (int)(this->h ) <= 0 || 0 >= (int)(this->h)//縮小して大きさが0になった場合
			) {
				end_flag = true;//タスク消去フラグを立てる
		}

		//もし最初に規定した最大フレームまで進んだら、カウントしない
		if (this->frame < cnt)
		{
			cnt = this->frame;
		}
	}

	if(end_flag) {
		if(this->ctrl != NULL) {
			this->ctrl->pop_event();
		}
		this->taskDelete();
	}
}


//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_LOOP::disp() {
	
	int x = (int)this->x;
	int y = (int)this->y;
	int w = (int)this->w;
	int h = (int)this->h;
	u16 a = (int)this->a;
	u16 r = (int)this->r;

	int b = (int)this->blend;


	float curr_pos_x = 0.0f;
	float curr_pos_y = 0.0f;

	for (int j = 0; j < (int)m_iLoopNumY; j++)
	{
		curr_pos_y = y + m_fLoopStartPosY + j * h;

		for (int i = 0; i < (int)m_iLoopNumX; i++)
		{
			curr_pos_x = x + m_fLoopStartPosX + i * w;

			int disp_w = static_cast<int>(this->w);
			int disp_h = static_cast<int>(this->h);

			::disp_commonObjDxCenterFull( this->objId, (int)curr_pos_x, (int)curr_pos_y, z, disp_w, disp_h, r, FALSE, FALSE, RGB_WHITE, a, b);
		}
	}	
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_LOOP * nn::yb::main::effect_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_LOOP * effectAnimArg) {

	nn::yb::main::EFFECT_LOOP * effect_anim = new nn::yb::main::EFFECT_LOOP( parent, effectAnimArg);

	return effect_anim;

}

//---------------------------------------------------------------------------
//初期値・終了値・全体値設定
nn::yb::main::EFFECT_LOOP * nn::yb::main::effect_loop_fadein_out( 
		TASK_SAVE * parent, int objId, int mode,
		int frame,
		int z, 
		int blend, 
		int kind, 
		//loop speed
		int speed_x, int speed_y,
		//fade in
		int intime,
		int isx, int iex, 
		int isy, int iey,
		int isw, int iew, 
		int ish, int ieh, 
		int isr, int ier, 
		int isa, int iea, 
		//animation
		int animtime,
		int asx, int aex, 
		int asy, int aey,
		int asw, int aew, 
		int ash, int aeh, 
		int asr, int aer, 
		int asa, int aea, 
		//fade out
		int outtime,
		int osx, int oex, 
		int osy, int oey,
		int osw, int oew, 
		int osh, int oeh, 
		int osr, int oer, 
		int osa, int oea, 
		nn::yb::main::CEffectCtrl * ctrlArg
	){

	nn::yb::main::EFFECT_LOOP effect_anim_arg;
	
	effect_anim_arg.ctrl = ctrlArg;

	effect_anim_arg.mode = mode;
	effect_anim_arg.objId = objId;
	effect_anim_arg.frame = frame;
	effect_anim_arg.kind = kind;
	effect_anim_arg.blend = blend;
	////angle %= 360;
	////effect_anim_arg.vX = (float)(::gSin[angle] * speed);
	////effect_anim_arg.vY = (float)(-::gCos[angle] * speed);
	effect_anim_arg.z = z;
	effect_anim_arg.w = (float)isw;//初期値の大きさにする(for debug)
	effect_anim_arg.h = (float)ish;
	effect_anim_arg.m_fLoopSpeedX = (float)speed_x;
	effect_anim_arg.m_fLoopSpeedY = (float)speed_y;

	//fade in
	effect_anim_arg.intime = intime;
	effect_anim_arg.isx = isx; effect_anim_arg.iex = iex; 
	effect_anim_arg.isy = isy; effect_anim_arg.iey = iey;
	effect_anim_arg.isw = isw; effect_anim_arg.iew = iew; 
	effect_anim_arg.ish = ish; effect_anim_arg.ieh = ieh; 
	effect_anim_arg.isr = isr; effect_anim_arg.ier = ier; 
	effect_anim_arg.isa = isa; effect_anim_arg.iea = iea; 
	//animation
	effect_anim_arg.animtime = animtime;
	effect_anim_arg.asx = asx; effect_anim_arg.aex = aex; 
	effect_anim_arg.asy = asy; effect_anim_arg.aey = aey;
	effect_anim_arg.asw = asw; effect_anim_arg.aew = aew; 
	effect_anim_arg.ash = ash; effect_anim_arg.aeh = aeh; 
	effect_anim_arg.asr = asr; effect_anim_arg.aer = aer; 
	effect_anim_arg.asa = asa; effect_anim_arg.aea = aea; 
	//fade out
	effect_anim_arg.outtime = outtime;
	effect_anim_arg.osx = osx; effect_anim_arg.oex = oex; 
	effect_anim_arg.osy = osy; effect_anim_arg.oey = oey;
	effect_anim_arg.osw = osw; effect_anim_arg.oew = oew; 
	effect_anim_arg.osh = osh; effect_anim_arg.oeh = oeh; 
	effect_anim_arg.osr = osr; effect_anim_arg.oer = oer; 
	effect_anim_arg.osa = osa; effect_anim_arg.oea = oea; 

	return nn::yb::main::effect_arg( parent, &effect_anim_arg);
}

//---------------------------------------------------------------------------
//added by nullpo 20090112 
//スペルカード共通エフェクト
nn::yb::main::EFFECT_SLIDE * nn::yb::main::effect_bg_mahouzin( TASK_SAVE * parent, int x, int y, int z, nn::yb::main::CEffectCtrl * ctrlArg) {

	nn::yb::main::EFFECT_SLIDE effect_slide_arg;
	
	effect_slide_arg.ctrl = ctrlArg;

	effect_slide_arg.mode = nn::yb::enum_effect::SLIDE_MODE_DISP;
	effect_slide_arg.x = (float)x;
	effect_slide_arg.y = (float)y;

	//座標以外の各種パラメータは固定
	int angle = ::n_commonRandN( 360 );
	int speed = 5 + ::n_commonRandN( 5) * 2;
	int obj_id = OBJ_ID_DUN_SPELL_BGCIRCLE_0;
	effect_slide_arg.objId = obj_id;
	angle %= 360;
	effect_slide_arg.vX = 0;
	effect_slide_arg.vY = 0;
	effect_slide_arg.z = 129;//注：キャラクタの後ろに描画(キャラはz=128)

	effect_slide_arg.w = 512;
	effect_slide_arg.h = 512;
	effect_slide_arg.vW = -effect_slide_arg.w / 200.0f;//200フレームで0になるよう縮小
	effect_slide_arg.vH = -effect_slide_arg.h / 200.0f;
	effect_slide_arg.a = 31;
	effect_slide_arg.vA = -0.05f;
	effect_slide_arg.r = (float)angle;
	effect_slide_arg.vR = 5.0f;

	effect_slide_arg.blend = DISP_COMMON_BLEND_MODE_ADD;


	return nn::yb::main::effect_out_arg( parent, &effect_slide_arg);

}

//---------------------------------------------------------------------------
//開始時衝撃波
nn::yb::main::EFFECT_SLIDE * nn::yb::main::effect_first_flash( TASK_SAVE * parent, int x, int y, int z, nn::yb::main::CEffectCtrl * ctrlArg) {

	//////衝撃波の生成
	////////nn::yb::main::effect_out_ex(parent, OBJ_ID_DUN_SPELL_FLASH_0, 320, 240, 1, 0, 0, 32, 150.0f, 32, 150.0f, 0, 0.0f, 31, -0.5f, DISP_COMMON_BLEND_MODE_ADD, ctrlArg);//拡大(第一波)
	////nn::yb::main::effect_out_ex(parent, OBJ_ID_DUN_SPELL_FLASH_0, 320, 240, 1, 0, 0, 32, 30.0f, 32, 30.0f, 0, 0.0f, 31, -0.3f, DISP_COMMON_BLEND_MODE_ADD, ctrlArg);//拡大(第二波)
	////nn::yb::main::effect_out_ex(parent, OBJ_ID_DUN_SPELL_SONIC_0, 320, 240, 1, 0, 0, 512, -10.0f, 512, -10.0f, 0, 0.0f, 0, 1.0f, DISP_COMMON_BLEND_MODE_ADD, ctrlArg);//収縮(第一波)

	////nn::yb::main::effect_start_end_total(parent, OBJ_ID_DUN_SPELL_FLASH_ELLIPSE_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
	////	30,
	////	320, 320, 10,
	////	240, 240, 10,
	////	256,
	////	32, 1024, 30,
	////	32, 1024, 30,
	////	0, 0, 10,
	////	31, 0, 30,
	////	DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_ADD, ctrlArg);//拡大(第二波)

	

	//拡大(第一波)
	nn::yb::main::EFFECT_SLIDE effect_slide_arg;
	
	effect_slide_arg.ctrl = ctrlArg;

	effect_slide_arg.mode = nn::yb::enum_effect::SLIDE_MODE_FRAME;
	effect_slide_arg.x = (float)x;
	effect_slide_arg.y = (float)y;

	//座標以外の各種パラメータは固定
	int angle = ::n_commonRandN( 360 );
	int speed = 5 + ::n_commonRandN( 5) * 2;
	int obj_id = OBJ_ID_DUN_SPELL_FLASH_0;
	effect_slide_arg.objId = obj_id;
	angle %= 360;
	effect_slide_arg.frame = 120;//表示フレーム
	effect_slide_arg.vX = 0;
	effect_slide_arg.vY = 0;
	effect_slide_arg.z = 1;

	effect_slide_arg.w = 32;
	effect_slide_arg.h = 32;
	effect_slide_arg.vW = 120.0f;//拡大
	effect_slide_arg.vH = 120.0f;
	effect_slide_arg.a = 31;
	effect_slide_arg.vA = -0.5f;
	effect_slide_arg.r = 0;
	effect_slide_arg.vR = 0.0f;

	effect_slide_arg.blend = DISP_COMMON_BLEND_MODE_ADD;

	return nn::yb::main::effect_out_arg( parent, &effect_slide_arg);

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_QUAKE::EFFECT_QUAKE() {

	::nn_commonLog(999,  "EFFECT_QUAKE CREATE\n");
	
	memset( this, 0, sizeof( nn::yb::main::EFFECT_QUAKE));

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_QUAKE::EFFECT_QUAKE( TASK_SAVE * parent, yb::main::EFFECT_QUAKE * effectArg) : ::TASK_SAVE( parent) {

	::nn_commonLog(999,  "EFFECT_QUAKE START\n");

	this->cnt = effectArg->cnt;
	this->kind = effectArg->kind;
	this->frame = effectArg->frame;
	this->ctrl = effectArg->ctrl;
	this->w = effectArg->w;
	this->h = effectArg->h;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_QUAKE::~EFFECT_QUAKE() {

	::nn_commonLog(999,  "EFFECT_QUAKE END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_QUAKE::task() {

	this->disp();


	this->cnt++;
	if(this->cnt > this->frame) {
		if(this->ctrl != NULL) {
			this->ctrl->pop_event();
		}
		this->taskDelete();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_QUAKE::disp() {

	int currW = (int)this->w;
	int currH = (int)this->h;
	switch (this->kind)
	{
		case nn::yb::enum_effect::EFFECT_ANIME_FLAT:
		break;
		case nn::yb::enum_effect::EFFECT_ANIME_ADD:
			currW = (int)LunaAnimation::FlatSpeed(0, this->w, (float)this->frame, (float)this->cnt);
			currH = (int)LunaAnimation::FlatSpeed(0, this->h, (float)this->frame, (float)this->cnt);
		break;
		case nn::yb::enum_effect::EFFECT_ANIME_SUB:
			currW = (int)LunaAnimation::FlatSpeed(this->w, 0, (float)this->frame, (float)this->cnt);
			currH = (int)LunaAnimation::FlatSpeed(this->h, 0, (float)this->frame, (float)this->cnt);
		break;
	}
	if (currW < 1)
		currW = 1;
	if (currH < 1)
		currH = 1;

	int offsetX = ::n_commonRandN(currW);
	int offsetY = ::n_commonRandN(currH);
	::disp_commonSetOffset(offsetX, offsetY);
}


//---------------------------------------------------------------------------

nn::yb::main::EFFECT_QUAKE * nn::yb::main::effect_quake( TASK_SAVE * parent, int kind, int frame, int w, int h, nn::yb::main::CEffectCtrl * ctrlArg ) {

	nn::yb::main::CEffectCtrl * ctrl = ctrlArg;

	nn::yb::main::EFFECT_QUAKE effect_arg;
	effect_arg.w = (float)w;
	effect_arg.h = (float)h;
	effect_arg.kind = kind;
	effect_arg.frame = frame;
	effect_arg.ctrl = ctrl;
	nn::yb::main::EFFECT_QUAKE * effect = new nn::yb::main::EFFECT_QUAKE( parent, &effect_arg);

	return effect;

}



//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MUSOFUINSAN::EFFECT_MUSOFUINSAN() {

	::nn_commonLog(999,  "EFFECT_MUSOFUINSAN CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::EFFECT_MUSOFUINSAN));

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MUSOFUINSAN::EFFECT_MUSOFUINSAN( TASK_SAVE * parent, nn::yb::main::EFFECT_MUSOFUINSAN * effectMusofuinArg) : ::TASK_SAVE( parent) {

	::nn_commonLog(999,  "EFFECT_MUSOFUINSAN START\n");

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MUSOFUINSAN::~EFFECT_MUSOFUINSAN() {

	::nn_commonLog(999,  "EFFECT_MUSOFUINSAN END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MUSOFUINSAN::task() {

	this->taskDelete();

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MUSOFUINSAN::disp() {



}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MUSOFUINSAN::call_back( void * effectMusoufuinArg, void * dummy) {

	nn::yb::main::EFFECT_MUSOFUINSAN * effect_musofuin = (nn::yb::main::EFFECT_MUSOFUINSAN *)effectMusoufuinArg;

	effect_musofuin->cnt--;
	if(effect_musofuin->cnt == 0) {
		if(effect_musofuin->ctrl != NULL) {
			effect_musofuin->ctrl->pop_event();
		}
		effect_musofuin->taskDelete();
	}

}

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

nn::yb::main::EFFECT_FRAME_ANIMATION::EFFECT_FRAME_ANIMATION() {
	////nn::yb::main::EFFECT_SLIDE( );

	::nn_commonLog(999,  "EFFECT_FRAME_ANIMATION CREATE\n");
	
	memset( this, 0, sizeof( nn::yb::main::EFFECT_FRAME_ANIMATION));


	this->objIdMax = 0;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_FRAME_ANIMATION::EFFECT_FRAME_ANIMATION( ::TASK_SAVE * parent, nn::yb::main::EFFECT_FRAME_ANIMATION * effectArg) : ::nn::yb::main::EFFECT_SLIDE( parent, effectArg ){

	::nn_commonLog(999,  "EFFECT_FRAME_ANIMATION START\n");

	this->objIdMax = effectArg->objIdMax;
	 
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_FRAME_ANIMATION::~EFFECT_FRAME_ANIMATION() {
	
	::nn_commonLog(999,  "EFFECT_FRAME_ANIMATION END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_FRAME_ANIMATION::task() {
	nn::yb::main::EFFECT_SLIDE::task( );
 
	if (this->cnt % 5 == 0)
		this->objId++ ;
	if (this->objId >= this->objIdMax)
	{
		this->objId = this->objIdMax;
	}
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_FRAME_ANIMATION::disp() {
	nn::yb::main::EFFECT_SLIDE::disp( );
 

}
//---------------------------------------------------------------------------

nn::yb::main::EFFECT_FRAME_ANIMATION * nn::yb::main::effect_frame_anime( TASK_SAVE * parent, int objId, int objIdMax, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg, int frame) {

	nn::yb::main::EFFECT_FRAME_ANIMATION effect_arg;
	
	effect_arg.ctrl = ctrlArg;

	effect_arg.mode = nn::yb::enum_effect::SLIDE_MODE_FRAME;
	effect_arg.objId = objId;
	effect_arg.frame = frame;
	effect_arg.x = (float)x;
	effect_arg.y = (float)y;
	angle %= 360;
	effect_arg.vX = (float)(::gSin[angle] * speed);
	effect_arg.vY = (float)(-::gCos[angle] * speed);
	effect_arg.z = z;

	effect_arg.w = (float)w;
	effect_arg.h = (float)h;
	effect_arg.vW = (float)vW;
	effect_arg.vH = (float)vH;
	effect_arg.a = (float)a;
	effect_arg.vA = (float)vA;
	effect_arg.r = (float)r;
	effect_arg.vR = (float)vR;

	effect_arg.blend = blend;

	effect_arg.objIdMax = objIdMax;

	return nn::yb::main::effect_out_arg( parent, &effect_arg);

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_FRAME_ANIMATION * nn::yb::main::effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_FRAME_ANIMATION * effectArg) {

	nn::yb::main::EFFECT_FRAME_ANIMATION * effect = new nn::yb::main::EFFECT_FRAME_ANIMATION( parent, effectArg);

	return effect;

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_HOLE::EFFECT_ANIM_HOLE() 
: nn::yb::main::EFFECT_ANIM() {

	::nn_commonLog(999,  "EFFECT_ANIM_HOLE CREATE\n");
	
	memset( this, 0, sizeof( nn::yb::main::EFFECT_ANIM_HOLE));
 
	this->pop_exec_cb = NULL;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_HOLE::EFFECT_ANIM_HOLE( ::TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_HOLE * effectAnimArg) : nn::yb::main::EFFECT_ANIM (parent, effectAnimArg) {

	::nn_commonLog(999,  "EFFECT_ANIM_HOLE START\n");

	this->pop_exec_cb = effectAnimArg->pop_exec_cb;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_HOLE::~EFFECT_ANIM_HOLE() {

	::nn_commonLog(999,  "EFFECT_ANIM_HOLE END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_ANIM_HOLE::task() {
	////nn::yb::main::EFFECT_ANIM::task( );
 
	this->disp();

	switch (this->kind)
	{
		case enum_effect::EFFECT_ANIME_FLAT:
		{
			this->x = LunaAnimation::FlatSpeed((float)this->sx, (float)this->ex, (float)this->tx, (float)this->cnt);
			this->y = LunaAnimation::FlatSpeed((float)this->sy, (float)this->ey, (float)this->ty, (float)this->cnt);
			this->w = LunaAnimation::FlatSpeed((float)this->sw, (float)this->ew, (float)this->tw, (float)this->cnt);
			this->h = LunaAnimation::FlatSpeed((float)this->sh, (float)this->eh, (float)this->th, (float)this->cnt);
			this->a = LunaAnimation::FlatSpeed((float)this->sa, (float)this->ea, (float)this->ta, (float)this->cnt);
			this->r = LunaAnimation::FlatSpeed((float)this->sr, (float)this->er, (float)this->tr, (float)this->cnt);
			break;
		}
		case enum_effect::EFFECT_ANIME_ADD:
		{
			this->x = LunaAnimation::AddSpeed((float)this->sx, (float)this->ex, (float)this->tx, (float)this->cnt);
			this->y = LunaAnimation::AddSpeed((float)this->sy, (float)this->ey, (float)this->ty, (float)this->cnt);
			this->w = LunaAnimation::AddSpeed((float)this->sw, (float)this->ew, (float)this->tw, (float)this->cnt);
			this->h = LunaAnimation::AddSpeed((float)this->sh, (float)this->eh, (float)this->th, (float)this->cnt);
			this->a = LunaAnimation::AddSpeed((float)this->sa, (float)this->ea, (float)this->ta, (float)this->cnt);
			this->r = LunaAnimation::AddSpeed((float)this->sr, (float)this->er, (float)this->tr, (float)this->cnt);
			break;
		}
		case enum_effect::EFFECT_ANIME_SUB:
		{
			this->x = LunaAnimation::SubSpeed((float)this->sx, (float)this->ex, (float)this->tx, (float)this->cnt);
			this->y = LunaAnimation::SubSpeed((float)this->sy, (float)this->ey, (float)this->ty, (float)this->cnt);
			this->w = LunaAnimation::SubSpeed((float)this->sw, (float)this->ew, (float)this->tw, (float)this->cnt);
			this->h = LunaAnimation::SubSpeed((float)this->sh, (float)this->eh, (float)this->th, (float)this->cnt);
			this->a = LunaAnimation::SubSpeed((float)this->sa, (float)this->ea, (float)this->ta, (float)this->cnt);
			this->r = LunaAnimation::SubSpeed((float)this->sr, (float)this->er, (float)this->tr, (float)this->cnt);
			break;
		}
	}

	this->cnt++;

	bool end_flag = false;
	if(this->mode == nn::yb::enum_effect::SLIDE_MODE_FRAME) {
		if(this->cnt >= this->frame) {
			end_flag = true;
		}
	}
	else if(this->mode == nn::yb::enum_effect::SLIDE_MODE_DISP) {
		if(
			(int)(this->x + this->w / 2) < 0 + camera_offset_x || 640 + camera_offset_x < (int)(this->x - this->w / 2)
			|| (int)(this->y + this->h / 2) < 0 + camera_offset_y || 480 + camera_offset_y < (int)(this->y - this->h / 2)//画面内からはみ出した場合

			|| (int)(this->h ) <= 0 || 0 >= (int)(this->h)//縮小して大きさが0になった場合
			) {
				end_flag = true;//タスク消去フラグを立てる
		}

		//もし最初に規定した最大フレームまで進んだら、カウントしない
		if (this->frame < cnt)
		{
			cnt = this->frame;
		}
	}

	if(end_flag) {
		////this->ctrl->pop_hole_event();
		if ((this->pop_exec_cb) != NULL)	
			(this->pop_exec_cb)(this->ctrl, NULL);//直接pop_eventではなくcallbackを呼ぶ
		this->taskDelete();
	}
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_ANIM_HOLE::disp() {
	nn::yb::main::EFFECT_ANIM::disp( );

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_HOLE * nn::yb::main::effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_HOLE * effectAnimArg) {

	nn::yb::main::EFFECT_ANIM_HOLE * effect_anim = new nn::yb::main::EFFECT_ANIM_HOLE( parent, effectAnimArg);

	return effect_anim;
}

//---------------------------------------------------------------------------
//初期値・終了値・全体値設定
nn::yb::main::EFFECT_ANIM_HOLE * nn::yb::main::effect_start_end_total_hole(
	 TASK_SAVE * parent, int objId, int mode,
	 int frame,
	 int sx, int ex, int tx,
	 int sy, int ey, int ty,
	 int z, 
	 int sw, int ew, int tw,
	 int sh, int eh, int th,
	 int sr, int er, int tr,
	 int sa, int ea, int ta,
	 int blend, 
	 int kind, 
	 GXRgb color,
	 nn::yb::main::CEffectCtrl * ctrlArg,
	 void (*pop_exec_cb)(void *, void *)
													 
	 ) {

	nn::yb::main::EFFECT_ANIM_HOLE effect_anim_arg;
	
	effect_anim_arg.ctrl = ctrlArg;

	effect_anim_arg.mode = mode;
	effect_anim_arg.objId = objId;
	effect_anim_arg.frame = frame;
	effect_anim_arg.kind = kind;

	effect_anim_arg.sx = sx; effect_anim_arg.ex = ex; effect_anim_arg.tx = tx;
	effect_anim_arg.sy = sy; effect_anim_arg.ey = ey; effect_anim_arg.ty = ty;
	effect_anim_arg.z = z;
	effect_anim_arg.sw = sw; effect_anim_arg.ew = ew; effect_anim_arg.tw = tw;
	effect_anim_arg.sh = sh; effect_anim_arg.eh = eh; effect_anim_arg.th = th;
	effect_anim_arg.sr = sr; effect_anim_arg.er = er; effect_anim_arg.tr = tr;
	effect_anim_arg.sa = sa; effect_anim_arg.ea = ea; effect_anim_arg.ta = ta;
	
	effect_anim_arg.blend = blend;

	effect_anim_arg.color = color;

	effect_anim_arg.pop_exec_cb = pop_exec_cb;
	
	return nn::yb::main::effect_out_arg( parent, &effect_anim_arg);
}
//---------------------------------------------------------------------------
