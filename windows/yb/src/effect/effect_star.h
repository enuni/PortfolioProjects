
#ifndef __EFFECT_STAR_H__
#define __EFFECT_STAR_H__


#include "task.h"
#include "task_save.h"
#include "effect\\effect.h"

#include "bit.h"

#include <math.h>
#include <queue>

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace main {
struct D3DXVECTOR3;
//���G�t�F�N�g�N���X
class CEffectStar : public ::TASK_SAVE {

public:
	CEffectStar();
	CEffectStar( ::TASK_SAVE * parent, nn::yb::main::CEffectStar * effectArg, int frame, int x, int y);
	virtual ~CEffectStar();
	void task();
	void disp();
	static void call_back( void * effectArg, void * dummy);
	
	::nn::yb::main::CEffectCtrl * ctrl;
	
protected:
	int cnt;
	int frameCnt;

	int totalFrame;

	
	int blend;	//�u�����h���[�h(enum DISP_COMMON_BLEND_MODE�Q��)
	int frame;	//�\������ő�t���[��

protected:
	UINT m_eState;//��ԕϐ�

	UINT m_uiScaleCounter;//�g��k���p�J�E���^�[
	float m_fScaleLimit;

	UINT m_uiAnimeCounter;
	UINT m_uiAnimeCounterLimit;

	
	D3DXVECTOR3 m_vec3DSpeed;//���x
	D3DXVECTOR3 m_vec3DAccel;//�����x


	
	int objIdOffset;	//�\���I�u�W�FID�̃I�t�Z�b�g(�F�̐؂�ւ�)

	static int POS_STARBIT_FROM_X;
	static int POS_STARBIT_FROM_Y;
	static int POS_STARBIT_WIDTH;
	static int POS_STARBIT_HEIGHT;
	
	static float STARBIT_SPEED_X_INIT;
	static float STARBIT_SPEED_Y_INIT ;
	static float STARBIT_ACC_X_INIT ;
	static float STARBIT_ACC_Y_INIT ;

public:
	int objId;	//�\���I�u�W�FID
	
	float w;	//�c
	float h;	//��
	float r;	//�p�x
	float a;	//�s�����x

	D3DXVECTOR3 m_vec3DPos;
	D3DXVECTOR3 m_vec3DScale;
	D3DXVECTOR3 m_vec3DTarget;//�ړ��ڕW���W
	
	int m_iStarNum;//���I�u�W�F�N�g�̌�
	float m_fScale;//���I�u�W�F�N�g��Scale

};

//-----------------------------------------------------------------------------

nn::yb::main::CEffectStar * effect_star_arg( TASK_SAVE * parent, nn::yb::main::CEffectStar * effectAnimArg, int frame, int  x, int y);

//-----------------------------------------------------------------------------
nn::yb::main::CEffectStar * effect_star( TASK_SAVE * parent, int frame, int x, int y, int tx, int ty, int size, int color, int numEffect, float fScale,  nn::yb::main::CEffectCtrl * ctrlArg);
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb

} //namespece nn

#endif __EFFECT_STAR_H__