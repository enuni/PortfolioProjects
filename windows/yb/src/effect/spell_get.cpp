// spell_get を置換
// SpellGet を置換
// yb を置換
// effect を置換

#include "nn_common.h"
#include "nn_common2.h"
#include "disp_common.h"
#include "anime.h"

#include "yb_common.h"
#include "enum.h"
#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "animation_data.h"

#include "spell_get.h"
//---------------------------------------------------------------------------
nn::yb::effect::CSpellGet::CSpellGet(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CSpellGet Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructorCommon();

}
//---------------------------------------------------------------------------
nn::yb::effect::CSpellGet::CSpellGet(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CSpellGet Create(TASK_SAVE * oya)\n");
	
	this->constructorCommon();

}
//---------------------------------------------------------------------------
void nn::yb::effect::CSpellGet::constructorCommon() {
	
	this->state = nn::yb::e::spell_get::STATE_NONE;
	this->oldState = nn::yb::e::spell_get::STATE_NONE;
	this->waitCnt = 0;

	this->x	= 0.0;
	this->y = 0.0;
	this->vx = 0.0;
	this->vy = 0.0;

	this->dstX = 0.0;
	this->dstY = 0.0;
	this->speed = 0.0;
	this->limitAngle = 0.0;

	this->frameCnt = 0;

	this->dispAngle = 0;

	this->hitCardCbCb = NULL;
	this->hitCardCbCbArg = NULL;
	this->hitCardEffectState = 0;
	this->hitKeyCbCb = NULL;
	this->hitKeyCbCbArg = NULL;
	this->hitKeyEffectState = 0;

	this->objIdSpellCardIcon = 0;
	
}
//---------------------------------------------------------------------------
nn::yb::effect::CSpellGet::~CSpellGet() {

	::nn_commonLog( 1, "CSpellGet Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::effect::CSpellGet::start() {

	this->state	= nn::yb::e::spell_get::STATE_MOVE_CARD;

}
//---------------------------------------------------------------------------
void nn::yb::effect::CSpellGet::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CSpellGet::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;

	this->dispAngle = (this->dispAngle + 2) % 360;
	
	switch(this->state) {
		case nn::yb::e::spell_get::STATE_NONE:
			break;
		case nn::yb::e::spell_get::STATE_MOVE_CARD:
			nn::yb::common::yb_commonMoveHomingBullet( this->x, this->y,
				this->vx, this->vy,
				this->dstX, this->dstY,
				this->speed,
				this->limitAngle
			);
			if(this->x == this->dstX && this->y == this->dstY) {
				LAYOUT * layout_window = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_STATUS_WINDOW_ORG);
				LAYOUT * layout_card = disp_commonGetLayoutMain3d( LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_ICON_MUSO);
				nn::common::dxlib::CAnime * anime = nn::common::dxlib::CAnime::exec( this, ANIMATION_ID_GET_SPELL_CARD,
					layout_window->x + layout_card->x + (layout_card->w + 1) / 2,
					layout_window->y + layout_card->y + (layout_card->h + 1) / 2,
					nn::yb::e::global::DISP_Z_SPELL_GET_CARD_KEY, 0,
					nn::yb::effect::CSpellGet::hitCardCb, this
				);
				anime->changeObjId( ANIME_CHANGE_OBJ_ID_TYPE_OBJ_ID,
					OBJ_ID_MAIN_SPELL_CARD_ICON_1, this->objIdSpellCardIcon
				);
				this->hitCardEffectState = 1;
				this->state = nn::yb::e::spell_get::STATE_HIT_CARD;
			}
			break;
		case nn::yb::e::spell_get::STATE_HIT_CARD:
			this->frameCnt++;
			if(this->frameCnt >= 10) {
				LAYOUT * layout_z_key = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_KEY_Z);
				this->vx = -10.0;
				this->vy = -10.0;
				this->dstX = layout_z_key->x + layout_z_key->w / 2;
				this->dstY = layout_z_key->y + layout_z_key->h / 2;
				this->frameCnt = 0;
				this->state = nn::yb::e::spell_get::STATE_MOVE_KEY;
			}
			break;
		case nn::yb::e::spell_get::STATE_MOVE_KEY:
			nn::yb::common::yb_commonMoveHomingBullet( this->x, this->y,
				this->vx, this->vy,
				this->dstX, this->dstY,
				this->speed,
				this->limitAngle
			);
			if(this->x == this->dstX && this->y == this->dstY) {
				LAYOUT * layout_z_key = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_KEY_Z);
				nn::common::dxlib::CAnime::exec( this, ANIMATION_ID_GET_SPELL_KEY,
					layout_z_key->x + layout_z_key->w / 2,
					layout_z_key->y + layout_z_key->h / 2,
					nn::yb::e::global::DISP_Z_SPELL_GET_CARD_KEY, 0,
					nn::yb::effect::CSpellGet::hitKeyCb, this
				);
				this->hitKeyEffectState = 1;
				this->state = nn::yb::e::spell_get::STATE_HIT_KEY;
			}
			break;
		case nn::yb::e::spell_get::STATE_HIT_KEY:
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::effect::CSpellGet::disp() {

	switch(this->state) {
		case nn::yb::e::spell_get::STATE_NONE:
			break;
		case nn::yb::e::spell_get::STATE_MOVE_CARD:
		case nn::yb::e::spell_get::STATE_HIT_CARD:
		case nn::yb::e::spell_get::STATE_MOVE_KEY:
		case nn::yb::e::spell_get::STATE_HIT_KEY:
			{
				OBJ * obj_get = disp_commonGetObj( OBJ_ID_MAIN_SPELL_GET_2);
				if(this->state >= nn::yb::e::spell_get::STATE_HIT_CARD) {
					LAYOUT * layout_window = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_STATUS_WINDOW_ORG);
					LAYOUT * layout_card = disp_commonGetLayoutMain3d( LAYOUT_ID_STATUS_WINDOW_SPELL_CARD_ICON_MUSO);
					disp_commonObjDxCenterFull( OBJ_ID_MAIN_SPELL_GET_2,
						layout_window->x + layout_card->x + layout_card->w / 2,
						layout_window->y + layout_card->y + layout_card->h / 2,
						nn::yb::e::global::DISP_Z_SPELL_GET, (int)(obj_get->w * 2.2), (int)(obj_get->h * 2.2),
						this->dispAngle, FALSE, FALSE, RGB_WHITE,
						31, DISP_COMMON_BLEND_MODE_ADD
					);
				}
				disp_commonObjDxCenterFull( OBJ_ID_MAIN_SPELL_GET_2, (int)this->x, (int)this->y,
					nn::yb::e::global::DISP_Z_SPELL_GET, obj_get->w, obj_get->h, this->dispAngle, FALSE, FALSE, RGB_WHITE,
					31, DISP_COMMON_BLEND_MODE_ADD
				);
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::effect::CSpellGet::hitCardCb( void * spellGetArg) {

	nn::yb::effect::CSpellGet * spell_get = (nn::yb::effect::CSpellGet *)spellGetArg;

	spell_get->hitCard();

}
//---------------------------------------------------------------------------
void nn::yb::effect::CSpellGet::hitCard() {

	this->hitCardEffectState = 2;

	if(this->hitCardCbCb != NULL) {
		this->hitCardCbCb( this->hitCardCbCbArg);
	}

}
//---------------------------------------------------------------------------
void nn::yb::effect::CSpellGet::hitKeyCb( void * spellGetArg) {

	nn::yb::effect::CSpellGet * spell_get = (nn::yb::effect::CSpellGet *)spellGetArg;

	spell_get->hitKey();

}
//---------------------------------------------------------------------------
void nn::yb::effect::CSpellGet::hitKey() {

	this->hitCardEffectState = 2;

	if(this->hitKeyCbCb != NULL) {
		this->hitKeyCbCb( this->hitKeyCbCbArg);
	}
	this->taskDelete();

}
//---------------------------------------------------------------------------

