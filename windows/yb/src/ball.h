// BALL を置換
// ball を置換
// Ball を置換
// yb を置換
// main を置換

#ifndef __BALL_H__
#define __BALL_H__

#include "task_save.h"
#include "task.h"
#include "master.h"

// 前方宣言
#include "class_list.h"

#include "effect/effect_spell.h"
#include "effect/muso_reimu.h"

namespace nn {

namespace common {
namespace def {
namespace graphics2d {

typedef struct segment2_d SEGMENT2_D;

}
}
}

namespace yb {

namespace def {

typedef struct _hole_obj HOLE_OBJ;

}

namespace e {
namespace ball {

enum STATE {
	STATE_NONE,
	STATE_STANDBY,
	STATE_FLYING,
	STATE_SPELL,
	STATE_CUPIN,
	STATE_CUPIN_DISP,
	
	STATE_MAX
};

enum POWER_STATE {
	POWER_STATE_NORMAL,
	POWER_STATE_FULL,

	POWER_STATE_MAX
};

enum FLY_STATE {
	FLY_STATE_NONE,
	FLY_STATE_ON,

	FLY_STATE_MAX
};

enum DISP_STATE {
	DISP_STATE_DEFAULT,			// 通常
	DISP_STATE_DEAD,			// 気絶(正面)
	DISP_STATE_NORMAL_FRONT,	// 通常(正面)
	DISP_STATE_CRY,				// 泣き
	DISP_STATE_DOYA,			// 照れ(正面)→照れ
	DISP_STATE_JOY,				// 照れ
	DISP_SATTE_DIZZY,			// 目回し
	DISP_SATTE_DAMAGE,			// 気絶
	DISP_STATE_SHOCK,			// 気絶(正面)→気絶
	DISP_STATE_PRESS,			// 気絶(正面)+横拡大+縦縮小
	DISP_STATE_BURN,			// 気絶(正面)+RGB( 6, 4, 4)
	DISP_SATTE_PERA,			// 潰れ
	DISP_STATE_MUSO,			// 元の姿
	DISP_STATE_CLASH,			// お燐スペカはねられ

	DISP_STATE_MAX
};

enum BALL_FORM {
	BALL_FORM_DEFAULT,	// 通常
	BALL_FORM_PRESS,	// ペラペラ
	BALL_FORM_TORNADO,	// 竜巻
	BALL_FORM_MECHA,	// メカゆっくり
	BALL_FORM_SPEAR,	// スピア時のつぶれ状態
	BALL_FORM_ORIN,		// お燐火車
	BALL_FORM_VAMPIRE,	// 吸血鬼
	BALL_FORM_BURN,		// マスタースパーク

	BALL_FORM_MAX
};

} // namespece ball

namespace global {

enum SPELL_CARD;
}

} // namespece e

namespace enum_effect {
enum SPELL_EFFECT_KIND;
}

namespace main {
	
class CBall : public TASK_SAVE {
	
public:
	CBall( TASK_SAVE * oya);
	virtual ~CBall();
	void task();
	void disp();
	
	void start();
	void shot();
	void taskFlying();
	void taskCupin();

	void hit_cup();
	void add_angle( double addAngle);
	static void proc_wait_cnt_dec( void * ballArg);

	static void goodLuckEffectEndCb( void * ballArg);
	void goodLuckEffectEnd();

	bool taskInputGoodLuck();
	bool taskInputSpell();
	void taskFlySky();
	void taskFlyCup();
	void taskFlyBumper();
	void taskFlySlope( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyPillar( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyWarp( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyKick( nn::yb::def::HOLE_OBJ * obj);
	void taskFlySpin();
	bool taskFlyFall();
	bool taskFlyMove();		// ボールが停止したら true を返す
	void taskFlyCupin();
	void taskFlyCheckYakumono();	// 全ヤクモノを調べてflagを立てたりしておく

	void taskFlyDash( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyJump( nn::yb::def::HOLE_OBJ * obj);
	
	void taskFlyDamage( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyBunker( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyGravity( nn::yb::def::HOLE_OBJ * obj);

	void calcBoundUnzan( nn::yb::def::HOLE_OBJ * obj);//雲山との跳ね返り処理
	void calcBoundOku( nn::yb::def::HOLE_OBJ * obj);//お空との跳ね返り処理
	void defeatedOjama( nn::yb::def::HOLE_OBJ * obj);//お邪魔キャラを破壊
	void taskFlyUnzan( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyOku( nn::yb::def::HOLE_OBJ * obj); // 当たり判定等

	void taskFlyConveyor( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyFukiage( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyJumpWeak( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyJumpStrong( nn::yb::def::HOLE_OBJ * obj);
	void taskFlySeFloor( nn::yb::def::HOLE_OBJ * obj);
	
	static void warpEndCb( void * ballArg);
	void warEnd();

	void setDispState( int state);
	void setDispState( int state, int cnt);
	
	static void spellEffectEndCb( void * ballArg, void * dummy);
	static void spellEffectEndCb( void * ballArg);
	void spellEffectEnd();
	static void spellEffectMusoCb( void * ballArg, int bitId);
	static void spellEffectEndDummyCb( void * ballArg, void * dummy);

	static void spellEffectMusoReimuCb( void * ballArg);

	static void spellEffectEndGungnirCb( void * ballArg, void * dummy);
	void spellEffectEndGungnir();
	static void spellEffectEndTenkoCb( void * ballArg, void * dummy);
	void spellEffectEndTenko();
	static void spellEffectEndMomijiCb( void * ballArg, void *dummy);
	void spellEffectEndMomiji();
	static void spellEffectEndNitoriCb( void * ballArg, void * dummy);
	void spellEffectEndNitori();
	static void spellEffectEndFlanCb( void * ballArg, void * dummy);
	void spellEffectEndFlan();
	static void spellEffectEndOrinCb( void * ballArg, void * dummy);
	void spellEffectEndOrin();
	static void nn::yb::main::CBall::spellEffectOrinCb( void * ballArg, void * stateArg, void *, void * cbArgArg);
	void spellEffectOrin( int state, void (* cb)(void *, void *), void * cbArg);

	static void spellOrinClashCb( void * argBall);
	void spellOrinClash();

	void setSpeed( double argSpeed);
	double getSpeed();
	void setZSpeed( double argZSpeed);
	double getZSpeed();

	bool taskInputBallForm();
	void releaseFlan();
	void releaseGungnir();
	bool isHitOkuBullet( nn::yb::def::HOLE_OBJ * obj);
	bool isHitOku( nn::yb::def::HOLE_OBJ * obj);
	bool isHitUnzan( nn::yb::def::HOLE_OBJ * obj);
	void spellGungnirHitEffect();
	static void spellEffectEndGungnirHitCb( void * ballArg);
	void spellEffectEndGungnirHit();
	void releaseOrin();
	bool isHitWarp1( nn::yb::def::HOLE_OBJ * obj);
	bool isHitWarp2( nn::yb::def::HOLE_OBJ * obj);

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	double x;
	double y;
	int r;
	
	double oldX;
	double oldY;

	double z;

	double angle;

	double power;
	int cupHitState;
	int flySkipCnt;

	bool cupInFlag;

	int cupinDispCnt;

	int powerState;

	int procWaitCnt;

	nn::yb::stage::CHoleCtrl * holeCtrl;

	void (* flyingEndCb)( void *);
	void * flyingEndCbArg;

	double rotHorizon;
	double rotVertical;

	int goodLuckUpCnt;

	TASK * goodLuckAnimeTask;

	unsigned int slopeCnt;

	int flyState;

	bool warpFlag;		// そのワープ板でワープ済みのフラグ、1度板の外に出たらfalse

	bool flyInputProcFlag;		// 飛び中、そのフレームでプレイヤー入力処理したかのフラグ(同フレーム処理しないため)
	bool flyProcFlag;
	bool flyGroundOnFlag;
	nn::common::def::graphics2d::SEGMENT2_D * flyMove;
	bool flyStopFlag;
	bool flyBoundFlag;
	bool flySlopeOnFlag;	// 坂の上にいるかのフラグ
	bool flyWarpOnFlag;		// ワープ板の上にいるかのフラグ 立っていなければwarpFlagが解除出来る
	bool flyConveyorFlag;	// コンベアーの上にいるかのフラグ

	int flyBoundCnt;	// そのショットのバウンド回数をカウント
	int goodLuckUpCntBound;
	int flySlopeStopCnt;		// 坂の上でボールの停止判定にひっかかった回数
	bool flyCupOnFlag;
	int moveStopCnt;
	int flyDashStandbyCnt;
	bool flyFastSeFlag;
	int flyFastSeCnt;

	bool hideFlag;
	bool flyWarpExecFlag;	// 飛び中、そのフレームでワープしたかどうかのフラグ

	int dispState;
	int dispStateReturnCnt;
	
	// 110412
	bool flySkyCupinFlag;
	bool flyCupOnDispDownFlag;

	bool spellCardEnableFlag;
	int flyFrameCnt;
	// とりあえず壁状の柱へのめり込みは解消しない
	bool flyPillarHitFlag;
	bool musoFlag;
	
	bool dispSpellCardEnableFlag;

	int ballForm;
	unsigned int flyBallFormPressFrameCnt;

	void (* spellOrinCb)( void *, void *);
	void * spellOrinCbArg;
	int spellOrinState;
	TASK * spellOrinAnimeTask;

	//unsigned int flyBallFormMechaFrameCnt;

	bool flyFukiageCheckFlag;
	unsigned flyFukiageCnt;

	bool flyJumpCheckFlag;
	bool jumpFlag;

	bool flyJumpStrongCheckFlag;
	bool jumpStrongFlag;

	int flyDashSeWaitCnt;

	bool flyCheckBunkerOnFlag;

	bool flyGravityCheckFlag;
	unsigned flyGravityCnt;
	int flyMspaFrameCnt;

	int flyVampireRemainingFrame;
	bool flyCheckConveyorFlag;
	bool shotTimeOverFlag;
	
private:
	//added by nullpo: スペルカード処理
	void spellCard(nn::yb::e::global::SPELL_CARD kind);
	bool m_bIsGround;//CBallの現在のxy座標が地面上にあるか

	bool bGenkotsuEffect;//現在げんこつエフェクトを出したかどうかのフラグ
	bool bFlareEffect;//現在フレアエフェクトを出したかどうかのフラグ
	bool bBulletHitEffect;//お空の弾によるヒットエフェクトを出したかどうかのフラグ
	
	double speed;
	double zSpeed;

public:
	nn::yb::main::EFFECT_SPELL* m_pCurrSpellEffect;//現在のスペルエフェクト
	nn::yb::effect::CMusoReimu* m_pMusoReimu;
}; // CBall
	 
} // namespece main

} // namespace yb
} // namespace nn
	

#endif // __BALL_H__
