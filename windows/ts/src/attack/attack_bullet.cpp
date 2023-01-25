// attack_bullet を置換
// Bullet を置換
// ts を置換
// attack を置換

#include "nn_common.h"
#include "nn_common2.h"
#include "direct.h"

#include "battle.h"
#include "enemy.h"
#include "effect_bullet.h"
#include "quest.h"
#include "ball.h"

#include "attack_bullet.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::attack::CBullet::dataRecode[] = {
	DATA_END,
};
//---------------------------------------------------------------------------
nn::ts::attack::CBullet::CBullet(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CBullet Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::attack::CBullet::CBullet(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CBullet Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::attack::CBullet::CBullet() : TASK_SAVE() {

	::nn_commonLog( 1, "CBullet Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::constructCommon() {
	
	this->state = nn::ts::e::attack_bullet::STATE_NONE;
	this->oldState = nn::ts::e::attack_bullet::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->gm, 0, sizeof( this->gm));
	
}
//---------------------------------------------------------------------------
nn::ts::attack::CBullet::~CBullet() {

	::nn_commonLog( 1, "CBullet Delete\n");

	if(this->gm.param.endCBFunc != NULL) {
		this->gm.param.endCBFunc( this->gm.param.endCBFuncArg);
	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::start() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::startCommon() {

	this->gm.angle = this->gm.param.angle[0];

	this->fire();

	nn::dxlib::direct::waitSimple( this,
		this->gm.param.fireIntervalFrameN,
		nn::ts::attack::CBullet::fireIntervalCB, this);


}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::fireIntervalCB( void * argAttackBullet) {

	nn::ts::attack::CBullet * attack_bullet = (nn::ts::attack::CBullet *)argAttackBullet;

	attack_bullet->fireInterval();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::fireInterval() {

	this->gm.fireCnt++;

	if(this->gm.fireCnt < this->gm.param.fireN) {
		this->fire();
		
		nn::dxlib::direct::waitSimple( this,
			this->gm.param.fireIntervalFrameN,
			nn::ts::attack::CBullet::fireIntervalCB, this);
	}
	else {
		if(this->gm.bulletCnt == 0) {
			this->taskDelete();
		}
	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::endEffectBulletCB( void * argAttackBullet) {

	nn::ts::attack::CBullet * attack_bullet = (nn::ts::attack::CBullet *)argAttackBullet;

	attack_bullet->endEffectBullet();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::endEffectBullet() {

	this->gm.bulletCnt--;
	if(this->gm.bulletCnt == 0 && this->gm.fireCnt == this->gm.param.fireN) {
		this->taskDelete();
	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::fire() {

	if(this->gm.param.type == nn::ts::e::attack_bullet::TYPE_HOMING) {
		for( int i = 0; i < this->gm.param.fireBulletN; i++) {
		
			nn::ts::effect::CBullet * bullet = new nn::ts::effect::CBullet( this);
			bullet->gm.param.x = this->gm.param.x;
			bullet->gm.param.y = this->gm.param.y;
			bullet->gm.param.angle = (int)this->gm.angle;
			bullet->gm.param.speed = 4.0;
			bullet->gm.param.speedAdd = 0.0;
			bullet->gm.param.angleSpeed = 3.0;
			bullet->gm.param.angleSpeedAdd = 0.015;
			if(this->gm.param.targetN >= 1) {
				int select = n_commonRandN( this->gm.param.targetN);
				bullet->gm.param.target = this->gm.param.targets[select];
			}
			else {
				bullet->gm.param.target = NULL;
			}
			bullet->gm.param.type = nn::ts::e::effect_bullet::TYPE_HOMING;
			bullet->gm.param.attribute = this->gm.param.attribute;
			bullet->gm.param.moveCBFunc = nn::ts::attack::CBullet::moveBulletCB;
			bullet->gm.param.moveCBuncArg = this;
			bullet->gm.param.liveRect.x = this->gm.param.quest->lm.battle->gm.x - 256;
			bullet->gm.param.liveRect.y = this->gm.param.quest->lm.battle->gm.y - 256;
			bullet->gm.param.liveRect.w = this->gm.param.quest->lm.battle->gm.w + 512;
			bullet->gm.param.liveRect.h = this->gm.param.quest->lm.battle->gm.h + 512;
			bullet->gm.param.endCBFunc = nn::ts::attack::CBullet::endEffectBulletCB;
			bullet->gm.param.endCBFuncArg = this;
			bullet->start();
			this->gm.bulletCnt++;

			this->gm.angle = nn::common::math::varAngle( this->gm.angle, this->gm.param.addAngle[0]);

		}
	}
	else if(this->gm.param.type == nn::ts::e::attack_bullet::TYPE_MACHINEGUN) {
		this->gm.angle = this->gm.param.angle[this->gm.fireCnt];
		this->gm.fireBulletCnt = 0;

		this->state = nn::ts::e::attack_bullet::STATE_FIRE;
	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::moveBulletCB( void * argAttackBullet, void * argEffectBullet,
		double x1, double y1, double x2, double y2) {

	nn::ts::attack::CBullet * attack_bullet = (nn::ts::attack::CBullet *)argAttackBullet;

	attack_bullet->moveBullet( argEffectBullet, x1, y1, x2, y2);

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::moveBullet( void * argEffectBullet,
		double x1, double y1, double x2, double y2) {
	
	nn::ts::effect::CBullet * effect_bullet = (nn::ts::effect::CBullet *)argEffectBullet;
	if(effect_bullet->gm.param.target != NULL) {
		if(effect_bullet->gm.param.target->gameChar.game.enableFlag == false) {
			this->updateTarget( effect_bullet);
		}
	}

	std::vector<nn::common::util::type::t2d::COLLISION_ITEM> collisions;

	if(this->gm.param.hitTargetBit & nn::ts::e::common::HIT_TARGET_BIT_ENEMY) {
		this->gm.param.quest->lm.battle->addEnemyCollision( &collisions);
	}
	if(this->gm.param.hitTargetBit & nn::ts::e::common::HIT_TARGET_BIT_BALL) {
		this->gm.param.quest->addBallCollision( &collisions);
	}


	bool collision = false;
	std::vector<nn::common::util::type::t2d::COLLISION_ITEM>::iterator it;
	for( it = collisions.begin(); it != collisions.end(); it++) {
		
		double collision_x;
		double collision_y;
		double after_x_rate;
		double after_y_rate;
		double after_len;
		collision = nn::common::math::isCollisionLine2CircleEX(
			&collision_x, &collision_y, &after_x_rate, &after_y_rate, &after_len,
			it->p.x1, it->p.y1, it->r,
			x1,y1, x2,y2);
		if(collision) {
			break;
		}

	}

	if(collision) {
		if(it->id == nn::ts::e::common::COLLISION_ID_ENEMY) {
			nn::ts::common::ATTACKER_INFO ai;
			ai.hitCase = nn::ts::e::common::HIT_CASE_SHOT_2_ENMEY;
			ai.damage = this->gm.param.damage;

			nn::ts::common::hit( &(*it), &ai);
		}
		else if(it->id == nn::ts::e::common::COLLISION_ID_BALL) {
			nn::ts::quest::CBall * ball = (nn::ts::quest::CBall *)it->target;
			ball->damage( this->gm.param.damage);
		}

		effect_bullet->taskDelete();
	}
	
}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CBullet::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::attack_bullet::STATE_NONE:
			break;
		case nn::ts::e::attack_bullet::STATE_FIRE:
			if(this->gm.fireBulletCnt < this->gm.param.fireBulletN) {
				nn::ts::effect::CBullet * bullet = new nn::ts::effect::CBullet( this);
				bullet->gm.param.x = this->gm.param.x;
				bullet->gm.param.y = this->gm.param.y;
				bullet->gm.param.angle = (int)this->gm.angle;
				bullet->gm.param.speed = 6.0;
				//bullet->gm.param.angleSpeed = 0.0;
				//bullet->gm.param.targetX = this->gm.param.targetX;
				//bullet->gm.param.targetY = this->gm.param.targetY;
				bullet->gm.param.type = nn::ts::e::effect_bullet::TYPE_STRAIGHT;
				bullet->gm.param.attribute = this->gm.param.attribute;
				bullet->gm.param.moveCBFunc = nn::ts::attack::CBullet::moveBulletCB;
				bullet->gm.param.moveCBuncArg = this;
				bullet->gm.param.liveRect.x = this->gm.param.quest->lm.battle->gm.x - 256;
				bullet->gm.param.liveRect.y = this->gm.param.quest->lm.battle->gm.y - 256;
				bullet->gm.param.liveRect.w = this->gm.param.quest->lm.battle->gm.w + 512;
				bullet->gm.param.liveRect.h = this->gm.param.quest->lm.battle->gm.h + 512;
				bullet->gm.param.endCBFunc = nn::ts::attack::CBullet::endEffectBulletCB;
				bullet->gm.param.endCBFuncArg = this;
				bullet->start();
				this->gm.bulletCnt++;

				this->gm.angle = nn::common::math::varAngle( this->gm.angle, this->gm.param.addAngle[this->gm.fireCnt]);

				this->gm.fireBulletCnt++;
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::disp() {

	switch(this->state) {
		case nn::ts::e::attack_bullet::STATE_NONE:
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::attack::CBullet::getClassId() {
	return -1;		// ToDo
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::attack::CBullet::getDataRecode() {
	return nn::ts::attack::CBullet::dataRecode;
}
//---------------------------------------------------------------------------
void nn::ts::attack::CBullet::updateTarget( nn::ts::effect::CBullet * effectBullet) {

	nn::common::CGameChar * targets[16];
	int target_n = 0;

	for( int i = 0; i < this->gm.param.targetN; i++) {

		if(this->gm.param.targets[i]->gameChar.game.enableFlag) {
			targets[target_n] = this->gm.param.targets[i];
			target_n++;
		}

	}

	memcpy( this->gm.param.targets, targets, sizeof( this->gm.param.targets));
	this->gm.param.targetN = target_n;

	if(this->gm.param.targetN >= 1) {
		int select = n_commonRandN( this->gm.param.targetN);
		effectBullet->gm.param.target = this->gm.param.targets[select];
	}
	else {
		effectBullet->gm.param.target = NULL;
	}

}
//---------------------------------------------------------------------------

