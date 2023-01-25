
#ifndef __TENKO_H__
#define __TENKO_H__


#include "task.h"
#include "task_save.h"
#include "effect.h"
#include "effect_spell.h"
#include "anime.h"

#include "gungnir.h"

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace main {
	
class CBall;
//-----------------------------------------------------------------------------

//�v�΁u�V�n�J蓃v���X�v
	class EFFECT_TENKO : public nn::yb::main::EFFECT_SPELL {

public:
	EFFECT_TENKO();
	EFFECT_TENKO( ::TASK_SAVE * parent, nn::yb::main::EFFECT_TENKO * effectTENKOArg);
	virtual ~EFFECT_TENKO();
	void task();
	void disp();
	static void call_back( void * effectTENKOArg, void * dummy);
	static void anime_cb( void * effectTENKOArg);
	

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
	
	static const int FPS_TENKO;//FPS
	static const float SCALE_FPS_TENKO;//FPS�ɂ�鑬�x

	int iStarCt;
 
	nn::yb::main::CBall * m_pBall;//CBall�ւ̎Q��

	
	D3DXVECTOR3	m_vec3DStartPos;//�ړ��J�n���W

private:
	EFFECT_SPRITE m_sprKaname;//�v��(�㔼��)

	EFFECT_SPRITE m_sprKanameLower;//�v��(������)
	
	EFFECT_SPRITE m_sprTenko;//�V�q�X�v���C�g

	
	nn::yb::main::D3DXVECTOR3 m_vec3DPos;

	nn::common::dxlib::CAnime * m_taskTenkoAnime;
};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __TENKO_H__