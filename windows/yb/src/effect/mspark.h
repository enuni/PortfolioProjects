
#ifndef __MSPARK_H__
#define __MSPARK_H__


#include "task.h"
#include "task_save.h"
#include "effect.h"
#include "effect_spell.h"

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace e {
namespace mspark {
	
enum VEC {
	VEC_UP,
	VEC_RIGHT,
	VEC_DOWN,
	VEC_LEFT,

	VEC_MAX
};

}
}


namespace main {
	
class CBall;
//-----------------------------------------------------------------------------

//恋符「マスタースパーク」
	class EFFECT_MSPARK : public nn::yb::main::EFFECT_SPELL {

public:
	EFFECT_MSPARK();
	EFFECT_MSPARK( ::TASK_SAVE * parent, nn::yb::main::EFFECT_MSPARK * effectMSPARKArg);
	virtual ~EFFECT_MSPARK();
	void task();
	void disp();
	static void call_back( void * effectMSPARKArg, void * dummy);
	
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
	int targetX;
	int targetY;
	
	static const int FPS_MSPARK;//FPS
	static const float SCALE_FPS_MSPARK;//FPSによる速度

	int iStarCt;
 
	nn::yb::main::CBall * m_pBall;//CBallへの参照
	
	nn::yb::e::mspark::VEC vec;
	
};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __MSPARK_H__