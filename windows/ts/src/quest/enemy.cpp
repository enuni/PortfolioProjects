// enemy を置換
// Enemy を置換
// ts を置換
// quest を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "disp.h"
#include "anim.h"

#include "damage_disp.h"
#include "ts_save_manager.h"

#include "anime_data.h"

#include "attack_bullet.h"
#include "attack_laser.h"
#include "quest.h"
#include "ball.h"
#include "attack_explosion.h"

#include "enemy.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::quest::CEnemy::dataRecode[] = {
	DATA_LOCAL( nn::ts::quest::CEnemy, state),
	DATA_LOCAL( nn::ts::quest::CEnemy, oldState),
	DATA_LOCAL( nn::ts::quest::CEnemy, waitCnt),
	DATA_LOCAL( nn::ts::quest::CEnemy, gm.enemyID),
	DATA_LOCAL( nn::ts::quest::CEnemy, gm.param),
	DATA_LOCAL( nn::ts::quest::CEnemy, gm.status),
	DATA_LOCAL( nn::ts::quest::CEnemy, gm.stateFrameCnt),
	DATA_LOCAL( nn::ts::quest::CEnemy, gm.collsionCtrlCnt),
	DATA_PTR( nn::ts::quest::CEnemy, lm.quest),
	DATA_LOCAL( nn::ts::quest::CEnemy, lm.endAttackCBFuncId),
	DATA_PTR( nn::ts::quest::CEnemy, lm.endAttackCBFuncOwner),
	DATA_LOCAL( nn::ts::quest::CEnemy, lm.endDirectCBFuncId),
	DATA_PTR( nn::ts::quest::CEnemy, lm.endDirectCBFuncOwner),
	DATA_BASE(nn::common::CGameChar::dataRecode),
	DATA_END,
};

nn::ts::quest::ENEMY_DISP_MEMBER nn::ts::quest::CEnemy::dm;
//---------------------------------------------------------------------------
nn::ts::quest::CEnemy::CEnemy(TASK_SAVE * oya, nn_common::SaveManager * m) : nn::common::CGameChar( oya, m) {

	::nn_commonLog( 1, "CEnemy Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CEnemy::CEnemy(TASK_SAVE * oya) : nn::common::CGameChar( oya) {

	::nn_commonLog( 1, "CEnemy Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CEnemy::CEnemy() : nn::common::CGameChar() {

	::nn_commonLog( 1, "CEnemy Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::constructCommon() {
	
	this->state = nn::ts::e::enemy::STATE_NONE;
	this->oldState = nn::ts::e::enemy::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->gm, 0, sizeof( this->gm));
	memset( &this->lm, 0, sizeof( this->lm));

	this->dd = new nn::dxlib::direct::CDispDirect();
	this->dd->m.user.directEndCBFunc = nn::ts::quest::CEnemy::endDirectCB;
	this->dd->m.user.directEndCBFuncArg = this;
	
	strcpy_s( this->name, sizeof( this->name), "CEnemy");
	
}
//---------------------------------------------------------------------------
nn::ts::quest::CEnemy::~CEnemy() {

	::nn_commonLog( 1, "CEnemy Delete\n");

	delete this->dd;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::start() {
	
	this->startCommon();
	
	this->gm.status.hp = this->gm.param.maxHP[this->gm.status.gaugeCnt];
	this->gm.status.damageHp = this->gm.param.maxHP[this->gm.status.gaugeCnt];
	this->gameChar.game.enableFlag = true;
	this->gm.status.hitSkipFlag = false;
	for( int i = 0; i < this->gm.param.attackN; i++) {

		this->gm.status.attackTurns[i] = this->gm.param.attacks[i].firstTurn;

	}

	this->dd->start();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::startCommon() {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::damage( int damage) {

	this->gm.status.damageHp -= damage;
	if(this->gm.status.damageHp < 0) {
		this->gm.status.damageHp = 0;
	}

	if(this->lm.dd == NULL) {
		nn::ts::quest::CDamageDisp * dd = new nn::ts::quest::CDamageDisp( this);
		dd->gm.param.startX = (int)this->gameChar.game.hit.x;
		dd->gm.param.startY = (int)this->gameChar.game.hit.y;
		dd->gm.param.endX =   (int)this->gameChar.game.hit.x;
		dd->gm.param.endY =   (int)this->gameChar.game.hit.y - 60;
		dd->gm.param.moveFrameN = 60;
		dd->gm.param.exFrameN = 40;
		dd->gm.param.dmageFrameN = 50;
		dd->gm.param.z = nn::ts::e::common::Z_BATTLE_DAMAGE_DISP;
		dd->gm.param.startExRate = 0.50;
		dd->gm.param.endExRate = 0.70;
		dd->gm.param.blank = 1;
		dd->gm.param.alpha = DISP_ALPHA_DEFAULT;
		dd->gm.param.blend = DISP_BLEND_MODE_NONE;
		dd->gm.param.rgb = GX_RGB( 255, 243, 243);
		dd->gm.param.dispFrameN = 120;
		dd->gm.param.deleteCBFunc = nn::ts::quest::CEnemy::finDeleteDamgeDispCB;
		dd->gm.param.deleteCBFuncArg = this;
	
		dd->start();

		this->lm.dd = dd;

		this->lm.dd->damage( damage);
	}
	else {
		this->lm.dd->damage( damage);
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::collision( int damage) {

	this->state = nn::ts::e::enemy::STATE_COLLISION;
	this->gm.stateFrameCnt = 0;

	this->damage( damage);
}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::deleteDamageDisp() {

	if(this->lm.dd != NULL) {
		this->lm.dd->gm.deleteFlag = true;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::finDeleteDamgeDispCB( void * argEnemy) {

	nn::ts::quest::CEnemy * enemy = (nn::ts::quest::CEnemy *)argEnemy;

	enemy->finDeleteDamgeDisp();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::finDeleteDamgeDisp() {

	this->lm.dd = NULL;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::shotEnd() {

	this->deleteDamageDisp();

	this->gm.status.hp = this->gm.status.damageHp;

}
//---------------------------------------------------------------------------
int nn::ts::quest::CEnemy::turn() {
	
	int  attack_cnt= 0;
	for( int i = 0; i < this->gm.param.attackN; i++) {

		this->gm.status.attackTurns[i]--;
		if(this->gm.status.attackTurns[i] == 0) {
			attack_cnt++;
		}

	}

	if(attack_cnt >= 1) {
		if(this->gm.param.bossFlag) {
			this->state = nn::ts::e::enemy::STATE_ATTACK_DIRECT;
			nn::dxlib::CAnim * anime = nn::dxlib::CAnim::exec( this, ::ANIMATION_ID_ENEMY_ATTACK,
				(int)this->gameChar.game.hit.x, (int)this->gameChar.game.hit.y,
				nn::ts::e::common::Z_BATTLE_ENEMY, ANIM_FLAGS_CAMERA,
				nn::ts::quest::CEnemy::attackCB, this);
			anime->changeObjId( ANIM_CHANGE_OBJ_ID_TYPE_OBJ_ID,
				this->dm.objIdEnemys[1], this->dm.objIdEnemys[this->gm.enemyID]);
		}
		else {
			this->attack();
		}
	}

	return attack_cnt;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::attackCB( void * argEnemy) {

	nn::ts::quest::CEnemy * enemy = (nn::ts::quest::CEnemy *)argEnemy;

	enemy->attack();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::attack() {
	
	for( int i = 0; i < this->gm.param.attackN; i++) {

		nn::ts::quest::ENEMY_ATTACK * attack = &this->gm.param.attacks[i];

		if(this->gm.status.attackTurns[i] == 0) {
			if(attack->category == nn::ts::e::enemy::ATTACK_CATEGORY_MACHINGUN_DOWN) {
				nn::ts::attack::CBullet * bullet = new nn::ts::attack::CBullet( this);
				bullet->gm.param.x = (int)this->gameChar.game.hit.x + attack->x;
				bullet->gm.param.y = (int)this->gameChar.game.hit.y + attack->y;
				//bullet->gm.param.targetXs = 0;
				//bullet->gm.param.targetYs = 0;
				bullet->gm.param.fireN = 3;
				bullet->gm.param.fireBulletN = 10;
				bullet->gm.param.fireIntervalFrameN = 50;
				bullet->gm.param.damage = 1200;
				bullet->gm.param.quest = this->lm.quest;
				bullet->gm.param.angle[0] = 220.0;
				bullet->gm.param.angle[1] = 320.0;
				bullet->gm.param.angle[2] = 220.0;
				bullet->gm.param.addAngle[0] =  6.0;
				bullet->gm.param.addAngle[1] = -6.0;
				bullet->gm.param.addAngle[2] =  6.0;
				bullet->gm.param.type = nn::ts::e::attack_bullet::TYPE_MACHINEGUN;
				bullet->gm.param.hitTargetBit = nn::ts::e::common::HIT_TARGET_BIT_BALL;
				bullet->gm.param.attribute = nn::ts::e::common::ATTRIBUTE_DARK;
				bullet->gm.param.endCBFunc = nn::ts::quest::CEnemy::endAttackCB;
				bullet->gm.param.endCBFuncArg = this;

				bullet->start();
			}
			else if(attack->category == nn::ts::e::enemy::ATTACK_CATEGORY_LASER_2) {
				nn::ts::attack::CLaser * laser = new nn::ts::attack::CLaser( this);
				int src_x = (int)this->gameChar.game.hit.x + attack->x;
				int src_y = (int)this->gameChar.game.hit.y + attack->y;
				double target_x = 0.0;
				double target_y = 0.0;
				this->lm.quest->getNearBallPos( &target_x, &target_y,
					src_x, src_y);
				double target_angle = nn::common::math::pos2Angle_d(
					src_x, src_y, target_x, target_y);
				laser->gm.param.x = src_x;
				laser->gm.param.y = src_y;
				laser->gm.param.angle = (int)target_angle;
				laser->gm.param.quest = this->lm.quest;
				laser->gm.param.width = 100;
				laser->gm.param.damage = 4000;
				laser->gm.param.reflectionLaserN = 2;
				laser->gm.param.type = nn::ts::e::attack_laser::LASER_TYPE_REFLECTION;
				laser->gm.param.attribute = nn::ts::e::common::ATTRIBUTE_DARK;
				laser->gm.param.hitTargetBit = nn::ts::e::common::HIT_TARGET_BIT_BALL; 
				laser->gm.param.endCBFunc = nn::ts::quest::CEnemy::endAttackCB;
				laser->gm.param.endCBFuncArg = this;

				laser->start();
			}
			else if(attack->category == nn::ts::e::enemy::ATTACK_CATEGORY_LASER_4) {
				nn::ts::attack::CLaser * laser = new nn::ts::attack::CLaser( this);
				int src_x = (int)this->gameChar.game.hit.x + attack->x;
				int src_y = (int)this->gameChar.game.hit.y + attack->y;
				double target_x = 0.0;
				double target_y = 0.0;
				this->lm.quest->getNearBallPos( &target_x, &target_y,
					src_x, src_y);
				double target_angle = nn::common::math::pos2Angle_d(
					src_x, src_y, target_x, target_y);
				laser->gm.param.x = src_x;
				laser->gm.param.y = src_y;
				laser->gm.param.angle = (int)target_angle;
				laser->gm.param.quest = this->lm.quest;
				laser->gm.param.width = 100;
				laser->gm.param.damage = 4000;
				laser->gm.param.reflectionLaserN = 4;
				laser->gm.param.type = nn::ts::e::attack_laser::LASER_TYPE_REFLECTION;
				laser->gm.param.attribute = nn::ts::e::common::ATTRIBUTE_DARK;
				laser->gm.param.hitTargetBit = nn::ts::e::common::HIT_TARGET_BIT_BALL; 
				laser->gm.param.endCBFunc = nn::ts::quest::CEnemy::endAttackCB;
				laser->gm.param.endCBFuncArg = this;

				laser->start();
			}
			else if(attack->category == nn::ts::e::enemy::ATTACK_CATEGORY_HOMING) {
				nn::ts::attack::CBullet * bullet = new nn::ts::attack::CBullet( this);
				bullet->gm.param.x = (int)this->gameChar.game.hit.x + attack->x;
				bullet->gm.param.y = (int)this->gameChar.game.hit.y + attack->y;
				for( int i = 0; i < nn::ts::e::common::QUEST_BALL_N; i++) {
				
					nn::ts::quest::CBall * ball = this->lm.quest->lm.balls[i];
					bullet->gm.param.targets[i] = ball;

				}
				bullet->gm.param.targetN = nn::ts::e::common::QUEST_BALL_N;
				bullet->gm.param.fireN = 3;
				bullet->gm.param.fireBulletN = 4;
				bullet->gm.param.fireIntervalFrameN = 30;
				bullet->gm.param.damage = 1100;
				bullet->gm.param.quest = this->lm.quest;
				bullet->gm.param.angle[0] = 0.0;
				bullet->gm.param.addAngle[0] = 60.0;
				bullet->gm.param.type = nn::ts::e::attack_bullet::TYPE_HOMING;
				bullet->gm.param.hitTargetBit = nn::ts::e::common::HIT_TARGET_BIT_BALL;
				bullet->gm.param.attribute = nn::ts::e::common::ATTRIBUTE_DARK;
				bullet->gm.param.endCBFunc = nn::ts::quest::CEnemy::endAttackCB;
				bullet->gm.param.endCBFuncArg = this;

				bullet->start();
			}
			else if(attack->category == nn::ts::e::enemy::ATTACK_CATEGORY_EXPLOSION) {
				nn::ts::attack::CExplosion * explosion = new nn::ts::attack::CExplosion( this);
				explosion->gm.param.x = (int)this->gameChar.game.hit.x;
				explosion->gm.param.y = (int)this->gameChar.game.hit.y;
				explosion->gm.param.damage = 15000;
				explosion->gm.param.r = 180;
				explosion->gm.param.hitTargetBit = nn::ts::e::common::HIT_TARGET_BIT_BALL;
				//explosion->gm.param.ball = NULL;
				explosion->gm.param.battle = this->lm.quest->lm.battle;
				explosion->gm.param.quest = this->lm.quest;
				explosion->gm.param.endCBFunc = nn::ts::quest::CEnemy::endAttackCB;
				explosion->gm.param.endCBFuncArg = this;

				explosion->start();
			}
		}

	}
	
	this->state = nn::ts::e::enemy::STATE_ATTACK;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::endAttackCB( void * argEnemy) {

	nn::ts::quest::CEnemy * enemy = (nn::ts::quest::CEnemy *)argEnemy;

	enemy->endAttack();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::endAttack() {

	this->state = nn::ts::e::enemy::STATE_IDLE;
	for( int i = 0; i < this->gm.param.attackN; i++) {

		nn::ts::quest::ENEMY_ATTACK * attack = &this->gm.param.attacks[i];

		if(this->gm.status.attackTurns[i] == 0) {
			this->gm.status.attackTurns[i] = attack->resetTurn;
		}

	}

	if(this->lm.endAttackCBFuncId != 0) {
		void (* func)( void *) = (void (*)( void *))nn::common::util::CCallBackManager::get( this->lm.endAttackCBFuncId);
		if(func != NULL) {
			func( this->lm.endAttackCBFuncOwner);
		}
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CEnemy::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::enemy::STATE_NONE:
			break;
		case nn::ts::e::enemy::STATE_COLLISION:
			this->gm.collsionCtrlCnt++;
			if(this->gm.stateFrameCnt < 24) {
				this->gm.stateFrameCnt++;
			}
			else {
				this->gm.collsionCtrlCnt = 0;
				this->state = nn::ts::e::enemy::STATE_IDLE;
			}
			break;
		case nn::ts::e::enemy::STATE_DIRECT:
		case nn::ts::e::enemy::STATE_ESCAPE:
			this->dd->task();
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::disp() {

	switch(this->state) {
		case nn::ts::e::enemy::STATE_NONE:
			break;
		case nn::ts::e::enemy::STATE_IDLE:
		case nn::ts::e::enemy::STATE_COLLISION:
		case nn::ts::e::enemy::STATE_ATTACK:
		case nn::ts::e::enemy::STATE_DIRECT:
		case nn::ts::e::enemy::STATE_ESCAPE:
			int x = (int)this->gameChar.game.hit.x;
			int y = (int)this->gameChar.game.hit.y;
			if(this->state == nn::ts::e::enemy::STATE_COLLISION) {
				int shift_x = this->gm.collsionCtrlCnt % 12 - 6;
				if(shift_x < 0) {
					shift_x = -shift_x;
				}
				x += (int)((shift_x - 3) * 2);
			}
			int color = DISP_WHITE;
			if(this->state == nn::ts::e::enemy::STATE_ESCAPE) {
				color = DISP_GET_COLOR( 100, 100, 100);
			}
			nn::dxlib::disp::objColorCenter( this->dm.objIdEnemys[this->gm.enemyID],
				x, y + this->dd->m.ctrl.add.y, nn::ts::e::common::Z_BATTLE_ENEMY,
				color, DISP_ALPHA_DEFAULT, true);
#ifdef _DEBUG
			for ( int i = 0; i < this->gm.param.hitN; i++) {

				nn::ts::common::TS_HIT * ts_hit = &this->gm.param.hits[i];
				int color = DISP_BLUE;
				if(ts_hit->weakPointOnFlag) {
					color = DISP_RED;
				}
				nn::dxlib::disp::circleCenter(
					(int)this->gameChar.game.hit.x + ts_hit->hit.x,
					(int)this->gameChar.game.hit.y + ts_hit->hit.y + this->dd->m.ctrl.add.y,
					0, ts_hit->hit.r,
					color, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_NONE, true);
			}
#endif
			break;
	}
	
	switch(this->state) {
		case nn::ts::e::enemy::STATE_NONE:
			break;
		case nn::ts::e::enemy::STATE_IDLE:
		case nn::ts::e::enemy::STATE_COLLISION:
		case nn::ts::e::enemy::STATE_ATTACK_DIRECT:
		case nn::ts::e::enemy::STATE_ATTACK:
		case nn::ts::e::enemy::STATE_DIRECT:
			for( int i = 0; i < this->gm.param.attackN; i++) {
				
				LAYOUT * layout_turn = nn::dxlib::disp::getLayout( this->dm.partss[this->gm.enemyID].layoutIdTurns[i]);
				nn::ts::quest::ENEMY_ATTACK * attack = &this->gm.param.attacks[i];

				nn::dxlib::disp::objNumberCenter(
					this->gm.status.attackTurns[i], 1, layout_turn->obj_id,
					(int)this->gameChar.game.hit.x + attack->x,
					(int)this->gameChar.game.hit.y + attack->y + this->dd->m.ctrl.add.y,
					nn::ts::e::common::Z_BATTLE_ENEMY_TURN,
					-2, 0.8,
					DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_NONE, 0, true);

			}
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::quest::CEnemy::getClassId() {
	return nn::ts::e::ts_save_manager::CLASS_ID_ENEMY;
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::quest::CEnemy::getDataRecode() {
	return nn::ts::quest::CEnemy::dataRecode;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::endDirectCB( void * argEnmey) {

	nn::ts::quest::CEnemy * enemy = (nn::ts::quest::CEnemy *)argEnmey;

	enemy->endDirect();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::endDirect() {
	
	if(this->lm.endDirectCBFuncId != 0) {
		void (* func)( void *) = (void (*)( void *))nn::common::util::CCallBackManager::get( this->lm.endDirectCBFuncId);
		if(func != NULL) {
			func( this->lm.endDirectCBFuncOwner);
		}
	}

	this->state = nn::ts::e::enemy::STATE_IDLE;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::updateIfDead() {

	if(this->gameChar.game.enableFlag) {
		if(this->gm.status.damageHp == 0) {
			if(this->gm.param.bossFlag) {
				this->gameChar.game.enableFlag = false;
				this->gm.status.hitSkipFlag = true;
				this->gm.status.gaugeCnt++;
				if(this->gm.status.gaugeCnt>= this->gm.param.gaugeN) {
					// 最終ゲージ
					nn::dxlib::CAnim::exec( this,
						this->dm.animeIdBossExplosionSamll,
						(int)this->gameChar.game.hit.x, (int)this->gameChar.game.hit.y,
						nn::ts::e::common::Z_BATTLE_ENEMY_TURN - 5, ANIM_FLAGS_CAMERA,
						nn::ts::quest::CEnemy::nextDeadCB, this
					);
				}
				else {
					// 途中ゲージ
					nn::dxlib::CAnim::exec( this,
						this->dm.animeIdBossExplosionSamll,
						(int)this->gameChar.game.hit.x, (int)this->gameChar.game.hit.y,
						nn::ts::e::common::Z_BATTLE_ENEMY_TURN - 5, ANIM_FLAGS_CAMERA,
						nn::ts::quest::CEnemy::nextDeadCB, this
					);
				}
			}
			else {
				this->gameChar.game.enableFlag = false;
				this->gm.status.hitSkipFlag = true;
				this->state = nn::ts::e::enemy::STATE_NONE;
			}
		}
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::nextDeadCB( void * argEnemy) {

	nn::ts::quest::CEnemy * enemy = (nn::ts::quest::CEnemy *)argEnemy;

	enemy->nextDead();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::nextDead() {

	if(this->gm.status.gaugeCnt >= this->gm.param.gaugeN) {
		this->state = nn::ts::e::enemy::STATE_NONE;
	}
	else {
		if(this->state != nn::ts::e::enemy::STATE_ESCAPE) {
			this->dd->m.user.animeId = this->dm.animeIdBossEscape;
			this->dd->m.user.directEndCBFunc = nn::ts::quest::CEnemy::nextDeadCB;
			this->dd->m.user.directEndCBFuncArg = this;
			this->dd->start();

			this->state = nn::ts::e::enemy::STATE_ESCAPE;
		}
		else if(this->state == nn::ts::e::enemy::STATE_ESCAPE) {
			this->state = nn::ts::e::enemy::STATE_NONE;
		}
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::updateWeakPoint() {

	nn::ts::common::TS_HIT * next_weak_point_hits[4];
	int next_weak_point_hit_cnt = 0;

	for( int i = 0; i < this->gm.param.hitN;i ++) {

		nn::ts::common::TS_HIT * ts_hit = &this->gm.param.hits[i];
		if(ts_hit->weakPointFlag) {
			if(ts_hit->weakPointOnFlag) {
				ts_hit->weakPointOnFlag = false;
			}
			else {
				next_weak_point_hits[next_weak_point_hit_cnt] = ts_hit;
				next_weak_point_hit_cnt++;
			}
		}

	}

	if(next_weak_point_hit_cnt >= 1) {
		int index = ::n_commonRandN( next_weak_point_hit_cnt);
		next_weak_point_hits[index]->weakPointOnFlag = true;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CEnemy::dump() {
	
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CEnemy::dump() start ----------\n");
	LOG_CLASS_DUMP( "this = 0x%08x\n", this);
	LOG_CLASS_DUMP( "state = %d\n", this->state);
	LOG_CLASS_DUMP( "oldState = %d\n", this->oldState);
	LOG_CLASS_DUMP( "waitCnt = %d\n", this-waitCnt);
	LOG_CLASS_DUMP( "frameCnt = %u\n", this->frameCnt);
	LOG_CLASS_DUMP( "gm.enemyID = %d\n", this->gm.enemyID);
	LOG_CLASS_DUMP( "gm.param.hitN = %d\n", this->gm.param.hitN);
	for( int i = 0; i < this->gm.param.hitN; i++) {
	LOG_CLASS_DUMP( "gm.param.hits[%d].weakPointFlag = %d\n", i, this->gm.param.hits[i].weakPointFlag);
	LOG_CLASS_DUMP( "gm.param.hits[%d].weakPointOnFlag = %d\n", i, this->gm.param.hits[i].weakPointOnFlag);
	LOG_CLASS_DUMP( "gm.param.hits[%d].hit.hitType = %d\n", i, this->gm.param.hits[i].hit.hitType);
	LOG_CLASS_DUMP( "gm.param.hits[%d].hit.x = %d\n", i, this->gm.param.hits[i].hit.x);
	LOG_CLASS_DUMP( "gm.param.hits[%d].hit.y = %d\n", i, this->gm.param.hits[i].hit.y);
	LOG_CLASS_DUMP( "gm.param.hits[%d].hit.w = %d\n", i, this->gm.param.hits[i].hit.w);
	LOG_CLASS_DUMP( "gm.param.hits[%d].hit.h = %d\n", i, this->gm.param.hits[i].hit.h);
	LOG_CLASS_DUMP( "gm.param.hits[%d].hit.r = %d\n", i, this->gm.param.hits[i].hit.r);
	}
	LOG_CLASS_DUMP( "gm.param.gaugeN = %d\n", this->gm.param.gaugeN);
	for( int i = 0; i < this->gm.param.gaugeN; i++) {
	LOG_CLASS_DUMP( "gm.param.maxHP[%d] = %d\n", i, this->gm.param.maxHP[i]);
	}
	LOG_CLASS_DUMP( "gm.param.bossFlag = %d\n", this->gm.param.bossFlag);
	LOG_CLASS_DUMP( "gm.status.hp = %d\n", this->gm.status.hp);
	LOG_CLASS_DUMP( "gm.status.gaugeCnt = %d\n", this->gm.status.gaugeCnt);
	LOG_CLASS_DUMP( "this->gm.status.damageHp = %d\n", this->gm.status.damageHp);
	LOG_CLASS_DUMP( "gm.status.attackN = %d\n", this->gm.param.attackN);
	for( int i = 0; i < this->gm.param.attackN; i++) {
	LOG_CLASS_DUMP( "this->gm.status.attackTurns[%d].x = %d\n", i, this->gm.status.attackTurns[i]);
	LOG_CLASS_DUMP( "this->gm.param.attacks[%d].x = %d\n", i, this->gm.param.attacks[i].x);
	LOG_CLASS_DUMP( "this->gm.param.attacks[%d].y = %d\n", i, this->gm.param.attacks[i].y);
	LOG_CLASS_DUMP( "this->gm.param.attacks[%d].category = %d\n", i, this->gm.param.attacks[i].category);
	LOG_CLASS_DUMP( "this->gm.param.attacks[%d].firstTurn = %d\n", i, this->gm.param.attacks[i].firstTurn);
	LOG_CLASS_DUMP( "this->gm.param.attacks[%d].resetTurn = %d\n", i, this->gm.param.attacks[i].resetTurn);
	}
	LOG_CLASS_DUMP( "gm.stateFrameCnt = %d\n", this->gm.stateFrameCnt);
	LOG_CLASS_DUMP( "gm.collsionCtrlCnt = %d\n", this->gm.collsionCtrlCnt);
	LOG_CLASS_DUMP( "lm.quest = 0x%08x\n", this->lm.quest);
	LOG_CLASS_DUMP( "lm.endAttackCBFuncId = %d\n", this->lm.endAttackCBFuncId);
	LOG_CLASS_DUMP( "lm.endAttackCBFuncOwner = 0x%08x\n", this->lm.endAttackCBFuncOwner);
	LOG_CLASS_DUMP( "lm.endDirectCBFuncId = %d\n", this->lm.endDirectCBFuncId);
	LOG_CLASS_DUMP( "lm.endDirectCBFuncOwner = 0x%08x\n", this->lm.endDirectCBFuncOwner);
	LOG_CLASS_DUMP( "lm.dd = 0x%08x\n", this->lm.dd);
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CEnemy::dump() end   ----------\n");

}
//---------------------------------------------------------------------------

