
#ifndef __EFFECT_SPELL_H__
#define __EFFECT_SPELL_H__


#include "task.h"
#include "task_save.h"
#include "effect.h"

namespace nn {

namespace yb {

//-----------------------------------------------------------------------------

namespace main {
	
//-----------------------------------------------------------------------------

//�X�y���J�[�h���ʊ��N���X
class EFFECT_SPELL : public ::TASK_SAVE {

public:
	EFFECT_SPELL();
	EFFECT_SPELL( ::TASK_SAVE * parent, nn::yb::main::EFFECT_SPELL * effectEffectArg, int frame, int darkenFrame);
	virtual ~EFFECT_SPELL();
	void task();
	void disp();
	static void call_back( void * effectEffectArg, void * dummy);
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	int camera_offset_x;//���݂̃J�����ʒu
	int camera_offset_y;

	void initialize();//��ԏ�����

	static void cutin_cb( void * effectArg);

protected:
	int cnt;
	int frameCnt;
	int cardCnt;
	int ballCnt;

	
	int totalFrame;//�X�y���J�[�h��������

	TASK * m_taskCutIn;//�J�b�g�C���^�X�N
	nn::yb::main::EFFECT_ANIM_FADE* m_pBlackEffect;//�Ó]�G�t�F�N�g

protected:
	UINT m_eState;//��ԕϐ�
	
	UINT m_uiAnimeCounter;
	UINT m_uiAnimeCounterLimit;
};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __EFFECT_SPELL_H__