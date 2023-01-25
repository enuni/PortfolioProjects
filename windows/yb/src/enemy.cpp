// enemy を置換
// Enemy を置換
// yb を置換
// main を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "disp_common.h"
#include "sound_nns.h"

#include "ball.h"
#include "hole.h"
#include "hole_ctrl.h"
#include "stage.h"
#include "yb_common.h"

#include "enum.h"

#include "sound_data.sadl"
#include "obj_main_3d_data.h"
#include "edit_hole_obj.h"

//effect
#include "effect/effect_ctrl.h"
#include "effect/effect.h"

#include "enemy.h"

//---------------------------------------------------------------------------

nn::yb::main::CEnemy::CEnemy(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CEnemy Create\n");
	this->state = nn::yb::e::enemy::STATE_NONE;
	this->oldState = nn::yb::e::enemy::STATE_NONE;
	this->waitCnt = 0;

	this->x = 0;
	this->y = 0;
	this->cnt = 0;
	this->holeCtrl = NULL;

	this->type = nn::yb::e::enemy::TYPE_BAKEBAKE;

	this->z = 24;

	//for debug
	this->bIsExist = false;
	this->effectCtrl = NULL; 
	this->ctrl_cnt = 0;
}

//---------------------------------------------------------------------------

nn::yb::main::CEnemy::~CEnemy() {

	::nn_commonLog( 1, "CEnemy Delete\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::CEnemy::start() {

	this->bIsExist = true;
#if 0
	//nullpo 20101113 エフェクトが終了してからCEnemyクラスを消去
	this->effectCtrl = new nn::yb::main::CEffectCtrl( this); 
#else
#endif

}

//---------------------------------------------------------------------------

void nn::yb::main::CEnemy::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CEnemy::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	// 今の所キー処理する予定なし
	//::KEY_WORK * key = &this->master->keyWork;

	this->cnt++;
	int iCDLength = 32;//当たり判定のサイズ

	if(this->holeCtrl->ball->state == nn::yb::e::ball::STATE_FLYING && this->holeCtrl->ball->procWaitCnt == 0) {
		double vx_enemy2ball = this->x - this->holeCtrl->ball->x;
		double vy_enemy2ball = this->y - this->holeCtrl->ball->y;
		double vz_enemy2ball = (this->z + 6) - this->holeCtrl->ball->z;
		double len_enemy2ball_pow2 = NN_COMMON_POW2( vx_enemy2ball) +
			NN_COMMON_POW2( vy_enemy2ball) + NN_COMMON_POW2( vz_enemy2ball);
		if(len_enemy2ball_pow2 <= NN_COMMON_POW2((iCDLength))) {
			
#if 0
			//nullpo 20101113 エフェクトが終了してからCEnemyクラスを消去
#else
			this->dead();
#endif
			if (this->bIsExist)//生存している場合は、ヒットエフェクトの生成
			{
				sound_nnsPlaySe( SE_BON);
				//hit effect
				this->effectCtrl->push_event( nn::yb::main::CEnemy::effectCb, this, NULL);
				nn::yb::main::effect_start_end_total( this->holeCtrl, OBJ_ID_DUN_SPELL_HIT_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					20,
					this->x, this->x, 20,
					this->y - 12 - this->z, this->y - 12 - this->z, 20,
					1,//敵より手前に描画
					32, 384, 20,
					32, 384, 20,
					0, 90, 20,
					42, 0, 20,
					DISP_COMMON_BLEND_MODE_ADD, nn::yb::enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, this->effectCtrl);//拡大
				this->ctrl_cnt++;

				this->bIsExist = false;//生存フラグを消す
			}
		}
	}

	this->disp();

}

//---------------------------------------------------------------------------

void nn::yb::main::CEnemy::disp() {

	/*
	int scene_no = (this->cnt % 24) / 8;
	int obj_id_tbl[nn::yb::e::enemy::TYPE_MAX] = {
		OBJ_ID_MAIN_BAKEBAKE_0,
		OBJ_ID_MAIN_BLUE_FAIRY_0,
		OBJ_ID_MAIN_RED_FAIRY_0,
		OBJ_ID_MAIN_KEDAMA_0,
		OBJ_ID_MAIN_HANEMEDAMA_0,
		OBJ_ID_MAIN_GURIMOA_0,
	};
	int z = nn::yb::e::global::DISP_Z_BALL_UP_OBJ;
	if(this->y >= this->holeCtrl->ball->y) {
		z = nn::yb::e::global::DISP_Z_BALL_DOWN_OBJ;
	}
	disp_commonObjCenterBasic( obj_id_tbl[this->type] + scene_no, this->x, this->y - this->z - 12, z);
	disp_commonObjCenterColor( OBJ_ID_MAIN_SHADOW_1, this->x, this->y + 12, nn::yb::e::global::DISP_Z_OBJ_SHADOW, RGB_WHITE, 16);
	*/
	
	int scene_no = (this->cnt % 24) / 8;
	int category_tbl[nn::yb::e::enemy::TYPE_MAX] = {
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MOMIJI,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_TENKO,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_GREEN_FAIRY,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_BLUE_FAIRY,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_NITORI,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_FLAN,
		nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_ORIN,
	};
	int z = nn::yb::e::global::DISP_Z_BALL_UP_ENEMY;
	if(this->y >= this->holeCtrl->ball->y) {
		z = nn::yb::e::global::DISP_Z_BALL_DOWN_ENEMY;
	}
	
	nn::yb::common::DISP_HOLE_OBJ_DATA disp_data;
	nn::yb::common::DISP_HOLE_OBJ_DATA * d = (nn::yb::common::DISP_HOLE_OBJ_DATA *)&disp_data;

	memset( d, 0, sizeof( nn::yb::common::DISP_HOLE_OBJ_DATA));

	d->category = category_tbl[this->type];
	d->vec = 0;
	d->x1 = this->x;
	d->y1 = this->y;
	d->z1 = this->z;
	d->dispZ = z;

	d->hScale = 1.0;
	d->vScale = 1.0;

	d->color = RGB_WHITE;
	d->sceneNo = scene_no;
	
	nn::yb::common::yb_commonDispHoleObj( d);

}

//---------------------------------------------------------------------------


void nn::yb::main::CEnemy::effectCb( void * enemyArg, void * dummy) {

#if 0
	//nullpo 20101113 エフェクトが終了してからCEnemyクラスを消去
	nn::yb::main::CEnemy * enemy = (nn::yb::main::CEnemy *)enemyArg;

	enemy->ctrl_cnt--;
	if(enemy->ctrl_cnt == 0) {
		enemy->effectCtrl->pop_event();
		enemy->dead();
	}
#else
#endif

}
//---------------------------------------------------------------------------
void nn::yb::main::CEnemy::dead() {

	if(this->holeCtrl->stage->getHp() < nn::yb::e::global::STAGE_HP_MAX) {
		this->holeCtrl->stage->damage( +1);
	}

	this->holeCtrl->ball->setDispState( nn::yb::e::ball::DISP_STATE_DOYA, 30);

	int spell = nn::yb::main::CEnemy::getEnemyType2Spell( this->type);
	if(spell != 0) {
		this->holeCtrl->setSpell( spell);
	}

	this->holeCtrl->hole->defeatedEnemy( this);
	this->taskDelete();

}
//---------------------------------------------------------------------------
int nn::yb::main::CEnemy::getEnemyType2Spell(int type) {

	if(type < 0 || nn::yb::e::enemy::TYPE_MAX <= type) {
		return 0;
	}

	int spell_tbl[nn::yb::e::enemy::TYPE_MAX] = {
		0,										// nn::yb::e::enemy::TYPE_BAKEBAKE
		nn::yb::e::global::SPELL_CARD_MUSO,		// nn::yb::e::enemy::TYPE_BULE_FAIRY
		nn::yb::e::global::SPELL_CARD_SPEA,		// nn::yb::e::enemy::TYPE_RED_FAIRY
		0,										// nn::yb::e::enemy::TYPE_KEDAMA
		0,										// nn::yb::e::enemy::TYPE_HANEMEDAMA
		nn::yb::e::global::SPELL_CARD_MSPA,		// nn::yb::e::enemy::TYPE_MSPA
		nn::yb::e::global::SPELL_CARD_MOMIJI,	// nn::yb::e::enemy::TYPE_MOMIJI
		nn::yb::e::global::SPELL_CARD_TENKO,	// nn::yb::e::enemy::TYPE_TENKO
		0,										// nn::yb::e::enemy::TYPE_GREEN_FAIRY
		0,										// nn::yb::e::enemy::TYPE_BLUE_FAIRY
		nn::yb::e::global::SPELL_CARD_NITORI,	// nn::yb::e::enemy::TYPE_NITORI
		nn::yb::e::global::SPELL_CARD_FLAN,		// nn::yb::e::enemy::TYPE_FLAN
		nn::yb::e::global::SPELL_CARD_ORIN,		// nn::yb::e::enemy::TYPE_ORIN
	};

	return spell_tbl[type];

}
//---------------------------------------------------------------------------
