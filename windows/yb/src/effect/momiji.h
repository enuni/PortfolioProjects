
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

//旋符「紅葉扇風」
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
	static const float SCALE_FPS_MOMIJI;//FPSによる速度

	int iStarCt;
 
	nn::yb::main::CBall * m_pBall;//CBallへの参照

	
	D3DXVECTOR3	m_vec3DStartPos;//移動開始座標
	
	//移動座標２
	int shadowX2;
	int shadowY2;
	int srcX2;
	int srcY2;
	int vx2;
	int vy2;
	int dx2;
	int dy2;

private:
	//ホーミング回転用
	nn::yb::main::D3DXVECTOR3 m_vec3DPos;
	nn::yb::main::D3DXVECTOR3 m_vec3DTarget;//移動目標座標
	nn::yb::main::D3DXVECTOR3 m_vec3DSpeed;//速度
	nn::yb::main::D3DXVECTOR3 m_vec3DAccel;//加速度
	float m_fTargetAngle;//目標への角度
	float m_fHomingTargetAngle;//目標への角度を1回のホーミング処理で変化させる時の角度
	float m_fTornadoScale;//現在の竜巻のスケール

	EFFECT_SPRITE m_sprAya;//文スプライト

	TASK_SAVE * dummyEffect;

};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __MOMIJI_H__