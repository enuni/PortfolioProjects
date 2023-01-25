// battle を置換
// Battle を置換
// ts を置換
// quest を置換

#include "nn_common.h"
#include "nn_common2.h"
#include "direct.h"
#include "anim.h"

#include "ts_common.h"
#include "ts_save_manager.h"

#include "pull.h"
#include "quest.h"
#include "ball.h"
#include "enemy.h"
#include "anime_data.h"
#include "control.h"
#include "quest_control.h"
#include "battle_item.h"

#include "battle.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::quest::CBattle::dataRecode[] = {
	DATA_LOCAL( nn::ts::quest::CBattle, state),
	DATA_LOCAL( nn::ts::quest::CBattle, oldState),
	DATA_LOCAL( nn::ts::quest::CBattle, waitCnt),
	DATA_PTR( nn::ts::quest::CBattle, gm.quest),
	DATA_LOCAL( nn::ts::quest::CBattle, gm.x),
	DATA_LOCAL( nn::ts::quest::CBattle, gm.y),
	DATA_LOCAL( nn::ts::quest::CBattle, gm.w),
	DATA_LOCAL( nn::ts::quest::CBattle, gm.h),
	DATA_PTR( nn::ts::quest::CBattle, gm.enemys),
	DATA_LOCAL( nn::ts::quest::CBattle, gm.enemyN),
	DATA_LOCAL( nn::ts::quest::CBattle, gm.enemyTurnCnt),
	DATA_LOCAL( nn::ts::quest::CBattle, gm.enmeyAttackCnt),
	DATA_LOCAL( nn::ts::quest::CBattle, gm.itemPoints),
	DATA_LOCAL( nn::ts::quest::CBattle, gm.itemPointN),
	DATA_LOCAL( nn::ts::quest::CBattle, gm.itemN),
	DATA_PTR( nn::ts::quest::CBattle, lm.pull),
	DATA_PTR( nn::ts::quest::CBattle, lm.items),
	DATA_BASE(::TASK_SAVE::dataRecode),
	DATA_END,
};
//---------------------------------------------------------------------------
nn::ts::quest::CBattle::CBattle(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CBattle Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CBattle::CBattle(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CBattle Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CBattle::CBattle() : TASK_SAVE() {

	::nn_commonLog( 1, "CBattle Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::constructCommon() {
	
	this->setState( nn::ts::e::battle::STATE_NONE);
	this->oldState = nn::ts::e::battle::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->gm, 0, sizeof( this->gm));
	memset( &this->lm, 0, sizeof( this->lm));
	
	strcpy_s( this->name, sizeof( this->name), "CBattle");
	
}
//---------------------------------------------------------------------------
nn::ts::quest::CBattle::~CBattle() {

	::nn_commonLog( 1, "CBattle Delete\n");

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::start() {
	
	this->startCommon();

	this->updateWeakPoint();

	this->startPull();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::startCommon() {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::addEnemyCollision( void * argCollisions) {

	std::vector<nn::common::util::type::t2d::COLLISION_ITEM> * collisions = (std::vector<nn::common::util::type::t2d::COLLISION_ITEM> *)argCollisions;
	
	for( int i = 0; i < this->gm.enemyN; i++) {

		nn::ts::quest::CEnemy * enemy = this->gm.enemys[i];

		if(enemy->gm.status.hitSkipFlag) {
			continue;
		}

		for ( int j = 0; j < enemy->gm.param.hitN; j++) {

			nn::ts::common::TS_HIT * ts_hit = &enemy->gm.param.hits[j];
			ts_hit->owner = enemy;
			
			nn::common::util::type::t2d::COLLISION_ITEM ci;

			ci.shape = nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_CIRCLE;
			ci.p.x1 = enemy->gameChar.game.hit.x + ts_hit->hit.x;
			ci.p.y1 = enemy->gameChar.game.hit.y + ts_hit->hit.y;
			ci.r = ts_hit->hit.r;
			ci.id = nn::ts::e::common::COLLISION_ID_ENEMY;
			ci.target = ts_hit;

			collisions->push_back( ci);

		}

	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::addFieldCollision( void * argCollisions) {

	std::vector<nn::common::util::type::t2d::COLLISION_ITEM> * collisions = (std::vector<nn::common::util::type::t2d::COLLISION_ITEM> *)argCollisions;

	int tbl[][4] = {
		{ this->gm.x               , this->gm.y               , this->gm.x + this->gm.w, this->gm.y               },
	    { this->gm.x               , this->gm.y               , this->gm.x               , this->gm.y + this->gm.h},
	    { this->gm.x + this->gm.w, this->gm.y               , this->gm.x + this->gm.w, this->gm.y + this->gm.h},
	    { this->gm.x               , this->gm.y + this->gm.h, this->gm.x + this->gm.w, this->gm.y + this->gm.h},
	};
	int shape_tbl[4] = {
		nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_UP,
		nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_LEFT,
		nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_RIGHT,
		nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_LINE_DOWN,
	};

	for( int i = 0; i < 4; i++) {

		nn::common::util::type::t2d::COLLISION_ITEM ci;

		ci.shape = shape_tbl[i];
		ci.p.x1 = tbl[i][0]; ci.p.y1 = tbl[i][1];
		ci.p.x2 = tbl[i][2]; ci.p.y2 = tbl[i][3];
		collisions->push_back( ci);

	}
}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::addItemCollision( void * argCollisions) {

	std::vector<nn::common::util::type::t2d::COLLISION_ITEM> * collisions = (std::vector<nn::common::util::type::t2d::COLLISION_ITEM> *)argCollisions;

	for( int i = 0; i < this->gm.itemN; i++) {

		nn::ts::quest::CBattleItem * item = this->lm.items[i];
			
		nn::common::util::type::t2d::COLLISION_ITEM ci;

		ci.shape = nn::common::util::e::e2d::COLLISION_ITEM_SHAPE_CIRCLE;
		ci.p.x1 = item->gameChar.game.hit.x;
		ci.p.y1 = item->gameChar.game.hit.y;
		ci.r = item->gameChar.game.hit.r;
		ci.id = nn::ts::e::common::COLLISION_ID_ITEM;
		ci.target = item;

		collisions->push_back( ci);

	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CBattle::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::battle::STATE_NONE:
			break;
		case nn::ts::e::battle::STATE_PULL_START:
#ifdef _DEBUG
			this->saveManager->save( nn::ts::common::gCharactersSaveDataDebugQuest,
				true, nn::ts::e::common::CRC_KEY_DEBUG_QUEST);
#endif
			this->setState( nn::ts::e::battle::STATE_PULL);
			break;
		case nn::ts::e::battle::STATE_PULL:
			this->checkNextBattle();
			break;
		case nn::ts::e::battle::STATE_SHOT:
			this->checkNextBattle();
			break;
		case nn::ts::e::battle::STATE_ENEMY:
			break;
		case nn::ts::e::battle::STATE_GAMEOVER:
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::disp() {

	switch(this->state) {
		case nn::ts::e::battle::STATE_NONE:
			break;
		case nn::ts::e::battle::STATE_PULL_START:
			break;
		case nn::ts::e::battle::STATE_PULL:
			break;
		case nn::ts::e::battle::STATE_SHOT:
			break;
		case nn::ts::e::battle::STATE_ENEMY:
			break;
		case nn::ts::e::battle::STATE_GAMEOVER:
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::startPull() {
	
	if(this->lm.ctrl->gm.autoFlag) {
		if(this->gm.quest->gm.status.turn >= this->lm.ctrl->gm.autoEndTurn) {
			this->lm.ctrl->gm.autoFlag = false;
			this->lm.ctrl->gm.dispSkipFlag = false;
		}
		else {
			this->shot( this->lm.ctrl->questCtrl->gm.log.te[this->gm.quest->gm.status.turn].angle);
		}
	}

	if(this->lm.ctrl->gm.autoFlag == false) {
		nn::ts::quest::CPull * pull = new nn::ts::quest::CPull( this, this->saveManager);
		pull->lm.decideCBFuncId = nn::ts::e::common::CB_FUNC_ID_PULL_DECIDE;
		pull->lm.decideCBFuncOwner = this;
		pull->gm.dispX = (int)this->gm.quest->lm.balls[this->gm.quest->gm.status.turn % 4]->gm.status.x;
		pull->gm.dispY = (int)this->gm.quest->lm.balls[this->gm.quest->gm.status.turn % 4]->gm.status.y;
		pull->start();

		pull->state = nn::ts::e::pull::STATE_IDLE;

		this->setState( nn::ts::e::battle::STATE_PULL_START);

		this->lm.pull = pull;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::pullDecideCB( void * argBattle, void * argPull) {

	nn::ts::quest::CBattle * battle = (nn::ts::quest::CBattle *)argBattle;

	battle->pullDecide( argPull);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::pullDecide( void * argPull) {

	nn::ts::quest::CPull * pull = (nn::ts::quest::CPull *)argPull;

	::nn_commonLog( 1, "pull decide angle = %d\n", pull->gm.decideAngle);
	
	nn::ts::quest::CQuest * quest = this->gm.quest;
	quest->addTe( pull->gm.decideAngle, false);

	this->shot( pull->gm.decideAngle);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::shot( int angle) {

	nn::ts::quest::CQuest * quest = this->gm.quest;
	nn::ts::quest::CBall * ball = quest->lm.balls[quest->gm.status.turn % 4];

	ball->shotEndCBFunc = nn::ts::quest::CBattle::shotEndCB;
	ball->shotEndCBFuncArg = this;
	ball->shot( angle);

	this->state = nn::ts::e::battle::STATE_SHOT;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::nextTurnEnemyCB( void * argBattle) {

	nn::ts::quest::CBattle * battle = (nn::ts::quest::CBattle *)argBattle;

	battle->nextTurnEnemy();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::endAttackEnemyCB( void * argBattle) {

	nn::ts::quest::CBattle * battle = (nn::ts::quest::CBattle *)argBattle;

	battle->endAttackEnemy();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::endAttackEnemy() {
	
	nn::ts::quest::CQuest * quest = this->gm.quest;

	this->gm.enmeyAttackCnt--;
	if(this->gm.enmeyAttackCnt == 0 && this->gm.enemyTurnCnt > this->gm.enemyN) {
		if(this->gm.quest->gm.status.damageHp == 0) {
			nn::dxlib::CAnim * anime = nn::dxlib::CAnim::exec( this,
				::ANIMATION_ID_LOSE, 854 / 2, 480 / 2, nn::ts::e::common::Z_BATTLE_TEXT,
				ANIM_FLAGS_LAST_WAIT, NULL, NULL);
			this->state = nn::ts::e::battle::STATE_GAMEOVER;
			if(this->lm.ctrl->gm.autoFlag) {
				this->lm.ctrl->gm.autoFlag = false;
				this->lm.ctrl->gm.dispSkipFlag = false;
			}
		}
		else {	
			this->gm.quest->syncDisplayHP();
			nn::dxlib::CAnim * anime = nn::dxlib::CAnim::exec( this,
				::ANIMATION_ID_NEXT_TURN, 854 / 2, 480 / 2, nn::ts::e::common::Z_BATTLE_TEXT,
				0, nn::ts::quest::CBattle::nextTurnCB, this);
		}
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::nextTurnEnemy() {
	
	nn::ts::quest::CQuest * quest = this->gm.quest;

	if(this->gm.enemyTurnCnt < this->gm.enemyN) {
		if(this->gm.enemys[this->gm.enemyTurnCnt]->gameChar.game.enableFlag) {
			int attack_cnt = this->gm.enemys[this->gm.enemyTurnCnt]->turn();
			this->gm.enmeyAttackCnt += attack_cnt;
			nn::dxlib::direct::waitSimple( this, 20, nn::ts::quest::CBattle::nextTurnEnemyCB, this);
		}
		else {
			nn::dxlib::direct::waitSimple( this, 1, nn::ts::quest::CBattle::nextTurnEnemyCB, this);
		}
	}
	else {
		if(this->gm.enmeyAttackCnt == 0) {
			this->gm.quest->syncDisplayHP();
			nn::dxlib::CAnim * anime = nn::dxlib::CAnim::exec( this,
				::ANIMATION_ID_NEXT_TURN, 854 / 2, 480 / 2, nn::ts::e::common::Z_BATTLE_TEXT,
				0, nn::ts::quest::CBattle::nextTurnCB, this);

			int next_turn_char_index = (quest->gm.status.turn + 1) % 4;
			anime->changeObjId( ANIM_CHANGE_OBJ_ID_TYPE_OBJ_ID,
					quest->dm.objIdCharFulls[this->gm.quest->lm.balls[0]->gm.charID],
					quest->dm.objIdCharFulls[this->gm.quest->lm.balls[next_turn_char_index]->gm.charID]);
		}
	}
	this->gm.enemyTurnCnt++;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::nextTurnCB( void * argBattle) {

	nn::ts::quest::CBattle * battle = (nn::ts::quest::CBattle *)argBattle;

	battle->nextTurn();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::nextTurn() {
	
	nn::ts::quest::CQuest * quest = this->gm.quest;
	
	this->gm.enemyTurnCnt = 0;
	this->gm.quest->deleteDamageDispBall();
	this->updateWeakPoint();
	this->updateTurnItemPhase();
	this->createItemPhase();
	quest->gm.status.turn++;
	this->startPull();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::shotEndCB( void * argBattle) {

	nn::ts::quest::CBattle * battle = (nn::ts::quest::CBattle *)argBattle;

	battle->shotEnd();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::shotEnd() {
	
	nn::ts::quest::CQuest * quest = this->gm.quest;

	for( int i = 0; i < this->gm.enemyN; i++) {

		this->gm.enemys[i]->shotEnd();

	}
	for( int i = 0; i < nn::ts::e::common::QUEST_BALL_N; i++) {

		quest->lm.balls[i]->gm.friendComboFlag = false;

	}

	if(this->isEnemyAnnihilation()) {
	}
	else {
		nn::dxlib::direct::waitSimple( this, 20, nn::ts::quest::CBattle::nextTurnEnemyCB, this);
	}

}
//---------------------------------------------------------------------------
int nn::ts::quest::CBattle::getClassId() {
	return nn::ts::e::ts_save_manager::CLASS_ID_BATTLE;
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::quest::CBattle::getDataRecode() {
	return nn::ts::quest::CBattle::dataRecode;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::updateBallMoveFrameEnd() {

	for( int i = 0; i < this->gm.enemyN; i++) {

		nn::ts::quest::CEnemy * enemy = this->gm.enemys[i];
		enemy->updateIfDead();

	}

}
//---------------------------------------------------------------------------
bool nn::ts::quest::CBattle::isEnemyAnnihilation() {

	for( int i = 0; i < this->gm.enemyN; i++) {

		nn::ts::quest::CEnemy * enemy = this->gm.enemys[i];
		if(enemy->gameChar.game.enableFlag) {
			return false;
		}

	}

	return true;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::setState( int state) {

	this->state = state;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::updateWeakPoint() {

	for( int i = 0; i < this->gm.enemyN; i++) {

		nn::ts::quest::CEnemy * enmey = this->gm.enemys[i];
		enmey->updateWeakPoint();

	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::dump() {
	
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CBattle::dump() start ----------\n");
	LOG_CLASS_DUMP( "this = 0x%08x\n", this);
	LOG_CLASS_DUMP( "state = %d\n", this->state);
	LOG_CLASS_DUMP( "oldState = %d\n", this->oldState);
	LOG_CLASS_DUMP( "waitCnt = %d\n", this-waitCnt);
	LOG_CLASS_DUMP( "frameCnt = %u\n", this->frameCnt);
	LOG_CLASS_DUMP( "gm.x = %d\n", this->gm.x);
	LOG_CLASS_DUMP( "gm.y = %d\n", this->gm.y);
	LOG_CLASS_DUMP( "gm.w = %d\n", this->gm.w);
	LOG_CLASS_DUMP( "gm.h = %d\n", this->gm.h);
	LOG_CLASS_DUMP( "gm.enemyN = %d\n", this->gm.enemyN);
	for( int i = 0; i < this->gm.enemyN; i++) {
	this->gm.enemys[i]->dump();
	}
	LOG_CLASS_DUMP( "gm.enemyTurnCnt = %d\n", this->gm.enemyTurnCnt);
	LOG_CLASS_DUMP( "gm.enmeyAttackCnt = %d\n", this->gm.enmeyAttackCnt);
	LOG_CLASS_DUMP( "lm.pull = 0x%08x\n", this->lm.pull);
	if(this->lm.pull != NULL) {
	this->lm.pull->dump();
	}
	LOG_CLASS_DUMP( "this->parent = 0x%08x\n", this->parent);
	LOG_CLASS_DUMP( "this->childTop = 0x%08x\n", this->childTop);
	LOG_CLASS_DUMP( "this->childBottom = 0x%08x\n", this->childBottom);
	LOG_CLASS_DUMP( "this->brotherUp = 0x%08x\n", this->brotherUp);
	LOG_CLASS_DUMP( "this->brotherDown = 0x%08x\n", this->brotherDown);
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CBattle::dump() end   ----------\n");

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::checkNextBattle() {

	bool go_to_next_flag = true;

	for( int i = 0; i < 4; i++) {

		if(this->gm.quest->lm.balls[i]->state == nn::ts::e::ball::STATE_SHOT) {
			go_to_next_flag = false;
		}

	}

	for( int i = 0; i < this->gm.enemyN; i++) {

		if(this->gm.enemys[i]->state != nn::ts::e::enemy::STATE_NONE) {
			go_to_next_flag = false;
		}

	}

	if(this->isEnemyAnnihilation() == false) {
		go_to_next_flag = false;
		if(this->gm.quest->lm.boss->gm.status.damageHp == 0 &&
			this->gm.quest->lm.boss->gm.status.gaugeCnt >= this->gm.quest->lm.boss->gm.param.gaugeN
		) {
			go_to_next_flag = true;
		}
	}

	if(go_to_next_flag) {
		if(this->gm.quest->lm.boss->gm.status.gaugeCnt >= this->gm.quest->lm.boss->gm.param.gaugeN) {
			nn::dxlib::CAnim * anime = nn::dxlib::CAnim::exec( this,
				::ANIMATION_ID_WIN, 854 / 2, 480 / 2, nn::ts::e::common::Z_BATTLE_TEXT,
				ANIM_FLAGS_LAST_WAIT, NULL, NULL);
			if(this->lm.ctrl->gm.autoFlag) {
				this->lm.ctrl->gm.autoFlag = false;
				this->lm.ctrl->gm.dispSkipFlag = false;
			}
			this->state = nn::ts::e::battle::STATE_GAMEOVER;
		}
		else {
			this->gm.quest->nextBattle();
		}
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::createItemPhase() {

	int item_create_percent[] = { 50, 50};

	int item_create = n_commonRandFork( item_create_percent, 2);

	if(item_create) {
		nn::ts::quest::CBattleItem * item = new nn::ts::quest::CBattleItem( this, this->saveManager);

		int point_index = n_commonRandN( this->gm.itemPointN);
		item->gameChar.game.hit.x = this->gm.itemPoints[point_index].x + nn::ts::e::common::BATTLE_FIELD_X;
		item->gameChar.game.hit.y = this->gm.itemPoints[point_index].y + nn::ts::e::common::BATTLE_FIELD_Y;
		item->gameChar.game.hit.r = 25;
		item->m.game.param.id = nn::ts::e::common::BATTLE_ITEM_ID_RECOVERY;
		item->m.game.param.levelMax = 2;
		item->m.game.param.levelUpTurns[0] = 2;
		item->m.game.param.levelUpTurns[1] = 2;
		item->m.link.battle = this;
		item->start();

		this->lm.items[this->gm.itemN] = item;
		this->gm.itemN++;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::updateTurnItemPhase() {

	for( int i = 0;i < this->gm.itemN; i++) {

		this->lm.items[i]->updateTurn();

	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattle::excludeItem( nn::ts::quest::CBattleItem * item) {

	int i;
	for( i = 0; i < this->gm.itemN; i++) {

		if(this->lm.items[i] == item) {
			break;
		}

	}
	for( ; i < this->gm.itemN - 1; i++) {

		this->lm.items[i] = this->lm.items[i + 1];

	}
	this->gm.itemN--;

}
//---------------------------------------------------------------------------

