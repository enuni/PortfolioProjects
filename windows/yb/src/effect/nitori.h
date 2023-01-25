
#ifndef __NITORI_H__
#define __NITORI_H__


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

//�����u���J�������v
	class EFFECT_NITORI : public nn::yb::main::EFFECT_SPELL {

public:
	EFFECT_NITORI();
	EFFECT_NITORI( ::TASK_SAVE * parent, nn::yb::main::EFFECT_NITORI * effectNITORIArg);
	virtual ~EFFECT_NITORI();
	void task();
	void disp();
	static void call_back( void * effectNITORIArg, void * dummy);
	
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
	
	static const int FPS_NITORI;//FPS
	static const float SCALE_FPS_NITORI;//FPS�ɂ�鑬�x

	int iStarCt;
 
	nn::yb::main::CBall * m_pBall;//CBall�ւ̎Q��

	
	D3DXVECTOR3	m_vec3DStartPos;//�ړ��J�n���W

private:
	
	EFFECT_SPRITE m_sprNitori;//�ɂƂ�X�v���C�g
};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __NITORI_H__