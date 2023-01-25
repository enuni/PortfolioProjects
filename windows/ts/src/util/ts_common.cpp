
#include "ball.h"
#include "enemy.h"
#include "pull.h"
#include "damage_disp.h"
#include "quest.h"
#include "master_spark.h"
#include "effect_laser.h"
#include "effect_bullet.h"
#include "battle.h"
#include "battle_bg.h"
#include "title.h"
#include "battle_item.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "anime_data.h"

#include "ts_common.h"
//-----------------------------------------------------------------------------
const char nn::ts::common::gCharactersSaveDataDebugQuest[] = "debug_quest.data";
//-----------------------------------------------------------------------------
void nn::ts::common::setDispMember() {
	
	setDispMemberBall();
	setDispMemberEnemy();
	setDispMemberPull();
	setDispMemberDamageDisp();
	setDispMemberQuest();
	setDispMemberMasterSpark();
	setDispMemberLaser();
	setDispMemberBullet();
	setDispMemberTitle();
	setDIspMemverBattleItem();

}
//-----------------------------------------------------------------------------
void nn::ts::common::setDispMemberBall() {

	nn::ts::quest::CBall::dm.objIdBalls = (int *)malloc(sizeof(int) * nn::ts::e::common::CHAR_ID_MAX);
	memset( nn::ts::quest::CBall::dm.objIdBalls, 0, sizeof(int) * nn::ts::e::common::CHAR_ID_MAX);
	nn::ts::quest::CBall::dm.objIdBalls[nn::ts::e::common::CHAR_ID_REIMU] = ::OBJ_ID_BALL_0_0;
	nn::ts::quest::CBall::dm.objIdBalls[nn::ts::e::common::CHAR_ID_MARISA] = ::OBJ_ID_BALL_0_1;
	nn::ts::quest::CBall::dm.objIdBalls[nn::ts::e::common::CHAR_ID_SANAE] = ::OBJ_ID_BALL_0_2;
	nn::ts::quest::CBall::dm.objIdBalls[nn::ts::e::common::CHAR_ID_YOMU] = ::OBJ_ID_BALL_0_3;
	
	nn::ts::quest::CBall::dm.objIdBallBgs = (int *)malloc(sizeof(int) * nn::ts::e::common::CHAR_ID_MAX);
	nn::ts::quest::CBall::dm.objIdBallBgs[nn::ts::e::common::CHAR_ID_REIMU] = ::OBJ_ID_BALL_BG_BLUE_0;
	nn::ts::quest::CBall::dm.objIdBallBgs[nn::ts::e::common::CHAR_ID_MARISA] = ::OBJ_ID_BALL_BG_RED_0;
	nn::ts::quest::CBall::dm.objIdBallBgs[nn::ts::e::common::CHAR_ID_SANAE] = ::OBJ_ID_BALL_BG_BLUE_0;
	nn::ts::quest::CBall::dm.objIdBallBgs[nn::ts::e::common::CHAR_ID_YOMU] = ::OBJ_ID_BALL_BG_BLUE_0;

	nn::ts::quest::CBall::dm.animeIdBallIn = ::ANIMATION_ID_BALL_IN;

}
//-----------------------------------------------------------------------------
void nn::ts::common::setDispMemberEnemy() {

	nn::ts::quest::CEnemy::dm.objIdEnemys = (int *)malloc(sizeof(int) * nn::ts::e::common::ENEMY_ID_MAX);
	nn::ts::quest::CEnemy::dm.objIdEnemys[nn::ts::e::common::ENEMY_ID_RUMIA] = ::OBJ_ID_BOSS_RUMIA_0;
	nn::ts::quest::CEnemy::dm.objIdEnemys[nn::ts::e::common::ENEMY_ID_YOSEIAO] = ::OBJ_ID_ENEMY_YOSEIAO_0;
	nn::ts::quest::CEnemy::dm.objIdEnemys[nn::ts::e::common::ENEMY_ID_YOSEIMIDORI] = ::OBJ_ID_ENEMY_YOSEIMIDORI_0;
	nn::ts::quest::CEnemy::dm.objIdEnemys[nn::ts::e::common::ENEMY_ID_OKUU] = ::OBJ_ID_ENEMY_OKUU_0;

	nn::ts::quest::CEnemy::dm.objIdDameNumber = ::OBJ_ID_TEST_NUM_0;

	nn::ts::quest::CEnemy::dm.partss =
		(nn::ts::quest::ENEMY_PARTS *)malloc(
			sizeof(nn::ts::quest::ENEMY_PARTS) * nn::ts::e::common::ENEMY_ID_MAX
		);

	nn::ts::quest::ENEMY_PARTS * parts;
	parts = &nn::ts::quest::CEnemy::dm.partss[nn::ts::e::common::ENEMY_ID_RUMIA];
	parts->layoutIdTurns[0] = ::LAYOUT_ID_RUMIA_PARTS_UP;
	parts->layoutIdTurns[1] = ::LAYOUT_ID_RUMIA_PARTS_LEFT;
	parts->layoutIdTurns[2]	= ::LAYOUT_ID_RUMIA_PARTS_RIGHT;
	parts->layoutIdTurns[3]	= ::LAYOUT_ID_RUMIA_PARTS_DOWN;
	parts->layoutIdTurnN = 4;
	parts->layoutIdHits[0] = ::LAYOUT_ID_RUMIA_PARTS_HIT;
	parts->layoutIdHitN = 1;
	parts->layoutIdWeakPoints[0] = ::LAYOUT_ID_RUMIA_PARTS_UP;
	parts->layoutIdWeakPoints[1] = ::LAYOUT_ID_RUMIA_PARTS_LEFT;
	parts->layoutIdWeakPoints[2] = ::LAYOUT_ID_RUMIA_PARTS_RIGHT;
	parts->layoutIdWeakPointN = 3;

	parts = &nn::ts::quest::CEnemy::dm.partss[nn::ts::e::common::ENEMY_ID_YOSEIAO];
	parts->layoutIdTurns[0] = ::LAYOUT_ID_YOSEIAO_PARTS_UP;
	parts->layoutIdTurns[1] = ::LAYOUT_ID_YOSEIAO_PARTS_LEFT;
	parts->layoutIdTurnN = 2;
	parts->layoutIdHits[0] = ::LAYOUT_ID_YOSEIAO_PARTS_HIT;
	parts->layoutIdHitN = 1;
	//parts->layoutIdWeakPoints[0] = 0;
	parts->layoutIdWeakPointN = 0;

	parts = &nn::ts::quest::CEnemy::dm.partss[nn::ts::e::common::ENEMY_ID_YOSEIMIDORI];
	parts->layoutIdTurns[0] = ::LAYOUT_ID_YOSEIMIDORI_PARTS_LEFT;
	parts->layoutIdTurns[1] = ::LAYOUT_ID_YOSEIMIDORI_PARTS_RIGHT;
	parts->layoutIdTurnN = 2;
	parts->layoutIdHits[0] = ::LAYOUT_ID_YOSEIMIDORI_PARTS_HIT;
	parts->layoutIdHitN = 1;
	//parts->layoutIdWeakPoints[0] = 0;
	parts->layoutIdWeakPointN = 0;

	parts = &nn::ts::quest::CEnemy::dm.partss[nn::ts::e::common::ENEMY_ID_OKUU];
	parts->layoutIdTurns[0] = ::LAYOUT_ID_YOSEIMIDORI_PARTS_LEFT;
	parts->layoutIdTurns[1] = ::LAYOUT_ID_YOSEIMIDORI_PARTS_RIGHT;
	parts->layoutIdTurnN = 2;
	parts->layoutIdHits[0] = ::LAYOUT_ID_YOSEIMIDORI_PARTS_HIT;
	parts->layoutIdHitN = 1;
	//parts->layoutIdWeakPoints[0] = 0;
	parts->layoutIdWeakPointN = 0;

	nn::ts::quest::CEnemy::dm.animeIdEnemyIn = ::ANIMATION_ID_ENEMY_IN;

	nn::ts::quest::CEnemy::dm.animeIdBossExplosionSamll = ::ANIMATION_ID_BOSS_EXPLOSION_SMALL;
	nn::ts::quest::CEnemy::dm.animeIdBossEscape = ::ANIMATION_ID_BOSS_ESCAPE;

}
//-----------------------------------------------------------------------------
void nn::ts::common::setDispMemberPull() {

	nn::ts::quest::CPull::dm.objIdArrorw = ::OBJ_ID_TEST_ARROW_0;

}
//-----------------------------------------------------------------------------
void nn::ts::common::setDispMemberDamageDisp() {

	nn::ts::quest::CDamageDisp::dm.objIdNumberTop = ::OBJ_ID_TEST_NUM_0;

}
//-----------------------------------------------------------------------------
void nn::ts::common::setDispMemberQuest() {
	
	nn::ts::quest::CQuest::dm.layoutIdCharUpTop = ::LAYOUT_ID_BATTLE_CHAR_UP_1;
	nn::ts::quest::CQuest::dm.layoutIdCharUpOffset = ::LAYOUT_ID_BATTLE_CHAR_UP_OFFSET;
	nn::ts::quest::CQuest::dm.objIdCharUps = (int *)malloc(sizeof(int) * nn::ts::e::common::CHAR_ID_MAX);
	nn::ts::quest::CQuest::dm.objIdCharUps[nn::ts::e::common::CHAR_ID_REIMU] = ::OBJ_ID_BALL_UP_STATUS_0;
	nn::ts::quest::CQuest::dm.objIdCharUps[nn::ts::e::common::CHAR_ID_MARISA] = ::OBJ_ID_BALL_UP_STATUS_1;
	nn::ts::quest::CQuest::dm.objIdCharUps[nn::ts::e::common::CHAR_ID_SANAE] = ::OBJ_ID_BALL_UP_STATUS_2;
	nn::ts::quest::CQuest::dm.objIdCharUps[nn::ts::e::common::CHAR_ID_YOMU] = ::OBJ_ID_BALL_UP_STATUS_3;
	nn::ts::quest::CQuest::dm.layoutIdField = ::LAYOUT_ID_BATTLE_FIELD;
	nn::ts::quest::CQuest::dm.layoutIdHpBarBoss = ::LAYOUT_ID_BATTLE_BOSS_HP_BAR;
	nn::ts::quest::CQuest::dm.layoutIdHpBar = ::LAYOUT_ID_BATTLE_HP_BAR;
	nn::ts::quest::CQuest::dm.layoutIdHpBarBosFrame = ::LAYOUT_ID_BATTLE_BOSS_HP_BAR_FRAME;
	nn::ts::quest::CQuest::dm.layoutIdHpBarFrame = ::LAYOUT_ID_BATTLE_HP_BAR_FRAME;
	nn::ts::quest::CQuest::dm.layoutIdStage = ::LAYOUT_ID_BATTLE_STAGE;
	nn::ts::quest::CQuest::dm.layoutIdStageNum = ::LAYOUT_ID_BATTLE_STAGE_NUM;
	nn::ts::quest::CQuest::dm.layoutIdBoss = ::LAYOUT_ID_BATTLE_BOSS;
	nn::ts::quest::CQuest::dm.layoutIdHp = ::LAYOUT_ID_BATTLE_HP;
	nn::ts::quest::CQuest::dm.layoutIdFieldBg = ::LAYOUT_ID_BATTLE_FIELD_BG;
	nn::ts::quest::CQuest::dm.layoutIdFieldOuts[0] = ::LAYOUT_ID_BATTLE_FIELD_OUT_1;
	nn::ts::quest::CQuest::dm.layoutIdFieldOuts[1] = ::LAYOUT_ID_BATTLE_FIELD_OUT_2;
	nn::ts::quest::CQuest::dm.layoutIdFieldOuts[2] = ::LAYOUT_ID_BATTLE_FIELD_OUT_3;
	nn::ts::quest::CQuest::dm.layoutIdFieldOuts[3] = ::LAYOUT_ID_BATTLE_FIELD_OUT_4;
	nn::ts::quest::CQuest::dm.objIdCharFulls = (int *)malloc(sizeof(int) * nn::ts::e::common::CHAR_ID_MAX);
	nn::ts::quest::CQuest::dm.objIdCharFulls[nn::ts::e::common::CHAR_ID_REIMU] = ::OBJ_ID_TEST_CHAR_FULL_0;
	nn::ts::quest::CQuest::dm.objIdCharFulls[nn::ts::e::common::CHAR_ID_MARISA] = ::OBJ_ID_TEST_CHAR_FULL_1;
	nn::ts::quest::CQuest::dm.objIdCharFulls[nn::ts::e::common::CHAR_ID_SANAE] = ::OBJ_ID_TEST_CHAR_FULL_2;
	nn::ts::quest::CQuest::dm.objIdCharFulls[nn::ts::e::common::CHAR_ID_YOMU] = ::OBJ_ID_TEST_CHAR_FULL_3;
	nn::ts::quest::CQuest::dm.animeIdBattleFrameIn = ::ANIMATION_ID_BATTLE_FRAME_IN;
	nn::ts::quest::CQuest::dm.animeIdStageStart = ::ANIMATION_ID_STAGE_START;
	nn::ts::quest::CQuest::dm.animeIdWin = ::ANIMATION_ID_WIN;

}
//-----------------------------------------------------------------------------
void nn::ts::common::setDispMemberMasterSpark() {

	nn::ts::effect::CMasterSpark::dm.objIdMasterSpark = ::OBJ_ID_EFFECT_MASTER_SPARK_1_0;
	nn::ts::effect::CMasterSpark::dm.objIdMasterSparkBase = ::OBJ_ID_EFFECT_MASTER_SPARK_BASE_1_0;

}
//-----------------------------------------------------------------------------
void nn::ts::common::setDispMemberLaser() {

	nn::ts::effect::CLaser::dm.objIdLaser[0] = ::OBJ_ID_EFFECT_COMMON_LASER_0;
	nn::ts::effect::CLaser::dm.objIdLaser[1] = ::OBJ_ID_EFFECT_COMMON_LASER_1;
	nn::ts::effect::CLaser::dm.objIdLaser[2] = ::OBJ_ID_EFFECT_COMMON_LASER_1;
	nn::ts::effect::CLaser::dm.objIdLaser[3] = ::OBJ_ID_EFFECT_COMMON_LASER_1;
	nn::ts::effect::CLaser::dm.objIdLaser[4] = ::OBJ_ID_EFFECT_COMMON_LASER_1;

}
//-----------------------------------------------------------------------------
void nn::ts::common::setDispMemberBullet() {

	nn::ts::effect::CBullet::dm.objIdHoming[0] = ::OBJ_ID_EFFECT_COMMON_NORMAL_BULLET_0;
	nn::ts::effect::CBullet::dm.objIdHoming[1] = ::OBJ_ID_EFFECT_COMMON_NORMAL_BULLET_1;
	nn::ts::effect::CBullet::dm.objIdHoming[2] = ::OBJ_ID_EFFECT_COMMON_HOMING_BULLET_0;
	nn::ts::effect::CBullet::dm.objIdHoming[3] = ::OBJ_ID_EFFECT_COMMON_NORMAL_BULLET_2;
	nn::ts::effect::CBullet::dm.objIdHoming[4] = ::OBJ_ID_EFFECT_COMMON_NORMAL_BULLET_3;

	nn::ts::effect::CBullet::dm.objIdNormal[0] = ::OBJ_ID_EFFECT_COMMON_NORMAL_BULLET_0;
	nn::ts::effect::CBullet::dm.objIdNormal[1] = ::OBJ_ID_EFFECT_COMMON_NORMAL_BULLET_1;
	nn::ts::effect::CBullet::dm.objIdNormal[2] = ::OBJ_ID_EFFECT_COMMON_NORMAL_BULLET_0;
	nn::ts::effect::CBullet::dm.objIdNormal[3] = ::OBJ_ID_EFFECT_COMMON_NORMAL_BULLET_2;
	nn::ts::effect::CBullet::dm.objIdNormal[4] = ::OBJ_ID_EFFECT_COMMON_NORMAL_BULLET_3;

}
//-----------------------------------------------------------------------------
void nn::ts::common::setDispMemberTitle() {

	nn::ts::title::CTitle::dm.layoutIdAgrippa = ::LAYOUT_ID_LOGO_AGRIPPA;
	nn::ts::title::CTitle::dm.layoutIdN2nsoft = ::LAYOUT_ID_LOGO_N2NSOFT;
	nn::ts::title::CTitle::dm.animeIdTitleTextIn = ::ANIMATION_ID_TITLE_TEXT_IN;

}
//-----------------------------------------------------------------------------
void nn::ts::common::setDIspMemverBattleItem() {

	nn::ts::quest::CBattleItem::dm.objIdRecoveryItems = (int **)malloc(sizeof(void *) * nn::ts::e::common::BATTLE_ITEM_ID_MAX);

	nn::ts::quest::CBattleItem::dm.objIdRecoveryItems[nn::ts::e::common::BATTLE_ITEM_ID_NONE]
		= NULL;
	nn::ts::quest::CBattleItem::dm.objIdRecoveryItems[nn::ts::e::common::BATTLE_ITEM_ID_RECOVERY]
		= (int *)malloc(sizeof(int) * nn::ts::e::common::BATTLE_ITEM_LEVEL_N_RECOVERY);
	nn::ts::quest::CBattleItem::dm.objIdRecoveryItems[nn::ts::e::common::BATTLE_ITEM_ID_RECOVERY][0]
		= OBJ_ID_ITEM_RECOVERY_0;
	nn::ts::quest::CBattleItem::dm.objIdRecoveryItems[nn::ts::e::common::BATTLE_ITEM_ID_RECOVERY][1]
		= OBJ_ID_ITEM_RECOVERY_1;
	nn::ts::quest::CBattleItem::dm.objIdRecoveryItems[nn::ts::e::common::BATTLE_ITEM_ID_RECOVERY][2]
		= OBJ_ID_ITEM_RECOVERY_2;

}
//-----------------------------------------------------------------------------
void nn::ts::common::setCallBackFunc() {

	nn::common::util::CCallBackManager::init();

	nn::common::util::CCallBackManager::add( nn::ts::e::common::CB_FUNC_ID_PULL_DECIDE,				nn::ts::quest::CBattle::pullDecideCB);
	nn::common::util::CCallBackManager::add( nn::ts::e::common::CB_FUNC_ID_END_ATTACK_ENEMY,		nn::ts::quest::CBattle::endAttackEnemyCB);
	nn::common::util::CCallBackManager::add( nn::ts::e::common::CB_FUNC_ID_NEXT_MOVE_QUEST,			nn::ts::quest::CQuest::nextMoveCB);
	nn::common::util::CCallBackManager::add( nn::ts::e::common::CB_FUNC_ID_NEXT_DIRECT_BATTLE_BG,	nn::ts::quest::CBattleBg::nextDirectCB);

}
//-----------------------------------------------------------------------------
void nn::ts::common::hit( nn::common::util::type::t2d::COLLISION_ITEM * ci,
		  nn::ts::common::ATTACKER_INFO * ai
) {
	if(ai->hitCase == nn::ts::e::common::HIT_CASE_BALL_2_ENMEY ||
		ai->hitCase == nn::ts::e::common::HIT_CASE_SHOT_2_ENMEY
	) {
		nn::ts::common::TS_HIT * ts_hit = (nn::ts::common::TS_HIT *)ci->target; 
		nn::ts::quest::CEnemy * enemy = (nn::ts::quest::CEnemy *)ts_hit->owner;
		
		int damage = ai->damage;
		if(ts_hit->weakPointOnFlag) {
			damage *= 3;
		}
		if(ai->hitCase == nn::ts::e::common::HIT_CASE_BALL_2_ENMEY) {
			enemy->collision( damage);
		}
		else {
			enemy->damage( damage);
		}
	}
}
//-----------------------------------------------------------------------------