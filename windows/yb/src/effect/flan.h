
#ifndef __FLAN_H__
#define __FLAN_H__


#include "task.h"
#include "task_save.h"
#include "effect.h"
#include "effect_spell.h"

#include "gungnir.h"

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace main {
	
class CBall;
//-----------------------------------------------------------------------------

//
	class EFFECT_FLAN : public nn::yb::main::EFFECT_SPELL {

public:
	EFFECT_FLAN();
	EFFECT_FLAN( ::TASK_SAVE * parent, nn::yb::main::EFFECT_FLAN * effectFLANArg);
	virtual ~EFFECT_FLAN();
	void task();
	void disp();
	static void call_back( void * effectFLANArg, void * dummy);
	
	int shadowX;
	int shadowY;
	int srcX;
	int srcY;
	int vx;
	int vy;
	int dx;
	int dy;
	int diff;
	int shadowSpeed;
	int z;
	int angle;
	
	static const int FPS_FLAN;//FPS
	static const float SCALE_FPS_FLAN;//FPSによる速度
	static const int YUKKURI_2_FLAN_FRAME;

	int iStarCt;
 
	nn::yb::main::CBall * m_pBall;//CBallへの参照

	
	D3DXVECTOR3	m_vec3DStartPos;//移動開始座標

private:
	
	EFFECT_SPRITE m_sprFlan;//フランスプライト
	EFFECT_SPRITE m_sprFlanShadow;//フランシルエットスプライト
};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __FLAN_H__