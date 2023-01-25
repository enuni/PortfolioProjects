// bullet を置換
// Bullet を置換
// ts を置換
// effect を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "disp.h"

#include "ts_common.h"

#include "effect_bullet.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::effect::CBullet::dataRecode[] = {
	DATA_END,
};

nn::ts::effect::BULLET_DISP_MEMBER nn::ts::effect::CBullet::dm;
//---------------------------------------------------------------------------
nn::ts::effect::CBullet::CBullet(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CBullet Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::effect::CBullet::CBullet(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CBullet Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::effect::CBullet::CBullet() : TASK_SAVE() {

	::nn_commonLog( 1, "CBullet Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::effect::CBullet::constructCommon() {
	
	this->state = nn::ts::e::effect_bullet::STATE_NONE;
	this->oldState = nn::ts::e::effect_bullet::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->gm, 0, sizeof( this->gm));
	
}
//---------------------------------------------------------------------------
nn::ts::effect::CBullet::~CBullet() {

	::nn_commonLog( 1, "CBullet Delete\n");

	if(this->gm.param.endCBFunc != NULL) {
		this->gm.param.endCBFunc( this->gm.param.endCBFuncArg);
	}

}
//---------------------------------------------------------------------------
void nn::ts::effect::CBullet::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::effect::CBullet::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::effect::CBullet::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::effect::CBullet::start() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::effect::CBullet::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::effect::CBullet::startCommon() {

	this->gm.x = this->gm.param.x;
	this->gm.y = this->gm.param.y;
	this->gm.angle = this->gm.param.angle;
	this->gm.speed = this->gm.param.speed;
	this->gm.angleSpeed = this->gm.param.angleSpeed;

	this->state = nn::ts::e::effect_bullet::STATE_ACTIVE;

}
//---------------------------------------------------------------------------
void nn::ts::effect::CBullet::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CBullet::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::effect_bullet::STATE_NONE:
			break;
		case nn::ts::e::effect_bullet::STATE_ACTIVE:
			double x2 = this->gm.x + nn::common::math::angleSpeed2MoveX( this->gm.angle, this->gm.speed);
			double y2 = this->gm.y + nn::common::math::angleSpeed2MoveY( this->gm.angle, this->gm.speed);

			this->gm.speed += this->gm.param.speedAdd;
			this->gm.angleSpeed += this->gm.param.angleSpeedAdd;

			if(this->gm.param.moveCBFunc != NULL) {
				this->gm.param.moveCBFunc( this->gm.param.moveCBuncArg, this, this->gm.x, this->gm.y, x2, y2);
			}

			if(this->gm.param.hitCBFunc != NULL) {
				bool hit = nn::common::math::isCollisionLine2Circle(
					this->gm.param.target->gameChar.game.hit.x,
					this->gm.param.target->gameChar.game.hit.y,
					1.0,
					this->gm.x, this->gm.y, x2, y2);
				if(hit) {
					this->gm.param.hitCBFunc( this->gm.param.hitCBFuncArg, this);
				}
			}
			this->gm.x = x2;
			this->gm.y = y2;
			
			if(this->gm.param.type == nn::ts::e::effect_bullet::TYPE_HOMING &&
				this->gm.param.target != NULL
			) {
				nn::common::math::CVector3_d goal =
					nn::common::math::CVector3_d( this->gm.x, this->gm.y,
					this->gm.param.target->gameChar.game.hit.x,
					this->gm.param.target->gameChar.game.hit.y);
				int left_right = nn::common::math::getLeftRight( (int)this->gm.angle, goal);
				if(left_right == nn::common::math::e::LEFT_RIGHT_LEFT) {
					this->gm.angle = nn::common::math::varAngle( this->gm.angle, +this->gm.angleSpeed);
				}
				else if(left_right == nn::common::math::e::LEFT_RIGHT_RIGHT) {
					this->gm.angle = nn::common::math::varAngle( this->gm.angle, -this->gm.angleSpeed);
				}
				if(this->frameCnt >= 300) {
					this->gm.angleSpeed += 0.02;
				}
			}

			if(nn::common::graphics2d::isArea( (int)this->gm.x, (int)this->gm.y, &this->gm.param.liveRect) == false) {
				this->taskDelete();
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::effect::CBullet::disp() {

	switch(this->state) {
		case nn::ts::e::effect_bullet::STATE_NONE:
			break;
		case nn::ts::e::effect_bullet::STATE_ACTIVE:
			if(this->gm.param.type == nn::ts::e::effect_bullet::TYPE_HOMING) {
				if(this->gm.param.attribute == nn::ts::e::common::ATTRIBUTE_WOOD) {
					nn::dxlib::disp::objCenterFull( this->dm.objIdHoming[this->gm.param.attribute],
						(int)this->gm.x, (int)this->gm.y, nn::ts::e::common::Z_BATTLE_EFFECT, 48, 24,
						359 - (int)this->gm.angle, FALSE, FALSE,
						DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_ADD);
					nn::dxlib::disp::objCenterFull( this->dm.objIdHoming[this->gm.param.attribute],
						(int)this->gm.x, (int)this->gm.y, nn::ts::e::common::Z_BATTLE_EFFECT, 48, 24,
						359 - (int)this->gm.angle, FALSE, FALSE,
						DISP_WHITE, 255, DISP_BLEND_MODE_ADD);
				}
				else {
					nn::dxlib::disp::objCenterFull( this->dm.objIdHoming[this->gm.param.attribute],
						(int)this->gm.x, (int)this->gm.y, nn::ts::e::common::Z_BATTLE_EFFECT, 48, 24,
						359 - (int)this->gm.angle, FALSE, FALSE,
						DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_ADD);
				}
			}
			else if(this->gm.param.type == nn::ts::e::effect_bullet::TYPE_STRAIGHT) {
				nn::dxlib::disp::objCenterFull( this->dm.objIdNormal[this->gm.param.attribute],
					(int)this->gm.x, (int)this->gm.y, nn::ts::e::common::Z_BATTLE_EFFECT, 28, 28,
					0, FALSE, FALSE,
					DISP_WHITE, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_ADD);
			}
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::effect::CBullet::getClassId() {
	return -1;		// ToDo
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::effect::CBullet::getDataRecode() {
	return nn::ts::effect::CBullet::dataRecode;
}
//---------------------------------------------------------------------------

