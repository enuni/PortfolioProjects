
#ifndef __ORIN_H__
#define __ORIN_H__


#include "task.h"
#include "task_save.h"
#include "effect.h"
#include "effect_spell.h"

#include "master.h"

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace e {
namespace orin {

enum VEC {
	VEC_UP,
	VEC_RIGHT,
	VEC_DOWN,
	VEC_LEFT,

	VEC_MAX
};

enum STATE {
	STATE_DARK_OUT,
	STATE_CUTIN,
	STATE_DASH,
	STATE_SLOW,
	STATE_BRAKE,
	STATE_CLASH,
	STATE_STOP1,
	STATE_STOP2,
	STATE_SHOCK_BEFORE,
	STATE_SHOCK,
	STATE_SHOCK_KICK1,
	STATE_SHOCK_KICK2,
	STATE_SHOCK_KICK3,
	STATE_SHOCK_KICK4,

	STATE_MAX,
};

enum CB_STATE {
	CB_STATE_CLASH,
	CB_STATE_KICK,
	CB_STATE_SKIP,
	CB_STATE_END,

	CB_STATE_MAX
};

}
}

namespace main {

class EFFECT_ORIN : public nn::yb::main::EFFECT_SPELL {

public:
	EFFECT_ORIN();
	EFFECT_ORIN( ::TASK_SAVE * parent, nn::yb::main::EFFECT_ORIN * effectORINArg);
	virtual ~EFFECT_ORIN();
	void task();
	void disp();

	static void recvEventCb( void * effectORINArg, void * stateArg);
	void recvEvent( nn::yb::e::orin::CB_STATE state);
	
	int dstX;
	int dstY;
	int srcX;
	int srcY;
	int vx;
	int vy;
	int dx;
	int dy;
	int diff;
	int speed;
	int z;
	int dstX2;
	int dstY2;

	MASTER * master;
	
	static const int FPS_ORIN;//FPS
	static const float SCALE_FPS_ORIN;//FPS‚É‚æ‚é‘¬“x

	nn::yb::e::orin::VEC vec;

	void (* sendEventCb)( void * master, void * state, void * cbcb, void * cbcbArg);
	void * sendEventCbArg;
	
	nn::yb::e::orin::VEC vec2;
	
#if 0
	// •K—v‚É‚È‚Á‚½‚ç
	static void cutinEndCb( void * orinArg);
	void cutinEnd();
#endif

private:

};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __ORIN_H__