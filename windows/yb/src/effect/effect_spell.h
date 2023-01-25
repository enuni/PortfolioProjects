
#ifndef __EFFECT_SPELL_H__
#define __EFFECT_SPELL_H__


#include "task.h"
#include "task_save.h"
#include "effect.h"

namespace nn {

namespace yb {

//-----------------------------------------------------------------------------

namespace main {
	
//-----------------------------------------------------------------------------

//スペルカード共通基底クラス
class EFFECT_SPELL : public ::TASK_SAVE {

public:
	EFFECT_SPELL();
	EFFECT_SPELL( ::TASK_SAVE * parent, nn::yb::main::EFFECT_SPELL * effectEffectArg, int frame, int darkenFrame);
	virtual ~EFFECT_SPELL();
	void task();
	void disp();
	static void call_back( void * effectEffectArg, void * dummy);
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	int camera_offset_x;//現在のカメラ位置
	int camera_offset_y;

	void initialize();//状態初期化

	static void cutin_cb( void * effectArg);

protected:
	int cnt;
	int frameCnt;
	int cardCnt;
	int ballCnt;

	
	int totalFrame;//スペルカード持続時間

	TASK * m_taskCutIn;//カットインタスク
	nn::yb::main::EFFECT_ANIM_FADE* m_pBlackEffect;//暗転エフェクト

protected:
	UINT m_eState;//状態変数
	
	UINT m_uiAnimeCounter;
	UINT m_uiAnimeCounterLimit;
};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __EFFECT_SPELL_H__