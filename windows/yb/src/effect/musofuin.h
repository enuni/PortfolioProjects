
#ifndef __MUSOFUIN_H__
#define __MUSOFUIN_H__


#include "task.h"
#include "task_save.h"
#include "effect.h"
#include "effect_spell.h"
#include "ball.h"
#include "effect/bit.h"

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace main {
	
//-----------------------------------------------------------------------------

//�아�u���z����v
	class EFFECT_MUSOFUIN : public nn::yb::main::EFFECT_SPELL {

public:
	EFFECT_MUSOFUIN();
	EFFECT_MUSOFUIN( ::TASK_SAVE * parent, nn::yb::main::EFFECT_MUSOFUIN * effectMUSOFUINArg);
	virtual ~EFFECT_MUSOFUIN();
	void task();
	void disp();
	static void call_back( void * effectMUSOFUINArg, void * dummy);
	
	//090208���ʂ�
	int dstN;	//���e���
	int dstLayX[8];	//���e��x���W
	int dstLayY[8];	//���e��y���W
	int bitCnt;	//���˂����e�J�E���^

	static const int FPS_MUSOFUIN;//FPS
	static const float SCALE_FPS_MUSOFUIN;//FPS�ɂ�鑬�x

	int m_iStartX;//���ˍ��W
	int m_iStartY;//���ˍ��W

	
	bool m_bTargetHitEffect;//�ڕW���W�ɒ��e�G�t�F�N�g���o�����ǂ����̃t���O

	void  (*spellEffectMusoCb)( void *, int);//�r�b�g���e���R�[���o�b�N�֐�
	nn::yb::main::CBall* ball;//

	TASK_SAVE * bitRoot;
};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __MUSOFUIN_H__