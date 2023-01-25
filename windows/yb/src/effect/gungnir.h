
#ifndef __GUNGNIR_H__
#define __GUNGNIR_H__


#include "task.h"
#include "task_save.h"
#include "effect.h"
#include "effect_spell.h"
#include "bit.h"
#include "class_list.h"

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace main {

struct D3DXVECTOR3;

//�X�v���C�g�N���X
class EFFECT_SPRITE 
{
public:
	EFFECT_SPRITE();
	virtual ~EFFECT_SPRITE();

	void update();
	void disp();
	void dispMain();

public:

	D3DXVECTOR3	m_vec3DPos;	
	D3DXVECTOR3	m_vec3DSize;		//���A����

	D3DXVECTOR3	m_vec3DRotation;
	D3DXVECTOR3	m_vec3DScale;

	int m_iFileTexture;	//�e�N�X�`����ObjID

	int m_iAlpha;//�A���t�@�l

	int m_iBlendMode;//Blend mode

};


class CBall;
//-----------------------------------------------------------------------------

//�_���u�X�s�A�E�U�E�O���O�j���v
	class EFFECT_GUNGNIR : public nn::yb::main::EFFECT_SPELL {

public:
	EFFECT_GUNGNIR();
	EFFECT_GUNGNIR( ::TASK_SAVE * parent, nn::yb::main::EFFECT_GUNGNIR * effectGUNGNIRArg);
	virtual ~EFFECT_GUNGNIR();
	void task();
	void disp();
	static void call_back( void * effectGUNGNIRArg, void * dummy);
	static void anime_cb( void * effectGUNGNIRArg);
	


public:
	D3DXVECTOR3	m_vec3DTargetPos;//�^�[�Q�b�g�̍��W	
	D3DXVECTOR3	m_vec3DWavePos;//���n�Ռ��g�̍��W	

	bool m_bIsGround;//�n�ʂɒ��n�ł��邩�ǂ���
	float m_fGroundZ;//���n��Z���W(�n�ʂ�����ꍇ�͂O�A�����ꍇ�͗���)

 
	nn::yb::main::CBall * m_pBall;//CBall�ւ̎Q��
	::TASK_SAVE * m_taskBigbulletParent;

protected:
	////int cnt;
	////int frameCnt;

	int frame;	//�\������ő�t���[��


	

protected:
	EFFECT_SPRITE m_sprRemiria;
	EFFECT_SPRITE m_sprGungnir;
	////EFFECT_SPRITE m_sprMagicCircle;
	////EFFECT_SPRITE m_sprLightPillar;

};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __GUNGNIR_H__