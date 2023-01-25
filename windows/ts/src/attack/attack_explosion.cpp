// explosion を置換
// Explosion を置換
// ts を置換
// attack を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "anim.h"
#include "direct.h"

#include "anime_data.h"

#include "ts_common.h"
#include "battle.h"
#include "quest.h"
#include "enemy.h"
#include "ball.h"

#include "attack_explosion.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::attack::CExplosion::dataRecode[] = {
	DATA_END,
};
//---------------------------------------------------------------------------
nn::ts::attack::CExplosion::CExplosion(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CExplosion Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::attack::CExplosion::CExplosion(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CExplosion Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::attack::CExplosion::CExplosion() : TASK_SAVE() {

	::nn_commonLog( 1, "CExplosion Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::constructCommon() {
	
	this->state = nn::ts::e::explosion::STATE_NONE;
	this->oldState = nn::ts::e::explosion::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->gm, 0, sizeof( this->gm));
	
}
//---------------------------------------------------------------------------
nn::ts::attack::CExplosion::~CExplosion() {

	::nn_commonLog( 1, "CExplosion Delete\n");

	if(this->gm.param.endCBFunc != NULL) {
		this->gm.param.endCBFunc( this->gm.param.endCBFuncArg);
	}
}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::start() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::startCommon() {

	if(this->gm.param.r >= 150) {
		nn::dxlib::CAnim::exec( this, ANIMATION_ID_EXPLOSION_LARGE,
			this->gm.param.x, this->gm.param.y, 0,
			ANIM_FLAGS_CAMERA, endEffectExplosionCB, this
		);
	}
	else {
		nn::dxlib::CAnim::exec( this, ANIMATION_ID_EXPLOSION,
			this->gm.param.x, this->gm.param.y, 0,
			ANIM_FLAGS_CAMERA, endEffectExplosionCB, this
		);
	}

	nn::dxlib::direct::waitSimple( this,
		10, nn::ts::attack::CExplosion::landingCB, this
	);

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::endEffectExplosionCB( void * argExplosion) {

	nn::ts::attack::CExplosion * explosion = (nn::ts::attack::CExplosion *)argExplosion;

	explosion->endEffectExplosion();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::endEffectExplosion() {

	this->taskDelete();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::landingCB( void * argExplosion) {

	nn::ts::attack::CExplosion * explosion = (nn::ts::attack::CExplosion *)argExplosion;

	explosion->landing();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::landing() {

	std::vector<nn::common::util::type::t2d::COLLISION_ITEM> collisions;

	if(this->gm.param.hitTargetBit & nn::ts::e::common::HIT_TARGET_BIT_ENEMY) {
		this->gm.param.battle->addEnemyCollision( &collisions);
	}
	this->gm.param.quest->addBallCollision( &collisions);
	
	std::vector<nn::common::util::type::t2d::COLLISION_ITEM>::iterator it;


	for( it = collisions.begin(); it != collisions.end(); it++) {

		bool collision = nn::common::math::isCollisionCircle2Circle(
			this->gm.param.x, this->gm.param.y, this->gm.param.r,
			it->p.x1, it->p.y1, it->r
		);
					
		if(collision) {
			if(it->id == nn::ts::e::common::COLLISION_ID_ENEMY) {
				nn::ts::common::ATTACKER_INFO ai;
				ai.hitCase = nn::ts::e::common::HIT_CASE_SHOT_2_ENMEY;
				ai.damage = this->gm.param.damage;

				nn::ts::common::hit( &(*it), &ai);
			}
			else if(it->id == nn::ts::e::common::COLLISION_ID_BALL) {
				if(this->gm.param.hitTargetBit & nn::ts::e::common::HIT_TARGET_BIT_FRIEND) {
					nn::ts::quest::CBall * target = (nn::ts::quest::CBall *)it->target;
					target->friendCombo( this->gm.param.ball);
				}
				else if(this->gm.param.hitTargetBit & nn::ts::e::common::HIT_TARGET_BIT_BALL) {
					nn::ts::quest::CBall * target = (nn::ts::quest::CBall *)it->target;
					target->damage( this->gm.param.damage);
				}
			}
		}

	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CExplosion::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::explosion::STATE_NONE:
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CExplosion::disp() {

	switch(this->state) {
		case nn::ts::e::explosion::STATE_NONE:
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::attack::CExplosion::getClassId() {
	return -1;		// ToDo
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::attack::CExplosion::getDataRecode() {
	return nn::ts::attack::CExplosion::dataRecode;
}
//---------------------------------------------------------------------------

