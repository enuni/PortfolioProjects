
#ifndef __BIT_H__
#define __BIT_H__


#include "task.h"
#include "task_save.h"
#include "effect\\effect.h"
#include "ball.h"

#include <math.h>
#include <queue>

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace main {
	
//-----------------------------------------------------------------------------
struct D3DXVECTOR3
{
	float x;
	float y;
	float z;
public:
	D3DXVECTOR3()
	{
		x = y = z = 0.0f;
	}
	D3DXVECTOR3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	float GetLength()//3�����̋���
	{
		return ::sqrt(x * x + y * y + z * z);
	}
	float Get2DLength()//x, y�݂̂̋���
	{
		return ::sqrt(x * x + y * y);
	}
	void Add (const D3DXVECTOR3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}
	void Sub (const D3DXVECTOR3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}
	D3DXVECTOR3 &D3DXVECTOR3::operator = (const D3DXVECTOR3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}
	D3DXVECTOR3 &D3DXVECTOR3::operator += ( const D3DXVECTOR3& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
};

//�z�[�~���O�r�b�g�N���X
class BIT : public ::TASK_SAVE {

public:
	BIT();
	BIT( ::TASK_SAVE * parent, nn::yb::main::BIT * effectArg, int frame, int x, int y);
	virtual ~BIT();
	void task();
	void disp();
	static void call_back( void * effectArg, void * dummy);
	
	nn::yb::main::CEffectCtrl * ctrl;
	
protected:
	int cnt;
	int frameCnt;

	int totalFrame;

	
	int blend;	//�u�����h���[�h(enum DISP_COMMON_BLEND_MODE�Q��)
	int frame;	//�\������ő�t���[��

protected:
	UINT m_eState;//��ԕϐ�

	float m_fTargetAngle;//�ڕW�ւ̊p�x
	float m_fHomingTargetAngle;//�ڕW�ւ̊p�x��1��̃z�[�~���O�����ŕω������鎞�̊p�x
	UINT m_uiScaleCounter;//�g��k���p�J�E���^�[
	float m_fScaleLimit;

	UINT m_uiAnimeCounter;
	UINT m_uiAnimeCounterLimit;

	
	D3DXVECTOR3 m_vec3DSpeed;//���x
	D3DXVECTOR3 m_vec3DAccel;//�����x

public:
	int objId;	//�\���I�u�W�FID
	
	float w;	//�c
	float h;	//��
	float r;	//�p�x
	float a;	//�s�����x

	std::vector< D3DXVECTOR3 > m_vecAnimPos;
	
	D3DXVECTOR3 m_vec3DPos;
	D3DXVECTOR3 m_vec3DScale;
	D3DXVECTOR3 m_vec3DTarget;//�ړ��ڕW���W

	bool m_bTargetHitEffect;//�ڕW���W�ɒ��e�G�t�F�N�g���o�����ǂ����̃t���O

	int m_iID;//�r�b�g���e���R�[���o�b�N�pID
	
	////void (* eventCallBackFuncStack[256])( void *, void *);
	void  (*spellEffectMusoCb)( void *, int);//�r�b�g���e���R�[���o�b�N�֐�
	nn::yb::main::CBall* ball;//�r�b�g���e���R�[���o�b�N�֐��̈���(CBall)
	
protected:
	void updateHoming();//�z�[�~���O����
};

//-----------------------------------------------------------------------------

nn::yb::main::BIT * effect_bit_arg( TASK_SAVE * parent, nn::yb::main::BIT * effectAnimArg, int frame, int  x, int y);

//-----------------------------------------------------------------------------
nn::yb::main::BIT * effect_bit2( TASK_SAVE * parent, int frame, int x, int y, int tx, int ty, int size, int color, nn::yb::main::CBall* pBall, int ballCnt, void  (*pSpellEffectMusoCb)( void *, int), nn::yb::main::CEffectCtrl * ctrlArg, bool m_bTargetHitEffect);
nn::yb::main::BIT * effect_bit( TASK_SAVE * parent, int frame, int x, int y, int tx, int ty, int size, int color, nn::yb::main::CBall* pBall, int ballCnt, void  (*pSpellEffectMusoCb)( void *, int), nn::yb::main::CEffectCtrl * ctrlArg);
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb

} //namespece nn
#endif __BIT_H__