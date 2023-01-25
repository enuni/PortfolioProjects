
#include <math.h>

#include "nn_common.h"

#include "disp_common.h"
#include "touch.h"
#include "font.h"
#include "sound_nns.h"
#include "key_common.h"
#include "sin_cos.h"

#include "ball.h"
#include "edit_hole_obj.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "animation_data.h"

#include "enum.h"

#include "yb_common.h"

//---------------------------------------------------------------------------
void nn::yb::common::yb_commonDispObjRepeatScale( int objId, int x1, int y1, int x2, int y2, int z,
								 int xCenter, int yCenter, double hScale, double vScale,
								 BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend
) {
	OBJ * obj = disp_commonGetObj( objId);

	int w = x2 - x1;
	int h = y2 - y1;
	int center_x = (int)(xCenter + ((x1 + w / 2.0) - xCenter) * hScale);
	int center_y = (int)(yCenter + ((y1 + h / 2.0) - yCenter) * vScale);
	
	int y = y1;
	int scale_y = center_y - (int)(h / 2.0 * vScale);

	while(y < y2) {

		int disp_h;
		if(y2 - y > obj->h) {
			disp_h = obj->h;
		}
		else {
			disp_h = y2 - y;
		}
		int scale_h = (int)(disp_h * vScale);

		int x = x1;
		int scale_x = center_x - (int)(w / 2.0 * hScale);

		while(x < x2) {

			int disp_w;
			if(x2 - x > obj->w) {
				disp_w = obj->w;
			}
			else {
				disp_w = x2 - x;
			}
			int scale_w = (int)(disp_w * hScale);

			disp_commonObjUvWhFull( obj->texture_id, obj->u, obj->v, disp_w, disp_h, scale_x, scale_y, z,
				scale_w, scale_h, 0, flipH, flipV, rgb, alpha, blend);
			x += disp_w;
			scale_x += scale_w;

		}
		y += disp_h;
		scale_y += scale_h;

	}

}
//---------------------------------------------------------------------------
void nn::yb::common::yb_commonDispGroundScale(  int objId, int x1, int y1, int x2, int y2, int z, int wUnit, int hUnit,
											  int xCenter, int yCenter, double hScale, double vScale,
											  BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend
) {
	OBJ * obj = disp_commonGetObj( objId);

	int old_scale_y = yCenter + (int)((y1 - yCenter) * vScale);
	int old_scale_h = 0;

	int y = y1;
	int h = y2 - y1;
	while(h > 1) {

		int disp_h;
		if(h > hUnit) {
			disp_h = hUnit;
			h -= hUnit;
		}
		else {
			disp_h = h;
			h = 0;
		}
		int v = obj->v + (y % hUnit);
		if(flipV) {
			v = obj->v + (hUnit * 2 - (y % hUnit) - disp_h);
		}

		int scale_y = yCenter + (int)((y - yCenter) * vScale);
		int scale_h = (int)(ceil(disp_h * vScale));
		scale_y += (old_scale_y + old_scale_h) - scale_y;
		scale_h += scale_y - (old_scale_y + old_scale_h);
		old_scale_y = scale_y;
		old_scale_h = scale_h;

		int old_scale_x = xCenter + (int)((x1 - xCenter) * hScale);
		int old_scale_w = 0;

		int x = x1;
		int w = x2 - x1;
		while(w > 1) {

			int disp_w;
			if(w > wUnit) {
				disp_w = wUnit;
				w -= wUnit;
			}
			else {
				disp_w = w;
				w = 0;
			}
			int u = obj->u + (x % wUnit);
			if(flipH) {
				u = obj->u + (wUnit * 2 - (x % wUnit) - disp_w);
			}

			int scale_x = xCenter + (int)((x - xCenter) * hScale);
			int scale_w = (int)(ceil(disp_w * hScale));
			scale_x += (old_scale_x + old_scale_w) - scale_x;
			scale_w += scale_x - (old_scale_x + old_scale_w);
			old_scale_x = scale_x;
			old_scale_w = scale_w;
			
			disp_commonObjUvWhFull( obj->texture_id, u, v, disp_w, disp_h, scale_x, scale_y, z,
				scale_w, scale_h, 0, flipH, flipV, rgb, alpha, blend);
			x += disp_w;

		}
		y += disp_h;

	}
}
//---------------------------------------------------------------------------
void nn::yb::common::yb_commonDispGroundEx( int objId, int x1, int y1, int x2, int y2, int z, int wUnit, int hUnit,
										   BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend
) {

	OBJ * obj = disp_commonGetObj( objId);

	int y = y1;
	int h = y2 - y1;
	while(h > 1) {

		int disp_h;
		if(h > hUnit) {
			disp_h = hUnit;
			h -= hUnit;
		}
		else {
			disp_h = h;
			h = 0;
		}
		int v = obj->v + (y % hUnit);
		if(flipV) {
			v = obj->v + (hUnit * 2 - (y % hUnit) - disp_h);
		}

		int x = x1;
		int w = x2 - x1;
		while(w > 1) {

			int disp_w;
			if(w > wUnit) {
				disp_w = wUnit;
				w -= wUnit;
			}
			else {
				disp_w = w;
				w = 0;
			}
			int u = obj->u + (x % wUnit);
			if(flipH) {
				u = obj->u + (wUnit * 2 - (x % wUnit) - disp_w);
			}
			disp_commonObjUvFull( obj->texture_id, u, v, disp_w, disp_h, x, y, z,
				0, flipH, flipV, rgb, alpha, blend);
			x += disp_w;

		}
		y += disp_h;

	}

}

//---------------------------------------------------------------------------
void nn::yb::common::yb_commonDispGround( int objId, int x1, int y1, int x2, int y2, int z, int wUnit, int hUnit) {

	yb_commonDispGroundEx( objId, x1, y1, x2, y2, z, wUnit, hUnit,
		FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
	);

}
//---------------------------------------------------------------------------
nn::yb::common::CButton::CButton(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 777, "CButton Create\n");
	this->priority = TASK_DEFAULT_PRIORITY - 10;

	this->state = nn::yb::e::button::STATE_NONE;
	this->oldState = nn::yb::e::button::STATE_NONE;
	this->waitCnt = 0;

	this->x = 0;
	this->y = 0;
	this->w = 0;
	this->h = 0;
	this->id = 0;

	this->onCb = NULL;
	this->onCbArg = NULL;

	this->enableFlag = false;
	this->color = GX_RGB( 0, 0, 0);
	this->darkColor = GX_RGB( 0, 0, 0);

}
//---------------------------------------------------------------------------
nn::yb::common::CButton::~CButton() {

	::nn_commonLog( 777, "CButton Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::common::CButton::start() {

	this->enableFlag = true;

}
//---------------------------------------------------------------------------
void nn::yb::common::CButton::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CButton::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::TOUCH_WORK * touch = &this->master->touchWork;

	if(this->enableFlag && this->state != nn::yb::e::button::STATE_HIDE) {
		if(touch->touchPoint & TOUCH_TOUCH) {
			if(this->x <= touch->x && touch->x <= this->x + this->w &&
				this->y <= touch->y && touch->y <= this->y + this->h
			) {
				touch->touchPoint = 0;
				if(this->onCb != NULL) {
					this->onCb( this->onCbArg, this);
				}
			}
		}
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::common::CButton::disp() {
	
	int alpha = 31;
	if(!this->enableFlag) {
		alpha = 16;
	}
	switch(this->state) {
		case nn::yb::e::button::STATE_ON:
			disp_commonMain3dBoard( this->x, this->y, -500, this->w, this->h, this->color, alpha);
			fontPutsType( this->x + 2, this->y, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, this->caption);
			break;
		case nn::yb::e::button::STATE_OFF:
			disp_commonMain3dBoard( this->x, this->y, -500, this->w, this->h, this->darkColor, alpha);
			fontPutsType( this->x + 2, this->y, nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, this->caption);
			break;
		case nn::yb::e::button::STATE_HIDE:
			break;
	}
	

}
//---------------------------------------------------------------------------
nn::yb::common::CDialog::CDialog(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CDialog Create\n");
	this->state = nn::yb::e::dialog::STATE_NONE;
	this->oldState = nn::yb::e::dialog::STATE_NONE;
	this->waitCnt = 0;

	this->w = 240;
	this->h = 70;
	this->x = 320 - this->w / 2;
	this->y = 240 - this->h / 2;
	this->buttonN = 0;
	this->buttonNo = 0;
	memset( this->msg, 0, sizeof( this->msg));
	memset( this->buttonCaption, 0, sizeof( this->buttonCaption));
	memset( this->button, 0, sizeof( this->button));
	this->onCb = NULL;
	this->onArg = NULL;

	this->cursorX = 0;

	this->priority = 3;

}
//---------------------------------------------------------------------------
nn::yb::common::CDialog::~CDialog() {

	::nn_commonLog( 1, "CDialog Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::common::CDialog::start() {

	int button_w = 100;
	int button_h = 14 + 1;
	int button_x;
	int button_y = this->y + 40;
	for( int i = 0; i < this->buttonN; i++) {

		button_x = this->x + 10 + (button_w + 5) * i;
		this->button[i].x = button_x;
		this->button[i].y = button_y;
		this->button[i].w = button_w;
		this->button[i].h = button_h;

	}

}
//---------------------------------------------------------------------------
void nn::yb::common::CDialog::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CDialog::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	::TOUCH_WORK * touch = &this->master->touchWork;
	::KEYBOARD_WORK * kb = &this->master->keyboardWork;

	if(touch->touchPoint & TOUCH_TOUCH) {
		for( int i = 0; i < this->buttonN; i++) {

			nn::common::def::graphics2d::RECT * rect = &this->button[i];
			if(NN_COMMON_IS_POS_RECT( touch->x, touch->y, rect->x, rect->y, rect->w, rect->h)) {
				this->buttonNo = i;
				if(this->onCb) {
					this->onCb( this->onArg, this);
				}
				this->taskDelete();
				break;
			}

		}
	}
	else if(key_commonCursorMoveLR( key, this->buttonN - 1, &this->cursorX)) {

	}
	else if(kb->trigger[KEYBOARD_RETURN]) {
		this->buttonNo = this->cursorX;
		if(this->onCb) {
			this->onCb( this->onArg, this);
		}
		this->taskDelete();
	}

	memset( touch, 0, sizeof( TOUCH_WORK));
	key->press = 0;
	key->pulse = 0;
	key->trigger = 0;
	key->hold = 0;
	memset( kb, 0, sizeof( KEYBOARD_WORK));

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::common::CDialog::disp() {
	
	disp_commonMain3dBoard( this->x, this->y, -510, this->w, this->h, GX_RGB( 0, 0, 31), 31);
	fontPutsType( this->x + 10, this->y + 10, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_WHITE, this->msg);

	int x, y, w, h;
	for( int i = 0; i < this->buttonN; i++) {

		x = this->button[i].x;
		y = this->button[i].y;
		w = this->button[i].w;
		h = this->button[i].h;
		disp_commonMain3dBoard( x, y, -511, w, h, GX_RGB( 8, 8, 8), 31);
		GXRgb color = RGB_WHITE;
		if(i == this->cursorX) {
			color = RGB_RED;
		}
		fontPutsType( x + 1, y + 1, nn::yb::e::global::FONT_TYPE_SIZE_18, color, this->buttonCaption[i]);

	}

}
//---------------------------------------------------------------------------

void nn::yb::common::yb_commonDispScoreList( nn::yb::common::DISP_SCORE_LIST_DATA * data) {
	
	int score_list_shift_x = data->x;//layout_score_list->x + data->dispShiftX;
	int score_list_shift_y = data->y;//layout_score_list->y + data->dispShiftY;

	disp_commonMain3dObj( OBJ_ID_MAIN_SCORE_LIST_0, score_list_shift_x, score_list_shift_y, data->z);

	LAYOUT * layout_hole_no = disp_commonGetLayoutMain3d( LAYOUT_ID_SCORE_LIST_HOLE_NO);
	LAYOUT * layout_hole_no_2column = disp_commonGetLayoutMain3d( LAYOUT_ID_SCORE_LIST_HOLE_NO_2COLUMN);
	LAYOUT * layout_hole_no_2row = disp_commonGetLayoutMain3d( LAYOUT_ID_SCORE_LIST_HOLE_NO_2ROW);
	int w_2column = layout_hole_no_2column->x - layout_hole_no->x;
	int h_2row = layout_hole_no_2row->y - layout_hole_no->y;
	for( int i = 0; i < 8; i++) {

		int x = layout_hole_no->x + (i % 2) * w_2column + score_list_shift_x;
		int y = layout_hole_no->y + (i / 2) * h_2row + score_list_shift_y;
		disp_commonMain3dObjNumber( i + 1, layout_hole_no->obj_id, x, y, data->z - 1, 0);

	}

	LAYOUT * layout_score_1 = disp_commonGetLayoutMain3d( LAYOUT_ID_SCORE_LIST_SCORE_1UNIT);
	LAYOUT * layout_score_2 = disp_commonGetLayoutMain3d( LAYOUT_ID_SCORE_LIST_SCORE_2UNIT);
	int score_disp_cnt = 0;
	for( score_disp_cnt = 0; score_disp_cnt < data->holeCnt - 1; score_disp_cnt++) {

		int x = layout_score_1->x + (score_disp_cnt % 2) * w_2column + score_list_shift_x;
		int y = layout_score_1->y + (score_disp_cnt / 2) * h_2row + score_list_shift_y;
		int w = layout_score_2->x - layout_score_1->x;
		disp_commonMain3dObjNumberColor( data->score[score_disp_cnt], layout_score_1->obj_id, x, y, data->z - 1, w, GX_RGB( 8, 8, 8), 31);

	}
	if(data->nowHoleDispFlag) {
		int x = layout_score_1->x + (score_disp_cnt % 2) * w_2column + score_list_shift_x;
		int y = layout_score_1->y + (score_disp_cnt / 2) * h_2row + score_list_shift_y;
		int w = layout_score_2->x - layout_score_1->x;
		disp_commonMain3dObjNumberColor( data->shotCnt, layout_score_1->obj_id, x, y, data->z - 1, w, GX_RGB( 8, 8, 8), 31);
	}
	// トータル
	if(data->totalDispFlag) {
		LAYOUT * layout_total_1 = disp_commonGetLayoutMain3d( LAYOUT_ID_SCORE_LIST_TOTAL_1UNIT);
		LAYOUT * layout_total_2 = disp_commonGetLayoutMain3d( LAYOUT_ID_SCORE_LIST_TOTAL_2UNIT);
		int disp_total = data->totalScore;
		disp_commonMain3dObjNumberColor( disp_total, layout_total_1->obj_id,
			layout_total_1->x + score_list_shift_x, layout_total_1->y + score_list_shift_y,
			data->z - 1, layout_total_2->x - layout_total_1->x,
			GX_RGB( 0, 0, 0), 31
		);
	}
	
}

//---------------------------------------------------------------------------

void nn::yb::common::yb_commonPlaySeq( int seqNo) {

	sound_nnsStopSeq();
	sound_nnsPlaySeq( seqNo);

}

//---------------------------------------------------------------------------

void nn::yb::common::yb_commonCalcSpin( nn::yb::common::CALC_SPIN_DATA * data) {

	nn::yb::common::yb_commonAddAngle( data->angle, *data->spinHorizon);
	*data->spinHorizon *= nn::yb::e::global::FIELD_DEC_SPIN_BASE_RATE;

}

//---------------------------------------------------------------------------

void nn::yb::common::yb_commonCalcMove( nn::yb::common::CALC_MOVE_DATA * data) {
	
	*data->addX = nn::yb::common::yb_commonAngleSpeed2MoveX( data->angle, *data->speed);
	*data->addY = nn::yb::common::yb_commonAngleSpeed2MoveY( data->angle, *data->speed);

	double dec_speed_bound = nn::yb::e::global::FIELD_DEC_SPEED_BASE_BOUND_RATE;
	double dec_speed = nn::yb::e::global::FIELD_DEC_SPEED_BASE;
	double dec_speed_fly = nn::yb::e::global::FIELD_DEC_SPEED_BASE_FLY;
	switch(data->ballForm) {
	case nn::yb::e::ball::BALL_FORM_PRESS:
		{
			dec_speed_bound = 0.0;
		}
		break;
	case nn::yb::e::ball::BALL_FORM_TORNADO:
		{
			dec_speed = nn::yb::e::global::FIELD_TORNADO_DEC_SPEED_BASE;
		}
		break;
	case nn::yb::e::ball::BALL_FORM_ORIN:
	case nn::yb::e::ball::BALL_FORM_VAMPIRE:
		{
			dec_speed_fly = 0.0;
			dec_speed = 0.0;
		}
		break;
	case nn::yb::e::ball::BALL_FORM_BURN:
		{
			dec_speed = dec_speed_fly;
		}
		break;
	}

	*data->x = *data->x + *data->addX;
	*data->y = *data->y + *data->addY;
	if(data->flyBoundFlag) {
		*data->speed -= dec_speed_bound;
	}
	else {
		if(data->z == 0.0) {
			*data->speed -= dec_speed;
		}
		else {
			*data->speed -= dec_speed_fly;
		}
	}
	if(*data->speed <= 0.0) {
		*data->speed = 0.0;
	}

}

//---------------------------------------------------------------------------

void nn::yb::common::yb_commonCalcShot( nn::yb::common::CALC_SHOT_DATA * data) {

	double base_power;
	double base_z_power;
	if(data->powerState == 0) {
		if(data->flyState == nn::yb::e::ball::FLY_STATE_ON) {
			base_power = nn::yb::e::global::SHOT_BASE_SPEED_FLY_NORMAL;
			*data->rotHorizon = (double)data->cursorSpinHrizon * nn::yb::e::global::FLY_SPIN_HORIZON_FLY_RATE;
		}
		else {
			base_power = nn::yb::e::global::SHOT_BASE_SPEED_NORMAL;
			*data->rotHorizon = (double)data->cursorSpinHrizon * nn::yb::e::global::FLY_SPIN_HORIZON_RATE;
			// パワーが小さい場合、カーブし過ぎる場合
			//*data->rotHorizon = (double)data->cursorSpinHrizon * 0.666 * data->power / nn::yb::e::global::SHOT_BASE_MAX_POWER;
		}
		base_z_power = nn::yb::e::global::SHOT_BASE_Z_SPEED_NORMAL;
	}
	else {
		if(data->flyState == nn::yb::e::ball::FLY_STATE_ON) {
			base_power = nn::yb::e::global::SHOT_BASE_SPEED_FLY_FULL;
			*data->rotHorizon = (double)data->cursorSpinHrizon * nn::yb::e::global::FLY_SPIN_HORIZON_FLY_RATE;	// フライはフルパワーのみ
		}
		else {
			base_power = nn::yb::e::global::SHOT_BASE_SPEED_FULL;
			*data->rotHorizon = (double)data->cursorSpinHrizon * nn::yb::e::global::FLY_SPIN_HORIZON_RATE;
			// パワーが小さい場合、カーブし過ぎる場合
			//*data->rotHorizon = (double)data->cursorSpinHrizon * 0.666 * 2.0 * data->power / nn::yb::e::global::SHOT_BASE_MAX_POWER;
		}
		base_z_power = nn::yb::e::global::SHOT_BASE_Z_SPEED_FULL;
	}
	nn_commonLog( 101226, "*data->rotHorizon = %f\n", *data->rotHorizon);

	*data->speed = base_power * (10.0 + data->power) / (nn::yb::e::global::SHOT_BASE_MAX_POWER + 10.0);

	if(data->flyState == nn::yb::e::ball::FLY_STATE_ON) {
		*data->zSpeed = base_z_power * data->power / nn::yb::e::global::SHOT_BASE_MAX_POWER;
		*data->rotVertical = (double)data->stopSpinVertical /
			(double)nn::yb::e::global::SHOT_SPIN_VERTICAL_MOVE_DIV * 1.5;
	}
	else {
		*data->zSpeed = 0.0;
	}

}

//---------------------------------------------------------------------------

double nn::yb::common::yb_commonVec2angle( double x, double y) {

	nn::common::def::graphics2d::VECTOR2_D vec0;
	vec0.x = 1000.0;
	vec0.y = 0.0;

	nn::common::def::graphics2d::VECTOR2_D vec;
	vec.x = x;
	vec.y = y;

	return nn::yb::common::yb_commonVec2angle( &vec0, &vec);

}

//---------------------------------------------------------------------------

double nn::yb::common::yb_commonVec2angle( nn::common::def::graphics2d::VECTOR2_D * vec0, nn::common::def::graphics2d::VECTOR2_D * vec) {

	double dot = nn::common::graphics2d::vec2dot( vec0, vec);
	double cross = nn::common::graphics2d::vec2cross( vec0, vec);
	double after_angle = NN_COMMON_VEC2ANGLE2D( dot, cross);
	if(after_angle < 0.0) {
		after_angle += 360.0;
	}
	after_angle = 360.0 - after_angle;

	return after_angle;

}

//---------------------------------------------------------------------------
void nn::yb::common::yb_commonCalcDown( nn::yb::common::CALC_BALL_DATA * data) {
	
	*data->flyBoundFlag = false;
	*data->groundOnFlag = false;
	*data->addZ = 0.0;

	double fiying_g = nn::yb::e::global::FLYING_G;

	double ground_side_speed_up_rate = nn::yb::e::global::FLY_GROUND_SIDE_SPEED_UP_RATE;
	switch(data->ballForm) {
	case nn::yb::e::ball::BALL_FORM_PRESS:
		{
			fiying_g = nn::yb::e::global::FLYING_PRESS_G;
			ground_side_speed_up_rate = nn::yb::e::global::FLY_GROUND_PRESS_SIDE_SPEED_UP_RATE;
		}
		break;
	case nn::yb::e::ball::BALL_FORM_TORNADO:
		{
			if(*data->zSpeed >= 0.0) {
				fiying_g = nn::yb::e::global::FLYING_TORNADO_UP_G;
			}
			else {
				fiying_g = nn::yb::e::global::FLYING_TORNADO_DOWN_G;
			}
		}
		break;
	case nn::yb::e::ball::BALL_FORM_BURN:
	case nn::yb::e::ball::BALL_FORM_VAMPIRE:
		{
			fiying_g = 0.0;
			ground_side_speed_up_rate = 1.0;
		}
		break;
	case nn::yb::e::ball::BALL_FORM_ORIN:
		{
			fiying_g = nn::yb::e::global::FLYING_G * 2.0;
		}
		break;
	}

	if(( !data->groundFlag && !data->groundSideFlag ) || *data->z < -24.0) {
		*data->addZ = *data->zSpeed;
		*data->z += *data->addZ;
		*data->zSpeed -= fiying_g;
	}
	else if( !data->groundFlag && data->groundSideFlag && -24.0 <= *data->z && *data->z <= 0.0) {
		nn_commonLog( 110303, "地面の縁\n");

		*data->addZ = *data->zSpeed;
		*data->z += *data->addZ;
		*data->zSpeed -= fiying_g * nn::yb::e::global::FLY_GROUND_SIDE_G_RATE;

		if(*data->speed < nn::yb::e::global::FLY_GROUND_SIDE_SPEED_UP_MAX_SPEED) {
			*data->speed *= ground_side_speed_up_rate;
		}
	}
	// ToDo 110412
	// zSpeed一定以上で落下 ゴロで地面の縁と区別
	else {
		*data->addZ = *data->zSpeed;
		double z_after = *data->z + *data->addZ;

		if(*data->z == 0.0 && *data->zSpeed == 0.0) {
			// ゴロ
			*data->groundOnFlag = true;
			return;
		}
		else if(*data->z >= 0.0 && z_after < 0.0) {
			// 110412
			if(data->cupFlag) {
				*data->cupinFlag = true;
			}
			else {
				// 跳ね返り
				nn_commonLog( 110412, "*data->z = %f z_after = %f\n", *data->z, z_after);
				nn_commonLog( 101226, "*data->rotHorizon * 30.0 = %f\n", *data->rotHorizon * 30.0);
				
				if(data->ballForm == nn::yb::e::ball::BALL_FORM_PRESS) {
					*data->z = 0.0;
					*data->zSpeed = 0.0;
				}
				else if(data->ballForm == nn::yb::e::ball::BALL_FORM_TORNADO) {
					*data->z = 0.0;
					*data->zSpeed = 0.0;
				}
				else if(data->ballForm == nn::yb::e::ball::BALL_FORM_SPEAR) {
					*data->z = 0.0;
					*data->zSpeed = 0.0;
				}
				else if(data->ballForm == nn::yb::e::ball::BALL_FORM_ORIN) {
					*data->z = 0.0;
					*data->zSpeed = 0.0;
				}
				else {
					// data->ballForm == nn::yb::e::ball::BALL_FORM_DEFAULT
					// 水平
					nn::yb::common::yb_commonAddAngle( data->angle, *data->rotHorizon);
					*data->rotHorizon = 0.0;
					
					// 垂直
					if(*data->rotVertical != 0.0) {
						if(*data->rotVertical >= 0) {
							*data->speed += (*data->rotVertical * nn::yb::e::global::FLY_SPIN_VERTICAL_TOP_RATE);
						}
						else {
							*data->speed += (*data->rotVertical * nn::yb::e::global::FLY_SPIN_VERTICAL_BACK_RATE);
						}
						
						if(*data->speed <= 0.0) {
							nn::yb::common::yb_commonAddAngle( data->angle, 180.0);
							*data->speed = -*data->speed;
							*data->rotVertical = -*data->rotVertical;
						}
#if 1
						*data->rotVertical = 0.0;
#else
						// 2バウンド目以降で向きが変わるのは微妙
						*data->rotVertical = *data->rotVertical * 0.150;
#endif
					}
					// めり込まないよう
					*data->z = 0.0;
					*data->zSpeed = -*data->zSpeed;
					*data->zSpeed = *data->zSpeed * nn::yb::e::global::FLY_BOUND_Z_SPEED_RATE;
					double stop_check_speed = 0.5;
					if(-stop_check_speed < *data->zSpeed && *data->zSpeed < stop_check_speed &&
						-stop_check_speed < *data->z && *data->z < stop_check_speed
					) {
						*data->zSpeed = 0.0;
					}
				}
				*data->flyBoundFlag = true;
				*data->groundOnFlag = true;
			}
		}
		else if(*data->z < 0.0 && *data->zSpeed < 0.0) {
			// 地面よりちょっと下で地面と衝突
			// 下向きの場合、地面復帰でそれ以上落ちない
			*data->z = 0.0;
			*data->zSpeed = 0.0;
		}
		else {
			// 上向きの場合、空中と同じ
			// 空中
			*data->addZ = *data->zSpeed;
			*data->z += *data->addZ;
			*data->zSpeed -= fiying_g;
		}
	}

}

//---------------------------------------------------------------------------

double nn::yb::common::yb_commonAngleSpeed2MoveX( double angle, double speed) {

	int a = (int)angle;

	if(a < 0 || 360 <= a) {
		a = 0;
	}
	return gCos[a] * speed;

}

//---------------------------------------------------------------------------

double nn::yb::common::yb_commonAngleSpeed2MoveY( double angle, double speed) {

	int a = (int)angle;

	if(a < 0 || 360 <= a) {
		a = 0;
	}
	return -gSin[a] * speed;

}

//---------------------------------------------------------------------------

void nn::yb::common::yb_commonCalcBumper( nn::yb::common::CALC_BUMPER_DATA * data) {

	if((int)data->paraZ < 0 || data->paraBumperZ < (int)data->paraZ) {
		*data->returnResult = false;
		return;
	}
	
	nn::common::def::graphics2d::SEGMENT2_D seg_line;
	nn::common::graphics2d::line2segment( &seg_line, data->paraBumper);

	nn::common::def::graphics2d::segment2_d after;
	
	bool result = nn::common::graphics2d::isCollisionLine( data->returnCollision, &after, data->paraMove, &seg_line);

	if(result) {
		// 衝突
		nn_commonLog( 999, "衝突\n");
		nn::common::def::graphics2d::VECTOR2_D angle0;
		angle0.x = 1.0;
		angle0.y = 0.0;
		after.v.x = -after.v.x;
		// 後で ここまで複雑な計算いる？
		double dot_angle0_after_v = nn::common::graphics2d::vec2dot( &angle0, &after.v);
		double crs_angle0_after_v = nn::common::graphics2d::vec2cross( &angle0, &after.v);
		int after_angle = NN_COMMON_VEC2ANGLE2( dot_angle0_after_v, crs_angle0_after_v);
		if(after_angle < 0) {
			after_angle += 360;
		}
		*data->returnAngle = after_angle;
		*data->returnSpeed *= nn::yb::e::global::FLY_BUMPER_HIT_ADD_SPEED_RATE;
		//線にハマる
		*data->returnX = after.s.x;
		*data->returnY = after.s.y;
	}

	*data->returnResult = result;

}

//---------------------------------------------------------------------------

void nn::yb::common::yb_commonAddAngle( double * angle, double add) {


	*angle += add;
	if(*angle >= 360.0) {
		*angle -= 360.0;
	}
	if(*angle < 0.0) {
		*angle += 360.0;
	}
	if(*angle < 0.0 || 360.0 <= *angle) {
		printf( "yb_commonAddAngle() error\n");
		*angle = 0;
	}

}

//---------------------------------------------------------------------------

void nn::yb::common::yb_commonDispMsgBox( int x, int y, int z, int w, int h, int objIdEdge, GXRgb color, int alpha) {

	OBJ * obj = disp_commonGetObj( objIdEdge);

	if(w < obj->w * 2 || h < obj->h * 2) {
		return;
	}

	disp_commonObjEx( objIdEdge, x,					y + 1,							z,	0, FALSE, FALSE, color, alpha);
	disp_commonObjEx( objIdEdge, x,					y + (h - obj->h),		z,	0, FALSE, TRUE , color, alpha);
	disp_commonObjEx( objIdEdge, x + (w - obj->w) - 1,	y,						z,	0, TRUE,  FALSE, color, alpha);
	disp_commonObjEx( objIdEdge, x + (w - obj->w) - 1,	y + (h - obj->h),	z,	0, TRUE,  TRUE , color, alpha);

	disp_commonMain3dBoard( x + obj->w,	y,					z, w - (obj->w * 2),	obj->h + 1,				color, alpha);
	disp_commonMain3dBoard( x + obj->w,	y + (h - obj->h),	z, w - (obj->w * 2),	obj->h,				color, alpha);
	disp_commonMain3dBoard( x,			y + obj->h,		z, w,	h - (obj->h * 2) + 1,	color, alpha);

}

//---------------------------------------------------------------------------

void nn::yb::common::yb_commonDispHoleObj( nn::yb::common::DISP_HOLE_OBJ_DATA * d) {

	int y_shift = 0;
	int x_shift = 0;
	int x1;
	int x2;
	int y1;
	int y2;
	x1 = (int)(d->centerX + (d->x1 - d->centerX) * d->hScale);
	x2 = (int)(d->centerX + (d->x2 - d->centerX) * d->hScale);
	y1 = (int)(d->centerY + (d->y1 - d->centerY) * d->vScale);
	y2 = (int)(d->centerY + (d->y2 - d->centerY) * d->vScale);

	switch(d->category) {
		case nn::yb::e::edit_hole_obj::CATEGORY_BALL:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_YUKKURI_REIMU_4, x1 + x_shift, y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_BAKEBAKE_0 + d->sceneNo, x1 + x_shift, y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_MUSO_0 + d->sceneNo, x1 + x_shift, y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SPEA_0 + d->sceneNo, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MOMIJI:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_ENEMY_9 + d->sceneNo, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_TENKO:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_ENEMY_6 + d->sceneNo, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_GREEN_FAIRY:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_ENEMY_0 + d->sceneNo, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_BLUE_FAIRY:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_ENEMY_3 + d->sceneNo, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_NITORI:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_ENEMY_12 + d->sceneNo, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_FLAN:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_ENEMY_15 + d->sceneNo, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_ORIN:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_ENEMY_18 + d->sceneNo, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-38 * 3/4.0 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_OJAMA_4, x1, y1, nn::yb::e::global::DISP_Z_OBJ_SHADOW, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_OJAMA_0, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_REMIFLA:
		case nn::yb::e::edit_hole_obj::CATEGORY_UNZAN_MOVE_REMIFLA:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-52 * 3/4.0 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_OJAMA_4, x1, y1, nn::yb::e::global::DISP_Z_OBJ_SHADOW, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_OJAMA_9, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_OKU:
			{
				x_shift = (int)(0 * d->hScale);
				y_shift = (int)((-24 + -d->z1) * d->vScale);
				if(d->subCategory == nn::yb::e::edit_hole_obj::OKU_CATEGORY_NORMAL) {
					int obj_id_tbl[] = {
						OBJ_ID_MAIN_OJAMA_5,
						OBJ_ID_MAIN_OJAMA_6,
						OBJ_ID_MAIN_OJAMA_7,
						OBJ_ID_MAIN_OJAMA_6,
					};
					disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_OJAMA_4, x1, y1, nn::yb::e::global::DISP_Z_OBJ_SHADOW, d->hScale * 0.8, d->vScale * 0.6, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
					disp_commonObjDxScaleCenterFull( obj_id_tbl[d->frameCnt / nn::yb::e::global::OKU_NORMAL_SCENE_DISP_FRAME_CNT % NN_COMMON_GET_ARRAY_SIZE( obj_id_tbl)], x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
				}
				else {
					// OKU_CATEGORY_ATTACK
					disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_OJAMA_4, x1, y1, nn::yb::e::global::DISP_Z_OBJ_SHADOW, d->hScale * 0.8, d->vScale * 0.6, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
					disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_OJAMA_8, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
				}
			}
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_PILLAR:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)(-60 * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_OJAMA_3, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_KEDAMA_0 + d->sceneNo, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_HANEMEDAMA_0 + d->sceneNo, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA:
			x_shift = (int)(0 * d->hScale);
			y_shift = (int)((-24 + -d->z1) * d->vScale);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_SHADOW_1, x1, y1, d->dispZ + 1, d->hScale, d->vScale, 0, FALSE, FALSE, RGB_WHITE, 16, DISP_COMMON_BLEND_MODE_NONE);
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_GURIMOA_0 + d->sceneNo, x1 + x_shift , y1+ y_shift, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_BUMPER:
			disp_commonMain3dLine( (int)x1, (int)y1, (int)x2, (int)y2, d->dispZ, (int)(ceil(5 * (d->hScale + d->vScale) / 2)), d->color, 31);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_GROUND:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT:
		case nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP:

		case nn::yb::e::edit_hole_obj::CATEGORY_DAMAGE:
		case nn::yb::e::edit_hole_obj::CATEGORY_BUNKER:
		case nn::yb::e::edit_hole_obj::CATEGORY_GRAVITY:

		case nn::yb::e::edit_hole_obj::CATEGORY_FUKIAGE: 
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN:
		case nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT:

		case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_START:
		case nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_GOAL:
			{
				int ground_obj_id = OBJ_ID_HOLE_GROUND_1AREA_0;
				GXRgb slope_color = GX_RGB( 31, 24, 24);
				switch(d->subCategory) {
					case nn::yb::e::edit_hole_obj::GROUND_CATEGORY_GREEN:
						ground_obj_id = OBJ_ID_HOLE_GROUND_1AREA_0;
						slope_color = GX_RGB( 31, 24, 24);
						break;
					case nn::yb::e::edit_hole_obj::GROUND_CATEGORY_RED:
						ground_obj_id = OBJ_ID_HOLE_GROUND2_1AREA_0;
						slope_color = GX_RGB( 16, 16, 16);
						break;
					default:
						ground_obj_id = OBJ_ID_HOLE_GROUND_1AREA_0;
						slope_color = GX_RGB( 31, 24, 24);
						break;
				}
				if(d->category == nn::yb::e::edit_hole_obj::CATEGORY_GROUND ||
					d->category == nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_START ||
					d->category == nn::yb::e::edit_hole_obj::CATEGORY_TIME_ATTACK_GOAL
				) {
					nn::yb::common::yb_commonDispGroundScale( ground_obj_id, d->x1, d->y1, d->x2, d->y2, d->dispZ, 192, 192,
						d->centerX, d->centerY, d->hScale, d->vScale,
						FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
					);
				}
				else if (d->category == nn::yb::e::edit_hole_obj::CATEGORY_DAMAGE)
				{
					nn::yb::common::yb_commonDispObjRepeatScale( OBJ_ID_HOLE_DAMAGE_1AREA_0,
						d->x1, d->y1, d->x2, d->y2, d->dispZ - 2,
						d->centerX, d->centerY, d->hScale, d->vScale,
						FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
					);
				}
				else if (d->category == nn::yb::e::edit_hole_obj::CATEGORY_BUNKER)
				{
					nn::yb::common::yb_commonDispObjRepeatScale( OBJ_ID_HOLE_BUNKER_1AREA_0,
						d->x1, d->y1, d->x2, d->y2, d->dispZ - 2,
						d->centerX, d->centerY, d->hScale, d->vScale,
						FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
					);				}
				else if (d->category == nn::yb::e::edit_hole_obj::CATEGORY_GRAVITY)
				{
					int obj_id_tbl[] = {
						OBJ_ID_HOLE_GRAVITY_1AREA_0,
						OBJ_ID_HOLE_GRAVITY_1AREA_0,
						OBJ_ID_HOLE_GRAVITY_1AREA_1,
						OBJ_ID_HOLE_GRAVITY_1AREA_2,
						OBJ_ID_HOLE_GRAVITY_1AREA_3,
						OBJ_ID_HOLE_GRAVITY_1AREA_4,
						OBJ_ID_HOLE_GRAVITY_1AREA_5,
						OBJ_ID_HOLE_GRAVITY_1AREA_6,
						OBJ_ID_HOLE_GRAVITY_1AREA_7,
						OBJ_ID_HOLE_GRAVITY_1AREA_7,
					};
					int disp_index = (d->frameCnt / 5) % NN_COMMON_GET_ARRAY_SIZE( obj_id_tbl);
					nn::yb::common::yb_commonDispObjRepeatScale( obj_id_tbl[disp_index],
						d->x1, d->y1, d->x2, d->y2, d->dispZ - 2,
						d->centerX, d->centerY, d->hScale, d->vScale,
						FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
					);				}
				
				
				else if (d->category == nn::yb::e::edit_hole_obj::CATEGORY_FUKIAGE)
				{
					nn::yb::common::yb_commonDispObjRepeatScale( OBJ_ID_HOLE_AIR_0,
						d->x1, d->y1, d->x2, d->y2, d->dispZ - 2,
						d->centerX, d->centerY, d->hScale, d->vScale,
						FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
					);	
				}
				else if (d->category == nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_UP)
				{
					int obj_id_tbl[] = {
						OBJ_ID_HOLE_CONVEYOR_UP_1AREA_0,
						OBJ_ID_HOLE_CONVEYOR_UP_1AREA_1,
						OBJ_ID_HOLE_CONVEYOR_UP_1AREA_2,
						OBJ_ID_HOLE_CONVEYOR_UP_1AREA_3,
						OBJ_ID_HOLE_CONVEYOR_UP_1AREA_4,
						OBJ_ID_HOLE_CONVEYOR_UP_1AREA_5,
						OBJ_ID_HOLE_CONVEYOR_UP_1AREA_6,
						OBJ_ID_HOLE_CONVEYOR_UP_1AREA_7,
						OBJ_ID_HOLE_CONVEYOR_UP_1AREA_8,
						OBJ_ID_HOLE_CONVEYOR_UP_1AREA_9,
					};
					int disp_index = (d->frameCnt / 5) % NN_COMMON_GET_ARRAY_SIZE( obj_id_tbl);
					nn::yb::common::yb_commonDispObjRepeatScale( obj_id_tbl[disp_index],
						d->x1, d->y1, d->x2, d->y2, d->dispZ - 4,
						d->centerX, d->centerY, d->hScale, d->vScale,
						FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
					);	
				}
				else if (d->category == nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_RIGHT)
				{
					int obj_id_tbl[] = {
						OBJ_ID_HOLE_CONVEYOR_RIGHT_1AREA_0,
						OBJ_ID_HOLE_CONVEYOR_RIGHT_1AREA_1,
						OBJ_ID_HOLE_CONVEYOR_RIGHT_1AREA_2,
						OBJ_ID_HOLE_CONVEYOR_RIGHT_1AREA_3,
						OBJ_ID_HOLE_CONVEYOR_RIGHT_1AREA_4,
						OBJ_ID_HOLE_CONVEYOR_RIGHT_1AREA_5,
						OBJ_ID_HOLE_CONVEYOR_RIGHT_1AREA_6,
						OBJ_ID_HOLE_CONVEYOR_RIGHT_1AREA_7,
						OBJ_ID_HOLE_CONVEYOR_RIGHT_1AREA_8,
						OBJ_ID_HOLE_CONVEYOR_RIGHT_1AREA_9,
					};
					int disp_index = (d->frameCnt / 5) % NN_COMMON_GET_ARRAY_SIZE( obj_id_tbl);
					nn::yb::common::yb_commonDispObjRepeatScale( obj_id_tbl[disp_index],
						d->x1, d->y1, d->x2, d->y2, d->dispZ - 4,
						d->centerX, d->centerY, d->hScale, d->vScale,
						FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
					);	
				}
				else if (d->category == nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_LEFT)
				{
					int obj_id_tbl[] = {
						OBJ_ID_HOLE_CONVEYOR_LEFT_1AREA_0,
						OBJ_ID_HOLE_CONVEYOR_LEFT_1AREA_1,
						OBJ_ID_HOLE_CONVEYOR_LEFT_1AREA_2,
						OBJ_ID_HOLE_CONVEYOR_LEFT_1AREA_3,
						OBJ_ID_HOLE_CONVEYOR_LEFT_1AREA_4,
						OBJ_ID_HOLE_CONVEYOR_LEFT_1AREA_5,
						OBJ_ID_HOLE_CONVEYOR_LEFT_1AREA_6,
						OBJ_ID_HOLE_CONVEYOR_LEFT_1AREA_7,
						OBJ_ID_HOLE_CONVEYOR_LEFT_1AREA_8,
						OBJ_ID_HOLE_CONVEYOR_LEFT_1AREA_9,
					};
					int disp_index = (d->frameCnt / 5) % NN_COMMON_GET_ARRAY_SIZE( obj_id_tbl);
					nn::yb::common::yb_commonDispObjRepeatScale( obj_id_tbl[disp_index],
						d->x1, d->y1, d->x2, d->y2, d->dispZ - 4,
						d->centerX, d->centerY, d->hScale, d->vScale,
						FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
					);	
				}
				else if (d->category == nn::yb::e::edit_hole_obj::CATEGORY_CONVEYOR_DOWN)
				{
					int obj_id_tbl[] = {
						OBJ_ID_HOLE_CONVEYOR_DOWN_1AREA_0,
						OBJ_ID_HOLE_CONVEYOR_DOWN_1AREA_1,
						OBJ_ID_HOLE_CONVEYOR_DOWN_1AREA_2,
						OBJ_ID_HOLE_CONVEYOR_DOWN_1AREA_3,
						OBJ_ID_HOLE_CONVEYOR_DOWN_1AREA_4,
						OBJ_ID_HOLE_CONVEYOR_DOWN_1AREA_5,
						OBJ_ID_HOLE_CONVEYOR_DOWN_1AREA_6,
						OBJ_ID_HOLE_CONVEYOR_DOWN_1AREA_7,
						OBJ_ID_HOLE_CONVEYOR_DOWN_1AREA_8,
						OBJ_ID_HOLE_CONVEYOR_DOWN_1AREA_9,
					};
					int disp_index = (d->frameCnt / 5) % NN_COMMON_GET_ARRAY_SIZE( obj_id_tbl);
					nn::yb::common::yb_commonDispObjRepeatScale( obj_id_tbl[disp_index],
						d->x1, d->y1, d->x2, d->y2, d->dispZ - 4,
						d->centerX, d->centerY, d->hScale, d->vScale,
						FALSE, FALSE, RGB_WHITE, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
					);	
				}
				else {
					int obj_id = 0;
					BOOL flip_h = FALSE;
					BOOL flip_v = FALSE;
					nn::yb::common::yb_commonDispGroundScale( ground_obj_id, 
						d->x1, d->y1, d->x2, d->y2, d->dispZ - 1, 192, 192,
						d->centerX, d->centerY, d->hScale, d->vScale,
						FALSE, FALSE, slope_color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
					);
					if(d->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP) {
						obj_id = OBJ_ID_HOLE_DOWN_1AREA_0; flip_h = FALSE; flip_v = TRUE;
					}
					else if(d->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP) {
						obj_id = OBJ_ID_HOLE_RIGHT_DOWN_1AREA_0; flip_h = FALSE; flip_v = TRUE;
					}
					else if(d->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT) {
						obj_id = OBJ_ID_HOLE_RIGHT_1AREA_0; flip_h = FALSE; flip_v = FALSE;
					}
					else if(d->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN) {
						obj_id = OBJ_ID_HOLE_RIGHT_DOWN_1AREA_0; flip_h = FALSE; flip_v = FALSE;
					}
					else if(d->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN) {
						obj_id = OBJ_ID_HOLE_DOWN_1AREA_0; flip_h = FALSE; flip_v = FALSE;
					}
					else if(d->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN) {
						obj_id = OBJ_ID_HOLE_RIGHT_DOWN_1AREA_0; flip_h = TRUE; flip_v = FALSE;
					}
					else if(d->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT) {
						obj_id = OBJ_ID_HOLE_RIGHT_1AREA_0; flip_h = TRUE; flip_v = FALSE;
					}
					else if(d->category == nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP) {
						obj_id = OBJ_ID_HOLE_RIGHT_DOWN_1AREA_0; flip_h = TRUE; flip_v = TRUE;
					}
					nn::yb::common::yb_commonDispGroundScale( obj_id, 
						d->x1, d->y1, d->x2, d->y2, d->dispZ - 2, 192, 192,
						d->centerX, d->centerY, d->hScale, d->vScale,
						flip_h, flip_v, RGB_WHITE, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE
					);
				}
			}
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
			{
				switch(d->category) {
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_0, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_1, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_2, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_3, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_LEFT:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_4, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
				}
				switch(d->category) {
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_0, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_UP:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_UP:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_1, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_RIGHT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_RIGHT:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_2, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_DOWN:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_DOWN:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_3, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_UP_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_RIGHT_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_DOWN_LEFT:
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP_LEFT_LEFT:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_4, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
				}
			}
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
				switch(d->category) {
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_NONE:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_0, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_UP:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_1, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_UP:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_5, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_2, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_RIGHT_DOWN:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_6, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_DOWN:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_3, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_DOWN:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_7, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_4, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::CATEGORY_WARP2_LEFT_UP:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_8, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
				}
				switch(d->vec) {
					case nn::yb::e::edit_hole_obj::WARP2_VEC_NONE:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_0, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::WARP2_VEC_UP:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_1, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::WARP2_VEC_RIGHT_UP:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_5, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::WARP2_VEC_RIGHT:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_2, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::WARP2_VEC_RIGHT_DOWN:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_6, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::WARP2_VEC_DOWN:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_3, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::WARP2_VEC_LEFT_DOWN:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_7, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::WARP2_VEC_LEFT:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_4, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
					case nn::yb::e::edit_hole_obj::WARP2_VEC_LEFT_UP:
						disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_WARP_8, x2, y2, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
						break;
				}
			}
			break;

		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_KICK_0, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_RIGHT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_KICK_1, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_KICK_2, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_LEFT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_KICK_3, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_RIGHT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_KICK_4, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_RIGHT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_KICK_5, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_DOWN_LEFT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_KICK_6, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_KICK_UP_LEFT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_KICK_7, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;

		case nn::yb::e::edit_hole_obj::CATEGORY_DASH:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_DASH_0, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_DASH_1, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_RIGHT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_DASH_2, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_DASH_3, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_LEFT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_DASH_4, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_RIGHT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_DASH_5, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_RIGHT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_DASH_6, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_DOWN_LEFT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_DASH_7, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_DASH_UP_LEFT:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_DASH_8, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;

		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP:
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_JUMP_0, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
			
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_WEAK: //tentative
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_JUMP_0, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_JUMP_STRONG: //tentative
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_JUMP_2, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_SE_FLOOR://tentative
			disp_commonObjDxScaleCenterFull( OBJ_ID_MAIN_JUMP_0, x1, y1, d->dispZ, d->hScale, d->vScale, 0, FALSE, FALSE, d->color, ALPHA_DEFAULT, DISP_COMMON_BLEND_MODE_NONE);
			break;
	}

}


//---------------------------------------------------------------------------

void nn::yb::common::yb_commonMoveHomingBullet(
	double& x, double& y,    // 弾の座標
	double& vx, double& vy,  // 弾の速度
	double mx, double my,    // 自機の座標
	double speed,           // 弾の速さ
	double theta            // 旋回角度の上限
) {
	// 弾の元の速度
	double vx0=vx, vy0=vy;

	// 自機方向の速度ベクトル(vx1,vy1)を求める
	double vx1, vy1;
	double d=sqrt((mx-x)*(mx-x)+(my-y)*(my-y));
	if (d > 10.0) {
		vx1=(mx-x)/d*speed;
		vy1=(my-y)/d*speed;
	} else {
		/*
		vx1=0;
		vy1=speed;
		*/
		x = mx;
		y = my;
		return;
	}

	// 右回り旋回角度上限の速度ベクトル(vx2,vy2)を求める：
	// M_PIは円周率。
	double rad=PI/180*theta;
	double vx2=cos(rad)*vx0-sin(rad)*vy0;
	double vy2=sin(rad)*vx0+cos(rad)*vy0;

	// 自機方向と旋回角度上限のどちらに曲がるかを決める
	if (vx0*vx1+vy0*vy1>=vx0*vx2+vy0*vy2) {
		
		// 自機方向が旋回可能範囲内の場合：
		// 自機方向に曲がる
		vx=vx1;
		vy=vy1;

	} else {

		// 自機方向が旋回可能範囲外の場合：
		// 左回り旋回角度上限の速度ベクトル(vx3,vy3)を求める
		double vx3= cos(rad)*vx0+sin(rad)*vy0;
		double vy3=-sin(rad)*vx0+cos(rad)*vy0;
		
		// 弾から自機への相対位置ベクトル(px,py)を求める
		float px=(float)(mx-x), py=(float)(my-y);
		
		// 右回りか左回りかを決める
		if (px*vx2+py*vy2>=px*vx3+py*vy3) {

			// 右回りの場合
			vx=vx2;
			vy=vy2;

		} else {

			// 左回りの場合
			vx=vx3;
			vy=vy3;

		}
	}

	// 弾の座標(x,y)を更新して，弾を移動させる
	x+=vx;
	y+=vy;
}

//---------------------------------------------------------------------------

bool nn::yb::common::yb_commonIsSkip( void * masterArg) {

	MASTER * master = (MASTER *)masterArg;

	if(master->keyWork.trigger & nn::common::e::PAD_WIN_BUTTON_A) {
		return true;
	}

	return false;

}

//---------------------------------------------------------------------------

