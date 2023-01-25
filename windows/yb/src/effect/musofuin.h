
#ifndef __MUSOFUIN_H__
#define __MUSOFUIN_H__


#include "task.h"
#include "task_save.h"
#include "effect.h"
#include "effect_spell.h"
#include "ball.h"
#include "effect/bit.h"

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace main {
	
//-----------------------------------------------------------------------------

//霊符「夢想封印」
	class EFFECT_MUSOFUIN : public nn::yb::main::EFFECT_SPELL {

public:
	EFFECT_MUSOFUIN();
	EFFECT_MUSOFUIN( ::TASK_SAVE * parent, nn::yb::main::EFFECT_MUSOFUIN * effectMUSOFUINArg);
	virtual ~EFFECT_MUSOFUIN();
	void task();
	void disp();
	static void call_back( void * effectMUSOFUINArg, void * dummy);
	
	//090208えぬに
	int dstN;	//着弾先個数
	int dstLayX[8];	//着弾先x座標
	int dstLayY[8];	//着弾先y座標
	int bitCnt;	//発射した弾カウンタ

	static const int FPS_MUSOFUIN;//FPS
	static const float SCALE_FPS_MUSOFUIN;//FPSによる速度

	int m_iStartX;//発射座標
	int m_iStartY;//発射座標

	
	bool m_bTargetHitEffect;//目標座標に着弾エフェクトを出すかどうかのフラグ

	void  (*spellEffectMusoCb)( void *, int);//ビット着弾時コールバック関数
	nn::yb::main::CBall* ball;//

	TASK_SAVE * bitRoot;
};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __MUSOFUIN_H__