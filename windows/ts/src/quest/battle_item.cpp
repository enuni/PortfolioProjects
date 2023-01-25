// battle_item を置換
// BattleItem を置換
// ts を置換
// quest を置換

#include "nn_common.h"
#include "nn_common2.h"
#include "disp.h"

#include "ts_common.h"
#include "ts_save_manager.h"
#include "ball.h"
#include "battle.h"

#include "battle_item.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::quest::CBattleItem::dataRecode[] = {
	DATA_LOCAL( nn::ts::quest::CBattleItem, state),
	DATA_LOCAL( nn::ts::quest::CBattleItem, oldState),
	DATA_LOCAL( nn::ts::quest::CBattleItem, waitCnt),
	DATA_LOCAL( nn::ts::quest::CBattleItem, frameCnt),
	DATA_LOCAL( nn::ts::quest::CBattleItem, m.game),
	DATA_PTR( nn::ts::quest::CBattleItem, m.link.battle),
	DATA_BASE( nn::common::CGameChar::dataRecode),
	DATA_END,
};

nn::ts::quest::BATTLE_ITEM_DISP_MEMBER nn::ts::quest::CBattleItem::dm;
//---------------------------------------------------------------------------
nn::ts::quest::CBattleItem::CBattleItem(TASK_SAVE * oya, nn_common::SaveManager * m) : nn::common::CGameChar( oya, m) {

	::nn_commonLog( 1, "CBattleItem Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CBattleItem::CBattleItem(TASK_SAVE * oya) : nn::common::CGameChar( oya) {

	::nn_commonLog( 1, "CBattleItem Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CBattleItem::CBattleItem() : nn::common::CGameChar() {

	::nn_commonLog( 1, "CBattleItem Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::constructCommon() {
	
	this->state = nn::ts::e::battle_item::STATE_NONE;
	this->oldState = nn::ts::e::battle_item::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->m, 0, sizeof( this->m));
	
}
//---------------------------------------------------------------------------
nn::ts::quest::CBattleItem::~CBattleItem() {

	::nn_commonLog( 1, "CBattleItem Delete\n");

	this->m.link.battle->excludeItem( this);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::start() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::startCommon() {

	this->state = nn::ts::e::battle_item::STATE_ACTIVE;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CBattleItem::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::battle_item::STATE_NONE:
			break;
		case nn::ts::e::battle_item::STATE_ACTIVE:
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::disp() {

	switch(this->state) {
		case nn::ts::e::battle_item::STATE_NONE:
			break;
		case nn::ts::e::battle_item::STATE_ACTIVE:
			int x = (int)this->gameChar.game.hit.x;
			int y = (int)this->gameChar.game.hit.y;
			int r = (int)this->gameChar.game.hit.r;
			nn::dxlib::disp::objCenter(
				this->dm.objIdRecoveryItems[this->m.game.param.id][this->m.game.status.level],
				x, y, nn::ts::e::common::Z_BATTLE_ITEM, true);
#ifdef _DEBUG
			int color = DISP_WHITE;
			if(this->m.game.status.level == 1) {
				color = DISP_BLUE;
			}
			else if(this->m.game.status.level == 2) {
				color = DISP_RED;
			}
			nn::dxlib::disp::circleCenter(
				x, y, 0, r,
				color, DISP_ALPHA_DEFAULT, DISP_BLEND_MODE_NONE, true);
#endif
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::quest::CBattleItem::getClassId() {
	return nn::ts::e::ts_save_manager::CLASS_ID_BATTLE_ITEM;
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::quest::CBattleItem::getDataRecode() {
	return nn::ts::quest::CBattleItem::dataRecode;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::dump() {
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CBattleItem::dump() start ----------\n");
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CBattleItem::dump() end   ----------\n");
}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::hitBall( nn::ts::quest::CBall * ball) {

	if(this->m.game.param.id == nn::ts::e::common::BATTLE_ITEM_ID_RECOVERY) {
		if(this->m.game.status.level == 0) {
			ball->recovery( 20);
		}
		else if(this->m.game.status.level == 1) {
			ball->recovery( 50);
		}
		else if(this->m.game.status.level == 2) {
			ball->recovery( 80);
		}
	}

	this->taskDelete();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleItem::updateTurn() {

	int level = this->m.game.status.level;
	if(this->m.game.status.levelTurn < this->m.game.param.levelUpTurns[level]) {
		this->m.game.status.levelTurn++;
	}

	if(this->m.game.status.levelTurn >= this->m.game.param.levelUpTurns[level]) {
		if(this->m.game.status.level < this->m.game.param.levelMax) {
			this->m.game.status.level++;
			this->m.game.status.levelTurn = 0;
		}
	}

}
//---------------------------------------------------------------------------

