// ball ‚ð’uŠ·
// Ball ‚ð’uŠ·
// ts ‚ð’uŠ·
// quest ‚ð’uŠ·

#include "nn_common.h"
#include "nn_common2.h"

#include "disp.h"

#include "ts_common.h"
#include "ts_save_manager.h"

#include "quest.h"
#include "battle.h"
#include "enemy.h"
#include "attack_laser.h"
#include "effect_laser.h"
#include "attack_explosion.h"
#include "attack_bullet.h"
#include "damage_disp.h"
#include "battle_item.h"

#include "ball.h"
//---------------------------------------------------------------------------
enum {
	UP_DOWN_UP,
	UP_DOWN_DOWN,

	UP_DOWN_MAX
};
//---------------------------------------------------------------------------
enum {
	LEFT_RIGHT_LEFT,
	LEFT_RIGHT_RIGHT,

	LEFT_RIGHT_MAX
};
//---------------------------------------------------------------------------
enum {
	REFRECTION_DEFAULT,
	REFRECTION_HORIZONTAL,
	REFRECTION_VERTICAL,

	REFRECTION_MAX
};
	
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::quest::CBall::dataRecode[] = {
	DATA_LOCAL( nn::ts::quest::CBall, state),
	DATA_LOCAL( nn::ts::quest::CBall, oldState),
	DATA_LOCAL( nn::ts::quest::CBall, waitCnt),
	DATA_LOCAL( nn::ts::quest::CBall, gm),
	DATA_PTR( nn::ts::quest::CBall, lm.quest),
	DATA_BASE(::TASK_SAVE::dataRecode),
	DATA_END,
};

nn::ts::quest::BALL_DISP_MEMBER nn::ts::quest::CBall::dm;
//---------------------------------------------------------------------------
nn::ts::quest::CBall::CBall(TASK_SAVE * oya, nn_common::SaveManager * m) : nn::common::CGameChar( oya, m) {

	::nn_commonLog( 1, "CBall Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CBall::CBall(TASK_SAVE * oya) : nn::common::CGameChar( oya) {

	::nn_commonLog( 1, "CBall Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CBall::CBall() : nn::common::CGameChar() {

	::nn_commonLog( 1, "CBall Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::constructCommon() {
	
	this->state = nn::ts::e::ball::STATE_NONE;
	this->oldState = nn::ts::e::ball::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	this->shotEndCBFunc = NULL;
	this->shotEndCBFuncArg = NULL;

	memset( &this->gm, 0, sizeof( this->gm));
	
	this->loopCollisionStopCnt = 0;
	memset( this->loopCollisionStopP, 0, sizeof( this->loopCollisionStopP));

	this->dispDirect = new nn::dxlib::direct::CDispDirect();
	
	memset( &this->lm, 0, sizeof( this->lm));

	strcpy_s( this->name, sizeof( this->name), "CBall");
	
}
//---------------------------------------------------------------------------
nn::ts::quest::CBall::~CBall() {

	::nn_commonLog( 1, "CBall Delete\n");

	delete this->dispDirect;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::start() {
	
	this->startCommon();

	this->dispDirect->start();

	this->state = nn::ts::e::ball::STATE_IDLE;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::startCommon() {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::deleteDamageDisp() {

	if(this->lm.dd != NULL) {
		this->lm.dd->gm.deleteFlag = true;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::finDeleteDamgeDispCB( void * argBall) {

	nn::ts::quest::CBall * ball = (nn::ts::quest::CBall *)argBall;

	ball->finDeleteDamgeDisp();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::finDeleteDamgeDisp() {

	this->lm.dd = NULL;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::damage( int damage) {

	this->lm.quest->damage( damage);

	if(this->lm.dd == NULL) {
		nn::ts::quest::CDamageDisp * dd = new nn::ts::quest::CDamageDisp( this);
		dd->gm.param.startX = (int)this->gm.status.x;
		dd->gm.param.startY = (int)this->gm.status.y + 15;
		dd->gm.param.endX =   (int)this->gm.status.x;
		dd->gm.param.endY =   (int)this->gm.status.y - 25;
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
		dd->gm.param.deleteCBFunc = nn::ts::quest::CBall::finDeleteDamgeDispCB;
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
void nn::ts::quest::CBall::recovery( int percent) {

	this->lm.quest->recovery( percent);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CBall::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::ball::STATE_NONE:
			break;
		case nn::ts::e::ball::STATE_IDLE:
			break;
		case nn::ts::e::ball::STATE_SHOT:
			this->taskShot();
			break;
		case nn::ts::e::ball::STATE_MOVE:
			this->taskMove();
			break;
		case nn::ts::e::ball::STATE_DIRECT:
			this->dispDirect->task();
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::disp() {

	switch(this->state) {
		case nn::ts::e::ball::STATE_NONE:
			break;
		case nn::ts::e::ball::STATE_IDLE:
		case nn::ts::e::ball::STATE_SHOT:
		case nn::ts::e::ball::STATE_MOVE:
		case nn::ts::e::ball::STATE_DIRECT:
			int z = nn::ts::e::common::Z_BATTLE_BALL_WAIT;
			if(this->lm.quest->gm.status.turn == this->gm.turn) {
				z = nn::ts::e::common::Z_BATTLE_BALL_TURN;
			}
			nn::dxlib::disp::objCenter( this->dm.objIdBalls[this->gm.charID],
				(int)this->gm.status.x + this->dispDirect->m.ctrl.add.x,
				(int)this->gm.status.y + this->dispDirect->m.ctrl.add.y,
				z);

			nn::dxlib::disp::objCenterFull( this->dm.objIdBallBgs[this->gm.charID],
				(int)this->gm.status.x + this->dispDirect->m.ctrl.add.x,
				(int)this->gm.status.y + this->dispDirect->m.ctrl.add.y,
				z + 1,
				0, 0, 0, FALSE, FALSE,
				DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_ADD);
#ifdef _DEBUG
			for( int i = 0; i < this->loopCollisionStopCnt; i++) {

				nn::dxlib::disp::line(
					(int)(this->loopCollisionStopP[i].x1), (int)(this->loopCollisionStopP[i].y1),
					(int)(this->loopCollisionStopP[i].x2), (int)(this->loopCollisionStopP[i].y2),
					0, 1, DISP_RED, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_NONE, true);

			}
#endif
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::addOtherBallCollision( std::vector<nn::common::util::type::t2d::COLLISION_ITEM> * collisions) {
	
	nn::ts::quest::CQuest * quest = this->lm.quest;

	for( int i = 0; i < nn::ts::e::common::QUEST_BALL_N; i++) {

		nn::ts::quest::CBall * ball = quest->lm.balls[i];
		if(ball->gm.turn != this->gm.turn) {
			nn::common::util::type::t2d::COLLISION_ITEM ci;
			ci.shape = nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_CIRCLE;
			ci.p.x1 = ball->gm.status.x;
			ci.p.y1 = ball->gm.status.y;
			ci.r = nn::ts::e::common::BALL_R;
			ci.id = nn::ts::e::common::COLLISION_ID_BALL;
			ci.target = ball;

			collisions->push_back( ci);

		}

	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::hitEnemyReflec( nn::ts::quest::BALL_TASK_SHOT_MEMBER * tsm,
	nn::common::util::type::t2d::COLLISION_ITEM * ci) {
	
	if(tsm->cnt >= 5) {
		this->loopCollisionStopCnt = tsm->cnt - 1;
		for( int i = 0; i < tsm->cnt - 1; i++) {

			this->loopCollisionStopP[i].x1 = tsm->log.collisionX1s[i];
			this->loopCollisionStopP[i].y1 = tsm->log.collisionY1s[i];
			this->loopCollisionStopP[i].x2 = tsm->log.collisionX2s[i];
			this->loopCollisionStopP[i].y2 = tsm->log.collisionY2s[i];

			nn_commonLog( 1, "collision[%d]\n", i);
			nn_commonLog( 1, "circle x=%lf y=%lf r=%lf\n", tsm->log.cis[i].p.x1, tsm->log.cis[i].p.y1, tsm->log.cis[i].r);
			nn_commonLog( 1, "ball = x1=%lf y1=%lf x2=%lf y2=%lf\n",
				tsm->log.collisionX1s[i], tsm->log.collisionY1s[i],
				tsm->log.collisionX2s[i], tsm->log.collisionY2s[i]);
			nn_commonLog( 1, "angle = %d\n", tsm->log.angles[i]);
			nn_commonLog( 1, "after_angle=%d\n", tsm->log.afterAngles[i]);
			nn_commonLog( 1, "after_len=%lf\n", tsm->log.afterLens[i]);
			nn_commonLog( 1, "circle2ball=%lf\n", tsm->log.ci2Balls[i]);

		}

		tsm->x1 = tsm->collisionX + 2.0 * tsm->afterXRate;
		tsm->y1 = tsm->collisionY + 2.0 * tsm->afterYRate;
		tsm->x2 = tsm->x1;
		tsm->y2 = tsm->y1;
		tsm->collision = false;
		this->gm.status.speed = 0.0;
	}
	else {
		int center_range = 1000;
		int reflection = REFRECTION_DEFAULT;
		if((  45000 + center_range <= this->gm.shotAngle && this->gm.shotAngle <= 135000 - center_range) ||
			(225000 + center_range <= this->gm.shotAngle && this->gm.shotAngle <= 315000 - center_range)) {
			// “üŽËŠpc
				int in_base = 0;
				if(45000 + center_range <= this->gm.shotAngle && this->gm.shotAngle <= 135000 - center_range) {
					in_base =  90000;
				}
				else {
					in_base = 270000;
				}

				int hit_angle = (int)(1000.0 * nn::common::math::pos2Angle_d(
					ci->p.x1, ci->p.y1, tsm->collisionX, tsm->collisionY));
				int hit_diff = 0;
				int hit_left_right = 0;
				if(hit_angle / 90000 == 0) {
					hit_diff = hit_angle;
					hit_left_right = LEFT_RIGHT_RIGHT;
				}
				else if(hit_angle / 90000 == 1) {
					hit_diff = 180000 - hit_angle;
					hit_left_right = LEFT_RIGHT_LEFT;
				}
				else if(hit_angle / 90000 == 2) {
					hit_diff = hit_angle - 180000;
					hit_left_right = LEFT_RIGHT_LEFT;
				}
				else {
					hit_diff = 360000 - hit_angle;
					hit_left_right = LEFT_RIGHT_RIGHT;
				}

				int in_diff = in_base - this->gm.shotAngle;
				int in_left_right = 0;
				if(in_base == 90000) {
					if(in_diff < 0) {
						in_left_right = LEFT_RIGHT_RIGHT;
					}
					else {
						in_left_right = LEFT_RIGHT_LEFT;
					}
				}
				else {
					if(in_diff < 0) {
						in_left_right = LEFT_RIGHT_LEFT;
					}
					else {
						in_left_right = LEFT_RIGHT_RIGHT;
					}
				}
				if(in_diff < 0) {
					in_diff = -in_diff;
				}

				if(hit_left_right == in_left_right && hit_diff < in_diff) {
					// ‰¡”½ŽË
					reflection = REFRECTION_HORIZONTAL;
				}
				else {
					// c”½ŽË
					reflection = REFRECTION_VERTICAL;
				}
		}
		else if(                                           this->gm.shotAngle <=  45000 - center_range ||
			135000 + center_range <= this->gm.shotAngle && this->gm.shotAngle <= 225000 - center_range ||
			315000 + center_range <= this->gm.shotAngle) {
			// “üŽËŠp‰¡
				int in_base = 0;
				if(this->gm.shotAngle <=  45000 - center_range) {
					in_base =      0;
				}
				else if(135000 + center_range <= this->gm.shotAngle && this->gm.shotAngle <= 225000 - center_range) {
					in_base = 180000;
				}
				else {
					in_base = 360000;
				}

				int hit_angle = (int)(1000.0 * nn::common::math::pos2Angle_d(
					ci->p.x1, ci->p.y1, tsm->collisionX, tsm->collisionY));
				int hit_diff = 0;
				int hit_up_down = 0;
				if(hit_angle / 90000 == 0) {
					hit_diff = 90000 - hit_angle;
					hit_up_down = UP_DOWN_UP;
				}
				else if(hit_angle / 90000 == 1) {
					hit_diff = hit_angle - 90000;
					hit_up_down = UP_DOWN_UP;
				}
				else if(hit_angle / 90000 == 2) {
					hit_diff = 270000 - hit_angle;
					hit_up_down = UP_DOWN_DOWN;
				}
				else {
					hit_diff = hit_angle - 270000;
					hit_up_down = UP_DOWN_DOWN;
				}

				int in_diff = in_base - this->gm.shotAngle;
				int in_up_down = 0;
				if(in_base == 0) {
					in_up_down = 1;
				}
				else if(in_base == 180000) {
					if(in_diff < 0) {
						in_up_down = UP_DOWN_DOWN;
					}
					else {
						in_up_down = UP_DOWN_UP;
					}
				}
				else {
					in_up_down = UP_DOWN_UP;
				}
				if(in_diff < 0) {
					in_diff = -in_diff;
				}

				if(hit_up_down == in_up_down && hit_diff < in_diff) {
					// c”½ŽË
					reflection = REFRECTION_VERTICAL;
				}
				else {
					// ‰¡”½ŽË
					reflection = REFRECTION_HORIZONTAL;
				}
		}
		if(reflection == REFRECTION_VERTICAL) {
			tsm->afterAngle = this->gm.shotAngle % 180000;
			tsm->afterAngle = -(tsm->afterAngle - 90000);
			tsm->afterAngle = tsm->afterAngle + (this->gm.shotAngle / 180000) * 180000 + 90000;
			tsm->afterAngle = (tsm->afterAngle + 180000) % 360000;
		}
		else if(reflection == REFRECTION_HORIZONTAL) {
			tsm->afterAngle = this->gm.shotAngle % 180000;
			tsm->afterAngle = -(tsm->afterAngle - 90000);
			tsm->afterAngle = tsm->afterAngle + (this->gm.shotAngle / 180000) * 180000 + 90000;
		}
		else {
			tsm->afterAngle = (this->gm.shotAngle + 180000) % 360000;
		}

		tsm->x1 = tsm->collisionX;
		tsm->y1 = tsm->collisionY;
	}

	tsm->log.cis         [tsm->cnt - 1] = *ci;
	tsm->log.afterAngles [tsm->cnt - 1] = tsm->afterAngle;
	tsm->log.afterLens   [tsm->cnt - 1] = tsm->afterLen;
	tsm->log.collisionX1s[tsm->cnt - 1] = tsm->x1;
	tsm->log.collisionY1s[tsm->cnt - 1] = tsm->y1;
	tsm->log.collisionX2s[tsm->cnt - 1] = tsm->x1 + nn::common::math::angleSpeed2MoveX( tsm->afterAngle / 1000.0, tsm->afterLen);
	tsm->log.collisionY2s[tsm->cnt - 1] = tsm->y1 + nn::common::math::angleSpeed2MoveY( tsm->afterAngle / 1000.0, tsm->afterLen);
	tsm->log.ci2Balls    [tsm->cnt - 1] = nn::common::math::getLength( ci->p.x1, ci->p.y1, tsm->x1, tsm->y1);
	tsm->log.angles      [tsm->cnt - 1] = this->gm.shotAngle;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::hitEnemy( nn::ts::quest::BALL_TASK_SHOT_MEMBER * tsm,
	nn::common::util::type::t2d::COLLISION_ITEM * ci) {

	this->gm.status.speed -= 2.0;

	this->hitEnemyReflec( tsm, ci);

	nn::ts::common::ATTACKER_INFO ai;
	ai.hitCase = nn::ts::e::common::HIT_CASE_BALL_2_ENMEY;
	ai.damage = this->gm.param.ap;

	nn::ts::common::hit( ci, &ai);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::friendCombo( nn::ts::quest::CBall * parther) {
	
	if(this->gm.friendComboFlag == false) {
		if(this->gm.charID == nn::ts::e::common::CHAR_ID_MARISA) {
			nn::ts::attack::CLaser * laser = new nn::ts::attack::CLaser( this);
			laser->gm.param.x = (int)(this->gm.status.x);
			laser->gm.param.y = (int)(this->gm.status.y);
			laser->gm.param.angle = 90;
			laser->gm.param.quest = this->lm.quest;
			laser->gm.param.width = 200;
			laser->gm.param.damage = 20900;
			laser->gm.param.type = nn::ts::e::attack_laser::LASER_TYPE_MASTER_SPARK;

			laser->start();
		}
		else if(this->gm.charID == nn::ts::e::common::CHAR_ID_REIMU) {
			nn::ts::attack::CLaser * laser = new nn::ts::attack::CLaser( this);
			laser->gm.param.x = (int)(this->gm.status.x);
			laser->gm.param.y = (int)(this->gm.status.y);
			laser->gm.param.angle = parther->gm.shotAngle / 1000;
			laser->gm.param.quest = this->lm.quest;
			laser->gm.param.width = 50;
			laser->gm.param.damage = 10800;
			laser->gm.param.reflectionLaserN = 16;
			laser->gm.param.type = nn::ts::e::attack_laser::LASER_TYPE_REFLECTION;	
			laser->gm.param.attribute = nn::ts::e::common::ATTRIBUTE_FIRE;
			laser->gm.param.hitTargetBit = nn::ts::e::common::HIT_TARGET_BIT_ENEMY; 

			laser->start();
		}
		else if(this->gm.charID == nn::ts::e::common::CHAR_ID_SANAE) {
			nn::ts::attack::CExplosion * explosion = new nn::ts::attack::CExplosion( this);
			explosion->gm.param.x = (int)(this->gm.status.x);
			explosion->gm.param.y = (int)(this->gm.status.y);
			explosion->gm.param.damage = 4000;
			explosion->gm.param.r = 100;
			explosion->gm.param.hitTargetBit = nn::ts::e::common::HIT_TARGET_BIT_ENEMY | nn::ts::e::common::HIT_TARGET_BIT_FRIEND;
			explosion->gm.param.ball = parther;
			explosion->gm.param.battle = this->lm.quest->lm.battle;
			explosion->gm.param.quest = this->lm.quest;
			//explosion->gm.param.endCBFunc = NULL;
			//explosion->gm.param.endCBFuncArg = NULL;

			explosion->start();
		}
		else if(this->gm.charID == nn::ts::e::common::CHAR_ID_YOMU) {
			nn::ts::attack::CBullet * bullet = new nn::ts::attack::CBullet( this);
			bullet->gm.param.x = (int)this->gm.status.x;
			bullet->gm.param.y = (int)this->gm.status.y;
			for( int i = 0; i < this->lm.quest->lm.battle->gm.enemyN; i++) {
				
				nn::ts::quest::CEnemy * enemy = this->lm.quest->lm.battle->gm.enemys[i];
				bullet->gm.param.targets[i] = enemy;

			}
			bullet->gm.param.targetN = this->lm.quest->lm.battle->gm.enemyN;
			bullet->gm.param.fireN = 4;
			bullet->gm.param.fireBulletN = 4;
			bullet->gm.param.fireIntervalFrameN = 30;
			bullet->gm.param.damage = 3900;
			bullet->gm.param.quest = this->lm.quest;
			bullet->gm.param.angle[0] = 0.0;
			bullet->gm.param.addAngle[0] = 60.0;
			bullet->gm.param.type = nn::ts::e::attack_bullet::TYPE_HOMING;
			bullet->gm.param.hitTargetBit = nn::ts::e::common::HIT_TARGET_BIT_ENEMY;
			bullet->gm.param.attribute = nn::ts::e::common::ATTRIBUTE_WOOD;

			bullet->start();
		}
		this->gm.friendComboFlag = true;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::hitBall( nn::ts::quest::CBall * parther) {
	this->gm.shotAngle = (int)(nn::common::math::pos2Angle_d( parther->gm.status.x, parther->gm.status.y,
		this->gm.status.x, this->gm.status.y) * 1000.0);
	this->gm.status.speed = 3.0;
	this->gm.decSpeed = 0.1;

	this->friendCombo( parther);

	this->state = nn::ts::e::ball::STATE_MOVE;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::taskMove() {
	
	std::vector<nn::common::util::type::t2d::COLLISION_ITEM> collisions;

	this->lm.quest->lm.battle->addFieldCollision( &collisions);

	this->move( &collisions);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::move( std::vector<nn::common::util::type::t2d::COLLISION_ITEM> * collisions) {
	
	this->gm.status.speed -= this->gm.decSpeed;
	if(this->gm.status.speed <= 0.0) {
		this->gm.status.speed = 0.0;
		this->endShot();
		return;
	}

	double angle = this->gm.shotAngle / 1000.0;
	double speed = this->gm.status.speed;

	double move_x = nn::common::math::angleSpeed2MoveX( angle, speed);
	double move_y = nn::common::math::angleSpeed2MoveY( angle, speed);
	
	bool update_angle_flag = false;
	bool flag = true;

	nn::ts::quest::BALL_TASK_SHOT_MEMBER tsm;
	double x3;
	double y3;
	double x4;
	double y4;

	static int test = 1;

	memset( &tsm, 0, sizeof( tsm));
	tsm.x1 = gm.status.x;
	tsm.y1 = gm.status.y;
	tsm.x2 = gm.status.x + move_x;
	tsm.y2 = gm.status.y + move_y;

	while(flag) {
		if(tsm.x2 == 0.0 || tsm.y2 == 0.0) {
			nn_commonLog( 1, "error\n");
		}
		tsm.cnt++;

		flag = false;
		std::vector<nn::common::util::type::t2d::COLLISION_ITEM>::iterator it;
		for( it = collisions->begin(); it != collisions->end(); it++) {

			tsm.collision = false;
			if(it->shape == nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_UP ||
				it->shape == nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_RIGHT ||
				it->shape == nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_DOWN ||
				it->shape == nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_LEFT
			) {
				tsm.collision = nn::common::math::isCollisionLine2LineEx(
								&x3, &y3, &x4, &y4,
								tsm.x1, tsm.y1, tsm.x2, tsm.y2,
								it->p.x1, it->p.y1, it->p.x2, it->p.y2);
				if(tsm.collision) {
					if(x4 < 0.0 || y4 < 0.0) {
						nn_commonLog( 1, "error\n");
					}
					tsm.afterAngle = this->gm.shotAngle % 180000;
					tsm.afterAngle = -(tsm.afterAngle - 90000);
					tsm.afterAngle = tsm.afterAngle + (this->gm.shotAngle / 180000) * 180000 + 90000;
					if(it->shape == nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_UP ||
						it->shape == nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_DOWN
					) {
						tsm.afterAngle = (tsm.afterAngle + 180000) % 360000;
					}
					tsm.x1 = x3;
					tsm.y1 = y3;
					tsm.x2 = x4;
					tsm.y2 = y4;
					tsm.afterLen = nn::common::math::getLength( tsm.x1, tsm.y1, tsm.x2, tsm.y2);
				}
			}
			else if(it->shape == nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_CIRCLE) {
				if(tsm.cnt == 1 && test == 0) {
#if 1
					it->p.x1 = 1427.0;
					it->p.y1 = 1090.0;
					it->r = 50.0;
					tsm.x1 = 1377.063843;
					tsm.y1 = 1087.473267;
					tsm.x2 = 1382.028198;
					tsm.y2 = 1080.640503;
#endif
#if 0
					it->p.x1 = 1427.0;
					it->p.y1 = 1200.0;
					it->r = 70.0;
					x1 = 1358.504639;
					y1 = 1185.564331;
					x2 = 1363.371216;
					y2 = 1178.518799;
#endif
					test = 0;
				}
				tsm.collision = nn::common::math::isCollisionLine2CircleEX(
								&tsm.collisionX, &tsm.collisionY, &tsm.afterXRate, &tsm.afterYRate, &tsm.afterLen,
								it->p.x1, it->p.y1, it->r + nn::ts::e::common::BALL_R,
								tsm.x1, tsm.y1, tsm.x2, tsm.y2);
					
				if(tsm.collision) {
					if(it->id == nn::ts::e::common::COLLISION_ID_ENEMY) {
						this->hitEnemy( &tsm, &(*it));
					}
					else if(it->id == nn::ts::e::common::COLLISION_ID_BALL) {
						nn::ts::quest::CBall * target = (nn::ts::quest::CBall *)it->target;
						target->hitBall( this);
						tsm.collision = false;
					}
					else if(it->id == nn::ts::e::common::COLLISION_ID_ITEM) {
						nn::ts::quest::CBattleItem * target = (nn::ts::quest::CBattleItem *)it->target;
						target->hitBall( this);
						tsm.collision = false;
					}
				}
			}
			if(tsm.collision) {
				flag = true;
				//update_angle_flag = true;
				tsm.x2 = tsm.x1 + nn::common::math::angleSpeed2MoveX( tsm.afterAngle / 1000.0, tsm.afterLen);
				tsm.y2 = tsm.y1 + nn::common::math::angleSpeed2MoveY( tsm.afterAngle / 1000.0, tsm.afterLen);
				this->gm.shotAngle = tsm.afterAngle;
				
				if(it->shape == nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_CIRCLE) {
					nn_commonLog( 1, "collision[%d]\n", tsm.cnt - 1);
					nn_commonLog( 1, "circle x=%lf y=%lf r=%lf\n", tsm.log.cis[tsm.cnt - 1].p.x1, tsm.log.cis[tsm.cnt - 1].p.y1, tsm.log.cis[tsm.cnt - 1].r);
					nn_commonLog( 1, "ball = x1=%lf y1=%lf x2=%lf y2=%lf\n",
						tsm.log.collisionX1s[tsm.cnt - 1], tsm.log.collisionY1s[tsm.cnt - 1],
						tsm.log.collisionX2s[tsm.cnt - 1], tsm.log.collisionY2s[tsm.cnt - 1]);
					nn_commonLog( 1, "angle = %d\n", tsm.log.angles[tsm.cnt - 1]);
					nn_commonLog( 1, "after_angle=%d\n", tsm.log.afterAngles[tsm.cnt - 1]);
					nn_commonLog( 1, "after_len=%lf\n", tsm.log.afterLens[tsm.cnt - 1]);
					nn_commonLog( 1, "circle2ball=%lf\n", tsm.log.ci2Balls[tsm.cnt - 1]);
				}
				break;
			}
		}

	}
	if(tsm.x2 < 0.0 || tsm.y2 < 0.0) {
		nn_commonLog( 1, "error\n");
	}
	if(update_angle_flag) {
		this->gm.shotAngle = int(nn::common::math::pos2Angle_d( tsm.x1, tsm.y1, tsm.x2, tsm.y2) * 1000.0);
	}
	this->gm.status.x = tsm.x2;
	this->gm.status.y = tsm.y2;

	this->lm.quest->lm.battle->updateBallMoveFrameEnd();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::taskShot() {
	
	nn::ts::quest::CQuest * quest = this->lm.quest;
	nn::ts::quest::CBattle * battle = quest->lm.battle;

	std::vector<nn::common::util::type::t2d::COLLISION_ITEM> collisions;

	battle->addFieldCollision( &collisions);

	battle->addEnemyCollision( &collisions);

	this->addOtherBallCollision( &collisions);

	battle->addItemCollision( &collisions);

	this->move( &collisions);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::shot( int angle) {

	this->gm.shotAngle = (angle + 180000) % 360000;
	this->gm.status.speed = this->gm.param.sp / 1000.0;
#ifdef _DEBUG
	//this->gm.status.speed = 0.0;
#endif
	this->gm.decSpeed = 0.040;
	this->state = nn::ts::e::ball::STATE_SHOT;
	
	nn_commonLog( 1, "this->gm.status.x = %lf;\n", this->gm.status.x);
	nn_commonLog( 1, "this->gm.status.y = %lf;\n", this->gm.status.y);
	nn_commonLog( 1, "this->gm.shotAngle = %d;\n", this->gm.shotAngle);
	nn_commonLog( 1, "this->gm.status.speed = %lf;\n", this->gm.status.speed);
	nn_commonLog( 1, "this->gm.decSpeed = %lf;\n", this->gm.decSpeed);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::endShot() {

	this->state = nn::ts::e::ball::STATE_IDLE;

	if(this->shotEndCBFunc != NULL) {
		this->shotEndCBFunc( this->shotEndCBFuncArg);
		this->shotEndCBFunc = NULL;
		this->shotEndCBFuncArg = NULL;
	}

}
//---------------------------------------------------------------------------
int nn::ts::quest::CBall::getClassId() {
	return nn::ts::e::ts_save_manager::CLASS_ID_BALL;
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::quest::CBall::getDataRecode() {
	return nn::ts::quest::CBall::dataRecode;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CBall::dump() {
		
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CBall::dump() start ----------\n");
	LOG_CLASS_DUMP( "this = 0x%08x\n", this);
	LOG_CLASS_DUMP( "this->state = %d\n", this->state);
	LOG_CLASS_DUMP( "this->oldState = %d\n", this->oldState);
	LOG_CLASS_DUMP( "this->waitCnt = %d\n", this->waitCnt);
	LOG_CLASS_DUMP( "this->frameCnt = %u\n", this->frameCnt);
	LOG_CLASS_DUMP( "this->gm.charID = %d\n", this->gm.charID);
	LOG_CLASS_DUMP( "this->gm.status.x = %lf\n", this->gm.status.x);
	LOG_CLASS_DUMP( "this->gm.status.y = %lf\n", this->gm.status.y);
	LOG_CLASS_DUMP( "this->gm.status.speed = %lf\n", this->gm.status.speed);
	LOG_CLASS_DUMP( "this->gm.param.ap = %d\n", this->gm.param.ap);
	LOG_CLASS_DUMP( "this->gm.param.hp = %d\n", this->gm.param.hp);
	LOG_CLASS_DUMP( "this->gm.param.sp = %d\n", this->gm.param.sp);
	LOG_CLASS_DUMP( "this->gm.shotAngle = %d\n", this->gm.shotAngle);
	LOG_CLASS_DUMP( "this->gm.decSpeed = %lf\n", this->gm.decSpeed);
	LOG_CLASS_DUMP( "this->gm.turn = %d\n", this->gm.turn);
	LOG_CLASS_DUMP( "this->gm.friendComboFlag = %d\n", this->gm.friendComboFlag);
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CBall::dump() end   ----------\n");

}
//---------------------------------------------------------------------------

