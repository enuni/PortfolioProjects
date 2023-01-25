
#ifndef __MOMIJI_H__
#define __MOMIJI_H__


#include "task.h"
#include "task_save.h"
#include "effect.h"
#include "effect_spell.h"
#include "bit.h"
#include "gungnir.h"

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace main {
	
class CBall;
//-----------------------------------------------------------------------------

//�����u�g�t��v
	class EFFECT_MOMIJI : public nn::yb::main::EFFECT_SPELL {

public:
	EFFECT_MOMIJI();
	EFFECT_MOMIJI( ::TASK_SAVE * parent, nn::yb::main::EFFECT_MOMIJI * effectMOMIJIArg);
	virtual ~EFFECT_MOMIJI();
	void task();
	void disp();
	static void call_back( void * effectMOMIJIArg, void * dummy);
	
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
	
	static const int FPS_MOMIJI;//FPS
	static const float SCALE_FPS_MOMIJI;//FPS�ɂ�鑬�x

	int iStarCt;
 
	nn::yb::main::CBall * m_pBall;//CBall�ւ̎Q��

	
	D3DXVECTOR3	m_vec3DStartPos;//�ړ��J�n���W
	
	//�ړ����W�Q
	int shadowX2;
	int shadowY2;
	int srcX2;
	int srcY2;
	int vx2;
	int vy2;
	int dx2;
	int dy2;

private:
	//�z�[�~���O��]�p
	nn::yb::main::D3DXVECTOR3 m_vec3DPos;
	nn::yb::main::D3DXVECTOR3 m_vec3DTarget;//�ړ��ڕW���W
	nn::yb::main::D3DXVECTOR3 m_vec3DSpeed;//���x
	nn::yb::main::D3DXVECTOR3 m_vec3DAccel;//�����x
	float m_fTargetAngle;//�ڕW�ւ̊p�x
	float m_fHomingTargetAngle;//�ڕW�ւ̊p�x��1��̃z�[�~���O�����ŕω������鎞�̊p�x
	float m_fTornadoScale;//���݂̗����̃X�P�[��

	EFFECT_SPRITE m_sprAya;//���X�v���C�g

	TASK_SAVE * dummyEffect;

};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __MOMIJI_H__