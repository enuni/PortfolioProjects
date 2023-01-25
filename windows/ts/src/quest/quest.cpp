// quest を置換
// Quest を置換
// ts を置換
// quest を置換

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/iostreams/stream.hpp>
#include <string>
#include <map>

#include "nn_common.h"
#include "nn_common2.h"
#include "fs.h"

#include "ts_common.h"
#include "ts_save_manager.h"

#include "disp.h"
#include "anim.h"
#include "script.h"

#include "ball.h"
#include "battle.h"
#include "enemy.h"
#include "battle_bg.h"
#include "control.h"
#include "quest_control.h"

#include "quest.h"

extern nn::dxlib::CScript * script;
extern nn::dxlib::script::GlobalCommonVar * common_var;
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::quest::CQuest::dataRecode[] = {
	DATA_PTR( nn::ts::quest::CQuest, lm.battle),
	DATA_PTR( nn::ts::quest::CQuest, lm.balls),
	DATA_LOCAL( nn::ts::quest::CQuest, gm.status),
	DATA_PTR( nn::ts::quest::CQuest, lm.boss),
	DATA_PTR( nn::ts::quest::CQuest, lm.battleBg),
	DATA_PTR( nn::ts::quest::CQuest, lm.animeFrame),
	DATA_LOCAL( nn::ts::quest::CQuest, state),
	DATA_LOCAL( nn::ts::quest::CQuest, oldState),
	DATA_LOCAL( nn::ts::quest::CQuest, waitCnt),
	DATA_LOCAL( nn::ts::quest::CQuest, frameCnt),
	DATA_LOCAL( nn::ts::quest::CQuest, cm),
	DATA_PTR( nn::ts::quest::CQuest, lm.questCtrl),
	DATA_BASE(::TASK_SAVE::dataRecode),
	DATA_END,
};
nn::ts::quest::QUEST_DISP_MEMBER nn::ts::quest::CQuest::dm;
//---------------------------------------------------------------------------
nn::ts::quest::CQuest::CQuest(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CQuest Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CQuest::CQuest(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CQuest Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CQuest::CQuest() : TASK_SAVE() {

	::nn_commonLog( 1, "CQuest Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::constructCommon() {
	
	this->state = nn::ts::e::quest::STATE_NONE;
	this->oldState = nn::ts::e::quest::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->gm, 0, sizeof(this->gm));
	memset( &this->cm, 0, sizeof(this->cm));
	memset( &this->lm, 0, sizeof(this->lm));

	strcpy_s( this->name, sizeof( this->name), "CQuest");
	
}
//---------------------------------------------------------------------------
nn::ts::quest::CQuest::~CQuest() {

	::nn_commonLog( 1, "CQuest Delete\n");

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::start() {
	
	this->startCommon();

	int char_id_tbl[4] = {
		nn::ts::e::common::CHAR_ID_REIMU,
		nn::ts::e::common::CHAR_ID_MARISA,
		nn::ts::e::common::CHAR_ID_SANAE,
		nn::ts::e::common::CHAR_ID_YOMU,
	};
	
	LAYOUT * layout_filed = nn::dxlib::disp::getLayout( this->dm.layoutIdField);
	for( int i = 0; i < 4; i++) {
		 
		nn::ts::quest::CBall * ball = new nn::ts::quest::CBall( this, this->saveManager);
		ball->gm.status.x = 1000 + layout_filed->x + layout_filed->w / 5 * (1 + i);
		ball->gm.status.y = 1400;
		ball->gm.param.sp = 20000;
		ball->gm.param.ap = 27480;
		ball->gm.param.hp = 23400;
		ball->lm.quest = this;
		ball->gm.charID = char_id_tbl[i];
		ball->gm.turn = i;
		ball->dispDirect->m.user.animeId = ball->dm.animeIdBallIn;
		ball->dispDirect->m.user.layer = 0;
		ball->start();
		this->lm.balls[i] = ball;

		this->gm.status.maxHp += ball->gm.param.hp;

	}
	this->gm.status.hp = this->gm.status.maxHp;
	this->gm.status.damageHp = this->gm.status.maxHp;

	this->setupBoss();

	this->setupBattle();

	this->setupBg();

	this->lm.balls[0]->state = nn::ts::e::ball::STATE_DIRECT;

	nn::dxlib::direct::waitSimple( this, 10, nn::ts::quest::CQuest::nextMoveCB, this);

	this->state = nn::ts::e::quest::STATE_MOVE;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::startCommon() {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::addBallCollision( void * argCollisions) {

	std::vector<nn::common::util::type::t2d::COLLISION_ITEM> * collisions = (std::vector<nn::common::util::type::t2d::COLLISION_ITEM> *)argCollisions;
	
	for( int i = 0; i < nn::ts::e::common::QUEST_BALL_N; i++) {

		nn::ts::quest::CBall * ball = this->lm.balls[i];
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
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::deleteDamageDispBall() {

	for( int i = 0; i < nn::ts::e::common::QUEST_BALL_N; i++) {

		this->lm.balls[i]->deleteDamageDisp();

	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::getNearBallPos( double * nearX, double * nearY,
	double x, double y) {

	nn::ts::quest::CBall * min_ball = this->lm.balls[0];
	double min_len = nn::common::math::getLength( x, y, min_ball->gm.status.x, min_ball->gm.status.y);

	for( int i = 1; i < nn::ts::e::common::QUEST_BALL_N; i++) {

		nn::ts::quest::CBall * ball = this->lm.balls[i];
		double len = nn::common::math::getLength( x, y, ball->gm.status.x, ball->gm.status.y);
		if(len < min_len) {
			min_len = len;
			min_ball = ball;
		}

	}

	*nearX = min_ball->gm.status.x;
	*nearY = min_ball->gm.status.y;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::taskMove() {

	switch(this->cm.moveState) {
	case nn::ts::e::quest::MOVE_STATE_BG:
		break;
	case nn::ts::e::quest::MOVE_STATE_IN_ENEMY:
		break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CQuest::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::quest::STATE_NONE:
			break;
		case nn::ts::e::quest::STATE_MOVE:
			this->taskMove();
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::disp() {

	switch(this->state) {
		case nn::ts::e::quest::STATE_NONE:
		case nn::ts::e::quest::STATE_MOVE:
			break;
		case nn::ts::e::quest::STATE_BATTLE:
			{
				// フィールド
				//nn::dxlib::disp::objSimple( this->dm.layoutIdFieldBg, false);
				//for( int i = 0; i < 4; i++) {
		
					//nn::dxlib::disp::boardSimple( this->dm.layoutIdFieldOuts[i],
					//	DISP_BLACK, DISP_BLEND_MODE_NONE, false);

				//}

				// キャラアップ
				LAYOUT * layout_char_up_offset = nn::dxlib::disp::getLayout( this->dm.layoutIdCharUpOffset);

				for( int i = 0; i < nn::ts::e::common::QUEST_BALL_N; i++) {
				
					if(i == this->gm.status.turn % 4) {
						nn::dxlib::disp::objShiftChangeObj(
							this->dm.objIdCharUps[this->lm.balls[i]->gm.charID],
							this->dm.layoutIdCharUpTop,
							layout_char_up_offset->w, layout_char_up_offset->h * i, 0,  false);
					}
					else {
						nn::dxlib::disp::objShiftChangeObj(
							this->dm.objIdCharUps[this->lm.balls[i]->gm.charID],
							this->dm.layoutIdCharUpTop,
							0, layout_char_up_offset->h * i, 0,  false);
					}

				}

				int real_hp_w;
				int damage_hp_w;

				// ボスHP
				nn::ts::quest::CEnemy * boss =  this->lm.boss;
				if(boss->gm.status.gaugeCnt < boss->gm.param.gaugeN) {
					nn::dxlib::disp::objSimple( this->dm.layoutIdBoss, false);
					nn::dxlib::disp::objSimple( this->dm.layoutIdHpBarBosFrame, false);

					LAYOUT * layout_hp_bar_boss = nn::dxlib::disp::getLayout( this->dm.layoutIdHpBarBoss);
					// リアルHP
					real_hp_w = layout_hp_bar_boss->w * boss->gm.status.hp / boss->gm.param.maxHP[boss->gm.status.gaugeCnt];
					if(real_hp_w == 0 && boss->gm.status.hp >= 1) {
						real_hp_w = 1;
					}
					nn::dxlib::disp::boardFull(
						layout_hp_bar_boss->x, layout_hp_bar_boss->y, layout_hp_bar_boss->z - 1,
						real_hp_w, layout_hp_bar_boss->h, layout_hp_bar_boss->rotate,
						DISP_RED, layout_hp_bar_boss->alpha,
						DISP_BLEND_MODE_NONE, false);
					// ダメージHP
					damage_hp_w = layout_hp_bar_boss->w * boss->gm.status.damageHp / boss->gm.param.maxHP[boss->gm.status.gaugeCnt];
					if(damage_hp_w == 0 && boss->gm.status.damageHp >= 1) {
						damage_hp_w = 1;
					}
					nn::dxlib::disp::boardFull(
						layout_hp_bar_boss->x, layout_hp_bar_boss->y, layout_hp_bar_boss->z - 2,
						damage_hp_w, layout_hp_bar_boss->h, layout_hp_bar_boss->rotate,
						DISP_GET_COLOR( 255, 238, 92), layout_hp_bar_boss->alpha,
						DISP_BLEND_MODE_NONE, false);
				}

				// HP
				LAYOUT * layout_hp_bar = nn::dxlib::disp::getLayout( this->dm.layoutIdHpBar);
				nn::dxlib::disp::objSimple( this->dm.layoutIdHp, false);
				nn::dxlib::disp::objSimple( this->dm.layoutIdHpBarFrame, false);
				// リアルHP
				real_hp_w = layout_hp_bar->w * this->gm.status.hp / this->gm.status.maxHp;
				if(real_hp_w == 0 && this->gm.status.hp >= 1) {
					real_hp_w = 1;
				}
				nn::dxlib::disp::boardFull(
					layout_hp_bar->x, layout_hp_bar->y, layout_hp_bar->z - 1,
					real_hp_w, layout_hp_bar->h, layout_hp_bar->rotate,
					DISP_RED, layout_hp_bar->alpha,
					DISP_BLEND_MODE_NONE, false);
				// ダメージHP
				damage_hp_w = layout_hp_bar->w * this->gm.status.damageHp / this->gm.status.maxHp;
				if(damage_hp_w == 0 && this->gm.status.damageHp >= 1) {
					damage_hp_w = 1;
				}
				int color = DISP_GET_COLOR( 150, 255, 50);
				int damge_hp_rate = this->gm.status.damageHp * 100 / this->gm.status.maxHp;
				if(damge_hp_rate <= 25) {
					color = DISP_GET_COLOR( 255, 238, 92);
				}
				else if(damge_hp_rate <= 50) {
					color = DISP_GET_COLOR( 255, 150, 50);
				}
				nn::dxlib::disp::boardFull(
					layout_hp_bar->x, layout_hp_bar->y, layout_hp_bar->z - 2,
					damage_hp_w, layout_hp_bar->h, layout_hp_bar->rotate,
					color, layout_hp_bar->alpha,
					DISP_BLEND_MODE_NONE, false);

				// Stage
				nn::dxlib::disp::objSimple( this->dm.layoutIdStage, false);
				nn::dxlib::disp::objNumberSimple( 1, 0, this->dm.layoutIdStageNum, this->dm.layoutIdStageNum, false);
				
			}
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::quest::CQuest::getClassId() {
	return nn::ts::e::ts_save_manager::CLASS_ID_QUEST;
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::quest::CQuest::getDataRecode() {
	return nn::ts::quest::CQuest::dataRecode;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::nextMoveCB( void * argQuest) {

	nn::ts::quest::CQuest * quest = (nn::ts::quest::CQuest *)argQuest;

	quest->nextMove();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::nextMove() {

	if(this->state == nn::ts::e::quest::STATE_MOVE) {
		if(this->cm.moveState == nn::ts::e::quest::MOVE_IN_BALL_0) {
			this->lm.balls[1]->state = nn::ts::e::ball::STATE_DIRECT;
			nn::dxlib::direct::waitSimple( this, 10, nn::ts::quest::CQuest::nextMoveCB, this);

			this->cm.moveState = nn::ts::e::quest::MOVE_IN_BALL_1;
		}
		else if(this->cm.moveState == nn::ts::e::quest::MOVE_IN_BALL_1) {
			this->lm.balls[2]->state = nn::ts::e::ball::STATE_DIRECT;
			nn::dxlib::direct::waitSimple( this, 10, nn::ts::quest::CQuest::nextMoveCB, this);

			this->cm.moveState = nn::ts::e::quest::MOVE_IN_BALL_2;
		}
		else if(this->cm.moveState == nn::ts::e::quest::MOVE_IN_BALL_2) {
			this->lm.balls[3]->state = nn::ts::e::ball::STATE_DIRECT;
			this->lm.balls[3]->dispDirect->m.user.directEndCBFunc = nn::ts::quest::CQuest::nextMoveCB;
			this->lm.balls[3]->dispDirect->m.user.directEndCBFuncArg = this;

			this->cm.moveState = nn::ts::e::quest::MOVE_IN_BALL_3;
		}
		else if(this->cm.moveState == nn::ts::e::quest::MOVE_IN_BALL_3) {
#ifdef _DEBUG
			this->lm.battleBg->startDirect();

			this->cm.moveState = nn::ts::e::quest::MOVE_STATE_BG;
#else
			script->startScript( "battle_1_1_1.teco", 0, common_var);
			script->scriptEndCbFunc = nn::ts::quest::CQuest::nextMoveCB;
			script->scriptEndCbFuncArg = this;

			this->cm.moveState = nn::ts::e::quest::MOVE_TALK;
#endif
		}
		else if(this->cm.moveState == nn::ts::e::quest::MOVE_TALK) {
			this->lm.battleBg->startDirect();

			this->cm.moveState = nn::ts::e::quest::MOVE_STATE_BG;
		}
		else if(this->cm.moveState == nn::ts::e::quest::MOVE_STATE_BG) {
			nn::ts::quest::CBattle * battle = this->lm.battle;
			for( int i = 0; i < battle->gm.enemyN; i++) {
				
				nn::ts::quest::CEnemy * enemy = battle->gm.enemys[i];
				enemy->state = nn::ts::e::enemy::STATE_DIRECT;
				if(i == battle->gm.enemyN - 1) {
					enemy->lm.endDirectCBFuncId = nn::ts::e::common::CB_FUNC_ID_NEXT_MOVE_QUEST;
					enemy->lm.endDirectCBFuncOwner = this;
				}

			}
			this->cm.moveState = nn::ts::e::quest::MOVE_STATE_IN_ENEMY;
		}
		else if(this->cm.moveState == nn::ts::e::quest::MOVE_STATE_IN_ENEMY) {
			this->lm.animeFrame = nn::dxlib::CAnim::execSave(
				this, this->saveManager,
				this->dm.animeIdBattleFrameIn, 427, 240,
				nn::ts::e::common::Z_BATTLE_BATTLE_FRAME,
				DISP_ALPHA_DEFAULT,
				::ANIM_FLAGS_LAST_WAIT, nn::ts::quest::CQuest::nextMoveCB, this);
			this->cm.moveState = nn::ts::e::quest::MOVE_STATE_IN_FRAME;
		}
		else if(this->cm.moveState == nn::ts::e::quest::MOVE_STATE_IN_FRAME) {
			if(this->gm.status.battleCnt >= 2) {
				this->lm.battle->start();
				this->state = nn::ts::e::quest::STATE_BATTLE;

				this->recoveryHPNextBattle();
			}
			else {
				nn::dxlib::CAnim::execSave( this, this->saveManager,
					this->dm.animeIdStageStart, 427, 240,
					nn::ts::e::common::Z_BATTLE_TEXT,
					DISP_ALPHA_DEFAULT,
					0, nn::ts::quest::CQuest::nextMoveCB, this);
				this->cm.moveState = nn::ts::e::quest::MOVE_STATE_TEXT;
			}
		}
		else if(this->cm.moveState == nn::ts::e::quest::MOVE_STATE_TEXT) {
			if(this->gm.status.battleCnt >= 2 || this->lm.ctrl->gm.dispSkipFlag) {
				this->lm.battle->start();
				this->state = nn::ts::e::quest::STATE_BATTLE;
			}
			else {
				script->startScript( "tutorial_1.teco", 0, common_var);
				script->scriptEndCbFunc = nn::ts::quest::CQuest::nextMoveCB;
				script->scriptEndCbFuncArg = this;

				this->cm.moveState = nn::ts::e::quest::MOVE_STATE_TUTORIAL;
			}
		}
		else if(this->cm.moveState == nn::ts::e::quest::MOVE_STATE_TUTORIAL) {
			this->lm.battle->start();
			this->state = nn::ts::e::quest::STATE_BATTLE;
		}
	}
	else {
		this->state = nn::ts::e::quest::STATE_MOVE;
		this->cm.moveState = nn::ts::e::quest::MOVE_STATE_BG;

		this->lm.battleBg->startDirect();
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::setupBg() {

	nn::ts::quest::CBattleBg * battle_bg = new nn::ts::quest::CBattleBg( this, this->saveManager);
	battle_bg->m.lm.directEndCBFunc = nn::ts::quest::CQuest::nextMoveCB;
	battle_bg->m.lm.directEndCBFuncArg = this;

	battle_bg->m.cm.initScene.fogStart = 0.0f;
	battle_bg->m.cm.initScene.fogEnd = 0.0f;
	battle_bg->m.cm.initScene.cameraPositionX = 0.0f;
	battle_bg->m.cm.initScene.cameraPositionY = 100.0f;
	battle_bg->m.cm.initScene.cameraPositionZ = -100.0f;
	battle_bg->m.cm.initScene.cameraTargetX = 0.0f;
	battle_bg->m.cm.initScene.cameraTargetY = 0.0f;
	battle_bg->m.cm.initScene.cameraTargetZ = 0.0f;

	battle_bg->m.cm.initPolygons[0].size = 1400.0f;
	battle_bg->m.cm.initPolygons[0].y = 0.0f;
	battle_bg->m.cm.initPolygons[0].uvRate = 14.0f;
	battle_bg->m.cm.initPolygons[0].r = 255.0f;
	battle_bg->m.cm.initPolygons[0].g = 255.0f;
	battle_bg->m.cm.initPolygons[0].b = 255.0f;
	battle_bg->m.cm.initPolygons[0].a = 255.0f;
	battle_bg->m.cm.initPolygons[0].u = 0.0f;
	battle_bg->m.cm.initPolygons[0].v = 0.0f;
	battle_bg->m.cm.initPolygons[0].varShiftU = 0.0f;
	battle_bg->m.cm.initPolygons[0].varShiftV = -0.01f;

	battle_bg->m.cm.initPolygons[1].size = 1400.0f;
	battle_bg->m.cm.initPolygons[1].y = 100.0f;
	battle_bg->m.cm.initPolygons[1].uvRate = 4.0f;
	battle_bg->m.cm.initPolygons[1].r = 255.0f;
	battle_bg->m.cm.initPolygons[1].g = 255.0f;
	battle_bg->m.cm.initPolygons[1].b = 255.0f;
	battle_bg->m.cm.initPolygons[1].a = 0.0f;
	battle_bg->m.cm.initPolygons[1].u = 0.0f;
	battle_bg->m.cm.initPolygons[1].v = -0.0f;
	battle_bg->m.cm.initPolygons[1].varShiftU =  0.00025f;
	battle_bg->m.cm.initPolygons[1].varShiftV = -0.001f;

	battle_bg->m.cm.initPolygons[2].size = 1400.0f;
	battle_bg->m.cm.initPolygons[2].y = 200.0f;
	battle_bg->m.cm.initPolygons[2].uvRate = 4.0f;
	battle_bg->m.cm.initPolygons[2].r = 255.0f;
	battle_bg->m.cm.initPolygons[2].g = 255.0f;
	battle_bg->m.cm.initPolygons[2].b = 255.0f;
	battle_bg->m.cm.initPolygons[2].a = 0.0f;
	battle_bg->m.cm.initPolygons[2].u = 0.25f;
	battle_bg->m.cm.initPolygons[2].v = -0.25f;
	battle_bg->m.cm.initPolygons[2].varShiftU =  0.001f;
	battle_bg->m.cm.initPolygons[2].varShiftV = -0.002f;
	
	int handle_bush = LoadGraph( "data\\graphics\\image\\bush.png" ) ;
	if(handle_bush < 0) {
		nn_commonLog( 1, "error\n");
	}
	int handle_cloud = LoadGraph( "data\\graphics\\image\\cloud.png" ) ;
	if(handle_cloud < 0) {
		nn_commonLog( 1, "error\n");
	}
	battle_bg->m.cm.initPolygonHandles[0] = handle_bush;
	battle_bg->m.cm.initPolygonHandles[1] = handle_cloud;
	battle_bg->m.cm.initPolygonHandles[2] = handle_cloud;
	battle_bg->m.cm.initPolygonN = 3;
	
	battle_bg->start();

	this->lm.battleBg = battle_bg;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::nextBattle() {

	this->lm.battle->taskDelete();
	this->setupBattle();

	this->lm.animeFrame->back();
	this->lm.animeFrame->move();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::setupBoss() {
	
	nn::ts::quest::CEnemy * enemy;
	enemy = new nn::ts::quest::CEnemy( this, this->saveManager);
	enemy->gm.enemyID = nn::ts::e::common::ENEMY_ID_RUMIA;
	
	OBJ * obj = nn::dxlib::disp::getObj( enemy->dm.objIdEnemys[enemy->gm.enemyID]);

	nn::ts::common::TS_HIT * ts_hit;
	nn::ts::quest::ENEMY_PARTS * parts =  &enemy->dm.partss[enemy->gm.enemyID];
	for( int i = 0; i < parts->layoutIdHitN; i++) {

		LAYOUT * layout = nn::dxlib::disp::getLayout( parts->layoutIdHits[i]);
		ts_hit = &enemy->gm.param.hits[enemy->gm.param.hitN];
		ts_hit->hit.x = layout->x + layout->w / 2 - obj->w / 2;
		ts_hit->hit.y = layout->y + layout->h / 2 - obj->h / 2;
		ts_hit->hit.hitType = nn::ts::e::common::HIT_TYPE_CIRCLE;
		ts_hit->hit.r = layout->w / 2;
		enemy->gm.param.hitN++;

	}

	for( int i = 0; i < parts->layoutIdWeakPointN; i++) {
				
		LAYOUT * layout = nn::dxlib::disp::getLayout( parts->layoutIdWeakPoints[i]);
		ts_hit = &enemy->gm.param.hits[enemy->gm.param.hitN];
		ts_hit->hit.x = layout->x + layout->w / 2 - obj->w / 2;
		ts_hit->hit.y = layout->y + layout->h / 2 - obj->h / 2;
		ts_hit->hit.hitType = nn::ts::e::common::HIT_TYPE_CIRCLE;
		ts_hit->hit.r = layout->w / 2;
		ts_hit->weakPointFlag = true;
		enemy->gm.param.hitN++;

	}

	enemy->gm.param.maxHP[0] = 2000000;
	enemy->gm.param.maxHP[1] = 2500000;
	enemy->gm.param.maxHP[2] = 3000000;
	enemy->gm.param.gaugeN = 3;

	enemy->gm.param.bossFlag = true;
	
	int enmey_turn_tbls[][4] = {
		{15, 3, 4, 5},
	};
	int enmey_reset_turn_tbls[][4] = {
		{ 15, 3, 3, 4},
	};
	int enmey_category_tbls[][4] = {
		{  0, 2, 3, 3},
	};

	for( int i = 0; i < parts->layoutIdTurnN; i++) {

		LAYOUT * layout = nn::dxlib::disp::getLayout( parts->layoutIdTurns[i]);
		enemy->gm.param.attacks[i].x = layout->x + layout->w / 2 - obj->w / 2;
		enemy->gm.param.attacks[i].y = layout->y + layout->h / 2 - obj->h / 2;
		enemy->gm.param.attacks[i].firstTurn = enmey_turn_tbls[0][i];
		enemy->gm.param.attacks[i].resetTurn = enmey_reset_turn_tbls[0][i];
		enemy->gm.param.attacks[i].category = enmey_category_tbls[0][i];

	}
	enemy->gm.param.attackN = parts->layoutIdTurnN;

	enemy->lm.quest = this;

	this->lm.boss = enemy;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::writeXML() {
	
	boost::property_tree::ptree xml;
	boost::property_tree::ptree& xml_quest  = xml.add( "quest", "");
	boost::property_tree::ptree& xml_battle = xml_quest.add( "battle", "");
	xml_battle.put( "<xmlattr>.no", 1);
	{
		boost::property_tree::ptree& xml_enemy = xml_battle.add( "enemy", "");
		xml_enemy.put( "name", "妖精青");
		xml_enemy.put( "hp", 500000);
		xml_enemy.put( "x", 126);
		xml_enemy.put( "y", 42);
		{
			boost::property_tree::ptree& xml_attack = xml_enemy.add( "attack", "");
			xml_attack.put( "turn", 2);
			xml_attack.put( "reset_turn", 2);
			xml_attack.put( "category", "マシンガンダウン");
		}
		{
			boost::property_tree::ptree& xml_attack = xml_enemy.add( "attack", "");
			xml_attack.put( "turn", 6);
			xml_attack.put( "reset_turn", 6);
			xml_attack.put( "category", "マシンガンダウン");
		}
	}

	{
		boost::property_tree::ptree& xml_enemy = xml_battle.add( "enemy", "");
		xml_enemy.put( "name", "妖精緑");
		xml_enemy.put( "hp", 500000);
		xml_enemy.put( "x", 378);
		xml_enemy.put( "y", 42);
		{
			boost::property_tree::ptree& xml_attack = xml_enemy.add( "attack", "");
			xml_attack.put( "turn", 3);
			xml_attack.put( "reset_turn", 3);
			xml_attack.put( "category", "マシンガンダウン");
		}
		{
			boost::property_tree::ptree& xml_attack = xml_enemy.add( "attack", "");
			xml_attack.put( "turn", 6);
			xml_attack.put( "reset_turn", 6);
			xml_attack.put( "category", "マシンガンダウン");
		}
	}

	boost::property_tree::xml_parser::write_xml(
		"test.xml", xml, std::locale(),
		boost::property_tree::xml_parser::xml_writer_make_settings<std::string>(
			' ', 2, "Shift-JIS"));

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::readXMLEnemyName( nn::ts::quest::CEnemy * enemy, 
	boost::property_tree::ptree& xmlEnemyName)
{
	std::string name = xmlEnemyName.get_value<std::string>();
	LOG_XML( "name = %s\n", name.c_str());

	// 名前→ID
	enemy->gm.enemyID = this->lm.ctrl->enemyName2Id.find( name)->second;
	OBJ * obj = nn::dxlib::disp::getObj( enemy->dm.objIdEnemys[enemy->gm.enemyID]);

	nn::ts::common::TS_HIT * ts_hit;
	nn::ts::quest::ENEMY_PARTS * parts =  &enemy->dm.partss[enemy->gm.enemyID];
	for( int i = 0; i < parts->layoutIdHitN; i++) {

		LAYOUT * layout = nn::dxlib::disp::getLayout( parts->layoutIdHits[i]);
		ts_hit = &enemy->gm.param.hits[enemy->gm.param.hitN];
		ts_hit->hit.x = layout->x + layout->w / 2 - obj->w / 2;
		ts_hit->hit.y = layout->y + layout->h / 2 - obj->h / 2;
		ts_hit->hit.hitType = nn::ts::e::common::HIT_TYPE_CIRCLE;
		ts_hit->hit.r = layout->w / 2;
		enemy->gm.param.hitN++;

	}
	for( int i = 0; i < parts->layoutIdWeakPointN; i++) {
				
		LAYOUT * layout = nn::dxlib::disp::getLayout( parts->layoutIdWeakPoints[i]);
		ts_hit = &enemy->gm.param.hits[enemy->gm.param.hitN];
		ts_hit->hit.x = layout->x + layout->w / 2 - obj->w / 2;
		ts_hit->hit.y = layout->y + layout->h / 2 - obj->h / 2;
		ts_hit->hit.hitType = nn::ts::e::common::HIT_TYPE_CIRCLE;
		ts_hit->hit.r = layout->w / 2;
		ts_hit->weakPointFlag = true;
		enemy->gm.param.hitN++;

	}
}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::readXMLBoss( boost::property_tree::ptree& xmlBoss) {
	
	for(auto it_boss_item : xmlBoss.get_child( "")) {
		LOG_XML( "key = %s\n", it_boss_item.first.c_str());

		if(it_boss_item.first == "x") {
			double x = it_boss_item.second.get_value<double>();
			LOG_XML( "x = %lf\n", x);
			this->lm.boss->gameChar.game.hit.x = nn::ts::e::common::BATTLE_FIELD_X + x;
		}					
		else if(it_boss_item.first == "y") {
			double y = it_boss_item.second.get_value<double>();
			LOG_XML( "y = %lf\n", y);
			this->lm.boss->gameChar.game.hit.y = nn::ts::e::common::BATTLE_FIELD_Y + y;
		}	

	}

	this->lm.boss->dd->m.user.animeId = this->lm.boss->dm.animeIdEnemyIn;
	this->lm.boss->dd->m.user.layer = 0;
	this->lm.boss->dd->m.user.directEndCBFunc = nn::ts::quest::CEnemy::endDirectCB;
	this->lm.boss->dd->m.user.directEndCBFuncArg = this->lm.boss;

	this->lm.boss->lm.endAttackCBFuncId = nn::ts::e::common::CB_FUNC_ID_END_ATTACK_ENEMY;
	this->lm.boss->lm.endAttackCBFuncOwner = this->lm.battle;
	this->lm.boss->start();
	this->lm.battle->gm.enemys[this->lm.battle->gm.enemyN] = this->lm.boss;
	this->lm.battle->gm.enemyN++;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::readXMLEnemyAttack( nn::ts::quest::CEnemy * enemy,
		boost::property_tree::ptree& xmlEnemyAttack)
{
	nn::ts::quest::ENEMY_PARTS * parts =  &enemy->dm.partss[enemy->gm.enemyID];
	OBJ * obj = nn::dxlib::disp::getObj( enemy->dm.objIdEnemys[enemy->gm.enemyID]);
	LAYOUT * layout = nn::dxlib::disp::getLayout( parts->layoutIdTurns[enemy->gm.param.attackN]);
	nn::ts::quest::ENEMY_ATTACK * attack = &enemy->gm.param.attacks[enemy->gm.param.attackN];
	attack->x = layout->x + layout->w / 2 - obj->w / 2;
	attack->y = layout->y + layout->h / 2 - obj->h / 2;

	for(auto it_attack_item : xmlEnemyAttack.get_child( "")) {
								
		LOG_XML( "key = %s\n", it_attack_item.first.c_str());
		if(it_attack_item.first == "turn") {
			int turn = it_attack_item.second.get_value<int>();
			LOG_XML( "turn = %d\n", turn);
			attack->firstTurn = turn;
		}
		if(it_attack_item.first == "reset_turn") {
			int reset_turn = it_attack_item.second.get_value<int>();
			LOG_XML( "reset_turn = %d\n", reset_turn);
			attack->resetTurn = reset_turn;
		}
		if(it_attack_item.first == "category") {
			std::string category = it_attack_item.second.get_value<std::string>();
			LOG_XML( "category = %s\n", category.c_str());
			attack->category = this->lm.ctrl->enemyAttackName2Id.find( category)->second;
		}

	}
	enemy->gm.param.attackN++;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::readXMLEnemy( boost::property_tree::ptree& xmlEnmey) {
	
	nn::ts::quest::CEnemy * enemy;
	enemy = new nn::ts::quest::CEnemy( this, this->saveManager);

	for(auto it_enemy_item : xmlEnmey.get_child( "")) {
		LOG_XML( "key = %s\n", it_enemy_item.first.c_str());
					
		if(it_enemy_item.first == "name") {
			this->readXMLEnemyName( enemy, it_enemy_item.second);
		}
		else if(it_enemy_item.first == "hp") {
			int hp = it_enemy_item.second.get_value<int>();
			LOG_XML( "hp = %d\n", hp);
			enemy->gm.param.maxHP[0] = hp;
			enemy->gm.status.hp = enemy->gm.param.maxHP[0];
			enemy->gm.status.damageHp = enemy->gm.param.maxHP[0];
			enemy->gm.param.gaugeN++;
		}
		else if(it_enemy_item.first == "x") {
			double x = it_enemy_item.second.get_value<double>();
			LOG_XML( "x = %lf\n", x);
			enemy->gameChar.game.hit.x = nn::ts::e::common::BATTLE_FIELD_X + x;
		}
		else if(it_enemy_item.first == "y") {
			double y = it_enemy_item.second.get_value<double>();
			LOG_XML( "y = %lf\n", y);
			enemy->gameChar.game.hit.y = nn::ts::e::common::BATTLE_FIELD_Y + y;
		}
		else if(it_enemy_item.first == "attack") {
			this->readXMLEnemyAttack( enemy, it_enemy_item.second);
		}

	}

	enemy->lm.quest = this;
	enemy->lm.endAttackCBFuncId = nn::ts::e::common::CB_FUNC_ID_END_ATTACK_ENEMY;
	enemy->lm.endAttackCBFuncOwner = this->lm.battle;

	enemy->dd->m.user.animeId = enemy->dm.animeIdEnemyIn;
	enemy->dd->m.user.layer = 0;

	if(enemy->gm.param.bossFlag) {
		this->lm.boss = enemy;
	}

	enemy->start();

	this->lm.battle->gm.enemys[this->lm.battle->gm.enemyN] = enemy;
	this->lm.battle->gm.enemyN++;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::readXML() {
	
	unsigned int size = fsGetFileSize( "quest\\0001.xml");
	char * c = (char *)malloc( size);
	fsLoadFile( c, "quest\\0001.xml", size);

	boost::iostreams::stream<boost::iostreams::array_source> stream( c, size);
	boost::property_tree::ptree xml;
	boost::property_tree::read_xml( stream, xml);

	for(auto it_battle : xml.get_child( "quest")) {

		LOG_XML( "key = %s\n", it_battle.first.c_str());
		if(it_battle.first == "battle") {
			LOG_XML( "no = %d\n", it_battle.second.get<int>( "<xmlattr>.no"));
			if(it_battle.second.get<int>( "<xmlattr>.no") == this->gm.status.battleCnt) {
				for(auto it_battle_item : it_battle.second.get_child( "")) {
					LOG_XML( "key = %s\n", it_battle_item.first.c_str());

					if(it_battle_item.first == "enemy") {
						this->readXMLEnemy( it_battle_item.second);
					}
					else if(it_battle_item.first == "boss") {
						this->readXMLBoss( it_battle_item.second);
					}

				}
			}
		}

	}

	free( c);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::setupBattle() {
	
	LAYOUT * layout_filed = nn::dxlib::disp::getLayout( this->dm.layoutIdField);
	
	this->gm.status.battleCnt++;
	
	nn::ts::quest::CBattle * battle = new nn::ts::quest::CBattle( this, this->saveManager);
	this->lm.battle = battle;
	battle->gm.quest = this;
	battle->lm.ctrl = this->lm.ctrl;
	battle->gm.x = nn::ts::e::common::BATTLE_FIELD_X;
	battle->gm.y = nn::ts::e::common::BATTLE_FIELD_Y;
	battle->gm.w = nn::ts::e::common::BATTLE_FIELD_W;
	battle->gm.h = nn::ts::e::common::BATTLE_FIELD_H;
	battle->gm.itemPoints[0].x = 0;
	battle->gm.itemPoints[0].y = 100;
	battle->gm.itemPoints[1].x = nn::ts::e::common::BATTLE_FIELD_W - 100;
	battle->gm.itemPoints[1].y = 0;
	battle->gm.itemPoints[2].x = nn::ts::e::common::BATTLE_FIELD_W;
	battle->gm.itemPoints[2].y = nn::ts::e::common::BATTLE_FIELD_H - 100;
	battle->gm.itemPoints[3].x = 100;
	battle->gm.itemPoints[3].y = nn::ts::e::common::BATTLE_FIELD_H;
	battle->gm.itemPointN = 4;
	this->setupCamera();

	this->readXML();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::dump() {
		
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CQuest::dump() start ----------\n");
	LOG_CLASS_DUMP( "this = 0x%08x\n", this);
	LOG_CLASS_DUMP( "this->state = %d\n", this->state);
	LOG_CLASS_DUMP( "this->oldState = %d\n", this->oldState);
	LOG_CLASS_DUMP( "this->waitCnt = %d\n", this->waitCnt);
	LOG_CLASS_DUMP( "this->frameCnt = %u\n", this->frameCnt);
	LOG_CLASS_DUMP( "this->gm.status = %d\n", this->gm.status);
	LOG_CLASS_DUMP( "this->cm.moveState = %d\n", this->cm.moveState);
	LOG_CLASS_DUMP( "this->lm.animeFrame = 0x%08x\n", this->lm.animeFrame);
	LOG_CLASS_DUMP( "this->lm.animeFrame = 0x%08x\n", this->lm.battle);
	this->lm.battle->dump();
	for( int i = 0; i < 4; i++) {
	LOG_CLASS_DUMP( "this->lm.balls[%d] = 0x%08x\n", i, this->lm.balls[i]);
	this->lm.balls[i]->dump();
	}
	LOG_CLASS_DUMP( "this->lm.boss = 0x%08x\n", this->lm.boss);
	LOG_CLASS_DUMP( "this->lm.battleBg = 0x%08x\n", this->lm.battleBg);
	this->lm.battleBg->dump();
	LOG_CLASS_DUMP( "---------- nn::ts::quest::CQuest::dump() end   ----------\n");

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::addTe( int angle, bool spell) {

	this->lm.questCtrl->addTe( angle, spell);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::setupCamera() {
	
	nn::ts::quest::CBattle * battle = this->lm.battle;

	nn::dxlib::disp::setCameraFlag( true);
	nn::dxlib::disp::setCameraPos(
		battle->gm.x + battle->gm.w / 2, battle->gm.y + battle->gm.h / 2);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::recoveryHPNextBattle() {

	this->gm.status.damageHp += (this->gm.status.maxHp - this->gm.status.damageHp) / 2;
	this->syncDisplayHP();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::syncDisplayHP() {
	
	this->gm.status.hp =  this->gm.status.damageHp;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::damage( int damage) {

	this->gm.status.damageHp -= damage;
	if(this->gm.status.damageHp < 0) {
		this->gm.status.damageHp = 0;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuest::recovery( int percent) {

	int recovery = this->gm.status.maxHp * percent / 100;

	this->gm.status.damageHp += recovery;
	if(this->gm.status.damageHp > this->gm.status.maxHp) {
		this->gm.status.damageHp = this->gm.status.maxHp;
	}

}
//---------------------------------------------------------------------------

