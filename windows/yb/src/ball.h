// BALL ��u��
// ball ��u��
// Ball ��u��
// yb ��u��
// main ��u��

#ifndef __BALL_H__
#define __BALL_H__

#include "task_save.h"
#include "task.h"
#include "master.h"

// �O���錾
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
	DISP_STATE_DEFAULT,			// �ʏ�
	DISP_STATE_DEAD,			// �C��(����)
	DISP_STATE_NORMAL_FRONT,	// �ʏ�(����)
	DISP_STATE_CRY,				// ����
	DISP_STATE_DOYA,			// �Ƃ�(����)���Ƃ�
	DISP_STATE_JOY,				// �Ƃ�
	DISP_SATTE_DIZZY,			// �ډ�
	DISP_SATTE_DAMAGE,			// �C��
	DISP_STATE_SHOCK,			// �C��(����)���C��
	DISP_STATE_PRESS,			// �C��(����)+���g��+�c�k��
	DISP_STATE_BURN,			// �C��(����)+RGB( 6, 4, 4)
	DISP_SATTE_PERA,			// �ׂ�
	DISP_STATE_MUSO,			// ���̎p
	DISP_STATE_CLASH,			// ���ӃX�y�J�͂˂��

	DISP_STATE_MAX
};

enum BALL_FORM {
	BALL_FORM_DEFAULT,	// �ʏ�
	BALL_FORM_PRESS,	// �y���y��
	BALL_FORM_TORNADO,	// ����
	BALL_FORM_MECHA,	// ���J�������
	BALL_FORM_SPEAR,	// �X�s�A���̂Ԃ���
	BALL_FORM_ORIN,		// ���ӉΎ�
	BALL_FORM_VAMPIRE,	// �z���S
	BALL_FORM_BURN,		// �}�X�^�[�X�p�[�N

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
	bool taskFlyMove();		// �{�[������~������ true ��Ԃ�
	void taskFlyCupin();
	void taskFlyCheckYakumono();	// �S���N���m�𒲂ׂ�flag�𗧂Ă��肵�Ă���

	void taskFlyDash( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyJump( nn::yb::def::HOLE_OBJ * obj);
	
	void taskFlyDamage( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyBunker( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyGravity( nn::yb::def::HOLE_OBJ * obj);

	void calcBoundUnzan( nn::yb::def::HOLE_OBJ * obj);//�_�R�Ƃ̒��˕Ԃ菈��
	void calcBoundOku( nn::yb::def::HOLE_OBJ * obj);//����Ƃ̒��˕Ԃ菈��
	void defeatedOjama( nn::yb::def::HOLE_OBJ * obj);//���ז��L������j��
	void taskFlyUnzan( nn::yb::def::HOLE_OBJ * obj);
	void taskFlyOku( nn::yb::def::HOLE_OBJ * obj); // �����蔻�蓙

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

	bool warpFlag;		// ���̃��[�v�Ń��[�v�ς݂̃t���O�A1�x�̊O�ɏo����false

	bool flyInputProcFlag;		// ��ђ��A���̃t���[���Ńv���C���[���͏����������̃t���O(���t���[���������Ȃ�����)
	bool flyProcFlag;
	bool flyGroundOnFlag;
	nn::common::def::graphics2d::SEGMENT2_D * flyMove;
	bool flyStopFlag;
	bool flyBoundFlag;
	bool flySlopeOnFlag;	// ��̏�ɂ��邩�̃t���O
	bool flyWarpOnFlag;		// ���[�v�̏�ɂ��邩�̃t���O �����Ă��Ȃ����warpFlag�������o����
	bool flyConveyorFlag;	// �R���x�A�[�̏�ɂ��邩�̃t���O

	int flyBoundCnt;	// ���̃V���b�g�̃o�E���h�񐔂��J�E���g
	int goodLuckUpCntBound;
	int flySlopeStopCnt;		// ��̏�Ń{�[���̒�~����ɂЂ�����������
	bool flyCupOnFlag;
	int moveStopCnt;
	int flyDashStandbyCnt;
	bool flyFastSeFlag;
	int flyFastSeCnt;

	bool hideFlag;
	bool flyWarpExecFlag;	// ��ђ��A���̃t���[���Ń��[�v�������ǂ����̃t���O

	int dispState;
	int dispStateReturnCnt;
	
	// 110412
	bool flySkyCupinFlag;
	bool flyCupOnDispDownFlag;

	bool spellCardEnableFlag;
	int flyFrameCnt;
	// �Ƃ肠�����Ǐ�̒��ւ̂߂荞�݂͉������Ȃ�
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
	//added by nullpo: �X�y���J�[�h����
	void spellCard(nn::yb::e::global::SPELL_CARD kind);
	bool m_bIsGround;//CBall�̌��݂�xy���W���n�ʏ�ɂ��邩

	bool bGenkotsuEffect;//���݂��񂱂G�t�F�N�g���o�������ǂ����̃t���O
	bool bFlareEffect;//���݃t���A�G�t�F�N�g���o�������ǂ����̃t���O
	bool bBulletHitEffect;//����̒e�ɂ��q�b�g�G�t�F�N�g���o�������ǂ����̃t���O
	
	double speed;
	double zSpeed;

public:
	nn::yb::main::EFFECT_SPELL* m_pCurrSpellEffect;//���݂̃X�y���G�t�F�N�g
	nn::yb::effect::CMusoReimu* m_pMusoReimu;
}; // CBall
	 
} // namespece main

} // namespace yb
} // namespace nn
	

#endif // __BALL_H__
