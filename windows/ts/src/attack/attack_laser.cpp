// laser を置換
// Laser を置換
// ts を置換
// attack を置換

#include "nn_common.h"
#include "nn_common2.h"
#include "direct.h"

#include "ts_common.h"
#include "master_spark.h"
#include "battle.h"
#include "enemy.h"
#include "effect_laser.h"
#include "quest.h"
#include "ball.h"

#include "attack_laser.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::attack::CLaser::dataRecode[] = {
	DATA_END,
};
//---------------------------------------------------------------------------
nn::ts::attack::CLaser::CLaser(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CLaser Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::attack::CLaser::CLaser(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CLaser Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::attack::CLaser::CLaser() : TASK_SAVE() {

	::nn_commonLog( 1, "CLaser Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::constructCommon() {
	
	this->state = nn::ts::e::attack_laser::STATE_NONE;
	this->oldState = nn::ts::e::attack_laser::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->gm, 0, sizeof( this->gm));
	
}
//---------------------------------------------------------------------------
nn::ts::attack::CLaser::~CLaser() {

	::nn_commonLog( 1, "CLaser Delete\n");

	if(this->gm.param.endCBFunc != NULL) {
		this->gm.param.endCBFunc( this->gm.param.endCBFuncArg);
	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::start() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::startCommon() {

	if(this->gm.param .type == nn::ts::e::attack_laser::LASER_TYPE_MASTER_SPARK) {
		nn::ts::effect::CMasterSpark * ms = new nn::ts::effect::CMasterSpark( this);
		ms->gm.param.x = (int)(this->gm.param.x);
		ms->gm.param.y = (int)(this->gm.param.y);
		ms->gm.param.angle = this->gm.param.angle;
		ms->gm.param.w = 1000;
		ms->gm.param.h = this->gm.param.width;
		ms->gm.param.blank = 35;
		ms->gm.param.cameraFlag = true;
		ms->gm.param.endCBFunc = nn::ts::attack::CLaser::endEffectLaserCB;
		ms->gm.param.endCBFuncArg = this;

		ms->start();

		nn::dxlib::direct::waitSimple( this, 10,
			nn::ts::attack::CLaser::damgeCB, this);
		nn::dxlib::direct::waitSimple( this, 20,
			nn::ts::attack::CLaser::damgeCB, this);
		nn::dxlib::direct::waitSimple( this, 30,
			nn::ts::attack::CLaser::damgeCB, this);

		this->gm.x = this->gm.param.x;
		this->gm.y = this->gm.param.y;
		this->gm.angle = this->gm.param.angle;
		this->gm.len = 1000;

		this->state = nn::ts::e::attack_laser::STATE_ACTIVE;
	}
	else if(this->gm.param.type == nn::ts::e::attack_laser::LASER_TYPE_REFLECTION) {
		this->nextReflection( this->gm.param.x, this->gm.param.y,  this->gm.param.angle);

		this->state = nn::ts::e::attack_laser::STATE_ACTIVE;
	}
}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::nextReflection( int x, int y, int angle) {
	
	nn::ts::effect::CLaser * laser = new nn::ts::effect::CLaser( this);
	laser->gm.param.x = x;
	laser->gm.param.y = y;
	laser->gm.param.angle = angle;

	std::vector<nn::common::util::type::t2d::COLLISION_ITEM> collisions;
	this->gm.param.quest->lm.battle->addFieldCollision( &collisions);
			
	std::vector<nn::common::util::type::t2d::COLLISION_ITEM>::iterator it;
	double x1 = laser->gm.param.x;
	double y1 = laser->gm.param.y;
	double x2 = laser->gm.param.x + nn::common::math::angleSpeed2MoveX( laser->gm.param.angle, 1000.0);
	double y2 = laser->gm.param.y + nn::common::math::angleSpeed2MoveY( laser->gm.param.angle, 1000.0);
	double x3;
	double y3;
	double x4;
	double y4;
	bool flag = false;
	for( it = collisions.begin(); it != collisions.end(); it++) {

		bool collision = nn::common::math::isCollisionLine2LineEx(
						&x3, &y3, &x4, &y4,
						x1, y1, x2, y2,
						it->p.x1, it->p.y1, it->p.x2, it->p.y2);
		if(collision) {
			flag = true;
			break;
		}

	}
	nn_commonLog( 1, "x1 = %lf y1 = %lf x2 = %lf y2 = %lf\n", x1, y1, x2, y2);
	nn_commonLog( 1, "angle= %d\n", laser->gm.param.angle);
	laser->gm.param.w = (int)nn::common::math::getLength( x1, y1, x3, y3);
	laser->gm.param.h = this->gm.param.width;
	laser->gm.param.speed = 100;
	laser->gm.param.blank = 0;
	laser->gm.param.atribute = this->gm.param.attribute;
	laser->gm.param.cameraFlag = true;
	laser->gm.param.landingCBFunc = nn::ts::attack::CLaser::landingCB;
	laser->gm.param.landingCBFuncArg = this;
	laser->gm.param.endCBFunc = nn::ts::attack::CLaser::endEffectLaserCB;
	laser->gm.param.endCBFuncArg = this;
	laser->start();
	
	this->gm.laserCnt++;

	this->gm.reflectionX = (int)x3;
	this->gm.reflectionY = (int)y3;
	this->gm.reflectionAngle = (laser->gm.param.angle * 1000) % 180000;
	this->gm.reflectionAngle = -(this->gm.reflectionAngle - 90000);
	this->gm.reflectionAngle = this->gm.reflectionAngle + ((laser->gm.param.angle * 1000) / 180000) * 180000 + 90000;
	if(it->shape == nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_UP ||
		it->shape == nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_DOWN
	) {
		this->gm.reflectionAngle = (this->gm.reflectionAngle + 180000) % 360000;
	}
	this->gm.reflectionCnt++;

	this->gm.x = laser->gm.param.x;
	this->gm.y = laser->gm.param.y;
	this->gm.angle = laser->gm.param.angle;
	this->gm.len = laser->gm.param.w;

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::landingCB( void * argLaser) {

	nn::ts::attack::CLaser * laser = (nn::ts::attack::CLaser *)argLaser;

	laser->landing();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::landing() {

	this->damage();

	if(this->gm.reflectionCnt < this->gm.param.reflectionLaserN) {
		this->nextReflection( this->gm.reflectionX, this->gm.reflectionY,
			this->gm.reflectionAngle / 1000);
	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::damgeCB( void * argLaser) {

	nn::ts::attack::CLaser * laser = (nn::ts::attack::CLaser *)argLaser;

	laser->damage();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::damage() {

	nn::ts::quest::CBattle * battle = this->gm.param.quest->lm.battle;
	
	std::vector<nn::common::util::type::t2d::COLLISION_ITEM> collisions;

	if(this->gm.param.hitTargetBit & nn::ts::e::common::HIT_TARGET_BIT_ENEMY) {
		battle->addEnemyCollision( &collisions);
	}
	if(this->gm.param.hitTargetBit & nn::ts::e::common::HIT_TARGET_BIT_BALL) {
		this->gm.param.quest->addBallCollision( &collisions);
	}
	
	std::vector<nn::common::util::type::t2d::COLLISION_ITEM>::iterator it;
	for( it = collisions.begin(); it != collisions.end(); it++) {
		
		double collision_x;
		double collision_y;
		double after_x_rate;
		double after_y_rate;
		double after_len;

		double x1 = this->gm.x;
		double y1 = this->gm.y;
		double x2 = this->gm.x + nn::common::math::angleSpeed2MoveX( this->gm.angle, this->gm.len);
		double y2 = this->gm.y + nn::common::math::angleSpeed2MoveY( this->gm.angle, this->gm.len);
		bool collision = nn::common::math::isCollisionLine2CircleEX(
			&collision_x, &collision_y, &after_x_rate, &after_y_rate, &after_len,
			it->p.x1, it->p.y1, it->r + this->gm.param.width / 2,
			x1, y1, x2, y2);
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
			}

	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::endEffectLaserCB( void * argLaser) {

	nn::ts::attack::CLaser * laser = (nn::ts::attack::CLaser *)argLaser;

	laser->endEffectLaser();

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::endEffectLaser() {
	
	if(this->gm.param .type == nn::ts::e::attack_laser::LASER_TYPE_MASTER_SPARK) {
		this->taskDelete();
	}
	else if(this->gm.param.type == nn::ts::e::attack_laser::LASER_TYPE_REFLECTION) {
		this->gm.laserCnt--;
		if(this->gm.laserCnt == 0) {
			this->taskDelete();
		}
	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CLaser::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::attack_laser::STATE_NONE:
			break;
		case nn::ts::e::attack_laser::STATE_ACTIVE:
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::attack::CLaser::disp() {

	switch(this->state) {
		case nn::ts::e::attack_laser::STATE_NONE:
			break;
		case nn::ts::e::attack_laser::STATE_ACTIVE:
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::attack::CLaser::getClassId() {
	return -1;		// ToDo
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::attack::CLaser::getDataRecode() {
	return nn::ts::attack::CLaser::dataRecode;
}
//---------------------------------------------------------------------------

